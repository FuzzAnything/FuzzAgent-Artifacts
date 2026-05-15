#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <openssl/aes.h>
}
extern "C" {
#include <openssl/asn1.h>
}
extern "C" {
#include <openssl/asn1err.h>
}
extern "C" {
#include <openssl/asn1t.h>
}
extern "C" {
#include <openssl/async.h>
}
extern "C" {
#include <openssl/asyncerr.h>
}
extern "C" {
#include <openssl/bio.h>
}
extern "C" {
#include <openssl/bioerr.h>
}
extern "C" {
#include <openssl/blowfish.h>
}
extern "C" {
#include <openssl/bn.h>
}
extern "C" {
#include <openssl/bnerr.h>
}
extern "C" {
#include <openssl/buffer.h>
}
extern "C" {
#include <openssl/buffererr.h>
}
extern "C" {
#include <openssl/byteorder.h>
}
extern "C" {
#include <openssl/camellia.h>
}
extern "C" {
#include <openssl/cast.h>
}
extern "C" {
#include <openssl/cmac.h>
}
extern "C" {
#include <openssl/cmp.h>
}
extern "C" {
#include <openssl/cmp_util.h>
}
extern "C" {
#include <openssl/cmperr.h>
}
extern "C" {
#include <openssl/cms.h>
}
extern "C" {
#include <openssl/cmserr.h>
}
extern "C" {
#include <openssl/comp.h>
}
extern "C" {
#include <openssl/comperr.h>
}
extern "C" {
#include <openssl/conf.h>
}
extern "C" {
#include <openssl/conf_api.h>
}
extern "C" {
#include <openssl/conferr.h>
}
extern "C" {
#include <openssl/configuration.h>
}
extern "C" {
#include <openssl/conftypes.h>
}
extern "C" {
#include <openssl/core.h>
}
extern "C" {
#include <openssl/core_dispatch.h>
}
extern "C" {
#include <openssl/core_names.h>
}
extern "C" {
#include <openssl/core_object.h>
}
extern "C" {
#include <openssl/crmf.h>
}
extern "C" {
#include <openssl/crmferr.h>
}
extern "C" {
#include <openssl/crypto.h>
}
extern "C" {
#include <openssl/cryptoerr.h>
}
extern "C" {
#include <openssl/cryptoerr_legacy.h>
}
extern "C" {
#include <openssl/ct.h>
}
extern "C" {
#include <openssl/cterr.h>
}
extern "C" {
#include <openssl/decoder.h>
}
extern "C" {
#include <openssl/decodererr.h>
}
extern "C" {
#include <openssl/des.h>
}
extern "C" {
#include <openssl/dh.h>
}
extern "C" {
#include <openssl/dherr.h>
}
extern "C" {
#include <openssl/dsa.h>
}
extern "C" {
#include <openssl/dsaerr.h>
}
extern "C" {
#include <openssl/dtls1.h>
}
extern "C" {
#include <openssl/e_os2.h>
}
extern "C" {
#include <openssl/e_ostime.h>
}
extern "C" {
#include <openssl/ebcdic.h>
}
extern "C" {
#include <openssl/ec.h>
}
extern "C" {
#include <openssl/ecdh.h>
}
extern "C" {
#include <openssl/ecdsa.h>
}
extern "C" {
#include <openssl/ecerr.h>
}
extern "C" {
#include <openssl/ech.h>
}
extern "C" {
#include <openssl/encoder.h>
}
extern "C" {
#include <openssl/encodererr.h>
}
extern "C" {
#include <openssl/engine.h>
}
extern "C" {
#include <openssl/engineerr.h>
}
extern "C" {
#include <openssl/err.h>
}
extern "C" {
#include <openssl/ess.h>
}
extern "C" {
#include <openssl/esserr.h>
}
extern "C" {
#include <openssl/evp.h>
}
extern "C" {
#include <openssl/evperr.h>
}
extern "C" {
#include <openssl/fips_names.h>
}
extern "C" {
#include <openssl/fipskey.h>
}
extern "C" {
#include <openssl/hmac.h>
}
extern "C" {
#include <openssl/hpke.h>
}
extern "C" {
#include <openssl/http.h>
}
extern "C" {
#include <openssl/httperr.h>
}
extern "C" {
#include <openssl/idea.h>
}
extern "C" {
#include <openssl/indicator.h>
}
extern "C" {
#include <openssl/kdf.h>
}
extern "C" {
#include <openssl/kdferr.h>
}
extern "C" {
#include <openssl/lhash.h>
}
extern "C" {
#include <openssl/macros.h>
}
extern "C" {
#include <openssl/md2.h>
}
extern "C" {
#include <openssl/md4.h>
}
extern "C" {
#include <openssl/md5.h>
}
extern "C" {
#include <openssl/mdc2.h>
}
extern "C" {
#include <openssl/ml_kem.h>
}
extern "C" {
#include <openssl/modes.h>
}
extern "C" {
#include <openssl/obj_mac.h>
}
extern "C" {
#include <openssl/objects.h>
}
extern "C" {
#include <openssl/objectserr.h>
}
extern "C" {
#include <openssl/ocsp.h>
}
extern "C" {
#include <openssl/ocsperr.h>
}
extern "C" {
#include <openssl/opensslconf.h>
}
extern "C" {
#include <openssl/opensslv.h>
}
extern "C" {
#include <openssl/ossl_typ.h>
}
extern "C" {
#include <openssl/param_build.h>
}
extern "C" {
#include <openssl/params.h>
}
extern "C" {
#include <openssl/pem.h>
}
extern "C" {
#include <openssl/pem2.h>
}
extern "C" {
#include <openssl/pemerr.h>
}
extern "C" {
#include <openssl/pkcs12.h>
}
extern "C" {
#include <openssl/pkcs12err.h>
}
extern "C" {
#include <openssl/pkcs7.h>
}
extern "C" {
#include <openssl/pkcs7err.h>
}
extern "C" {
#include <openssl/posix_time.h>
}
extern "C" {
#include <openssl/prov_ssl.h>
}
extern "C" {
#include <openssl/proverr.h>
}
extern "C" {
#include <openssl/provider.h>
}
extern "C" {
#include <openssl/quic.h>
}
extern "C" {
#include <openssl/rand.h>
}
extern "C" {
#include <openssl/randerr.h>
}
extern "C" {
#include <openssl/rc2.h>
}
extern "C" {
#include <openssl/rc4.h>
}
extern "C" {
#include <openssl/rc5.h>
}
extern "C" {
#include <openssl/ripemd.h>
}
extern "C" {
#include <openssl/rsa.h>
}
extern "C" {
#include <openssl/rsaerr.h>
}
extern "C" {
#include <openssl/safestack.h>
}
extern "C" {
#include <openssl/seed.h>
}
extern "C" {
#include <openssl/self_test.h>
}
extern "C" {
#include <openssl/sha.h>
}
extern "C" {
#include <openssl/srp.h>
}
extern "C" {
#include <openssl/srtp.h>
}
extern "C" {
#include <openssl/ssl.h>
}
extern "C" {
#include <openssl/ssl2.h>
}
extern "C" {
#include <openssl/ssl3.h>
}
extern "C" {
#include <openssl/sslerr.h>
}
extern "C" {
#include <openssl/sslerr_legacy.h>
}
extern "C" {
#include <openssl/stack.h>
}
extern "C" {
#include <openssl/store.h>
}
extern "C" {
#include <openssl/storeerr.h>
}
extern "C" {
#include <openssl/symhacks.h>
}
extern "C" {
#include <openssl/thread.h>
}
extern "C" {
#include <openssl/tls1.h>
}
extern "C" {
#include <openssl/trace.h>
}
extern "C" {
#include <openssl/ts.h>
}
extern "C" {
#include <openssl/tserr.h>
}
extern "C" {
#include <openssl/txt_db.h>
}
extern "C" {
#include <openssl/types.h>
}
extern "C" {
#include <openssl/ui.h>
}
extern "C" {
#include <openssl/uierr.h>
}
extern "C" {
#include <openssl/whrlpool.h>
}
extern "C" {
#include <openssl/x509.h>
}
extern "C" {
#include <openssl/x509_acert.h>
}
extern "C" {
#include <openssl/x509_vfy.h>
}
extern "C" {
#include <openssl/x509err.h>
}
extern "C" {
#include <openssl/x509v3.h>
}
extern "C" {
#include <openssl/x509v3err.h>
}
//<ID> 236
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["d2i_OSSL_CRMF_CERTTEMPLATE","DSA_meth_new","DSA_meth_set_paramgen","DSA_meth_free","OSSL_CRMF_CERTTEMPLATE_free","BIO_new_fp","BF_set_key","BF_ecb_encrypt","COMP_zstd_oneshot","OSSL_FUNC_keymgmt_new","OSSL_FUNC_signature_digest_verify_init","PEM_write_bio","ossl_check_IPAddressFamily_sk_type","BIO_free","DSA_meth_free","OSSL_CRMF_CERTTEMPLATE_free"],"critical_calls":["d2i_OSSL_CRMF_CERTTEMPLATE","DSA_meth_new","DSA_meth_set_paramgen","BIO_new_fp","BF_set_key","BF_ecb_encrypt","COMP_zstd_oneshot","OSSL_FUNC_keymgmt_new","OSSL_FUNC_signature_digest_verify_init","PEM_write_bio","ossl_check_IPAddressFamily_sk_type","BIO_free","DSA_meth_free","OSSL_CRMF_CERTTEMPLATE_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse OSSL_CRMF_CERTTEMPLATE from input data
    const unsigned char *in_ptr = data;
    long in_len = size;
    OSSL_CRMF_CERTTEMPLATE *cert_template = d2i_OSSL_CRMF_CERTTEMPLATE(NULL, &in_ptr, in_len);
    
    // 2. Create DSA method and set paramgen callback
    DSA_METHOD *dsa_meth = DSA_meth_new("Fuzzer DSA Method", 0);
    if (dsa_meth) {
        // Create a dummy paramgen function
        auto dummy_paramgen = [](DSA *dsa, int bits, const unsigned char *seed,
                                 int seed_len, int *counter, unsigned long *h,
                                 BN_GENCB *cb) -> int {
            return 0; // Return success for fuzzing
        };
        DSA_meth_set_paramgen(dsa_meth, dummy_paramgen);
    }
    
    // 3. Create FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        DSA_meth_free(dsa_meth);
        OSSL_CRMF_CERTTEMPLATE_free(cert_template);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create FILE* for output
    FILE *out_file = fopen("output_file", "wb");
    
    // 5. Get file descriptors if needed
    int in_fd = fuzz_fileno(in_file);
    int out_fd = out_file ? fuzz_fileno(out_file) : -1;
    
    // 6. Create BIO from FILE for PEM operations
    BIO *bio_out = out_file ? BIO_new_fp(out_file, BIO_NOCLOSE) : NULL;
    
    // 7. Use Blowfish encryption
    BF_KEY bf_key;
    // Initialize with some data from input
    const unsigned char key_data[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    BF_set_key(&bf_key, 8, key_data);
    
    unsigned char bf_in[8] = {0};
    unsigned char bf_out[8] = {0};
    if (size >= 8) {
        memcpy(bf_in, data, 8);
    }
    BF_ecb_encrypt(bf_in, bf_out, &bf_key, BF_ENCRYPT);
    
    // 8. Get Zstd compression method
    COMP_METHOD *zstd_method = COMP_zstd_oneshot();
    
    // 9. Create OSSL_DISPATCH structures for function pointers
    struct ossl_dispatch_st dummy_dispatch[3] = {0};
    
    // 10. Get keymgmt_new function pointer
    OSSL_FUNC_keymgmt_new_fn *keymgmt_new_func = OSSL_FUNC_keymgmt_new(dummy_dispatch);
    
    // 11. Get signature digest verify init function pointer
    OSSL_FUNC_signature_digest_verify_init_fn *sig_digest_verify_init_func = 
        OSSL_FUNC_signature_digest_verify_init(dummy_dispatch);
    
    // 12. Write encrypted data to BIO in PEM format
    if (bio_out) {
        PEM_write_bio(bio_out, "BLOWFISH ENCRYPTED DATA", "FUZZER HEADER", bf_out, sizeof(bf_out));
    }
    
    // 13. Handle IPAddressFamily stack type (dummy usage)
    struct stack_st_IPAddressFamily *ip_stack = NULL;
    OPENSSL_STACK *ossl_stack = ossl_check_IPAddressFamily_sk_type(ip_stack);
    
    // 14. Clean up resources
    if (bio_out) {
        BIO_free(bio_out);
    } else if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    DSA_meth_free(dsa_meth);
    OSSL_CRMF_CERTTEMPLATE_free(cert_template);
    
    // 15. Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(in_fd);
	return 0;
}