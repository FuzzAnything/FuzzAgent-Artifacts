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
//<ID> 511
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"EVP_PKEY_verify_recover":[[1179,9,1179,20,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","ERR_clear_error","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","OSSL_CMP_CTX_new","X509_STORE_new","OSSL_CMP_CTX_build_cert_chain","X509_STORE_CTX_new","X509_STORE_CTX_set_error_depth","ossl_check_ASN1_STRING_TABLE_copyfunc_type","PKCS12_AUTHSAFES_it","d2i_PKCS12_fp","OSSL_ENCODER_fetch","EVP_PKEY_verify_recover","X509_REQ_new","EVP_MD_CTX_new","EVP_DigestSignInit","EVP_sha256","X509_REQ_sign_ctx","OPENSSL_store_u16_le","DSA_meth_new","DSA_meth_set_mod_exp","OSSL_STORE_LOADER_new","OSSL_STORE_LOADER_get0_provider","OPENSSL_LH_new","ossl_check_CONF_VALUE_lh_hashfunc_type","ossl_check_CONF_VALUE_lh_compfunc_type","OPENSSL_LH_set_thunks","ossl_check_X509_sk_type","OPENSSL_LH_free","OSSL_STORE_LOADER_free","DSA_meth_free","X509_REQ_free","EVP_MD_CTX_free","EVP_PKEY_CTX_free","EVP_PKEY_free","OSSL_ENCODER_free","PKCS12_free","X509_STORE_CTX_free","X509_STORE_free","OSSL_CMP_CTX_free","OPENSSL_sk_free"],"critical_calls":["OPENSSL_init_crypto","ERR_clear_error","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","OSSL_CMP_CTX_new","X509_STORE_new","OSSL_CMP_CTX_build_cert_chain","X509_STORE_CTX_new","X509_STORE_CTX_set_error_depth","ossl_check_ASN1_STRING_TABLE_copyfunc_type","PKCS12_AUTHSAFES_it","d2i_PKCS12_fp","OSSL_ENCODER_fetch","EVP_PKEY_verify_recover","X509_REQ_new","EVP_MD_CTX_new","EVP_DigestSignInit","EVP_sha256","X509_REQ_sign_ctx","OPENSSL_store_u16_le","DSA_meth_new","DSA_meth_set_mod_exp","OSSL_STORE_LOADER_new","OSSL_STORE_LOADER_get0_provider","OPENSSL_LH_new","ossl_check_CONF_VALUE_lh_compfunc_type","ossl_check_CONF_VALUE_lh_hashfunc_type","OPENSSL_LH_set_thunks","ossl_check_X509_sk_type","OPENSSL_LH_free","OSSL_STORE_LOADER_free","DSA_meth_free","X509_REQ_free","EVP_MD_CTX_free","EVP_PKEY_CTX_free","EVP_PKEY_free","OSSL_ENCODER_free","PKCS12_free","X509_STORE_CTX_free","X509_STORE_free","OSSL_CMP_CTX_free","OPENSSL_sk_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL
    OPENSSL_init_crypto(0, NULL);
    ERR_clear_error();

    // Create a FILE* from input data for PKCS12 reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Variables to be cleaned up
    OPENSSL_STACK *stack = NULL;
    OSSL_CMP_CTX *cmp_ctx = NULL;
    X509_STORE *store = NULL;
    X509_STORE_CTX *store_ctx = NULL;
    PKCS12 *p12 = NULL;
    OSSL_ENCODER *encoder = NULL;
    X509_REQ *req = NULL;
    EVP_MD_CTX *md_ctx = NULL;
    EVP_PKEY_CTX *pkey_ctx = NULL;
    EVP_PKEY *pkey = NULL;
    DSA_METHOD *dsa_meth = NULL;
    OSSL_STORE_LOADER *loader = NULL;
    OPENSSL_LHASH *lhash = NULL;

    // 1. Create and manipulate OPENSSL_STACK with thunks
    stack = OPENSSL_sk_new_null();
    if (stack) {
        OPENSSL_sk_set_thunks(stack, [](OPENSSL_sk_freefunc freefunc_arg, void *ptr) {
            // Custom thunk function
            if (freefunc_arg && ptr) {
                freefunc_arg(ptr);
            }
        });
    }

    // 2. Create OSSL_CMP context
    cmp_ctx = OSSL_CMP_CTX_new(NULL, NULL);
    
    // 3. Create X509_STORE and build certificate chain
    store = X509_STORE_new();
    if (cmp_ctx && store) {
        OSSL_CMP_CTX_build_cert_chain(cmp_ctx, store, NULL);
    }

    // 4. Create and configure X509_STORE_CTX
    store_ctx = X509_STORE_CTX_new();
    if (store_ctx) {
        X509_STORE_CTX_set_error_depth(store_ctx, 1);
    }

    // 5. Use ASN1_STRING_TABLE copy function type
    OPENSSL_sk_copyfunc copyfunc = ossl_check_ASN1_STRING_TABLE_copyfunc_type(NULL);

    // 6. Get PKCS12 ASN.1 item
    const ASN1_ITEM *p12_item = PKCS12_AUTHSAFES_it();

    // 7. Read PKCS12 from input data
    p12 = d2i_PKCS12_fp(in_file, NULL);

    // 8. Fetch an encoder
    encoder = OSSL_ENCODER_fetch(NULL, "RSA", NULL);

    // 9. Setup EVP_PKEY verification recover (using data as signature)
    if (size > 0) {
        size_t routlen = 0;
        unsigned char *rout = NULL;
        EVP_PKEY_verify_recover(pkey_ctx, rout, &routlen, data, size);
    }

    // 10. Create and sign X509_REQ
    req = X509_REQ_new();
    md_ctx = EVP_MD_CTX_new();
    if (req && md_ctx) {
        EVP_PKEY_CTX *tmp_ctx = NULL;
        if (EVP_DigestSignInit(md_ctx, &tmp_ctx, EVP_sha256(), NULL, pkey)) {
            X509_REQ_sign_ctx(req, md_ctx);
        }
    }

    // 11. Write uint16_t value using OPENSSL_store_u16_le
    if (size >= 2) {
        uint16_t val = (data[0] << 8) | data[1];
        unsigned char out[2];
        OPENSSL_store_u16_le(out, val);
        fwrite(out, 1, 2, out_file);
    }

    // 12. Create and setup DSA method
    dsa_meth = DSA_meth_new("Test DSA", 0);
    if (dsa_meth) {
        DSA_meth_set_mod_exp(dsa_meth, [](DSA *dsa, BIGNUM *r, const BIGNUM *a,
                                          const BIGNUM *p, const BIGNUM *m,
                                          const BIGNUM *e, const BIGNUM *ctx,
                                          BN_CTX *bn_ctx, BN_MONT_CTX *mont) {
            return 1; // Dummy implementation
        });
    }

    // 13. Create OSSL_STORE_LOADER and get provider
    loader = OSSL_STORE_LOADER_new(NULL, "file");
    if (loader) {
        const OSSL_PROVIDER *provider = OSSL_STORE_LOADER_get0_provider(loader);
    }

    // 14. Create and setup OPENSSL_LHASH with thunks
    lhash = OPENSSL_LH_new(
        ossl_check_CONF_VALUE_lh_hashfunc_type(NULL),
        ossl_check_CONF_VALUE_lh_compfunc_type(NULL)
    );
    if (lhash) {
        OPENSSL_LH_set_thunks(lhash,
            [](const void *data, OPENSSL_LH_HASHFUNC hfn) {
                return hfn ? hfn(data) : 0;
            },
            [](const void *data1, const void *data2, OPENSSL_LH_COMPFUNC cmp) {
                return cmp ? cmp(data1, data2) : 0;
            },
            [](void *data, OPENSSL_LH_DOALL_FUNC func) {
                if (func) func(data);
            },
            [](void *data1, void *data2, OPENSSL_LH_DOALL_FUNCARG func) {
                if (func) func(data1, data2);
            }
        );
    }

    // 15. Check X509 stack type
    struct stack_st_X509 *x509_stack = NULL;
    OPENSSL_STACK *checked_stack = ossl_check_X509_sk_type(x509_stack);

    // Cleanup all allocated resources
    if (lhash) OPENSSL_LH_free(lhash);
    if (loader) OSSL_STORE_LOADER_free(loader);
    if (dsa_meth) DSA_meth_free(dsa_meth);
    if (req) X509_REQ_free(req);
    if (md_ctx) EVP_MD_CTX_free(md_ctx);
    if (pkey_ctx) EVP_PKEY_CTX_free(pkey_ctx);
    if (pkey) EVP_PKEY_free(pkey);
    if (encoder) OSSL_ENCODER_free(encoder);
    if (p12) PKCS12_free(p12);
    if (store_ctx) X509_STORE_CTX_free(store_ctx);
    if (store) X509_STORE_free(store);
    if (cmp_ctx) OSSL_CMP_CTX_free(cmp_ctx);
    if (stack) OPENSSL_sk_free(stack);

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}