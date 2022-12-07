import voucher
from voucher import *  # for `{ATTR,SA}_*`
import gc

#print('dir(voucher):', dir(voucher))
help(voucher)

print('voucher.demo():', voucher.demo())


def test_assert_eq(title, left, right, diag=True):
    result = left == right
    print('[test]', title, ':', '✅' if result else '❌')
    if diag and not result:
        print('test failed with')
        print('  left:', left)
        print('  right:', right)

def test_assert(title, condition):
    test_assert_eq(title, condition, True, diag=False)

if 1:  # test misc.
    test_assert_eq('bytes from list', bytes([17, 34, 51]), b'\x11\x22\x33')
    test_assert_eq('list from bytes', list(b'\x11\x22\x33'), [17, 34, 51])

if 1:  # test `voucher` module
    tpl = voucher.test_ffi()
    test_assert_eq('voucher.test_ffi',
        tpl, (42, False, None, True, False, b'\xa0\xb1\xc2\xd3\xe4\xf5', False))
    # print(tpl)

    voucher.init_psa_crypto()

    bs_jada = voucher.get_vch_jada()
    test_assert_eq('voucher.get_vch_jada', len(bs_jada), 328)
    # print(len(bs_jada), bs_jada, list(bs_jada))

    bs_f2 = voucher.get_vch_F2_00_02()
    test_assert_eq('voucher.get_vch_F2_00_02', len(bs_f2), 771)
    # print(len(bs_f2), bs_f2)

    bs_pem_f2 = voucher.get_masa_pem_F2_00_02()
    test_assert_eq('voucher.get_masa_pem_F2_00_02', len(bs_pem_f2), 684)
    # print(len(bs_pem_f2), bs_pem_f2)

    # The default `MP_RIOT_HEAPSIZE ?= 8192U` set in Makefile is not enough
    try:
        n = len(list(bs_jada))
    except MemoryError:
        n = -1
    test_assert_eq('no MemoryError for simple ops', n, 328)

    test_assert('voucher.validate - jada', voucher.validate(bs_jada))
    test_assert('voucher.validate - F2_00_02', voucher.validate(bs_f2, bs_pem_f2))

    # voucher.dump(bs_jada)  # TODO handle panic on decode failure

    #

    bs_key_pem_f2 = voucher.get_key_pem_F2_00_02()
    test_assert_eq('voucher.get_key_pem_F2_00_02', len(bs_key_pem_f2), 227)

    bs_device_crt_f2 = voucher.get_device_crt_F2_00_02()
    test_assert_eq('voucher.get_device_crt_F2_00_02', len(bs_device_crt_f2), 644)

    #

    bs_vrq_sample = voucher.get_vrq_F2_00_02()
    test_assert_eq('bs_vrq_sample (with bare signature)', len(bs_vrq_sample), 622)

    test_assert('voucher.{sign,validate} - vrq sample F2_00_02 via pubkey',
        voucher.validate(bs_vrq_sample, bs_device_crt_f2))
    test_assert('voucher.{sign,validate} - vrq sample F2_00_02 via privkey',
        voucher.validate(bs_vrq_sample, bs_key_pem_f2))

    #

    bs_vrq = voucher.create_vrq_F2_00_02()
    # print('bs_vrq', list(bs_vrq))
    test_assert_eq('bs_vrq', len(bs_vrq), 555)

    test_assert('voucher.{sign,validate} - validating an unsigned voucher should fail',
        not voucher.validate(bs_vrq, bs_device_crt_f2))

    bs_vrq_signed = voucher.sign(bs_vrq, bs_key_pem_f2, SA_ES256)
    # print('bs_vrq_signed', list(bs_vrq_signed))
    test_assert_eq('bs_vrq_signed (with asn1 signature)', len(bs_vrq_signed), 630)

    test_assert('voucher.{sign,validate} - vrq F2_00_02 via pubkey',
        voucher.validate(bs_vrq_signed, bs_device_crt_f2))
    test_assert('voucher.{sign,validate} - vrq F2_00_02 via privkey',
        voucher.validate(bs_vrq_signed, bs_key_pem_f2))

    #

    for y in range(0, 2):
        vrq = voucher.vrq()
        print('(before gc) heap:', gc.mem_free())
        del vrq
        gc.collect()  # finalizer `mp_vou_del()` should be invoked via `__del__`
        print('(after gc) heap:', gc.mem_free())

    def wip():
        print('@@ ======== WIP ========')

        vrq = voucher.vrq()
        help(vrq)
        vch = voucher.vch()
        help(vch)

        #-- rust
        # vrq.set(Attr::Assertion(Assertion::Proximity))
        #   .set(Attr::CreatedOn(1599086034))
        #   .set(Attr::SerialNumber(b"00-D0-E5-F2-00-02".to_vec()));
        #--
        vrq.set(ATTR_ASSERTION, ASSERTION_PROXIMITY) \
           .set(ATTR_CREATED_ON, 1599086034) \
           .set(ATTR_SERIAL_NUMBER, '00-D0-E5-F2-00-02') \
           .set(ATTR_NONCE, b'\x11\x22\x33') \
           .set(ATTR_DOMAIN_CERT_REVOCATION_CHECKS, True) \
           .dump()

        #

        # https://animagus-minerva.github.io/voucher/doc/minerva_voucher/index.html#2-encoding-a-voucher-into-cbor
        vrq.set(ATTR_ASSERTION, ASSERTION_PROXIMITY) \
           .set(ATTR_CREATED_ON, 1599086034) \
           .set(ATTR_NONCE, bytes([48, 130, 1, 216, 48, 130, 1, 94, 160, 3, 2, 1, 2, 2, 1, 1, 48, 10, 6, 8, 42, 134, 72, 206, 61, 4, 3, 2, 48, 115, 49, 18, 48, 16, 6, 10, 9, 146, 38, 137, 147, 242, 44, 100, 1, 25, 22, 2, 99, 97, 49, 25, 48, 23, 6, 10, 9, 146, 38, 137, 147, 242, 44, 100, 1, 25, 22, 9, 115, 97, 110, 100, 101, 108, 109, 97, 110, 49, 66, 48, 64, 6, 3, 85, 4, 3, 12, 57, 35, 60, 83, 121, 115, 116, 101, 109, 86, 97, 114, 105, 97, 98, 108, 101, 58, 48, 120, 48, 48, 48, 48, 53, 53, 98, 56, 50, 53, 48, 99, 48, 100, 98, 56, 62, 32, 85, 110, 115, 116, 114, 117, 110, 103, 32, 70, 111, 117, 110, 116, 97, 105, 110, 32, 67, 65, 48, 30, 23, 13, 50, 48, 48, 56, 50, 57, 48, 52, 48, 48, 49, 54, 90, 23, 13, 50, 50, 48, 56, 50, 57, 48, 52, 48, 48, 49, 54, 90, 48, 70, 49, 18, 48, 16, 6, 10, 9, 146, 38, 137, 147, 242, 44, 100, 1, 25, 22, 2, 99, 97, 49, 25, 48, 23, 6, 10, 9, 146, 38, 137, 147, 242, 44, 100, 1, 25, 22, 9, 115, 97, 110, 100, 101, 108, 109, 97, 110, 49, 21, 48, 19, 6, 3, 85, 4, 3, 12, 12, 85, 110, 115, 116, 114, 117, 110, 103, 32, 74, 82, 67, 48, 89, 48, 19, 6, 7, 42, 134, 72, 206, 61, 2, 1, 6, 8, 42, 134, 72, 206, 61, 3, 1, 7, 3, 66, 0, 4, 150, 101, 80, 114, 52, 186, 159, 229, 221, 230, 95, 246, 240, 129, 111, 233, 72, 158, 129, 12, 18, 7, 59, 70, 143, 151, 100, 43, 99, 0, 141, 2, 15, 87, 201, 124, 148, 127, 132, 140, 178, 14, 97, 214, 201, 136, 141, 21, 180, 66, 31, 215, 242, 106, 183, 228, 206, 5, 248, 167, 76, 211, 139, 58, 163, 16, 48, 14, 48, 12, 6, 3, 85, 29, 19, 1, 1, 255, 4, 2, 48, 0, 48, 10, 6, 8, 42, 134, 72, 206, 61, 4, 3, 2, 3, 104, 0, 48, 101, 2, 49, 0, 135, 158, 205, 227, 138, 5, 18, 46, 182, 247, 44, 178, 27, 195, 210, 92, 190, 230, 87, 55, 112, 86, 156, 236, 35, 12, 164, 140, 57, 241, 64, 77, 114, 212, 215, 85, 5, 155, 128, 130, 2, 14, 212, 29, 79, 17, 159, 231, 2, 48, 60, 20, 216, 138, 10, 252, 64, 71, 207, 31, 135, 184, 115, 193, 106, 40, 191, 184, 60, 15, 136, 67, 77, 157, 243, 247, 168, 110, 45, 198, 189, 136, 149, 68, 47, 32, 55, 237, 204, 228, 133, 91, 17, 218, 154, 25, 228, 232])) \
           .set(ATTR_PROXIMITY_REGISTRAR_CERT, bytes([102, 114, 118, 85, 105, 90, 104, 89, 56, 80, 110, 86, 108, 82, 75, 67, 73, 83, 51, 113, 77, 81])) \
           .set(ATTR_SERIAL_NUMBER, '00-D0-E5-F2-00-02')

        KEY_PEM_F2_00_02 = voucher.get_key_pem_F2_00_02()        # debug, privkey
        DEVICE_CRT_F2_00_02 = voucher.get_device_crt_F2_00_02()  # debug, pubkey

        test_assert('vrq.validate(DEVICE_CRT_F2_00_02) - with pubkey PEM, should fail (unsigned)',
            not vrq.validate(DEVICE_CRT_F2_00_02))
        test_assert('vrq.validate(KEY_PEM_F2_00_02) - with privkey PEM, should fail (unsigned)',
            not vrq.validate(KEY_PEM_F2_00_02))

        vrq.sign(KEY_PEM_F2_00_02, SA_ES256)##.dump()

        test_assert('vrq.validate(DEVICE_CRT_F2_00_02) - with pubkey PEM',
            vrq.validate(DEVICE_CRT_F2_00_02))
        test_assert('vrq.validate(KEY_PEM_F2_00_02) - with privkey PEM',
            vrq.validate(KEY_PEM_F2_00_02))

        test_assert('vch_jada.validate() - without PEM, `signer_cert` is used instead',
            voucher.from_cbor(voucher.get_vch_jada()).validate())
        test_assert('vch_f2.validate() - without PEM, should fail',
            not voucher.from_cbor(voucher.get_vch_F2_00_02()).validate())
        test_assert('vrq_f2.validate() - without PEM, should fail',
            not voucher.from_cbor(voucher.get_vrq_F2_00_02()).validate())

        #

        try:
            bad_cbor = b'\x11\x22\x33'
            vou = voucher.from_cbor(bad_cbor)
        except ValueError:
            vou = None
        test_assert_eq('from_cbor() - ValueError on bad cbor', vou, None)

        obj_vch_jada = voucher.from_cbor(voucher.get_vch_jada())
        obj_vch_f2 = voucher.from_cbor(voucher.get_vch_F2_00_02())
        obj_vrq_f2 = voucher.from_cbor(voucher.get_vrq_F2_00_02())

        print('* type checks:', type(obj_vch_jada), type(obj_vch_f2), type(obj_vrq_f2))
        test_assert('from_cbor() - type of obj_vch_jada', isinstance(obj_vch_jada, voucher.vch))
        test_assert('from_cbor() - type of obj_vch_f2', isinstance(obj_vch_f2, voucher.vch))
        test_assert('from_cbor() - type of obj_vrq_f2', isinstance(obj_vrq_f2, voucher.vrq))

        test_assert_eq('from_cbor() - attr len of obj_vch_jada', obj_vch_jada.len(), 6)
        test_assert_eq('from_cbor() - attr len of obj_vch_f2', obj_vch_f2.len(), 5)
        test_assert_eq('from_cbor() - attr len of obj_vrq_f2', obj_vrq_f2.len(), 5)

        test_assert_eq('to_cbor() - obj_vch_jada', obj_vch_jada.to_cbor(), voucher.get_vch_jada())
        test_assert_eq('to_cbor() - obj_vch_f2', obj_vch_f2.to_cbor(), voucher.get_vch_F2_00_02())
        test_assert_eq('to_cbor() - obj_vrq_f2', obj_vrq_f2.to_cbor(), voucher.get_vrq_F2_00_02())

        try:
            cbor = voucher.vch().to_cbor()
        except ValueError:
            cbor = None
        test_assert_eq('to_cbor() - exception on `vch` without required attributes', cbor, None)

        try:
            cbor = voucher.vrq() \
                          .set(ATTR_SERIAL_NUMBER, '00-D0-E5-F2-00-02') \
                          .to_cbor()
        except ValueError:
            cbor = None
        test_assert_eq('to_cbor() - exception on `vrq` without required attributes', cbor, None)

        try:
            cbor = voucher.vrq() \
                          .set(ATTR_ASSERTION, ASSERTION_PROXIMITY) \
                          .set(ATTR_SERIAL_NUMBER, '00-D0-E5-F2-00-02') \
                          .to_cbor()
        except ValueError:
            cbor = None
        test_assert_eq('to_cbor() - `vrq` with required attributes', len(cbor), 43)

        #

        vch = voucher.from_cbor(voucher.get_vch_jada())
        vch.dump()

        print('* `.get()` `int` attributes in vch_jada')
        test_assert_eq('get - ATTR_ASSERTION', vch.get(ATTR_ASSERTION), ASSERTION_PROXIMITY)
        test_assert_eq('get - ATTR_CREATED_ON', vch.get(ATTR_CREATED_ON), 1475868702)
        test_assert_eq('get - ATTR_EXPIRES_ON', vch.get(ATTR_EXPIRES_ON), 1506816000)
        test_assert_eq('get - None for ATTR_LAST_RENEWAL_DATE', vch.get(ATTR_LAST_RENEWAL_DATE), None)

        print('* `.get()` `bool` attributes in vch_jada')
        test_assert_eq('get - ATTR_DOMAIN_CERT_REVOCATION_CHECKS', vch.get(ATTR_DOMAIN_CERT_REVOCATION_CHECKS), None)

        v = voucher.vrq().set(ATTR_DOMAIN_CERT_REVOCATION_CHECKS, True)
        test_assert_eq('get - ATTR_DOMAIN_CERT_REVOCATION_CHECKS', v.get(ATTR_DOMAIN_CERT_REVOCATION_CHECKS), True)
        v.set(ATTR_DOMAIN_CERT_REVOCATION_CHECKS, False)
        test_assert_eq('get - ATTR_DOMAIN_CERT_REVOCATION_CHECKS', v.get(ATTR_DOMAIN_CERT_REVOCATION_CHECKS), False)

        print('* `.get()` `bytes` attributes in vch_jada')
        test_assert_eq('get - None for ATTR_IDEVID_ISSUER', vch.get(ATTR_IDEVID_ISSUER), None)
        test_assert_eq('get - ATTR_NONCE', vch.get(ATTR_NONCE), b'abcd12345')
        test_assert_eq('get - None for ATTR_PINNED_DOMAIN_CERT', vch.get(ATTR_PINNED_DOMAIN_CERT), None)
        test_assert_eq('get - ATTR_PINNED_DOMAIN_PUBK_SHA256', vch.get(ATTR_PINNED_DOMAIN_PUBK_SHA256), None)
        test_assert_eq('get - None for ATTR_PRIOR_SIGNED_VOUCHER_REQUEST', vch.get(ATTR_PRIOR_SIGNED_VOUCHER_REQUEST), None)
        test_assert_eq('get - None for ATTR_PROXIMITY_REGISTRAR_CERT', vch.get(ATTR_PROXIMITY_REGISTRAR_CERT), None)
        test_assert_eq('get - None for ATTR_PROXIMITY_REGISTRAR_PUBK', vch.get(ATTR_PROXIMITY_REGISTRAR_PUBK), None)
        test_assert_eq('get - None for ATTR_PROXIMITY_REGISTRAR_PUBK_SHA256', vch.get(ATTR_PROXIMITY_REGISTRAR_PUBK_SHA256), None)
        test_assert_eq('get - ATTR_SERIAL_NUMBER', vch.get(ATTR_SERIAL_NUMBER), b'JADA123456789')

        print('* `.remove()` attributes in vch_jada')
        test_assert_eq('remove - # of attributes before', vch.len(), 6)
        test_assert_eq('remove - ATTR_CREATED_ON', vch.remove(ATTR_CREATED_ON), True)
        test_assert_eq('remove - # of attributes after', vch.len(), 5)
        test_assert_eq('remove - ATTR_CREATED_ON not exist on get', vch.get(ATTR_CREATED_ON), None)
        test_assert_eq('remove - ATTR_CREATED_ON not exist on remove', vch.remove(ATTR_CREATED_ON), False)
        test_assert_eq('remove - False for ATTR_LAST_RENEWAL_DATE', vch.remove(ATTR_LAST_RENEWAL_DATE), False)

        #

        print('* "iterable" through `.getiter`')
        vch = voucher.from_cbor(voucher.get_vch_jada())##.dump()

        print('.getiter - for _ in vch:')
        for kv in vch:
            print('attr - kv:', kv)

        print('.getiter - for _ in enumerate(vch):')
        for item in enumerate(vch):
            print('attr - (at, kv):', item)

        print('* `.subscr`-based getter')
        test_assert_eq('vch[ATTR_ASSERTION]', vch[ATTR_ASSERTION], ASSERTION_PROXIMITY)
        test_assert_eq('vch[ATTR_CREATED_ON]', vch[ATTR_CREATED_ON], 1475868702)
        test_assert_eq('vch[ATTR_EXPIRES_ON]', vch[ATTR_EXPIRES_ON], 1506816000)
        test_assert_eq('None for vch[ATTR_LAST_RENEWAL_DATE]', vch[ATTR_LAST_RENEWAL_DATE], None)
        test_assert_eq('None for vch[ATTR_DOMAIN_CERT_REVOCATION_CHECKS]', vch[ATTR_DOMAIN_CERT_REVOCATION_CHECKS], None)
        test_assert_eq('None for vch[ATTR_IDEVID_ISSUER]', vch[ATTR_IDEVID_ISSUER], None)
        test_assert_eq('vch[ATTR_NONCE]', vch[ATTR_NONCE], b'abcd12345')
        test_assert_eq('None for vch[ATTR_PINNED_DOMAIN_CERT]', vch[ATTR_PINNED_DOMAIN_CERT], None)
        test_assert_eq('None for vch[ATTR_PINNED_DOMAIN_PUBK_SHA256]', vch[ATTR_PINNED_DOMAIN_PUBK_SHA256], None)
        test_assert_eq('None for vch[ATTR_PRIOR_SIGNED_VOUCHER_REQUEST]', vch[ATTR_PRIOR_SIGNED_VOUCHER_REQUEST], None)
        test_assert_eq('None for vch[ATTR_PROXIMITY_REGISTRAR_CERT]', vch[ATTR_PROXIMITY_REGISTRAR_CERT], None)
        test_assert_eq('None for vch[ATTR_PROXIMITY_REGISTRAR_PUBK]', vch[ATTR_PROXIMITY_REGISTRAR_PUBK], None)
        test_assert_eq('None for vch[ATTR_PROXIMITY_REGISTRAR_PUBK_SHA256]', vch[ATTR_PROXIMITY_REGISTRAR_PUBK_SHA256], None)
        test_assert_eq('vch[ATTR_SERIAL_NUMBER]', vch[ATTR_SERIAL_NUMBER], b'JADA123456789')

        print('* `.subscr`-based setter')
        vrq = voucher.vrq()
        vrq[ATTR_ASSERTION] = ASSERTION_PROXIMITY
        vrq[ATTR_CREATED_ON] = 1599086034
        vrq[ATTR_SERIAL_NUMBER] = '00-D0-E5-F2-00-02'
        vrq[ATTR_NONCE] = b'\x11\x22\x33'
        vrq[ATTR_DOMAIN_CERT_REVOCATION_CHECKS] = True
        #vrq.dump()
        test_assert_eq('vrq[ATTR_ASSERTION]', vrq[ATTR_ASSERTION], ASSERTION_PROXIMITY)
        test_assert_eq('vrq[ATTR_CREATED_ON]', vrq[ATTR_CREATED_ON], 1599086034)
        test_assert_eq('None for vrq[ATTR_EXPIRES_ON]', vrq[ATTR_EXPIRES_ON], None)
        test_assert_eq('None for vrq[ATTR_LAST_RENEWAL_DATE]', vrq[ATTR_LAST_RENEWAL_DATE], None)
        test_assert_eq('vrq[ATTR_DOMAIN_CERT_REVOCATION_CHECKS]', vrq[ATTR_DOMAIN_CERT_REVOCATION_CHECKS], True)
        test_assert_eq('None for vrq[ATTR_IDEVID_ISSUER]', vrq[ATTR_IDEVID_ISSUER], None)
        test_assert_eq('vrq[ATTR_NONCE]', vrq[ATTR_NONCE], b'\x11\x22\x33')
        test_assert_eq('None for vrq[ATTR_PINNED_DOMAIN_CERT]', vrq[ATTR_PINNED_DOMAIN_CERT], None)
        test_assert_eq('None for vrq[ATTR_PINNED_DOMAIN_PUBK_SHA256]', vrq[ATTR_PINNED_DOMAIN_PUBK_SHA256], None)
        test_assert_eq('None for vrq[ATTR_PRIOR_SIGNED_VOUCHER_REQUEST]', vrq[ATTR_PRIOR_SIGNED_VOUCHER_REQUEST], None)
        test_assert_eq('None for vrq[ATTR_PROXIMITY_REGISTRAR_CERT]', vrq[ATTR_PROXIMITY_REGISTRAR_CERT], None)
        test_assert_eq('None for vrq[ATTR_PROXIMITY_REGISTRAR_PUBK]', vrq[ATTR_PROXIMITY_REGISTRAR_PUBK], None)
        test_assert_eq('None for vrq[ATTR_PROXIMITY_REGISTRAR_PUBK_SHA256]', vrq[ATTR_PROXIMITY_REGISTRAR_PUBK_SHA256], None)
        test_assert_eq('vrq[ATTR_SERIAL_NUMBER]', vrq[ATTR_SERIAL_NUMBER], b'00-D0-E5-F2-00-02')

        try:
            ret = vch[0:1]
        except ValueError:
            ret = None
        test_assert_eq('.subscr-based slicing is N/A', ret, None)

        ### `.print` _print
        print(vch)
        print(vrq)

        ### `{get,set}_signer_cert()`
        v = from_cbor(get_vch_jada())
        test_assert('validate voucher with signer_cert, should succeed', v.validate())

        cert_orig = v.get_signer_cert()
        test_assert_eq('get_signer_cert - cert_orig', cert_orig, bytes([4, 186, 197, 177, 28, 173, 143, 153, 249, 199, 43, 5, 207, 75, 158, 38, 210, 68, 220, 24, 159, 116, 82, 40, 37, 90, 33, 154, 134, 214, 160, 158, 255, 32, 19, 139, 248, 45, 193, 182, 213, 98, 190, 15, 165, 74, 183, 128, 74, 58, 100, 182, 215, 44, 207, 237, 107, 111, 182, 237, 40, 187, 252, 17, 126]))

        v.set_signer_cert(b'123')
        test_assert_eq('set_signer_cert - invalid cert', v.get_signer_cert(), b'123')
        test_assert('set_signer_cert - validate with invalid cert, should fail', not v.validate())

        v.set_signer_cert(cert_orig)
        test_assert_eq('set_signer_cert - cert_orig', v.get_signer_cert(), cert_orig)
        test_assert('set_signer_cert - validate with cert_orig', v.validate())

        v = from_cbor(get_vch_F2_00_02())
        test_assert('validate voucher without signer_cert, should fail', not v.validate())
        test_assert_eq('get_signer_cert - expect None', v.get_signer_cert(), None)
        v.dump()
        # the CBOR-encoded `COSE_Sign1` structure, a.k.a. `to_verify`
        test_assert_eq('get_content', v.get_content(), bytes([132, 106, 83, 105, 103, 110, 97, 116, 117, 114, 101, 49, 67, 161, 1, 38, 64, 89, 2, 183, 161, 25, 9, 147, 165, 1, 102, 108, 111, 103, 103, 101, 100, 2, 193, 26, 95, 86, 209, 119, 11, 113, 48, 48, 45, 68, 48, 45, 69, 53, 45, 70, 50, 45, 48, 48, 45, 48, 50, 7, 118, 88, 83, 121, 70, 52, 76, 76, 73, 105, 113, 85, 50, 45, 79, 71, 107, 54, 108, 70, 67, 65, 103, 8, 121, 2, 116, 77, 73, 73, 66, 48, 84, 67, 67, 65, 86, 97, 103, 65, 119, 73, 66, 65, 103, 73, 66, 65, 106, 65, 75, 66, 103, 103, 113, 104, 107, 106, 79, 80, 81, 81, 68, 65, 122, 66, 120, 77, 82, 73, 119, 69, 65, 89, 75, 67, 90, 73, 109, 105, 90, 80, 121, 76, 71, 81, 66, 71, 82, 89, 67, 89, 50, 69, 120, 71, 84, 65, 88, 66, 103, 111, 74, 107, 105, 97, 74, 107, 47, 73, 115, 90, 65, 69, 90, 70, 103, 108, 122, 89, 87, 53, 107, 90, 87, 120, 116, 89, 87, 52, 120, 81, 68, 65, 43, 66, 103, 78, 86, 66, 65, 77, 77, 78, 121, 77, 56, 85, 51, 108, 122, 100, 71, 86, 116, 86, 109, 70, 121, 97, 87, 70, 105, 98, 71, 85, 54, 77, 72, 103, 119, 77, 68, 65, 119, 77, 68, 65, 119, 78, 71, 89, 53, 77, 84, 70, 104, 77, 68, 52, 103, 86, 87, 53, 122, 100, 72, 74, 49, 98, 109, 99, 103, 82, 109, 57, 49, 98, 110, 82, 104, 97, 87, 52, 103, 81, 48, 69, 119, 72, 104, 99, 78, 77, 84, 99, 120, 77, 84, 65, 51, 77, 106, 77, 48, 78, 84, 73, 52, 87, 104, 99, 78, 77, 84, 107, 120, 77, 84, 65, 51, 77, 106, 77, 48, 78, 84, 73, 52, 87, 106, 66, 68, 77, 82, 73, 119, 69, 65, 89, 75, 67, 90, 73, 109, 105, 90, 80, 121, 76, 71, 81, 66, 71, 82, 89, 67, 89, 50, 69, 120, 71, 84, 65, 88, 66, 103, 111, 74, 107, 105, 97, 74, 107, 47, 73, 115, 90, 65, 69, 90, 70, 103, 108, 122, 89, 87, 53, 107, 90, 87, 120, 116, 89, 87, 52, 120, 69, 106, 65, 81, 66, 103, 78, 86, 66, 65, 77, 77, 67, 87, 120, 118, 89, 50, 70, 115, 97, 71, 57, 122, 100, 68, 66, 90, 77, 66, 77, 71, 66, 121, 113, 71, 83, 77, 52, 57, 65, 103, 69, 71, 67, 67, 113, 71, 83, 77, 52, 57, 65, 119, 69, 72, 65, 48, 73, 65, 66, 74, 90, 108, 85, 72, 73, 48, 117, 112, 47, 108, 51, 101, 90, 102, 57, 118, 67, 66, 98, 43, 108, 73, 110, 111, 69, 77, 69, 103, 99, 55, 82, 111, 43, 88, 90, 67, 116, 106, 65, 73, 48, 67, 68, 49, 102, 74, 102, 74, 82, 47, 104, 73, 121, 121, 68, 109, 72, 87, 121, 89, 105, 78, 70, 98, 82, 67, 72, 57, 102, 121, 97, 114, 102, 107, 122, 103, 88, 52, 112, 48, 122, 84, 105, 122, 113, 106, 68, 84, 65, 76, 77, 65, 107, 71, 65, 49, 85, 100, 69, 119, 81, 67, 77, 65, 65, 119, 67, 103, 89, 73, 75, 111, 90, 73, 122, 106, 48, 69, 65, 119, 77, 68, 97, 81, 65, 119, 90, 103, 73, 120, 65, 76, 81, 77, 78, 117, 114, 102, 56, 116, 118, 53, 48, 108, 82, 79, 68, 53, 68, 81, 88, 72, 69, 79, 74, 74, 78, 87, 51, 81, 86, 50, 103, 57, 81, 69, 100, 68, 83, 107, 50, 77, 89, 43, 65, 111, 83, 114, 66, 83, 109, 71, 83, 78, 106, 104, 52, 111, 108, 69, 79, 104, 69, 117, 76, 103, 73, 120, 65, 74, 52, 110, 87, 102, 78, 119, 43, 66, 106, 98, 90, 109, 75, 105, 73, 105, 85, 69, 99, 84, 119, 72, 77, 104, 71, 86, 88, 97, 77, 72, 89, 47, 70, 55, 110, 51, 57, 119, 119, 75, 99, 66, 66, 83, 79, 110, 100, 78, 80, 113, 67, 112, 79, 69, 76, 108, 54, 98, 113, 51, 67, 90, 113, 81, 61, 61]))

        ### refactor with 'modvoucher_debug.c'

        #

    wip()

    #