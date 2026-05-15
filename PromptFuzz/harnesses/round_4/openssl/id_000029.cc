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
//<ID> 426
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 5
//<Quality> {"density":7,"unique_branches":{"ossl_ssl_connection_new_int":[[910,17,910,63,0,0,4,0]],"ssl_set_new_record_layer":[[1347,9,1347,18,0,0,4,1],[1350,16,1350,26,0,0,4,0],[1350,30,1350,70,0,0,4,0],[1350,30,1350,70,0,0,4,1]]},"library_calls":["ERR_clear_error","EC_GROUP_new_by_curve_name","EC_GROUP_have_precompute_mult","EC_GROUP_free","CRYPTO_zalloc","OSSL_ALLOWED_ATTRIBUTES_CHOICE_free","OBJ_nid2obj","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_create_by_OBJ","X509_EXTENSION_free","ASN1_OCTET_STRING_free","ASN1_OBJECT_free","EC_KEY_METHOD_new","EC_KEY_METHOD_set_compute_key","EC_KEY_METHOD_free","SSL_CTX_new","TLS_client_method","SSL_new","SSL_set_allow_early_data_cb","SSL_free","SSL_CTX_free","PKEY_USAGE_PERIOD_new","PKEY_USAGE_PERIOD_free","ERR_peek_last_error_data"],"critical_calls":["ERR_clear_error","EC_GROUP_new_by_curve_name","EC_GROUP_have_precompute_mult","EC_GROUP_free","CRYPTO_zalloc","OSSL_ALLOWED_ATTRIBUTES_CHOICE_free","OBJ_nid2obj","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_create_by_OBJ","X509_EXTENSION_free","ASN1_OCTET_STRING_free","ASN1_OBJECT_free","EC_KEY_METHOD_new","EC_KEY_METHOD_set_compute_key","EC_KEY_METHOD_free","SSL_CTX_new","TLS_client_method","SSL_new","SSL_set_allow_early_data_cb","SSL_free","SSL_CTX_free","PKEY_USAGE_PERIOD_new","PKEY_USAGE_PERIOD_free","ERR_peek_last_error_data"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified APIs to achieve a scenario involving certificate extension creation with error handling:

*/


// Dummy callback functions
static int  id_000029_dummy_compute_key(unsigned char **pout, size_t *poutlen, 
                             const EC_POINT *pub_key, const EC_KEY *ecdh) {
    return 0;
}

static int  id_000029_dummy_early_data_cb(SSL *s, void *arg) {
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data
    
    // Initialize OpenSSL error system
    ERR_clear_error();
    
    // 1. Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Read an EC_GROUP from the input (simplified - just create one)
    // In real usage, we'd parse EC parameters from the input
    EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (group) {
        // 3. Use EC_GROUP_have_precompute_mult
        int has_precompute = EC_GROUP_have_precompute_mult(group);
        (void)has_precompute;  // Use result to avoid unused warning
        
        // Clean up EC_GROUP
        EC_GROUP_free(group);
    }
    
    // 4. Create and free OSSL_ALLOWED_ATTRIBUTES_CHOICE
    OSSL_ALLOWED_ATTRIBUTES_CHOICE *attr_choice = NULL;
    // Note: We don't have constructor in API list, so we'll allocate manually
    // In real code, we'd use proper constructor
    attr_choice = (OSSL_ALLOWED_ATTRIBUTES_CHOICE *)OPENSSL_zalloc(sizeof(OSSL_ALLOWED_ATTRIBUTES_CHOICE));
    if (attr_choice) {
        OSSL_ALLOWED_ATTRIBUTES_CHOICE_free(attr_choice);
    }
    
    // 5. Create X509_EXTENSION using input data
    X509_EXTENSION *ex = NULL;
    ASN1_OBJECT *obj = OBJ_nid2obj(NID_basic_constraints);  // Create an ASN1 object
    if (obj) {
        // Create an octet string from part of the input data
        ASN1_OCTET_STRING *octet_str = ASN1_OCTET_STRING_new();
        if (octet_str && size > 0) {
            // Use first min(size, 100) bytes for octet string data
            size_t copy_len = size > 100 ? 100 : size;
            if (ASN1_OCTET_STRING_set(octet_str, data, copy_len)) {
                // Create the extension
                ex = X509_EXTENSION_create_by_OBJ(NULL, obj, 0, octet_str);
                if (ex) {
                    X509_EXTENSION_free(ex);
                }
            }
            ASN1_OCTET_STRING_free(octet_str);
        }
        ASN1_OBJECT_free(obj);
    }
    
    // 6. Create and setup EC_KEY_METHOD
    EC_KEY_METHOD *ec_method = EC_KEY_METHOD_new(NULL);
    if (ec_method) {
        EC_KEY_METHOD_set_compute_key(ec_method,  id_000029_dummy_compute_key);
        EC_KEY_METHOD_free(ec_method);
    }
    
    // 7. Create SSL context and set early data callback
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (ctx) {
        SSL *ssl = SSL_new(ctx);
        if (ssl) {
            SSL_set_allow_early_data_cb(ssl,  id_000029_dummy_early_data_cb, NULL);
            SSL_free(ssl);
        }
        SSL_CTX_free(ctx);
    }
    
    // 8. Create PKEY_USAGE_PERIOD
    PKEY_USAGE_PERIOD *usage_period = PKEY_USAGE_PERIOD_new();
    if (usage_period) {
        // In real usage, we'd populate it with data
        PKEY_USAGE_PERIOD_free(usage_period);
    }
    
    // 9. Check for any OpenSSL errors that occurred
    const char *err_data = NULL;
    int err_flags = 0;
    unsigned long last_err = ERR_peek_last_error_data(&err_data, &err_flags);
    (void)last_err;  // Use result to avoid unused warning
    
    // Clean up
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}