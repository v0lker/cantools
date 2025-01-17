/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018-2019 Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**

 */

#include <string.h>

#include "letter_terminated_can_id_6_0.h"

#define CTOOLS_MAX(x,y) (((x) < (y)) ? (y) : (x))
#define CTOOLS_MIN(x,y) (((x) < (y)) ? (x) : (y))

int letter_terminated_can_id_6_0_symbol1_pack(
    uint8_t *dst_p,
    const struct letter_terminated_can_id_6_0_symbol1_t *src_p,
    size_t size)
{
    (void)src_p;

    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    return (8);
}

int letter_terminated_can_id_6_0_symbol1_unpack(
    struct letter_terminated_can_id_6_0_symbol1_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    (void)dst_p;
    (void)src_p;

    if (size < 8u) {
        return (-EINVAL);
    }

    return (0);
}

static int letter_terminated_can_id_6_0_symbol1_check_ranges(struct letter_terminated_can_id_6_0_symbol1_t *msg)
{
    int idx = 1;
    (void)msg;
    (void)idx;

    return 0;
}

int letter_terminated_can_id_6_0_symbol1_wrap_pack(
    uint8_t *outbuf, size_t outbuf_sz)
{
    struct letter_terminated_can_id_6_0_symbol1_t msg;


    int ret = letter_terminated_can_id_6_0_symbol1_check_ranges(&msg);
    if (ret) {
        return ret;
    }

    ret = letter_terminated_can_id_6_0_symbol1_pack(outbuf, &msg, outbuf_sz);
    if (8 != ret) {
        return -1;
    }

    return 0;
}

int letter_terminated_can_id_6_0_symbol1_wrap_unpack(
    uint8_t *inbuf, size_t inbuf_sz)
{
    struct letter_terminated_can_id_6_0_symbol1_t msg;
    memset(&msg, 0, sizeof(msg));

    if (letter_terminated_can_id_6_0_symbol1_unpack(&msg, inbuf, inbuf_sz)) {
        return -1;
    }

    int ret = letter_terminated_can_id_6_0_symbol1_check_ranges(&msg);

    return ret;
}

bool is_extended_frame(uint32_t frame_id)
{
    return false;
}

#undef CTOOLS_MAX
#undef CTOOLS_MIN
