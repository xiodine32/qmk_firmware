#include QMK_KEYBOARD_H

#define SM_LSFT OSM(MOD_LSFT)
#define SM_LCTL OSM(MOD_LCTL)
#define SM_LALT OSM(MOD_LALT)
#define SM_LGUI OSM(MOD_LGUI)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,   MO(1),  KC_SPC,     KC_ENT,   MO(2), KC_RALT
                                      //`--------------------------'  `--------------------------'

  ),

    [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, XXXXXXX,   KC_P7,   KC_P8,   KC_P9, KC_PPLS,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, XXXXXXX,   KC_P4,   KC_P5,   KC_P6, KC_PCMM,                      KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, XXXXXXX, CW_TOGG,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,   KC_P0,   KC_P1,   KC_P2,   KC_P3, KC_PEQL,                      KC_HOME, KC_PGDN, KC_PGUP,  KC_END,  KC_INS,  KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT,   MO(3), KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

    [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, KC_PLUS, KC_AMPR, KC_ASTR, KC_LPRN, KC_LPRN,                      KC_RPRN, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, KC_UNDS,  KC_DLR, KC_PERC, KC_CIRC, KC_LBRC,                      KC_RBRC, KC_BSLS,  KC_GRV, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_RPRN, KC_EXLM,   KC_AT, KC_HASH, KC_LCBR,                      KC_RCBR, KC_PIPE, KC_TILD, XXXXXXX, XXXXXXX, KC_BSLS,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,   MO(3),  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

    [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                        KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, SM_LGUI, SM_LALT, SM_LCTL, SM_LSFT, XXXXXXX,                      KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY, KC_CAPS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_PSCR, KC_SCRL, KC_PAUS, RGB_TOG, RGB_MOD,                      RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI,  KC_DEL, QK_BOOT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  )
};
// clang-format on

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
static void render_layer_state(void) {
    switch (get_highest_layer(layer_state | default_layer_state)) {
        case 0:
            oled_write_P(PSTR("BASIC"), false);
            break;
        case 1:
            oled_write_P(PSTR("NUMBR"), false);
            break;
        case 2:
            oled_write_P(PSTR("SYMBL"), false);
            break;
        case 3:
            oled_write_P(PSTR("FUNCT"), false);
            break;
        default:
            oled_write_P(PSTR("?????"), false);
            break;
    }
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
