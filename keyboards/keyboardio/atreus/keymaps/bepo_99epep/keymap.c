// Copyright (C) 2019, 2020  Keyboard.io, Inc
//
// this is the style you want to emulate.
// This is the canonical layout file for the Quantum project. If you want to add another keyboard,
#include QMK_KEYBOARD_H
#include "keymap_bepo.h"
#include "keymap_french.h"
#include "config.h"

enum layer_names { BEPO, NAVIG, NUMPAD, MOUSE };

typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

// Tap Dance declarations
enum {
    TDLAYR,
    TDSLP,
//    TDOVRD,
};

// Declare the functions to be used with your tap dance key(s)

// Function associated with all tap dances
uint8_t cur_dance(qk_tap_dance_state_t *state);

// Functions associated with individual tap dances
void ql_finished(qk_tap_dance_state_t *state, void *user_data);
void ql_reset(qk_tap_dance_state_t *state, void *user_data);

#include "g/keymap_combo.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BEPO] = LAYOUT( /* bépo 4 lignes - l'en manque sur les bords */
BP_DQUO,	BP_LDAQ,     BP_RDAQ,     BP_LPRN, BP_RPRN,		         BP_AT,   BP_PLUS,  BP_MINS,	  BP_SLSH,	BP_ASTR,
BP_B,		BP_EACU,     BP_P,	  BP_O,	   BP_EGRV,		         BP_DCIR, BP_V,	    BP_D,	  BP_L,		BP_J,
LSFT_T(BP_A),	BP_U,	     BP_I,	  BP_E,	   BP_COMM, KC_APP,     KC_LGUI, BP_C,    BP_T,	    BP_S,	  BP_R,		RSFT_T(BP_N),
LCTL_T(BP_AGRV),RALT_T(BP_Y),LALT_T(BP_X),BP_DOT,  BP_K,    TD(TDLAYR), KC_SPC,  BP_QUOT, BP_Q,     LALT_T(BP_G), RALT_T(BP_H), RCTL_T(BP_F)
),

  [NAVIG] = LAYOUT( /* Fn à gauche, navigation à droite */
KC_F9, 		KC_F10,	KC_F11, KC_F12,   KC_PAUS,		     KC_CUT,  KC_HOME, KC_INS,  KC_PGUP, KEY_OVERRIDE_OFF,
KC_F5, 		KC_F6,	KC_F7, 	KC_F8, 	  KC_SLCK,		     KC_PSTE, KC_END,  KC_UP,   KC_PGDN, KEY_OVERRIDE_ON,
LSFT_T(KC_F1),	KC_F2, 	KC_F3, 	KC_F4, 	  KC_PSCR,   _______,_______,KC_COPY, KC_LEFT, KC_DOWN, KC_RGHT, KC_RSFT,
KC_LCTL, 	KC_RALT,KC_LALT,TD(TDSLP),KC_NO,_______,_______,KC_UNDO, KC_NO,   KC_LALT, KC_RALT, KC_RCTL
),

  [NUMPAD] = LAYOUT( /* Multimédia à gauche, pavé numérique à droite */
KC_NO,	 KC_NO, KC_BRID, KC_BRIU,  KC_CUT,		      BP_PERC, KC_P7, KC_P8,  KC_P9,  KC_PMNS,
KC_MPRV, KC_MPLY, KC_MSTP, KC_MNXT,  KC_PSTE,		      KC_PSLS, KC_P4, KC_P5,  KC_P6,  KC_PPLS,
KC_LSFT, KC_MUTE, KC_VOLD, KC_VOLU,  KC_COPY,_______, _______,KC_PAST, KC_P1, KC_P2,  KC_P3,  KC_PEQL,
KC_LCTL, KC_RALT, KC_LALT, TD(TDSLP),KC_UNDO,_______, _______,BP_LPRN, BP_COMM,KC_P0, KC_PDOT,BP_RPRN
),

  [MOUSE] = LAYOUT( /* De quoi gigoter le pointeur */
KC_NO,   KC_ACL0, KC_ACL1, KC_ACL2,  KC_CUT,		     KC_CUT,  KC_NO, KC_NO, KC_NO, KC_NO,
KC_BTN3, KC_BTN2, KC_WH_U, KC_BTN1,  KC_PSTE,		     KC_PSTE, KC_BTN1, KC_MS_U, KC_BTN2, KC_BTN3,
KC_LSFT, KC_WH_L, KC_WH_D, KC_WH_R,  KC_COPY,_______,_______,KC_COPY, KC_MS_L, KC_MS_D, KC_MS_R, KC_RSFT,
KC_LCTL, KC_RALT, KC_LALT, TD(TDSLP),KC_UNDO,_______,_______,KC_UNDO, KC_LALT, KC_RALT, KC_ENT,  KC_RCTL
)
};

// Réveil sur la couche bépo
void suspend_wakeup_init_user(void)
{
    layer_move(BEPO);
}


// Détermine la couche à activer en fonction de l’état du tap-dance sur Fun
// layers_array est lu soit en commençant à NAVIG si Fun est maintenu
// soit à BEPO s’il est relaché
const uint8_t layers_array[4] = {
    BEPO,
    NAVIG,
    NUMPAD,
    MOUSE
};

uint8_t cur_dance(qk_tap_dance_state_t *state) {
	int layer_switch = 0;
	if (state->pressed) {
		// si Fun est maintenu on commence sur NAVIG, si c’est la
		// quatrième pression on revient à BEPO
		layer_switch = state->count % 4;
	} else {
		// sinon ça commence au début
		layer_switch = state->count - 1;
	}
	return layers_array[layer_switch];
}
// Initialize tap structure associated with example tap dance key
static tap ql_tap_state = {
    .is_press_action = true,
    .state = 0
};
// Pour sauvegarder la couche actuelle si le tap-dance est maintenu
static uint32_t layer_state_backup;
static bool is_hold;

// Functions that control what our tap dance key does
void ql_finished(qk_tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
	is_hold = false;
	if (state->pressed) {
		layer_state_backup = layer_state;
		is_hold = true;
	}
    layer_move(ql_tap_state.state);
}

void ql_reset(qk_tap_dance_state_t *state, void *user_data) {
    // Si le tap-dance était maintenu, on revient à la couche précédente
	if (is_hold) {
		layer_state_set(layer_state_backup);
	}
    ql_tap_state.state = 0;
}

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // One key to roll them all (layers)
    [TDLAYR] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, ql_finished, ql_reset, 275),
    // DoubleTap pour faire dodo
    [TDSLP] = ACTION_TAP_DANCE_DOUBLE(KC_NO, KC_SLEP),
    // DoubleTap pour bépozerty
//    [TDOVRD] = ACTION_TAP_DANCE_DOUBLE(KEY_OVERRIDE_OFF, KEY_OVERRIDE_ON),
};


// Overrides definition

//const key_override_t dot_ko = ko_make_with_layers_and_negmods(0, KC_V, S(KC_COMM), 1, MOD_MASK_SHIFT);

const key_override_t coln_ko = ko_make_basic(MOD_MASK_SHIFT, KC_V, KC_DOT);
const key_override_t dot_ko = ko_make_basic(0, KC_V, S(KC_COMM));

const key_override_t z_ko = ko_make_basic(0, KC_LBRC, KC_W);

const key_override_t agrv_ko = ko_make_basic(0, LCTL_T(BP_AGRV), KC_0);
const key_override_t sagrv_ko = ko_make_basic(MOD_MASK_SHIFT, LCTL_T(BP_AGRV), S(KC_Q) );

const key_override_t a_ko = ko_make_basic(0, MT(MOD_LSFT, BP_A), MT(MOD_LSFT, KC_Q));


const key_override_t **key_overrides = (const key_override_t *[]){
	&coln_ko,
	&dot_ko,
	&z_ko,
	&agrv_ko,
	&sagrv_ko,
	&a_ko,
	NULL
};

