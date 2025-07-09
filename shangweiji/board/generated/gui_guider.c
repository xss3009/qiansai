/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "widgets_init.h"
#include "use_common.h"


#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "gg_external_data.h"
#endif



// 全局定时器指针
static lv_timer_t *chart_update_timer = NULL;

// 图表更新回调函数
static void chart_update_cb(lv_timer_t *timer)
{
    static uint32_t last_chart_update = 0;  // 上次图表更新时间
       const uint32_t chart_update_interval = 60000; // 60秒更新间隔(ms)

       lv_ui *ui = (lv_ui *)timer->user_data;
       uint32_t current_time = lv_tick_get();

        //lv_ui *ui = (lv_ui *)timer->user_data;
        char buf1[32];
        snprintf(buf1, sizeof(buf1), "%d", device_id);
        char buf11[32];
                snprintf(buf11, sizeof(buf11), "%d", 1);
        char buf2[32];
        snprintf(buf2, sizeof(buf2), "%d", temperature);
        char buf3[32];
        snprintf(buf3, sizeof(buf3), "%d", sensor1_value);
        uint16_t combined_value = ((uint16_t)sensor3_value << 8) | sensor2_value;
        char buf4[32];
        snprintf(buf4, sizeof(buf4), "%d", combined_value);
        // 更新标签文本
        lv_label_set_text(ui->some_c33_3, buf1);
        lv_label_set_text(ui->some_c33_6, buf11);
        lv_label_set_text(ui->some_la_1, buf2);
        lv_label_set_text(ui->some_la_2, buf3);
        lv_label_set_text(ui->some_la_3, buf4);


        if (current_time - last_chart_update >= chart_update_interval) {
                // 更新图表数据
                lv_chart_set_next_value(ui->some_chart_1, ui->some_chart_1_0, temperature);
                lv_chart_refresh(ui->some_chart_1);

                // 更新最后更新时间戳
                last_chart_update = current_time;

                rt_kprintf("图表数据更新(60秒间隔): 温度=%d\n", temperature);
            }


    if (status_flag == 1)
    {
        //lv_ui *ui = (lv_ui *)lv_event_get_user_data(e);
        lv_obj_add_flag(ui->some_c_331, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui->some_c_332, LV_OBJ_FLAG_HIDDEN);

    }

    if (status_flag == 0)
        {
            //lv_ui *ui = (lv_ui *)lv_event_get_user_data(e);
            lv_obj_add_flag(ui->some_c_332, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui->some_c_331, LV_OBJ_FLAG_HIDDEN);

        }

}

// 在UI初始化后启动定时器
void start_chart_animation(lv_ui *ui)
{
    // 如果定时器已存在，先删除
    if (chart_update_timer) {
        lv_timer_del(chart_update_timer);
        chart_update_timer = NULL;
    }

    // 创建新定时器（每500毫秒更新一次）
    chart_update_timer = lv_timer_create(chart_update_cb, 500, ui);
    lv_timer_set_repeat_count(chart_update_timer, -1); // 无限循环
}








void ui_init_style(lv_style_t * style)
{
    if (style->prop_cnt > 1)
        lv_style_reset(style);
    else
        lv_style_init(style);
}

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del)
{
    lv_obj_t * act_scr = lv_scr_act();

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
    if(auto_del) {
        gg_edata_task_clear(act_scr);
    }
#endif
    if (auto_del && is_clean) {
        lv_obj_clean(act_scr);
    }
    if (new_scr_del) {
        setup_scr(ui);
    }
    lv_scr_load_anim(*new_scr, anim_type, time, delay, auto_del);
    *old_scr_del = auto_del;
}

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb)
{
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, var);
    lv_anim_set_exec_cb(&anim, exec_cb);
    lv_anim_set_values(&anim, start_value, end_value);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_delay(&anim, delay);
    lv_anim_set_path_cb(&anim, path_cb);
    lv_anim_set_repeat_count(&anim, repeat_cnt);
    lv_anim_set_repeat_delay(&anim, repeat_delay);
    lv_anim_set_playback_time(&anim, playback_time);
    lv_anim_set_playback_delay(&anim, playback_delay);
    if (start_cb) {
        lv_anim_set_start_cb(&anim, start_cb);
    }
    if (ready_cb) {
        lv_anim_set_ready_cb(&anim, ready_cb);
    }
    if (deleted_cb) {
        lv_anim_set_deleted_cb(&anim, deleted_cb);
    }
    lv_anim_start(&anim);
}

void init_scr_del_flag(lv_ui *ui)
{

    ui->some_del = true;
}

void setup_ui(lv_ui *ui)
{
    init_scr_del_flag(ui);
    init_keyboard(ui);
    setup_scr_some(ui);
    lv_scr_load(ui->some);
    start_chart_animation(ui);
}

void init_keyboard(lv_ui *ui)
{

}
