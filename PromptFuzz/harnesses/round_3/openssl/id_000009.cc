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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"SSL_CTX_set_ct_validation_callback":[[6782,9,6782,25,0,0,4,1]]},"library_calls":["TLS_method","SSL_CTX_new","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","EVP_PKEY_new","BN_new","BN_new","BN_bin2bn","BN_rand","RSA_new","RSA_generate_key_ex","EVP_PKEY_assign","EVP_PKEY_get1_RSA","RSA_free","DH_meth_new","d2i_PKCS7_ENC_CONTENT","PKCS7_RECIP_INFO_new","PKCS7_RECIP_INFO_free","RC2_set_key","RC2_cfb64_encrypt","RC2_cfb64_encrypt","BIO_dump_indent_fp","PKCS7_ENC_CONTENT_free","DH_meth_free","EVP_PKEY_free","RSA_free","BN_free","BN_free","OSSL_CMP_CTX_free","SSL_CTX_free"],"critical_calls":["TLS_method","SSL_CTX_new","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","EVP_PKEY_new","BN_new","BN_new","BN_bin2bn","BN_rand","RSA_new","RSA_generate_key_ex","EVP_PKEY_assign","EVP_PKEY_get1_RSA","RSA_free","DH_meth_new","d2i_PKCS7_ENC_CONTENT","PKCS7_RECIP_INFO_new","PKCS7_RECIP_INFO_free","RC2_set_key","RC2_cfb64_encrypt","RC2_cfb64_encrypt","BIO_dump_indent_fp","PKCS7_ENC_CONTENT_free","DH_meth_free","EVP_PKEY_free","BN_free","BN_free","OSSL_CMP_CTX_free","SSL_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create SSL context with TLS method
    const SSL_METHOD *meth = TLS_method();
    SSL_CTX *ssl_ctx = SSL_CTX_new(meth);
    
    // 2. Set CT validation callback
    if (ssl_ctx) {
        SSL_CTX_set_ct_validation_callback(ssl_ctx, nullptr, nullptr);
    }
    
    // 3. Create CMP context
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(nullptr, nullptr);
    
    // 4. Get HTTP callback argument from CMP context
    if (cmp_ctx) {
        void *http_arg = OSSL_CMP_CTX_get_http_cb_arg(cmp_ctx);
        (void)http_arg; // Use variable to avoid unused warning
    }
    
    // 5. Create EVP_PKEY and generate RSA key
    EVP_PKEY *pkey = EVP_PKEY_new();
    BIGNUM *bn = BN_new();
    BIGNUM *e = BN_new();
    
    if (bn && size > 0) {
        // Convert first min(size, 256) bytes to BIGNUM
        size_t len = size > 256 ? 256 : size;
        BN_bin2bn(data, len, bn);
    }
    
    // Generate random BIGNUM for RSA exponent
    if (e) {
        BN_rand(e, 32, 0, 0); // 256-bit exponent
    }
    
    // Generate RSA key
    RSA *rsa = RSA_new();
    if (rsa && bn && e) {
        RSA_generate_key_ex(rsa, 2048, e, nullptr);
        
        // Assign RSA key to EVP_PKEY
        if (pkey) {
            EVP_PKEY_assign(pkey, EVP_PKEY_RSA, rsa);
            
            // Get RSA key back from EVP_PKEY
            RSA *rsa2 = EVP_PKEY_get1_RSA(pkey);
            if (rsa2) {
                RSA_free(rsa2);
            }
        }
    }
    
    // 6. Create DH method
    DH_METHOD *dh_method = DH_meth_new("fuzz_dh_method", 0);
    
    // 7. Parse PKCS7 encrypted content from data
    PKCS7_ENC_CONTENT *enc_content = nullptr;
    if (size > 0) {
        const unsigned char *p = data;
        enc_content = d2i_PKCS7_ENC_CONTENT(nullptr, &p, size);
    }
    
    // 8. Create and free PKCS7 recipient info
    PKCS7_RECIP_INFO *recip_info = PKCS7_RECIP_INFO_new();
    if (recip_info) {
        PKCS7_RECIP_INFO_free(recip_info);
    }
    
    // 9. RC2 encryption/decryption using data
    if (size >= 16) { // Need at least 16 bytes for key and IV
        RC2_KEY rc2_key;
        unsigned char ivec[8] = {0};
        int num = 0;
        
        // Use first 16 bytes as RC2 key material
        RC2_set_key(&rc2_key, 16, data, 128);
        
        // Use next 8 bytes as IV if available
        if (size >= 24) {
            memcpy(ivec, data + 16, 8);
        }
        
        // Encrypt/decrypt a small buffer
        unsigned char in[32] = {0};
        unsigned char out[32] = {0};
        
        if (size >= 56) { // Have enough data for encryption
            memcpy(in, data + 24, 32);
            RC2_cfb64_encrypt(in, out, 32, &rc2_key, ivec, &num, 1); // Encrypt
            RC2_cfb64_encrypt(out, in, 32, &rc2_key, ivec, &num, 0); // Decrypt back
        }
    }
    
    // 10. Dump data to file using BIO_dump_indent_fp
    FILE *out_file = fopen("output_file", "wb");
    if (out_file && size > 0) {
        BIO_dump_indent_fp(out_file, data, size > 1024 ? 1024 : size, 4);
        assert_file_closed(&out_file);;
    }
    
    // 11. Clean up all allocated resources
    if (enc_content) {
        PKCS7_ENC_CONTENT_free(enc_content);
    }
    
    if (dh_method) {
        DH_meth_free(dh_method);
    }
    
    if (pkey) {
        EVP_PKEY_free(pkey);
    } else if (rsa) {
        RSA_free(rsa);
    }
    
    if (bn) {
        BN_free(bn);
    }
    
    if (e) {
        BN_free(e);
    }
    
    if (cmp_ctx) {
        OSSL_CMP_CTX_free(cmp_ctx);
    }
    
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
    }
    
    assert_file_closed(&out_file);
	return 0;
}