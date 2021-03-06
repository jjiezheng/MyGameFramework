#ifndef __FRAMEWORK_INPUT_KEYSYMS_H_INCLUDED__
#define __FRAMEWORK_INPUT_KEYSYMS_H_INCLUDED__

#ifdef SDL

#ifdef _MSC_VER
#include <SDL_keysym.h>
#else
#include <SDL/SDL_keysym.h>
#endif

typedef enum KEYS {
	KSYM_UNKNOWN            = 0,
	KSYM_ESCAPE             = SDLK_ESCAPE,
	KSYM_BACKSPACE          = SDLK_BACKSPACE,
	KSYM_END                = SDLK_END,
	KSYM_SOFT_LEFT          = 0,
	KSYM_SOFT_RIGHT         = 0,
	KSYM_HOME               = SDLK_HOME,
	KSYM_BACK               = 0,
	KSYM_CALL               = 0,
	KSYM_ENDCALL            = 0,
	KSYM_0                  = SDLK_0,
	KSYM_1                  = SDLK_1,
	KSYM_2                  = SDLK_2,
	KSYM_3                  = SDLK_3,
	KSYM_4                  = SDLK_4,
	KSYM_5                  = SDLK_5,
	KSYM_6                  = SDLK_6,
	KSYM_7                  = SDLK_7,
	KSYM_8                  = SDLK_8,
	KSYM_9                  = SDLK_9,
	KSYM_ASTERISK           = SDLK_ASTERISK,
	KSYM_HASH               = SDLK_HASH,
	KSYM_UP                 = SDLK_UP,
	KSYM_DOWN               = SDLK_DOWN,
	KSYM_LEFT               = SDLK_LEFT,
	KSYM_RIGHT              = SDLK_RIGHT,
	KSYM_DPAD_CENTER        = 0,
	KSYM_VOLUME_UP          = 0,
	KSYM_VOLUME_DOWN        = 0,
	KSYM_CAMERA             = 0,
	KSYM_CLEAR              = 0,
	KSYM_A                  = SDLK_a,
	KSYM_B                  = SDLK_b,
	KSYM_C                  = SDLK_c,
	KSYM_D                  = SDLK_d,
	KSYM_E                  = SDLK_e,
	KSYM_F                  = SDLK_f,
	KSYM_G                  = SDLK_g,
	KSYM_H                  = SDLK_h,
	KSYM_I                  = SDLK_i,
	KSYM_J                  = SDLK_j,
	KSYM_K                  = SDLK_k,
	KSYM_L                  = SDLK_l,
	KSYM_M                  = SDLK_m,
	KSYM_N                  = SDLK_n,
	KSYM_O                  = SDLK_o,
	KSYM_P                  = SDLK_p,
	KSYM_Q                  = SDLK_q,
	KSYM_R                  = SDLK_r,
	KSYM_S                  = SDLK_s,
	KSYM_T                  = SDLK_t,
	KSYM_U                  = SDLK_u,
	KSYM_V                  = SDLK_v,
	KSYM_W                  = SDLK_w,
	KSYM_X                  = SDLK_x,
	KSYM_Y                  = SDLK_y,
	KSYM_Z                  = SDLK_z,
	KSYM_COMMA              = SDLK_COMMA,
	KSYM_PERIOD             = SDLK_PERIOD,
	KSYM_LALT               = SDLK_LALT,
	KSYM_RALT               = SDLK_RALT,
	KSYM_RCTRL              = SDLK_RCTRL,
	KSYM_LCTRL              = SDLK_LCTRL,
	KSYM_LSHIFT             = SDLK_LSHIFT,
	KSYM_RSHIFT             = SDLK_RSHIFT,
	KSYM_TAB                = SDLK_TAB,
	KSYM_SPACE              = SDLK_SPACE,
	KSYM_SYM                = 0,
	KSYM_RETURN             = SDLK_RETURN,
	KSYM_DELETE             = SDLK_DELETE,
	KSYM_MINUS              = SDLK_MINUS,
	KSYM_EQUALS             = SDLK_EQUALS,
	KSYM_LEFTBRACKET        = SDLK_LEFTBRACKET,
	KSYM_RIGHTBRACKET       = SDLK_RIGHTBRACKET,
	KSYM_BACKSLASH          = SDLK_BACKSLASH,
	KSYM_SEMICOLON          = SDLK_SEMICOLON,
	KSYM_QUOTE              = SDLK_QUOTE,
	KSYM_SLASH              = SDLK_SLASH,
	KSYM_AT                 = SDLK_AT,
	KSYM_PLUS               = SDLK_PLUS,
	KSYM_MENU               = 0,
	KSYM_SEARCH             = 0,
	KSYM_PAGEUP             = SDLK_PAGEUP,
	KSYM_PAGEDOWN           = SDLK_PAGEDOWN,

	// these are all set to correspond to the Xperia Play's physical gamepad
	KSYM_BUTTON_A           = 0,
	KSYM_BUTTON_B           = 0,
	KSYM_BUTTON_C           = 0,
	KSYM_BUTTON_X           = 0,
	KSYM_BUTTON_Y           = 0,
	KSYM_BUTTON_Z           = 0,
	KSYM_BUTTON_L1          = 0,
	KSYM_BUTTON_R1          = 0,
	KSYM_BUTTON_L2          = 0,
	KSYM_BUTTON_R2          = 0,
	KSYM_BUTTON_THUMBL      = 0,
	KSYM_BUTTON_THUMBR      = 0,
	KSYM_BUTTON_START       = 0,
	KSYM_BUTTON_SELECT      = 0,
	KSYM_BUTTON_MODE        = 0,
	// ---

	KSYM_LAST               = SDLK_LAST
} KEYS;

#elif __S3E__

#include "s3eKeyboard.h"

typedef enum KEYS
{
	KSYM_UNKNOWN            = 0,
	KSYM_ESCAPE             = s3eKeyEsc,
	KSYM_BACKSPACE          = s3eKeyBackspace,
	KSYM_END                = s3eKeyKbEnd,
	KSYM_SOFT_LEFT          = s3eKeyLSK,
	KSYM_SOFT_RIGHT         = s3eKeyRSK,
	KSYM_HOME               = s3eKeyHomePage,
	KSYM_BACK               = s3eKeyBack,
	KSYM_CALL               = s3eKeyAccept,
	KSYM_ENDCALL            = s3eKeyEnd,
	KSYM_0                  = s3eKey0,
	KSYM_1                  = s3eKey1,
	KSYM_2                  = s3eKey2,
	KSYM_3                  = s3eKey3,
	KSYM_4                  = s3eKey4,
	KSYM_5                  = s3eKey5,
	KSYM_6                  = s3eKey6,
	KSYM_7                  = s3eKey7,
	KSYM_8                  = s3eKey8,
	KSYM_9                  = s3eKey9,
	KSYM_ASTERISK           = s3eKeyStar,
	KSYM_HASH               = s3eKeyHash,
	KSYM_UP                 = s3eKeyUp,
	KSYM_DOWN               = s3eKeyDown,
	KSYM_LEFT               = s3eKeyLeft,
	KSYM_RIGHT              = s3eKeyRight,
	KSYM_DPAD_CENTER        = s3eKeyOk,
	KSYM_VOLUME_UP          = s3eKeyVolUp,
	KSYM_VOLUME_DOWN        = s3eKeyVolDown,
	KSYM_CAMERA             = s3eKeyCamera,
	KSYM_CLEAR              = s3eKeyCLR,
	KSYM_A                  = s3eKeyA,
	KSYM_B                  = s3eKeyB,
	KSYM_C                  = s3eKeyC,
	KSYM_D                  = s3eKeyD,
	KSYM_E                  = s3eKeyE,
	KSYM_F                  = s3eKeyF,
	KSYM_G                  = s3eKeyG,
	KSYM_H                  = s3eKeyH,
	KSYM_I                  = s3eKeyI,
	KSYM_J                  = s3eKeyJ,
	KSYM_K                  = s3eKeyK,
	KSYM_L                  = s3eKeyL,
	KSYM_M                  = s3eKeyM,
	KSYM_N                  = s3eKeyN,
	KSYM_O                  = s3eKeyO,
	KSYM_P                  = s3eKeyP,
	KSYM_Q                  = s3eKeyQ,
	KSYM_R                  = s3eKeyR,
	KSYM_S                  = s3eKeyS,
	KSYM_T                  = s3eKeyT,
	KSYM_U                  = s3eKeyU,
	KSYM_V                  = s3eKeyV,
	KSYM_W                  = s3eKeyW,
	KSYM_X                  = s3eKeyX,
	KSYM_Y                  = s3eKeyY,
	KSYM_Z                  = s3eKeyZ,
	KSYM_COMMA              = s3eKeyComma,
	KSYM_PERIOD             = s3eKeyPeriod,
	KSYM_LALT               = s3eKeyLeftAlt,
	KSYM_RALT               = s3eKeyRightAlt,
	KSYM_RCTRL              = s3eKeyRightControl,
	KSYM_LCTRL              = s3eKeyLeftControl,
	KSYM_LSHIFT             = s3eKeyLeftShift,
	KSYM_RSHIFT             = s3eKeyRightShift,
	KSYM_TAB                = s3eKeyTab,
	KSYM_SPACE              = s3eKeySpace,
	KSYM_SYM                = s3eKeySym,
	KSYM_RETURN             = s3eKeyEnter,
	KSYM_DELETE             = s3eKeyDelete,
	KSYM_MINUS              = s3eKeyMinus,
	KSYM_EQUALS             = s3eKeyEquals,
	KSYM_LEFTBRACKET        = s3eKeyLeftBracket,
	KSYM_RIGHTBRACKET       = s3eKeyRightBracket,
	KSYM_BACKSLASH          = s3eKeyBackSlash,
	KSYM_SEMICOLON          = s3eKeySemicolon,
	KSYM_QUOTE              = s3eKeyApostrophe,
	KSYM_SLASH              = s3eKeySlash,
	KSYM_AT                 = s3eKeyAt,
	KSYM_PLUS               = s3eKeyNumPadPlus,
	KSYM_MENU               = s3eKeyMenu,
	KSYM_SEARCH             = s3eKeySearch,
	KSYM_PAGEUP             = s3eKeyPageUp,
	KSYM_PAGEDOWN           = s3eKeyPageDown,

	// these are all set to correspond to the Xperia Play's physical gamepad
	KSYM_BUTTON_A           = s3eKeyAbsGameA, //s3eKeyAbsOk,
	KSYM_BUTTON_B           = s3eKeyAbsGameB, //s3eKeyAbsBSK,
	KSYM_BUTTON_C           = 0,
	KSYM_BUTTON_X           = s3eKeyAbsGameD,
	KSYM_BUTTON_Y           = s3eKeyAbsGameC,
	KSYM_BUTTON_Z           = 0,
	KSYM_BUTTON_L1          = s3eKeyLS,
	KSYM_BUTTON_R1          = s3eKeyRS,
	KSYM_BUTTON_L2          = 0,
	KSYM_BUTTON_R2          = 0,
	KSYM_BUTTON_THUMBL      = 0,
	KSYM_BUTTON_THUMBR      = 0,
	KSYM_BUTTON_START       = s3eKeyRSK,
	KSYM_BUTTON_SELECT      = s3eKeyAbsASK,
	KSYM_BUTTON_MODE        = 0,
	// ---

	KSYM_LAST               = s3eKeyCount
} KEYS;

#else

#error Unknown platform

#endif

#endif
