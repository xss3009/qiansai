/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *some;
	bool some_del;
	lv_obj_t *some_c_1;
	lv_obj_t *some_img_1;
	lv_obj_t *some_bS_1;
	lv_obj_t *some_bS_1_label;
	lv_obj_t *some_chart_1;
	lv_chart_series_t *some_chart_1_0;
	lv_obj_t *some_label_10;
	lv_obj_t *some_label_11;
	lv_obj_t *some_label_1;
	lv_obj_t *some_la_1;
	lv_obj_t *some_la_2;
	lv_obj_t *some_la_3;
	lv_obj_t *some_label_12;
	lv_obj_t *some_label_13;
	lv_obj_t *some_label_14;
	lv_obj_t *some_c_3;
	lv_obj_t *some_img_9;
	lv_obj_t *some_c_332;
	lv_obj_t *some_img_10;
	lv_obj_t *some_spangroup_6;
	lv_span_t *some_spangroup_6_span;
	lv_obj_t *some_spangroup_5;
	lv_span_t *some_spangroup_5_span;
	lv_obj_t *some_spangroup_4;
	lv_span_t *some_spangroup_4_span;
	lv_obj_t *some_c33_5;
	lv_obj_t *some_c33_6;
	lv_obj_t *some_c33_7;
	lv_obj_t *some_c_331;
	lv_obj_t *some_img_11;
	lv_obj_t *some_c33_1;
	lv_obj_t *some_spangroup_2;
	lv_span_t *some_spangroup_2_span;
	lv_obj_t *some_spangroup_1;
	lv_span_t *some_spangroup_1_span;
	lv_obj_t *some_spangroup_3;
	lv_span_t *some_spangroup_3_span;
	lv_obj_t *some_c33_2;
	lv_obj_t *some_c33_3;
	lv_obj_t *some_bs_2;
	lv_obj_t *some_bs_2_label;
	lv_obj_t *some_c_2;
	lv_obj_t *some_img_4;
	lv_obj_t *some_btn_3;
	lv_obj_t *some_btn_3_label;
	lv_obj_t *some_btn_4;
	lv_obj_t *some_btn_4_label;
	lv_obj_t *some_btn_5;
	lv_obj_t *some_btn_5_label;
	lv_obj_t *some_btn_6;
	lv_obj_t *some_btn_6_label;
	lv_obj_t *some_img_8;
	lv_obj_t *some_img_12;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_some(lv_ui *ui);
LV_IMG_DECLARE(_some_alpha_490x286);
LV_IMG_DECLARE(_Warehouse_alpha_489x286);
LV_IMG_DECLARE(_qinghuo_alpha_100x100);
LV_IMG_DECLARE(_milcarr_alpha_100x100);
LV_IMG_DECLARE(_pape_alpha_494x282);
LV_IMG_DECLARE(_huozai4_alpha_100x100);
LV_IMG_DECLARE(_huozai4_alpha_100x100);

LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_16)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_12)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_22)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_24)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_19)


#ifdef __cplusplus
}
#endif
#endif
