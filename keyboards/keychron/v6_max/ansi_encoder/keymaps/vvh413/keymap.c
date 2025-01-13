/* Copyright 2024 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <string.h>
#include "color.h"
#include "eeprom.h"
#include "keycodes.h"
#include "matrix.h"
#include "rgb_matrix.h"
#include "timer.h"
#include "via.h"
#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "config.h"

enum layers {
    WIN_BASE,
    WIN_FN,
    MAC_BASE,
    MAC_FN,
};

enum custom_keycodes {
    BUF_P_0 = SAFE_RANGE,
    BUF_P_1,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_BASE] = LAYOUT_ansi_109(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,     KC_WSTP,    KC_PSCR,  RGB_TOG,  KC_F20,   MC_0,     MC_1,     MC_2,     MC_3,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,     KC_BSPC,    KC_INS,   KC_HOME,  KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,    KC_BSLS,    KC_DEL,   KC_END,   KC_PGDN,  KC_P7,    KC_P8,    KC_P9,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,                                   KC_P4,    KC_P5,    KC_P6,    KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,              KC_UP,              KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RWIN,  MO(WIN_FN), KC_RCTL,    KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT,  KC_PENT),
    [WIN_FN] = LAYOUT_ansi_109(
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,    KC_MUTE,    _______,  _______,  _______,  MC_4,     MC_5,     MC_6,     MC_7,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,                                  _______,  _______,  _______,  _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,              _______,            _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,            _______,  _______),
    [MAC_BASE] = LAYOUT_ansi_109(
        KC_ESC,   KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,    KC_MUTE,    KC_PSCR,  RGB_TOG,  KC_F20,   MC_8,     MC_9,     BUF_P_0,  BUF_P_1,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,     KC_BSPC,    KC_INS,   KC_HOME,  KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,    KC_BSLS,    KC_DEL,   KC_END,   KC_PGDN,  MC_17,    MC_18,    MC_19,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,                                   MC_14,    MC_15,    MC_16,    KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,              KC_UP,              MC_11,    MC_12,    MC_13,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RWIN,  TG(MAC_FN), KC_RCTL,    KC_LEFT,  KC_DOWN,  KC_RGHT,  MC_10,              KC_PDOT,  KC_PENT),
    [MAC_FN] = LAYOUT_ansi_109(
        DM_RSTP,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,     RGB_TOG,    _______,  _______,  _______,  DM_REC1,  DM_REC2,  DM_PLY1,  DM_PLY2,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    _______,  _______,  _______,  KC_P7,    KC_P8,    KC_P9,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,                                  KC_P4,    KC_P5,    KC_P6,    _______,
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,              _______,            KC_P1,    KC_P2,    KC_P3,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,    _______,    _______,  _______,  _______,  KC_P0,              _______,  _______),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [WIN_BASE] = {ENCODER_CCW_CW(KC_WBAK, KC_WFWD)},
    [WIN_FN]   = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_FN]   = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
};
#endif // ENCODER_MAP_ENABLE

typedef struct {
    bool     init;
    HSV      layer_colors[4];
    uint8_t buffer_delay;
} custom_config_t;
custom_config_t custom_config;

const custom_config_t DEFAULT_CUSTOM_CONFIG = {
    .init         = false,
    .layer_colors = {
        [WIN_BASE] = {HSV_MAGENTA},
        [WIN_FN]   = {HSV_GREEN},
        [MAC_BASE] = {HSV_CYAN},
        [MAC_FN]   = {HSV_CORAL},
    },
    .buffer_delay = 10,
};

// clang-format on
enum via_layer_color_value {
    id_layer_color        = 1,
    id_reset_layer_colors = 2,
    id_test               = 3,
    id_mic_state          = 4,
    id_buffer             = 5,
    id_buffer_delay       = 6,
};

bool     show_brightness  = false;
uint16_t brightness_timer = 0;

bool     show_test  = false;
uint8_t  test_idx   = 0;
uint16_t test_timer = 0;

bool mic_indicator = false;
#define MIC_LED_ID 15

#define BUF_SIZE 1024
char buffer[2][BUF_SIZE] = {{0}, {0}};

void keyboard_post_init_user(void) {
    eeprom_read_block(&custom_config, ((void *)VIA_EEPROM_CUSTOM_CONFIG_ADDR), sizeof(custom_config_t));

    if (!custom_config.init) {
        custom_config.init = true;
        memcpy(&custom_config, &DEFAULT_CUSTOM_CONFIG, sizeof(DEFAULT_CUSTOM_CONFIG));
        eeprom_update_block(&custom_config, ((void *)VIA_EEPROM_CUSTOM_CONFIG_ADDR), sizeof(custom_config_t));
    }
}

void brightness_turn_on(void) {
    if (!show_brightness) {
        show_brightness = true;
    }
    brightness_timer = timer_read();
}

void brightness_turn_off(void) {
    if (show_brightness && timer_elapsed(brightness_timer) > 1000) {
        show_brightness = false;
    }
}

void test_turn_on(uint8_t index) {
    if (!show_test) {
        show_test = true;
    }
    test_idx   = index;
    test_timer = timer_read();
}

void test_turn_off(void) {
    if (show_test && timer_elapsed(test_timer) > 1000) {
        show_test = false;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }
    switch (keycode) {
        case RGB_TOG:
        case RGB_VAD:
        case RGB_VAI: {
            if (record->event.pressed) {
                brightness_turn_on();
            }
            break;
        }
        case KC_F20: {
            if (record->event.pressed) {
                mic_indicator = !mic_indicator;
            }
            break;
        }
        case BUF_P_0:
        case BUF_P_1: {
            if (record->event.pressed) {
                SEND_STRING_DELAY(buffer[keycode - BUF_P_0], custom_config.buffer_delay);
            }
            break;
        }
    }
    return true;
}

void matrix_scan_user() {
    brightness_turn_off();
    test_turn_off();
}

void set_layer_color(uint16_t layer, uint8_t led_min, uint8_t led_max) {
    HSV hsv = custom_config.layer_colors[layer];
    if (hsv.v > rgb_matrix_get_val()) {
        hsv.v = rgb_matrix_get_val();
    }

    RGB rgb = hsv_to_rgb(hsv);
    for (uint8_t i = led_min; i < led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x01)) { // 0x01 == LED_FLAG_MODIFIER
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    set_layer_color(get_highest_layer(layer_state | default_layer_state), led_min, led_max);

    if (show_brightness) {
        uint8_t idx = (rgb_matrix_get_val() - 31) / 16;
        RGB_MATRIX_INDICATOR_SET_COLOR(idx, 255, 0, 0);
    }
    if (show_test) {
        RGB_MATRIX_INDICATOR_SET_COLOR(test_idx, 0, 0, 255);
    }
    if (mic_indicator) {
        RGB_MATRIX_INDICATOR_SET_COLOR(MIC_LED_ID, 255, 0, 0);
    }
    return false;
}

void custom_set_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch (*value_id) {
        case id_layer_color: {
            uint8_t index = value_data[0];
            if (index >= 0 && index < 4) {
                custom_config.layer_colors[index].h = value_data[1];
                custom_config.layer_colors[index].s = value_data[2];
            }
            break;
        }
        case id_reset_layer_colors: {
            memcpy(custom_config.layer_colors, DEFAULT_CUSTOM_CONFIG.layer_colors, sizeof(DEFAULT_CUSTOM_CONFIG.layer_colors));
            break;
        }
        case id_test: {
            test_turn_on(value_data[0]);
            break;
        }
        case id_mic_state: {
            mic_indicator = value_data[0];
            break;
        }
        case id_buffer: {
            uint16_t offset = (value_data[0] << 8) | value_data[1];
            uint8_t  size   = value_data[2]; // size <= 27
            if (size <= 27 && offset + size <= BUF_SIZE * 2) {
                memcpy(&buffer[offset / BUF_SIZE][offset % BUF_SIZE], &value_data[3], size);
            }
            break;
        }
        case id_buffer_delay: {
            custom_config.buffer_delay = value_data[0];
            break;
        }
    }
}

void custom_get_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch (*value_id) {
        case id_layer_color: {
            uint8_t index = value_data[0];
            if (index >= 0 && index < 4) {
                value_data[1] = custom_config.layer_colors[index].h;
                value_data[2] = custom_config.layer_colors[index].s;
            }
            break;
        }
        case id_mic_state: {
            value_data[0] = mic_indicator;
            break;
        }
        case id_buffer: {
            uint16_t offset = (value_data[0] << 8) | value_data[1];
            uint8_t  size   = value_data[2];
            if (size <= 26 && offset + size <= BUF_SIZE * 2) {
                memcpy(&value_data[3], &buffer[offset / BUF_SIZE][offset % BUF_SIZE], size);
            }
            break;
        }
        case id_buffer_delay: {
            value_data[0] = custom_config.buffer_delay;
            break;
        }
    }
}

void custom_save(void) {
    eeprom_update_block(&custom_config, ((void *)VIA_EEPROM_CUSTOM_CONFIG_ADDR), sizeof(custom_config_t));
}

void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id        = &(data[0]);
    uint8_t *channel_id        = &(data[1]);
    uint8_t *value_id_and_data = &(data[2]);

    if (*channel_id == id_custom_channel) {
        switch (*command_id) {
            case id_custom_set_value: {
                custom_set_value(value_id_and_data);
                break;
            }
            case id_custom_get_value: {
                custom_get_value(value_id_and_data);
                break;
            }
            case id_custom_save: {
                custom_save();
                break;
            }
            default: {
                // Unhandled message.
                *command_id = id_unhandled;
                break;
            }
        }
        return;
    }
    // Return the unhandled state
    *command_id = id_unhandled;
}
