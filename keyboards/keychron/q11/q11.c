/* Copyright 2022 @ Keychron (https://www.keychron.com)
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

#include "quantum.h"

// Mask out handedness diode to prevent it
// from keeping the keyboard awake
// - just mirroring `KC_NO` in the `LAYOUT`
//   macro to keep it simple
const matrix_row_t matrix_mask[] = {
    0b011111111, 0b011111111, 0b011111111, 0b001111111, 0b011111101, 0b001011111, 0b111111111, 0b101111111, 0b111111111, 0b110111111, 0b010111111, 0b111011110,
};

#ifdef DIP_SWITCH_ENABLE
bool dip_switch_update_kb(uint8_t index, bool active) {
    if (!dip_switch_update_user(index, active)) {
        return false;
    }
    if (index == 0) {
        default_layer_set(1UL << (active ? 0 : 2));
    }
    return true;
}
#endif

#define ADC_BUFFER_DEPTH 1
#define ADC_NUM_CHANNELS 1
#define ADC_SAMPLING_RATE ADC_SMPR_SMP_12P5
#define ADC_RESOLUTION ADC_CFGR_RES_10BITS

static int16_t analogReadPin_my(pin_t pin) {
    ADCConfig          adcCfg = {};
    adcsample_t        sampleBuffer[ADC_NUM_CHANNELS * ADC_BUFFER_DEPTH];
    ADCDriver         *targetDriver       = &ADCD1;
    ADCConversionGroup adcConversionGroup = {
        .circular     = FALSE,
        .num_channels = (uint16_t)(ADC_NUM_CHANNELS),
        .cfgr         = ADC_RESOLUTION,
    };

    palSetLineMode(pin, PAL_MODE_INPUT_ANALOG);
    switch (pin) {
        case B0:
            adcConversionGroup.smpr[2] = ADC_SMPR2_SMP_AN15(ADC_SAMPLING_RATE);
            adcConversionGroup.sqr[0]  = ADC_SQR1_SQ1_N(ADC_CHANNEL_IN15);
            sampleBuffer[0]            = 0;
            break;
        case B1:
            adcConversionGroup.smpr[2] = ADC_SMPR2_SMP_AN16(ADC_SAMPLING_RATE);
            adcConversionGroup.sqr[0]  = ADC_SQR1_SQ1_N(ADC_CHANNEL_IN16);
            sampleBuffer[0]            = 0;
            break;
        default:
            return 0;
    }
    adcStart(targetDriver, &adcCfg);
    if (adcConvert(targetDriver, &adcConversionGroup, &sampleBuffer[0], ADC_BUFFER_DEPTH) != MSG_OK) {
        return 0;
    }

    return *sampleBuffer;
}

#if defined(ENCODER_ENABLE) && defined(PAL_USE_CALLBACKS)

void encoder_pad_cb(void *param) {
    encoder_inerrupt_read((uint32_t)param & 0XFF);
}

void encoder_interrupt_init(void) {
    if (is_keyboard_left()) {
        pin_t encoders_pad_a_left[NUM_ENCODERS_LEFT] = ENCODERS_PAD_A;
        pin_t encoders_pad_b_left[NUM_ENCODERS_LEFT] = ENCODERS_PAD_B;
        for (uint32_t i = 0; i < NUM_ENCODERS_LEFT; i++) {
            palEnableLineEvent(encoders_pad_a_left[i], PAL_EVENT_MODE_BOTH_EDGES);
            palEnableLineEvent(encoders_pad_b_left[i], PAL_EVENT_MODE_BOTH_EDGES);
            palSetLineCallback(encoders_pad_a_left[i], encoder_pad_cb, (void *)i);
            palSetLineCallback(encoders_pad_b_left[i], encoder_pad_cb, (void *)i);
        }/*  */
    } else {
        pin_t encoders_pad_a_right[NUM_ENCODERS_RIGHT] = ENCODERS_PAD_A_RIGHT;
        pin_t encoders_pad_b_right[NUM_ENCODERS_RIGHT] = ENCODERS_PAD_A_RIGHT;
        for (uint32_t i = 0; i < NUM_ENCODERS_RIGHT; i++) {
            palEnableLineEvent(encoders_pad_a_right[i], PAL_EVENT_MODE_BOTH_EDGES);
            palEnableLineEvent(encoders_pad_b_right[i], PAL_EVENT_MODE_BOTH_EDGES);
            palSetLineCallback(encoders_pad_a_right[i], encoder_pad_cb, (void *)i);
            palSetLineCallback(encoders_pad_b_right[i], encoder_pad_cb, (void *)i);
        }
    }
}

#endif

void keyboard_post_init_kb(void) {
    if (is_keyboard_left()) {
        setPinOutput(A0);
        writePinHigh(A0);
    } else {
        if ((analogReadPin_my(B0) > 1000) || (analogReadPin_my(B1) > 1000)) {
            setPinInput(A11);
            setPinInput(A12);
        }
    }
#if defined(ENCODER_ENABLE) && defined(PAL_USE_CALLBACKS)
    encoder_interrupt_init();
#endif
    // allow user keymaps to do custom post_init
    keyboard_post_init_user();
}
