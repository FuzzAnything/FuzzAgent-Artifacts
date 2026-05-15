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
//<ID> 399
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 11
//<Quality> {"density":15,"unique_branches":{"SHA1_Final":[[247,9,247,30,0,0,4,0],[247,9,247,30,0,0,4,1]],"rsa_multiprime_keygen":[[396,17,397,29,0,0,4,0],[609,9,609,17,0,0,4,0]],"SHA1_Update_thunk":[[214,9,214,14,0,0,4,0],[214,9,214,14,0,0,4,1],[222,9,222,17,0,0,4,0],[222,9,222,17,0,0,4,1],[23,26,23,52,3,0,4,1],[23,26,23,52,4,0,4,1],[22,24,22,50,5,0,4,1]]},"library_calls":["OPENSSL_init_ssl","ERR_clear_error","BIO_new_fp","BIO_s_mem","BIO_new","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_X509_VERIFY_PARAM_sk_type","X509_VERIFY_PARAM_new","ossl_check_X509_VERIFY_PARAM_type","OPENSSL_sk_push","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get0_trustedStore","EVP_idea_cfb64","SSL_CTX_new","TLS_method","SSL_CTX_SRP_CTX_free","BIO_write","BIO_get_shutdown","SHA1_Init","SHA1_Update","SHA1_Final","DSA_new","BN_new","BN_new","BN_bin2bn","BN_free","BN_set_word","DSA_set0_key","DSA_do_sign","DSA_SIG_free","RSA_new","BN_new","BN_GENCB_new","BN_set_word","RSA_generate_key_ex","BN_GENCB_free","BN_free","EVP_PKEY_new","EVP_PKEY_assign","PEM_read_bio_X509_REQ","BIO_ctrl","d2i_X509_REQ_bio","X509_REQ_check_private_key","DSA_free","RSA_free","EVP_PKEY_free","X509_REQ_free","X509_REQ_free","OPENSSL_sk_pop_free","ossl_check_X509_VERIFY_PARAM_freefunc_type","X509_VERIFY_PARAM_free","X509_VERIFY_PARAM_free","OSSL_CMP_CTX_free","SSL_CTX_free","BIO_free","BIO_free","OPENSSL_sk_free"],"critical_calls":["OPENSSL_init_ssl","ERR_clear_error","BIO_new_fp","BIO_s_mem","BIO_new","OPENSSL_sk_new_null","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_X509_VERIFY_PARAM_sk_type","X509_VERIFY_PARAM_new","ossl_check_X509_VERIFY_PARAM_type","OPENSSL_sk_push","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get0_trustedStore","EVP_idea_cfb64","SSL_CTX_new","TLS_method","SSL_CTX_SRP_CTX_free","BIO_write","BIO_get_shutdown","SHA1_Init","SHA1_Update","SHA1_Final","DSA_new","BN_new","BN_new","BN_bin2bn","BN_free","BN_set_word","DSA_set0_key","DSA_do_sign","DSA_SIG_free","RSA_new","BN_new","BN_GENCB_new","BN_set_word","RSA_generate_key_ex","BN_GENCB_free","BN_free","EVP_PKEY_new","EVP_PKEY_assign","PEM_read_bio_X509_REQ","BIO_ctrl","d2i_X509_REQ_bio","X509_REQ_check_private_key","DSA_free","RSA_free","EVP_PKEY_free","X509_REQ_free","X509_REQ_free","OPENSSL_sk_pop_free","ossl_check_X509_VERIFY_PARAM_freefunc_type","X509_VERIFY_PARAM_free","X509_VERIFY_PARAM_free","OSSL_CMP_CTX_free","SSL_CTX_free","BIO_free","BIO_free","OPENSSL_sk_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL
    OPENSSL_init_ssl(0, nullptr);
    ERR_clear_error();

    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output FILE*
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create BIO from FILE* for reading
    BIO *bio_fp = BIO_new_fp(in_file, BIO_NOCLOSE);
    // Create memory BIO for writing
    const BIO_METHOD *mem_method = BIO_s_mem();
    BIO *bio_mem = BIO_new(mem_method);

    // Create stack and set thunks
    OPENSSL_STACK *stack = OPENSSL_sk_new_null();
    OPENSSL_sk_set_thunks(stack, [](OPENSSL_sk_freefunc freefunc_arg, void *ptr) {
        // Thunk function - does nothing special
    });

    // Create X509_VERIFY_PARAM stack
    struct stack_st_X509_VERIFY_PARAM *param_stack = 
        (struct stack_st_X509_VERIFY_PARAM *)OPENSSL_sk_new_null();
    OPENSSL_STACK *checked_stack = ossl_check_X509_VERIFY_PARAM_sk_type(param_stack);
    
    // Create and push X509_VERIFY_PARAM
    X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
    X509_VERIFY_PARAM *checked_param = ossl_check_X509_VERIFY_PARAM_type(param);
    OPENSSL_sk_push(checked_stack, checked_param);

    // Create OSSL_CMP_CTX
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(nullptr, nullptr);
    if (cmp_ctx) {
        // Get trusted store
        X509_STORE *trusted_store = OSSL_CMP_CTX_get0_trustedStore(cmp_ctx);
        // Use the store (dummy operation)
        if (trusted_store) {
            // Store exists but we won't modify it
        }
    }

    // Initialize IDEA cipher (CFB64 mode)
    const EVP_CIPHER *idea_cipher = EVP_idea_cfb64();
    // Use cipher (dummy operation)
    if (idea_cipher) {
        // Cipher exists but we won't encrypt/decrypt in this fuzz driver
    }

    // SSL SRP context operations
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        SSL_CTX_SRP_CTX_free(ssl_ctx);
    }

    // Write data to memory BIO
    if (size > 0) {
        BIO_write(bio_mem, data, (int)size);
    }
    
    // Get BIO shutdown state
    int shutdown_state = BIO_get_shutdown(bio_mem);

    // SHA1 operations
    SHA_CTX sha_ctx;
    unsigned char sha_hash[SHA_DIGEST_LENGTH];
    // Initialize and update SHA1 context with input data
    SHA1_Init(&sha_ctx);
    if (size > 0) {
        SHA1_Update(&sha_ctx, data, size);
    }
    SHA1_Final(sha_hash, &sha_ctx);

    // DSA operations
    DSA *dsa = DSA_new();
    if (dsa) {
        BIGNUM *pub_key = BN_new();
        BIGNUM *priv_key = BN_new();
        
        // Convert parts of input data to BIGNUMs
        size_t bn_size = size > 20 ? 20 : size;
        if (bn_size > 0) {
            BIGNUM *temp_bn = BN_bin2bn(data, (int)bn_size, nullptr);
            if (temp_bn) {
                BN_free(temp_bn);
            }
        }
        
        // Set a word value
        if (pub_key) {
            BN_set_word(pub_key, 65537);
        }
        
        // Set DSA keys (using dummy values since we don't have actual key material)
        DSA_set0_key(dsa, pub_key, priv_key);
        
        // Try to sign the SHA1 hash
        if (size > 0) {
            DSA_SIG *sig = DSA_do_sign(sha_hash, SHA_DIGEST_LENGTH, dsa);
            if (sig) {
                // Free the signature if created
                DSA_SIG_free(sig);
            }
        }
    }

    // RSA operations
    RSA *rsa = RSA_new();
    if (rsa) {
        BIGNUM *e = BN_new();
        BN_GENCB *cb = BN_GENCB_new();
        
        if (e) {
            BN_set_word(e, 65537);
            // Generate RSA key (small bits for speed)
            RSA_generate_key_ex(rsa, 512, e, cb);
        }
        
        if (cb) {
            BN_GENCB_free(cb);
        }
        if (e) {
            BN_free(e);
        }
    }

    // EVP_PKEY operations
    EVP_PKEY *pkey = EVP_PKEY_new();
    if (pkey && rsa) {
        // Assign RSA key to EVP_PKEY
        EVP_PKEY_assign(pkey, EVP_PKEY_RSA, rsa);
        rsa = nullptr; // Ownership transferred
    }

    // Read X509_REQ from BIO
    X509_REQ *req1 = PEM_read_bio_X509_REQ(bio_fp, nullptr, nullptr, nullptr);
    
    // Reset BIO position and try DER format
    BIO_ctrl(bio_fp, BIO_CTRL_RESET, 0, nullptr);
    X509_REQ *req2 = d2i_X509_REQ_bio(bio_fp, nullptr);
    
    // Check private key against certificate request if both exist
    if (req1 && pkey) {
        X509_REQ_check_private_key(req1, pkey);
    }

    // Free resources
    if (dsa) {
        DSA_free(dsa);
    }
    
    if (rsa) {
        RSA_free(rsa);
    }
    
    if (pkey) {
        EVP_PKEY_free(pkey);
    }
    
    if (req1) {
        X509_REQ_free(req1);
    }
    
    if (req2) {
        X509_REQ_free(req2);
    }
    
    // Free stack with pop_free
    OPENSSL_sk_pop_free(checked_stack, 
        ossl_check_X509_VERIFY_PARAM_freefunc_type(
            [](X509_VERIFY_PARAM *param) { 
                X509_VERIFY_PARAM_free(param); 
            }));
    
    // Free OSSL_CMP_CTX
    if (cmp_ctx) {
        OSSL_CMP_CTX_free(cmp_ctx);
    }
    
    // Free SSL_CTX
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
    }
    
    // Free BIOs
    if (bio_fp) {
        BIO_free(bio_fp);
    }
    
    if (bio_mem) {
        BIO_free(bio_mem);
    }
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Free the general stack
    if (stack) {
        OPENSSL_sk_free(stack);
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}