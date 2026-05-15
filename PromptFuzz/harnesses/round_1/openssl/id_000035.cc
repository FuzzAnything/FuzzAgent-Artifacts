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
//<ID> 431
//<Prompt> []
/*<Combination>: [
*/
//<score> 38.666668, nr_unique_branch: 7
//<Quality> {"density":29,"unique_branches":{"rsa_multiprime_keygen":[[427,17,427,60,0,0,4,1],[432,17,432,49,0,0,4,0],[433,20,433,60,0,0,4,0],[439,17,439,43,0,0,4,1]],"ossl_encode_der_length":[[49,13,49,28,0,0,4,0],[50,16,50,37,0,0,4,1]],"OSSL_PARAM_allocate_from_text":[[320,9,320,19,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","BIO_new_fp","BIO_new_fp","OSSL_PARAM_allocate_from_text","SSL_CTX_new","TLS_method","SSL_new","SSL_trace","DSA_new","BN_new","BN_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","DSA_set0_pqg","DSA_set0_key","EVP_PKEY_new","EVP_PKEY_set1_DSA","PEM_write_bio_PrivateKey","EVP_aes_256_cbc","EVP_PKEY_free","DSA_size","DSA_free","d2i_TS_REQ","TS_REQ_get_nonce","TS_REQ_free","EVP_PKEY_new","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_set1_RSA","PEM_write_bio_PUBKEY","BN_free","RSA_free","EVP_PKEY_free","DES_set_key_unchecked","DES_ofb64_encrypt","BIO_write","OPENSSL_sk_new_null","BIO_new","BIO_s_mem","BIO_new","BIO_s_mem","OPENSSL_sk_push","OPENSSL_sk_push","ossl_check_BIO_freefunc_type","OPENSSL_sk_pop_free","BIO_free","BIO_free","OPENSSL_sk_free","CRYPTO_free","SSL_free","SSL_CTX_free","BIO_free","BIO_free"],"critical_calls":["OPENSSL_init_ssl","BIO_new_fp","BIO_new_fp","OSSL_PARAM_allocate_from_text","SSL_CTX_new","TLS_method","SSL_new","SSL_trace","DSA_new","BN_new","BN_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","DSA_set0_pqg","DSA_set0_key","EVP_PKEY_new","EVP_PKEY_set1_DSA","PEM_write_bio_PrivateKey","EVP_aes_256_cbc","EVP_PKEY_free","DSA_size","DSA_free","d2i_TS_REQ","TS_REQ_get_nonce","TS_REQ_free","EVP_PKEY_new","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_set1_RSA","PEM_write_bio_PUBKEY","BN_free","RSA_free","EVP_PKEY_free","DES_set_key_unchecked","DES_ofb64_encrypt","BIO_write","OPENSSL_sk_new_null","BIO_new","BIO_s_mem","BIO_new","BIO_s_mem","OPENSSL_sk_push","OPENSSL_sk_push","ossl_check_BIO_freefunc_type","OPENSSL_sk_pop_free","CRYPTO_free","SSL_free","SSL_CTX_free","BIO_free","BIO_free"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 256) {
	return 0;
	}
	  // Need minimum data for operations
    
    // Initialize OpenSSL
    OPENSSL_init_ssl(0, NULL);
    
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
    
    // Create BIOs from FILE pointers
    BIO *in_bio = BIO_new_fp(in_file, BIO_NOCLOSE);
    BIO *out_bio = BIO_new_fp(out_file, BIO_NOCLOSE);
    
    // 1. Use OSSL_PARAM_allocate_from_text
    OSSL_PARAM *param = NULL;
    OSSL_PARAM param_defs[] = {
        OSSL_PARAM_END,
        OSSL_PARAM_END
    };
    int found = 0;
    
    // Create key-value pair from input data
    const char *key = "test_key";
    char *value = (char *)malloc(64);
    if (value) {
        size_t value_len = size > 64 ? 64 : size;
        memcpy(value, data, value_len);
        value[value_len-1] = '\0';
        
        OSSL_PARAM_allocate_from_text(param, param_defs, key, value, strlen(value), &found);
        free(value);
    }
    
    // 2. Use SSL_trace for debugging
    // Create dummy SSL context for tracing
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = SSL_new(ctx);
    if (ssl) {
        SSL_trace(1, 0x0303, 23, data, size > 100 ? 100 : size, ssl, NULL);
    }
    
    // 3. Try to read and write private key
    // First, try to create a DSA key from input data
    DSA *dsa = DSA_new();
    if (dsa) {
        // Create a minimal DSA key for testing
        BIGNUM *p = BN_new();
        BIGNUM *q = BN_new();
        BIGNUM *g = BN_new();
        BIGNUM *pub_key = BN_new();
        BIGNUM *priv_key = BN_new();
        
        if (p && q && g && pub_key && priv_key) {
            // Use some data for key parameters
            BN_bin2bn(data, size > 64 ? 64 : size, p);
            BN_bin2bn(data + 64, size > 128 ? 64 : (size - 64), q);
            BN_bin2bn(data + 128, size > 192 ? 64 : (size - 128), g);
            BN_bin2bn(data + 192, size > 256 ? 64 : (size - 192), pub_key);
            
            DSA_set0_pqg(dsa, p, q, g);
            DSA_set0_key(dsa, pub_key, priv_key);
            
            // Convert to EVP_PKEY
            EVP_PKEY *pkey = EVP_PKEY_new();
            if (pkey) {
                EVP_PKEY_set1_DSA(pkey, dsa);
                
                // Write private key to BIO
                // Use part of input data as encryption key
                const unsigned char *kstr = data;
                int klen = size > 32 ? 32 : size;
                
                PEM_write_bio_PrivateKey(out_bio, pkey, EVP_aes_256_cbc(), 
                                        kstr, klen, NULL, NULL);
                
                EVP_PKEY_free(pkey);
            }
        }
        
        // 8. Use DSA_size
        int dsa_size = DSA_size(dsa);
        (void)dsa_size; // Use result to avoid unused variable warning
        
        DSA_free(dsa);
    }
    
    // 4. Use TS_REQ_get_nonce
    // Create a TS_REQ from input data
    const unsigned char *p = data;
    TS_REQ *ts_req = d2i_TS_REQ(NULL, &p, size);
    if (ts_req) {
        const ASN1_INTEGER *nonce = TS_REQ_get_nonce(ts_req);
        (void)nonce; // Use result
        TS_REQ_free(ts_req);
    }
    
    // 5. Write public key (if we had one)
    // Create a simple RSA key for testing
    EVP_PKEY *pubkey = EVP_PKEY_new();
    if (pubkey) {
        RSA *rsa = RSA_new();
        if (rsa) {
            BIGNUM *e = BN_new();
            if (e) {
                BN_set_word(e, 65537);
                RSA_generate_key_ex(rsa, 512, e, NULL);
                EVP_PKEY_set1_RSA(pubkey, rsa);
                PEM_write_bio_PUBKEY(out_bio, pubkey);
                BN_free(e);
            }
            RSA_free(rsa);
        }
        EVP_PKEY_free(pubkey);
    }
    
    // 6. Use DES_ofb64_encrypt
    if (size >= 64) {
        DES_key_schedule schedule;
        DES_cblock key_block, ivec;
        int num = 0;
        
        // Use first 8 bytes as DES key
        memcpy(key_block, data, 8);
        // Use next 8 bytes as IV
        memcpy(ivec, data + 8, 8);
        
        DES_set_key_unchecked(&key_block, &schedule);
        
        // Prepare input/output buffers
        size_t des_len = size > 128 ? 64 : (size - 64);
        if (des_len > 0) {
            unsigned char *in_buf = (unsigned char *)malloc(des_len);
            unsigned char *out_buf = (unsigned char *)malloc(des_len);
            
            if (in_buf && out_buf) {
                memcpy(in_buf, data + 64, des_len);
                DES_ofb64_encrypt(in_buf, out_buf, des_len, &schedule, &ivec, &num);
                
                // Write encrypted data to output
                BIO_write(out_bio, out_buf, des_len);
                
                free(in_buf);
                free(out_buf);
            }
        }
    }
    
    // 7. Use OPENSSL_sk_freefunc type conversion
    // Create a stack of BIOs
    OPENSSL_STACK *bio_stack = OPENSSL_sk_new_null();
    if (bio_stack) {
        BIO *bio1 = BIO_new(BIO_s_mem());
        BIO *bio2 = BIO_new(BIO_s_mem());
        
        if (bio1 && bio2) {
            OPENSSL_sk_push(bio_stack, bio1);
            OPENSSL_sk_push(bio_stack, bio2);
            
            // Get the free function type
            OPENSSL_sk_freefunc free_func = ossl_check_BIO_freefunc_type(sk_BIO_freefunc(NULL));
            (void)free_func; // Use result
            
            // Free the stack (which will free the BIOs)
            OPENSSL_sk_pop_free(bio_stack, (OPENSSL_sk_freefunc)BIO_free);
        } else {
            if (bio1) BIO_free(bio1);
            if (bio2) BIO_free(bio2);
            OPENSSL_sk_free(bio_stack);
        }
    }
    
    // Cleanup
    if (param) {
        OPENSSL_free(param);
    }
    
    if (ssl) {
        SSL_free(ssl);
    }
    if (ctx) {
        SSL_CTX_free(ctx);
    }
    
    BIO_free(in_bio);
    BIO_free(out_bio);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}