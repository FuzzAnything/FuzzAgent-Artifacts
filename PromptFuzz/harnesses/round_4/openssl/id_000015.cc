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
//<ID> 101
//<Prompt> []
/*<Combination>: [
*/
//<score> 132, nr_unique_branch: 41
//<Quality> {"density":11,"unique_branches":{"cms_ec_cb":[[89,9,89,39,0,0,4,0],[89,9,89,39,0,0,4,1]],"ossl_cms_EncryptedContent_init_bio":[[44,11,44,21,0,0,4,0],[47,9,47,18,0,0,4,1],[55,9,55,12,0,0,4,0],[60,13,60,28,0,0,4,1],[65,9,65,23,0,0,4,0],[68,13,68,33,0,0,4,0],[71,9,71,23,0,0,4,1],[78,9,78,67,0,0,4,1],[83,9,83,12,0,0,4,0],[85,13,85,36,0,0,4,1],[85,40,85,73,0,0,4,0],[203,9,203,18,0,0,4,0],[208,9,208,11,0,0,4,1]],"ossl_cms_AuthEnvelopedData_init_bio":[[1257,9,1257,27,0,0,4,1],[1264,9,1264,20,0,0,4,0]],"cms_auth_enveloped_data_init":[[99,9,99,29,0,0,4,0],[101,13,101,45,0,0,4,1]],"CMS_dataInit":[[183,5,183,43,0,0,4,0]],"enc_new":[[73,9,73,53,0,0,4,1],[77,9,77,28,0,0,4,1]],"enc_free":[[94,9,94,18,0,0,4,1],[98,9,98,18,0,0,4,1]],"CMS_AuthEnvelopedData_create_ex":[[321,9,321,20,0,0,4,1],[324,9,324,21,0,0,4,1],[326,9,328,39,0,0,4,1]],"EVP_CIPHER_CTX_get0_cipher":[[420,9,420,20,0,0,4,1]],"enc_ctrl":[[318,9,318,20,0,0,4,1],[322,5,322,24,0,0,4,1],[330,5,330,22,0,0,4,1],[340,5,340,27,0,0,4,1],[345,5,345,26,0,0,4,1],[350,5,350,24,0,0,4,1],[383,5,383,33,0,0,4,1],[386,5,386,32,0,0,4,1],[391,5,391,30,0,0,4,0],[396,5,396,22,0,0,4,1],[406,5,406,12,0,0,4,1]],"ossl_cms_EncryptedContent_init":[[220,9,220,12,0,0,4,1],[226,9,226,23,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","EVP_aes_256_ctr","CMS_AuthEnvelopedData_create","BIO_new_fp","CMS_final_digest","BIO_free","CMS_ContentInfo_free","X509_STORE_new","X509_STORE_get_check_issued","X509_STORE_free","OSSL_FUNC_rand_nonce","BIO_new_fp","d2i_X509_ACERT_bio","X509_ACERT_get0_holder_entityName","X509_ACERT_free","BIO_free","TLS_method","SSL_CTX_new","SSL_CTX_get0_client_cert_type","SSL_CTX_free"],"critical_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","EVP_aes_256_ctr","CMS_AuthEnvelopedData_create","BIO_new_fp","CMS_final_digest","BIO_free","CMS_ContentInfo_free","X509_STORE_new","X509_STORE_get_check_issued","X509_STORE_free","OSSL_FUNC_rand_nonce","BIO_new_fp","d2i_X509_ACERT_bio","X509_ACERT_get0_holder_entityName","X509_ACERT_free","BIO_free","TLS_method","SSL_CTX_new","SSL_CTX_get0_client_cert_type","SSL_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL if needed
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    OPENSSL_init_ssl(0, NULL);
    
    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 1. Use CMS_final_digest
    // Create a dummy CMS_ContentInfo structure
    const EVP_CIPHER *cipher = EVP_aes_256_ctr();
    CMS_ContentInfo *cms = CMS_AuthEnvelopedData_create(cipher);
    if (cms) {
        // Create a BIO from the input file
        BIO *dcont = BIO_new_fp(in_file, BIO_NOCLOSE);
        if (dcont) {
            // Use part of input data as digest
            unsigned int md_len = size > 32 ? 32 : (unsigned int)size;
            unsigned char md[32];
            if (size > 0) {
                memcpy(md, data, md_len);
            }
            
            // Call CMS_final_digest
            int ret = CMS_final_digest(cms, md, md_len, dcont, 0);
            (void)ret; // Result is ignored for fuzzing
            
            BIO_free(dcont);
        }
        CMS_ContentInfo_free(cms);
    }
    
    // 2. Use X509_STORE_get_check_issued
    // Create a simple X509_STORE
    X509_STORE *store = X509_STORE_new();
    if (store) {
        // Get the check_issued function
        X509_STORE_CTX_check_issued_fn check_issued = X509_STORE_get_check_issued(store);
        (void)check_issued; // Function pointer is ignored for fuzzing
        
        X509_STORE_free(store);
    }
    
    // 3. Use OSSL_FUNC_rand_nonce
    // Create a dummy OSSL_DISPATCH structure
    // Note: This is simplified as we need a proper provider context
    OSSL_DISPATCH dispatch[1] = {0};
    OSSL_FUNC_rand_nonce_fn *nonce_fn = OSSL_FUNC_rand_nonce(dispatch);
    (void)nonce_fn; // Function pointer is ignored for fuzzing
    
    // 4. Use X509_ACERT_get0_holder_entityName
    // Try to parse X509_ACERT from input data
    fseek(in_file, 0, SEEK_SET); // Reset file pointer
    BIO *bio = BIO_new_fp(in_file, BIO_NOCLOSE);
    if (bio) {
        // Try to read X509_ACERT
        X509_ACERT *acert = d2i_X509_ACERT_bio(bio, NULL);
        if (acert) {
            const GENERAL_NAMES *names = X509_ACERT_get0_holder_entityName(acert);
            (void)names; // Result is ignored for fuzzing
            X509_ACERT_free(acert);
        }
        BIO_free(bio);
    }
    
    // 5. Use SSL_CTX_get0_client_cert_type
    // Create SSL_CTX
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (ctx) {
        unsigned char *cert_type = NULL;
        size_t cert_type_len = 0;
        
        // Get client certificate type
        int ret = SSL_CTX_get0_client_cert_type(ctx, &cert_type, &cert_type_len);
        (void)ret; // Result is ignored for fuzzing
        
        // Write some data to output file using the input
        if (size > 0 && out_file) {
            fwrite(data, 1, size > 1024 ? 1024 : size, out_file);
        }
        
        SSL_CTX_free(ctx);
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}