#ifndef MICROPY_INCLUDED_RIOT_MODVOUCHER_H
#define MICROPY_INCLUDED_RIOT_MODVOUCHER_H

#include "voucher_if.h"

//

MP_DECLARE_CONST_FUN_OBJ_1(mp_vou_del_obj);
MP_DECLARE_CONST_FUN_OBJ_1(mp_vou_to_cbor_obj);
MP_DECLARE_CONST_FUN_OBJ_1(mp_vou_dump_obj);
MP_DECLARE_CONST_FUN_OBJ_1(mp_vou_len_obj);
MP_DECLARE_CONST_FUN_OBJ_3(mp_vou_set_obj);
MP_DECLARE_CONST_FUN_OBJ_2(mp_vou_get_obj);
MP_DECLARE_CONST_FUN_OBJ_2(mp_vou_remove_obj);
MP_DECLARE_CONST_FUN_OBJ_3(mp_vou_sign_obj);
MP_DECLARE_CONST_FUN_OBJ_VAR_BETWEEN(mp_vou_validate_obj);

const mp_rom_map_elem_t voucher_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&mp_vou_del_obj) },
    { MP_ROM_QSTR(MP_QSTR_to_cbor), MP_ROM_PTR(&mp_vou_to_cbor_obj) },
    { MP_ROM_QSTR(MP_QSTR_dump), MP_ROM_PTR(&mp_vou_dump_obj) },
    { MP_ROM_QSTR(MP_QSTR_len), MP_ROM_PTR(&mp_vou_len_obj) },
    { MP_ROM_QSTR(MP_QSTR_set), MP_ROM_PTR(&mp_vou_set_obj) },
    { MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&mp_vou_get_obj) },
    { MP_ROM_QSTR(MP_QSTR_remove), MP_ROM_PTR(&mp_vou_remove_obj) },
    { MP_ROM_QSTR(MP_QSTR_sign), MP_ROM_PTR(&mp_vou_sign_obj) },
    { MP_ROM_QSTR(MP_QSTR_validate), MP_ROM_PTR(&mp_vou_validate_obj) },
};

MP_DEFINE_CONST_DICT(voucher_locals_dict, voucher_locals_dict_table);

//

mp_obj_t mp_vrq_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);
mp_obj_t mp_vch_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);
mp_obj_t mp_vou_getiter(mp_obj_t self_in, mp_obj_iter_buf_t *iter_buf);
mp_obj_t mp_vou_subscr(mp_obj_t self_in, mp_obj_t attr_key, mp_obj_t attr_val);
void mp_vou_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind);

const mp_obj_type_t voucher_vrq_type = {
    { &mp_type_type },
    .name = MP_QSTR_vrq,
    .make_new = mp_vrq_make_new,
    .getiter = mp_vou_getiter,
    .subscr = mp_vou_subscr,
    .print = mp_vou_print,
    .locals_dict = (void*)&voucher_locals_dict,
};

const mp_obj_type_t voucher_vch_type = {
    { &mp_type_type },
    .name = MP_QSTR_vch,
    .make_new = mp_vch_make_new,
    .getiter = mp_vou_getiter,
    .subscr = mp_vou_subscr,
    .print = mp_vou_print,
    .locals_dict = (void*)&voucher_locals_dict,
};

#endif // MICROPY_INCLUDED_RIOT_MODVOUCHER_H