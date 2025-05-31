/*
 * @file: status_ids.h (example)
 * @brief Example definition file for application-specific status IDs.
 *
 * @note
 *   This file is not part of the installed API. Copy and modify
 *   for your own project. Place it in your source tree and include
 *   it before using the status module.
 */

#ifndef STATUS_IDS_H
#define STATUS_IDS_H

#include "../include/status.h"

#define STATUS_ID_PLL_FAULT     STATUS_ENCODE(0, 0)
#define STATUS_ID_OVERCURRENT   STATUS_ENCODE(0, 1)
#define STATUS_ID_OVERTEMP_WARN STATUS_ENCODE(1, 2)
#define STATUS_ID_FAN_FAULT     STATUS_ENCODE(2, 5)

#endif // STATUS_IDS_H
