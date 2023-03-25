//
//  rb_keys.h
//  Game engine base code
//
//  Key code handling (copy of SDL)
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_KEYS_H
#define RB_KEYS_H

typedef enum {
    RB_SCANCODE_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    RB_SCANCODE_A = 4,
    RB_SCANCODE_B = 5,
    RB_SCANCODE_C = 6,
    RB_SCANCODE_D = 7,
    RB_SCANCODE_E = 8,
    RB_SCANCODE_F = 9,
    RB_SCANCODE_G = 10,
    RB_SCANCODE_H = 11,
    RB_SCANCODE_I = 12,
    RB_SCANCODE_J = 13,
    RB_SCANCODE_K = 14,
    RB_SCANCODE_L = 15,
    RB_SCANCODE_M = 16,
    RB_SCANCODE_N = 17,
    RB_SCANCODE_O = 18,
    RB_SCANCODE_P = 19,
    RB_SCANCODE_Q = 20,
    RB_SCANCODE_R = 21,
    RB_SCANCODE_S = 22,
    RB_SCANCODE_T = 23,
    RB_SCANCODE_U = 24,
    RB_SCANCODE_V = 25,
    RB_SCANCODE_W = 26,
    RB_SCANCODE_X = 27,
    RB_SCANCODE_Y = 28,
    RB_SCANCODE_Z = 29,

    RB_SCANCODE_1 = 30,
    RB_SCANCODE_2 = 31,
    RB_SCANCODE_3 = 32,
    RB_SCANCODE_4 = 33,
    RB_SCANCODE_5 = 34,
    RB_SCANCODE_6 = 35,
    RB_SCANCODE_7 = 36,
    RB_SCANCODE_8 = 37,
    RB_SCANCODE_9 = 38,
    RB_SCANCODE_0 = 39,

    RB_SCANCODE_RETURN = 40,
    RB_SCANCODE_ESCAPE = 41,
    RB_SCANCODE_BACKSPACE = 42,
    RB_SCANCODE_TAB = 43,
    RB_SCANCODE_SPACE = 44,

    RB_SCANCODE_MINUS = 45,
    RB_SCANCODE_EQUALS = 46,
    RB_SCANCODE_LEFTBRACKET = 47,
    RB_SCANCODE_RIGHTBRACKET = 48,
    RB_SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
                                  *   key on ISO keyboards and at the right end
                                  *   of the QWERTY row on ANSI keyboards.
                                  *   Produces REVERSE SOLIDUS (backslash) and
                                  *   VERTICAL LINE in a US layout, REVERSE
                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
                                  *   layout, NUMBER SIGN and TILDE in a UK
                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
                                  *   in a Swiss German layout, NUMBER SIGN and
                                  *   APOSTROPHE in a German layout, GRAVE
                                  *   ACCENT and POUND SIGN in a French Mac
                                  *   layout, and ASTERISK and MICRO SIGN in a
                                  *   French Windows layout.
                                  */
    RB_SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate RB_SCANCODE_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    RB_SCANCODE_SEMICOLON = 51,
    RB_SCANCODE_APOSTROPHE = 52,
    RB_SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                              *   and ISO keyboards). Produces GRAVE ACCENT and
                              *   TILDE in a US Windows layout and in US and UK
                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                              *   and NOT SIGN in a UK Windows layout, SECTION
                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                              *   layouts on ISO keyboards, SECTION SIGN and
                              *   DEGREE SIGN in a Swiss German layout (Mac:
                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                              *   DEGREE SIGN in a German layout (Mac: only on
                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                              *   French Windows layout, COMMERCIAL AT and
                              *   NUMBER SIGN in a French Mac layout on ISO
                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                              *   SIGN in a Swiss German, German, or French Mac
                              *   layout on ANSI keyboards.
                              */
    RB_SCANCODE_COMMA = 54,
    RB_SCANCODE_PERIOD = 55,
    RB_SCANCODE_SLASH = 56,

    RB_SCANCODE_CAPSLOCK = 57,

    RB_SCANCODE_F1 = 58,
    RB_SCANCODE_F2 = 59,
    RB_SCANCODE_F3 = 60,
    RB_SCANCODE_F4 = 61,
    RB_SCANCODE_F5 = 62,
    RB_SCANCODE_F6 = 63,
    RB_SCANCODE_F7 = 64,
    RB_SCANCODE_F8 = 65,
    RB_SCANCODE_F9 = 66,
    RB_SCANCODE_F10 = 67,
    RB_SCANCODE_F11 = 68,
    RB_SCANCODE_F12 = 69,

    RB_SCANCODE_PRINTSCREEN = 70,
    RB_SCANCODE_SCROLLLOCK = 71,
    RB_SCANCODE_PAUSE = 72,
    RB_SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    RB_SCANCODE_HOME = 74,
    RB_SCANCODE_PAGEUP = 75,
    RB_SCANCODE_DELETE = 76,
    RB_SCANCODE_END = 77,
    RB_SCANCODE_PAGEDOWN = 78,
    RB_SCANCODE_RIGHT = 79,
    RB_SCANCODE_LEFT = 80,
    RB_SCANCODE_DOWN = 81,
    RB_SCANCODE_UP = 82,

    RB_SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    RB_SCANCODE_KP_DIVIDE = 84,
    RB_SCANCODE_KP_MULTIPLY = 85,
    RB_SCANCODE_KP_MINUS = 86,
    RB_SCANCODE_KP_PLUS = 87,
    RB_SCANCODE_KP_ENTER = 88,
    RB_SCANCODE_KP_1 = 89,
    RB_SCANCODE_KP_2 = 90,
    RB_SCANCODE_KP_3 = 91,
    RB_SCANCODE_KP_4 = 92,
    RB_SCANCODE_KP_5 = 93,
    RB_SCANCODE_KP_6 = 94,
    RB_SCANCODE_KP_7 = 95,
    RB_SCANCODE_KP_8 = 96,
    RB_SCANCODE_KP_9 = 97,
    RB_SCANCODE_KP_0 = 98,
    RB_SCANCODE_KP_PERIOD = 99,

    RB_SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    RB_SCANCODE_APPLICATION = 101, /**< windows contextual menu, compose */
    RB_SCANCODE_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    RB_SCANCODE_KP_EQUALS = 103,
    RB_SCANCODE_F13 = 104,
    RB_SCANCODE_F14 = 105,
    RB_SCANCODE_F15 = 106,
    RB_SCANCODE_F16 = 107,
    RB_SCANCODE_F17 = 108,
    RB_SCANCODE_F18 = 109,
    RB_SCANCODE_F19 = 110,
    RB_SCANCODE_F20 = 111,
    RB_SCANCODE_F21 = 112,
    RB_SCANCODE_F22 = 113,
    RB_SCANCODE_F23 = 114,
    RB_SCANCODE_F24 = 115,
    RB_SCANCODE_EXECUTE = 116,
    RB_SCANCODE_HELP = 117,
    RB_SCANCODE_MENU = 118,
    RB_SCANCODE_SELECT = 119,
    RB_SCANCODE_STOP = 120,
    RB_SCANCODE_AGAIN = 121,   /**< redo */
    RB_SCANCODE_UNDO = 122,
    RB_SCANCODE_CUT = 123,
    RB_SCANCODE_COPY = 124,
    RB_SCANCODE_PASTE = 125,
    RB_SCANCODE_FIND = 126,
    RB_SCANCODE_MUTE = 127,
    RB_SCANCODE_VOLUMEUP = 128,
    RB_SCANCODE_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     RB_SCANCODE_LOCKINGCAPSLOCK = 130,  */
/*     RB_SCANCODE_LOCKINGNUMLOCK = 131, */
/*     RB_SCANCODE_LOCKINGSCROLLLOCK = 132, */
    RB_SCANCODE_KP_COMMA = 133,
    RB_SCANCODE_KP_EQUALSAS400 = 134,

    RB_SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    RB_SCANCODE_INTERNATIONAL2 = 136,
    RB_SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
    RB_SCANCODE_INTERNATIONAL4 = 138,
    RB_SCANCODE_INTERNATIONAL5 = 139,
    RB_SCANCODE_INTERNATIONAL6 = 140,
    RB_SCANCODE_INTERNATIONAL7 = 141,
    RB_SCANCODE_INTERNATIONAL8 = 142,
    RB_SCANCODE_INTERNATIONAL9 = 143,
    RB_SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
    RB_SCANCODE_LANG2 = 145, /**< Hanja conversion */
    RB_SCANCODE_LANG3 = 146, /**< Katakana */
    RB_SCANCODE_LANG4 = 147, /**< Hiragana */
    RB_SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
    RB_SCANCODE_LANG6 = 149, /**< reserved */
    RB_SCANCODE_LANG7 = 150, /**< reserved */
    RB_SCANCODE_LANG8 = 151, /**< reserved */
    RB_SCANCODE_LANG9 = 152, /**< reserved */

    RB_SCANCODE_ALTERASE = 153, /**< Erase-Eaze */
    RB_SCANCODE_SYSREQ = 154,
    RB_SCANCODE_CANCEL = 155,
    RB_SCANCODE_CLEAR = 156,
    RB_SCANCODE_PRIOR = 157,
    RB_SCANCODE_RETURN2 = 158,
    RB_SCANCODE_SEPARATOR = 159,
    RB_SCANCODE_OUT = 160,
    RB_SCANCODE_OPER = 161,
    RB_SCANCODE_CLEARAGAIN = 162,
    RB_SCANCODE_CRSEL = 163,
    RB_SCANCODE_EXSEL = 164,

    RB_SCANCODE_KP_00 = 176,
    RB_SCANCODE_KP_000 = 177,
    RB_SCANCODE_THOUSANDSSEPARATOR = 178,
    RB_SCANCODE_DECIMALSEPARATOR = 179,
    RB_SCANCODE_CURRENCYUNIT = 180,
    RB_SCANCODE_CURRENCYSUBUNIT = 181,
    RB_SCANCODE_KP_LEFTPAREN = 182,
    RB_SCANCODE_KP_RIGHTPAREN = 183,
    RB_SCANCODE_KP_LEFTBRACE = 184,
    RB_SCANCODE_KP_RIGHTBRACE = 185,
    RB_SCANCODE_KP_TAB = 186,
    RB_SCANCODE_KP_BACKSPACE = 187,
    RB_SCANCODE_KP_A = 188,
    RB_SCANCODE_KP_B = 189,
    RB_SCANCODE_KP_C = 190,
    RB_SCANCODE_KP_D = 191,
    RB_SCANCODE_KP_E = 192,
    RB_SCANCODE_KP_F = 193,
    RB_SCANCODE_KP_XOR = 194,
    RB_SCANCODE_KP_POWER = 195,
    RB_SCANCODE_KP_PERCENT = 196,
    RB_SCANCODE_KP_LESS = 197,
    RB_SCANCODE_KP_GREATER = 198,
    RB_SCANCODE_KP_AMPERSAND = 199,
    RB_SCANCODE_KP_DBLAMPERSAND = 200,
    RB_SCANCODE_KP_VERTICALBAR = 201,
    RB_SCANCODE_KP_DBLVERTICALBAR = 202,
    RB_SCANCODE_KP_COLON = 203,
    RB_SCANCODE_KP_HASH = 204,
    RB_SCANCODE_KP_SPACE = 205,
    RB_SCANCODE_KP_AT = 206,
    RB_SCANCODE_KP_EXCLAM = 207,
    RB_SCANCODE_KP_MEMSTORE = 208,
    RB_SCANCODE_KP_MEMRECALL = 209,
    RB_SCANCODE_KP_MEMCLEAR = 210,
    RB_SCANCODE_KP_MEMADD = 211,
    RB_SCANCODE_KP_MEMSUBTRACT = 212,
    RB_SCANCODE_KP_MEMMULTIPLY = 213,
    RB_SCANCODE_KP_MEMDIVIDE = 214,
    RB_SCANCODE_KP_PLUSMINUS = 215,
    RB_SCANCODE_KP_CLEAR = 216,
    RB_SCANCODE_KP_CLEARENTRY = 217,
    RB_SCANCODE_KP_BINARY = 218,
    RB_SCANCODE_KP_OCTAL = 219,
    RB_SCANCODE_KP_DECIMAL = 220,
    RB_SCANCODE_KP_HEXADECIMAL = 221,

    RB_SCANCODE_LCTRL = 224,
    RB_SCANCODE_LSHIFT = 225,
    RB_SCANCODE_LALT = 226, /**< alt, option */
    RB_SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
    RB_SCANCODE_RCTRL = 228,
    RB_SCANCODE_RSHIFT = 229,
    RB_SCANCODE_RALT = 230, /**< alt gr, option */
    RB_SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

    RB_SCANCODE_MODE = 257,    /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special KMOD_MODE for it I'm adding it here
                                 */

    /* @} *//* Usage page 0x07 */

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    RB_SCANCODE_AUDIONEXT = 258,
    RB_SCANCODE_AUDIOPREV = 259,
    RB_SCANCODE_AUDIOSTOP = 260,
    RB_SCANCODE_AUDIOPLAY = 261,
    RB_SCANCODE_AUDIOMUTE = 262,
    RB_SCANCODE_MEDIASELECT = 263,
    RB_SCANCODE_WWW = 264,
    RB_SCANCODE_MAIL = 265,
    RB_SCANCODE_CALCULATOR = 266,
    RB_SCANCODE_COMPUTER = 267,
    RB_SCANCODE_AC_SEARCH = 268,
    RB_SCANCODE_AC_HOME = 269,
    RB_SCANCODE_AC_BACK = 270,
    RB_SCANCODE_AC_FORWARD = 271,
    RB_SCANCODE_AC_STOP = 272,
    RB_SCANCODE_AC_REFRESH = 273,
    RB_SCANCODE_AC_BOOKMARKS = 274,

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    RB_SCANCODE_BRIGHTNESSDOWN = 275,
    RB_SCANCODE_BRIGHTNESSUP = 276,
    RB_SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    RB_SCANCODE_KBDILLUMTOGGLE = 278,
    RB_SCANCODE_KBDILLUMDOWN = 279,
    RB_SCANCODE_KBDILLUMUP = 280,
    RB_SCANCODE_EJECT = 281,
    RB_SCANCODE_SLEEP = 282,

    RB_SCANCODE_APP1 = 283,
    RB_SCANCODE_APP2 = 284,

    /* @} *//* Walther keys */

    /**
     *  \name Usage page 0x0C (additional media keys)
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    RB_SCANCODE_AUDIOREWIND = 285,
    RB_SCANCODE_AUDIOFASTFORWARD = 286,

    /* @} *//* Usage page 0x0C (additional media keys) */

    /* Add any other keys here. */

    RB_NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
} RBScanCode;

#define RB_SCANCODE_MASK (1<<30)
#define RB_SCANCODE_TO_KEYCODE(X)  (X | RB_SCANCODE_MASK)

typedef enum {
    RB_KEYCODE_UNKNOWN = 0,

    RB_KEYCODE_RETURN = '\r',
    RB_KEYCODE_ESCAPE = '\x1B',
    RB_KEYCODE_BACKSPACE = '\b',
    RB_KEYCODE_TAB = '\t',
    RB_KEYCODE_SPACE = ' ',
    RB_KEYCODE_EXCLAIM = '!',
    RB_KEYCODE_QUOTEDBL = '"',
    RB_KEYCODE_HASH = '#',
    RB_KEYCODE_PERCENT = '%',
    RB_KEYCODE_DOLLAR = '$',
    RB_KEYCODE_AMPERSAND = '&',
    RB_KEYCODE_QUOTE = '\'',
    RB_KEYCODE_LEFTPAREN = '(',
    RB_KEYCODE_RIGHTPAREN = ')',
    RB_KEYCODE_ASTERISK = '*',
    RB_KEYCODE_PLUS = '+',
    RB_KEYCODE_COMMA = ',',
    RB_KEYCODE_MINUS = '-',
    RB_KEYCODE_PERIOD = '.',
    RB_KEYCODE_SLASH = '/',
    RB_KEYCODE_0 = '0',
    RB_KEYCODE_1 = '1',
    RB_KEYCODE_2 = '2',
    RB_KEYCODE_3 = '3',
    RB_KEYCODE_4 = '4',
    RB_KEYCODE_5 = '5',
    RB_KEYCODE_6 = '6',
    RB_KEYCODE_7 = '7',
    RB_KEYCODE_8 = '8',
    RB_KEYCODE_9 = '9',
    RB_KEYCODE_COLON = ':',
    RB_KEYCODE_SEMICOLON = ';',
    RB_KEYCODE_LESS = '<',
    RB_KEYCODE_EQUALS = '=',
    RB_KEYCODE_GREATER = '>',
    RB_KEYCODE_QUESTION = '?',
    RB_KEYCODE_AT = '@',

    /*
       Skip uppercase letters
     */

    RB_KEYCODE_LEFTBRACKET = '[',
    RB_KEYCODE_BACKSLASH = '\\',
    RB_KEYCODE_RIGHTBRACKET = ']',
    RB_KEYCODE_CARET = '^',
    RB_KEYCODE_UNDERSCORE = '_',
    RB_KEYCODE_BACKQUOTE = '`',
    RB_KEYCODE_a = 'a',
    RB_KEYCODE_b = 'b',
    RB_KEYCODE_c = 'c',
    RB_KEYCODE_d = 'd',
    RB_KEYCODE_e = 'e',
    RB_KEYCODE_f = 'f',
    RB_KEYCODE_g = 'g',
    RB_KEYCODE_h = 'h',
    RB_KEYCODE_i = 'i',
    RB_KEYCODE_j = 'j',
    RB_KEYCODE_k = 'k',
    RB_KEYCODE_l = 'l',
    RB_KEYCODE_m = 'm',
    RB_KEYCODE_n = 'n',
    RB_KEYCODE_o = 'o',
    RB_KEYCODE_p = 'p',
    RB_KEYCODE_q = 'q',
    RB_KEYCODE_r = 'r',
    RB_KEYCODE_s = 's',
    RB_KEYCODE_t = 't',
    RB_KEYCODE_u = 'u',
    RB_KEYCODE_v = 'v',
    RB_KEYCODE_w = 'w',
    RB_KEYCODE_x = 'x',
    RB_KEYCODE_y = 'y',
    RB_KEYCODE_z = 'z',

    RB_KEYCODE_CAPSLOCK = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_CAPSLOCK),

    RB_KEYCODE_F1 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F1),
    RB_KEYCODE_F2 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F2),
    RB_KEYCODE_F3 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F3),
    RB_KEYCODE_F4 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F4),
    RB_KEYCODE_F5 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F5),
    RB_KEYCODE_F6 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F6),
    RB_KEYCODE_F7 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F7),
    RB_KEYCODE_F8 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F8),
    RB_KEYCODE_F9 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F9),
    RB_KEYCODE_F10 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F10),
    RB_KEYCODE_F11 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F11),
    RB_KEYCODE_F12 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F12),

    RB_KEYCODE_PRINTSCREEN = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_PRINTSCREEN),
    RB_KEYCODE_SCROLLLOCK = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_SCROLLLOCK),
    RB_KEYCODE_PAUSE = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_PAUSE),
    RB_KEYCODE_INSERT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_INSERT),
    RB_KEYCODE_HOME = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_HOME),
    RB_KEYCODE_PAGEUP = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_PAGEUP),
    RB_KEYCODE_DELETE = '\x7F',
    RB_KEYCODE_END = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_END),
    RB_KEYCODE_PAGEDOWN = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_PAGEDOWN),
    RB_KEYCODE_RIGHT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_RIGHT),
    RB_KEYCODE_LEFT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_LEFT),
    RB_KEYCODE_DOWN = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_DOWN),
    RB_KEYCODE_UP = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_UP),

    RB_KEYCODE_NUMLOCKCLEAR = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_NUMLOCKCLEAR),
    RB_KEYCODE_KP_DIVIDE = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_DIVIDE),
    RB_KEYCODE_KP_MULTIPLY = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_MULTIPLY),
    RB_KEYCODE_KP_MINUS = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_MINUS),
    RB_KEYCODE_KP_PLUS = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_PLUS),
    RB_KEYCODE_KP_ENTER = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_ENTER),
    RB_KEYCODE_KP_1 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_1),
    RB_KEYCODE_KP_2 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_2),
    RB_KEYCODE_KP_3 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_3),
    RB_KEYCODE_KP_4 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_4),
    RB_KEYCODE_KP_5 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_5),
    RB_KEYCODE_KP_6 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_6),
    RB_KEYCODE_KP_7 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_7),
    RB_KEYCODE_KP_8 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_8),
    RB_KEYCODE_KP_9 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_9),
    RB_KEYCODE_KP_0 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_0),
    RB_KEYCODE_KP_PERIOD = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_PERIOD),

    RB_KEYCODE_APPLICATION = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_APPLICATION),
    RB_KEYCODE_POWER = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_POWER),
    RB_KEYCODE_KP_EQUALS = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_EQUALS),
    RB_KEYCODE_F13 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F13),
    RB_KEYCODE_F14 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F14),
    RB_KEYCODE_F15 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F15),
    RB_KEYCODE_F16 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F16),
    RB_KEYCODE_F17 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F17),
    RB_KEYCODE_F18 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F18),
    RB_KEYCODE_F19 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F19),
    RB_KEYCODE_F20 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F20),
    RB_KEYCODE_F21 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F21),
    RB_KEYCODE_F22 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F22),
    RB_KEYCODE_F23 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F23),
    RB_KEYCODE_F24 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_F24),
    RB_KEYCODE_EXECUTE = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_EXECUTE),
    RB_KEYCODE_HELP = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_HELP),
    RB_KEYCODE_MENU = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_MENU),
    RB_KEYCODE_SELECT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_SELECT),
    RB_KEYCODE_STOP = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_STOP),
    RB_KEYCODE_AGAIN = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AGAIN),
    RB_KEYCODE_UNDO = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_UNDO),
    RB_KEYCODE_CUT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_CUT),
    RB_KEYCODE_COPY = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_COPY),
    RB_KEYCODE_PASTE = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_PASTE),
    RB_KEYCODE_FIND = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_FIND),
    RB_KEYCODE_MUTE = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_MUTE),
    RB_KEYCODE_VOLUMEUP = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_VOLUMEUP),
    RB_KEYCODE_VOLUMEDOWN = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_VOLUMEDOWN),
    RB_KEYCODE_KP_COMMA = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_COMMA),
    RB_KEYCODE_KP_EQUALSAS400 =
        RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_EQUALSAS400),

    RB_KEYCODE_ALTERASE = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_ALTERASE),
    RB_KEYCODE_SYSREQ = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_SYSREQ),
    RB_KEYCODE_CANCEL = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_CANCEL),
    RB_KEYCODE_CLEAR = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_CLEAR),
    RB_KEYCODE_PRIOR = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_PRIOR),
    RB_KEYCODE_RETURN2 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_RETURN2),
    RB_KEYCODE_SEPARATOR = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_SEPARATOR),
    RB_KEYCODE_OUT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_OUT),
    RB_KEYCODE_OPER = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_OPER),
    RB_KEYCODE_CLEARAGAIN = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_CLEARAGAIN),
    RB_KEYCODE_CRSEL = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_CRSEL),
    RB_KEYCODE_EXSEL = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_EXSEL),

    RB_KEYCODE_KP_00 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_00),
    RB_KEYCODE_KP_000 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_000),
    RB_KEYCODE_THOUSANDSSEPARATOR =
        RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_THOUSANDSSEPARATOR),
    RB_KEYCODE_DECIMALSEPARATOR =
        RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_DECIMALSEPARATOR),
    RB_KEYCODE_CURRENCYUNIT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_CURRENCYUNIT),
    RB_KEYCODE_CURRENCYSUBUNIT =
        RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_CURRENCYSUBUNIT),
    RB_KEYCODE_KP_LEFTPAREN = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_LEFTPAREN),
    RB_KEYCODE_KP_RIGHTPAREN = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_RIGHTPAREN),
    RB_KEYCODE_KP_LEFTBRACE = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_LEFTBRACE),
    RB_KEYCODE_KP_RIGHTBRACE = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_RIGHTBRACE),
    RB_KEYCODE_KP_TAB = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_TAB),
    RB_KEYCODE_KP_BACKSPACE = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_BACKSPACE),
    RB_KEYCODE_KP_A = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_A),
    RB_KEYCODE_KP_B = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_B),
    RB_KEYCODE_KP_C = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_C),
    RB_KEYCODE_KP_D = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_D),
    RB_KEYCODE_KP_E = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_E),
    RB_KEYCODE_KP_F = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_F),
    RB_KEYCODE_KP_XOR = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_XOR),
    RB_KEYCODE_KP_POWER = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_POWER),
    RB_KEYCODE_KP_PERCENT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_PERCENT),
    RB_KEYCODE_KP_LESS = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_LESS),
    RB_KEYCODE_KP_GREATER = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_GREATER),
    RB_KEYCODE_KP_AMPERSAND = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_AMPERSAND),
    RB_KEYCODE_KP_DBLAMPERSAND =
        RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_DBLAMPERSAND),
    RB_KEYCODE_KP_VERTICALBAR =
        RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_VERTICALBAR),
    RB_KEYCODE_KP_DBLVERTICALBAR =
        RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_DBLVERTICALBAR),
    RB_KEYCODE_KP_COLON = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_COLON),
    RB_KEYCODE_KP_HASH = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_HASH),
    RB_KEYCODE_KP_SPACE = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_SPACE),
    RB_KEYCODE_KP_AT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_AT),
    RB_KEYCODE_KP_EXCLAM = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_EXCLAM),
    RB_KEYCODE_KP_MEMSTORE = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_MEMSTORE),
    RB_KEYCODE_KP_MEMRECALL = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_MEMRECALL),
    RB_KEYCODE_KP_MEMCLEAR = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_MEMCLEAR),
    RB_KEYCODE_KP_MEMADD = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_MEMADD),
    RB_KEYCODE_KP_MEMSUBTRACT =
        RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_MEMSUBTRACT),
    RB_KEYCODE_KP_MEMMULTIPLY =
        RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_MEMMULTIPLY),
    RB_KEYCODE_KP_MEMDIVIDE = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_MEMDIVIDE),
    RB_KEYCODE_KP_PLUSMINUS = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_PLUSMINUS),
    RB_KEYCODE_KP_CLEAR = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_CLEAR),
    RB_KEYCODE_KP_CLEARENTRY = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_CLEARENTRY),
    RB_KEYCODE_KP_BINARY = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_BINARY),
    RB_KEYCODE_KP_OCTAL = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_OCTAL),
    RB_KEYCODE_KP_DECIMAL = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_DECIMAL),
    RB_KEYCODE_KP_HEXADECIMAL =
        RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KP_HEXADECIMAL),

    RB_KEYCODE_LCTRL = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_LCTRL),
    RB_KEYCODE_LSHIFT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_LSHIFT),
    RB_KEYCODE_LALT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_LALT),
    RB_KEYCODE_LGUI = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_LGUI),
    RB_KEYCODE_RCTRL = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_RCTRL),
    RB_KEYCODE_RSHIFT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_RSHIFT),
    RB_KEYCODE_RALT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_RALT),
    RB_KEYCODE_RGUI = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_RGUI),

    RB_KEYCODE_MODE = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_MODE),

    RB_KEYCODE_AUDIONEXT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AUDIONEXT),
    RB_KEYCODE_AUDIOPREV = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AUDIOPREV),
    RB_KEYCODE_AUDIOSTOP = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AUDIOSTOP),
    RB_KEYCODE_AUDIOPLAY = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AUDIOPLAY),
    RB_KEYCODE_AUDIOMUTE = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AUDIOMUTE),
    RB_KEYCODE_MEDIASELECT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_MEDIASELECT),
    RB_KEYCODE_WWW = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_WWW),
    RB_KEYCODE_MAIL = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_MAIL),
    RB_KEYCODE_CALCULATOR = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_CALCULATOR),
    RB_KEYCODE_COMPUTER = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_COMPUTER),
    RB_KEYCODE_AC_SEARCH = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AC_SEARCH),
    RB_KEYCODE_AC_HOME = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AC_HOME),
    RB_KEYCODE_AC_BACK = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AC_BACK),
    RB_KEYCODE_AC_FORWARD = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AC_FORWARD),
    RB_KEYCODE_AC_STOP = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AC_STOP),
    RB_KEYCODE_AC_REFRESH = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AC_REFRESH),
    RB_KEYCODE_AC_BOOKMARKS = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AC_BOOKMARKS),

    RB_KEYCODE_BRIGHTNESSDOWN =
        RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_BRIGHTNESSDOWN),
    RB_KEYCODE_BRIGHTNESSUP = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_BRIGHTNESSUP),
    RB_KEYCODE_DISPLAYSWITCH = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_DISPLAYSWITCH),
    RB_KEYCODE_KBDILLUMTOGGLE =
        RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KBDILLUMTOGGLE),
    RB_KEYCODE_KBDILLUMDOWN = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KBDILLUMDOWN),
    RB_KEYCODE_KBDILLUMUP = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_KBDILLUMUP),
    RB_KEYCODE_EJECT = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_EJECT),
    RB_KEYCODE_SLEEP = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_SLEEP),
    RB_KEYCODE_APP1 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_APP1),
    RB_KEYCODE_APP2 = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_APP2),

    RB_KEYCODE_AUDIOREWIND = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AUDIOREWIND),
    RB_KEYCODE_AUDIOFASTFORWARD = RB_SCANCODE_TO_KEYCODE(RB_SCANCODE_AUDIOFASTFORWARD)
} RBKeyCode;

typedef enum _RBEventType {
	RBEVT_None = 0,
	
	RBEVT_KeyPressed = 0x300,	// A key was pressed (data in event.key)
	RBEVT_KeyReleased,			// A key was released (data in event.key)
	RBEVT_TextEntered,
	RBEVT_TextInput,
} RBEventType;

typedef struct _RBEvent {
	RBEventType type; 		// Event type
	RBScanCode scan_code;   // scan code of the key that has been pressed
	RBKeyCode key_code;     // Key code of the key that has been pressed
    bool control;
    bool shift;
    bool alt;
} RBEvent;

#endif
