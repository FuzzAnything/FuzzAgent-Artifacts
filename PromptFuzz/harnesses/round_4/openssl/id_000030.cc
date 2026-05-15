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
//<ID> 428
//<Prompt> []
/*<Combination>: [
*/
//<score> 56, nr_unique_branch: 46
//<Quality> {"density":8,"unique_branches":{"EC_GROUP_set_generator":[[373,9,373,26,0,0,4,1],[379,9,379,29,0,0,4,1],[379,33,379,57,0,0,4,1],[380,12,380,40,0,0,4,1],[390,9,390,22,0,0,4,1],[390,26,390,43,0,0,4,1],[390,47,390,68,0,0,4,1],[391,12,391,62,0,0,4,1],[401,9,401,25,0,0,4,0],[401,29,401,53,0,0,4,1],[406,9,406,33,0,0,4,0],[408,13,408,37,0,0,4,1],[411,9,411,52,0,0,4,1],[414,9,414,45,0,0,4,1],[418,9,418,25,0,0,4,0],[418,29,418,50,0,0,4,0],[419,13,419,55,0,0,4,1],[431,9,431,32,0,0,4,0]],"ec_group_new_from_data":[[3055,9,3055,27,0,0,4,1],[3074,9,3074,73,0,0,4,1],[3075,12,3075,76,0,0,4,1],[3076,12,3076,76,0,0,4,1],[3081,9,3081,22,0,0,4,1],[3086,16,3086,57,0,0,4,0],[3087,13,3087,67,0,0,4,1],[3105,9,3105,42,0,0,4,1],[3110,9,3110,73,0,0,4,1],[3111,12,3111,76,0,0,4,1],[3115,9,3115,62,0,0,4,1],[3119,9,3119,77,0,0,4,1],[3120,12,3120,53,0,0,4,1],[3124,9,3124,52,0,0,4,1],[3128,9,3128,17,0,0,4,1],[3136,9,3136,64,0,0,4,0],[3151,13,3151,28,0,0,4,1],[3155,13,3155,37,0,0,4,1],[3173,9,3173,12,0,0,4,1]],"ossl_ec_wNAF_have_precompute_mult":[[292,5,292,29,1,0,4,1]],"EC_GROUP_have_precompute_mult":[[1218,9,1218,30,0,0,4,0]],"X509_EXTENSION_create_by_OBJ":[[216,9,216,21,0,0,4,1],[216,25,216,38,0,0,4,0],[231,9,231,21,0,0,4,0],[231,25,231,38,0,0,4,0]],"ec_precompute_mont_data":[[1242,9,1242,20,0,0,4,1],[1246,9,1246,33,0,0,4,1],[1249,9,1249,62,0,0,4,1]]},"library_calls":["ERR_new","ERR_set_debug","ERR_set_error","ERR_peek_last_error_data","EC_GROUP_new_by_curve_name","EC_GROUP_have_precompute_mult","OSSL_ALLOWED_ATTRIBUTES_CHOICE_new","OBJ_nid2obj","OSSL_ALLOWED_ATTRIBUTES_CHOICE_free","OBJ_nid2obj","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_create_by_OBJ","X509_EXTENSION_free","ASN1_OCTET_STRING_free","ASN1_OBJECT_free","EC_KEY_METHOD_new","EC_KEY_METHOD_set_compute_key","EC_KEY_METHOD_free","SSL_CTX_new","TLS_method","SSL_new","SSL_set_allow_early_data_cb","SSL_free","SSL_CTX_free","PKEY_USAGE_PERIOD_new","ASN1_GENERALIZEDTIME_new","ASN1_GENERALIZEDTIME_new","PKEY_USAGE_PERIOD_free","EC_GROUP_free","ERR_clear_error"],"critical_calls":["ERR_set_error","ERR_set_debug","ERR_new","ERR_peek_last_error_data","EC_GROUP_new_by_curve_name","EC_GROUP_have_precompute_mult","OSSL_ALLOWED_ATTRIBUTES_CHOICE_new","OBJ_nid2obj","OSSL_ALLOWED_ATTRIBUTES_CHOICE_free","OBJ_nid2obj","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_create_by_OBJ","X509_EXTENSION_free","ASN1_OCTET_STRING_free","ASN1_OBJECT_free","EC_KEY_METHOD_new","EC_KEY_METHOD_set_compute_key","EC_KEY_METHOD_free","SSL_CTX_new","TLS_method","SSL_new","SSL_set_allow_early_data_cb","SSL_free","SSL_CTX_free","PKEY_USAGE_PERIOD_new","ASN1_GENERALIZEDTIME_new","ASN1_GENERALIZEDTIME_new","PKEY_USAGE_PERIOD_free","EC_GROUP_free","ERR_clear_error"],"visited":0}
/**/


// Dummy compute key function for EC_KEY_METHOD
static int  id_000030_dummy_compute_key(unsigned char **pout, size_t *poutlen, 
                            const EC_POINT *pub_key, const EC_KEY *ecdh) {
    *pout = nullptr;
    *poutlen = 0;
    return 0;
}

// Dummy early data callback
static int  id_000030_dummy_early_data_cb(SSL *s, void *arg) {
    return 1; // Allow early data
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // 1. Use ERR_peek_last_error_data - first seed an error
    ERR_put_error(ERR_LIB_EC, 0, EC_R_INVALID_ENCODING, __FILE__, __LINE__);
    const char *err_data;
    int flags;
    unsigned long err = ERR_peek_last_error_data(&err_data, &flags);
    
    // 2. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Create EC_GROUP and check precomputation
    EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    if (group) {
        int has_precompute = EC_GROUP_have_precompute_mult(group);
        (void)has_precompute; // Use variable to avoid unused warning
    }
    
    // 4. Create and free OSSL_ALLOWED_ATTRIBUTES_CHOICE
    OSSL_ALLOWED_ATTRIBUTES_CHOICE *choice = OSSL_ALLOWED_ATTRIBUTES_CHOICE_new();
    if (choice) {
        // Initialize with dummy data
        choice->type = 0;
        choice->choice.attributeType = OBJ_nid2obj(NID_commonName);
        OSSL_ALLOWED_ATTRIBUTES_CHOICE_free(choice);
    }
    
    // 5. Create X509_EXTENSION using error data
    ASN1_OBJECT *obj = OBJ_nid2obj(NID_basic_constraints);
    ASN1_OCTET_STRING *octet_str = ASN1_OCTET_STRING_new();
    if (octet_str && err_data) {
        // Use error string as extension data
        ASN1_OCTET_STRING_set(octet_str, 
                             (const unsigned char*)err_data, 
                             strlen(err_data));
        X509_EXTENSION *ex = nullptr;
        X509_EXTENSION_create_by_OBJ(&ex, obj, 0, octet_str);
        if (ex) {
            X509_EXTENSION_free(ex);
        }
        ASN1_OCTET_STRING_free(octet_str);
    }
    ASN1_OBJECT_free(obj);
    
    // 6. Set up EC_KEY_METHOD with compute key function
    EC_KEY_METHOD *ec_method = EC_KEY_METHOD_new(NULL);
    if (ec_method) {
        EC_KEY_METHOD_set_compute_key(ec_method,  id_000030_dummy_compute_key);
        EC_KEY_METHOD_free(ec_method);
    }
    
    // 7. Set up SSL with early data callback
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    if (ctx) {
        SSL *ssl = SSL_new(ctx);
        if (ssl) {
            SSL_set_allow_early_data_cb(ssl,  id_000030_dummy_early_data_cb, NULL);
            SSL_free(ssl);
        }
        SSL_CTX_free(ctx);
    }
    
    // 8. Create PKEY_USAGE_PERIOD
    PKEY_USAGE_PERIOD *period = PKEY_USAGE_PERIOD_new();
    if (period) {
        // Read some data from input file for the period
        unsigned char buf[100];
        size_t read_len = fread(buf, 1, sizeof(buf) < size ? sizeof(buf) : size, in_file);
        
        // Try to parse as time (simplified - just for fuzzing)
        if (read_len > 0) {
            // Use first byte as notBefore flag
            if (buf[0] & 0x01) {
                period->notBefore = ASN1_GENERALIZEDTIME_new();
            }
            if (buf[0] & 0x02) {
                period->notAfter = ASN1_GENERALIZEDTIME_new();
            }
        }
        PKEY_USAGE_PERIOD_free(period);
    }
    
    // 9. Clean up
    if (group) EC_GROUP_free(group);
    assert_file_closed(&in_file);;
    ERR_clear_error();
    
    assert_file_closed(&in_file);
	return 0;
}