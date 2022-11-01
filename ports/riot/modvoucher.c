#include "py/mpconfig.h"
#include "py/objstr.h"
#include "py/runtime.h"

#include "stdio.h"
#include "string.h"
#include "modvoucher.h"

#if MICROPY_PY_VOUCHER

STATIC mp_obj_t debug_demo(void) {
    printf("[modvoucher.c] debug_demo(): ^^\n");

    // ...

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(debug_demo_obj, debug_demo);

STATIC mp_obj_t debug_test_ffi(void) {
    printf("debug_test_ffi(): ^^\n");

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
STATIC MP_DEFINE_CONST_FUN_OBJ_0(debug_test_ffi_obj, debug_test_ffi);

STATIC mp_obj_t debug_init_psa_crypto(void) {
    printf("[modvoucher.c] debug_init_psa_crypto(): ^^\n");

    vi_init_psa_crypto();

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(debug_init_psa_crypto_obj, debug_init_psa_crypto);

STATIC mp_obj_t debug_get_vch_jada(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vi_get_voucher_jada(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(debug_get_vch_jada_obj, debug_get_vch_jada);

//

STATIC mp_obj_t debug_get_vch_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vi_get_voucher_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(debug_get_vch_F2_00_02_obj, debug_get_vch_F2_00_02);

STATIC mp_obj_t debug_get_masa_pem_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vi_get_masa_pem_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(debug_get_masa_pem_F2_00_02_obj, debug_get_masa_pem_F2_00_02);

//

STATIC mp_obj_t debug_get_key_pem_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vi_get_key_pem_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(debug_get_key_pem_F2_00_02_obj, debug_get_key_pem_F2_00_02);

STATIC mp_obj_t debug_get_device_crt_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vi_get_device_crt_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(debug_get_device_crt_F2_00_02_obj, debug_get_device_crt_F2_00_02);

STATIC mp_obj_t debug_get_vrq_F2_00_02(void) {
    uint8_t *ptr_static;
    size_t sz;

    sz = vi_get_vrq_F2_00_02(&ptr_static);
    return mp_obj_new_bytes(ptr_static, sz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(debug_get_vrq_F2_00_02_obj, debug_get_vrq_F2_00_02);

STATIC mp_obj_t debug_create_vrq_F2_00_02(void) {
    uint8_t *ptr_heap;
    size_t sz_heap;
    mp_obj_t obj;

    sz_heap = vi_create_vrq_F2_00_02(&ptr_heap);
    obj = mp_obj_new_bytes(ptr_heap, sz_heap);
    free(ptr_heap);

    return obj;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(debug_create_vrq_F2_00_02_obj, debug_create_vrq_F2_00_02);

STATIC mp_obj_t debug_sign(mp_obj_t bs_vch, mp_obj_t bs_key, mp_obj_t alg) {
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
STATIC MP_DEFINE_CONST_FUN_OBJ_3(debug_sign_obj, debug_sign);

//

STATIC mp_obj_t debug_dump(mp_obj_t self_in) {
    if (mp_obj_is_type(self_in, &mp_type_bytes)) {
        GET_STR_DATA_LEN(self_in, str_data, str_len);
        vi_dump(str_data, str_len);
        return mp_const_none;
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("'voucher' arg must be <class 'bytes'>"));
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(debug_dump_obj, debug_dump);

STATIC mp_obj_t debug_validate(size_t n_args, const mp_obj_t *args) {
    // printf("debug_validate(): n_args: %d\n", n_args);

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
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(debug_validate_obj, 1, 2, debug_validate);

//

typedef struct _mp_obj_vou_t {
    mp_obj_base_t base;
    vi_provider_t *provider;
} mp_obj_vou_t;

#define MP_OBJ_TO_PROVIDER_PTR(obj)  (((mp_obj_vou_t *) MP_OBJ_TO_PTR(obj))->provider)

STATIC mp_obj_t mp_vou_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args, bool is_vrq) {
    mp_arg_check_num(n_args, n_kw, 0, 0, false);

    mp_obj_vou_t *obj = m_new_obj_with_finaliser(mp_obj_vou_t);
    obj->base.type = type;
    vi_provider_allocate(&obj->provider, is_vrq);

    return MP_OBJ_FROM_PTR(obj);
}

mp_obj_t mp_vrq_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    return mp_vou_make_new(type, n_args, n_kw, args, true);
}

mp_obj_t mp_vch_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    return mp_vou_make_new(type, n_args, n_kw, args, false);
}

STATIC mp_obj_t mp_vou_from_cbor(mp_obj_t cbor) {
    if (!mp_obj_is_type(cbor, &mp_type_bytes)) {
        mp_raise_ValueError(MP_ERROR_TEXT("'cbor' arg must be <class 'bytes'>"));
    }
    GET_STR_DATA_LEN(cbor, str_data, str_len);

    mp_obj_vou_t *obj = m_new_obj_with_finaliser(mp_obj_vou_t);
    if (!vi_provider_allocate_from_cbor(&obj->provider, str_data, str_len)) {
        mp_raise_ValueError(MP_ERROR_TEXT("bad cbor voucher"));
    }
    obj->base.type = vi_provider_is_vrq(obj->provider) ?
        &voucher_vrq_type : &voucher_vch_type;

    return MP_OBJ_FROM_PTR(obj);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mp_vou_from_cbor_obj, mp_vou_from_cbor);

STATIC mp_obj_t mp_vou_to_cbor(mp_obj_t self_in) {
    uint8_t *ptr_heap;
    size_t sz_heap;
    mp_obj_t obj;

    sz_heap = vi_provider_to_cbor(MP_OBJ_TO_PROVIDER_PTR(self_in), &ptr_heap);
    obj = mp_obj_new_bytes(ptr_heap, sz_heap);
    free(ptr_heap);

    return obj;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_vou_to_cbor_obj, mp_vou_to_cbor);

//

STATIC mp_obj_t mp_vou_del(mp_obj_t self_in) {
    vi_provider_free(&MP_OBJ_TO_PROVIDER_PTR(self_in));

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_vou_del_obj, mp_vou_del);

STATIC mp_obj_t mp_vou_dump(mp_obj_t self_in) {
    vi_provider_dump(MP_OBJ_TO_PROVIDER_PTR(self_in));

    return self_in;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_vou_dump_obj, mp_vou_dump);

STATIC mp_obj_t mp_vou_len(mp_obj_t self_in) {
    size_t len = vi_provider_len(MP_OBJ_TO_PROVIDER_PTR(self_in));

    return mp_obj_new_int_from_uint(len);
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_vou_len_obj, mp_vou_len);

STATIC mp_obj_t mp_vou_set(mp_obj_t self_in, mp_obj_t attr_key_in, mp_obj_t attr_val_in) {
    vi_provider_t *ptr = MP_OBJ_TO_PROVIDER_PTR(self_in);
    printf("mp_vou_set(): provider: %p\n", ptr);

    mp_uint_t key = mp_obj_get_int(attr_key_in);

    // Note: py/obj.h
    //   #define mp_obj_is_type(o, t) (...) // this does not work for checking int, str or fun; use below macros for that
    //   ...
    bool result;
    if (mp_obj_is_int(attr_val_in)) { // Yang::{Enumeration,DateAndTime}
        mp_uint_t val = mp_obj_get_int(attr_val_in);
        printf("(int) val: %d\n", val);
        result = vi_provider_set_int(ptr, key, val);
    } else if (mp_obj_is_bool(attr_val_in)) { // Yang::Boolean
        bool val = mp_obj_get_int(attr_val_in);
        printf("(bool) val: %d\n", val);
        result = vi_provider_set_bool(ptr, key, val);
    } else if (mp_obj_is_str(attr_val_in)) { // Yang::String
        GET_STR_DATA_LEN(attr_val_in, str_data, str_len);
        printf("(str) data: '%s' | len: %d\n", str_data, str_len);
        result = vi_provider_set_bytes(ptr, key, str_data, str_len);
    } else if (mp_obj_is_type(attr_val_in, &mp_type_bytes)) { // Yang::Binary
        GET_STR_DATA_LEN(attr_val_in, str_data, str_len);
        if (str_len > 0) {
            printf("(bytes) data[0]: 0x%x | len: %d\n", str_data[0], str_len);
        } else {
            printf("(bytes) len: %d\n", str_len);
        }
        result = vi_provider_set_bytes(ptr, key, str_data, str_len);
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid 'attr_val' type"));
    }

    if (!result) {
        mp_raise_msg_varg(&mp_type_ValueError,
            MP_ERROR_TEXT("'set' operation failed for attr key(%d)"), key);
    }

    return self_in;
}
MP_DEFINE_CONST_FUN_OBJ_3(mp_vou_set_obj, mp_vou_set);

STATIC mp_obj_t mp_vou_sign(mp_obj_t self_in, mp_obj_t privkey_pem, mp_obj_t alg_in) {
    if (!mp_obj_is_type(privkey_pem, &mp_type_bytes)) {
        mp_raise_ValueError(MP_ERROR_TEXT("'pem' arg must be <class 'bytes'>"));
    }
    GET_STR_DATA_LEN(privkey_pem, str_data, str_len);

    mp_uint_t alg = mp_obj_get_int(alg_in);
    if (!vi_provider_sign(MP_OBJ_TO_PROVIDER_PTR(self_in), str_data, str_len, alg)) {
        mp_raise_msg_varg(&mp_type_ValueError,
            MP_ERROR_TEXT("'sign' operation failed for alg(%d)"), alg);
    }

    return self_in;
}
MP_DEFINE_CONST_FUN_OBJ_3(mp_vou_sign_obj, mp_vou_sign);

STATIC mp_obj_t mp_vou_validate(size_t n_args, const mp_obj_t *args) {
    bool result;
    vi_provider_t *ptr = MP_OBJ_TO_PROVIDER_PTR(args[0]);
    if (n_args == 1) { // without PEM (`signer_cert` is used instead)
        result = vi_provider_validate(ptr);
    } else { // with PEM
        if (!mp_obj_is_type(args[1], &mp_type_bytes)) {
            mp_raise_ValueError(MP_ERROR_TEXT("'pem' arg must be <class 'bytes'>"));
        }
        GET_STR_DATA_LEN(args[1], str_data, str_len);
        result = vi_provider_validate_with_pem(ptr, str_data, str_len);
    }

    return mp_obj_new_bool(result);
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mp_vou_validate_obj, 1, 2, mp_vou_validate);

//

STATIC const mp_rom_map_elem_t mp_module_voucher_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_voucher) },
    { MP_ROM_QSTR(MP_QSTR_vrq), MP_ROM_PTR(&voucher_vrq_type) },
    { MP_ROM_QSTR(MP_QSTR_vch), MP_ROM_PTR(&voucher_vch_type) },
    { MP_ROM_QSTR(MP_QSTR_from_cbor), MP_ROM_PTR(&mp_vou_from_cbor_obj) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_ASSERTION), MP_ROM_INT(ATTR_ASSERTION) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_CREATED_ON), MP_ROM_INT(ATTR_CREATED_ON) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_DOMAIN_CERT_REVOCATION_CHECKS), MP_ROM_INT(ATTR_DOMAIN_CERT_REVOCATION_CHECKS) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_EXPIRES_ON), MP_ROM_INT(ATTR_EXPIRES_ON) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_IDEVID_ISSUER), MP_ROM_INT(ATTR_IDEVID_ISSUER) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_LAST_RENEWAL_DATE), MP_ROM_INT(ATTR_LAST_RENEWAL_DATE) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_NONCE), MP_ROM_INT(ATTR_NONCE) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_PINNED_DOMAIN_CERT), MP_ROM_INT(ATTR_PINNED_DOMAIN_CERT) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_PINNED_DOMAIN_PUBK), MP_ROM_INT(ATTR_PINNED_DOMAIN_PUBK) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_PINNED_DOMAIN_PUBK_SHA256), MP_ROM_INT(ATTR_PINNED_DOMAIN_PUBK_SHA256) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_PRIOR_SIGNED_VOUCHER_REQUEST), MP_ROM_INT(ATTR_PRIOR_SIGNED_VOUCHER_REQUEST) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_PROXIMITY_REGISTRAR_CERT), MP_ROM_INT(ATTR_PROXIMITY_REGISTRAR_CERT) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_PROXIMITY_REGISTRAR_PUBK), MP_ROM_INT(ATTR_PROXIMITY_REGISTRAR_PUBK) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_PROXIMITY_REGISTRAR_PUBK_SHA256), MP_ROM_INT(ATTR_PROXIMITY_REGISTRAR_PUBK_SHA256) },
    { MP_ROM_QSTR(MP_QSTR_ATTR_SERIAL_NUMBER), MP_ROM_INT(ATTR_SERIAL_NUMBER) },
    { MP_ROM_QSTR(MP_QSTR_ASSERTION_VERIFIED), MP_ROM_INT(ASSERTION_VERIFIED) },
    { MP_ROM_QSTR(MP_QSTR_ASSERTION_LOGGED), MP_ROM_INT(ASSERTION_LOGGED) },
    { MP_ROM_QSTR(MP_QSTR_ASSERTION_PROXIMITY), MP_ROM_INT(ASSERTION_PROXIMITY) },
    { MP_ROM_QSTR(MP_QSTR_SA_ES256), MP_ROM_INT(SA_ES256) },
    { MP_ROM_QSTR(MP_QSTR_SA_ES384), MP_ROM_INT(SA_ES384) },
    { MP_ROM_QSTR(MP_QSTR_SA_ES512), MP_ROM_INT(SA_ES512) },
    { MP_ROM_QSTR(MP_QSTR_SA_PS256), MP_ROM_INT(SA_PS256) },
    //---- debug/test stuff
    { MP_ROM_QSTR(MP_QSTR_demo), MP_ROM_PTR(&debug_demo_obj) },
    { MP_ROM_QSTR(MP_QSTR_test_ffi), MP_ROM_PTR(&debug_test_ffi_obj) },
    { MP_ROM_QSTR(MP_QSTR_init_psa_crypto), MP_ROM_PTR(&debug_init_psa_crypto_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_vch_jada), MP_ROM_PTR(&debug_get_vch_jada_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_vch_F2_00_02), MP_ROM_PTR(&debug_get_vch_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_masa_pem_F2_00_02), MP_ROM_PTR(&debug_get_masa_pem_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_key_pem_F2_00_02), MP_ROM_PTR(&debug_get_key_pem_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_device_crt_F2_00_02), MP_ROM_PTR(&debug_get_device_crt_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_vrq_F2_00_02), MP_ROM_PTR(&debug_get_vrq_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_create_vrq_F2_00_02), MP_ROM_PTR(&debug_create_vrq_F2_00_02_obj) },
    { MP_ROM_QSTR(MP_QSTR_dump), MP_ROM_PTR(&debug_dump_obj) },
    { MP_ROM_QSTR(MP_QSTR_validate), MP_ROM_PTR(&debug_validate_obj) },
    { MP_ROM_QSTR(MP_QSTR_sign), MP_ROM_PTR(&debug_sign_obj) },
};

STATIC MP_DEFINE_CONST_DICT(mp_module_voucher_globals, mp_module_voucher_globals_table);

const mp_obj_module_t mp_module_voucher = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_voucher_globals,
};

#endif // MICROPY_PY_VOUCHER