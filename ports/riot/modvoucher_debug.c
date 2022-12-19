#include "py/mpconfig.h"
#include "py/objstr.h"
#include "py/runtime.h"
#include "stdio.h"
#include "string.h"
#include "modvoucher.h"

#if MICROPY_PY_VOUCHER_DEBUG

//STATIC mp_obj_t vd_demo(void) {
//    printf("[modvoucher.c] vd_demo(): ^^\n");
//
//    return mp_const_none;
//}
//STATIC MP_DEFINE_CONST_FUN_OBJ_0(vd_demo_obj, vd_demo);
STATIC mp_obj_t demo(void) {
    printf("[modvoucher.c] demo(): ^^\n");

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(debug_demo_obj, demo);

STATIC mp_obj_t vd_test_ffi(void) {
    printf("vd_test_ffi(): ^^\n");

    { // basic stuff
        int input = 4;
        int output = vi_square(input);
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
STATIC MP_DEFINE_CONST_FUN_OBJ_0(vd_test_ffi_obj, vd_test_ffi);

STATIC mp_obj_t vd_get_vch_jada(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vi_get_voucher_jada(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(vd_get_vch_jada_obj, vd_get_vch_jada);

STATIC mp_obj_t vd_get_vch_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vi_get_voucher_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(vd_get_vch_F2_00_02_obj, vd_get_vch_F2_00_02);

STATIC mp_obj_t vd_get_masa_pem_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vi_get_masa_pem_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(vd_get_masa_pem_F2_00_02_obj, vd_get_masa_pem_F2_00_02);

STATIC mp_obj_t vd_get_key_pem_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vi_get_key_pem_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(vd_get_key_pem_F2_00_02_obj, vd_get_key_pem_F2_00_02);

STATIC mp_obj_t vd_get_device_crt_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vi_get_device_crt_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(vd_get_device_crt_F2_00_02_obj, vd_get_device_crt_F2_00_02);

STATIC mp_obj_t vd_get_vrq_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vi_get_vrq_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(vd_get_vrq_F2_00_02_obj, vd_get_vrq_F2_00_02);

STATIC mp_obj_t vd_create_vrq_F2_00_02(void) {
    uint8_t *ptr_heap;
    size_t sz_heap;
    mp_obj_t obj;

    sz_heap = vi_create_vrq_F2_00_02(&ptr_heap);
    obj = mp_obj_new_bytes(ptr_heap, sz_heap);
    free(ptr_heap);

    return obj;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(vd_create_vrq_F2_00_02_obj, vd_create_vrq_F2_00_02);

STATIC mp_obj_t vd_sign(mp_obj_t bs_vch, mp_obj_t bs_key, mp_obj_t alg) {
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

        sz_heap = vi_sign(ptr_raw, sz_raw, ptr_key, sz_key, &ptr_heap, mp_obj_get_int(alg));
        obj = mp_obj_new_bytes(ptr_heap, sz_heap);
        free(ptr_heap);

        return obj;
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("both 'voucher' and 'key' args must be <class 'bytes'>"));
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(vd_sign_obj, vd_sign);

STATIC mp_obj_t vd_dump(mp_obj_t self_in) {
    if (mp_obj_is_type(self_in, &mp_type_bytes)) {
        GET_STR_DATA_LEN(self_in, str_data, str_len);
        vi_dump(str_data, str_len);
        return mp_const_none;
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("'voucher' arg must be <class 'bytes'>"));
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(vd_dump_obj, vd_dump);

STATIC mp_obj_t vd_validate(size_t n_args, const mp_obj_t *args) {
    // printf("vd_validate(): n_args: %d\n", n_args);

    if (n_args == 1) {
        if (mp_obj_is_type(args[0], &mp_type_bytes)) {
            GET_STR_DATA_LEN(args[0], str_data, str_len);
            return mp_obj_new_bool(vi_validate(str_data, str_len));
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

            return mp_obj_new_bool(vi_validate_with_pem(ptr, sz, ptr_pem, sz_pem));
        } else {
            mp_raise_ValueError(MP_ERROR_TEXT("both 'voucher' and 'pem' args must be <class 'bytes'>"));
        }
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(vd_validate_obj, 1, 2, vd_validate);

STATIC const mp_rom_map_elem_t mp_module_voucher_debug_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_voucher_debug) },
    //{ MP_ROM_QSTR(MP_QSTR_vd_demo), MP_ROM_PTR(&vd_demo_obj) },
    { MP_ROM_QSTR(MP_QSTR_vd_test_ffi), MP_ROM_PTR(&vd_test_ffi_obj) },
    { MP_ROM_QSTR(MP_QSTR_vd_get_vch_jada), MP_ROM_PTR(&vd_get_vch_jada_obj) },
    { MP_ROM_QSTR(MP_QSTR_vd_get_vch_F2_00_02), MP_ROM_PTR(&vd_get_vch_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_vd_get_masa_pem_F2_00_02), MP_ROM_PTR(&vd_get_masa_pem_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_vd_get_key_pem_F2_00_02), MP_ROM_PTR(&vd_get_key_pem_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_vd_get_device_crt_F2_00_02), MP_ROM_PTR(&vd_get_device_crt_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_vd_get_vrq_F2_00_02), MP_ROM_PTR(&vd_get_vrq_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_vd_create_vrq_F2_00_02), MP_ROM_PTR(&vd_create_vrq_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_vd_dump), MP_ROM_PTR(&vd_dump_obj) },
    { MP_ROM_QSTR(MP_QSTR_vd_validate), MP_ROM_PTR(&vd_validate_obj) },
    { MP_ROM_QSTR(MP_QSTR_vd_sign), MP_ROM_PTR(&vd_sign_obj) },
};

STATIC MP_DEFINE_CONST_DICT(mp_module_voucher_debug_globals, mp_module_voucher_debug_globals_table);

const mp_obj_module_t mp_module_voucher_debug = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_voucher_debug_globals,
};

#endif // MICROPY_PY_VOUCHER_DEBUG