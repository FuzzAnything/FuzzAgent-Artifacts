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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 9.5, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["OPENSSL_init_crypto","OPENSSL_INIT_new","OPENSSL_INIT_set_config_appname","OPENSSL_INIT_free","d2i_X509_NAME","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","EC_KEY_METHOD_new","EC_KEY_METHOD_set_sign","EC_KEY_METHOD_free","EC_GROUP_free","CMS_ContentInfo_new","X509_new","ASN1_INTEGER_set","X509_get_serialNumber","X509_gmtime_adj","X509_getm_notBefore","X509_gmtime_adj","X509_getm_notAfter","X509_get0_signature","CMS_add1_recipient_cert","ASN1_GENERALIZEDTIME_new","OBJ_nid2obj","ASN1_TYPE_new","CMS_add0_recipient_key","ASN1_GENERALIZEDTIME_free","ASN1_OBJECT_free","ASN1_TYPE_free","X509_free","CMS_ContentInfo_free","ERR_add_error_txt","X509_NAME_free","ERR_clear_error","CONF_modules_unload"],"critical_calls":["OPENSSL_init_crypto","OPENSSL_INIT_new","OPENSSL_INIT_set_config_appname","OPENSSL_INIT_free","d2i_X509_NAME","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","EC_KEY_METHOD_new","EC_KEY_METHOD_set_sign","EC_KEY_METHOD_free","EC_GROUP_free","CMS_ContentInfo_new","X509_new","ASN1_INTEGER_set","X509_get_serialNumber","X509_gmtime_adj","X509_getm_notBefore","X509_gmtime_adj","X509_getm_notAfter","X509_get0_signature","CMS_add1_recipient_cert","ASN1_GENERALIZEDTIME_new","OBJ_nid2obj","ASN1_TYPE_new","CMS_add0_recipient_key","ASN1_GENERALIZEDTIME_free","ASN1_OBJECT_free","ASN1_TYPE_free","X509_free","CMS_ContentInfo_free","ERR_add_error_txt","ERR_clear_error","CONF_modules_unload"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, NULL);
    
    // Create OPENSSL_INIT_SETTINGS and set config appname
    OPENSSL_INIT_SETTINGS *settings = OPENSSL_INIT_new();
    if (settings) {
        OPENSSL_INIT_set_config_appname(settings, "fuzzer_app");
        OPENSSL_INIT_free(settings);
    }
    
    // Parse X509_NAME from input data
    const unsigned char *p = data;
    X509_NAME *name = d2i_X509_NAME(NULL, &p, size);
    
    // Create EC_GROUP and get its field type
    EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_group) {
        int field_type = EC_GROUP_get_field_type(ec_group);
        (void)field_type; // Use the value to avoid unused variable warning
        
        // Create EC_KEY_METHOD and set sign functions
        EC_KEY_METHOD *ec_method = EC_KEY_METHOD_new(NULL);
        if (ec_method) {
            EC_KEY_METHOD_set_sign(ec_method, 
                [](int type, const unsigned char *dgst, int dgst_len,
                   unsigned char *sig, unsigned int *sig_len,
                   const BIGNUM *kinv, const BIGNUM *r, EC_KEY *eckey) {
                    return 0; // Dummy implementation
                },
                NULL,  // sign_setup
                NULL); // sign_sig
            EC_KEY_METHOD_free(ec_method);
        }
        EC_GROUP_free(ec_group);
    }
    
    // Create CMS_ContentInfo structure
    CMS_ContentInfo *cms = CMS_ContentInfo_new();
    if (cms) {
        // Create a dummy X509 certificate for recipient
        X509 *cert = X509_new();
        if (cert) {
            // Set basic fields in certificate
            ASN1_INTEGER_set(X509_get_serialNumber(cert), 1);
            X509_gmtime_adj(X509_get_notBefore(cert), 0);
            X509_gmtime_adj(X509_get_notAfter(cert), 31536000L); // 1 year
            
            // Get signature from certificate (even though it's not signed yet)
            const ASN1_BIT_STRING *sig = NULL;
            const X509_ALGOR *algor = NULL;
            X509_get0_signature(&sig, &algor, cert);
            
            // Add recipient certificate to CMS
            CMS_RecipientInfo *ri_cert = CMS_add1_recipient_cert(cms, cert, 0);
            (void)ri_cert; // Use the value
            
            // Create ASN1 objects for recipient key
            ASN1_GENERALIZEDTIME *date = ASN1_GENERALIZEDTIME_new();
            ASN1_OBJECT *otherTypeId = OBJ_nid2obj(NID_pkcs9_contentType);
            ASN1_TYPE *otherType = ASN1_TYPE_new();
            
            // Use input data for key and ID (split the input)
            size_t key_len = size / 2;
            size_t id_len = size - key_len;
            unsigned char *key_data = (unsigned char*)data;
            unsigned char *id_data = (unsigned char*)data + key_len;
            
            // Add recipient key to CMS
            CMS_RecipientInfo *ri_key = CMS_add0_recipient_key(
                cms, NID_aes_128_cbc, 
                key_data, key_len,
                id_data, id_len,
                date, otherTypeId, otherType);
            (void)ri_key; // Use the value
            
            // Cleanup
            ASN1_GENERALIZEDTIME_free(date);
            ASN1_OBJECT_free(otherTypeId);
            ASN1_TYPE_free(otherType);
            X509_free(cert);
        }
        CMS_ContentInfo_free(cms);
    }
    
    // Add error text if X509_NAME parsing failed
    if (!name) {
        ERR_add_error_txt(":", "Failed to parse X509_NAME from input");
    } else {
        X509_NAME_free(name);
    }
    
    // Clean up any remaining OpenSSL state
    ERR_clear_error();
    CONF_modules_unload(1);
    CRYPTO_cleanup_all_ex_data();
    
    return 0;
}