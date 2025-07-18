/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

#include "lvgl.h"
extern lv_ui guider_ui;
#include "lvgl.h"
extern lv_ui guider_ui;
#include "lvgl.h"
extern lv_ui guider_ui;
#include "lvgl.h"
extern lv_ui guider_ui;
#include "lvgl.h"
extern lv_ui guider_ui;
#include "lvgl.h"
extern lv_ui guider_ui;

static void some_bS_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_ui *ui = (lv_ui *)lv_event_get_user_data(e);
        lv_obj_add_flag(ui->some_c_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui->some_c_2, LV_OBJ_FLAG_HIDDEN);

        break;
    }
    default:
        break;
    }
}

static void some_bs_2_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_ui *ui = (lv_ui *)lv_event_get_user_data(e);
        lv_obj_add_flag(ui->some_c_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui->some_c_2, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void some_btn_4_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_ui *ui = (lv_ui *)lv_event_get_user_data(e);
        lv_obj_add_flag(ui->some_c_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui->some_c_3, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void some_btn_5_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_ui *ui = (lv_ui *)lv_event_get_user_data(e);
        lv_obj_add_flag(ui->some_c_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui->some_c_1, LV_OBJ_FLAG_HIDDEN);

        break;
    }
    default:
        break;
    }
}

static void some_btn_6_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_ui *ui = (lv_ui *)lv_event_get_user_data(e);
        lv_obj_add_flag(ui->some_c_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui->some_c_1, LV_OBJ_FLAG_HIDDEN);

        break;
    }
    default:
        break;
    }
}

static void some_img_8_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_ui *ui = (lv_ui *)lv_event_get_user_data(e);
        start_img8_animation(ui->some_img_8);
        break;
    }
    default:
        break;
    }
}

void events_init_some (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->some_bS_1, some_bS_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->some_bs_2, some_bs_2_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->some_btn_4, some_btn_4_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->some_btn_5, some_btn_5_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->some_btn_6, some_btn_6_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->some_img_8, some_img_8_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
