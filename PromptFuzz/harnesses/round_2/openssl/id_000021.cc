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
//<ID> 506
//<Prompt> []
/*<Combination>: [
*/
//<score> 110, nr_unique_branch: 27
//<Quality> {"density":20,"unique_branches":{"ossl_pem_check_suffix":[[1049,9,1049,34,0,0,4,0]],"sec_alloc_realloc":[[61,9,61,26,0,0,4,0],[61,9,61,26,0,0,4,1],[62,13,62,24,0,0,4,0]],"decoder_process":[[1192,13,1192,16,0,0,4,0]],"BUF_MEM_free":[[46,13,46,43,0,0,4,0]],"PEM_get_EVP_CIPHER_INFO":[[525,9,525,25,0,0,4,1],[525,29,525,46,0,0,4,1],[525,50,525,67,0,0,4,1],[528,9,528,31,0,0,4,0],[58,30,58,74,11,0,4,1]],"pem2der_decode":[[179,9,179,12,0,0,4,1],[188,9,188,32,0,0,4,0],[188,9,188,32,0,0,4,1],[195,13,195,58,0,0,4,0],[208,17,208,31,0,0,4,0],[208,17,208,31,0,0,4,1],[209,13,209,60,0,0,4,1],[212,9,212,23,0,0,4,1]],"check_pem":[[136,9,136,47,0,0,4,0],[139,13,139,46,0,0,4,1],[141,13,141,49,0,0,4,1],[144,13,144,21,0,0,4,1]],"OSSL_DECODER_from_bio":[[102,13,102,45,0,0,4,1],[102,49,102,70,0,0,4,1]],"BUF_MEM_grow_clean":[[129,9,129,43,0,0,4,0]],"pem_read_bio_key_decoder":[[66,17,66,70,0,0,4,1]]},"library_calls":["BN_CTX_new","BN_RECP_CTX_new","ossl_check_X509_LOOKUP_compfunc_type","OPENSSL_sk_new","PEM_read_RSAPrivateKey","RSA_get0_n","RSA_get0_q","EVP_MD_CTX_new","EVP_sha256","EVP_DigestInit_ex","BN_num_bits","BN_bn2bin","EVP_DigestUpdate","EVP_DigestUpdate","EVP_DigestFinal_ex","EVP_MD_CTX_free","X509_ATTRIBUTE_create","PKCS7_SIGNER_INFO_new","BN_dup","BN_kronecker","OPENSSL_sk_push","OPENSSL_sk_push","OPENSSL_sk_pop","OPENSSL_sk_pop","DES_options","AES_options","BN_free","X509_ATTRIBUTE_free","PKCS7_SIGNER_INFO_free","OPENSSL_sk_free","BN_CTX_free","BN_RECP_CTX_free","RSA_free"],"critical_calls":["BN_CTX_new","BN_RECP_CTX_new","ossl_check_X509_LOOKUP_compfunc_type","OPENSSL_sk_new","PEM_read_RSAPrivateKey","RSA_get0_n","RSA_get0_q","EVP_MD_CTX_new","EVP_sha256","EVP_DigestInit_ex","BN_num_bits","BN_bn2bin","EVP_DigestUpdate","EVP_DigestUpdate","EVP_DigestFinal_ex","EVP_MD_CTX_free","X509_ATTRIBUTE_create","PKCS7_SIGNER_INFO_new","BN_dup","BN_kronecker","OPENSSL_sk_push","OPENSSL_sk_push","OPENSSL_sk_pop","OPENSSL_sk_pop","DES_options","AES_options","BN_free","X509_ATTRIBUTE_free","PKCS7_SIGNER_INFO_free","OPENSSL_sk_free","BN_CTX_free","BN_RECP_CTX_free","RSA_free"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified OpenSSL APIs to process RSA private keys and compute cryptographic operations:

*/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create FILE pointer from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Initialize BN contexts
    BN_CTX *bn_ctx = BN_CTX_new();
    BN_RECP_CTX *recp_ctx = BN_RECP_CTX_new();
    
    // Create OPENSSL stack with comparison function
    OPENSSL_sk_compfunc cmp = ossl_check_X509_LOOKUP_compfunc_type(NULL);
    OPENSSL_STACK *stack = OPENSSL_sk_new(cmp);
    
    // Read RSA private key from input data
    RSA *rsa = PEM_read_RSAPrivateKey(in_file, NULL, NULL, NULL);
    
    if (rsa) {
        // Get RSA components
        const BIGNUM *n = RSA_get0_n(rsa);
        const BIGNUM *q = RSA_get0_q(rsa);
        
        // Create EVP context for hashing
        EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
        const EVP_MD *sha256 = EVP_sha256();
        
        // Initialize digest
        EVP_DigestInit_ex(md_ctx, sha256, NULL);
        
        // Get modulus bits
        int bits = BN_num_bits(n);
        
        // Convert BIGNUM to binary
        unsigned char *bn_buf = new unsigned char[bits / 8 + 1];
        int bn_len = BN_bn2bin(n, bn_buf);
        
        // Update digest with modulus
        EVP_DigestUpdate(md_ctx, bn_buf, bn_len);
        
        // Update digest with input data
        EVP_DigestUpdate(md_ctx, data, size);
        
        // Finalize digest
        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int hash_len;
        EVP_DigestFinal_ex(md_ctx, hash, &hash_len);
        
        // Clean up EVP context
        EVP_MD_CTX_free(md_ctx);
        
        // Create X509 attribute
        X509_ATTRIBUTE *attr = X509_ATTRIBUTE_create(NID_commonName, V_ASN1_UTF8STRING, 
                                                    (void *)"fuzz_test");
        
        // Create PKCS7 signer info
        PKCS7_SIGNER_INFO *si = PKCS7_SIGNER_INFO_new();
        
        // Duplicate BIGNUM
        BIGNUM *n_dup = BN_dup(n);
        
        // Compute Kronecker symbol
        if (bn_ctx && n && q) {
            int kronecker = BN_kronecker(n, q, bn_ctx);
            (void)kronecker; // Use to avoid unused variable warning
        }
        
        // Push items to stack
        if (stack) {
            OPENSSL_sk_push(stack, bn_buf);
            OPENSSL_sk_push(stack, hash);
        }
        
        // Pop items from stack
        if (stack) {
            void *item1 = OPENSSL_sk_pop(stack);
            void *item2 = OPENSSL_sk_pop(stack);
            (void)item1; (void)item2; // Use to avoid unused variable warning
        }
        
        // Get cipher options (for completeness)
        const char *des_opts = DES_options();
        const char *aes_opts = AES_options();
        (void)des_opts; (void)aes_opts; // Use to avoid unused variable warning
        
        // Create output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            // Get file descriptor
            int fd = fuzz_fileno(out_file);
            (void)fd; // Use to avoid unused variable warning
            
            // Write hash to file
            fwrite(hash, 1, hash_len, out_file);
            assert_file_closed(&out_file);;
        }
        
        // Clean up
        delete[] bn_buf;
        BN_free(n_dup);
        X509_ATTRIBUTE_free(attr);
        if (si) PKCS7_SIGNER_INFO_free(si);
    }
    
    // Close input file
    assert_file_closed(&in_file);;
    
    // Clean up all allocated resources
    if (stack) OPENSSL_sk_free(stack);
    if (bn_ctx) BN_CTX_free(bn_ctx);
    if (recp_ctx) BN_RECP_CTX_free(recp_ctx);
    if (rsa) RSA_free(rsa);
    
    assert_file_closed(&in_file);
	return 0;
}