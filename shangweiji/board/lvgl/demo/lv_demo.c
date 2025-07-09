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

/* 声明 LVGL 硬件初始化函数 */
void lv_port_disp_init(void);
void lv_port_indev_init(void);

lv_ui lv_gui_guiser; // UI 全局变量

/* 文件浏览器相关变量 */
static lv_obj_t *file_explorer;
static char current_path[256] = "/";
static char selected_file[256];

/* 全局变量 - 在接收线程中更新 */
volatile uint8_t v2_data1 = 0;
volatile uint8_t v2_data2 = 0;
volatile uint8_t v2_data3 = 0;
volatile char v2_timestamp[26] = {0}; // 初始化为空
volatile uint8_t new_data_available = 0; // 新数据标志

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

    FIL fp;
    FRESULT res = f_open(&fp, filename, FA_OPEN_APPEND | FA_WRITE);

    if (res != FR_OK) {
        // 如果文件不存在，尝试创建
        res = f_open(&fp, filename, FA_CREATE_NEW | FA_WRITE);
        if (res != FR_OK) {
            rt_kprintf("File creation failed! Error: %d\n", res);
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

    rt_kprintf("Data saved to: %s\n", filename);
}

/* 自动保存函数 - 由接收线程调用 */
void save_data_to_file(void)
{
    // 使用 v2_data1 作为文件名
    char filename[16];
    snprintf(filename, sizeof(filename), "0x%02X.txt", v2_data1);

    FIL fp;
    FRESULT res = f_open(&fp, filename, FA_OPEN_APPEND | FA_WRITE);

    if (res != FR_OK) {
        // 如果文件不存在，尝试创建
        res = f_open(&fp, filename, FA_CREATE_NEW | FA_WRITE);
        if (res != FR_OK) {
            rt_kprintf("Auto-save failed! Error: %d\n", res);
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

    rt_kprintf("Data auto-saved to: %s\n", filename);
}

/* 文件浏览器事件回调 */
static void file_explorer_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED) {
        const char *filename = lv_file_explorer_get_selected_file_name(obj);
        if (filename) {
            rt_kprintf("Item selected: %s\n", filename);
        }
    }
}

/* 创建文件浏览器 */
static void create_file_explorer(lv_obj_t *parent)
{
    lv_fs_fatfs_set_filter(true);
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
                         28,
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
