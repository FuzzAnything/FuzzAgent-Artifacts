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
//<ID> 277
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 12
//<Quality> {"density":5,"unique_branches":{"ossl_crypto_new_ex_data_ex":[[238,9,238,15,0,0,4,0],[239,13,239,33,0,0,4,0],[243,13,243,28,0,0,4,0],[244,25,244,31,0,0,4,0],[244,25,244,31,0,0,4,1],[249,9,249,15,0,0,4,0],[249,19,249,34,0,0,4,1],[251,17,251,23,0,0,4,0],[252,13,252,31,0,0,4,0],[252,13,252,31,0,0,4,1],[252,35,252,63,0,0,4,1],[258,9,258,25,0,0,4,1]]},"library_calls":["OPENSSL_init_ssl","OSSL_LIB_CTX_new","X509_REQ_new","d2i_X509_REQ_fp","ASN1_BIT_STRING_new","ASN1_STRING_set","X509_REQ_set0_signature","SSL_CTX_new","TLS_method","SSL_CTX_set_verify","UI_new","UI_OpenSSL","UI_set_method","CRYPTO_get_ex_new_index","UI_set_ex_data","X509_STORE_new","X509_STORE_CTX_new","X509_STORE_CTX_init","X509_STORE_CTX_verify","BN_get_rfc2409_prime_1024","ASIdentifiers_new","OSSL_PROVIDER_available","CRYPTO_malloc","CRYPTO_free","ASIdentifiers_free","BN_free","X509_STORE_CTX_free","X509_STORE_free","UI_free","SSL_CTX_free","ASN1_BIT_STRING_free","X509_REQ_free","OSSL_LIB_CTX_free","CMS_EnvelopedData_it"],"critical_calls":["OPENSSL_init_ssl","OSSL_LIB_CTX_new","X509_REQ_new","d2i_X509_REQ_fp","ASN1_BIT_STRING_new","ASN1_STRING_set","X509_REQ_set0_signature","SSL_CTX_new","TLS_method","SSL_CTX_set_verify","UI_new","UI_OpenSSL","UI_set_method","CRYPTO_get_ex_new_index","UI_set_ex_data","X509_STORE_new","X509_STORE_CTX_new","X509_STORE_CTX_init","X509_STORE_CTX_verify","BN_get_rfc2409_prime_1024","ASIdentifiers_new","OSSL_PROVIDER_available","CRYPTO_malloc","CRYPTO_free","ASIdentifiers_free","BN_free","X509_STORE_CTX_free","X509_STORE_free","UI_free","SSL_CTX_free","ASN1_BIT_STRING_free","X509_REQ_free","OSSL_LIB_CTX_free","CMS_EnvelopedData_it"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Initialize OpenSSL
    OPENSSL_init_ssl(0, nullptr);

    // Create FILE pointer from input data
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

    // Create OSSL_LIB_CTX
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    
    // Create X509_REQ
    X509_REQ *req = X509_REQ_new();
    X509_REQ *req_from_file = d2i_X509_REQ_fp(in_file, &req);
    
    // Create ASN1_BIT_STRING
    ASN1_BIT_STRING *bit_string = ASN1_BIT_STRING_new();
    if (bit_string && size > 0) {
        // Use first min(size, INT_MAX) bytes of data
        int len = size > INT_MAX ? INT_MAX : (int)size;
        ASN1_STRING_set(bit_string, data, len);
    }
    
    // Set signature for X509_REQ
    if (req && bit_string) {
        X509_REQ_set0_signature(req, bit_string);
    }
    
    // Create SSL_CTX and set verify callback
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_NONE, nullptr);
    }
    
    // Create UI and set method
    UI *ui = UI_new();
    if (ui) {
        UI_METHOD *ui_method = UI_OpenSSL();
        UI_set_method(ui, ui_method);
        
        // Get ex_data index and set ex_data
        int idx = CRYPTO_get_ex_new_index(0, 0, nullptr, nullptr, nullptr, nullptr);
        if (idx >= 0) {
            UI_set_ex_data(ui, idx, (void *)"test_data");
        }
    }
    
    // Create X509_STORE_CTX and initialize it
    X509_STORE *store = X509_STORE_new();
    X509_STORE_CTX *store_ctx = X509_STORE_CTX_new();
    if (store_ctx && store) {
        // Use a dummy X509 certificate as target (NULL here, but API requires it)
        X509_STORE_CTX_init(store_ctx, store, nullptr, nullptr);
        X509_STORE_CTX_verify(store_ctx);
    }
    
    // Create BIGNUM using RFC2409 prime
    BIGNUM *bn = BN_get_rfc2409_prime_1024(nullptr);
    
    // Create ASIdentifiers
    ASIdentifiers *asid = ASIdentifiers_new();
    
    // Check if provider is available
    if (libctx) {
        OSSL_PROVIDER_available(libctx, "default");
    }
    
    // Allocate memory using CRYPTO_malloc
    void *mem = CRYPTO_malloc(size > 1024 ? 1024 : size, __FILE__, __LINE__);
    if (mem && size > 0) {
        memcpy(mem, data, size > 1024 ? 1024 : size);
    }
    
    // Clean up resources
    if (mem) CRYPTO_free(mem, __FILE__, __LINE__);
    if (asid) ASIdentifiers_free(asid);
    if (bn) BN_free(bn);
    if (store_ctx) X509_STORE_CTX_free(store_ctx);
    if (store) X509_STORE_free(store);
    if (ui) UI_free(ui);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    if (bit_string && !req) ASN1_BIT_STRING_free(bit_string); // Only free if not transferred to req
    if (req) X509_REQ_free(req);
    if (libctx) OSSL_LIB_CTX_free(libctx);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Note: CMS_EnvelopedData_it() is a const function that returns a pointer
    // It doesn't require cleanup, but we call it to satisfy the requirement
    const ASN1_ITEM *cms_it = CMS_EnvelopedData_it();
    (void)cms_it; // Use it to avoid unused variable warning
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}