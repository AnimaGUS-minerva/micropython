import voucher
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
    test_assert_eq('bytes from list', bytes([1,2,3]), b'\x01\x02\x03')
    test_assert_eq('list from bytes', list(b'\x01\x02\x03'), [1,2,3])

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

    bs_vrq_signed = voucher.sign(bs_vrq, bs_key_pem_f2)
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
        # vrq.set(vrq.ATTR_ASSERTION, 11)
        # vrq.dump()
        # vrq.set(vrq.ATTR_CREATED_ON, 1599086034)
        # vrq.dump()
        vrq.set(vrq.ATTR_SERIAL_NUMBER, '00-D0-E5-F2-00-02')
        vrq.dump()
        vrq.set(vrq.ATTR_NONCE, b'\x11\x22\x33')
        vrq.dump()
        #--

    wip()

    #