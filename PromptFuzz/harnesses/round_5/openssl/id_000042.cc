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
//<ID> 386
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 3
//<Quality> {"density":12,"unique_branches":{"get_error_values":[[489,13,489,26,0,0,4,0],[496,13,496,26,0,0,4,0]],"ERR_add_error_txt":[[69,9,69,17,0,0,4,0]]},"library_calls":["OPENSSL_INIT_new","OPENSSL_INIT_set_config_appname","OPENSSL_init_crypto","OPENSSL_INIT_free","ERR_clear_error","d2i_X509_NAME","X509_NAME_free","ERR_add_error_txt","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","EC_GROUP_free","X509_new","X509_get_serialNumber","ASN1_INTEGER_set","X509_getm_notBefore","X509_free","TLS_method","SSL_CTX_new","SSL_CTX_set_new_pending_conn_cb","BIO_new_fp","BIO_free","SSL_CTX_free","CMS_ContentInfo_new","X509_new","CMS_add1_recipient_cert","X509_free","ASN1_GENERALIZEDTIME_new","OBJ_nid2obj","ASN1_TYPE_new","CMS_add0_recipient_key","ASN1_GENERALIZEDTIME_free","ASN1_OBJECT_free","ASN1_TYPE_free","CMS_ContentInfo_free","ERR_add_error_txt","CONF_modules_unload"],"critical_calls":["OPENSSL_INIT_new","OPENSSL_INIT_set_config_appname","OPENSSL_init_crypto","OPENSSL_INIT_free","ERR_clear_error","d2i_X509_NAME","X509_NAME_free","ERR_add_error_txt","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","EC_GROUP_free","X509_new","X509_get_serialNumber","ASN1_INTEGER_set","X509_getm_notBefore","X509_free","TLS_method","SSL_CTX_new","SSL_CTX_set_new_pending_conn_cb","BIO_new_fp","BIO_free","SSL_CTX_free","CMS_ContentInfo_new","X509_new","CMS_add1_recipient_cert","X509_free","ASN1_GENERALIZEDTIME_new","OBJ_nid2obj","ASN1_TYPE_new","CMS_add0_recipient_key","ASN1_GENERALIZEDTIME_free","ASN1_OBJECT_free","ASN1_TYPE_free","CMS_ContentInfo_free","ERR_add_error_txt","CONF_modules_unload"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize OpenSSL with custom settings
    OPENSSL_INIT_SETTINGS *init_settings = OPENSSL_INIT_new();
    if (init_settings) {
        OPENSSL_INIT_set_config_appname(init_settings, "fuzz_app");
        OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, init_settings);
        OPENSSL_INIT_free(init_settings);
    }
    
    // Clear any previous errors
    ERR_clear_error();
    
    // 2. Parse X509_NAME from input data
    const unsigned char *p = data;
    X509_NAME *name = d2i_X509_NAME(NULL, &p, static_cast<long>(size));
    if (name) {
        X509_NAME_free(name);
    }
    
    // Add error text for debugging
    ERR_add_error_txt(":", "X509_NAME parsing attempted");
    
    // 3. Create and examine EC group
    EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    if (ec_group) {
        EC_GROUP_get_field_type(ec_group);
        EC_GROUP_free(ec_group);
    }
    
    // 4. Create X509 certificate and set properties
    X509 *cert = X509_new();
    if (cert) {
        // Set serial number from first 4 bytes of data
        ASN1_INTEGER *serial = X509_get_serialNumber(cert);
        if (size >= 4) {
            long serial_val = (static_cast<long>(data[0]) << 24) |
                             (static_cast<long>(data[1]) << 16) |
                             (static_cast<long>(data[2]) << 8) |
                             static_cast<long>(data[3]);
            ASN1_INTEGER_set(serial, serial_val);
        }
        
        // Get and set notBefore time (just getting, not setting)
        X509_getm_notBefore(cert);
        X509_free(cert);
    }
    
    // 5. SSL/TLS setup with pending connection callback
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (ctx) {
        SSL_CTX_set_new_pending_conn_cb(ctx, [](SSL_CTX *c, SSL *s, void *arg) -> int {
            return 1;
        }, nullptr);
        
        // Create BIO from input data
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            BIO *bio = BIO_new_fp(in_file, BIO_NOCLOSE);
            if (bio) {
                BIO_free(bio);
            }
            assert_file_closed(&in_file);;
        }
        
        // Create output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            assert_file_closed(&out_file);;
        }
        
        SSL_CTX_free(ctx);
    }
    
    // 6. CMS operations with various ASN.1 types
    CMS_ContentInfo *cms = CMS_ContentInfo_new();
    if (cms) {
        // Create another certificate for recipient
        X509 *recip_cert = X509_new();
        if (recip_cert) {
            CMS_add1_recipient_cert(cms, recip_cert, 0);
            X509_free(recip_cert);
        }
        
        // Create ASN.1 types for recipient key
        ASN1_GENERALIZEDTIME *date = ASN1_GENERALIZEDTIME_new();
        ASN1_OBJECT *obj = OBJ_nid2obj(NID_rsaEncryption);
        ASN1_TYPE *type = ASN1_TYPE_new();
        
        // Use input data for key material if available
        unsigned char *key_data = nullptr;
        size_t key_len = 0;
        if (size > 0) {
            key_data = const_cast<unsigned char*>(data);
            key_len = (size > 32) ? 32 : size;  // Use up to 32 bytes
        }
        
        // Add recipient key
        CMS_add0_recipient_key(cms, NID_rsaEncryption, 
                              key_data, key_len,
                              nullptr, 0,  // No id
                              date, obj, type);
        
        // Free ASN.1 types
        ASN1_GENERALIZEDTIME_free(date);
        ASN1_OBJECT_free(obj);
        ASN1_TYPE_free(type);
        
        CMS_ContentInfo_free(cms);
    }
    
    // 7. Cleanup and final error handling
    ERR_add_error_txt(":", "Fuzzing completed");
    CONF_modules_unload(1);
    
    return 0;
}