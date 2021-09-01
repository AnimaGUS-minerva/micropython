/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Linaro Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/mpconfig.h"
#if MICROPY_PY_VOUCHER

#include "py/runtime.h"

#include "stdio.h"
#include "string.h"
#include "voucher.h"

STATIC mp_obj_t mod_demo(void) {
    printf("mod_demo(): ^^\n");

    int input = 4;
    int output = square(input);
    printf("input: %d output: %d\n", input, output);

    if (1) {
        uint8_t mac[6] = { 0xA0, 0xB1, 0xC2, 0xD3, 0xE4, 0xF5 };
        if (0) {
            uint8_t mac2[6] = { 0x00, 0xB1, 0xC2, 0xD3, 0xE4, 0xF5 };
            return mp_obj_new_bool(memcmp(mac2, mac, sizeof(mac)) == 0);
        }
        return mp_obj_new_bytes(mac, sizeof(mac));
    } else {
        mp_obj_t tuple[5] = {
            MP_OBJ_NEW_SMALL_INT(output),
            mp_obj_new_bool(1 == 0),
            mp_const_none,
            mp_const_true,
            mp_const_false,
        };
        printf("sizeof(tuple): %d\n", sizeof(tuple));
        return mp_obj_new_tuple(5, tuple);
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_demo_obj, mod_demo);

STATIC const mp_rom_map_elem_t mp_module_voucher_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_voucher) },
    { MP_ROM_QSTR(MP_QSTR_demo), MP_ROM_PTR(&mod_demo_obj) },
};

STATIC MP_DEFINE_CONST_DICT(mp_module_voucher_globals, mp_module_voucher_globals_table);

const mp_obj_module_t mp_module_voucher = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_voucher_globals,
};

#endif // MICROPY_PY_VOUCHER
