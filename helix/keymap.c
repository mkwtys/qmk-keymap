#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
  extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

enum layer_number {
  _QWERTY = 0,
  _LOWER,
  _RAISE,
  _ADJUST
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  EISU,
  KANA,
  RGBRST
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // ,-----------------------------------------.             ,-----------------------------------------.
  // |   `  |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  | Del  |
  // |------+------+------+------+------+------|             |------+------+------+------+------+------|
  // | Tab  |   Q  |   W  |   E  |   R  |   T  |             |   Y  |   U  |   I  |   O  |   P  | Bksp |
  // |------+------+------+------+------+------|             |------+------+------+------+------+------|
  // | Ctrl |   A  |   S  |   D  |   F  |   G  |             |   H  |   J  |   K  |   L  |   ;  |  '   |
  // |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
  // | Shift|   Z  |   X  |   C  |   V  |   B  |   [  |   ]  |   N  |   M  |   ,  |   .  |   /  |Enter |
  // |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
  // |Adjust| Esc  | Alt  | GUI  | EISU |Lower |Space |Space |Raise | KANA | Left | Down |  Up  |Right |
  // `-------------------------------------------------------------------------------------------------'
  [_QWERTY] = LAYOUT( \
      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL, \
      KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
      KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
      KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_LBRC, KC_RBRC, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT , \
      ADJUST,  KC_ESC,  KC_LALT, KC_LGUI, EISU,    LOWER,   KC_SPC,  KC_SPC,  RAISE,   KANA,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
      ),
  // ,-----------------------------------------.             ,-----------------------------------------.
  // |   ~  |   !  |   @  |   #  |   $  |   %  |             |   ^  |   &  |  //  |   (  |   )  |      |
  // |------+------+------+------+------+------|             |------+------+------+------+------+------|
  // |   ~  |   !  |   @  |   #  |   $  |   %  |             |   ^  |   &  |  //  |   (  |   )  |      |
  // |------+------+------+------+------+------|             |------+------+------+------+------+------|
  // |      |  F1  |  F2  |  F3  |  F4  |  F5  |             |  F6  |   _  |   +  |   {  |   }  |  |   |
  // |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
  // |      |  F7  |  F8  |  F9  |  F10 |  F11 |  (   |   )  |  F12 |      |      | Home | End  |      |
  // |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
  // |      |      |      |      |      |      |      |      |      |      | Next | Vol- | Vol+ | Play |
  // `-------------------------------------------------------------------------------------------------'
  [_LOWER] = LAYOUT( \
      KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______, \
      KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______, \
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, \
      _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_LPRN, KC_RPRN, KC_F12,  _______, _______, KC_HOME, KC_END,  _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY \
      ),
  // ,-----------------------------------------.             ,-----------------------------------------.
  // |   `  |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  |      |
  // |------+------+------+------+------+------|             |------+------+------+------+------+------|
  // |   `  |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  |      |
  // |------+------+------+------+------+------|             |------+------+------+------+------+------|
  // |      |  F1  |  F2  |  F3  |  F4  |  F5  |             |  F6  |   -  |   =  |   [  |   ]  |  \   |
  // |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
  // |      |  F7  |  F8  |  F9  |  F10 |  F11 |      |      |  F12 |      |      |PageDn|PageUp|      |
  // |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
  // |      |      |      |      |      |      |      |      |      |      | Next | Vol- | Vol+ | Play |
  // `-------------------------------------------------------------------------------------------------'
  [_RAISE] = LAYOUT( \
      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, \
      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, \
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, \
      _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  _______, _______, KC_F12,  _______, _______, KC_PGDN, KC_PGUP, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY \
      ),
  // ,-----------------------------------------.             ,-----------------------------------------.
  // |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |             |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |
  // |------+------+------+------+------+------|             |------+------+------+------+------+------|
  // |      | Reset|RGBRST|      |      |      |             |      |      |      |      |      |      |
  // |------+------+------+------+------+------|             |------+------+------+------+------+------|
  // |      |      |      |      |      | Mac  |             | Win  |Qwerty|      |      |      |      |
  // |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
  // |      |      |      |      |      |      |      |      |      |      |RGB ON| HUE+ | SAT+ | VAL+ |
  // |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
  // |      |      |      |      |      |      |      |      |      |      | MODE | HUE- | SAT- | VAL- |
  // `-------------------------------------------------------------------------------------------------'
  [_ADJUST] =  LAYOUT( \
      KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, \
      _______, RESET,   RGBRST,  _______, _______, _______,                   _______, _______, _______, _______, _______, _______, \
      _______, _______, _______, _______, _______, AG_NORM,                   AG_SWAP, QWERTY,  _______, _______, _______, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD \
      )
};

// define variables for reactive RGB
int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL << _QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
      } else {
        layer_off(_LOWER);
      }
      update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
      } else {
        layer_off(_RAISE);
      }
      update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
    //led operations - RGB mode change now updates the RGB_current_mode to allow the right RGB mode to be set after reactive keys are released
    case RGB_MOD:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          rgblight_mode(RGB_current_mode);
          rgblight_step();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      return false;
      break;
    case EISU:
      if (record->event.pressed) {
        if (keymap_config.swap_lalt_lgui == false) {
          register_code(KC_LANG2);
        } else {
          SEND_STRING(SS_LALT("`"));
        }
      } else {
        unregister_code(KC_LANG2);
      }
      return false;
      break;
    case KANA:
      if (record->event.pressed) {
        if (keymap_config.swap_lalt_lgui == false) {
          register_code(KC_LANG1);
        } else {
          SEND_STRING(SS_LALT("`"));
        }
      } else {
        unregister_code(KC_LANG1);
      }
      return false;
      break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      break;
  }
  return true;
}

void matrix_init_user(void) {
  #ifdef RGBLIGHT_ENABLE
    RGB_current_mode = rgblight_config.mode;
  #endif
  //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
  #ifdef SSD1306OLED
    iota_gfx_init(!has_usb()); // turns on the display
  #endif
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED
  void matrix_scan_user(void) {
    iota_gfx_task(); // this is what updates the display continuously
  }

  void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
    if (memcmp(dest->display, source->display, sizeof(dest->display))) {
      memcpy(dest->display, source->display, sizeof(dest->display));
      dest->dirty = true;
    }
  }

  //assign the right code to your layers for OLED display
  #define L_BASE 0
  #define L_LOWER (1 << _LOWER)
  #define L_RAISE (1 << _RAISE)
  #define L_ADJUST (1 << _ADJUST)
  #define L_ADJUST_TRI (L_ADJUST | L_RAISE | L_LOWER)

  static void render_logo(struct CharacterMatrix *matrix) {
    static char logo[] = {
      0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
      0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
      0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,
      0};
    matrix_write(matrix, logo);
  }

  void render_status(struct CharacterMatrix *matrix) {
    // Render to mode icon
    static char logo[][2][3] = {{{0x95,0x96,0},{0xb5,0xb6,0}},{{0x97,0x98,0},{0xb7,0xb8,0}}};
    if (keymap_config.swap_lalt_lgui == false) {
      matrix_write(matrix, logo[0][0]);
      matrix_write_P(matrix, PSTR("\n"));
      matrix_write(matrix, logo[0][1]);
    } else {
      matrix_write(matrix, logo[1][0]);
      matrix_write_P(matrix, PSTR("\n"));
      matrix_write(matrix, logo[1][1]);
    }

    // Define layers here, Have not worked out how to have text displayed for each layer. Copy down the number you see and add a case for it below
    char buf[40];
    snprintf(buf,sizeof(buf), "Undef-%ld", layer_state);
    matrix_write_P(matrix, PSTR("\nLayer: "));
    switch (layer_state) {
      case L_BASE:
        matrix_write_P(matrix, PSTR("Default"));
        break;
      case L_RAISE:
        matrix_write_P(matrix, PSTR("Raise"));
        break;
      case L_LOWER:
        matrix_write_P(matrix, PSTR("Lower"));
        break;
      case L_ADJUST:
      case L_ADJUST_TRI:
        matrix_write_P(matrix, PSTR("Adjust"));
        break;
      default:
        matrix_write(matrix, buf);
    }

    // Host Keyboard LED Status
    char led[40];
    snprintf(led, sizeof(led), "\n%s  %s  %s",
            (host_keyboard_leds() & (1 << USB_LED_NUM_LOCK)) ? "NUMLOCK" : "       ",
            (host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK)) ? "CAPS" : "    ",
            (host_keyboard_leds() & (1 << USB_LED_SCROLL_LOCK)) ? "SCLK" : "    ");
    matrix_write(matrix, led);
  }

  void iota_gfx_task_user(void) {
    struct CharacterMatrix matrix;

    #if DEBUG_TO_SCREEN
      if (debug_enable) {
        return;
      }
    #endif

    matrix_clear(&matrix);
    if (is_master) {
      render_status(&matrix);
    } else {
      render_logo(&matrix);
    }
    matrix_update(&display, &matrix);
  }
#endif
