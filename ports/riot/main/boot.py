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

        vrq.sign(KEY_PEM_F2_00_02, SA_ES256) \
           .dump()

        test_assert('vrq.validate(DEVICE_CRT_F2_00_02) - with pubkey PEM',
            vrq.validate(DEVICE_CRT_F2_00_02))

        test_assert('vrq.validate(KEY_PEM_F2_00_02) - with privkey PEM',
            vrq.validate(KEY_PEM_F2_00_02))

        # TODO
        # vch_jada.validate()  # without PEM (`signer_cert` is used instead)

    wip()

    #