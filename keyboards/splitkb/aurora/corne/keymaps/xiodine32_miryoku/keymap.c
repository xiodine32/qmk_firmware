// Copyright 2022 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#include QMK_KEYBOARD_H

#define LAYOUT_miryoku(K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, N30, N31, K32, K33, K34, K35, K36, K37, N38, N39) LAYOUT_split_3x6_3(KC_NO, K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, KC_NO, KC_NO, K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, KC_NO, KC_NO, K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, KC_NO, K32, K33, K34, K35, K36, K37)

#include "manna-harbour_miryoku.h"

// Additional Features double tap guard

enum {
    U_TD_BOOT,
#define MIRYOKU_X(LAYER, STRING) U_TD_U_##LAYER,
    MIRYOKU_LAYER_LIST
#undef MIRYOKU_X
};

void u_td_fn_boot(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) {
        reset_keyboard();
    }
}

#define MIRYOKU_X(LAYER, STRING)                                        \
    void u_td_fn_U_##LAYER(tap_dance_state_t *state, void *user_data) { \
        if (state->count == 2) {                                        \
            default_layer_set((layer_state_t)1 << U_##LAYER);           \
        }                                                               \
    }
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X

tap_dance_action_t tap_dance_actions[] = {[U_TD_BOOT] = ACTION_TAP_DANCE_FN(u_td_fn_boot),
#define MIRYOKU_X(LAYER, STRING) [U_TD_U_##LAYER] = ACTION_TAP_DANCE_FN(u_td_fn_U_##LAYER),
                                          MIRYOKU_LAYER_LIST
#undef MIRYOKU_X
};

// keymap

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
#define MIRYOKU_X(LAYER, STRING) [U_##LAYER] = U_MACRO_VA_ARGS(MIRYOKU_LAYERMAPPING_##LAYER, MIRYOKU_LAYER_##LAYER),
    MIRYOKU_LAYER_LIST
#undef MIRYOKU_X
};

// shift functions

const key_override_t capsword_key_override = ko_make_basic(MOD_MASK_SHIFT, CW_TOGG, KC_CAPS);

const key_override_t **key_overrides = (const key_override_t *[]){&capsword_key_override, NULL};

// thumb combos

#if defined(MIRYOKU_KLUDGE_THUMBCOMBOS)
const uint16_t PROGMEM thumbcombos_base_right[] = {LT(U_SYM, KC_ENT), LT(U_NUM, KC_BSPC), COMBO_END};
const uint16_t PROGMEM thumbcombos_base_left[]  = {LT(U_NAV, KC_SPC), LT(U_MOUSE, KC_TAB), COMBO_END};
const uint16_t PROGMEM thumbcombos_nav[]        = {KC_ENT, KC_BSPC, COMBO_END};
const uint16_t PROGMEM thumbcombos_mouse[]      = {KC_BTN2, KC_BTN1, COMBO_END};
const uint16_t PROGMEM thumbcombos_media[]      = {KC_MSTP, KC_MPLY, COMBO_END};
const uint16_t PROGMEM thumbcombos_num[]        = {KC_0, KC_MINS, COMBO_END};
#    if defined(MIRYOKU_LAYERS_FLIP)
const uint16_t PROGMEM thumbcombos_sym[] = {KC_UNDS, KC_LPRN, COMBO_END};
#    else
const uint16_t PROGMEM thumbcombos_sym[] = {KC_RPRN, KC_UNDS, COMBO_END};
#    endif
const uint16_t PROGMEM thumbcombos_fun[]       = {KC_SPC, KC_TAB, COMBO_END};
combo_t                key_combos[COMBO_COUNT] = {COMBO(thumbcombos_base_right, LT(U_FUN, KC_DEL)),
                                                  COMBO(thumbcombos_base_left, LT(U_MEDIA, KC_ESC)),
                                                  COMBO(thumbcombos_nav, KC_DEL),
                                                  COMBO(thumbcombos_mouse, KC_BTN3),
                                                  COMBO(thumbcombos_media, KC_MUTE),
                                                  COMBO(thumbcombos_num, KC_DOT),
#    if defined(MIRYOKU_LAYERS_FLIP)
                                   COMBO(thumbcombos_sym, KC_RPRN),
#    else
                                   COMBO(thumbcombos_sym, KC_LPRN),
#    endif
                                                  COMBO(thumbcombos_fun, KC_APP)};
#endif

void keyboard_pre_init_user(void) {
    // Set our LED pin as output
    setPinOutput(24);
    // Turn the LED off
    // (Due to technical reasons, high is off and low is on)
    writePinHigh(24);
}

static void render_kb_LED_state(void) {
    // Host Keyboard LED Status
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_P(led_usb_state.num_lock ? PSTR("N ") : PSTR("  "), false);
    oled_write_P(led_usb_state.caps_lock ? PSTR("C ") : is_caps_word_on() ? PSTR("CW") : PSTR("  "), false);
    oled_write_P(led_usb_state.scroll_lock ? PSTR("S ") : PSTR("  "), false);
}

static void render_mod_status_gui_alt(uint8_t modifiers) {
    static const char PROGMEM gui_off_1[] = {0x85, 0x86, 0};
    static const char PROGMEM gui_off_2[] = {0xa5, 0xa6, 0};
    static const char PROGMEM gui_on_1[]  = {0x8d, 0x8e, 0};
    static const char PROGMEM gui_on_2[]  = {0xad, 0xae, 0};

    static const char PROGMEM alt_off_1[] = {0x87, 0x88, 0};
    static const char PROGMEM alt_off_2[] = {0xa7, 0xa8, 0};
    static const char PROGMEM alt_on_1[]  = {0x8f, 0x90, 0};
    static const char PROGMEM alt_on_2[]  = {0xaf, 0xb0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[]  = {0xc7, 0};
    static const char PROGMEM on_off_2[]  = {0xc8, 0};
    static const char PROGMEM off_on_1[]  = {0xc9, 0};
    static const char PROGMEM off_on_2[]  = {0xca, 0};
    static const char PROGMEM on_on_1[]   = {0xcb, 0};
    static const char PROGMEM on_on_2[]   = {0xcc, 0};

    if (modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_1, false);
    } else {
        oled_write_P(gui_off_1, false);
    }

    if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
        oled_write_P(on_on_1, false);
    } else if (modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_1, false);
    } else if (modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if (modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_1, false);
    } else {
        oled_write_P(alt_off_1, false);
    }

    if (modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_2, false);
    } else {
        oled_write_P(gui_off_2, false);
    }

    if (modifiers & MOD_MASK_GUI & MOD_MASK_ALT) {
        oled_write_P(on_on_2, false);
    } else if (modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_2, false);
    } else if (modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if (modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_2, false);
    } else {
        oled_write_P(alt_off_2, false);
    }
}

static void render_mod_status_ctrl_shift(uint8_t modifiers) {
    static const char PROGMEM ctrl_off_1[] = {0x89, 0x8a, 0};
    static const char PROGMEM ctrl_off_2[] = {0xa9, 0xaa, 0};
    static const char PROGMEM ctrl_on_1[]  = {0x91, 0x92, 0};
    static const char PROGMEM ctrl_on_2[]  = {0xb1, 0xb2, 0};

    static const char PROGMEM shift_off_1[] = {0x8b, 0x8c, 0};
    static const char PROGMEM shift_off_2[] = {0xab, 0xac, 0};
    static const char PROGMEM shift_on_1[]  = {0xcd, 0xce, 0};
    static const char PROGMEM shift_on_2[]  = {0xcf, 0xd0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[]  = {0xc7, 0};
    static const char PROGMEM on_off_2[]  = {0xc8, 0};
    static const char PROGMEM off_on_1[]  = {0xc9, 0};
    static const char PROGMEM off_on_2[]  = {0xca, 0};
    static const char PROGMEM on_on_1[]   = {0xcb, 0};
    static const char PROGMEM on_on_2[]   = {0xcc, 0};

    if (modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_1, false);
    } else {
        oled_write_P(ctrl_off_1, false);
    }

    if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
        oled_write_P(on_on_1, false);
    } else if (modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_1, false);
    } else if (modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if (modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_1, false);
    } else {
        oled_write_P(shift_off_1, false);
    }

    if (modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_2, false);
    } else {
        oled_write_P(ctrl_off_2, false);
    }

    if (modifiers & MOD_MASK_CTRL & MOD_MASK_SHIFT) {
        oled_write_P(on_on_2, false);
    } else if (modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_2, false);
    } else if (modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if (modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_2, false);
    } else {
        oled_write_P(shift_off_2, false);
    }
}
static void render_layer_state(void) {
    switch (get_highest_layer(layer_state)) {
        case U_BASE:
            oled_write_P(PSTR("BASIC"), false);
            break;
        case U_EXTRA:
            oled_write_P(PSTR("EXTRA"), false);
            break;
        case U_TAP:
            oled_write_P(PSTR("EXTRA"), false);
            break;
        case U_BUTTON:
            oled_write_P(PSTR("BUTTN"), false);
            break;
        case U_NAV:
            oled_write_P(PSTR(" NAV "), false);
            break;
        case U_MOUSE:
            oled_write_P(PSTR("MOUSE"), false);
            break;
        case U_MEDIA:
            oled_write_P(PSTR("MEDIA"), false);
            break;
        case U_NUM:
            oled_write_P(PSTR(" NUM "), false);
            break;
        case U_SYM:
            oled_write_P(PSTR(" SYM "), false);
            break;
        case U_FUN:
            oled_write_P(PSTR(" FUN "), false);
            break;
        default: //  for any other layers, or the default layer
            oled_write_P(PSTR("     "), false);
            break;
    }
}
layer_state_t layer_state_set_user(layer_state_t state) {
    layer_state       = state;
    int current_value = rgblight_get_val();
    int current_sat   = rgblight_get_sat();
    switch (get_highest_layer(layer_state)) {
        case U_BASE:
            rgb_matrix_sethsv(0x55, current_sat, current_value);
            break;
        case U_EXTRA:
            rgb_matrix_sethsv(0x20, current_sat, current_value);
            break;
        case U_TAP:
            rgb_matrix_sethsv(0x0, current_sat, current_value);
            break;
        case U_BUTTON:
            rgb_matrix_sethsv(0x60, current_sat, current_value);
            break;
        case U_NAV:
            rgb_matrix_sethsv(0x80, current_sat, current_value);
            break;
        case U_MOUSE:
            rgb_matrix_sethsv(0xa0, current_sat, current_value);
            break;
        case U_MEDIA:
            rgb_matrix_sethsv(0xc0, current_sat, current_value);
            break;
        case U_NUM:
            rgb_matrix_sethsv(0xe0, current_sat, current_value);
            break;
        case U_SYM:
            rgb_matrix_sethsv(0x80, current_sat, current_value);
            break;
        case U_FUN:
            rgb_matrix_sethsv(0x90, current_sat, current_value);
            break;
        default: //  for any other layers, or the default layer
            rgb_matrix_sethsv(0xA0, current_sat, current_value);
            break;
    }
    return state;
}

bool oled_task_user(void) {
    oled_set_brightness(1);
    if (is_keyboard_master()) {
        static char text[6];
        snprintf(text, sizeof(text), " %03d ", rgb_matrix_get_mode());
        oled_write_P(text, false);
        oled_write_P(PSTR(" xio "), false);
        oled_write_P(PSTR("     "), false);
        render_layer_state();
        oled_write_P(PSTR("     "), false);
        render_mod_status_gui_alt(get_mods() | get_oneshot_mods());
        render_mod_status_ctrl_shift(get_mods() | get_oneshot_mods());
        render_kb_LED_state();
    } else {
        // clang-format off
        static const char PROGMEM xio_art[] = {
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,254, 62, 30, 30, 30, 30,158,158,222, 94, 94, 94, 94, 94, 94,222,158,158, 30, 30, 30, 30, 62,254,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,240,252,254,255,195,193,192,192,192,192,192,192,192,192,193,195,255,254,252,240,  0,  0,  0,  0,  0,  0,
            0,  0,  0,192,  0,  0,  7, 63,127,255,195,131,  3,  3,  3,  3,  3,  3,  3,  3,131,195,255,127, 63, 15,  0,  0,192,  0,  0,  0,  0,  0,  0,127,126,124,120,120,120,120,121,121,123,122,122,122,122,122,122,123,123,121,120,120,120,120,124,126,127,  0,  0,  0,254,252,248,248,240,240,224,224,224,224,224,192,192,192,192,192,192,192,192,192,192,224,224,224,224,192,176,224, 64,144,136, 96,127, 79, 63, 39, 79,159, 51, 39,207, 27, 71, 19,155,103, 39,155,247,103,155,247,231,155,231,215,155,107,
            63,103,223,191,190,254,129, 36,  1,  8,130,  0, 96,  2,  8, 32,  2,136,145,132,131,139, 14, 63, 63,127,255,255,255,127,255,254,253,241, 78, 25,177,198,248,224,129,  0,  8, 38,  0, 64,252,254,251,249,128,  0,  0,  0,  0,  1,  0,224,  0,  0,  9,  0,  0,227,255,249,254,243, 68,  0,255,253,237,146, 98,200,  1,160,129,  3,131,129,  0,128,128,128,192,192,208,224,224,242,248,254,255,255,255,255,127, 63, 31,  4,  3,  1,  7,  7, 14,155,142, 52, 29,217, 62,254,231,159, 31, 31,199, 15, 15, 15, 15, 15, 15,  7,  7,  3,
            1,  4,  8,144, 32,136,248,249,188,154, 22,111,143, 17, 55,  4, 15, 39, 31, 25, 30, 63, 63, 63,127,254,255,253,254,254,249,242,217,228,178,128,  0,  3,255,191, 55,  4,  9,194,  0,  0,240,248,252,240, 64,  0,  0,  0,  0,  0, 34,192,  0,  3, 39, 15, 63,255,255,255,255,255, 31,  0, 64,  4, 32,  0,  2, 12, 48,  1, 67, 15,143, 27, 16, 48,  0,144,160,160,136,224,240,248,242,224,248,253,255, 63,103, 72, 27,132,176,230,136,160, 33,  8,224, 33,  8,194, 16,132, 41,105,198, 31,255,255,127,127,255,127,255,191,
            63,207,177,115,204,156,115,204,
         };
        // clang-format on
        oled_write_raw_P(xio_art, sizeof(xio_art));
        oled_set_cursor(0, 0);
        oled_write_P(PSTR(" WPM "), false);
        static char wpm_str[6];
        sprintf(wpm_str, " %03d ", get_current_wpm());
        oled_write(wpm_str, false);
    }
    return false;
}
