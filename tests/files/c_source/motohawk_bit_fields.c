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

#include "motohawk_bit_fields.h"

#define CTOOLS_MAX(x,y) (((x) < (y)) ? (y) : (x))
#define CTOOLS_MIN(x,y) (((x) < (y)) ? (x) : (y))

static inline uint8_t pack_left_shift_u8(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint8_t pack_left_shift_u16(
    uint16_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint8_t pack_right_shift_u16(
    uint16_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value >> shift) & mask);
}

static inline uint16_t unpack_left_shift_u16(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint16_t)((uint16_t)(value & mask) << shift);
}

static inline uint8_t unpack_right_shift_u8(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value & mask) >> shift);
}

static inline uint16_t unpack_right_shift_u16(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint16_t)((uint16_t)(value & mask) >> shift);
}

int motohawk_bit_fields_example_message_pack(
    uint8_t *dst_p,
    const struct motohawk_bit_fields_example_message_t *src_p,
    size_t size)
{
    uint16_t temperature;

    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[0] |= pack_left_shift_u8(src_p->enable, 7u, 0x80u);
    dst_p[0] |= pack_left_shift_u8(src_p->average_radius, 1u, 0x7eu);
    temperature = (uint16_t)src_p->temperature;
    dst_p[0] |= pack_right_shift_u16(temperature, 11u, 0x01u);
    dst_p[1] |= pack_right_shift_u16(temperature, 3u, 0xffu);
    dst_p[2] |= pack_left_shift_u16(temperature, 5u, 0xe0u);

    return (8);
}

int motohawk_bit_fields_example_message_unpack(
    struct motohawk_bit_fields_example_message_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    uint16_t temperature;

    if (size < 8u) {
        return (-EINVAL);
    }

    dst_p->enable = unpack_right_shift_u8(src_p[0], 7u, 0x80u);
    dst_p->average_radius = unpack_right_shift_u8(src_p[0], 1u, 0x7eu);
    temperature = unpack_left_shift_u16(src_p[0], 11u, 0x01u);
    temperature |= unpack_left_shift_u16(src_p[1], 3u, 0xffu);
    temperature |= unpack_right_shift_u16(src_p[2], 5u, 0xe0u);

    if ((temperature & (1u << 11)) != 0u) {
        temperature |= 0xf000u;
    }

    dst_p->temperature = (int16_t)temperature;

    return (0);
}

static int motohawk_bit_fields_example_message_check_ranges(struct motohawk_bit_fields_example_message_t *msg)
{
    int idx = 1;

    if (!motohawk_bit_fields_example_message_enable_is_in_range(msg->enable))
        return idx;

    idx++;

    if (!motohawk_bit_fields_example_message_average_radius_is_in_range(msg->average_radius))
        return idx;

    idx++;

    if (!motohawk_bit_fields_example_message_temperature_is_in_range(msg->temperature))
        return idx;

    idx++;

    return 0;
}

int motohawk_bit_fields_example_message_wrap_pack(
    uint8_t *outbuf, size_t outbuf_sz,
    double enable,
    double average_radius,
    double temperature)
{
    struct motohawk_bit_fields_example_message_t msg;

    msg.enable = motohawk_bit_fields_example_message_enable_encode(enable);
    msg.average_radius = motohawk_bit_fields_example_message_average_radius_encode(average_radius);
    msg.temperature = motohawk_bit_fields_example_message_temperature_encode(temperature);

    int ret = motohawk_bit_fields_example_message_check_ranges(&msg);
    if (ret) {
        return ret;
    }

    ret = motohawk_bit_fields_example_message_pack(outbuf, &msg, outbuf_sz);
    if (8 != ret) {
        return -1;
    }

    return 0;
}

int motohawk_bit_fields_example_message_wrap_unpack(
    uint8_t *inbuf, size_t inbuf_sz,
    double *enable,
    double *average_radius,
    double *temperature)
{
    struct motohawk_bit_fields_example_message_t msg;
    memset(&msg, 0, sizeof(msg));

    if (motohawk_bit_fields_example_message_unpack(&msg, inbuf, inbuf_sz)) {
        return -1;
    }

    int ret = motohawk_bit_fields_example_message_check_ranges(&msg);

    if (enable)
        *enable = motohawk_bit_fields_example_message_enable_decode(msg.enable);

    if (average_radius)
        *average_radius = motohawk_bit_fields_example_message_average_radius_decode(msg.average_radius);

    if (temperature)
        *temperature = motohawk_bit_fields_example_message_temperature_decode(msg.temperature);

    return ret;
}

uint8_t motohawk_bit_fields_example_message_enable_encode(double value)
{
    return (uint8_t)(value);
}

double motohawk_bit_fields_example_message_enable_decode(uint8_t value)
{
    return ((double)value);
}

double motohawk_bit_fields_example_message_enable_clamp(double val)
{
    double ret = val;


    return ret;
}

bool motohawk_bit_fields_example_message_enable_is_in_range(uint8_t value)
{
    return (value <= 1u);
}

uint8_t motohawk_bit_fields_example_message_average_radius_encode(double value)
{
    return (uint8_t)(value / 0.1);
}

double motohawk_bit_fields_example_message_average_radius_decode(uint8_t value)
{
    return ((double)value * 0.1);
}

double motohawk_bit_fields_example_message_average_radius_clamp(double val)
{
    double ret = val;
    ret = CTOOLS_MAX(ret, 0.0);
    ret = CTOOLS_MIN(ret, 5.0);
    return ret;
}

bool motohawk_bit_fields_example_message_average_radius_is_in_range(uint8_t value)
{
    return (value <= 50u);
}

int16_t motohawk_bit_fields_example_message_temperature_encode(double value)
{
    return (int16_t)((value - 250.0) / 0.01);
}

double motohawk_bit_fields_example_message_temperature_decode(int16_t value)
{
    return (((double)value * 0.01) + 250.0);
}

double motohawk_bit_fields_example_message_temperature_clamp(double val)
{
    double ret = val;
    ret = CTOOLS_MAX(ret, 229.52);
    ret = CTOOLS_MIN(ret, 270.47);
    return ret;
}

bool motohawk_bit_fields_example_message_temperature_is_in_range(int16_t value)
{
    return ((value >= -2048) && (value <= 2047));
}

bool is_extended_frame(uint32_t frame_id)
{
    return false;
}

#undef CTOOLS_MAX
#undef CTOOLS_MIN
