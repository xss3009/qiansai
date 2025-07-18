#include "rtconfig.h"
#include <lvgl.h>
#include "lv_conf.h"
#include "gui_guider.h"
#include "events_init.h"
#include <rtthread.h>
#include "lv_file_explorer.h"
#include <ff.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "use_common.h"

/* 声明 LVGL 硬件初始化函数 */
void lv_port_disp_init(void);
void lv_port_indev_init(void);

lv_ui lv_gui_guiser; // UI 全局变量

/* 文件浏览器相关变量 */
static lv_obj_t *file_explorer;
static char current_path[256] = "/";

/* 全局变量 - 在接收线程中更新 */
volatile uint8_t v2_data1 = 0;
volatile uint8_t v2_data2 = 0;
volatile uint8_t v2_data3 = 0;
volatile char v2_timestamp[26] = {0}; // 初始化为空
volatile uint8_t new_data_available = 0; // 新数据标志

/* 文件加载上下文结构 - 添加分页支持 */
typedef struct file_load_context {
    char path[256];
    lv_obj_t *label;
    lv_obj_t *container;
    lv_obj_t *modal;
    lv_obj_t *page_label; // 显示页码的标签
    uint32_t file_size;
    uint32_t cur_page;
    uint32_t max_page;
    uint32_t page_size;
    rt_mutex_t mutex;      // 互斥锁保护上下文
    volatile uint8_t valid; // 上下文有效性标志
} file_load_context_t;

/* 用于异步UI更新的结构体 */
typedef struct ui_update_data {
    file_load_context_t *ctx;
    char *page_buf;
    const char *message;
} ui_update_data_t;

/* 全局互斥锁保护文件操作 */
static rt_mutex_t file_mutex = RT_NULL;

/* 初始化互斥锁 */
static void init_mutex(void)
{
    if (file_mutex == RT_NULL) {
        file_mutex = rt_mutex_create("file_mtx", RT_IPC_FLAG_PRIO);
    }
}

/* 异步UI更新回调函数 */
static void ui_update_callback(void *data)
{
    ui_update_data_t *update = (ui_update_data_t *)data;
    file_load_context_t *ctx = update->ctx;

    if (ctx && ctx->valid) {
        if (update->message) {
            lv_label_set_text(ctx->label, update->message);
        }
        else if (update->page_buf) {
            lv_label_set_text(ctx->label, update->page_buf);

            // 更新页码显示
            if (ctx->page_label) {
                lv_label_set_text_fmt(ctx->page_label, "Page: %d/%d",
                                      ctx->cur_page + 1,
                                      ctx->max_page);
            }
        }

        // 清理模态窗口并显示内容
        if (ctx->modal) {
            lv_obj_del(ctx->modal);
            ctx->modal = NULL;
        }
        lv_obj_clear_flag(ctx->container, LV_OBJ_FLAG_HIDDEN);
    }

    // 释放资源
    if (update->page_buf) {
        rt_free(update->page_buf);
    }
    rt_free(update);
}

/* 页面加载线程入口函数 */
static void page_loader_thread_entry(void *param)
{
    file_load_context_t *ctx = (file_load_context_t *)param;
    FIL file;
    FRESULT res;
    char *page_buf = RT_NULL;
    ui_update_data_t *update = RT_NULL;

    // 检查上下文有效性
    if (ctx == NULL || !ctx->valid) {
        rt_kprintf("Page loader: invalid context\n");
        return;
    }

    // 锁定上下文
    if (rt_mutex_take(ctx->mutex, RT_WAITING_FOREVER) != RT_EOK) {
        rt_kprintf("Failed to take context mutex\n");
        return;
    }

    // 锁定文件系统
    if (rt_mutex_take(file_mutex, RT_WAITING_FOREVER) != RT_EOK) {
        rt_mutex_release(ctx->mutex);
        return;
    }

    res = f_open(&file, ctx->path, FA_READ);
    if (res != FR_OK) {
        rt_kprintf("Page load error: %d\n", res);

        // 创建UI更新数据
        update = rt_malloc(sizeof(ui_update_data_t));
        if (update) {
            update->ctx = ctx;
            update->page_buf = RT_NULL;
            update->message = "Error opening file!";
            lv_async_call(ui_update_callback, update);
        }
        goto cleanup;
    }

    // 分配页面缓冲区
    page_buf = rt_malloc(ctx->page_size + 1);
    if (!page_buf) {
        f_close(&file);

        // 创建UI更新数据
        update = rt_malloc(sizeof(ui_update_data_t));
        if (update) {
            update->ctx = ctx;
            update->page_buf = RT_NULL;
            update->message = "Memory error!";
            lv_async_call(ui_update_callback, update);
        }
        goto cleanup;
    }

    // 计算读取位置
    uint32_t offset = ctx->cur_page * ctx->page_size;
    if (offset > ctx->file_size) {
        offset = 0;
    }

    // 设置读取位置
    f_lseek(&file, offset);

    // 读取页面数据
    UINT bytes_read;
    uint32_t to_read = ctx->page_size;
    if (offset + to_read > ctx->file_size) {
        to_read = ctx->file_size - offset;
    }

    res = f_read(&file, page_buf, to_read, &bytes_read);
    f_close(&file);

    if (res == FR_OK && bytes_read > 0) {
        // 处理非文本字符
        for (uint32_t i = 0; i < bytes_read; i++) {
            if (page_buf[i] == '\0') page_buf[i] = ' ';
            else if (!isprint((unsigned char)page_buf[i]) && !isspace((unsigned char)page_buf[i])) {
                page_buf[i] = '.';
            }
        }
        page_buf[bytes_read] = '\0';

        // 创建UI更新数据
        update = rt_malloc(sizeof(ui_update_data_t));
        if (update) {
            update->ctx = ctx;
            update->page_buf = page_buf; // 传递缓冲区所有权
            update->message = RT_NULL;
            lv_async_call(ui_update_callback, update);
            page_buf = NULL; // 所有权已转移
        }
    } else {
        // 创建UI更新数据
        update = rt_malloc(sizeof(ui_update_data_t));
        if (update) {
            update->ctx = ctx;
            update->page_buf = RT_NULL;
            update->message = "Read error!";
            lv_async_call(ui_update_callback, update);
        }
    }

cleanup:
    // 释放资源
    if (page_buf) {
        rt_free(page_buf);
    }

    // 释放锁
    rt_mutex_release(file_mutex);
    rt_mutex_release(ctx->mutex);
}

/* 文件加载线程入口函数 - 只读取文件元信息 */
static void file_loader_thread_entry(void *param)
{
    file_load_context_t *ctx = (file_load_context_t *)param;
    FIL file;
    FRESULT res;
    ui_update_data_t *update = RT_NULL;

    // 检查上下文有效性
    if (ctx == NULL || !ctx->valid) {
        rt_kprintf("File loader: invalid context\n");
        return;
    }

    // 锁定上下文
    if (rt_mutex_take(ctx->mutex, RT_WAITING_FOREVER) != RT_EOK) {
        return;
    }

    // 锁定文件系统
    if (rt_mutex_take(file_mutex, RT_WAITING_FOREVER) != RT_EOK) {
        rt_mutex_release(ctx->mutex);
        return;
    }

    res = f_open(&file, ctx->path, FA_READ);
    if (res != FR_OK) {
        rt_kprintf("File open error: %d\n", res);

        // 创建UI更新数据
        update = rt_malloc(sizeof(ui_update_data_t));
        if (update) {
            update->ctx = ctx;
            update->page_buf = RT_NULL;
            update->message = "Error opening file!";
            lv_async_call(ui_update_callback, update);
        }
        goto cleanup;
    }

    // 获取文件大小
    ctx->file_size = f_size(&file);
    rt_kprintf("File size: %u bytes\n", ctx->file_size);
    f_close(&file);

    // 设置分页参数
    ctx->page_size = 1024; // 每页1024字符
    ctx->max_page = (ctx->file_size + ctx->page_size - 1) / ctx->page_size;
    ctx->cur_page = 0;

    // 加载第一页
    rt_thread_t loader = rt_thread_create("page_loader", page_loader_thread_entry, ctx,
                                         2048,  // 栈大小
                                         8,     // 优先级
                                         10);   // 时间片

    if (loader) {
        rt_thread_startup(loader);
    } else {
        // 创建UI更新数据
        update = rt_malloc(sizeof(ui_update_data_t));
        if (update) {
            update->ctx = ctx;
            update->page_buf = RT_NULL;
            update->message = "Page loader failed!";
            lv_async_call(ui_update_callback, update);
        }
    }

cleanup:
    // 释放锁
    rt_mutex_release(file_mutex);
    rt_mutex_release(ctx->mutex);
}

/* 翻页按钮事件处理 */
static void prev_page_btn_handler(lv_event_t *e)
{
    file_load_context_t *ctx = lv_event_get_user_data(e);

    // 检查上下文有效性
    if (ctx == NULL || !ctx->valid) {
        rt_kprintf("Prev page: invalid context\n");
        return;
    }

    if (ctx->cur_page > 0) {
        ctx->cur_page--;

        // 显示加载指示器
        lv_obj_clear_flag(ctx->modal, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ctx->container, LV_OBJ_FLAG_HIDDEN);

        rt_thread_t loader = rt_thread_create("page_loader", page_loader_thread_entry, ctx,
                                             2048,  // 栈大小
                                             8,     // 优先级
                                             10);   // 时间片
        if (loader) {
            rt_thread_startup(loader);
        } else {
            // 恢复显示
            lv_obj_clear_flag(ctx->container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ctx->modal, LV_OBJ_FLAG_HIDDEN);
            lv_label_set_text(ctx->label, "Thread create failed!");
        }
    }
}

static void next_page_btn_handler(lv_event_t *e)
{
    file_load_context_t *ctx = lv_event_get_user_data(e);

    // 检查上下文有效性
    if (ctx == NULL || !ctx->valid) {
        rt_kprintf("Next page: invalid context\n");
        return;
    }

    if (ctx->cur_page < ctx->max_page - 1) {
        ctx->cur_page++;

        // 显示加载指示器
        lv_obj_clear_flag(ctx->modal, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ctx->container, LV_OBJ_FLAG_HIDDEN);

        rt_thread_t loader = rt_thread_create("page_loader", page_loader_thread_entry, ctx,
                                             2048,  // 栈大小
                                             8,     // 优先级
                                             10);   // 时间片
        if (loader) {
            rt_thread_startup(loader);
        } else {
            // 恢复显示
            lv_obj_clear_flag(ctx->container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ctx->modal, LV_OBJ_FLAG_HIDDEN);
            lv_label_set_text(ctx->label, "Thread create failed!");
        }
    }
}

/* 按钮事件处理函数 */
static void back_btn_event_handler(lv_event_t *e)
{
    lv_obj_t *container = lv_obj_get_parent(lv_obj_get_parent(e->current_target));
    lv_obj_del(container);
}

static void refresh_btn_event_handler(lv_event_t *e)
{
    lv_file_explorer_open_dir(file_explorer, current_path);
}

static void save_btn_event_handler(lv_event_t *e)
{
    // 使用 v2_data1 作为文件名
    char filename[16];
    snprintf(filename, sizeof(filename), "0x%02X.txt", v2_data1);

    // 锁定文件系统
    if (rt_mutex_take(file_mutex, RT_WAITING_FOREVER) != RT_EOK) {
        return;
    }

    FIL fp;
    FRESULT res = f_open(&fp, filename, FA_OPEN_APPEND | FA_WRITE);

    if (res != FR_OK) {
        // 如果文件不存在，尝试创建
        res = f_open(&fp, filename, FA_CREATE_NEW | FA_WRITE);
        if (res != FR_OK) {
            rt_kprintf("File creation failed! Error: %d\n", res);
            rt_mutex_release(file_mutex);
            return;
        }
    }

    // 写入数据
    UINT bytes_written;
    char buffer[128];

    // 写入时间戳和数据
    int len = snprintf(buffer, sizeof(buffer), "[%s] Data: 0x%02X 0x%02X 0x%02X\n",
                      v2_timestamp, v2_data1, v2_data2, v2_data3);

    f_write(&fp, buffer, len, &bytes_written);
    f_close(&fp);

    // 释放文件系统锁
    rt_mutex_release(file_mutex);

    rt_kprintf("Data saved to: %s\n", filename);
}

/* 自动保存函数 - 由接收线程调用 */
void save_data_to_file(void)
{
    // 使用 v2_data1 作为文件名
    char filename[16];
    snprintf(filename, sizeof(filename), "0x%02X.txt", v2_data1);

    // 锁定文件系统
    if (rt_mutex_take(file_mutex, RT_WAITING_FOREVER) != RT_EOK) {
        return;
    }

    FIL fp;
    FRESULT res = f_open(&fp, filename, FA_OPEN_APPEND | FA_WRITE);

    if (res != FR_OK) {
        // 如果文件不存在，尝试创建
        res = f_open(&fp, filename, FA_CREATE_NEW | FA_WRITE);
        if (res != FR_OK) {
            rt_kprintf("Auto-save failed! Error: %d\n", res);
            rt_mutex_release(file_mutex);
            return;
        }
    }

    // 写入数据
    UINT bytes_written;
    char buffer[128];

    // 写入时间戳和数据
    int len = snprintf(buffer, sizeof(buffer), "[%s] Data: 0x%02X 0x%02X 0x%02X\n",
                      v2_timestamp, v2_data1, v2_data2, v2_data3);

    f_write(&fp, buffer, len, &bytes_written);
    f_close(&fp);

    // 释放文件系统锁
    rt_mutex_release(file_mutex);

    rt_kprintf("Data auto-saved to: %s\n", filename);
}

static void close_event_cb(lv_event_t *e)
{
    lv_obj_t *container = lv_event_get_user_data(e);
    file_load_context_t *ctx = lv_obj_get_user_data(container);

    if (ctx) {
        // 标记上下文无效
        ctx->valid = 0;

        // 等待任何正在进行的操作完成
        rt_mutex_take(ctx->mutex, RT_WAITING_FOREVER);

        // 释放资源
        rt_mutex_delete(ctx->mutex);
        rt_free(ctx);
    }

    lv_obj_del(container);
}

/* 文件浏览器事件回调 - 修改为分页显示 */
static void file_explorer_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED) {
        const char *filename = lv_file_explorer_get_selected_file_name(obj);
        const char *path = lv_file_explorer_get_current_path(obj);

        if (filename && path) {
            char full_path[256];

            // 创建模态背景（防止重复点击）
            lv_obj_t *modal = lv_obj_create(lv_scr_act());
            lv_obj_set_size(modal, LV_PCT(100), LV_PCT(100));
            lv_obj_set_style_bg_opa(modal, LV_OPA_50, 0);
            lv_obj_set_style_bg_color(modal, lv_color_black(), 0);
            lv_obj_set_style_border_width(modal, 0, 0);
            lv_obj_add_flag(modal, LV_OBJ_FLAG_CLICKABLE);

            // 创建加载指示器
            lv_obj_t *spinner = lv_spinner_create(modal, 1000, 60);
            lv_obj_set_size(spinner, 100, 100);
            lv_obj_center(spinner);
            lv_obj_set_style_arc_color(spinner, lv_color_white(), LV_PART_MAIN);
            lv_obj_set_style_arc_color(spinner, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);

            // 创建加载标签
            lv_obj_t *loading_label = lv_label_create(modal);
            lv_label_set_text(loading_label, "Loading file...");
            lv_obj_align_to(loading_label, spinner, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
            lv_obj_set_style_text_color(loading_label, lv_color_white(), 0);

            // 强制立即显示加载界面
            lv_task_handler();
            rt_thread_mdelay(50);

            // 确保路径格式正确
            if (path[strlen(path) - 1] == '/') {
                snprintf(full_path, sizeof(full_path), "%s%s", path, filename);
            } else {
                snprintf(full_path, sizeof(full_path), "%s/%s", path, filename);
            }

            // 替换反斜杠
            for (char *p = full_path; *p; p++) {
                if (*p == '\\') *p = '/';
            }

            rt_kprintf("Opening file: %s\n", full_path);

            /* 创建文本显示容器 */
            lv_obj_t *container = lv_obj_create(lv_scr_act());
            lv_obj_set_size(container, LV_PCT(90), LV_PCT(80));
            lv_obj_center(container);
            lv_obj_set_style_bg_color(container, lv_color_white(), 0);
            lv_obj_set_style_border_width(container, 2, 0);
            lv_obj_set_style_border_color(container, lv_palette_main(LV_PALETTE_BLUE), 0);
            lv_obj_set_style_radius(container, 10, 0);
            lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);  // 先隐藏

            // 创建标题标签
            lv_obj_t *title = lv_label_create(container);
            char title_buf[100];
            snprintf(title_buf, sizeof(title_buf), "File: %s", filename);
            lv_label_set_text(title, title_buf);
            lv_obj_align(title, LV_ALIGN_TOP_LEFT, 10, 10);

            // 创建文本区域 - 使用LABEL
            lv_obj_t *content_label = lv_label_create(container);
            lv_obj_set_size(content_label, LV_PCT(95), LV_PCT(70)); // 为按钮留空间
            lv_obj_align_to(content_label, title, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
            lv_label_set_text(content_label, "Loading...");
            lv_label_set_long_mode(content_label, LV_LABEL_LONG_WRAP);
            lv_obj_set_style_text_font(content_label, &lv_font_montserrat_12, 0);
            lv_obj_set_style_text_color(content_label, lv_color_black(), 0);
            lv_obj_set_style_bg_opa(content_label, LV_OPA_0, 0);

            // 创建翻页按钮容器
            lv_obj_t *btn_container = lv_obj_create(container);
            lv_obj_remove_style_all(btn_container);
            lv_obj_set_size(btn_container, LV_PCT(95), 40);
            lv_obj_align_to(btn_container, content_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

            // 创建页码标签
            lv_obj_t *page_label = lv_label_create(btn_container);
            lv_label_set_text(page_label, "Page: 0/0");
            lv_obj_align(page_label, LV_ALIGN_CENTER, 0, 0);

            // 创建翻页按钮
            lv_obj_t *prev_btn = lv_btn_create(btn_container);
            lv_obj_set_size(prev_btn, 40, 30);
            lv_obj_align(prev_btn, LV_ALIGN_LEFT_MID, 0, 0);
            lv_obj_t *prev_label = lv_label_create(prev_btn);
            lv_label_set_text(prev_label, LV_SYMBOL_LEFT);
            lv_obj_center(prev_label);

            lv_obj_t *next_btn = lv_btn_create(btn_container);
            lv_obj_set_size(next_btn, 40, 30);
            lv_obj_align(next_btn, LV_ALIGN_RIGHT_MID, 0, 0);
            lv_obj_t *next_label = lv_label_create(next_btn);
            lv_label_set_text(next_label, LV_SYMBOL_RIGHT);
            lv_obj_center(next_label);

            // 创建关闭按钮
            lv_obj_t *close_btn = lv_btn_create(container);
            lv_obj_set_size(close_btn, 100, 40);
            lv_obj_align(close_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
            lv_obj_t *close_label = lv_label_create(close_btn);
            lv_label_set_text(close_label, "Close");
            lv_obj_center(close_label);
            lv_obj_add_event_cb(close_btn, close_event_cb, LV_EVENT_CLICKED, container);

            /* 在后台线程中读取文件信息 */
            file_load_context_t *ctx = rt_malloc(sizeof(file_load_context_t));
            if (!ctx) {
                lv_label_set_text(content_label, "Memory error!");
                lv_obj_del(modal);
                lv_obj_clear_flag(container, LV_OBJ_FLAG_HIDDEN);
                return;
            }

            // 初始化上下文
            memset(ctx, 0, sizeof(file_load_context_t));
            strncpy(ctx->path, full_path, sizeof(ctx->path) - 1);
            ctx->path[sizeof(ctx->path) - 1] = '\0';
            ctx->label = content_label;
            ctx->container = container;
            ctx->modal = modal;
            ctx->page_label = page_label;
            ctx->file_size = 0;
            ctx->cur_page = 0;
            ctx->max_page = 0;
            ctx->page_size = 1024; // 每页1024字节
            ctx->valid = 1; // 标记为有效

            // 创建互斥锁
            ctx->mutex = rt_mutex_create("ctx_mtx", RT_IPC_FLAG_PRIO);
            if (!ctx->mutex) {
                rt_free(ctx);
                lv_label_set_text(content_label, "Mutex create failed!");
                lv_obj_del(modal);
                lv_obj_clear_flag(container, LV_OBJ_FLAG_HIDDEN);
                return;
            }

            // 设置容器用户数据
            lv_obj_set_user_data(container, ctx);

            // 设置按钮事件
            lv_obj_add_event_cb(prev_btn, prev_page_btn_handler, LV_EVENT_CLICKED, ctx);
            lv_obj_add_event_cb(next_btn, next_page_btn_handler, LV_EVENT_CLICKED, ctx);

            // 创建文件加载线程 - 提高优先级
            rt_thread_t load_thread = rt_thread_create("file_loader",
                file_loader_thread_entry,
                ctx,
                4096,  // 增加栈大小
                10,    // 提高优先级
                10);

            if (load_thread) {
                rt_thread_startup(load_thread);
            } else {
                lv_label_set_text(content_label, "Thread creation failed!");
                lv_obj_del(modal);
                lv_obj_clear_flag(container, LV_OBJ_FLAG_HIDDEN);
                rt_mutex_delete(ctx->mutex);
                rt_free(ctx);
            }
        }
    }
}

/* 创建文件浏览器 */
static void create_file_explorer(lv_obj_t *parent)
{
    file_explorer = lv_file_explorer_create(parent);
    lv_obj_set_size(file_explorer, LV_PCT(95), LV_PCT(80));
    lv_obj_align(file_explorer, LV_ALIGN_CENTER, 0, 0);
    lv_file_explorer_set_sort(file_explorer, 1);
    lv_obj_add_event_cb(file_explorer, file_explorer_event_cb, LV_EVENT_ALL, NULL);

    char lvgl_path[32];
    snprintf(lvgl_path, sizeof(lvgl_path), "%c:/", LV_FS_FATFS_LETTER);
    lv_file_explorer_open_dir(file_explorer, lvgl_path);

    lv_obj_t *btn_container = lv_obj_create(parent);
    lv_obj_remove_style_all(btn_container);
    lv_obj_set_size(btn_container, LV_PCT(95), 50);
    lv_obj_align_to(btn_container, file_explorer, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    // 返回按钮
    lv_obj_t *back_btn = lv_btn_create(btn_container);
    lv_obj_set_size(back_btn, 100, 40);
    lv_obj_align(back_btn, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_t *back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, "Back");
    lv_obj_center(back_label);
    lv_obj_add_event_cb(back_btn, back_btn_event_handler, LV_EVENT_CLICKED, NULL);

    // 刷新按钮
    lv_obj_t *refresh_btn = lv_btn_create(btn_container);
    lv_obj_set_size(refresh_btn, 100, 40);
    lv_obj_align(refresh_btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_t *refresh_label = lv_label_create(refresh_btn);
    lv_label_set_text(refresh_label, "Refresh");
    lv_obj_center(refresh_label);
    lv_obj_add_event_cb(refresh_btn, refresh_btn_event_handler, LV_EVENT_CLICKED, NULL);

    // 保存数据按钮
    lv_obj_t *save_btn = lv_btn_create(btn_container);
    lv_obj_set_size(save_btn, 100, 40);
    lv_obj_align(save_btn, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_t *save_label = lv_label_create(save_btn);
    lv_label_set_text(save_label, "Save Data");
    lv_obj_center(save_label);
    lv_obj_add_event_cb(save_btn, save_btn_event_handler, LV_EVENT_CLICKED, NULL);
}

/* 显示文件浏览器的按钮事件回调 */
static void show_file_explorer_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        lv_obj_t *container = lv_obj_create(lv_scr_act());
        lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
        lv_obj_set_style_bg_color(container, lv_color_white(), 0);
        create_file_explorer(container);
    }
}

/* 修改为匹配示例的线程定义 */
#ifdef rt_align
rt_align(RT_ALIGN_SIZE)
#else
ALIGN(RT_ALIGN_SIZE)
#endif
static rt_uint8_t lvgl_thread_stack[8192];

static struct rt_thread lvgl_thread;

void lv_user_gui_init(void)
{
    // 初始化互斥锁
    init_mutex();

    setup_ui(&lv_gui_guiser);
    events_init(&lv_gui_guiser);

    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, 150, 50);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, -20, -20);
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "File Explorer");
    lv_obj_center(label);
    lv_obj_add_event_cb(btn, show_file_explorer_cb, LV_EVENT_CLICKED, NULL);
}

/* LVGL 线程入口函数 */
static void lvgl_thread_entry(void *parameter)
{
    // 初始化互斥锁
    init_mutex();

    extern void lv_fs_fatfs_init(void);
    lv_fs_fatfs_init();

    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    snprintf(current_path, sizeof(current_path), "%c:/", LV_FS_FATFS_LETTER);
    rt_kprintf("File explorer initial path: %s\n", current_path);

    lv_user_gui_init();

    /* 测试LVGL文件系统 */
    lv_fs_file_t test_file;
    char test_path[32];
    snprintf(test_path, sizeof(test_path), "%c:/test.txt", LV_FS_FATFS_LETTER);

    lv_fs_res_t res = lv_fs_open(&test_file, test_path, LV_FS_MODE_WR);
    if (res == LV_FS_RES_OK) {
        rt_kprintf("LVGL filesystem test: write OK\n");
        const char *data = "LVGL FS test\n";
        uint32_t written = 0;
        lv_fs_write(&test_file, data, strlen(data), &written);
        lv_fs_close(&test_file);
    } else {
        rt_kprintf("LVGL filesystem test failed: %d\n", res);
    }

    /* LVGL 主循环 */
    while(1)
    {
        lv_task_handler();

        // 检查是否有新数据需要保存
        if (new_data_available) {
            save_data_to_file();
            new_data_available = 0; // 重置标志
        }

        rt_thread_mdelay(5);
    }
}

/* 修改为匹配示例的初始化方式 - 更改函数名避免冲突 */
int my_lvgl_thread_init(void)
{
    rt_err_t err;

#ifdef BSP_USING_TOUCH
    extern rt_err_t rt_hw_gt911_register(void);
    if (rt_hw_gt911_register() != RT_EOK) {
        rt_kprintf("Failed to register GT911 touch device!\n");
    } else {
        rt_kprintf("GT911 touch device registered successfully\n");
    }
#endif

    err = rt_thread_init(&lvgl_thread,
                         "LVGL",
                         lvgl_thread_entry,
                         RT_NULL,
                         &lvgl_thread_stack[0],
                         sizeof(lvgl_thread_stack),
                         15,  // 提高LVGL线程优先级
                         20);

    if(err != RT_EOK)
    {
        rt_kprintf("Failed to create LVGL thread\n");
        return -RT_ERROR;
    }

    rt_thread_startup(&lvgl_thread);
    rt_kprintf("[LVGL] Thread started successfully!\n");
    return RT_EOK;
}

INIT_DEVICE_EXPORT(my_lvgl_thread_init);
