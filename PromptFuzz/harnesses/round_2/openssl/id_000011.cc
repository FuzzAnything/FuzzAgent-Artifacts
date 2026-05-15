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
//<ID> 265
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"DES_options":[[25,9,25,13,0,0,4,0]]},"library_calls":["BN_CTX_new","BN_RECP_CTX_new","OPENSSL_sk_new","PEM_read_RSAPrivateKey","RSA_get0_n","EVP_MD_CTX_new","EVP_sha256","EVP_DigestInit_ex","BN_num_bits","BN_bn2bin","EVP_DigestUpdate","EVP_DigestFinal_ex","EVP_MD_CTX_free","X509_ATTRIBUTE_create","PKCS7_SIGNER_INFO_new","PKCS7_add1_attrib_digest","PKCS7_SIGNER_INFO_free","RSA_get0_p","RSA_get0_q","BN_dup","BN_dup","BN_kronecker","BN_free","BN_free","DES_options","AES_options","OPENSSL_sk_push","OPENSSL_sk_pop","X509_ATTRIBUTE_free","RSA_free","BN_CTX_free","BN_RECP_CTX_free","OPENSSL_sk_free"],"critical_calls":["BN_CTX_new","BN_RECP_CTX_new","OPENSSL_sk_new","PEM_read_RSAPrivateKey","RSA_get0_n","EVP_MD_CTX_new","EVP_sha256","EVP_DigestInit_ex","BN_num_bits","BN_bn2bin","EVP_DigestUpdate","EVP_DigestFinal_ex","EVP_MD_CTX_free","X509_ATTRIBUTE_create","PKCS7_SIGNER_INFO_new","PKCS7_add1_attrib_digest","PKCS7_SIGNER_INFO_free","RSA_get0_p","RSA_get0_q","BN_dup","BN_dup","BN_kronecker","BN_free","BN_free","DES_options","AES_options","OPENSSL_sk_push","OPENSSL_sk_pop","X509_ATTRIBUTE_free","RSA_free","BN_CTX_free","BN_RECP_CTX_free","OPENSSL_sk_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create FILE* from input data
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
    
    // Initialize structures
    RSA *rsa_key = nullptr;
    BN_CTX *bn_ctx = BN_CTX_new();
    BN_RECP_CTX *bn_recp_ctx = BN_RECP_CTX_new();
    OPENSSL_STACK *stack = OPENSSL_sk_new(NULL);
    
    // 1. Read RSA private key from input file
    rsa_key = PEM_read_RSAPrivateKey(in_file, nullptr, nullptr, nullptr);
    
    // 2. Create X509_ATTRIBUTE with digest from RSA key
    X509_ATTRIBUTE *attr = nullptr;
    if (rsa_key) {
        const BIGNUM *n = RSA_get0_n(rsa_key);
        if (n) {
            // Use RSA modulus as value for attribute
            unsigned char digest[EVP_MAX_MD_SIZE];
            unsigned int digest_len = 0;
            
            // Create digest from RSA modulus bytes
            EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
            if (md_ctx) {
                const EVP_MD *md = EVP_sha256();
                EVP_DigestInit_ex(md_ctx, md, nullptr);
                
                // Convert BIGNUM to bytes
                int n_size = BN_num_bytes(n);
                unsigned char *n_bytes = new unsigned char[n_size];
                BN_bn2bin(n, n_bytes);
                EVP_DigestUpdate(md_ctx, n_bytes, n_size);
                delete[] n_bytes;
                
                EVP_DigestFinal_ex(md_ctx, digest, &digest_len);
                EVP_MD_CTX_free(md_ctx);
                
                // Create attribute with digest
                attr = X509_ATTRIBUTE_create(NID_pkcs9_contentType, 
                                            V_ASN1_OCTET_STRING, 
                                            digest);
                
                // Create PKCS7_SIGNER_INFO and add digest attribute
                PKCS7_SIGNER_INFO *si = PKCS7_SIGNER_INFO_new();
                if (si) {
                    PKCS7_add1_attrib_digest(si, digest, digest_len);
                    PKCS7_SIGNER_INFO_free(si);
                }
            }
        }
    }
    
    // 3. Calculate Kronecker symbol using BIGNUMs from RSA key
    if (rsa_key && bn_ctx) {
        const BIGNUM *p = RSA_get0_p(rsa_key);
        const BIGNUM *q = RSA_get0_q(rsa_key);
        
        if (p && q) {
            // Create temporary BIGNUMs for calculation
            BIGNUM *a = BN_dup(p);
            BIGNUM *b = BN_dup(q);
            
            if (a && b) {
                // Calculate Kronecker symbol
                int kronecker = BN_kronecker(a, b, bn_ctx);
                
                // Use the result to seed RNG if available
                if (kronecker != -2) {  // -2 indicates error
                    unsigned char seed[32];
                    memset(seed, kronecker & 0xFF, sizeof(seed));
                    
                    // Get RNG seed function if available
                    // Note: We don't have OSSL_DISPATCH in fuzzer context,
                    // so we can't call OSSL_FUNC_rand_get_seed directly
                    // This is a placeholder to show the API would be used
                    // OSSL_FUNC_rand_get_seed_fn *get_seed = OSSL_FUNC_rand_get_seed(nullptr);
                }
                
                BN_free(a);
                BN_free(b);
            }
        }
    }
    
    // 4. Get cipher options
    const char *des_opts = DES_options();
    const char *aes_opts = AES_options();
    
    // Use the options (dummy operation to ensure calls aren't optimized out)
    if (des_opts && aes_opts) {
        // Write options to output file
        fwrite(des_opts, strlen(des_opts), 1, out_file);
        fwrite(aes_opts, strlen(aes_opts), 1, out_file);
    }
    
    // 5. Use the OPENSSL_STACK
    if (stack && attr) {
        OPENSSL_sk_push(stack, attr);
        OPENSSL_sk_pop(stack);
    }
    
    // 6. Clean up
    if (attr) X509_ATTRIBUTE_free(attr);
    if (rsa_key) RSA_free(rsa_key);
    if (bn_ctx) BN_CTX_free(bn_ctx);
    if (bn_recp_ctx) BN_RECP_CTX_free(bn_recp_ctx);
    if (stack) OPENSSL_sk_free(stack);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}