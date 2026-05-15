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
//<ID> 419
//<Prompt> []
/*<Combination>: [
*/
//<score> 102, nr_unique_branch: 107
//<Quality> {"density":6,"unique_branches":{"RC5_32_set_key":[[25,9,25,18,0,0,4,1],[28,9,28,34,0,0,4,0],[28,38,28,63,0,0,4,1],[34,17,34,31,0,0,4,0],[34,17,34,31,0,0,4,1],[41,9,41,11,0,0,4,1],[51,17,51,22,0,0,4,0],[51,17,51,22,0,0,4,1],[54,9,54,16,0,0,4,0],[58,17,58,22,0,0,4,0],[58,17,58,22,0,0,4,1],[64,13,64,22,0,0,4,0],[64,13,64,22,0,0,4,1],[66,13,66,22,0,0,4,0],[66,13,66,22,0,0,4,1]],"ossl_cms_content_bio":[[135,9,135,21,0,0,4,0]],"CMS_sign_ex":[[545,9,545,32,0,0,4,1]],"BIO_new_ex":[[98,9,98,31,0,0,4,1],[103,9,103,31,0,0,4,0]],"deflt_query":[[763,5,763,21,0,0,4,0]],"evp_kem_new":[[287,9,287,20,0,0,4,1],[290,9,290,41,0,0,4,1],[291,12,291,39,0,0,4,1]],"evp_kem_from_algorithm":[[309,9,309,42,0,0,4,1],[315,9,315,75,0,0,4,1],[319,12,319,33,0,0,4,0],[319,12,319,33,0,0,4,1],[320,17,320,33,0,0,4,1],[321,9,321,34,0,0,4,0],[321,9,321,34,0,0,4,1],[322,17,322,36,0,0,4,1],[327,9,327,44,0,0,4,0],[327,9,327,44,0,0,4,1],[328,17,328,46,0,0,4,1],[333,9,333,49,0,0,4,0],[333,9,333,49,0,0,4,1],[334,17,334,51,0,0,4,1],[339,9,339,39,0,0,4,0],[339,9,339,39,0,0,4,1],[340,17,340,41,0,0,4,1],[345,9,345,44,0,0,4,0],[345,9,345,44,0,0,4,1],[346,17,346,46,0,0,4,1],[351,9,351,49,0,0,4,0],[351,9,351,49,0,0,4,1],[352,17,352,51,0,0,4,1],[357,9,357,39,0,0,4,0],[357,9,357,39,0,0,4,1],[358,17,358,41,0,0,4,1],[363,9,363,35,0,0,4,0],[363,9,363,35,0,0,4,1],[364,17,364,37,0,0,4,1],[369,9,369,34,0,0,4,0],[369,9,369,34,0,0,4,1],[370,17,370,36,0,0,4,1],[374,9,374,42,0,0,4,0],[374,9,374,42,0,0,4,1],[375,17,375,44,0,0,4,1],[381,9,381,47,0,0,4,0],[381,9,381,47,0,0,4,1],[382,17,382,49,0,0,4,1],[388,9,388,42,0,0,4,0],[388,9,388,42,0,0,4,1],[389,17,389,44,0,0,4,1],[395,9,395,47,0,0,4,0],[395,9,395,47,0,0,4,1],[396,17,396,49,0,0,4,1],[404,9,404,22,0,0,4,1],[405,13,405,26,0,0,4,0],[405,30,405,43,0,0,4,0],[405,30,405,43,0,0,4,1],[405,47,405,60,0,0,4,1],[406,13,406,26,0,0,4,0],[406,30,406,43,0,0,4,0],[406,30,406,43,0,0,4,1],[406,47,406,60,0,0,4,1],[407,12,407,34,0,0,4,1],[408,13,408,29,0,0,4,0],[408,13,408,29,0,0,4,1],[408,33,408,49,0,0,4,1],[409,13,409,29,0,0,4,0],[409,33,409,49,0,0,4,1]],"hexstr2buf_sep":[[222,60,222,62,0,0,4,0],[222,60,222,62,0,0,4,1],[225,13,225,22,0,0,4,1],[228,13,228,16,0,0,4,1],[234,13,234,20,0,0,4,1],[234,24,234,31,0,0,4,1],[239,13,239,22,0,0,4,0],[240,17,240,28,0,0,4,1],[248,9,248,23,0,0,4,0]],"OPENSSL_hexchar2int":[[197,5,197,13,0,0,4,0],[206,5,206,13,0,0,4,0]],"OSSL_CMP_CTX_set_certConf_cb":[[266,13,266,24,0,0,4,1]],"ossl_hexstr2buf_sep":[[269,9,269,19,0,0,4,1],[274,9,274,46,0,0,4,1],[277,9,277,23,0,0,4,0],[280,9,280,58,0,0,4,0],[281,13,281,27,0,0,4,0]],"BIO_free":[[140,32,140,60,0,0,4,1]],"aes_128_ecb_newctx":[[223,13,223,24,0,0,4,0],[205,9,205,32,1,0,4,1]],"EVP_EncryptInit_ex":[[580,9,580,21,0,0,4,1]],"EVP_KEM_free":[[435,9,435,20,0,0,4,1],[439,9,439,14,0,0,4,0]],"RC5_32_decrypt":[[140,9,140,26,0,0,4,1],[150,16,150,33,0,0,4,0]]},"library_calls":["OPENSSL_hexstr2buf","OPENSSL_store_u32_le","RC5_32_set_key","RC5_32_decrypt","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_get0_libctx","EVP_KEM_fetch","EVP_KEM_get0_description","EVP_KEM_free","CMS_sign","EVP_PKEY_new","CMS_add1_recipient_cert","CMS_RecipientInfo_get0_pkey_ctx","CMS_ContentInfo_free","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_aes_128_ecb","EVP_CipherFinal_ex","X509_STORE_new","X509_STORE_get_check_revocation","OSSL_CMP_CTX_new","OSSL_CMP_CTX_set_certConf_cb","CRYPTO_free","EVP_PKEY_CTX_free","EVP_CIPHER_CTX_free","OSSL_CMP_CTX_free","X509_STORE_free"],"critical_calls":["OPENSSL_hexstr2buf","OPENSSL_store_u32_le","RC5_32_set_key","RC5_32_decrypt","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_get0_libctx","EVP_KEM_fetch","EVP_KEM_get0_description","EVP_KEM_free","CMS_sign","EVP_PKEY_new","CMS_add1_recipient_cert","CMS_RecipientInfo_get0_pkey_ctx","CMS_ContentInfo_free","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_aes_128_ecb","EVP_CipherFinal_ex","X509_STORE_new","X509_STORE_get_check_revocation","OSSL_CMP_CTX_new","OSSL_CMP_CTX_set_certConf_cb","CRYPTO_free","EVP_PKEY_CTX_free","EVP_CIPHER_CTX_free","OSSL_CMP_CTX_free","X509_STORE_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize variables
    RC5_32_KEY rc5_key;
    EVP_PKEY_CTX *pkey_ctx = NULL;
    CMS_RecipientInfo *ri = NULL;
    EVP_CIPHER_CTX *cipher_ctx = NULL;
    OSSL_CMP_CTX *cmp_ctx = NULL;
    X509_STORE *store = NULL;
    const EVP_KEM *kem = NULL;
    unsigned char *hex_buf = NULL;
    long hex_buf_len = 0;
    
    // 1. Use OPENSSL_hexstr2buf with sample hex data
    const char *hex_str = "48656c6c6f"; // "Hello" in hex
    hex_buf = OPENSSL_hexstr2buf(hex_str, &hex_buf_len);
    
    // 2. Use OPENSSL_store_u32_le
    uint32_t test_val = 0x12345678;
    unsigned char store_out[4];
    OPENSSL_store_u32_le(store_out, test_val);
    
    // 3. RC5 decryption (requires proper key setup)
    if (size >= 16) { // Need enough data for key and input
        // Initialize RC5 key with part of input data
        RC5_32_set_key(&rc5_key, 16, data, 12);
        
        // Prepare data for decryption (two 32-bit words)
        unsigned long rc5_data[2] = {0};
        if (size >= 24) {
            memcpy(rc5_data, data + 16, 8);
            RC5_32_decrypt(rc5_data, &rc5_key);
        }
    }
    
    // 4. Setup EVP_PKEY_CTX and get library context
    pkey_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (pkey_ctx) {
        OSSL_LIB_CTX *libctx = EVP_PKEY_CTX_get0_libctx(pkey_ctx);
        
        // Try to get a KEM and its description
        kem = EVP_KEM_fetch(libctx, "RSA", NULL);
        if (kem) {
            const char *kem_desc = EVP_KEM_get0_description(kem);
            EVP_KEM_free((EVP_KEM*)kem);
        }
    }
    
    // 5. Create dummy CMS structures
    CMS_ContentInfo *cms = CMS_sign(NULL, NULL, NULL, NULL, 
                                    CMS_NOCERTS | CMS_DETACHED);
    if (cms) {
        // Add a recipient
        X509 *cert = NULL;
        EVP_PKEY *pkey = EVP_PKEY_new();
        if (pkey) {
            ri = CMS_add1_recipient_cert(cms, cert, 0);
            if (ri) {
                // Get PKEY context from recipient info
                pkey_ctx = CMS_RecipientInfo_get0_pkey_ctx(ri);
            }
        }
        CMS_ContentInfo_free(cms);
    }
    
    // 6. Setup cipher context and use EVP_CipherFinal_ex
    cipher_ctx = EVP_CIPHER_CTX_new();
    if (cipher_ctx) {
        // Initialize with some cipher
        if (EVP_EncryptInit_ex(cipher_ctx, EVP_aes_128_ecb(), 
                              NULL, data, NULL)) {
            unsigned char out[16];
            int outl = 0;
            // Finalize cipher operation
            EVP_CipherFinal_ex(cipher_ctx, out, &outl);
        }
    }
    
    // 7. Create X509_STORE and get revocation function
    store = X509_STORE_new();
    if (store) {
        X509_STORE_CTX_check_revocation_fn rev_func = 
            X509_STORE_get_check_revocation(store);
    }
    
    // 8. Create CMP context and set certConf callback
    cmp_ctx = OSSL_CMP_CTX_new(NULL, NULL);
    if (cmp_ctx) {
        // Define a simple callback function
        OSSL_CMP_certConf_cb_t cb = [](OSSL_CMP_CTX *ctx, X509 *cert, 
                                      int fail_info, const char **txt) -> int {
            return 1; // Accept all for fuzzing
        };
        OSSL_CMP_CTX_set_certConf_cb(cmp_ctx, cb);
    }
    
    // 9. Use the FILE* for reading/writing operations
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // Read some data from input file
    unsigned char read_buf[64];
    if (size > 0) {
        fread(read_buf, 1, size < 64 ? size : 64, in_file);
        
        // Write to output file
        fwrite(read_buf, 1, size < 64 ? size : 64, out_file);
        fflush(out_file);
    }
    
    // Cleanup
    if (hex_buf) OPENSSL_free(hex_buf);
    if (pkey_ctx) EVP_PKEY_CTX_free(pkey_ctx);
    if (cipher_ctx) EVP_CIPHER_CTX_free(cipher_ctx);
    if (cmp_ctx) OSSL_CMP_CTX_free(cmp_ctx);
    if (store) X509_STORE_free(store);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}