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
//<ID> 167
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"ERR_add_error_txt":[[80,13,80,42,0,0,4,1]]},"library_calls":["OPENSSL_INIT_new","OPENSSL_INIT_set_config_appname","OPENSSL_init_crypto","OPENSSL_INIT_free","ERR_add_error_txt","ERR_clear_error","CONF_modules_unload","d2i_X509_NAME","X509_NAME_free","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","EC_GROUP_free","X509_new","X509_get_serialNumber","ASN1_INTEGER_set","X509_getm_notBefore","X509_getm_notAfter","X509_gmtime_adj","X509_gmtime_adj","X509_get0_signature","X509_free","CMS_ContentInfo_new","X509_new","CMS_add1_recipient_cert","X509_free","ASN1_GENERALIZEDTIME_new","OBJ_nid2obj","ASN1_TYPE_new","CMS_add0_recipient_key","ASN1_GENERALIZEDTIME_free","ASN1_OBJECT_free","ASN1_TYPE_free","CMS_ContentInfo_free","ERR_add_error_txt","ERR_add_error_txt","ERR_clear_error","CONF_modules_unload"],"critical_calls":["OPENSSL_INIT_new","OPENSSL_INIT_set_config_appname","OPENSSL_init_crypto","OPENSSL_INIT_free","d2i_X509_NAME","X509_NAME_free","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","EC_GROUP_free","X509_new","X509_get_serialNumber","ASN1_INTEGER_set","X509_getm_notBefore","X509_getm_notAfter","X509_gmtime_adj","X509_gmtime_adj","X509_get0_signature","X509_free","CMS_ContentInfo_new","X509_new","CMS_add1_recipient_cert","X509_free","ASN1_GENERALIZEDTIME_new","OBJ_nid2obj","ASN1_TYPE_new","CMS_add0_recipient_key","ASN1_GENERALIZEDTIME_free","ASN1_OBJECT_free","ASN1_TYPE_free","CMS_ContentInfo_free","ERR_add_error_txt","ERR_add_error_txt","ERR_clear_error","CONF_modules_unload"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL with settings
    OPENSSL_INIT_SETTINGS *settings = OPENSSL_INIT_new();
    if (settings) {
        OPENSSL_INIT_set_config_appname(settings, "fuzz_app");
        OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, settings);
        OPENSSL_INIT_free(settings);
    }

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ERR_add_error_txt(":", "Failed to create memory file");
        ERR_clear_error();
        CONF_modules_unload(1);
        assert_file_closed(&in_file);
	return 0;
    }

    // Parse X509_NAME from input data
    const unsigned char *p = data;
    X509_NAME *name = d2i_X509_NAME(NULL, &p, size);
    if (name) {
        // Clean up X509_NAME
        X509_NAME_free(name);
    }

    // Create EC_GROUP
    EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_group) {
        EC_GROUP_get_field_type(ec_group);
        EC_GROUP_free(ec_group);
    }

    // Create and manipulate X509 certificate
    X509 *cert = X509_new();
    if (cert) {
        // Set serial number
        ASN1_INTEGER *serial = X509_get_serialNumber(cert);
        if (serial) {
            ASN1_INTEGER_set(serial, 12345);
        }

        // Set validity times
        ASN1_TIME *notBefore = X509_getm_notBefore(cert);
        ASN1_TIME *notAfter = X509_getm_notAfter(cert);
        if (notBefore) {
            X509_gmtime_adj(notBefore, 0);
        }
        if (notAfter) {
            X509_gmtime_adj(notAfter, 365 * 24 * 3600);
        }

        // Get signature info
        const ASN1_BIT_STRING *sig = NULL;
        const X509_ALGOR *algor = NULL;
        X509_get0_signature(&sig, &algor, cert);

        X509_free(cert);
    }

    // Create and manipulate CMS_ContentInfo
    CMS_ContentInfo *cms = CMS_ContentInfo_new();
    if (cms) {
        // Create another X509 certificate for recipient
        X509 *recip_cert = X509_new();
        if (recip_cert) {
            // Add recipient certificate to CMS
            CMS_RecipientInfo *ri_cert = CMS_add1_recipient_cert(cms, recip_cert, 0);
            X509_free(recip_cert);
        }

        // Create objects for recipient key
        ASN1_GENERALIZEDTIME *date = ASN1_GENERALIZEDTIME_new();
        ASN1_OBJECT *obj = OBJ_nid2obj(NID_aes_128_cbc);
        ASN1_TYPE *type = ASN1_TYPE_new();
        
        // Use input data for key and id (if we have enough data)
        unsigned char *key = NULL;
        unsigned char *id = NULL;
        size_t keylen = 0;
        size_t idlen = 0;
        
        if (size >= 32) {
            key = (unsigned char *)data;
            keylen = 16;
            id = (unsigned char *)data + 16;
            idlen = 16;
        } else if (size >= 16) {
            key = (unsigned char *)data;
            keylen = size;
            id = (unsigned char *)data;
            idlen = size;
        } else {
            key = (unsigned char *)data;
            keylen = size;
            id = (unsigned char *)data;
            idlen = size;
        }

        // Add recipient key to CMS
        CMS_RecipientInfo *ri_key = CMS_add0_recipient_key(cms, NID_aes_128_cbc, 
                                                          key, keylen, 
                                                          id, idlen, 
                                                          date, obj, type);

        // Clean up
        if (!ri_key) {
            // If CMS_add0_recipient_key didn't take ownership, free manually
            ASN1_GENERALIZEDTIME_free(date);
            ASN1_OBJECT_free(obj);
            ASN1_TYPE_free(type);
        }
        // Note: CMS_add0_recipient_key takes ownership of date, obj, type on success
        
        CMS_ContentInfo_free(cms);
    }

    // Add error messages
    ERR_add_error_txt(":", "Test error message 1");
    ERR_add_error_txt(":", "Test error message 2");
    
    // Clear errors
    ERR_clear_error();
    
    // Clean up
    assert_file_closed(&in_file);;
    
    // Unload configuration modules
    CONF_modules_unload(1);
    
    assert_file_closed(&in_file);
	return 0;
}