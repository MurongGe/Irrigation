//
// Created by ShunX on 2024/4/3.
//

#include "main.h"
#include "../../Lvgl/lvgl.h"
#include <stdio.h>
#include "gui.h"
#include "sys.h"

extern lv_indev_t *indev_keypad;

/** 控制交互数据 **/
uint8_t connect_flag = 0;
struct __Head_alert_Info Head_alert_info;
extern const char *got_wifi_name;
extern const char *got_wifi_psd;
extern __IO uint8_t start_connect_wifi;
extern __IO uint8_t wifi_connected_flag;
extern __IO uint8_t LvglDataUpdateFlag;
extern __IO uint8_t IsTimeUpdate;
extern __IO uint8_t IsIoTControl;
extern SysInfoType SysInfo;

/** 外部显示数据 **/
extern uint8_t time_data[6];
extern lv_img_dsc_t img_air_temp;
extern lv_img_dsc_t img_air_light;
extern lv_img_dsc_t img_soil_humid;
extern lv_img_dsc_t img_pump;
extern lv_img_dsc_t img_beep;
extern lv_img_dsc_t img_lamp;
extern const lv_font_t lv_font_zhcn_32;
extern const lv_font_t lv_font_zhcn_24;

/** init page **/
lv_obj_t *init_page;
lv_obj_t *label_title;
lv_obj_t *label_user;
lv_obj_t *img_sicau;

/** wifi page obj**/
lv_obj_t *wifi_page;
lv_obj_t *label_WIFI;
lv_obj_t *btn_wifi_right;
lv_obj_t *label_wifi_right;
lv_obj_t *label_wifi_name;
lv_obj_t *textarea_wifi_name;
lv_obj_t *label_wifi_psd;
lv_obj_t *textarea_wifi_psd;
lv_obj_t *keyboard_wifi;
lv_obj_t *btn_wifi_connect;
lv_obj_t *label_wifi_connect;
lv_obj_t *btn_clear;
lv_obj_t *label_clear;
lv_obj_t *ckbox_remb;
lv_obj_t *label_connect_tips;
lv_obj_t *btn_disconnect;
lv_obj_t *label_disconnect;
lv_obj_t *msgbox_wifi_warn;

/** home page obj **/
lv_obj_t *home_page;
lv_obj_t *home_left_btn;
lv_obj_t *home_left_symbol;
lv_obj_t *label_HOME;
lv_obj_t *label_time;
lv_obj_t *btn_home_right;
lv_obj_t *home_right_symbol;
lv_obj_t *switch_auto_mode;
lv_obj_t *label_auto_mode;
lv_obj_t *arc_bg;
lv_obj_t *label_enviroment_txt;
lv_obj_t *line_enviroment_txt;
lv_obj_t *arc_air_temp;
lv_obj_t *arc_air_humid;
lv_obj_t *arc_air_light;
lv_obj_t *arc_soil_humid;
lv_obj_t *label_air_temp;
lv_obj_t *label_air_humid;
lv_obj_t *label_air_light;
lv_obj_t *label_soil_humid;
lv_obj_t *label_air_temp_txt;
lv_obj_t *label_air_humid_txt;
lv_obj_t *label_air_light_txt;
lv_obj_t *label_soil_humid_txt;

/** setting page **/
lv_obj_t *setting_page;
lv_obj_t *btn_setting_left;
lv_obj_t *label_setting_left;
lv_obj_t *label_SETTINGS;
lv_obj_t *line_setting;
lv_obj_t *tabview_setting;
lv_obj_t *tab_state;
lv_obj_t *tab_threshold;
lv_obj_t *tab_timer;
lv_obj_t *img_beep_disp;
lv_obj_t *img_lamp_disp;
lv_obj_t *img_pump_disp;
lv_obj_t *label_beep_name;
lv_obj_t *label_lamp_name;
lv_obj_t *label_pump_name;
lv_obj_t *label_beep_describe;
lv_obj_t *label_lamp_describe;
lv_obj_t *label_pump_describe;
lv_obj_t *led_beep_sta;
lv_obj_t *led_lamp_sta;
lv_obj_t *led_pump_sta;
lv_obj_t *switch_beep_sta;
lv_obj_t *switch_lamp_sta;
lv_obj_t *switch_pump_sta;
lv_obj_t *img_air_temp_disp;
lv_obj_t *img_soil_humid_disp;
lv_obj_t *img_air_light_disp;
lv_obj_t *slider_air_temp;
lv_obj_t *slider_soil_humid;
lv_obj_t *slider_air_light;
lv_obj_t *label_air_temp_up_var;
lv_obj_t *label_soil_humid_down_var;
lv_obj_t *label_air_light_down_var;
lv_obj_t *label_air_temp_up_symbol;
lv_obj_t *label_soil_humid_down_symbol;
lv_obj_t *label_air_light_down_symbol;

/** 定时相关变量 **/
lv_obj_t *bg_alert_setting;
lv_obj_t *btn_setting_save;
lv_obj_t *btn_setting_cancel;
lv_obj_t *label_setting_save;
lv_obj_t *label_setting_cancel;
lv_obj_t *label_alert_setting_titlle;
lv_obj_t *roller_timer_hur;
lv_obj_t *roller_timer_min;
lv_obj_t *roller_exe_min;
lv_obj_t *roller_exe_sec;
lv_obj_t *label_timer_name;
lv_obj_t *label_exe_name;
lv_obj_t *ckbox_exe_lighting;
lv_obj_t *ckbox_exe_pumping;
lv_obj_t *ckbox_exe_alerting;
lv_obj_t *btn_edit_del;
lv_obj_t *label_edit_del;
lv_obj_t *msgbox_tips;
__IO bool flag_editing = false;
__IO bool isEditeThreshold = false;

/** function declare **/
void wifi_page_create(void);

void home_page_create(void);

void setting_page_create(void);

/** event callback **/
static void wifi_input_cb(lv_event_t *e);

static void handle_wifi_cb(lv_event_t *e);

static void page_change_cb(lv_event_t *e);

static void obj_value_chang_cb(lv_event_t *e);

static void handle_add_alert_cb(lv_event_t *e);

static void Alert_list_init(void);

void gui_init() {
  //static lv_group_t *group;
  //// 分组设置
  //group = lv_group_create();
  //lv_group_set_default(group);
  //lv_indev_set_group(indev_keypad, group);

  init_page = lv_obj_create(NULL);
  //lv_obj_set_style_bg_color(init_page, lv_color_hex(0x0), LV_STATE_DEFAULT);

  img_sicau = lv_img_create(init_page);
  lv_img_set_src(img_sicau, "0:/picture/SICAU150.bin");
  lv_obj_align(img_sicau, LV_ALIGN_TOP_MID, 0, 5);

  label_title = lv_label_create(init_page);
  lv_label_set_text_fmt(label_title, "智能盆栽灌溉系统");
  lv_obj_set_style_text_font(label_title, &lv_font_zhcn_32, LV_STATE_DEFAULT);
  lv_obj_align(label_title, LV_ALIGN_CENTER, 0, 55);

  label_user = lv_label_create(init_page);
  lv_label_set_text_fmt(label_user, "四川农业大学周顺心原创");
  lv_obj_set_style_text_font(label_user, &lv_font_zhcn_24, LV_STATE_DEFAULT);
  lv_obj_align(label_user, LV_ALIGN_CENTER, 0, 100);

  lv_scr_load(init_page);
  delay_ms(3000);
  lv_obj_del(img_sicau);
  lv_obj_del(label_title);
  lv_obj_del(label_user);

  wifi_page_create();
  home_page_create();
  setting_page_create();

  lv_scr_load(home_page);
}

/** Wi-Fi连接界面创建 **/
void wifi_page_create(void) {
  wifi_page = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(wifi_page, lv_color_hex(0x45B4B4), LV_STATE_DEFAULT);

  label_WIFI = lv_label_create(wifi_page);
  lv_obj_align(label_WIFI, LV_ALIGN_TOP_MID, 0, 10);
  lv_label_set_text_fmt(label_WIFI, LV_SYMBOL_WIFI " WIFI Connect");
  lv_obj_set_style_text_font(label_WIFI, &lv_font_montserrat_24, LV_STATE_DEFAULT);

  btn_wifi_right = lv_btn_create(wifi_page);
  lv_obj_set_size(btn_wifi_right, 50, 30);
  lv_obj_align(btn_wifi_right, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_add_event_cb(btn_wifi_right, page_change_cb, LV_EVENT_SHORT_CLICKED, NULL);
  label_wifi_right = lv_label_create(btn_wifi_right);
  lv_obj_center(label_wifi_right);
  lv_label_set_text_fmt(label_wifi_right, LV_SYMBOL_RIGHT);
  lv_obj_set_style_text_font(label_wifi_right, &lv_font_montserrat_32, LV_STATE_DEFAULT);

  textarea_wifi_name = lv_textarea_create(wifi_page);
  lv_obj_set_width(textarea_wifi_name, 200);
  lv_obj_align(textarea_wifi_name, LV_ALIGN_CENTER, 0, -50);
  lv_textarea_set_placeholder_text(textarea_wifi_name, "wifi name");
  lv_textarea_set_one_line(textarea_wifi_name, true);
  lv_textarea_set_text(textarea_wifi_name, "honor-play4pro");
  textarea_wifi_psd = lv_textarea_create(wifi_page);
  lv_obj_set_width(textarea_wifi_psd, 200);
  lv_textarea_set_text(textarea_wifi_psd, "18384546746");
  lv_obj_align(textarea_wifi_psd, LV_ALIGN_CENTER, 0, 0);
  lv_textarea_set_placeholder_text(textarea_wifi_psd, "wifi password");
  lv_textarea_set_one_line(textarea_wifi_psd, true);
  /* 文本回调 */
  lv_obj_add_event_cb(textarea_wifi_name, wifi_input_cb, LV_EVENT_SHORT_CLICKED, NULL);
  lv_obj_add_event_cb(textarea_wifi_psd, wifi_input_cb, LV_EVENT_SHORT_CLICKED, NULL);

  label_wifi_name = lv_label_create(wifi_page);
  lv_obj_align_to(label_wifi_name, textarea_wifi_name, LV_ALIGN_OUT_LEFT_MID, -20, 0);
  lv_label_set_text_fmt(label_wifi_name, "SSID:");
  lv_obj_set_style_text_font(label_wifi_name, &lv_font_montserrat_16, LV_STATE_DEFAULT);
  label_wifi_psd = lv_label_create(wifi_page);
  lv_obj_align_to(label_wifi_psd, textarea_wifi_psd, LV_ALIGN_OUT_LEFT_MID, -20, 0);
  lv_label_set_text_fmt(label_wifi_psd, "PSD :");
  lv_obj_set_style_text_font(label_wifi_psd, &lv_font_montserrat_16, LV_STATE_DEFAULT);

  btn_wifi_connect = lv_btn_create(wifi_page);
  lv_obj_set_size(btn_wifi_connect, 100, 40);
  lv_obj_align(btn_wifi_connect, LV_ALIGN_BOTTOM_MID, -80, -20);
  lv_obj_add_event_cb(btn_wifi_connect, handle_wifi_cb, LV_EVENT_SHORT_CLICKED, LV_STATE_DEFAULT);
  label_wifi_connect = lv_label_create(btn_wifi_connect);
  lv_label_set_text_fmt(label_wifi_connect, "connect");
  lv_obj_center(label_wifi_connect);

  btn_clear = lv_btn_create(wifi_page);
  lv_obj_set_size(btn_clear, 100, 40);
  lv_obj_align(btn_clear, LV_ALIGN_BOTTOM_MID, 80, -20);
  lv_obj_add_event_cb(btn_clear, handle_wifi_cb, LV_EVENT_SHORT_CLICKED, LV_STATE_DEFAULT);
  label_clear = lv_label_create(btn_clear);
  lv_label_set_text_fmt(label_clear, "clear");
  lv_obj_center(label_clear);

  ckbox_remb = lv_checkbox_create(wifi_page);
  lv_obj_align(ckbox_remb, LV_ALIGN_CENTER, -20, 40);
  lv_checkbox_set_text(ckbox_remb, "remember password");

  label_connect_tips = lv_label_create(wifi_page);
  lv_label_set_text_fmt(label_connect_tips, "Waiting WiFi connect ...");
  lv_obj_set_style_text_font(label_connect_tips, &lv_font_montserrat_16, LV_STATE_DEFAULT);
  lv_obj_center(label_connect_tips);
  lv_obj_add_flag(label_connect_tips, LV_OBJ_FLAG_HIDDEN);

  btn_disconnect = lv_btn_create(wifi_page);
  lv_obj_set_size(btn_disconnect, 100, 40);
  lv_obj_align(btn_disconnect, LV_ALIGN_CENTER, 0, 80);
  lv_obj_set_style_bg_color(btn_disconnect, lv_color_hex(0xff0000), LV_STATE_DEFAULT);
  label_disconnect = lv_label_create(btn_disconnect);
  lv_obj_center(label_disconnect);
  lv_label_set_text_fmt(label_disconnect, "Disconnect");
  lv_obj_add_flag(btn_disconnect, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_event_cb(btn_disconnect, handle_wifi_cb, LV_EVENT_CLICKED, LV_STATE_DEFAULT);

  keyboard_wifi = lv_keyboard_create(wifi_page);
  lv_keyboard_set_popovers(keyboard_wifi, true);
  lv_obj_add_flag(keyboard_wifi, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_event_cb(keyboard_wifi, wifi_input_cb, LV_EVENT_READY, NULL);

  if (connect_flag == 1) {
    lv_obj_add_flag(label_wifi_name, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(textarea_wifi_name, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(label_wifi_psd, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(textarea_wifi_psd, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(btn_wifi_connect, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(label_wifi_connect, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(btn_clear, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(label_clear, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ckbox_remb, LV_OBJ_FLAG_HIDDEN);

    lv_obj_clear_flag(label_connect_tips, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(btn_disconnect, LV_OBJ_FLAG_HIDDEN);
  }
}

/** 环境监控界面创建 **/
void home_page_create(void) {
  home_page = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(home_page, lv_color_hex(0x99c8f6), LV_STATE_DEFAULT);

  home_left_btn = lv_btn_create(home_page);
  lv_obj_set_size(home_left_btn, 50, 30);
  lv_obj_align(home_left_btn, LV_ALIGN_TOP_LEFT, 0, 0);
  home_left_symbol = lv_label_create(home_left_btn);
  lv_label_set_text_fmt(home_left_symbol, LV_SYMBOL_LEFT);
  lv_obj_set_style_text_font(home_left_symbol, &lv_font_montserrat_32, LV_STATE_DEFAULT);
  lv_obj_center(home_left_symbol);
  lv_obj_add_event_cb(home_left_btn, page_change_cb, LV_EVENT_SHORT_CLICKED, NULL);

  btn_home_right = lv_btn_create(home_page);
  lv_obj_set_size(btn_home_right, 50, 30);
  lv_obj_align(btn_home_right, LV_ALIGN_TOP_RIGHT, 0, 0);
  home_right_symbol = lv_label_create(btn_home_right);
  lv_label_set_text_fmt(home_right_symbol, LV_SYMBOL_RIGHT);
  lv_obj_set_style_text_font(home_right_symbol, &lv_font_montserrat_32, LV_STATE_DEFAULT);
  lv_obj_center(home_right_symbol);
  lv_obj_add_event_cb(btn_home_right, page_change_cb, LV_EVENT_SHORT_CLICKED, NULL);

  label_HOME = lv_label_create(home_page);
  lv_obj_align(label_HOME, LV_ALIGN_TOP_MID, 0, 10);
  lv_label_set_text_fmt(label_HOME, LV_SYMBOL_HOME " HOME");
  lv_obj_set_style_text_font(label_HOME, &lv_font_montserrat_24, LV_STATE_DEFAULT);

  label_time = lv_label_create(home_page);
  lv_label_set_text_fmt(label_time, "%2d : %2d : %2d", time_data[3], time_data[4], time_data[5]);
  lv_obj_align(label_time, LV_ALIGN_LEFT_MID, 10, -50);
  lv_obj_set_style_text_font(label_time, &lv_font_montserrat_24, LV_STATE_DEFAULT);

  /* 自动与手动  */
  switch_auto_mode = lv_switch_create(home_page);
  lv_obj_set_size(switch_auto_mode, 40, 24);
  lv_obj_align(switch_auto_mode, LV_ALIGN_RIGHT_MID, -10, -45);
  lv_obj_add_state(switch_auto_mode, LV_STATE_CHECKED);
  lv_obj_add_event_cb(switch_auto_mode, obj_value_chang_cb, LV_EVENT_VALUE_CHANGED, NULL);
  label_auto_mode = lv_label_create(home_page);
  lv_label_set_text_fmt(label_auto_mode, "Auto Mode");
  lv_obj_set_style_text_font(label_auto_mode, &lv_font_montserrat_16, LV_STATE_DEFAULT);
  lv_obj_align_to(label_auto_mode, switch_auto_mode, LV_ALIGN_OUT_LEFT_MID, 0, 0);


  /******圆弧*********/
  arc_bg = lv_obj_create(home_page);
  lv_obj_set_size(arc_bg, 300, 150);
  lv_obj_align(arc_bg, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_bg_color(arc_bg, lv_color_hex(0xf0f0c0), LV_STATE_DEFAULT);
  label_enviroment_txt = lv_label_create(arc_bg);
  lv_obj_align(label_enviroment_txt, LV_ALIGN_TOP_MID, 0, -10);
  lv_label_set_text_fmt(label_enviroment_txt, "Enviroment Monitor");
  lv_obj_set_style_text_font(label_enviroment_txt, &lv_font_montserrat_16, LV_STATE_DEFAULT);
  line_enviroment_txt = lv_line_create(arc_bg);
  static lv_point_t pt[2] = {{0,   50},
                             {200, 50}};
  lv_line_set_points(line_enviroment_txt, pt, 2);
  lv_obj_align_to(line_enviroment_txt, label_enviroment_txt, LV_ALIGN_BOTTOM_MID, 0, 10);

  arc_air_temp = lv_arc_create(arc_bg);
  lv_obj_set_size(arc_air_temp, 60, 60);
  lv_obj_align(arc_air_temp, LV_ALIGN_LEFT_MID, -10, 15);
  label_air_temp = lv_label_create(arc_air_temp);
  lv_arc_set_range(arc_air_temp, -30, 60);
  lv_arc_set_value(arc_air_temp, 25);
  lv_obj_center(label_air_temp);
  lv_label_set_text_fmt(label_air_temp, "25C");
  label_air_temp_txt = lv_label_create(arc_bg);
  lv_obj_align_to(label_air_temp_txt, arc_air_temp, LV_ALIGN_BOTTOM_MID, -8, 16);
  lv_label_set_text_fmt(label_air_temp_txt, "Air Temp");
  lv_obj_set_style_text_font(label_air_temp_txt, &lv_font_montserrat_10, LV_STATE_DEFAULT);

  arc_air_humid = lv_arc_create(arc_bg);
  lv_obj_set_size(arc_air_humid, 60, 60);
  lv_obj_align(arc_air_humid, LV_ALIGN_LEFT_MID, 65, 15);
  lv_arc_set_value(arc_air_humid, 20);
  label_air_humid = lv_label_create(arc_air_humid);
  lv_obj_center(label_air_humid);
  lv_label_set_text_fmt(label_air_humid, "20%%");
  label_air_humid_txt = lv_label_create(arc_bg);
  lv_obj_align_to(label_air_humid_txt, arc_air_humid, LV_ALIGN_BOTTOM_MID, -8, 16);
  lv_label_set_text_fmt(label_air_humid_txt, "Air Humid");
  lv_obj_set_style_text_font(label_air_humid_txt, &lv_font_montserrat_10, LV_STATE_DEFAULT);

  arc_air_light = lv_arc_create(arc_bg);
  lv_obj_set_size(arc_air_light, 60, 60);
  lv_obj_align(arc_air_light, LV_ALIGN_RIGHT_MID, -65, 15);
  lv_arc_set_value(arc_air_light, 30);
  label_air_light = lv_label_create(arc_air_light);
  lv_obj_center(label_air_light);
  lv_label_set_text_fmt(label_air_light, "30%%");
  label_air_light_txt = lv_label_create(arc_bg);
  lv_obj_align_to(label_air_light_txt, arc_air_light, LV_ALIGN_BOTTOM_MID, -8, 16);
  lv_label_set_text_fmt(label_air_light_txt, "Air Light");
  lv_obj_set_style_text_font(label_air_light_txt, &lv_font_montserrat_10, LV_STATE_DEFAULT);

  arc_soil_humid = lv_arc_create(arc_bg);
  lv_obj_set_size(arc_soil_humid, 60, 60);
  lv_arc_set_value(arc_soil_humid, 40);
  lv_obj_align(arc_soil_humid, LV_ALIGN_RIGHT_MID, 10, 15);
  label_soil_humid = lv_label_create(arc_soil_humid);
  lv_obj_center(label_soil_humid);
  lv_label_set_text_fmt(label_soil_humid, "40%%");
  label_soil_humid_txt = lv_label_create(arc_bg);
  lv_obj_align_to(label_soil_humid_txt, arc_soil_humid, LV_ALIGN_BOTTOM_MID, -8, 16);
  lv_label_set_text_fmt(label_soil_humid_txt, "Soil Humid");
  lv_obj_set_style_text_font(label_soil_humid_txt, &lv_font_montserrat_10, LV_STATE_DEFAULT);

  // arc 回调函数
  lv_obj_add_event_cb(arc_air_temp, obj_value_chang_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(arc_air_humid, obj_value_chang_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(arc_air_light, obj_value_chang_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(arc_soil_humid, obj_value_chang_cb, LV_EVENT_VALUE_CHANGED, NULL);


  lv_obj_set_style_arc_width(arc_air_temp, 6, LV_PART_MAIN);
  lv_obj_set_style_arc_width(arc_air_temp, 6, LV_PART_INDICATOR);
  lv_obj_set_style_arc_color(arc_air_temp, lv_color_hex(0x3ca03c), LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(arc_air_temp, lv_color_hex(0x3ca03c), LV_PART_KNOB);
  lv_obj_set_style_arc_width(arc_air_humid, 6, LV_PART_MAIN);
  lv_obj_set_style_arc_width(arc_air_humid, 6, LV_PART_INDICATOR);
  lv_obj_set_style_arc_color(arc_air_humid, lv_color_hex(0xff40f0), LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(arc_air_humid, lv_color_hex(0xff40f0), LV_PART_KNOB);
  lv_obj_set_style_arc_width(arc_air_light, 6, LV_PART_MAIN);
  lv_obj_set_style_arc_width(arc_air_light, 6, LV_PART_INDICATOR);
  lv_obj_set_style_arc_color(arc_air_light, lv_color_hex(0xbaba30), LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(arc_air_light, lv_color_hex(0x3ca03c), LV_PART_KNOB);
  lv_obj_set_style_arc_width(arc_soil_humid, 6, LV_PART_MAIN);
  lv_obj_set_style_arc_width(arc_soil_humid, 6, LV_PART_INDICATOR);

  lv_event_send(switch_auto_mode, LV_EVENT_VALUE_CHANGED, NULL);


}

/** 系统设置界面创建 **/
void setting_page_create(void) {
  setting_page = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(setting_page, lv_color_hex(0xb9de86), LV_STATE_DEFAULT);

  btn_setting_left = lv_btn_create(setting_page);
  lv_obj_set_size(btn_setting_left, 50, 30);
  lv_obj_align(btn_setting_left, LV_ALIGN_TOP_LEFT, 0, 0);
  label_setting_left = lv_label_create(btn_setting_left);
  lv_obj_center(label_setting_left);
  lv_label_set_text_fmt(label_setting_left, LV_SYMBOL_LEFT);
  lv_obj_set_style_text_font(label_setting_left, &lv_font_montserrat_32, LV_STATE_DEFAULT);
  lv_obj_add_event_cb(btn_setting_left, page_change_cb, LV_EVENT_SHORT_CLICKED, NULL);

  label_SETTINGS = lv_label_create(setting_page);
  lv_obj_align(label_SETTINGS, LV_ALIGN_TOP_MID, 0, 10);
  lv_label_set_text_fmt(label_SETTINGS, LV_SYMBOL_SETTINGS " SETTINGS");
  lv_obj_set_style_text_font(label_SETTINGS, &lv_font_montserrat_24, LV_STATE_DEFAULT);

  btn_edit_del = lv_btn_create(setting_page);
  lv_obj_set_size(btn_edit_del, 48, 24);
  lv_obj_align(btn_edit_del, LV_ALIGN_TOP_RIGHT, -10, 10);
  label_edit_del = lv_label_create(btn_edit_del);
  lv_label_set_text_fmt(label_edit_del, "Edite");
  lv_obj_center(label_edit_del);
  lv_obj_add_flag(btn_edit_del, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_event_cb(btn_edit_del, handle_add_alert_cb, LV_EVENT_SHORT_CLICKED, NULL);

  line_setting = lv_line_create(setting_page);
  static lv_point_t pt_setting_line[2] = {{0,   50},
                                          {280, 50}};
  lv_line_set_points(line_setting, pt_setting_line, 2);
  lv_obj_align_to(line_setting, label_SETTINGS, LV_ALIGN_BOTTOM_MID, 0, 10);

  tabview_setting = lv_tabview_create(setting_page, LV_DIR_TOP, 30);
  lv_obj_set_size(tabview_setting, 300, 180);
  lv_obj_align(tabview_setting, LV_ALIGN_TOP_MID, 0, 52);
  tab_state = lv_tabview_add_tab(tabview_setting, "State");
  tab_threshold = lv_tabview_add_tab(tabview_setting, "Threshold");
  tab_timer = lv_tabview_add_tab(tabview_setting, "Timer");
  lv_obj_add_event_cb(tabview_setting, obj_value_chang_cb, LV_EVENT_VALUE_CHANGED, NULL);

  img_lamp_disp = lv_img_create(tab_state);
  lv_obj_set_size(img_lamp_disp, 32, 32);
  lv_obj_align(img_lamp_disp, LV_ALIGN_TOP_MID, -64, 0);
  lv_img_set_src(img_lamp_disp, &img_lamp);
  img_pump_disp = lv_img_create(tab_state);
  lv_obj_set_size(img_pump_disp, 32, 32);
  lv_obj_align(img_pump_disp, LV_ALIGN_CENTER, -64, 0);
  lv_img_set_src(img_pump_disp, &img_pump);
  img_beep_disp = lv_img_create(tab_state);
  lv_obj_set_size(img_beep_disp, 32, 32);
  lv_obj_align(img_beep_disp, LV_ALIGN_BOTTOM_MID, -64, 0);
  lv_img_set_src(img_beep_disp, &img_beep);

  label_beep_name = lv_label_create(tab_state);
  lv_label_set_text_fmt(label_beep_name, "Beep: ");
  lv_obj_set_style_text_font(label_beep_name, &lv_font_montserrat_16, LV_STATE_DEFAULT);
  lv_obj_align_to(label_beep_name, img_beep_disp, LV_ALIGN_LEFT_MID, -60, 0);
  label_lamp_name = lv_label_create(tab_state);
  lv_label_set_text_fmt(label_lamp_name, "Bulb: ");
  lv_obj_set_style_text_font(label_lamp_name, &lv_font_montserrat_16, LV_STATE_DEFAULT);
  lv_obj_align_to(label_lamp_name, img_lamp_disp, LV_ALIGN_LEFT_MID, -60, 0);
  label_pump_name = lv_label_create(tab_state);
  lv_label_set_text_fmt(label_pump_name, "Pump: ");
  lv_obj_set_style_text_font(label_pump_name, &lv_font_montserrat_16, LV_STATE_DEFAULT);
  lv_obj_align_to(label_pump_name, img_pump_disp, LV_ALIGN_LEFT_MID, -60, 0);

  label_beep_describe = lv_label_create(tab_state);
  lv_label_set_text_fmt(label_beep_describe, "this device is not working");
  lv_obj_set_style_text_font(label_beep_describe, &lv_font_montserrat_10, LV_STATE_DEFAULT);
  lv_obj_align_to(label_beep_describe, img_beep_disp, LV_ALIGN_LEFT_MID, 40, 0);
  label_lamp_describe = lv_label_create(tab_state);
  lv_label_set_text_fmt(label_lamp_describe, "this device is not working");
  lv_obj_set_style_text_font(label_lamp_describe, &lv_font_montserrat_10, LV_STATE_DEFAULT);
  lv_obj_align_to(label_lamp_describe, img_lamp_disp, LV_ALIGN_LEFT_MID, 40, 0);
  label_pump_describe = lv_label_create(tab_state);
  lv_label_set_text_fmt(label_pump_describe, "this device is not working");
  lv_obj_set_style_text_font(label_pump_describe, &lv_font_montserrat_10, LV_STATE_DEFAULT);
  lv_obj_align_to(label_pump_describe, img_pump_disp, LV_ALIGN_LEFT_MID, 40, 0);


  led_lamp_sta = lv_led_create(tab_state);
  lv_led_set_color(led_lamp_sta, lv_color_hex(0x2b2b2b));
  lv_obj_set_size(led_lamp_sta, 16, 16);
  lv_obj_align(led_lamp_sta, LV_ALIGN_TOP_RIGHT, 0, 5);
  led_pump_sta = lv_led_create(tab_state);
  lv_led_set_color(led_pump_sta, lv_color_hex(0x2b2b2b));
  lv_obj_set_size(led_pump_sta, 16, 16);
  lv_obj_align(led_pump_sta, LV_ALIGN_RIGHT_MID, 0, 0);
  led_beep_sta = lv_led_create(tab_state);
  lv_led_set_color(led_beep_sta, lv_color_hex(0x2b2b2b));
  lv_obj_set_size(led_beep_sta, 16, 16);
  lv_obj_align(led_beep_sta, LV_ALIGN_BOTTOM_RIGHT, 0, -5);

  switch_lamp_sta = lv_switch_create(tab_state);
  lv_obj_set_size(switch_lamp_sta, 42, 20);
  lv_obj_align(switch_lamp_sta, LV_ALIGN_TOP_RIGHT, 0, 5);
  switch_pump_sta = lv_switch_create(tab_state);
  lv_obj_set_size(switch_pump_sta, 42, 20);
  lv_obj_align(switch_pump_sta, LV_ALIGN_RIGHT_MID, 0, 0);
  switch_beep_sta = lv_switch_create(tab_state);
  lv_obj_set_size(switch_beep_sta, 42, 20);
  lv_obj_align(switch_beep_sta, LV_ALIGN_BOTTOM_RIGHT, 0, -5);
  lv_obj_add_event_cb(switch_lamp_sta, obj_value_chang_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(switch_beep_sta, obj_value_chang_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(switch_pump_sta, obj_value_chang_cb, LV_EVENT_VALUE_CHANGED, NULL);

  lv_obj_add_flag(switch_lamp_sta, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(switch_pump_sta, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(switch_beep_sta, LV_OBJ_FLAG_HIDDEN);

  img_air_temp_disp = lv_img_create(tab_threshold);
  lv_obj_align(img_air_temp_disp, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_img_set_src(img_air_temp_disp, &img_air_temp);

  img_air_light_disp = lv_img_create(tab_threshold);
  lv_obj_align(img_air_light_disp, LV_ALIGN_LEFT_MID, 0, 0);
  lv_img_set_src(img_air_light_disp, &img_air_light);

  img_soil_humid_disp = lv_img_create(tab_threshold);
  lv_obj_align(img_soil_humid_disp, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  lv_img_set_src(img_soil_humid_disp, &img_soil_humid);

  label_air_temp_up_var = lv_label_create(tab_threshold);
  lv_obj_align_to(label_air_temp_up_var, img_air_temp_disp, LV_ALIGN_RIGHT_MID, 46, 0);
  lv_label_set_text_fmt(label_air_temp_up_var, "30C");
  lv_obj_set_style_text_font(label_air_temp_up_var, &lv_font_montserrat_16, LV_STATE_DEFAULT);
  label_air_light_down_var = lv_label_create(tab_threshold);
  lv_obj_align_to(label_air_light_down_var, img_air_light_disp, LV_ALIGN_RIGHT_MID, 38, 0);
  lv_label_set_text_fmt(label_air_light_down_var, "40%%");
  lv_obj_set_style_text_font(label_air_light_down_var, &lv_font_montserrat_16, LV_STATE_DEFAULT);
  label_soil_humid_down_var = lv_label_create(tab_threshold);
  lv_obj_align_to(label_soil_humid_down_var, img_soil_humid_disp, LV_ALIGN_RIGHT_MID, 46, 0);
  lv_label_set_text_fmt(label_soil_humid_down_var, "40%%");
  lv_obj_set_style_text_font(label_soil_humid_down_var, &lv_font_montserrat_16, LV_STATE_DEFAULT);

  slider_air_temp = lv_slider_create(tab_threshold);
  lv_obj_align(slider_air_temp, LV_ALIGN_TOP_MID, 30, 10);
  lv_obj_set_size(slider_air_temp, 150, 12);
  lv_slider_set_range(slider_air_temp, 0, 60);
  lv_slider_set_value(slider_air_temp, 30, LV_ANIM_ON);
  slider_air_light = lv_slider_create(tab_threshold);
  lv_obj_align(slider_air_light, LV_ALIGN_CENTER, 30, 0);
  lv_obj_set_size(slider_air_light, 150, 12);
  lv_slider_set_value(slider_air_light, 40, LV_ANIM_ON);
  slider_soil_humid = lv_slider_create(tab_threshold);
  lv_obj_align(slider_soil_humid, LV_ALIGN_BOTTOM_MID, 30, -10);
  lv_obj_set_size(slider_soil_humid, 150, 12);
  lv_slider_set_value(slider_soil_humid, 40, LV_ANIM_ON);
  lv_obj_add_event_cb(slider_air_temp, obj_value_chang_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(slider_air_light, obj_value_chang_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(slider_soil_humid, obj_value_chang_cb, LV_EVENT_VALUE_CHANGED, NULL);
  // 默认不可编辑
  lv_obj_clear_flag(slider_air_temp, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_clear_flag(slider_air_light, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_clear_flag(slider_soil_humid, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_bg_color(slider_air_temp, lv_color_hex(0xff2000), LV_PART_KNOB);
  lv_obj_set_style_bg_color(slider_air_temp, lv_color_hex(0xff2000), LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(slider_air_light, lv_color_hex(0xff2000), LV_PART_KNOB);
  lv_obj_set_style_bg_color(slider_air_light, lv_color_hex(0xff2000), LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(slider_soil_humid, lv_color_hex(0xff2000), LV_PART_KNOB);
  lv_obj_set_style_bg_color(slider_soil_humid, lv_color_hex(0xff2000), LV_PART_INDICATOR);

  label_air_temp_up_symbol = lv_label_create(tab_threshold);
  lv_label_set_text_fmt(label_air_temp_up_symbol, LV_SYMBOL_UP);
  lv_obj_align(label_air_temp_up_symbol, LV_ALIGN_TOP_RIGHT, 0, 0);
  label_air_light_down_symbol = lv_label_create(tab_threshold);
  lv_label_set_text_fmt(label_air_light_down_symbol, LV_SYMBOL_DOWN);
  lv_obj_align(label_air_light_down_symbol, LV_ALIGN_RIGHT_MID, 0, 0);
  label_soil_humid_down_symbol = lv_label_create(tab_threshold);
  lv_label_set_text_fmt(label_soil_humid_down_symbol, LV_SYMBOL_DOWN);
  lv_obj_align(label_soil_humid_down_symbol, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

/*  alert_bg1 = lv_obj_create(tab_timer);
  lv_obj_set_size(alert_bg1, 240, 45);
  lv_obj_align(alert_bg1, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_bg_color(alert_bg1, lv_color_hex(0xc0c0c0), LV_STATE_DEFAULT);
  alert_bg2 = lv_obj_create(tab_timer);
  lv_obj_set_size(alert_bg2, 240, 45);
  lv_obj_align(alert_bg2, LV_ALIGN_TOP_MID, 0, 55);
  lv_obj_set_style_bg_color(alert_bg2, lv_color_hex(0xc0c0c0), LV_STATE_DEFAULT);
  alert_bg3 = lv_obj_create(tab_timer);
  lv_obj_set_size(alert_bg3, 240, 45);
  lv_obj_align(alert_bg3, LV_ALIGN_TOP_MID, 0, 110);
  lv_obj_set_style_bg_color(alert_bg3, lv_color_hex(0xc0c0c0), LV_STATE_DEFAULT);

  btn_add_alert = lv_btn_create(tab_timer);
  lv_obj_add_event_cb(btn_add_alert, handle_add_alert_cb, LV_EVENT_SHORT_CLICKED, NULL);
  lv_obj_set_size(btn_add_alert, 60, 25);
  lv_obj_align_to(btn_add_alert, alert_bg1, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_bg_color(btn_add_alert, lv_color_hex(0x40ac56), LV_STATE_DEFAULT);
  label_add_alert = lv_label_create(btn_add_alert);
  lv_obj_center(label_add_alert);
  lv_label_set_text_fmt(label_add_alert, LV_SYMBOL_PLUS);*/

  bg_alert_setting = lv_obj_create(setting_page);
  lv_obj_set_size(bg_alert_setting, 260, 200);
  lv_obj_center(bg_alert_setting);
  lv_obj_add_flag(bg_alert_setting, LV_OBJ_FLAG_HIDDEN);

  btn_setting_save = lv_btn_create(bg_alert_setting);
  lv_obj_set_size(btn_setting_save, 50, 25);
  lv_obj_align(btn_setting_save, LV_ALIGN_TOP_RIGHT, 0, -5);
  label_setting_save = lv_label_create(btn_setting_save);
  lv_obj_center(label_setting_save);
  lv_label_set_text_fmt(label_setting_save, "Save");
  lv_obj_add_event_cb(btn_setting_save, handle_add_alert_cb, LV_EVENT_SHORT_CLICKED, NULL);

  btn_setting_cancel = lv_btn_create(bg_alert_setting);
  lv_obj_set_size(btn_setting_cancel, 50, 25);
  lv_obj_align(btn_setting_cancel, LV_ALIGN_TOP_LEFT, 0, -5);
  label_setting_cancel = lv_label_create(btn_setting_cancel);
  lv_obj_center(label_setting_cancel);
  lv_label_set_text_fmt(label_setting_cancel, "Quit");
  lv_obj_add_event_cb(btn_setting_cancel, handle_add_alert_cb, LV_EVENT_SHORT_CLICKED, NULL);

  label_alert_setting_titlle = lv_label_create(bg_alert_setting);
  lv_obj_align(label_alert_setting_titlle, LV_ALIGN_TOP_MID, 0, -10);
  lv_label_set_text_fmt(label_alert_setting_titlle, "Alert Settings");
  lv_obj_set_style_text_font(label_alert_setting_titlle, &lv_font_montserrat_16, LV_STATE_DEFAULT);

  static const char roller_options_24[] = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n";
  static const char roller_options_60[] = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n";

  roller_timer_hur = lv_roller_create(bg_alert_setting);
  lv_obj_set_size(roller_timer_hur, 40, 100);
  lv_obj_align(roller_timer_hur, LV_ALIGN_LEFT_MID, 0, 10);
  lv_roller_set_options(roller_timer_hur, roller_options_24, LV_ROLLER_MODE_NORMAL);
  lv_obj_set_style_text_line_space(roller_timer_hur, 16, LV_STATE_DEFAULT);

  roller_timer_min = lv_roller_create(bg_alert_setting);
  lv_obj_set_size(roller_timer_min, 40, 100);
  lv_obj_align(roller_timer_min, LV_ALIGN_LEFT_MID, 55, 10);
  lv_roller_set_options(roller_timer_min, roller_options_60, LV_ROLLER_MODE_NORMAL);
  lv_obj_set_style_text_line_space(roller_timer_min, 20, LV_STATE_DEFAULT);

  roller_exe_min = lv_roller_create(bg_alert_setting);
  lv_obj_set_size(roller_exe_min, 40, 100);
  lv_obj_align(roller_exe_min, LV_ALIGN_RIGHT_MID, -55, 10);
  lv_roller_set_options(roller_exe_min, roller_options_60, LV_ROLLER_MODE_NORMAL);

  roller_exe_sec = lv_roller_create(bg_alert_setting);
  lv_obj_set_size(roller_exe_sec, 40, 100);
  lv_obj_align(roller_exe_sec, LV_ALIGN_RIGHT_MID, 0, 10);
  lv_roller_set_options(roller_exe_sec, roller_options_60, LV_ROLLER_MODE_NORMAL);

  label_timer_name = lv_label_create(bg_alert_setting);
  lv_obj_align(label_timer_name, LV_ALIGN_LEFT_MID, 10, -50);
  lv_label_set_text_fmt(label_timer_name, "time: H/M");
  lv_obj_set_style_text_font(label_timer_name, &lv_font_montserrat_16, LV_STATE_DEFAULT);
  label_exe_name = lv_label_create(bg_alert_setting);
  lv_obj_align(label_exe_name, LV_ALIGN_RIGHT_MID, -10, -50);
  lv_label_set_text_fmt(label_exe_name, "hold: M/S");
  lv_obj_set_style_text_font(label_exe_name, &lv_font_montserrat_16, LV_STATE_DEFAULT);

  ckbox_exe_pumping = lv_checkbox_create(bg_alert_setting);
  lv_obj_align(ckbox_exe_pumping, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  lv_checkbox_set_text(ckbox_exe_pumping, "pumping");
  lv_obj_set_style_text_font(ckbox_exe_pumping, &lv_font_montserrat_10, LV_STATE_DEFAULT);
  ckbox_exe_lighting = lv_checkbox_create(bg_alert_setting);
  lv_obj_align(ckbox_exe_lighting, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_checkbox_set_text(ckbox_exe_lighting, "lighting");
  lv_obj_set_style_text_font(ckbox_exe_lighting, &lv_font_montserrat_10, LV_STATE_DEFAULT);
  ckbox_exe_alerting = lv_checkbox_create(bg_alert_setting);
  lv_obj_align(ckbox_exe_alerting, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
  lv_checkbox_set_text(ckbox_exe_alerting, "alerting");
  lv_obj_set_style_text_font(ckbox_exe_alerting, &lv_font_montserrat_10, LV_STATE_DEFAULT);
  lv_obj_add_event_cb(ckbox_exe_alerting, handle_add_alert_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(ckbox_exe_lighting, handle_add_alert_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(ckbox_exe_pumping, handle_add_alert_cb, LV_EVENT_VALUE_CHANGED, NULL);

  Alert_list_init();
}

/** 动态内存 闹钟创建于 **/
static void Alert_list_init(void) {
  /** 初始化链表头指针 **/
  Head_alert_info.first = (Alert_Body_Type *) lv_mem_alloc(sizeof(Alert_Body_Type));
  Head_alert_info.last = Head_alert_info.first;
  Head_alert_info.num_alert = 0;
  Head_alert_info.first->ckbox_del = NULL;
  Head_alert_info.first->event_alerting = false;
  Head_alert_info.first->event_lighting = false;
  Head_alert_info.first->event_pumping = false;
  Head_alert_info.first->isfirstone = true;
  Head_alert_info.first->islastone = true;
  Head_alert_info.first->label_alert_content = NULL;
  Head_alert_info.first->next = NULL;
  Head_alert_info.first->pre = NULL;
  Head_alert_info.first->time_alert.hur = 0;
  Head_alert_info.first->time_alert.min = 0;
  Head_alert_info.first->time_hold.min = 0;
  Head_alert_info.first->time_hold.sec = 0;

  Head_alert_info.first->bg_alert = lv_obj_create(tab_timer);
  lv_obj_set_size(Head_alert_info.first->bg_alert, 240, 44);
  lv_obj_align(Head_alert_info.first->bg_alert, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_bg_color(Head_alert_info.first->bg_alert, lv_color_hex(0xc0c0c0), LV_STATE_DEFAULT);

  Head_alert_info.first->btn_add = lv_btn_create(tab_timer);
  lv_obj_add_event_cb(Head_alert_info.first->btn_add, handle_add_alert_cb, LV_EVENT_SHORT_CLICKED, NULL);
  lv_obj_set_size(Head_alert_info.first->btn_add, 60, 25);
  lv_obj_set_style_bg_color(Head_alert_info.first->btn_add, lv_color_hex(0x40ac56), LV_STATE_DEFAULT);
  lv_obj_align_to(Head_alert_info.first->btn_add, Head_alert_info.first->bg_alert, LV_ALIGN_CENTER, 0, 0);

  Head_alert_info.first->label_add = lv_label_create(Head_alert_info.first->btn_add);
  lv_obj_center(Head_alert_info.first->label_add);
  lv_label_set_text_fmt(Head_alert_info.first->label_add, LV_SYMBOL_PLUS);
}

/** Wifi 输入相关 回调函数 **/
static void wifi_input_cb(lv_event_t *e) {

  /** 获取回调事件与 触发对象 **/
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);

  if (obj == textarea_wifi_name && code == LV_EVENT_SHORT_CLICKED) {

    /** 键盘关联输入框 **/
    lv_keyboard_set_textarea(keyboard_wifi, textarea_wifi_name);
    lv_obj_clear_flag(keyboard_wifi, LV_OBJ_FLAG_HIDDEN);

    /** 输入框聚焦，光标闪烁 **/
    lv_obj_add_state(textarea_wifi_name, LV_STATE_FOCUSED);
  }
  if (obj == textarea_wifi_psd && code == LV_EVENT_SHORT_CLICKED) {

    /** 键盘关联输入框 **/
    lv_keyboard_set_textarea(keyboard_wifi, textarea_wifi_psd);
    lv_obj_clear_flag(keyboard_wifi, LV_OBJ_FLAG_HIDDEN);

    /** 输入框聚焦，光标闪烁 **/
    lv_obj_add_state(textarea_wifi_psd, LV_STATE_FOCUSED);
    lv_obj_align_to(textarea_wifi_psd, keyboard_wifi, LV_ALIGN_OUT_TOP_MID, 0, 0);
  }
  if (obj == keyboard_wifi && code == LV_EVENT_READY) {

     /** 获取键盘关联对象 **/
    lv_obj_t *textarea = lv_keyboard_get_textarea(keyboard_wifi);
    if (textarea == textarea_wifi_psd) {
      lv_obj_align(textarea_wifi_psd, LV_ALIGN_CENTER, 0, 0);
    }

    /** 取消关联和聚焦，影藏键盘 **/
    lv_obj_clear_state(textarea, LV_STATE_FOCUSED);
    lv_keyboard_set_textarea(keyboard_wifi, NULL);
    lv_obj_add_flag(keyboard_wifi, LV_OBJ_FLAG_HIDDEN);
  }
}

/** wifi连接相关回调函数 **/
static void handle_wifi_cb(lv_event_t *e) {

  /** 获取回调触发对象 **/
  lv_obj_t *obj = lv_event_get_target(e);

  const char *wifi_name = lv_textarea_get_text(textarea_wifi_name);
  const char *wifi_psd = lv_textarea_get_text(textarea_wifi_psd);

  /** 连接按钮按下 **/
  if (obj == btn_wifi_connect) {

    /** 检查输入是否空 **/
    if (*wifi_name != '\0' && *wifi_psd != '\0') {

      /** 切换等待Wi-Fi连接界面 **/
      lv_obj_add_flag(label_wifi_name, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(textarea_wifi_name, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(label_wifi_psd, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(textarea_wifi_psd, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(btn_wifi_connect, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(label_wifi_connect, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(btn_clear, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(label_clear, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ckbox_remb, LV_OBJ_FLAG_HIDDEN);

      lv_obj_clear_flag(label_connect_tips, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(btn_disconnect, LV_OBJ_FLAG_HIDDEN);

      lv_label_set_text_fmt(label_connect_tips, "Waiting WiFi connect ...");

      /** 获取Wi-Fi名和密码，通知主程序开始连接Wi-Fi **/
      connect_flag = 1;
      got_wifi_name = wifi_name;
      got_wifi_psd = wifi_psd;
      start_connect_wifi = 1;

      /** 强制等待 **/
      lv_obj_add_state(btn_wifi_right, LV_STATE_DISABLED);
      lv_obj_add_state(btn_wifi_connect, LV_STATE_DISABLED);
      lv_obj_add_state(btn_disconnect, LV_STATE_DISABLED);

    } else {

      /** 连接失败提示 **/
      msgbox_wifi_warn = lv_msgbox_create(wifi_page, LV_SYMBOL_WARNING " Notice",
                                          "error wifi name or password", NULL, true);
      lv_obj_set_size(msgbox_wifi_warn, 250, 100);
      lv_obj_center(msgbox_wifi_warn);
    }
  }
  if (obj == btn_disconnect) {

    /** 取消连接标记 **/
    connect_flag = 0;

    /** 切换连接界面 **/
    lv_obj_clear_flag(label_wifi_name, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(textarea_wifi_name, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(label_wifi_psd, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(textarea_wifi_psd, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(btn_wifi_connect, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(label_wifi_connect, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(btn_clear, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(label_clear, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ckbox_remb, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(label_connect_tips, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(btn_disconnect, LV_OBJ_FLAG_HIDDEN);
  }
  if (obj == btn_clear) {

    /** 点击清空输入框 **/
    while (*lv_textarea_get_text(textarea_wifi_name) != '\0') {
      lv_textarea_del_char(textarea_wifi_name);
    }
    while (*lv_textarea_get_text(textarea_wifi_psd) != '\0') {
      lv_textarea_del_char(textarea_wifi_psd);
    }

  }
}

/** 页面切换回调函数 **/
static void page_change_cb(lv_event_t *e) {
  lv_obj_t *obj = lv_event_get_target(e);

  if (obj == btn_wifi_right) {
    lv_scr_load(home_page);
  }
  if (obj == home_left_btn) {
    lv_scr_load(wifi_page);
  }
  if (obj == btn_home_right) {
    lv_scr_load(setting_page);
  }
  if (obj == btn_setting_left) {
    lv_scr_load(home_page);
  }
}

/** 对象值变化回调函数 **/
static void obj_value_chang_cb(lv_event_t *e) {
  /** 获取回调事件与 触发对象 **/
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);

  /** 系统模式按钮切换  **/
  if (code == LV_EVENT_VALUE_CHANGED && obj == switch_auto_mode) {

    /** 自动模式处理 **/
    if (lv_obj_has_state(switch_auto_mode, LV_STATE_CHECKED)) {
      SysInfo.sys_mode = 0;
      lv_label_set_text_fmt(label_auto_mode, "Auto Mode");
      lv_obj_align_to(label_auto_mode, switch_auto_mode, LV_ALIGN_OUT_LEFT_MID, 0, 0);
      lv_obj_clear_flag(arc_air_temp, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_clear_flag(arc_air_humid, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_clear_flag(arc_air_light, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_clear_flag(arc_soil_humid, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_set_style_opa(arc_air_temp, LV_OPA_0, LV_PART_KNOB);
      lv_obj_set_style_opa(arc_air_humid, LV_OPA_0, LV_PART_KNOB);
      lv_obj_set_style_opa(arc_air_light, LV_OPA_0, LV_PART_KNOB);
      lv_obj_set_style_opa(arc_soil_humid, LV_OPA_0, LV_PART_KNOB);

      lv_obj_add_flag(switch_lamp_sta, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(switch_pump_sta, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(switch_beep_sta, LV_OBJ_FLAG_HIDDEN);

      lv_obj_clear_flag(led_lamp_sta, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(led_pump_sta, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(led_beep_sta, LV_OBJ_FLAG_HIDDEN);

    } else {

      /** 手动模式处理 **/
      SysInfo.sys_mode = 1;
      lv_label_set_text_fmt(label_auto_mode, "Manual Mode");
      lv_obj_align_to(label_auto_mode, switch_auto_mode, LV_ALIGN_OUT_LEFT_MID, 0, 0);
      lv_obj_add_flag(arc_air_temp, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_flag(arc_air_humid, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_flag(arc_air_light, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_flag(arc_soil_humid, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_set_style_opa(arc_air_temp, LV_OPA_100, LV_PART_KNOB);
      lv_obj_set_style_opa(arc_air_humid, LV_OPA_100, LV_PART_KNOB);
      lv_obj_set_style_opa(arc_air_light, LV_OPA_100, LV_PART_KNOB);
      lv_obj_set_style_opa(arc_soil_humid, LV_OPA_100, LV_PART_KNOB);

      lv_obj_clear_flag(switch_lamp_sta, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(switch_pump_sta, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(switch_beep_sta, LV_OBJ_FLAG_HIDDEN);

      lv_obj_add_flag(led_lamp_sta, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(led_pump_sta, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(led_beep_sta, LV_OBJ_FLAG_HIDDEN);
    }
  }

  /** 选项卡切换回调处理 **/
  if (obj == tabview_setting && code == LV_EVENT_VALUE_CHANGED) {
    if (lv_tabview_get_tab_act(obj) == 1) {

      lv_obj_clear_flag(btn_edit_del, LV_OBJ_FLAG_HIDDEN);

      if (isEditeThreshold) {
        lv_label_set_text_fmt(label_edit_del, "Save");
      } else lv_label_set_text_fmt(label_edit_del, "Edite");

    } else if (lv_tabview_get_tab_act(tabview_setting) == 2) {
      lv_obj_clear_flag(btn_edit_del, LV_OBJ_FLAG_HIDDEN);
      flag_editing ? lv_label_set_text_fmt(label_edit_del, "Del") : lv_label_set_text_fmt(label_edit_del, "Edite");
    } else
      lv_obj_add_flag(btn_edit_del, LV_OBJ_FLAG_HIDDEN);
  }

  /** 滑动条滑动回调 **/
  if (obj == slider_air_temp || obj == slider_soil_humid || obj == slider_air_light) {
    if (obj == slider_air_temp) {
      lv_label_set_text_fmt(label_air_temp_up_var, "%dC", lv_slider_get_value(obj));
    } else if (obj == slider_air_light) {
      lv_label_set_text_fmt(label_air_light_down_var, "%d%%", lv_slider_get_value(obj));
    } else if (obj == slider_soil_humid) {
      lv_label_set_text_fmt(label_soil_humid_down_var, "%d%%", lv_slider_get_value(obj));
    }
  }

  /** 圆弧滑动回调 **/
  if (SysInfo.sys_mode == 1 && obj == arc_air_temp) {
    lv_label_set_text_fmt(label_air_temp, "%dC", lv_arc_get_value(obj));
  } else if (SysInfo.sys_mode == 1 && obj == arc_air_light) {
    lv_label_set_text_fmt(label_air_light, "%d%%", lv_arc_get_value(obj));

  } else if (SysInfo.sys_mode == 1 && obj == arc_air_humid) {
    lv_label_set_text_fmt(label_air_humid, "%d%%", lv_arc_get_value(obj));

  } else if (SysInfo.sys_mode == 1 && obj == arc_soil_humid) {
    lv_label_set_text_fmt(label_soil_humid, "%d%%", lv_arc_get_value(obj));
  }
}

/** 闹钟添加和删除回调处理 **/
static void handle_add_alert_cb(lv_event_t *e) {
  /** 获取回调事件与 触发对象 **/
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);

  bool islighting = false;
  bool isalerting = false;
  bool ispumping = false;
  uint16_t timer_hur = 0;
  uint16_t timer_min = 0;
  uint16_t hold_min = 0;
  uint16_t hold_sec = 0;
  Alert_Body_Type *new_alert;

  const static char *str_alerting = "alerting";
  const static char *str_lighting = "lighting";
  const static char *str_pumping = "pumping";
  const char *str_disp = NULL;

  /** 点击添加闹钟 **/
  if (obj == Head_alert_info.last->btn_add) {
    lv_obj_clear_flag(bg_alert_setting, LV_OBJ_FLAG_HIDDEN);
  }

  /** 点击取消按钮 **/
  if (obj == btn_setting_cancel) {
    lv_obj_add_flag(bg_alert_setting, LV_OBJ_FLAG_HIDDEN);
  }

  /** 保存闹钟按钮 **/
  if (obj == btn_setting_save)
  {
    /** 获取滚轮闹钟时间和持续时间，复选框事件记录 **/
    isalerting = lv_obj_has_state(ckbox_exe_alerting, LV_STATE_CHECKED);
    islighting = lv_obj_has_state(ckbox_exe_lighting, LV_STATE_CHECKED);
    ispumping = lv_obj_has_state(ckbox_exe_pumping, LV_STATE_CHECKED);
    timer_hur = lv_roller_get_selected(roller_timer_hur);
    timer_min = lv_roller_get_selected(roller_timer_min);
    hold_min = lv_roller_get_selected(roller_exe_min);
    hold_sec = lv_roller_get_selected(roller_exe_sec);

    /** 限制一种事件 **/
    if (isalerting) {
      str_disp = str_alerting;
    } else if (islighting) {
      str_disp = str_lighting;
    } else {
      str_disp = str_pumping;
    }

    /** 闹钟参数正常。可以添加 **/
    if ((isalerting || islighting || ispumping) && (hold_min || hold_sec)) {

      /** 为新闹钟动态内存分配**/
      new_alert = (Alert_Body_Type *) lv_mem_alloc(sizeof(Alert_Body_Type));
      new_alert->pre = Head_alert_info.last;
      Head_alert_info.last = new_alert;

      /** 删除最后一个按钮 **/
      lv_obj_del(new_alert->pre->btn_add);
      new_alert->btn_add = NULL;
      new_alert->label_add = NULL;

      /** 添加信息 尾插法 **/
      new_alert->pre->event_alerting = isalerting;
      new_alert->pre->event_lighting = islighting;
      new_alert->pre->event_pumping = ispumping;
      new_alert->pre->islastone = false;
      new_alert->pre->next = new_alert;
      new_alert->pre->time_alert.hur = timer_hur;
      new_alert->pre->time_alert.min = timer_min;
      new_alert->pre->time_hold.min = hold_min;
      new_alert->pre->time_hold.sec = hold_sec;

      new_alert->ckbox_del = NULL;
      new_alert->event_alerting = false;
      new_alert->event_lighting = false;
      new_alert->event_pumping = false;
      new_alert->isfirstone = false;
      new_alert->islastone = true;
      new_alert->next = NULL;
      new_alert->time_alert.hur = 0;
      new_alert->time_alert.min = 0;
      new_alert->time_hold.min = 0;
      new_alert->time_hold.sec = 0;

      new_alert->pre->label_alert_content = lv_label_create(tab_timer);

      lv_label_set_text_fmt(new_alert->pre->label_alert_content, LV_SYMBOL_BELL " %d:%d will exe %s %d s", timer_hur,
                            timer_min, str_disp, hold_min * 60 + hold_sec);
      lv_obj_align_to(new_alert->pre->label_alert_content, new_alert->pre->bg_alert, LV_ALIGN_LEFT_MID, 0, 0);

      new_alert->pre->ckbox_del = lv_checkbox_create(tab_timer);
      lv_checkbox_set_text(new_alert->pre->ckbox_del, "");
      lv_obj_align_to(new_alert->pre->ckbox_del, new_alert->pre->bg_alert, LV_ALIGN_LEFT_MID, -20, 0);
      lv_obj_add_flag(new_alert->pre->ckbox_del, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_state(new_alert->pre->ckbox_del, LV_STATE_CHECKED);

      // 添加成功 改变选项卡颜色
      lv_obj_set_style_bg_color(new_alert->pre->bg_alert, lv_color_hex(0x00809c), LV_STATE_DEFAULT);

      new_alert->bg_alert = lv_obj_create(tab_timer);
      lv_obj_set_size(new_alert->bg_alert, 240, 44);
      lv_obj_align_to(new_alert->bg_alert, new_alert->pre->bg_alert, LV_ALIGN_CENTER, 0, 60);
      lv_obj_set_style_bg_color(new_alert->bg_alert, lv_color_hex(0xc0c0c0), LV_STATE_DEFAULT);

      new_alert->btn_add = lv_btn_create(tab_timer);
      lv_obj_add_event_cb(new_alert->btn_add, handle_add_alert_cb, LV_EVENT_SHORT_CLICKED, NULL);
      lv_obj_set_size(new_alert->btn_add, 60, 25);
      lv_obj_set_style_bg_color(new_alert->btn_add, lv_color_hex(0x40ac56), LV_STATE_DEFAULT);
      lv_obj_align_to(new_alert->btn_add, new_alert->bg_alert, LV_ALIGN_CENTER, 0, 0);

      new_alert->label_add = lv_label_create(new_alert->btn_add);
      lv_obj_center(new_alert->label_add);
      lv_label_set_text_fmt(new_alert->label_add, LV_SYMBOL_PLUS);

      Head_alert_info.num_alert++;
    }

    lv_obj_add_flag(bg_alert_setting, LV_OBJ_FLAG_HIDDEN);
  }

  /** 点击删除闹钟 **/
  if (obj == btn_edit_del && code == LV_EVENT_SHORT_CLICKED && lv_tabview_get_tab_act(tabview_setting) == 2 &&
      Head_alert_info.num_alert) {
    /** 进入编辑闹钟模式 **/
    if (!flag_editing) {

      /** 标记正在编辑 **/
      flag_editing = 1;

      /** 禁用左滑按钮 **/
      lv_obj_add_state(btn_setting_left, LV_STATE_DISABLED);

      /** 按钮改为删除 **/
      lv_label_set_text_fmt(label_edit_del, "Del");

      /// 影藏添加选项卡
      lv_obj_add_flag(Head_alert_info.last->bg_alert, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(Head_alert_info.last->btn_add, LV_OBJ_FLAG_HIDDEN);

      /// 便利可选中目标右移动 取消ckbox影藏
      uint16_t i;
      Alert_Body_Type *opt_del = Head_alert_info.first;

      /// 遍历闹钟，重新对齐
      for (i = 0; i < Head_alert_info.num_alert; i++) {
        if (opt_del->isfirstone == true) {
          lv_obj_align(opt_del->bg_alert, LV_ALIGN_TOP_MID, 30, 0);
        } else
          lv_obj_align_to(opt_del->bg_alert, opt_del->pre->bg_alert, LV_ALIGN_CENTER, 0, 60);

        lv_obj_clear_flag(opt_del->ckbox_del, LV_OBJ_FLAG_HIDDEN);
        lv_obj_align_to(opt_del->label_alert_content, opt_del->bg_alert, LV_ALIGN_LEFT_MID, 0, 0);

        opt_del = opt_del->next;
      }
    } else { /// 开始删除选中对象

      /// 显示 添加按钮
      lv_obj_clear_flag(Head_alert_info.last->bg_alert, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(Head_alert_info.last->btn_add, LV_OBJ_FLAG_HIDDEN);

      /// 先把对象移动回来
      uint16_t i;
      Alert_Body_Type *opt_del = Head_alert_info.first;

      /// 按钮改为删除
      lv_label_set_text_fmt(label_edit_del, "Edite");

      for (i = 0; i < Head_alert_info.num_alert; i++) {
        if (opt_del->isfirstone == true)
          lv_obj_align(opt_del->bg_alert, LV_ALIGN_TOP_MID, 0, 0);
        else
          lv_obj_align_to(opt_del->bg_alert, opt_del->pre->bg_alert, LV_ALIGN_CENTER, 0, 60);

        lv_obj_add_flag(opt_del->ckbox_del, LV_OBJ_FLAG_HIDDEN);
        lv_obj_align_to(opt_del->label_alert_content, opt_del->bg_alert, LV_ALIGN_LEFT_MID, 0, 0);

        opt_del = opt_del->next;
      }

      /// 根据复选框内容删除
      opt_del = Head_alert_info.first;
      volatile uint16_t num_exist_alert = Head_alert_info.num_alert;
      for (i = 0; i < num_exist_alert; i++) {
        if (lv_obj_has_state(opt_del->ckbox_del, LV_STATE_CHECKED)) {
          lv_obj_del(opt_del->bg_alert);
          lv_obj_del(opt_del->ckbox_del);
          lv_obj_del(opt_del->label_alert_content);
          if (opt_del->isfirstone) {
            opt_del->next->isfirstone = true;
            opt_del->next->pre = NULL;
            Head_alert_info.first = opt_del->next;
            lv_mem_free(opt_del);
          } else {
            opt_del->pre->next = opt_del->next;
            opt_del->next->pre = opt_del->pre;
            lv_mem_free(opt_del);
          }
          Head_alert_info.num_alert--;
        }
        opt_del = opt_del->next;
      }

      /// 重新对齐
      opt_del = Head_alert_info.first;
      num_exist_alert = Head_alert_info.num_alert;
      for (i = 0; i <= num_exist_alert; i++) {
        if (opt_del->isfirstone)
          lv_obj_align(opt_del->bg_alert, LV_ALIGN_TOP_MID, 0, 0);
        else
          lv_obj_align_to(opt_del->bg_alert, opt_del->pre->bg_alert, LV_ALIGN_CENTER, 0, 60);

        if (opt_del->islastone != true) {
          lv_obj_align_to(opt_del->label_alert_content, opt_del->bg_alert, LV_ALIGN_LEFT_MID, 0, 0);
          lv_obj_align_to(opt_del->ckbox_del, opt_del->bg_alert, LV_ALIGN_LEFT_MID, -20, 0);
        } else {
          lv_obj_align_to(opt_del->btn_add, opt_del->bg_alert, LV_ALIGN_CENTER, 0, 0);
        }
        opt_del = opt_del->next;
      }

      /// 恢复 tv选项卡 和 左滑按钮
      lv_obj_clear_state(btn_setting_left, LV_STATE_DISABLED);

      /// 退出编辑
      flag_editing = false;
    }
  }

  if (obj == ckbox_exe_alerting && lv_obj_has_state(obj, LV_STATE_CHECKED)) {
    lv_obj_clear_state(ckbox_exe_lighting, LV_STATE_CHECKED);
    lv_obj_clear_state(ckbox_exe_pumping, LV_STATE_CHECKED);
  }
  if (obj == ckbox_exe_lighting && lv_obj_has_state(obj, LV_STATE_CHECKED)) {
    lv_obj_clear_state(ckbox_exe_alerting, LV_STATE_CHECKED);
    lv_obj_clear_state(ckbox_exe_pumping, LV_STATE_CHECKED);
  }
  if (obj == ckbox_exe_pumping && lv_obj_has_state(obj, LV_STATE_CHECKED)) {
    lv_obj_clear_state(ckbox_exe_alerting, LV_STATE_CHECKED);
    lv_obj_clear_state(ckbox_exe_lighting, LV_STATE_CHECKED);
  }
  if (obj == btn_edit_del && lv_tabview_get_tab_act(tabview_setting) == 1) {

    if (isEditeThreshold) {
      // 关闭 阈值编辑
      isEditeThreshold = 0;
      lv_label_set_text_fmt(label_edit_del, "Edite");
      // 关闭滑动 设置颜色
      lv_obj_clear_flag(slider_air_temp, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_clear_flag(slider_air_light, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_clear_flag(slider_soil_humid, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_set_style_bg_color(slider_air_temp, lv_color_hex(0xff2000), LV_PART_KNOB);
      lv_obj_set_style_bg_color(slider_air_temp, lv_color_hex(0xff2000), LV_PART_INDICATOR);
      lv_obj_set_style_bg_color(slider_air_light, lv_color_hex(0xff2000), LV_PART_KNOB);
      lv_obj_set_style_bg_color(slider_air_light, lv_color_hex(0xff2000), LV_PART_INDICATOR);
      lv_obj_set_style_bg_color(slider_soil_humid, lv_color_hex(0xff2000), LV_PART_KNOB);
      lv_obj_set_style_bg_color(slider_soil_humid, lv_color_hex(0xff2000), LV_PART_INDICATOR);

      SysInfo.air_temp_up = lv_slider_get_value(slider_air_temp);
      SysInfo.light_down = lv_slider_get_value(slider_air_light);
      SysInfo.soil_humid_down = lv_slider_get_value(slider_soil_humid);

      // 创建保存提示
      msgbox_tips = lv_msgbox_create(setting_page, "Tips", "Saved OK!", NULL, true);
      lv_obj_set_size(msgbox_tips, 240, 100);
      lv_obj_center(msgbox_tips);
    } else {
      // 开启 阈值编辑
      isEditeThreshold = 1;
      lv_label_set_text_fmt(label_edit_del, "Save");
      // 关闭滑动 设置颜色
      lv_obj_add_flag(slider_air_temp, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_flag(slider_air_light, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_flag(slider_soil_humid, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_set_style_bg_color(slider_air_temp, lv_color_hex(0x5596db), LV_PART_KNOB);
      lv_obj_set_style_bg_color(slider_air_temp, lv_color_hex(0x5596db), LV_PART_INDICATOR);
      lv_obj_set_style_bg_color(slider_air_light, lv_color_hex(0x5596db), LV_PART_KNOB);
      lv_obj_set_style_bg_color(slider_air_light, lv_color_hex(0x5596db), LV_PART_INDICATOR);
      lv_obj_set_style_bg_color(slider_soil_humid, lv_color_hex(0x5596db), LV_PART_KNOB);
      lv_obj_set_style_bg_color(slider_soil_humid, lv_color_hex(0x5596db), LV_PART_INDICATOR);
    }
  }
}

/** 定义LVGL数据刷新回调函数 **/
void myLvglTimerTask(lv_timer_t *tmr) {
  if (wifi_connected_flag == 1) {
    /// 连接成功
    lv_label_set_text_fmt(label_connect_tips, "WiFi Connected!");
    lv_obj_clear_state(btn_wifi_right, LV_STATE_DISABLED);
    lv_obj_clear_state(btn_wifi_connect, LV_STATE_DISABLED);
    lv_obj_clear_state(btn_disconnect, LV_STATE_DISABLED);

    wifi_connected_flag = 3;
  } else if (wifi_connected_flag == 2) /// 暂时定义 wifi 连接失败
  {
    wifi_connected_flag = 0;
    lv_obj_clear_flag(label_wifi_name, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(textarea_wifi_name, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(label_wifi_psd, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(textarea_wifi_psd, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(btn_wifi_connect, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(label_wifi_connect, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(btn_clear, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(label_clear, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ckbox_remb, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(label_connect_tips, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(btn_disconnect, LV_OBJ_FLAG_HIDDEN);

    lv_obj_clear_state(btn_wifi_connect, LV_STATE_DISABLED);
    lv_obj_clear_state(btn_wifi_right, LV_STATE_DISABLED);
    lv_obj_clear_state(btn_disconnect, LV_STATE_DISABLED);

    msgbox_wifi_warn = lv_msgbox_create(wifi_page, LV_SYMBOL_WARNING " Notice",
                                        "WiFi connect error!", NULL, true);
    lv_obj_set_size(msgbox_wifi_warn, 250, 100); /* 设置大小 */
    lv_obj_center(msgbox_wifi_warn);             /* 设置位置 */
  }
  if (LvglDataUpdateFlag && SysInfo.sys_mode == 0) {
    LvglDataUpdateFlag = 0;
    /// 数据刷新
    lv_label_set_text_fmt(label_air_temp, "%dC", SysInfo.air_temp);
    lv_arc_set_value(arc_air_temp, SysInfo.air_temp);

    lv_label_set_text_fmt(label_air_humid, "%d%%", SysInfo.air_humid);
    lv_arc_set_value(arc_air_humid, SysInfo.air_humid);


    lv_label_set_text_fmt(label_air_light, "%d%%", SysInfo.air_light);
    lv_arc_set_value(arc_air_light, SysInfo.air_light);

    lv_label_set_text_fmt(label_soil_humid, "%d%%", SysInfo.soil_humid);
    lv_arc_set_value(arc_soil_humid, SysInfo.soil_humid);


    /// 执行状态跟新
    SysInfo.sta_bulb ? lv_led_set_color(led_lamp_sta, lv_color_hex(0x00ff00)) : lv_led_set_color(led_lamp_sta,
                                                                                                 lv_color_hex(
                                                                                                         0x2b2b2b));
    SysInfo.sta_pump ? lv_led_set_color(led_pump_sta, lv_color_hex(0x00ff00)) : lv_led_set_color(led_pump_sta,
                                                                                                 lv_color_hex(
                                                                                                         0x2b2b2b));
    SysInfo.sta_beep ? lv_led_set_color(led_beep_sta, lv_color_hex(0x00ff00)) : lv_led_set_color(led_beep_sta,
                                                                                                 lv_color_hex(
                                                                                                         0x2b2b2b));
    SysInfo.sta_bulb ? lv_label_set_text_fmt(label_lamp_describe, "This device is working") : lv_label_set_text_fmt(
            label_lamp_describe, "This device is not working");
    SysInfo.sta_pump ? lv_label_set_text_fmt(label_pump_describe, "This device is working") : lv_label_set_text_fmt(
            label_pump_describe, "This device is not working");
    SysInfo.sta_beep ? lv_label_set_text_fmt(label_beep_describe, "This device is working") : lv_label_set_text_fmt(
            label_beep_describe, "This device is not working");

    /// 查看是否 更新阈值
    if (!isEditeThreshold) {
      lv_label_set_text_fmt(label_air_temp_up_var, "%dC", SysInfo.air_temp_up);
      lv_label_set_text_fmt(label_air_light_down_var, "%d%%", SysInfo.light_down);
      lv_label_set_text_fmt(label_soil_humid_down_var, "%d%%", SysInfo.soil_humid_down);

      lv_slider_set_value(slider_air_temp, SysInfo.air_temp_up, LV_ANIM_ON);
      lv_slider_set_value(slider_air_light, SysInfo.light_down, LV_ANIM_ON);
      lv_slider_set_value(slider_soil_humid, SysInfo.soil_humid_down, LV_ANIM_ON);
    }
  }

  /// 时间更新
  if (IsTimeUpdate) {
    IsTimeUpdate = 0;
    lv_label_set_text_fmt(label_time, "%d%d : %d%d : %d%d",
                          SysInfo.hur / 10, SysInfo.hur % 10,
                          SysInfo.min / 10, SysInfo.min % 10,
                          SysInfo.sec / 10, SysInfo.sec % 10);
  }

  /// 手动模式 且 不能网络控制时
  if(SysInfo.sys_mode == 1 && IsIoTControl == 0)
  {
    SysInfo.sta_beep = lv_obj_has_state(switch_beep_sta, LV_STATE_CHECKED);
    SysInfo.sta_pump = lv_obj_has_state(switch_pump_sta, LV_STATE_CHECKED);
    SysInfo.sta_bulb = lv_obj_has_state(switch_lamp_sta, LV_STATE_CHECKED);
    SysInfo.sta_beep ? lv_label_set_text_fmt(label_beep_describe, "This device is working") : \
                        lv_label_set_text_fmt(label_beep_describe, "This device is not working");
    SysInfo.sta_bulb ? lv_label_set_text_fmt(label_lamp_describe, "This device is working") : \
                        lv_label_set_text_fmt(label_lamp_describe, "This device is not working");
    SysInfo.sta_pump ? lv_label_set_text_fmt(label_pump_describe, "This device is working") : \
                        lv_label_set_text_fmt(label_pump_describe, "This device is not working");
  }
}
