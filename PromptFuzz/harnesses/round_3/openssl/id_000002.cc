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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 3
//<Quality> {"density":9,"unique_branches":{"rand_get0_public":[[791,13,791,87,0,0,4,1]],"rand_get0_private":[[834,13,834,86,0,0,4,0],[835,16,835,76,0,0,4,1]]},"library_calls":["d2i_PKCS7_ENC_CONTENT","RC2_set_key","RC2_cfb64_encrypt","DH_meth_new","EVP_PKEY_new","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_assign","EVP_PKEY_get1_RSA","RSA_free","BN_free","SSL_CTX_new","TLS_method","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","BIO_dump_indent_fp","PKCS7_RECIP_INFO_new","PKCS7_RECIP_INFO_free","ASN1_item_free","PKCS7_ENC_CONTENT_it","DH_meth_free","EVP_PKEY_free","RSA_free","SSL_CTX_free","OSSL_CMP_CTX_free"],"critical_calls":["d2i_PKCS7_ENC_CONTENT","RC2_set_key","RC2_cfb64_encrypt","DH_meth_new","EVP_PKEY_new","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_assign","EVP_PKEY_get1_RSA","RSA_free","BN_free","SSL_CTX_new","TLS_method","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","BIO_dump_indent_fp","PKCS7_RECIP_INFO_new","PKCS7_RECIP_INFO_free","ASN1_item_free","PKCS7_ENC_CONTENT_it","DH_meth_free","EVP_PKEY_free","SSL_CTX_free","OSSL_CMP_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for minimal input
    if (size < 64) {
	return 0;
	}
	
    
    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for writing output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Decode PKCS7_ENC_CONTENT from input
    PKCS7_ENC_CONTENT *enc_content = NULL;
    const unsigned char *p = data;
    enc_content = d2i_PKCS7_ENC_CONTENT(NULL, &p, size);
    
    // 2. Setup RC2 encryption/decryption
    RC2_KEY rc2_key;
    unsigned char ivec[8] = {0};
    int num = 0;
    
    // Use part of input data as RC2 key
    RC2_set_key(&rc2_key, 16, data, 128); // 16-byte key
    
    // Encrypt/decrypt a small portion of data
    size_t crypto_len = size > 128 ? 128 : size;
    unsigned char *out_buf = (unsigned char *)malloc(crypto_len);
    if (out_buf && enc_content) {
        RC2_cfb64_encrypt(data, out_buf, crypto_len, &rc2_key, ivec, &num, 
                         size % 2 ? RC2_ENCRYPT : RC2_DECRYPT);
    }
    
    // 3. Create DH method
    DH_METHOD *dh_method = DH_meth_new("TestDH", 0);
    
    // 4. Create RSA key from EVP_PKEY
    EVP_PKEY *pkey = EVP_PKEY_new();
    RSA *rsa = RSA_new();
    if (pkey && rsa) {
        // Generate a minimal RSA key for testing
        BIGNUM *bn = BN_new();
        if (bn) {
            BN_set_word(bn, RSA_F4);
            RSA_generate_key_ex(rsa, 512, bn, NULL);
            EVP_PKEY_assign_RSA(pkey, rsa);
            
            // Get RSA from EVP_PKEY
            struct rsa_st *rsa2 = EVP_PKEY_get1_RSA(pkey);
            if (rsa2) {
                RSA_free(rsa2); // Free the extra reference
            }
            BN_free(bn);
        }
    }
    
    // 5. Setup SSL CT validation callback
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        // Create dummy callback
        ssl_ct_validation_cb callback = [](const CT_POLICY_EVAL_CTX *ctx, 
                                           const struct stack_st_SCT *scts, 
                                           void *arg) {
            return 1; // Accept all
        };
        
        SSL_CTX_set_ct_validation_callback(ssl_ctx, callback, NULL);
    }
    
    // 6. Get HTTP callback argument from CMP context
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(NULL, NULL);
    if (cmp_ctx) {
        void *http_arg = OSSL_CMP_CTX_get_http_cb_arg(cmp_ctx);
        (void)http_arg; // Use to avoid unused warning
    }
    
    // 7. Dump data using BIO_dump_indent_fp
    if (out_buf && crypto_len > 0) {
        BIO_dump_indent_fp(out_file, out_buf, crypto_len > 32 ? 32 : crypto_len, 4);
    }
    
    // 8. Create and free PKCS7_RECIP_INFO
    PKCS7_RECIP_INFO *ri = PKCS7_RECIP_INFO_new();
    if (ri) {
        PKCS7_RECIP_INFO_free(ri);
    }
    
    // Cleanup
    if (enc_content) {
        // Note: PKCS7_ENC_CONTENT doesn't have a direct free function in the provided list
        // We'll rely on d2i allocation cleanup
        ASN1_item_free((ASN1_VALUE *)enc_content, ASN1_ITEM_rptr(PKCS7_ENC_CONTENT));
    }
    
    if (dh_method) {
        DH_meth_free(dh_method);
    }
    
    if (pkey) {
        EVP_PKEY_free(pkey);
    } else if (rsa) {
        RSA_free(rsa);
    }
    
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
    }
    
    if (cmp_ctx) {
        OSSL_CMP_CTX_free(cmp_ctx);
    }
    
    free(out_buf);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}