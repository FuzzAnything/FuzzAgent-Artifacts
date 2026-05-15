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
//<ID> 260
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 9
//<Quality> {"density":12,"unique_branches":{"SMIME_read_ASN1_ex":[[428,9,428,14,0,0,4,0]],"ASN1_GENERALIZEDTIME_adj":[[74,9,74,19,0,0,4,1],[77,9,77,19,0,0,4,1],[77,23,77,33,0,0,4,1]],"ossl_asn1_time_from_tm":[[255,9,255,29,0,0,4,1],[260,16,260,38,0,0,4,1],[263,16,263,46,0,0,4,1],[283,9,283,39,0,0,4,0],[284,13,284,41,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","BIO_new_mem_buf","BIO_free","BIO_new_fp","BIO_free","ASN1_PCTX_new","SMIME_read_PKCS7","CMS_ContentInfo_print_ctx","PKCS7_free","ASN1_GENERALIZEDTIME_new","ASN1_GENERALIZEDTIME_set","ASN1_GENERALIZEDTIME_free","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_const_CTLOG_sk_type","OPENSSL_sk_free","ossl_check_CTLOG_sk_type","OSSL_STORE_LOADER_new","OSSL_STORE_LOADER_set_open_ex","OSSL_STORE_LOADER_free","BIO_free","ASN1_PCTX_free","BIO_free","BIO_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","BIO_new_mem_buf","BIO_new_fp","ASN1_PCTX_new","SMIME_read_PKCS7","CMS_ContentInfo_print_ctx","PKCS7_free","ASN1_GENERALIZEDTIME_new","ASN1_GENERALIZEDTIME_set","ASN1_GENERALIZEDTIME_free","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_const_CTLOG_sk_type","OPENSSL_sk_free","ossl_check_CTLOG_sk_type","OSSL_STORE_LOADER_new","OSSL_STORE_LOADER_set_open_ex","OSSL_STORE_LOADER_free","BIO_free","ASN1_PCTX_free","BIO_free","BIO_free","ERR_clear_error"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    OPENSSL_init_ssl(0, NULL);
    
    // Create memory BIO from input data
    BIO *in_bio = BIO_new_mem_buf(data, size);
    if (!in_bio) {
        return 0;
    }
    
    // Create output file for CMS printing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        BIO_free(in_bio);
        assert_file_closed(&out_file);
	return 0;
    }
    
    BIO *out_bio = BIO_new_fp(out_file, BIO_NOCLOSE);
    if (!out_bio) {
        assert_file_closed(&out_file);;
        BIO_free(in_bio);
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Create ASN1 printing context
    ASN1_PCTX *pctx = ASN1_PCTX_new();
    
    // 1. Read PKCS7 structure from S/MIME data
    BIO *bcont = NULL;
    PKCS7 *p7 = SMIME_read_PKCS7(in_bio, &bcont);
    
    if (p7) {
        // 2. Convert PKCS7 to CMS_ContentInfo (they share the same structure)
        CMS_ContentInfo *cms = (CMS_ContentInfo *)p7;
        
        // 3. Print CMS content with indentation
        CMS_ContentInfo_print_ctx(out_bio, cms, 4, pctx);
        
        // Free PKCS7/CMS structure
        PKCS7_free(p7);
    }
    
    // 4. Create and set ASN1_GENERALIZEDTIME
    ASN1_GENERALIZEDTIME *gt = ASN1_GENERALIZEDTIME_new();
    if (gt) {
        // Use current time
        time_t now = time(NULL);
        ASN1_GENERALIZEDTIME_set(gt, now);
        ASN1_GENERALIZEDTIME_free(gt);
    }
    
    // 5. Create dummy CTLOG stack and check its type
    struct stack_st_CTLOG *ctlog_stack = sk_CTLOG_new_null();
    if (ctlog_stack) {
        // This is a type-checking function - just call it
        const OPENSSL_STACK *checked_stack = 
            ossl_check_const_CTLOG_sk_type(ctlog_stack);
        (void)checked_stack; // Prevent unused variable warning
        sk_CTLOG_free(ctlog_stack);
    }
    
    // 6. Create dummy OSSL_STORE_LOADER and set open_ex function
    OSSL_STORE_LOADER *loader = OSSL_STORE_LOADER_new(NULL, "dummy");
    if (loader) {
        // Create a dummy open_ex function
        OSSL_STORE_open_ex_fn dummy_open_ex = [](const OSSL_STORE_LOADER *loader,
                                                 const char *uri,
                                                 OSSL_LIB_CTX *libctx,
                                                 const char *propq,
                                                 const UI_METHOD *ui_method,
                                                 void *ui_data) -> OSSL_STORE_LOADER_CTX* {
            return NULL;
        };
        
        // Set the open_ex function
        OSSL_STORE_LOADER_set_open_ex(loader, dummy_open_ex);
        OSSL_STORE_LOADER_free(loader);
    }
    
    // Cleanup
    if (bcont) {
        BIO_free(bcont);
    }
    
    ASN1_PCTX_free(pctx);
    BIO_free(out_bio);
    BIO_free(in_bio);
    assert_file_closed(&out_file);;
    
    // Clear any remaining OpenSSL errors
    ERR_clear_error();
    
    assert_file_closed(&out_file);
	return 0;
}