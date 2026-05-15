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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["OPENSSL_init_ssl","CMS_sign","BIO_new_fp","CMS_final_digest","BIO_free","CMS_ContentInfo_free","X509_STORE_new","X509_STORE_get_check_issued","X509_STORE_free","OSSL_FUNC_rand_nonce","d2i_X509_ACERT_fp","d2i_X509_ACERT","X509_ACERT_get0_holder_entityName","X509_ACERT_free","SSL_CTX_new","TLS_method","SSL_CTX_get0_client_cert_type","SSL_CTX_free"],"critical_calls":["OPENSSL_init_ssl","CMS_sign","BIO_new_fp","CMS_final_digest","BIO_free","CMS_ContentInfo_free","X509_STORE_new","X509_STORE_get_check_issued","X509_STORE_free","OSSL_FUNC_rand_nonce","d2i_X509_ACERT_fp","d2i_X509_ACERT","X509_ACERT_get0_holder_entityName","X509_ACERT_free","SSL_CTX_new","TLS_method","SSL_CTX_get0_client_cert_type","SSL_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create FILE* for output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize OpenSSL (if needed)
    OPENSSL_init_ssl(0, NULL);

    // 1. Create CMS_ContentInfo and use CMS_final_digest
    {
        // Create a simple CMS SignedData structure
        CMS_ContentInfo *cms = CMS_sign(NULL, NULL, NULL, NULL, 
                                         CMS_NOCERTS | CMS_PARTIAL);
        if (cms) {
            // Create a dummy digest (20 bytes for SHA-1)
            unsigned char md[20] = {0};
            unsigned int mdlen = sizeof(md);
            
            // Use part of input data as digest if available
            if (size >= mdlen) {
                memcpy(md, data, mdlen);
            }
            
            // Create a BIO from input file for dcont
            BIO *dcont = BIO_new_fp(in_file, BIO_NOCLOSE);
            if (dcont) {
                // Reset file position
                rewind(in_file);
                
                // Call CMS_final_digest
                int result = CMS_final_digest(cms, md, mdlen, dcont, 0);
                (void)result; // Result ignored for fuzzing
                
                BIO_free(dcont);
            }
            
            CMS_ContentInfo_free(cms);
        }
    }

    // 2. Create X509_STORE and use X509_STORE_get_check_issued
    {
        X509_STORE *store = X509_STORE_new();
        if (store) {
            // Get the check_issued function
            X509_STORE_CTX_check_issued_fn check_issued = 
                X509_STORE_get_check_issued(store);
            (void)check_issued; // Function pointer may be NULL
            
            X509_STORE_free(store);
        }
    }

    // 3. Use OSSL_FUNC_rand_nonce through a provider dispatch table
    {
        // Create a minimal dispatch table
        OSSL_DISPATCH dispatch[] = {
            {0, NULL}
        };
        
        // Get the nonce function (may return NULL)
        OSSL_FUNC_rand_nonce_fn *nonce_fn = OSSL_FUNC_rand_nonce(dispatch);
        (void)nonce_fn; // May be NULL
        
        // If we had a proper nonce function, we could use it
        if (nonce_fn) {
            // Create a dummy context and buffer
            void *dummy_ctx = NULL;
            unsigned char nonce_buf[16];
            
            // Call the nonce function
            size_t nonce_len = nonce_fn(dummy_ctx, nonce_buf, 
                                       sizeof(nonce_buf), 16, 0);
            (void)nonce_len;
        }
    }

    // 4. Create X509_ACERT and use X509_ACERT_get0_holder_entityName
    {
        // Read X509_ACERT from input file
        rewind(in_file);
        X509_ACERT *acert = d2i_X509_ACERT_fp(in_file, NULL);
        if (!acert && size > 0) {
            // If parsing failed, try to create minimal structure
            // using part of input data
            const unsigned char *p = data;
            acert = d2i_X509_ACERT(NULL, &p, 
                                  size > 100 ? 100 : (long)size);
        }
        
        if (acert) {
            // Get holder entity name
            const GENERAL_NAMES *holder = 
                X509_ACERT_get0_holder_entityName(acert);
            (void)holder; // May be NULL
            
            X509_ACERT_free(acert);
        }
    }

    // 5. Create SSL_CTX and use SSL_CTX_get0_client_cert_type
    {
        SSL_CTX *ctx = SSL_CTX_new(TLS_method());
        if (ctx) {
            unsigned char *cert_type = NULL;
            size_t cert_type_len = 0;
            
            // Get client certificate type
            int result = SSL_CTX_get0_client_cert_type(ctx, &cert_type, 
                                                      &cert_type_len);
            (void)result; // Result ignored for fuzzing
            (void)cert_type; // May be NULL
            (void)cert_type_len;
            
            SSL_CTX_free(ctx);
        }
    }

    // Clean up resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}