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
//<ID> 244
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_X509_VERIFY_PARAM_sk_type","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get0_trustedStore","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_GENERAL_SUBTREE_copyfunc_type","BIO_new_fp","BIO_get_shutdown","DSA_new","DSA_do_sign","d2i_X509_REQ_fp","EVP_PKEY_new","X509_REQ_check_private_key","DSA_SIG_free","DSA_free","X509_REQ_free","EVP_PKEY_free","OSSL_CMP_CTX_free","BIO_free","BIO_free","OPENSSL_sk_free","ossl_check_X509_VERIFY_PARAM_sk_type","OPENSSL_sk_free","ossl_check_GENERAL_SUBTREE_sk_type"],"critical_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_X509_VERIFY_PARAM_sk_type","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get0_trustedStore","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_GENERAL_SUBTREE_copyfunc_type","BIO_new_fp","BIO_get_shutdown","DSA_new","DSA_do_sign","d2i_X509_REQ_fp","EVP_PKEY_new","X509_REQ_check_private_key","DSA_SIG_free","DSA_free","X509_REQ_free","EVP_PKEY_free","OSSL_CMP_CTX_free","BIO_free","BIO_free","OPENSSL_sk_free","ossl_check_X509_VERIFY_PARAM_sk_type","OPENSSL_sk_free","ossl_check_GENERAL_SUBTREE_sk_type"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Initialize OpenSSL if needed
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    OPENSSL_init_ssl(0, NULL);

    // Create temporary file from input data
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

    BIO *bio_in = NULL;
    BIO *bio_out = NULL;
    DSA *dsa = NULL;
    X509_REQ *req = NULL;
    EVP_PKEY *pkey = NULL;
    OSSL_CMP_CTX *ctx = NULL;
    OPENSSL_STACK *stack = NULL;
    struct stack_st_X509_VERIFY_PARAM *verify_param_stack = NULL;
    DSA_SIG *sig = NULL;

    // 1. Use ossl_check_X509_VERIFY_PARAM_sk_type
    verify_param_stack = sk_X509_VERIFY_PARAM_new_null();
    if (verify_param_stack) {
        stack = ossl_check_X509_VERIFY_PARAM_sk_type(verify_param_stack);
        // Do something with the stack if needed
    }

    // 2. Use OSSL_CMP_CTX_get0_trustedStore
    ctx = OSSL_CMP_CTX_new(NULL, NULL);
    if (ctx) {
        X509_STORE *trusted_store = OSSL_CMP_CTX_get0_trustedStore(ctx);
        // We have the trusted store, could use it for verification
    }

    // 3. Use ossl_check_GENERAL_SUBTREE_copyfunc_type
    // Create a dummy GENERAL_SUBTREE stack and copy function
    struct stack_st_GENERAL_SUBTREE *subtree_stack = sk_GENERAL_SUBTREE_new_null();
    if (subtree_stack) {
        OPENSSL_sk_copyfunc copyfunc = ossl_check_GENERAL_SUBTREE_copyfunc_type(
            (sk_GENERAL_SUBTREE_copyfunc)NULL);
        // The copy function type is checked/returned
    }

    // 4. Use BIO_get_shutdown
    bio_in = BIO_new_fp(in_file, BIO_NOCLOSE);
    if (bio_in) {
        int shutdown_state = BIO_get_shutdown(bio_in);
        // shutdown_state indicates the shutdown state of BIO
    }

    // 5. Use DSA_do_sign - Create a DSA key and sign some data
    dsa = DSA_new();
    if (dsa) {
        // Generate DSA parameters for signing (simplified - in real usage would load a key)
        // We'll use the first 20 bytes of input as digest to sign
        int dlen = size > 20 ? 20 : size;
        sig = DSA_do_sign(data, dlen, dsa);
        // Note: This will likely fail without proper DSA key setup, but we call the API
    }

    // 6. Use X509_REQ_check_private_key
    // Try to load an X509_REQ from the input file
    fseek(in_file, 0, SEEK_SET);
    req = d2i_X509_REQ_fp(in_file, NULL);
    
    if (req) {
        // Create a dummy EVP_PKEY for checking
        pkey = EVP_PKEY_new();
        if (pkey) {
            int check_result = X509_REQ_check_private_key(req, pkey);
            // Result indicates if private key matches (will likely be 0 in this dummy case)
        }
    }

    // Clean up resources
    if (sig) DSA_SIG_free(sig);
    if (dsa) DSA_free(dsa);
    if (req) X509_REQ_free(req);
    if (pkey) EVP_PKEY_free(pkey);
    if (ctx) OSSL_CMP_CTX_free(ctx);
    if (bio_in) BIO_free(bio_in);
    if (bio_out) BIO_free(bio_out);
    if (verify_param_stack) sk_X509_VERIFY_PARAM_free(verify_param_stack);
    if (subtree_stack) sk_GENERAL_SUBTREE_free(subtree_stack);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file if created
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}