//
/// Created by Zhou 2024/1/30
//

#ifndef __GUI_H
#define __GUI_H

#ifdef __cplusplus
extern "C" {
#endif

void gui_init(void);


struct __alert_time {
    uint16_t hur;
    uint16_t min;
};
struct __time_hold {
    uint16_t min;
    uint16_t sec;
};

struct __alert_body {
    bool islastone;
    bool isfirstone;
    lv_obj_t *bg_alert;
    lv_obj_t *label_alert_content;
    lv_obj_t *ckbox_del;
    lv_obj_t *btn_add;
    lv_obj_t *label_add;
    struct __alert_time time_alert;
    struct __time_hold time_hold;
    bool event_pumping;
    bool event_lighting;
    bool event_alerting;
    struct __alert_body *next;
    struct __alert_body *pre;
};

struct __Head_alert_Info {
    uint8_t num_alert;
    struct __alert_body *first;
    struct __alert_body *last;

};

typedef struct __alert_body Alert_Body_Type;

#ifdef __cplusplus
}
#endif

#endif /* __GUI_H */

