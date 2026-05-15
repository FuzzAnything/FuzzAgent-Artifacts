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
//<ID> 164
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["OPENSSL_init_crypto","OPENSSL_INIT_new","OPENSSL_INIT_set_config_appname","OPENSSL_INIT_free","d2i_X509_NAME","EC_KEY_METHOD_new","EC_KEY_METHOD_set_sign","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","X509_new","X509_get0_signature","CMS_ContentInfo_new","CMS_add1_recipient_cert","ASN1_GENERALIZEDTIME_new","OBJ_nid2obj","ASN1_TYPE_new","ASN1_TYPE_set","CMS_add0_recipient_key","ASN1_GENERALIZEDTIME_free","ASN1_OBJECT_free","ASN1_TYPE_free","ERR_add_error_txt","X509_NAME_free","EC_KEY_METHOD_free","EC_GROUP_free","X509_free","CMS_ContentInfo_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_crypto","OPENSSL_INIT_new","OPENSSL_INIT_set_config_appname","OPENSSL_INIT_free","d2i_X509_NAME","EC_KEY_METHOD_new","EC_KEY_METHOD_set_sign","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","X509_new","X509_get0_signature","CMS_ContentInfo_new","CMS_add1_recipient_cert","ASN1_GENERALIZEDTIME_new","OBJ_nid2obj","ASN1_TYPE_new","ASN1_TYPE_set","CMS_add0_recipient_key","ASN1_GENERALIZEDTIME_free","ASN1_OBJECT_free","ASN1_TYPE_free","ERR_add_error_txt","X509_NAME_free","EC_KEY_METHOD_free","EC_GROUP_free","X509_free","CMS_ContentInfo_free","ERR_clear_error"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL library
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, NULL);
    
    // Create OPENSSL_INIT_SETTINGS and set config appname
    OPENSSL_INIT_SETTINGS *settings = OPENSSL_INIT_new();
    if (settings) {
        OPENSSL_INIT_set_config_appname(settings, "fuzzer_app");
        // Note: We would normally pass settings to OPENSSL_init_crypto,
        // but for fuzzing we'll just create and free it
        OPENSSL_INIT_free(settings);
    }
    
    // Create a FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Parse X509_NAME from input data
    const unsigned char *p = data;
    X509_NAME *name = d2i_X509_NAME(NULL, &p, size);
    
    // Create EC_KEY_METHOD and set sign function
    EC_KEY_METHOD *ec_method = EC_KEY_METHOD_new(NULL);
    if (ec_method) {
        EC_KEY_METHOD_set_sign(ec_method, 
            [](int type, const unsigned char *dgst, int dlen, 
               unsigned char *sig, unsigned int *siglen, 
               const BIGNUM *kinv, const BIGNUM *r, EC_KEY *eckey) {
                // Simple sign function for fuzzing
                if (sig && siglen && *siglen >= 64) {
                    memset(sig, 0xAA, 64);
                    *siglen = 64;
                    return 1;
                }
                return 0;
            },
            NULL,  // sign_setup
            NULL   // sign_sig
        );
    }
    
    // Create EC_GROUP and get its field type
    EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    if (group) {
        int field_type = EC_GROUP_get_field_type(group);
        (void)field_type; // Use field_type to avoid unused variable warning
    }
    
    // Create a dummy X509 certificate for testing
    X509 *cert = X509_new();
    if (cert) {
        // Get signature information from certificate
        const ASN1_BIT_STRING *psig = NULL;
        const X509_ALGOR *palg = NULL;
        X509_get0_signature(&psig, &palg, cert);
    }
    
    // Create CMS_ContentInfo structure
    CMS_ContentInfo *cms = CMS_ContentInfo_new();
    if (cms) {
        // Add recipient certificate to CMS
        if (cert) {
            CMS_RecipientInfo *ri_cert = CMS_add1_recipient_cert(cms, cert, 0);
            (void)ri_cert;
        }
        
        // Add recipient key to CMS using part of input data as key
        size_t keylen = size > 32 ? 32 : size;
        size_t idlen = size > 16 ? 16 : size;
        
        unsigned char *key_data = (unsigned char*)data;
        unsigned char *id_data = (unsigned char*)data + (keylen % size);
        
        // Create ASN1_GENERALIZEDTIME for testing
        ASN1_GENERALIZEDTIME *time = ASN1_GENERALIZEDTIME_new();
        // Create ASN1_OBJECT for testing
        ASN1_OBJECT *obj = OBJ_nid2obj(NID_commonName);
        // Create ASN1_TYPE for testing
        ASN1_TYPE *type = ASN1_TYPE_new();
        if (type) {
            ASN1_TYPE_set(type, V_ASN1_NULL, NULL);
        }
        
        CMS_RecipientInfo *ri_key = CMS_add0_recipient_key(
            cms, 
            NID_aes_128_cbc, 
            key_data, 
            keylen, 
            id_data, 
            idlen, 
            time, 
            obj, 
            type
        );
        (void)ri_key;
        
        // Clean up CMS objects
        if (time) ASN1_GENERALIZEDTIME_free(time);
        if (obj) ASN1_OBJECT_free(obj);
        if (type) ASN1_TYPE_free(type);
    }
    
    // Add error text for testing
    ERR_add_error_txt(": ", "Fuzzing error context");
    
    // Clean up all allocated resources
    if (name) X509_NAME_free(name);
    if (ec_method) EC_KEY_METHOD_free(ec_method);
    if (group) EC_GROUP_free(group);
    if (cert) X509_free(cert);
    if (cms) CMS_ContentInfo_free(cms);
    
    // Clear OpenSSL error queue
    ERR_clear_error();
    
    // Close the file pointer
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}