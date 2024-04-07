#pragma once

#define SPLIT_WPM_ENABLE
#define OLED_TIMEOUT 60000
#define SPLIT_TRANSPORT_MIRROR

#include "rgb.h"

#define ONESHOT_TAP_TOGGLE 3  /* Tapping this number of times holds the key until tapped once again. */
#define ONESHOT_TIMEOUT 10000 /* Time (in ms) before the one shot key is released */

// mouse key accelerations, based on the default values of Miryoku
#undef MOUSEKEY_DELAY
#define MOUSEKEY_DELAY 0
#undef MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL 16
#undef MOUSEKEY_WHEEL_DELAY
#define MOUSEKEY_WHEEL_DELAY 0
#undef MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED 6
#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX 64
