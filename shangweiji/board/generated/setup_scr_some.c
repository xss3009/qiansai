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
#include "events_init.h"
#include "widgets_init.h"
//#include "custom.h"
#include "lvgl.h"

static lv_anim_t img8_anim;  // 定义动画对象

/* 添加动画执行函数 */
static void set_img_zoom(void* img, int32_t zoom) {
    lv_img_set_zoom((lv_obj_t*)img, zoom);
}

/* 动画启动接口函数 */
void start_img8_animation(lv_ui* ui) {
    lv_anim_start(&img8_anim);
}

/* 动画停止接口函数 */
void stop_img8_animation(lv_ui* ui) {
    lv_anim_del(ui->some_img_8, set_img_zoom);
    lv_img_set_zoom(ui->some_img_8, 256);  // 恢复原始大小
}


void setup_scr_some(lv_ui *ui)
{
    //Write codes some
    ui->some = lv_obj_create(NULL);
    lv_obj_set_size(ui->some, 480, 272);
    lv_obj_set_scrollbar_mode(ui->some, LV_SCROLLBAR_MODE_OFF);

    //Write style for some, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->some, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_c_1
    ui->some_c_1 = lv_obj_create(ui->some);
    lv_obj_set_pos(ui->some_c_1, -8, -8);
    lv_obj_set_size(ui->some_c_1, 486, 281);
    lv_obj_set_scrollbar_mode(ui->some_c_1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->some_c_1, LV_OBJ_FLAG_HIDDEN);

    //Write style for some_c_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_c_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->some_c_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->some_c_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->some_c_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_c_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_c_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->some_c_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->some_c_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_c_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_c_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_c_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_c_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_c_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_img_13
    ui->some_img_13 = lv_img_create(ui->some_c_1);
    lv_obj_add_flag(ui->some_img_13, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->some_img_13, &_Warehouse_alpha_488x282);
    lv_img_set_pivot(ui->some_img_13, 50,50);
    lv_img_set_angle(ui->some_img_13, 0);
    lv_obj_set_pos(ui->some_img_13, -2, -2);
    lv_obj_set_size(ui->some_img_13, 488, 282);

    //Write style for some_img_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->some_img_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->some_img_13, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_img_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->some_img_13, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_bS_1
    ui->some_bS_1 = lv_btn_create(ui->some_c_1);
    ui->some_bS_1_label = lv_label_create(ui->some_bS_1);
    lv_label_set_text(ui->some_bS_1_label, "");
    lv_label_set_long_mode(ui->some_bS_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->some_bS_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->some_bS_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->some_bS_1_label, LV_PCT(100));
    lv_obj_set_pos(ui->some_bS_1, 395, 4);
    lv_obj_set_size(ui->some_bS_1, 85, 50);

    //Write style for some_bS_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->some_bS_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->some_bS_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->some_bS_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->some_bS_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_bS_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_bS_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_bS_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_bS_1, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_bS_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_bS_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_label_10
    ui->some_label_10 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_label_10, "当前温度：");
    lv_label_set_long_mode(ui->some_label_10, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_label_10, 276, 62);
    lv_obj_set_size(ui->some_label_10, 121, 31);

    //Write style for some_label_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_label_10, lv_color_hex(0xa4bd38), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_label_10, &lv_font_SourceHanSerifSC_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_label_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_label_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_label_11
    ui->some_label_11 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_label_11, "当前湿度：");
    lv_label_set_long_mode(ui->some_label_11, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_label_11, 276, 125);
    lv_obj_set_size(ui->some_label_11, 119, 30);

    //Write style for some_label_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_label_11, lv_color_hex(0xa4bd38), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_label_11, &lv_font_SourceHanSerifSC_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_label_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_label_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_label_1
    ui->some_label_1 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_label_1, "当前光度：");
    lv_label_set_long_mode(ui->some_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_label_1, 276, 186);
    lv_obj_set_size(ui->some_label_1, 119, 30);

    //Write style for some_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_label_1, lv_color_hex(0xa4bd38), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_label_1, &lv_font_SourceHanSerifSC_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_la_1
    ui->some_la_1 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_la_1, "0\n");
    lv_label_set_long_mode(ui->some_la_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_la_1, 366, 57);
    lv_obj_set_size(ui->some_la_1, 105, 29);

    //Write style for some_la_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_la_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_la_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_la_1, lv_color_hex(0x3bbd38), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_la_1, &lv_font_SourceHanSerifSC_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_la_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_la_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_la_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_la_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_la_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_la_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_la_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_la_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_la_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_la_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_la_2
    ui->some_la_2 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_la_2, "0\n");
    lv_label_set_long_mode(ui->some_la_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_la_2, 366, 120);
    lv_obj_set_size(ui->some_la_2, 105, 29);

    //Write style for some_la_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_la_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_la_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_la_2, lv_color_hex(0x3bbd38), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_la_2, &lv_font_SourceHanSerifSC_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_la_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_la_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_la_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_la_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_la_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_la_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_la_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_la_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_la_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_la_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_la_3
    ui->some_la_3 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_la_3, "0\n");
    lv_label_set_long_mode(ui->some_la_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_la_3, 366, 182);
    lv_obj_set_size(ui->some_la_3, 105, 29);

    //Write style for some_la_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_la_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_la_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_la_3, lv_color_hex(0x3bbd38), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_la_3, &lv_font_SourceHanSerifSC_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_la_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_la_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_la_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_la_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_la_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_la_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_la_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_la_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_la_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_la_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_label_12
    ui->some_label_12 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_label_12, "℃");
    lv_label_set_long_mode(ui->some_label_12, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_label_12, 403, 57);
    lv_obj_set_size(ui->some_label_12, 100, 32);

    //Write style for some_label_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_label_12, lv_color_hex(0x2FDA64), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_label_12, &lv_font_SourceHanSerifSC_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_label_12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_label_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_label_13
    ui->some_label_13 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_label_13, "lx");
    lv_label_set_long_mode(ui->some_label_13, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_label_13, 402, 183);
    lv_obj_set_size(ui->some_label_13, 100, 32);

    //Write style for some_label_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_label_13, lv_color_hex(0x2FDA64), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_label_13, &lv_font_SourceHanSerifSC_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_label_13, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_label_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_label_14
    ui->some_label_14 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_label_14, "%RH");
    lv_label_set_long_mode(ui->some_label_14, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_label_14, 433, 118);
    lv_obj_set_size(ui->some_label_14, 42, 22);

    //Write style for some_label_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_label_14, lv_color_hex(0x2FDA64), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_label_14, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_label_14, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_label_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_label_15
    ui->some_label_15 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_label_15, "传感器在线数：");
    lv_label_set_long_mode(ui->some_label_15, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_label_15, 12, 62);
    lv_obj_set_size(ui->some_label_15, 177, 27);

    //Write style for some_label_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_label_15, lv_color_hex(0xa4bd38), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_label_15, &lv_font_SourceHanSerifSC_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_label_15, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_label_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_label_16
    ui->some_label_16 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_label_16, "传感器总数：");
    lv_label_set_long_mode(ui->some_label_16, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_label_16, 18, 125);
    lv_obj_set_size(ui->some_label_16, 144, 30);

    //Write style for some_label_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_label_16, lv_color_hex(0xa4bd38), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_label_16, &lv_font_SourceHanSerifSC_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_label_16, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_label_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_label_17
    ui->some_label_17 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_label_17, "PM2.5：");
    lv_label_set_long_mode(ui->some_label_17, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_label_17, 9, 186);
    lv_obj_set_size(ui->some_label_17, 121, 31);

    //Write style for some_label_17, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_label_17, lv_color_hex(0xa4bd38), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_label_17, &lv_font_SourceHanSerifSC_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_label_17, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_label_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_label_18
    ui->some_label_18 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_label_18, "mg/m³");
    lv_label_set_long_mode(ui->some_label_18, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_label_18, 175, 186);
    lv_obj_set_size(ui->some_label_18, 65, 23);

    //Write style for some_label_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_label_18, lv_color_hex(0x2FDA64), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_label_18, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_label_18, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_label_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_la_4
    ui->some_la_4 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_la_4, "0\n");
    lv_label_set_long_mode(ui->some_la_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_la_4, 166, 60);
    lv_obj_set_size(ui->some_la_4, 105, 29);

    //Write style for some_la_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_la_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_la_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_la_4, lv_color_hex(0x3bbd38), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_la_4, &lv_font_SourceHanSerifSC_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_la_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_la_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_la_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_la_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_la_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_la_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_la_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_la_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_la_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_la_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_la_5
    ui->some_la_5 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_la_5, "0\n");
    lv_label_set_long_mode(ui->some_la_5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_la_5, 88, 184);
    lv_obj_set_size(ui->some_la_5, 105, 29);

    //Write style for some_la_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_la_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_la_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_la_5, lv_color_hex(0x3bbd38), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_la_5, &lv_font_SourceHanSerifSC_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_la_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_la_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_la_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_la_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_la_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_la_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_la_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_la_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_la_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_la_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_label_19
    ui->some_label_19 = lv_label_create(ui->some_c_1);
    lv_label_set_text(ui->some_label_19, "24\n");
    lv_label_set_long_mode(ui->some_label_19, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_label_19, 136, 121);
    lv_obj_set_size(ui->some_label_19, 105, 29);

    //Write style for some_label_19, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_label_19, lv_color_hex(0x3bbd38), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_label_19, &lv_font_SourceHanSerifSC_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_label_19, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_label_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_c_3
    ui->some_c_3 = lv_obj_create(ui->some);
    lv_obj_set_pos(ui->some_c_3, -3, -2);
    lv_obj_set_size(ui->some_c_3, 482, 275);
    lv_obj_set_scrollbar_mode(ui->some_c_3, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->some_c_3, LV_OBJ_FLAG_HIDDEN);

    //Write style for some_c_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_c_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->some_c_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->some_c_3, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->some_c_3, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_c_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_c_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->some_c_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->some_c_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_c_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_c_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_c_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_c_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_c_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_img_9
    ui->some_img_9 = lv_img_create(ui->some_c_3);
    lv_obj_add_flag(ui->some_img_9, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->some_img_9, &_Warehouse_alpha_489x286);
    lv_img_set_pivot(ui->some_img_9, 50,50);
    lv_img_set_angle(ui->some_img_9, 0);
    lv_obj_set_pos(ui->some_img_9, -7, -8);
    lv_obj_set_size(ui->some_img_9, 489, 286);

    //Write style for some_img_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->some_img_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->some_img_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_img_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->some_img_9, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_c_332
    ui->some_c_332 = lv_obj_create(ui->some_c_3);
    lv_obj_set_pos(ui->some_c_332, -2, 1);
    lv_obj_set_size(ui->some_c_332, 483, 196);
    lv_obj_set_scrollbar_mode(ui->some_c_332, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->some_c_332, LV_OBJ_FLAG_HIDDEN);

    //Write style for some_c_332, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_c_332, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->some_c_332, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->some_c_332, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->some_c_332, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_c_332, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_c_332, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_c_332, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_c_332, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_c_332, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_c_332, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_c_332, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_img_10
    ui->some_img_10 = lv_img_create(ui->some_c_332);
    lv_obj_add_flag(ui->some_img_10, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->some_img_10, &_qinghuo_alpha_100x100);
    lv_img_set_pivot(ui->some_img_10, 50,50);
    lv_img_set_angle(ui->some_img_10, 0);
    lv_obj_set_pos(ui->some_img_10, 12, 39);
    lv_obj_set_size(ui->some_img_10, 100, 100);

    //Write style for some_img_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->some_img_10, 171, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->some_img_10, lv_color_hex(0x938810), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->some_img_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_img_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->some_img_10, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_spangroup_6
    ui->some_spangroup_6 = lv_spangroup_create(ui->some_c_332);
    lv_spangroup_set_align(ui->some_spangroup_6, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->some_spangroup_6, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->some_spangroup_6, LV_SPAN_MODE_FIXED);
    //create span
    ui->some_spangroup_6_span = lv_spangroup_new_span(ui->some_spangroup_6);
    lv_span_set_text(ui->some_spangroup_6_span, "货物状态:");
    lv_style_set_text_color(&ui->some_spangroup_6_span->style, lv_color_hex(0xddff00));
    lv_style_set_text_decor(&ui->some_spangroup_6_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->some_spangroup_6_span->style, &lv_font_SourceHanSerifSC_Regular_24);
    lv_obj_set_pos(ui->some_spangroup_6, 118, 46);
    lv_obj_set_size(ui->some_spangroup_6, 110, 30);

    //Write style state: LV_STATE_DEFAULT for &style_some_spangroup_6_main_main_default
    static lv_style_t style_some_spangroup_6_main_main_default;
    ui_init_style(&style_some_spangroup_6_main_main_default);

    lv_style_set_border_width(&style_some_spangroup_6_main_main_default, 0);
    lv_style_set_radius(&style_some_spangroup_6_main_main_default, 0);
    lv_style_set_bg_opa(&style_some_spangroup_6_main_main_default, 0);
    lv_style_set_pad_top(&style_some_spangroup_6_main_main_default, 0);
    lv_style_set_pad_right(&style_some_spangroup_6_main_main_default, 0);
    lv_style_set_pad_bottom(&style_some_spangroup_6_main_main_default, 0);
    lv_style_set_pad_left(&style_some_spangroup_6_main_main_default, 0);
    lv_style_set_shadow_width(&style_some_spangroup_6_main_main_default, 0);
    lv_obj_add_style(ui->some_spangroup_6, &style_some_spangroup_6_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->some_spangroup_6);

    //Write codes some_spangroup_5
    ui->some_spangroup_5 = lv_spangroup_create(ui->some_c_332);
    lv_spangroup_set_align(ui->some_spangroup_5, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->some_spangroup_5, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->some_spangroup_5, LV_SPAN_MODE_BREAK);
    //create span
    ui->some_spangroup_5_span = lv_spangroup_new_span(ui->some_spangroup_5);
    lv_span_set_text(ui->some_spangroup_5_span, "货物坐标误差：");
    lv_style_set_text_color(&ui->some_spangroup_5_span->style, lv_color_hex(0xddff00));
    lv_style_set_text_decor(&ui->some_spangroup_5_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->some_spangroup_5_span->style, &lv_font_SourceHanSerifSC_Regular_24);
    lv_obj_set_pos(ui->some_spangroup_5, 118, 76);
    lv_obj_set_size(ui->some_spangroup_5, 170, 27);

    //Write style state: LV_STATE_DEFAULT for &style_some_spangroup_5_main_main_default
    static lv_style_t style_some_spangroup_5_main_main_default;
    ui_init_style(&style_some_spangroup_5_main_main_default);

    lv_style_set_border_width(&style_some_spangroup_5_main_main_default, 0);
    lv_style_set_radius(&style_some_spangroup_5_main_main_default, 0);
    lv_style_set_bg_opa(&style_some_spangroup_5_main_main_default, 0);
    lv_style_set_pad_top(&style_some_spangroup_5_main_main_default, 0);
    lv_style_set_pad_right(&style_some_spangroup_5_main_main_default, 0);
    lv_style_set_pad_bottom(&style_some_spangroup_5_main_main_default, 0);
    lv_style_set_pad_left(&style_some_spangroup_5_main_main_default, 0);
    lv_style_set_shadow_width(&style_some_spangroup_5_main_main_default, 0);
    lv_obj_add_style(ui->some_spangroup_5, &style_some_spangroup_5_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->some_spangroup_5);

    //Write codes some_spangroup_4
    ui->some_spangroup_4 = lv_spangroup_create(ui->some_c_332);
    lv_spangroup_set_align(ui->some_spangroup_4, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->some_spangroup_4, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->some_spangroup_4, LV_SPAN_MODE_BREAK);
    //create span
    ui->some_spangroup_4_span = lv_spangroup_new_span(ui->some_spangroup_4);
    lv_span_set_text(ui->some_spangroup_4_span, "装配货物id：");
    lv_style_set_text_color(&ui->some_spangroup_4_span->style, lv_color_hex(0xddff00));
    lv_style_set_text_decor(&ui->some_spangroup_4_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->some_spangroup_4_span->style, &lv_font_SourceHanSerifSC_Regular_24);
    lv_obj_set_pos(ui->some_spangroup_4, 118, 113);
    lv_obj_set_size(ui->some_spangroup_4, 170, 27);

    //Write style state: LV_STATE_DEFAULT for &style_some_spangroup_4_main_main_default
    static lv_style_t style_some_spangroup_4_main_main_default;
    ui_init_style(&style_some_spangroup_4_main_main_default);

    lv_style_set_border_width(&style_some_spangroup_4_main_main_default, 0);
    lv_style_set_radius(&style_some_spangroup_4_main_main_default, 0);
    lv_style_set_bg_opa(&style_some_spangroup_4_main_main_default, 0);
    lv_style_set_pad_top(&style_some_spangroup_4_main_main_default, 0);
    lv_style_set_pad_right(&style_some_spangroup_4_main_main_default, 0);
    lv_style_set_pad_bottom(&style_some_spangroup_4_main_main_default, 0);
    lv_style_set_pad_left(&style_some_spangroup_4_main_main_default, 0);
    lv_style_set_shadow_width(&style_some_spangroup_4_main_main_default, 0);
    lv_obj_add_style(ui->some_spangroup_4, &style_some_spangroup_4_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->some_spangroup_4);

    //Write codes some_c33_5
    ui->some_c33_5 = lv_label_create(ui->some_c_332);
    lv_label_set_text(ui->some_c33_5, "无");
    lv_label_set_long_mode(ui->some_c33_5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_c33_5, 272, 76);
    lv_obj_set_size(ui->some_c33_5, 72, 27);

    //Write style for some_c33_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_c33_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_c33_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_c33_5, lv_color_hex(0x00ff4e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_c33_5, &lv_font_SourceHanSerifSC_Regular_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_c33_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_c33_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_c33_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_c33_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_c33_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_c33_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_c33_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_c33_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_c33_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_c33_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_c33_6
    ui->some_c33_6 = lv_label_create(ui->some_c_332);
    lv_label_set_text(ui->some_c33_6, "0");
    lv_label_set_long_mode(ui->some_c33_6, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_c33_6, 257, 113);
    lv_obj_set_size(ui->some_c33_6, 100, 32);

    //Write style for some_c33_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_c33_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_c33_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_c33_6, lv_color_hex(0xffbf00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_c33_6, &lv_font_SourceHanSerifSC_Regular_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_c33_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_c33_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_c33_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_c33_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_c33_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_c33_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_c33_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_c33_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_c33_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_c33_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_c33_7
    ui->some_c33_7 = lv_label_create(ui->some_c_332);
    lv_label_set_text(ui->some_c33_7, "装配完成");
    lv_label_set_long_mode(ui->some_c33_7, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_c33_7, 225, 48);
    lv_obj_set_size(ui->some_c33_7, 100, 32);

    //Write style for some_c33_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_c33_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_c33_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_c33_7, lv_color_hex(0x3eff00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_c33_7, &lv_font_SourceHanSerifSC_Regular_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_c33_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_c33_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_c33_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_c33_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_c33_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_c33_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_c33_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_c33_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_c33_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_c33_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_c_331
    ui->some_c_331 = lv_obj_create(ui->some_c_3);
    lv_obj_set_pos(ui->some_c_331, -2, 1);
    lv_obj_set_size(ui->some_c_331, 483, 196);
    lv_obj_set_scrollbar_mode(ui->some_c_331, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->some_c_331, LV_OBJ_FLAG_HIDDEN);

    //Write style for some_c_331, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_c_331, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->some_c_331, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->some_c_331, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->some_c_331, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_c_331, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_c_331, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_c_331, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_c_331, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_c_331, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_c_331, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_c_331, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_img_11
    ui->some_img_11 = lv_img_create(ui->some_c_331);
    lv_obj_add_flag(ui->some_img_11, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->some_img_11, &_milcarr_alpha_100x100);
    lv_img_set_pivot(ui->some_img_11, 50,50);
    lv_img_set_angle(ui->some_img_11, 0);
    lv_obj_set_pos(ui->some_img_11, 12, 35);
    lv_obj_set_size(ui->some_img_11, 100, 100);

    //Write style for some_img_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->some_img_11, 83, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->some_img_11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->some_img_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_img_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->some_img_11, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_c33_1
    ui->some_c33_1 = lv_label_create(ui->some_c_331);
    lv_label_set_text(ui->some_c33_1, "配送中");
    lv_label_set_long_mode(ui->some_c33_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_c33_1, 223, 46);
    lv_obj_set_size(ui->some_c33_1, 100, 32);

    //Write style for some_c33_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_c33_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_c33_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_c33_1, lv_color_hex(0xff9600), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_c33_1, &lv_font_SourceHanSerifSC_Regular_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_c33_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_c33_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_c33_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_c33_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_c33_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_c33_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_c33_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_c33_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_c33_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_c33_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_spangroup_2
    ui->some_spangroup_2 = lv_spangroup_create(ui->some_c_331);
    lv_spangroup_set_align(ui->some_spangroup_2, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->some_spangroup_2, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->some_spangroup_2, LV_SPAN_MODE_FIXED);
    //create span
    ui->some_spangroup_2_span = lv_spangroup_new_span(ui->some_spangroup_2);
    lv_span_set_text(ui->some_spangroup_2_span, "小车状态:");
    lv_style_set_text_color(&ui->some_spangroup_2_span->style, lv_color_hex(0xddff00));
    lv_style_set_text_decor(&ui->some_spangroup_2_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->some_spangroup_2_span->style, &lv_font_SourceHanSerifSC_Regular_24);
    lv_obj_set_pos(ui->some_spangroup_2, 118, 46);
    lv_obj_set_size(ui->some_spangroup_2, 110, 30);

    //Write style state: LV_STATE_DEFAULT for &style_some_spangroup_2_main_main_default
    static lv_style_t style_some_spangroup_2_main_main_default;
    ui_init_style(&style_some_spangroup_2_main_main_default);

    lv_style_set_border_width(&style_some_spangroup_2_main_main_default, 0);
    lv_style_set_radius(&style_some_spangroup_2_main_main_default, 0);
    lv_style_set_bg_opa(&style_some_spangroup_2_main_main_default, 0);
    lv_style_set_pad_top(&style_some_spangroup_2_main_main_default, 0);
    lv_style_set_pad_right(&style_some_spangroup_2_main_main_default, 0);
    lv_style_set_pad_bottom(&style_some_spangroup_2_main_main_default, 0);
    lv_style_set_pad_left(&style_some_spangroup_2_main_main_default, 0);
    lv_style_set_shadow_width(&style_some_spangroup_2_main_main_default, 0);
    lv_obj_add_style(ui->some_spangroup_2, &style_some_spangroup_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->some_spangroup_2);

    //Write codes some_spangroup_1
    ui->some_spangroup_1 = lv_spangroup_create(ui->some_c_331);
    lv_spangroup_set_align(ui->some_spangroup_1, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->some_spangroup_1, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->some_spangroup_1, LV_SPAN_MODE_BREAK);
    //create span
    ui->some_spangroup_1_span = lv_spangroup_new_span(ui->some_spangroup_1);
    lv_span_set_text(ui->some_spangroup_1_span, "配送货物体积：");
    lv_style_set_text_color(&ui->some_spangroup_1_span->style, lv_color_hex(0xddff00));
    lv_style_set_text_decor(&ui->some_spangroup_1_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->some_spangroup_1_span->style, &lv_font_SourceHanSerifSC_Regular_24);
    lv_obj_set_pos(ui->some_spangroup_1, 118, 76);
    lv_obj_set_size(ui->some_spangroup_1, 170, 27);

    //Write style state: LV_STATE_DEFAULT for &style_some_spangroup_1_main_main_default
    static lv_style_t style_some_spangroup_1_main_main_default;
    ui_init_style(&style_some_spangroup_1_main_main_default);

    lv_style_set_border_width(&style_some_spangroup_1_main_main_default, 0);
    lv_style_set_radius(&style_some_spangroup_1_main_main_default, 0);
    lv_style_set_bg_opa(&style_some_spangroup_1_main_main_default, 0);
    lv_style_set_pad_top(&style_some_spangroup_1_main_main_default, 0);
    lv_style_set_pad_right(&style_some_spangroup_1_main_main_default, 0);
    lv_style_set_pad_bottom(&style_some_spangroup_1_main_main_default, 0);
    lv_style_set_pad_left(&style_some_spangroup_1_main_main_default, 0);
    lv_style_set_shadow_width(&style_some_spangroup_1_main_main_default, 0);
    lv_obj_add_style(ui->some_spangroup_1, &style_some_spangroup_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->some_spangroup_1);

    //Write codes some_spangroup_3
    ui->some_spangroup_3 = lv_spangroup_create(ui->some_c_331);
    lv_spangroup_set_align(ui->some_spangroup_3, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->some_spangroup_3, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->some_spangroup_3, LV_SPAN_MODE_BREAK);
    //create span
    ui->some_spangroup_3_span = lv_spangroup_new_span(ui->some_spangroup_3);
    lv_span_set_text(ui->some_spangroup_3_span, "配送货物id：");
    lv_style_set_text_color(&ui->some_spangroup_3_span->style, lv_color_hex(0xddff00));
    lv_style_set_text_decor(&ui->some_spangroup_3_span->style, LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(&ui->some_spangroup_3_span->style, &lv_font_SourceHanSerifSC_Regular_24);
    lv_obj_set_pos(ui->some_spangroup_3, 118, 113);
    lv_obj_set_size(ui->some_spangroup_3, 170, 27);

    //Write style state: LV_STATE_DEFAULT for &style_some_spangroup_3_main_main_default
    static lv_style_t style_some_spangroup_3_main_main_default;
    ui_init_style(&style_some_spangroup_3_main_main_default);

    lv_style_set_border_width(&style_some_spangroup_3_main_main_default, 0);
    lv_style_set_radius(&style_some_spangroup_3_main_main_default, 0);
    lv_style_set_bg_opa(&style_some_spangroup_3_main_main_default, 0);
    lv_style_set_pad_top(&style_some_spangroup_3_main_main_default, 0);
    lv_style_set_pad_right(&style_some_spangroup_3_main_main_default, 0);
    lv_style_set_pad_bottom(&style_some_spangroup_3_main_main_default, 0);
    lv_style_set_pad_left(&style_some_spangroup_3_main_main_default, 0);
    lv_style_set_shadow_width(&style_some_spangroup_3_main_main_default, 0);
    lv_obj_add_style(ui->some_spangroup_3, &style_some_spangroup_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->some_spangroup_3);

    //Write codes some_c33_2
    ui->some_c33_2 = lv_label_create(ui->some_c_331);
    lv_label_set_text(ui->some_c33_2, "18cm³");
    lv_label_set_long_mode(ui->some_c33_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_c33_2, 271, 76);
    lv_obj_set_size(ui->some_c33_2, 168, 26);

    //Write style for some_c33_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_c33_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_c33_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_c33_2, lv_color_hex(0xffa300), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_c33_2, &lv_font_SourceHanSerifSC_Regular_19, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_c33_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_c33_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_c33_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_c33_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_c33_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_c33_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_c33_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_c33_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_c33_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_c33_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_c33_3
    ui->some_c33_3 = lv_label_create(ui->some_c_331);
    lv_label_set_text(ui->some_c33_3, "0");
    lv_label_set_long_mode(ui->some_c33_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->some_c33_3, 257, 113);
    lv_obj_set_size(ui->some_c33_3, 100, 32);

    //Write style for some_c33_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_c33_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_c33_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_c33_3, lv_color_hex(0xffbf00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_c33_3, &lv_font_SourceHanSerifSC_Regular_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_c33_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->some_c33_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->some_c33_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_c33_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_c33_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_c33_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_c33_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_c33_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_c33_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_c33_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_bs_2
    ui->some_bs_2 = lv_btn_create(ui->some_c_3);
    ui->some_bs_2_label = lv_label_create(ui->some_bs_2);
    lv_label_set_text(ui->some_bs_2_label, "");
    lv_label_set_long_mode(ui->some_bs_2_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->some_bs_2_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->some_bs_2, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->some_bs_2_label, LV_PCT(100));
    lv_obj_set_pos(ui->some_bs_2, 397, 4);
    lv_obj_set_size(ui->some_bs_2, 78, 51);

    //Write style for some_bs_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->some_bs_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->some_bs_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_bs_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_bs_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_bs_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_bs_2, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_bs_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_bs_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_c_2
    ui->some_c_2 = lv_obj_create(ui->some);
    lv_obj_set_pos(ui->some_c_2, -3, -2);
    lv_obj_set_size(ui->some_c_2, 486, 274);
    lv_obj_set_scrollbar_mode(ui->some_c_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for some_c_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->some_c_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->some_c_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->some_c_2, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->some_c_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_c_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->some_c_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->some_c_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->some_c_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->some_c_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->some_c_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->some_c_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->some_c_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_c_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_img_4
    ui->some_img_4 = lv_img_create(ui->some_c_2);
    lv_obj_add_flag(ui->some_img_4, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->some_img_4, &_pape_alpha_494x282);
    lv_img_set_pivot(ui->some_img_4, 50,50);
    lv_img_set_angle(ui->some_img_4, 0);
    lv_obj_set_pos(ui->some_img_4, -6, -4);
    lv_obj_set_size(ui->some_img_4, 494, 282);

    //Write style for some_img_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->some_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->some_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->some_img_4, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_btn_3
    ui->some_btn_3 = lv_btn_create(ui->some_c_2);
    ui->some_btn_3_label = lv_label_create(ui->some_btn_3);
    lv_label_set_text(ui->some_btn_3_label, "");
    lv_label_set_long_mode(ui->some_btn_3_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->some_btn_3_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->some_btn_3, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->some_btn_3_label, LV_PCT(100));
    lv_obj_set_pos(ui->some_btn_3, 39, 142);
    lv_obj_set_size(ui->some_btn_3, 143, 58);

    //Write style for some_btn_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->some_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->some_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_btn_3, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_btn_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_btn_3, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_btn_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_btn_4
    ui->some_btn_4 = lv_btn_create(ui->some_c_2);
    ui->some_btn_4_label = lv_label_create(ui->some_btn_4);
    lv_label_set_text(ui->some_btn_4_label, "");
    lv_label_set_long_mode(ui->some_btn_4_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->some_btn_4_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->some_btn_4, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->some_btn_4_label, LV_PCT(100));
    lv_obj_set_pos(ui->some_btn_4, 294, 142);
    lv_obj_set_size(ui->some_btn_4, 138, 56);

    //Write style for some_btn_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->some_btn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->some_btn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_btn_4, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_btn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_btn_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_btn_4, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_btn_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_btn_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_btn_5
    ui->some_btn_5 = lv_btn_create(ui->some_c_2);
    ui->some_btn_5_label = lv_label_create(ui->some_btn_5);
    lv_label_set_text(ui->some_btn_5_label, "");
    lv_label_set_long_mode(ui->some_btn_5_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->some_btn_5_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->some_btn_5, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->some_btn_5_label, LV_PCT(100));
    lv_obj_set_pos(ui->some_btn_5, 39, 13);
    lv_obj_set_size(ui->some_btn_5, 135, 59);

    //Write style for some_btn_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->some_btn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->some_btn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_btn_5, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_btn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_btn_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_btn_5, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_btn_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_btn_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_btn_6
    ui->some_btn_6 = lv_btn_create(ui->some_c_2);
    ui->some_btn_6_label = lv_label_create(ui->some_btn_6);
    lv_label_set_text(ui->some_btn_6_label, "");
    lv_label_set_long_mode(ui->some_btn_6_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->some_btn_6_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->some_btn_6, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->some_btn_6_label, LV_PCT(100));
    lv_obj_set_pos(ui->some_btn_6, 294, 13);
    lv_obj_set_size(ui->some_btn_6, 147, 51);

    //Write style for some_btn_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->some_btn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->some_btn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_btn_6, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->some_btn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->some_btn_6, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->some_btn_6, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->some_btn_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->some_btn_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_img_8
    ui->some_img_8 = lv_img_create(ui->some);
    lv_obj_add_flag(ui->some_img_8, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->some_img_8, &_huozai4_alpha_100x100);
    lv_img_set_pivot(ui->some_img_8, 50,50);
    lv_img_set_angle(ui->some_img_8, 0);
    lv_obj_set_pos(ui->some_img_8, 189, 79);
    lv_obj_set_size(ui->some_img_8, 100, 100);
    lv_obj_add_flag(ui->some_img_8, LV_OBJ_FLAG_HIDDEN);

    //Write style for some_img_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->some_img_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->some_img_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_img_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->some_img_8, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes some_img_12
    ui->some_img_12 = lv_img_create(ui->some);
    lv_obj_add_flag(ui->some_img_12, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->some_img_12, &_huozai4_alpha_100x100);
    lv_img_set_pivot(ui->some_img_12, 50,50);
    lv_img_set_angle(ui->some_img_12, 0);
    lv_obj_set_pos(ui->some_img_12, 189, 79);
    lv_obj_set_size(ui->some_img_12, 100, 100);
    lv_obj_add_flag(ui->some_img_12, LV_OBJ_FLAG_HIDDEN);

    //Write style for some_img_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->some_img_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->some_img_12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->some_img_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->some_img_12, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of some.

    lv_obj_set_scroll_dir(ui->some, LV_DIR_NONE);
    lv_obj_clear_flag(ui->some, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui->some, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ui->some, LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_clear_flag(ui->some, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_clear_flag(ui->some, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    lv_obj_clear_flag(ui->some, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
    lv_obj_clear_flag(ui->some, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_scroll_dir(ui->some, LV_DIR_NONE);

    lv_obj_set_scroll_dir(ui->some_c_1, LV_DIR_NONE);
    lv_obj_clear_flag(ui->some_c_1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui->some_c_1, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ui->some_c_1, LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_clear_flag(ui->some_c_1, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_clear_flag(ui->some_c_1, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    lv_obj_clear_flag(ui->some_c_1, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
    lv_obj_clear_flag(ui->some_c_1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_scroll_dir(ui->some_c_1, LV_DIR_NONE);

    lv_obj_set_scroll_dir(ui->some_c_2, LV_DIR_NONE);
    lv_obj_clear_flag(ui->some_c_2, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui->some_c_2, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ui->some_c_2, LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_clear_flag(ui->some_c_2, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_clear_flag(ui->some_c_2, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    lv_obj_clear_flag(ui->some_c_2, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
    lv_obj_clear_flag(ui->some_c_2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_scroll_dir(ui->some_c_2, LV_DIR_NONE);

    lv_obj_set_scroll_dir(ui->some_c_3, LV_DIR_NONE);
    lv_obj_clear_flag(ui->some_c_3, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui->some_c_3, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ui->some_c_3, LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_clear_flag(ui->some_c_3, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_clear_flag(ui->some_c_3, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    lv_obj_clear_flag(ui->some_c_3, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
    lv_obj_clear_flag(ui->some_c_3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_scroll_dir(ui->some_c_3, LV_DIR_NONE);



    lv_anim_init(&img8_anim);
    lv_anim_set_var(&img8_anim, ui->some_img_8);
    lv_anim_set_exec_cb(&img8_anim, (lv_anim_exec_xcb_t)set_img_zoom);
    lv_anim_set_values(&img8_anim, 3500, 150);  // 350% -> 150% 缩放
    lv_anim_set_time(&img8_anim, 800);
    lv_anim_set_playback_time(&img8_anim, 800);  // 回放时间
    lv_anim_set_repeat_count(&img8_anim, LV_ANIM_REPEAT_INFINITE);  // 无限循环
    lv_anim_set_playback_delay(&img8_anim, 0);
    lv_anim_set_repeat_delay(&img8_anim, 0);

    //Update current screen layout.
    lv_obj_update_layout(ui->some);

    //Init events for screen.
    events_init_some(ui);
}
