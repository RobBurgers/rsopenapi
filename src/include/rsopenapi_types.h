/**
 *  @file
 *  @brief   rsopenapi extension to rtdb types.
 *  @curator Rob Burgers
 */
#ifndef RSOPENAPI_TYPES_H_
#define RSOPENAPI_TYPES_H_

#include "rtdb_types.h"

namespace rsopen { // rtdb data types

typedef struct kick {
    uint32_t tick = 0;
    float speed = 0; // m/s
    bool is_lob = false;

    SERIALIZE_DATA_FIXED(tick, speed, is_lob);

    kick(uint32_t tick_ = 0, float speed_ = 0, bool is_lob_ = false)
    : tick(tick_), speed(speed_), is_lob(is_lob_)
    {}
} kick_t;

}

#endif /* RSOPENAPI_TYPES_H_ */
