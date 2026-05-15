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
//<ID> 168
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"asn1_string_set_int64":[[360,9,360,14,0,0,4,0]]},"library_calls":["OPENSSL_INIT_new","OPENSSL_INIT_set_config_appname","OPENSSL_init_crypto","OPENSSL_INIT_free","OPENSSL_init_crypto","ERR_clear_error","d2i_X509_NAME","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","CMS_ContentInfo_new","X509_new","X509_get_serialNumber","ASN1_INTEGER_set","X509_getm_notBefore","X509_getm_notAfter","X509_gmtime_adj","X509_gmtime_adj","X509_get0_signature","CMS_add1_recipient_cert","ASN1_GENERALIZEDTIME_new","OBJ_nid2obj","ASN1_TYPE_new","CMS_add0_recipient_key","ASN1_GENERALIZEDTIME_free","ASN1_OBJECT_free","ASN1_TYPE_free","X509_free","CMS_ContentInfo_free","EC_GROUP_free","X509_NAME_free","ERR_add_error_txt","ERR_clear_error","CONF_modules_unload"],"critical_calls":["OPENSSL_INIT_new","OPENSSL_INIT_set_config_appname","OPENSSL_init_crypto","OPENSSL_INIT_free","ERR_clear_error","d2i_X509_NAME","EC_GROUP_new_by_curve_name","EC_GROUP_get_field_type","CMS_ContentInfo_new","X509_new","X509_get_serialNumber","ASN1_INTEGER_set","X509_getm_notBefore","X509_getm_notAfter","X509_gmtime_adj","X509_gmtime_adj","X509_get0_signature","CMS_add1_recipient_cert","ASN1_GENERALIZEDTIME_new","OBJ_nid2obj","ASN1_TYPE_new","CMS_add0_recipient_key","ASN1_GENERALIZEDTIME_free","ASN1_OBJECT_free","ASN1_TYPE_free","X509_free","CMS_ContentInfo_free","EC_GROUP_free","X509_NAME_free","ERR_add_error_txt","ERR_clear_error","CONF_modules_unload"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL with custom settings
    OPENSSL_INIT_SETTINGS *settings = OPENSSL_INIT_new();
    if (settings) {
        OPENSSL_INIT_set_config_appname(settings, "fuzz_app");
        OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, settings);
        OPENSSL_INIT_free(settings);
    } else {
        OPENSSL_init_crypto(0, NULL);
    }

    // Clear any previous errors
    ERR_clear_error();

    // Parse X509_NAME from input data
    X509_NAME *name = NULL;
    const unsigned char *p = data;
    long len = (long)size;
    name = d2i_X509_NAME(NULL, &p, len);
    
    // Create EC group
    EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_group) {
        int field_type = EC_GROUP_get_field_type(ec_group);
        (void)field_type; // Use variable to avoid unused warning
    }

    // Create and populate CMS ContentInfo
    CMS_ContentInfo *cms = CMS_ContentInfo_new();
    if (cms) {
        // Create X509 certificate
        X509 *cert = X509_new();
        if (cert) {
            // Set serial number from first 4 bytes of data
            ASN1_INTEGER *serial = X509_get_serialNumber(cert);
            long serial_val = 0;
            if (size >= sizeof(long)) {
                serial_val = *((long*)data);
            }
            ASN1_INTEGER_set(serial, serial_val);
            
            // Set certificate validity period
            ASN1_TIME *notBefore = X509_getm_notBefore(cert);
            ASN1_TIME *notAfter = X509_getm_notAfter(cert);
            if (notBefore) X509_gmtime_adj(notBefore, 0);
            if (notAfter) X509_gmtime_adj(notAfter, 365*24*3600);
            
            // Get signature info
            const ASN1_BIT_STRING *psig = NULL;
            const X509_ALGOR *palg = NULL;
            X509_get0_signature(&psig, &palg, cert);
            
            // Add recipient certificate to CMS
            CMS_RecipientInfo *ri_cert = CMS_add1_recipient_cert(cms, cert, 0);
            (void)ri_cert; // Use variable
            
            // Create additional recipient with key
            ASN1_GENERALIZEDTIME *date = ASN1_GENERALIZEDTIME_new();
            ASN1_OBJECT *obj = OBJ_nid2obj(NID_pkcs7_data);
            ASN1_TYPE *type = ASN1_TYPE_new();
            
            // Use remaining data for key material
            size_t keylen = size > 32 ? 32 : size;
            size_t idlen = size > 64 ? 32 : (size > 32 ? size - 32 : 0);
            
            unsigned char *key = NULL;
            unsigned char *id = NULL;
            
            if (keylen > 0) key = (unsigned char*)data;
            if (idlen > 0 && size > 32) id = (unsigned char*)data + 32;
            
            CMS_RecipientInfo *ri_key = CMS_add0_recipient_key(cms, NID_aes_128_cbc,
                key, keylen, id, idlen, date, obj, type);
            (void)ri_key; // Use variable
            
            // Cleanup ASN.1 objects
            ASN1_GENERALIZEDTIME_free(date);
            ASN1_OBJECT_free(obj);
            ASN1_TYPE_free(type);
            
            X509_free(cert);
        }
        
        CMS_ContentInfo_free(cms);
    }
    
    // Cleanup EC group
    if (ec_group) {
        EC_GROUP_free(ec_group);
    }
    
    // Cleanup X509_NAME
    if (name) {
        X509_NAME_free(name);
    }
    
    // Add error text for debugging
    ERR_add_error_txt("::", "Fuzzing completed");
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    // Unload configuration modules
    CONF_modules_unload(1);
    
    return 0;
}