#pragma once

#define IGNORE_MOD_TAP_INTERRUPT
#define PERMISSIVE_HOLD
#define TAPPING_TOGGLE 1
#define COMBO_VARIABLE_LEN
#define QMK_KEYS_PER_SCAN 4
#define COMBO_TERM 50
#define TAPPING_TERM 260

enum layer_names {
    BEPO,
    NAVIG,
    NUMPAD,
    MOUSE
};


#define COMBO_ONLY_FROM_LAYER BEPO
