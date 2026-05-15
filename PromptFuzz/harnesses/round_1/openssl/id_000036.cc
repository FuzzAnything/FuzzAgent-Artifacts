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
//<ID> 438
//<Prompt> []
/*<Combination>: [
*/
//<score> 144, nr_unique_branch: 25
//<Quality> {"density":9,"unique_branches":{"PKCS12_SAFEBAG_get1_cert":[[96,9,96,51,0,0,4,0]],"ossl_cms_resolve_libctx":[[102,9,102,23,0,0,4,1]],"asn1_ex_i2c":[[604,5,604,30,0,0,4,0]],"ossl_cms_ctx_get0_propq":[[86,12,86,23,0,0,4,1]],"d2i_CMS_ContentInfo":[[35,47,35,56,0,0,4,0],[41,9,41,19,0,0,4,0],[41,9,41,19,0,0,4,1]],"ossl_cms_SignerInfos_set_cmsctx":[[786,17,786,42,0,0,4,1]],"cms_get_enveloped_type":[[56,9,56,17,0,0,4,0]],"cms_get0_certificate_choices":[[493,5,493,26,0,0,4,1],[496,5,496,29,0,0,4,1],[501,5,501,43,0,0,4,1],[506,5,506,12,0,0,4,0]],"cms_get_enveloped_type_simple":[[41,5,41,29,0,0,4,1],[44,5,44,43,0,0,4,1],[47,5,47,12,0,0,4,0]],"ossl_cms_get0_cmsctx":[[76,12,76,23,0,0,4,1]],"ossl_cms_RecipientInfos_set_cmsctx":[[199,17,199,45,0,0,4,1]],"ossl_cms_ctx_get0_libctx":[[81,12,81,23,0,0,4,1]],"UTF8_getc":[[60,13,60,26,0,0,4,0],[76,13,76,28,0,0,4,0]],"CMS_get0_RecipientInfos":[[181,5,181,32,0,0,4,1],[184,5,184,28,0,0,4,1],[187,5,187,12,0,0,4,0]],"asn1_item_embed_d2i":[[406,21,406,34,0,0,4,0]]},"library_calls":["CRYPTO_THREAD_lock_new","d2i_PKCS12_SAFEBAG","PKCS12_SAFEBAG_get1_cert","d2i_X509_ACERT","X509_ACERT_get0_issuerUID","d2i_CMS_ContentInfo","CMS_get0_RecipientInfos","OPENSSL_sk_num","ossl_check_const_CMS_RecipientInfo_sk_type","OPENSSL_sk_value","ossl_check_const_CMS_RecipientInfo_sk_type","CMS_RecipientInfo_type","CMS_RecipientInfo_kari_get0_reks","CMS_ContentInfo_free","CRYPTO_atomic_add","ERR_peek_error_line","PKCS12_SAFEBAG_free","X509_free","X509_ACERT_free","CRYPTO_THREAD_lock_free","ERR_clear_error"],"critical_calls":["CRYPTO_THREAD_lock_new","d2i_PKCS12_SAFEBAG","PKCS12_SAFEBAG_get1_cert","d2i_X509_ACERT","X509_ACERT_get0_issuerUID","d2i_CMS_ContentInfo","CMS_get0_RecipientInfos","OPENSSL_sk_num","ossl_check_const_CMS_RecipientInfo_sk_type","OPENSSL_sk_value","ossl_check_const_CMS_RecipientInfo_sk_type","CMS_RecipientInfo_type","CMS_RecipientInfo_kari_get0_reks","CMS_ContentInfo_free","CRYPTO_atomic_add","ERR_peek_error_line","PKCS12_SAFEBAG_free","X509_free","X509_ACERT_free","CRYPTO_THREAD_lock_free","ERR_clear_error"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Create FILE* for reading input
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Initialize variables
    PKCS12_SAFEBAG *bag = NULL;
    X509 *cert = NULL;
    X509_ACERT *acert = NULL;
    CMS_RecipientInfo *ri = NULL;
    ASN1_BIT_STRING *issuer_uid = NULL;
    STACK_OF(CMS_RecipientEncryptedKey) *reks = NULL;
    const char *err_file = NULL;
    int err_line = 0;
    CRYPTO_RWLOCK *lock = CRYPTO_THREAD_lock_new();
    int atomic_val = 0;
    int atomic_ret = 0;

    // 1. Read PKCS12 SAFEBAG from input
    // Parse first part of input as PKCS12_SAFEBAG
    const unsigned char *p = data;
    long remaining = size;
    
    // Try to parse as PKCS12_SAFEBAG
    bag = d2i_PKCS12_SAFEBAG(NULL, &p, remaining);
    if (bag) {
        remaining -= (p - data);
        
        // Get certificate from SAFEBAG
        cert = PKCS12_SAFEBAG_get1_cert(bag);
    }

    // 2. Try to parse next part as X509_ACERT
    if (remaining > 0) {
        acert = d2i_X509_ACERT(NULL, &p, remaining);
        if (acert) {
            // Get issuer UID from X509_ACERT
            issuer_uid = (ASN1_BIT_STRING *)X509_ACERT_get0_issuerUID(acert);
            remaining -= (p - (data + (size - remaining)));
        }
    }

    // 3. Try to parse next part as CMS ContentInfo to get RecipientInfo
    if (remaining > 0) {
        CMS_ContentInfo *cms = d2i_CMS_ContentInfo(NULL, &p, remaining);
        if (cms) {
            // Get first recipient info from CMS
            STACK_OF(CMS_RecipientInfo) *ris = CMS_get0_RecipientInfos(cms);
            if (ris && sk_CMS_RecipientInfo_num(ris) > 0) {
                ri = sk_CMS_RecipientInfo_value(ris, 0);
                
                // Get recipient encrypted keys for KARI type
                if (ri && CMS_RecipientInfo_type(ri) == CMS_RECIPINFO_AGREE) {
                    reks = CMS_RecipientInfo_kari_get0_reks(ri);
                }
            }
            CMS_ContentInfo_free(cms);
        }
    }

    // 4. Use CRYPTO_atomic_add for thread-safe counter
    if (lock) {
        CRYPTO_atomic_add(&atomic_val, 1, &atomic_ret, lock);
    }

    // 5. Check for any OpenSSL errors
    unsigned long err = ERR_peek_error_line(&err_file, &err_line);

    // Cleanup
    if (bag) PKCS12_SAFEBAG_free(bag);
    if (cert) X509_free(cert);
    if (acert) X509_ACERT_free(acert);
    if (lock) CRYPTO_THREAD_lock_free(lock);
    assert_file_closed(&in_file);;
    
    // Note: We don't own these pointers, so we don't free them:
    // - issuer_uid (owned by X509_ACERT)
    // - reks (owned by CMS_RecipientInfo)
    // - ri (owned by CMS_ContentInfo which was freed)

    // Clear error queue
    ERR_clear_error();

    assert_file_closed(&in_file);
	return 0;
}