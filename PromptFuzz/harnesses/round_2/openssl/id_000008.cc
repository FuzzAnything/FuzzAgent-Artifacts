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
//<ID> 64
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 16
//<Quality> {"density":9,"unique_branches":{"ossl_cms_ctx_get0_propq":[[86,12,86,23,0,0,4,0]],"d2i_CMS_ContentInfo":[[41,9,41,19,0,0,4,0]],"CMS_get0_SignerInfos":[[837,12,837,22,0,0,4,1]],"ossl_cms_ctx_get0_libctx":[[81,12,81,23,0,0,4,0]],"ossl_cms_RecipientInfos_set_cmsctx":[[199,17,199,45,0,0,4,1]],"ossl_cms_resolve_libctx":[[102,9,102,23,0,0,4,1]],"cms_get_enveloped_type":[[56,9,56,17,0,0,4,0]],"cms_get_enveloped_type_simple":[[41,5,41,29,0,0,4,1],[44,5,44,43,0,0,4,1],[47,5,47,12,0,0,4,0]],"ossl_cms_SignerInfos_set_cmsctx":[[786,17,786,42,0,0,4,1]],"CMS_get0_RecipientInfos":[[181,5,181,32,0,0,4,1],[184,5,184,28,0,0,4,1],[187,5,187,12,0,0,4,0]],"cms_get0_signed":[[29,9,29,58,0,0,4,0]],"ossl_cms_get0_cmsctx":[[76,12,76,23,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","sk_OCSP_RESPONSE_new","X509_STORE_new","X509_STORE_get1_objects","PKCS7_ATTR_VERIFY_it","d2i_CMS_ContentInfo","CMS_get1_certs","EC_KEY_new_by_curve_name","EC_KEY_generate_key","i2d_ECPrivateKey","PKCS7_new","BIO_new_mem_buf","PKCS7_dataInit","OSSL_CMP_CTX_new","OSSL_CMP_CTX_reset_geninfo_ITAVs","ERR_load_COMP_strings","RSA_meth_new","CRYPTO_free","EC_KEY_free","OPENSSL_sk_pop_free","ossl_check_X509_sk_type","ossl_check_X509_freefunc_type","CMS_ContentInfo_free","BIO_free","BIO_free","PKCS7_free","OSSL_CMP_CTX_free","RSA_meth_free","sk_OCSP_RESPONSE_pop_free","OPENSSL_sk_pop_free","ossl_check_X509_OBJECT_sk_type","ossl_check_X509_OBJECT_freefunc_type","X509_STORE_free"],"critical_calls":["OPENSSL_init_crypto","sk_OCSP_RESPONSE_new","X509_STORE_new","X509_STORE_get1_objects","PKCS7_ATTR_VERIFY_it","d2i_CMS_ContentInfo","CMS_get1_certs","EC_KEY_new_by_curve_name","EC_KEY_generate_key","i2d_ECPrivateKey","PKCS7_new","BIO_new_mem_buf","PKCS7_dataInit","OSSL_CMP_CTX_new","OSSL_CMP_CTX_reset_geninfo_ITAVs","ERR_load_COMP_strings","RSA_meth_new","CRYPTO_free","EC_KEY_free","OPENSSL_sk_pop_free","ossl_check_X509_freefunc_type","ossl_check_X509_sk_type","CMS_ContentInfo_free","BIO_free","BIO_free","PKCS7_free","OSSL_CMP_CTX_free","RSA_meth_free","sk_OCSP_RESPONSE_pop_free","OPENSSL_sk_pop_free","ossl_check_X509_OBJECT_freefunc_type","ossl_check_X509_OBJECT_sk_type","X509_STORE_free"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL if needed
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | OPENSSL_INIT_ADD_ALL_CIPHERS | OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    
    // 1. Create OCSP response stack
    struct stack_st_OCSP_RESPONSE *ocsp_stack = sk_OCSP_RESPONSE_new(NULL);
    
    // 2. Create X509_STORE and get objects
    X509_STORE *store = X509_STORE_new();
    struct stack_st_X509_OBJECT *store_objs = NULL;
    if (store) {
        store_objs = X509_STORE_get1_objects(store);
    }
    
    // 3. Get PKCS7 attribute verify ASN1 item
    const ASN1_ITEM *pkcs7_attr_verify_it = PKCS7_ATTR_VERIFY_it();
    
    // 4. Parse CMS content from input data and get certificates
    CMS_ContentInfo *cms = NULL;
    struct stack_st_X509 *certs = NULL;
    if (size > 0) {
        const unsigned char *p = data;
        cms = d2i_CMS_ContentInfo(NULL, &p, (long)size);
        if (cms) {
            certs = CMS_get1_certs(cms);
        }
    }
    
    // 5. Create EC key and encode to DER
    EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    unsigned char *ec_der = NULL;
    long ec_der_len = 0;
    if (ec_key && EC_KEY_generate_key(ec_key)) {
        ec_der_len = i2d_ECPrivateKey(ec_key, &ec_der);
    }
    
    // 6. Create PKCS7 structure and initialize data
    PKCS7 *p7 = PKCS7_new();
    BIO *bio_data = NULL;
    BIO *p7_bio = NULL;
    if (p7 && size > 0) {
        // Create BIO from input data
        bio_data = BIO_new_mem_buf(data, (int)size);
        if (bio_data) {
            p7_bio = PKCS7_dataInit(p7, bio_data);
        }
    }
    
    // 7. Create CMP context and reset geninfo ITAVs
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(NULL, NULL);
    if (cmp_ctx) {
        OSSL_CMP_CTX_reset_geninfo_ITAVs(cmp_ctx);
    }
    
    // 8. Load COMP error strings
    int comp_strings_loaded = ERR_load_COMP_strings();
    
    // 9. Create RSA method
    RSA_METHOD *rsa_meth = RSA_meth_new("Fuzz RSA Method", 0);
    
    // Cleanup
    
    // Free EC key and DER encoding
    if (ec_der) {
        OPENSSL_free(ec_der);
    }
    if (ec_key) {
        EC_KEY_free(ec_key);
    }
    
    // Free CMS and certificates
    if (certs) {
        sk_X509_pop_free(certs, X509_free);
    }
    if (cms) {
        CMS_ContentInfo_free(cms);
    }
    
    // Free PKCS7 and BIOs
    if (p7_bio) {
        BIO_free(p7_bio);
    }
    if (bio_data) {
        BIO_free(bio_data);
    }
    if (p7) {
        PKCS7_free(p7);
    }
    
    // Free CMP context
    if (cmp_ctx) {
        OSSL_CMP_CTX_free(cmp_ctx);
    }
    
    // Free RSA method
    if (rsa_meth) {
        RSA_meth_free(rsa_meth);
    }
    
    // Free OCSP stack
    if (ocsp_stack) {
        sk_OCSP_RESPONSE_pop_free(ocsp_stack, OCSP_RESPONSE_free);
    }
    
    // Free X509 store objects
    if (store_objs) {
        sk_X509_OBJECT_pop_free(store_objs, X509_OBJECT_free);
    }
    if (store) {
        X509_STORE_free(store);
    }
    
    return 0;
}