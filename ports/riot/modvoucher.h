#ifndef MICROPY_INCLUDED_RIOT_MODVOUCHER_H
#define MICROPY_INCLUDED_RIOT_MODVOUCHER_H

#include "voucher_if.h"

extern const mp_obj_type_t voucher_vrq_type;
extern const mp_obj_type_t voucher_vch_type;

#if MICROPY_PY_VOUCHER_DEBUG
extern const mp_obj_module_t mp_module_voucher_debug;

MP_DECLARE_CONST_FUN_OBJ_0(debug_demo_obj);
// ...
// ...
#endif

#endif // MICROPY_INCLUDED_RIOT_MODVOUCHER_H