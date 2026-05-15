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
//<ID> 429
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"X509_EXTENSION_set_critical":[[253,20,253,26,0,0,4,0]]},"library_calls":["ERR_peek_last_error_data","PKEY_USAGE_PERIOD_new","EC_GROUP_new_by_curve_name","EC_GROUP_have_precompute_mult","OBJ_nid2obj","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_create_by_OBJ","EC_KEY_METHOD_new","EC_KEY_METHOD_set_compute_key","SSL_CTX_new","TLS_method","SSL_new","SSL_set_allow_early_data_cb","SSL_free","SSL_CTX_free","CRYPTO_malloc","OSSL_ALLOWED_ATTRIBUTES_CHOICE_free","X509_EXTENSION_free","ASN1_OBJECT_free","ASN1_OCTET_STRING_free","EC_GROUP_free","EC_KEY_METHOD_free","PKEY_USAGE_PERIOD_free","ERR_clear_error"],"critical_calls":["ERR_peek_last_error_data","PKEY_USAGE_PERIOD_new","EC_GROUP_new_by_curve_name","EC_GROUP_have_precompute_mult","OBJ_nid2obj","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_create_by_OBJ","EC_KEY_METHOD_new","EC_KEY_METHOD_set_compute_key","SSL_CTX_new","TLS_method","SSL_new","SSL_set_allow_early_data_cb","SSL_free","SSL_CTX_free","CRYPTO_malloc","OSSL_ALLOWED_ATTRIBUTES_CHOICE_free","X509_EXTENSION_free","ASN1_OBJECT_free","ASN1_OCTET_STRING_free","EC_GROUP_free","EC_KEY_METHOD_free","PKEY_USAGE_PERIOD_free","ERR_clear_error"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Use ERR_peek_last_error_data to check for existing errors
    const char *err_data;
    int err_flags;
    unsigned long err = ERR_peek_last_error_data(&err_data, &err_flags);

    // 2. Create and use PKEY_USAGE_PERIOD
    PKEY_USAGE_PERIOD *usage_period = PKEY_USAGE_PERIOD_new();
    if (usage_period) {
        // Try to read PKEY_USAGE_PERIOD from input file
        fseek(in_file, 0, SEEK_SET);
        // Note: We don't have d2i_PKEY_USAGE_PERIOD_fp in provided APIs,
        // so we'll just create and free it
    }

    // 3. Create EC_GROUP and check precomputation
    EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_group) {
        int has_precompute = EC_GROUP_have_precompute_mult(ec_group);
        // Use the result to avoid unused variable warning
        (void)has_precompute;
    }

    // 4. Create ASN1_OBJECT and ASN1_OCTET_STRING for X509_EXTENSION
    ASN1_OBJECT *obj = OBJ_nid2obj(NID_basic_constraints);
    ASN1_OCTET_STRING *octet = ASN1_OCTET_STRING_new();
    
    if (octet && size > 0) {
        // Use input data for ASN1_OCTET_STRING
        size_t copy_size = size > 100 ? 100 : size; // Limit size
        ASN1_OCTET_STRING_set(octet, data, copy_size);
    }

    // 5. Create X509_EXTENSION using the object and octet string
    X509_EXTENSION *ex = NULL;
    if (obj && octet) {
        ex = X509_EXTENSION_create_by_OBJ(NULL, obj, 1, octet);
    }

    // 6. Create and configure EC_KEY_METHOD
    EC_KEY_METHOD *ec_method = EC_KEY_METHOD_new(NULL);
    if (ec_method) {
        // Create a dummy compute_key function
        int (*dummy_compute_key)(unsigned char **, size_t *, 
                                const EC_POINT *, const EC_KEY *) = 
            [](unsigned char **out, size_t *outlen, 
               const EC_POINT *pub_key, const EC_KEY *priv_key) -> int {
                return 0;
            };
        
        EC_KEY_METHOD_set_compute_key(ec_method, dummy_compute_key);
    }

    // 7. Create SSL context and set early data callback
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        SSL *ssl = SSL_new(ssl_ctx);
        if (ssl) {
            // Create a dummy early data callback
            SSL_allow_early_data_cb_fn dummy_cb = 
                [](SSL *s, void *arg) -> int {
                    return 1;
                };
            
            SSL_set_allow_early_data_cb(ssl, dummy_cb, NULL);
            
            SSL_free(ssl);
        }
        SSL_CTX_free(ssl_ctx);
    }

    // 8. Create and free OSSL_ALLOWED_ATTRIBUTES_CHOICE
    OSSL_ALLOWED_ATTRIBUTES_CHOICE *allowed_attr = 
        (OSSL_ALLOWED_ATTRIBUTES_CHOICE *)OPENSSL_malloc(sizeof(OSSL_ALLOWED_ATTRIBUTES_CHOICE));
    if (allowed_attr) {
        allowed_attr->type = 0;
        allowed_attr->choice.attributeType = NULL;
        OSSL_ALLOWED_ATTRIBUTES_CHOICE_free(allowed_attr);
    }

    // 9. Try to read more data from input file
    unsigned char buffer[256];
    if (in_file) {
        size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
        // Use bytes_read to avoid unused variable warning
        (void)bytes_read;
    }

    // Cleanup
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (ex) {
        X509_EXTENSION_free(ex);
    }
    
    if (obj) {
        ASN1_OBJECT_free(obj);
    }
    
    if (octet) {
        ASN1_OCTET_STRING_free(octet);
    }
    
    if (ec_group) {
        EC_GROUP_free(ec_group);
    }
    
    if (ec_method) {
        EC_KEY_METHOD_free(ec_method);
    }
    
    if (usage_period) {
        PKEY_USAGE_PERIOD_free(usage_period);
    }

    // Clear error queue
    ERR_clear_error();

    assert_file_closed(&in_file);
	return 0;
}