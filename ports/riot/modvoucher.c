/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Linaro Limited
 * Copyright (c) 2021 ANIMA Minerva toolkit
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

#include "py/objstr.h"
#include "py/runtime.h"

#include "stdio.h"
#include "string.h"
#include "voucher.h" // via minerva_voucher crate

STATIC mp_obj_t mod_demo(void) {
    printf("[modvoucher.c] mod_demo(): ^^\n");

    // ...

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_demo_obj, mod_demo);

STATIC mp_obj_t mod_test_ffi(void) {
    printf("mod_test_ffi(): ^^\n");

    { // basic stuff
        int input = 4;
        int output = vch_square(input);
        printf("input: %d output: %d\n", input, output);
    }

    // return misc types

    uint8_t mac[6] = { 0xA0, 0xB1, 0xC2, 0xD3, 0xE4, 0xF5 };
    uint8_t mac2[6] = { 0x00, 0xB1, 0xC2, 0xD3, 0xE4, 0xF5 };

#define SZ 7
    mp_obj_t tuple[SZ] = {
        MP_OBJ_NEW_SMALL_INT(42), // 42
        mp_obj_new_bool(1 == 0), // False
        mp_const_none, // None
        mp_const_true, // True
        mp_const_false, // False
        mp_obj_new_bytes(mac, sizeof(mac)), // b'\xa0\xb1\xc2\xd3\xe4\xf5'
        mp_obj_new_bool(memcmp(mac2, mac, sizeof(mac)) == 0), // False
    };
    printf("sizeof(tuple): %d\n", sizeof(tuple));

    return mp_obj_new_tuple(SZ, tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_test_ffi_obj, mod_test_ffi);

STATIC mp_obj_t mod_init_psa_crypto(void) {
    printf("[modvoucher.c] mod_init_psa_crypto(): ^^\n");

    vch_init_psa_crypto();

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_init_psa_crypto_obj, mod_init_psa_crypto);

STATIC mp_obj_t mod_get_vch_jada(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vch_get_voucher_jada(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_get_vch_jada_obj, mod_get_vch_jada);

//

STATIC mp_obj_t mod_get_vch_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vch_get_voucher_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_get_vch_F2_00_02_obj, mod_get_vch_F2_00_02);

STATIC mp_obj_t mod_get_masa_pem_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vch_get_masa_pem_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_get_masa_pem_F2_00_02_obj, mod_get_masa_pem_F2_00_02);

//

STATIC mp_obj_t mod_get_key_pem_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vch_get_key_pem_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_get_key_pem_F2_00_02_obj, mod_get_key_pem_F2_00_02);

STATIC mp_obj_t mod_get_device_crt_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vch_get_device_crt_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_get_device_crt_F2_00_02_obj, mod_get_device_crt_F2_00_02);

STATIC mp_obj_t mod_get_vrq_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vch_get_vrq_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_get_vrq_F2_00_02_obj, mod_get_vrq_F2_00_02);

STATIC mp_obj_t mod_create_vrq_F2_00_02(void) {
    uint8_t *ptr_heap;
    size_t sz_heap;
    mp_obj_t obj;

    sz_heap = vch_create_vrq_F2_00_02(&ptr_heap);
    obj = mp_obj_new_bytes(ptr_heap, sz_heap);
    free(ptr_heap);

    return obj;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_create_vrq_F2_00_02_obj, mod_create_vrq_F2_00_02);

STATIC mp_obj_t mod_sign(mp_obj_t bs_vch, mp_obj_t bs_key) {
    if (mp_obj_is_type(bs_vch, &mp_type_bytes) &&
        mp_obj_is_type(bs_key, &mp_type_bytes)) {
        uint8_t *ptr_raw, *ptr_key, *ptr_heap;
        size_t sz_raw, sz_key, sz_heap;
        mp_obj_t obj;

        {
            GET_STR_DATA_LEN(bs_vch, str_data, str_len);
            ptr_raw = (uint8_t *)str_data;
            sz_raw = str_len;
        }

        {
            GET_STR_DATA_LEN(bs_key, str_data, str_len);
            ptr_key = (uint8_t *)str_data;
            sz_key = str_len;
        }

        sz_heap = vch_sign(ptr_raw, sz_raw, ptr_key, sz_key, &ptr_heap);
        obj = mp_obj_new_bytes(ptr_heap, sz_heap);
        free(ptr_heap);

        return obj;
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("both 'voucher' and 'key' args must be <class 'bytes'>"));
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_sign_obj, mod_sign);

//

STATIC mp_obj_t mod_debug(mp_obj_t self_in) {
    if (mp_obj_is_type(self_in, &mp_type_bytes)) {
        GET_STR_DATA_LEN(self_in, str_data, str_len);
        vch_debug(str_data, str_len);
        return mp_const_none;
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("'voucher' arg must be <class 'bytes'>"));
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_debug_obj, mod_debug);

STATIC mp_obj_t mod_validate(size_t n_args, const mp_obj_t *args) {
    // printf("mod_validate(): n_args: %d\n", n_args);

    if (n_args == 1) {
        if (mp_obj_is_type(args[0], &mp_type_bytes)) {
            GET_STR_DATA_LEN(args[0], str_data, str_len);
            return mp_obj_new_bool(vch_validate(str_data, str_len));
        } else {
            mp_raise_ValueError(MP_ERROR_TEXT("'voucher' arg must be <class 'bytes'>"));
        }
    } else { // with MASA pem
        if (mp_obj_is_type(args[0], &mp_type_bytes) &&
            mp_obj_is_type(args[1], &mp_type_bytes)) {
            uint8_t *ptr, *ptr_pem;
            size_t sz, sz_pem;

            {
                GET_STR_DATA_LEN(args[0], str_data, str_len);
                ptr = (uint8_t *)str_data;
                sz = str_len;
            }

            {
                GET_STR_DATA_LEN(args[1], str_data, str_len);
                ptr_pem = (uint8_t *)str_data;
                sz_pem = str_len;
            }

            return mp_obj_new_bool(vch_validate_with_pem(ptr, sz, ptr_pem, sz_pem));
        } else {
            mp_raise_ValueError(MP_ERROR_TEXT("both 'voucher' and 'pem' args must be <class 'bytes'>"));
        }
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_validate_obj, 1, 2, mod_validate);

//

typedef mp_int_t voucher_t; // @@ !!!!

typedef struct _mp_obj_voucher_t {
    mp_obj_base_t base;
    voucher_t t;
    mp_obj_t callback;
} mp_obj_voucher_t;

STATIC mp_obj_t mp_voucher_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    printf("mp_voucher_make_new(): n_args: %d\n", n_args);

    mp_arg_check_num(n_args, n_kw, 1, 1, false);
    if (!mp_obj_is_callable(args[0])) {
        mp_raise_TypeError("callback argument must be callable");
    }

    mp_obj_voucher_t *o = m_new_obj(mp_obj_voucher_t);
    o->base.type = type;
    memset(&o->t, '\0', sizeof(o->t));
    o->callback = args[0];
//    o->t.callback = mp_riot_isr_callback;
//    o->t.arg = &o->callback;
    return  MP_OBJ_FROM_PTR(o);
}

STATIC mp_obj_t mp_voucher_set(mp_obj_t self_in, mp_obj_t val_in) {
    mp_obj_voucher_t *o = MP_OBJ_TO_PTR(self_in);
    mp_int_t val = mp_obj_get_int(val_in);

//    if (val <= 0) {
//        mp_raise_ValueError("voucher.set(): `val_in` must be positive");
//    }
//
//    voucher_set(&o->t, val);
    //==== !!!!
    printf("mp_voucher_set(): before: %d\n", o->t);
    o->t = val;
    printf("mp_voucher_set(): after: %d\n", o->t);

    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_2(mp_voucher_set_obj, mp_voucher_set);

STATIC const mp_rom_map_elem_t voucher_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_set), MP_ROM_PTR(&mp_voucher_set_obj) },
};

STATIC MP_DEFINE_CONST_DICT(voucher_locals_dict, voucher_locals_dict_table);
STATIC const mp_obj_type_t voucher_type = {
    { &mp_type_type },
    .name = MP_QSTR_voucher,
    .make_new = mp_voucher_make_new,
    .locals_dict = (void*)&voucher_locals_dict,
};

//

STATIC const mp_rom_map_elem_t mp_module_voucher_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_voucher) },
    { MP_ROM_QSTR(MP_QSTR_voucher), MP_ROM_PTR(&voucher_type) },
    //---- debug/test stuff
    { MP_ROM_QSTR(MP_QSTR_demo), MP_ROM_PTR(&mod_demo_obj) },
    { MP_ROM_QSTR(MP_QSTR_test_ffi), MP_ROM_PTR(&mod_test_ffi_obj) },
    { MP_ROM_QSTR(MP_QSTR_init_psa_crypto), MP_ROM_PTR(&mod_init_psa_crypto_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_vch_jada), MP_ROM_PTR(&mod_get_vch_jada_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_vch_F2_00_02), MP_ROM_PTR(&mod_get_vch_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_masa_pem_F2_00_02), MP_ROM_PTR(&mod_get_masa_pem_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_key_pem_F2_00_02), MP_ROM_PTR(&mod_get_key_pem_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_device_crt_F2_00_02), MP_ROM_PTR(&mod_get_device_crt_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_vrq_F2_00_02), MP_ROM_PTR(&mod_get_vrq_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_create_vrq_F2_00_02), MP_ROM_PTR(&mod_create_vrq_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_sign), MP_ROM_PTR(&mod_sign_obj) },
    { MP_ROM_QSTR(MP_QSTR_debug), MP_ROM_PTR(&mod_debug_obj) },
    { MP_ROM_QSTR(MP_QSTR_validate), MP_ROM_PTR(&mod_validate_obj) },
};

STATIC MP_DEFINE_CONST_DICT(mp_module_voucher_globals, mp_module_voucher_globals_table);

const mp_obj_module_t mp_module_voucher = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_voucher_globals,
};

#endif // MICROPY_PY_VOUCHER
