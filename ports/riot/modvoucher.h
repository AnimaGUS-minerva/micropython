#ifndef MICROPY_INCLUDED_RIOT_MODVOUCHER_H
#define MICROPY_INCLUDED_RIOT_MODVOUCHER_H

#include "voucher_if.h"

extern const mp_obj_type_t voucher_vrq_type;
extern const mp_obj_type_t voucher_vch_type;

#if MICROPY_PY_VOUCHER_DEBUG
extern const mp_obj_module_t mp_module_voucher_debug; // !!!!!! to remove

MP_DECLARE_CONST_FUN_OBJ_0(debug_demo_obj);
MP_DECLARE_CONST_FUN_OBJ_0(debug_test_ffi_obj);
MP_DECLARE_CONST_FUN_OBJ_0(debug_get_vch_jada_obj);
MP_DECLARE_CONST_FUN_OBJ_0(debug_get_vch_F2_00_02_obj);
MP_DECLARE_CONST_FUN_OBJ_0(debug_get_masa_pem_F2_00_02_obj);
MP_DECLARE_CONST_FUN_OBJ_0(debug_get_key_pem_F2_00_02_obj);
MP_DECLARE_CONST_FUN_OBJ_0(debug_get_device_crt_F2_00_02_obj);
MP_DECLARE_CONST_FUN_OBJ_0(debug_get_vrq_F2_00_02_obj);
MP_DECLARE_CONST_FUN_OBJ_0(debug_create_vrq_F2_00_02_obj);
MP_DECLARE_CONST_FUN_OBJ_3(debug_sign_obj);
MP_DECLARE_CONST_FUN_OBJ_1(debug_parse_obj);
MP_DECLARE_CONST_FUN_OBJ_VAR_BETWEEN(debug_validate_obj);
#endif

#endif // MICROPY_INCLUDED_RIOT_MODVOUCHER_H