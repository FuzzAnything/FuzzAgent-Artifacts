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
//<ID> 556
//<Prompt> []
/*<Combination>: [
*/
//<score> 329, nr_unique_branch: 13
//<Quality> {"density":47,"unique_branches":{"prepare_from_text":[[42,9,42,22,0,0,4,0],[44,9,44,18,0,0,4,0],[58,30,58,74,13,0,4,1]],"ossl_rsa_multip_cap":[[102,9,102,20,0,0,4,1],[104,14,104,25,0,0,4,0]],"bn_mul_fixed_top":[[550,9,550,36,0,0,4,0],[550,40,550,67,0,0,4,1]],"bn_to_asn1_string":[[503,9,503,17,0,0,4,1],[512,9,512,23,0,0,4,1]],"OSSL_PARAM_allocate_from_text":[[320,9,320,19,0,0,4,1],[320,23,320,40,0,0,4,1],[323,9,324,54,0,0,4,0]],"bn_sqr_fixed_top":[[59,16,59,23,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","BIO_new_fp","BIO_new_fp","TLS_method","SSL_CTX_new","SSL_new","SSL_trace","DSA_new","BN_new","BN_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","DSA_set0_pqg","DSA_set0_key","EVP_PKEY_new","EVP_PKEY_set1_DSA","EVP_aes_256_cbc","PEM_write_bio_PrivateKey","d2i_TS_REQ","TS_REQ_get_nonce","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_new","EVP_PKEY_set1_RSA","PEM_write_bio_PUBKEY","DES_set_key_unchecked","DES_ofb64_encrypt","OSSL_PARAM_allocate_from_text","OPENSSL_sk_new_null","OPENSSL_sk_push","BIO_new","BIO_s_mem","BIO_write","PKCS7_new","X509_STORE_new","PKCS7_verify","DSA_size","TS_REQ_free","OPENSSL_sk_free","BIO_free","PKCS7_free","X509_STORE_free","X509_ALGOR_new","X509_ALGOR_free","SSL_free","SSL_CTX_free","EVP_PKEY_free","EVP_PKEY_free","DSA_free","RSA_free","BN_free","BIO_free","BIO_free"],"critical_calls":["OPENSSL_init_ssl","BIO_new_fp","BIO_new_fp","TLS_method","SSL_CTX_new","SSL_new","SSL_trace","DSA_new","BN_new","BN_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","DSA_set0_pqg","DSA_set0_key","EVP_PKEY_new","EVP_PKEY_set1_DSA","EVP_aes_256_cbc","PEM_write_bio_PrivateKey","d2i_TS_REQ","TS_REQ_get_nonce","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_new","EVP_PKEY_set1_RSA","PEM_write_bio_PUBKEY","DES_set_key_unchecked","DES_ofb64_encrypt","OSSL_PARAM_allocate_from_text","OPENSSL_sk_new_null","OPENSSL_sk_push","BIO_new","BIO_s_mem","BIO_write","PKCS7_new","X509_STORE_new","PKCS7_verify","DSA_size","TS_REQ_free","OPENSSL_sk_free","BIO_free","PKCS7_free","X509_STORE_free","X509_ALGOR_new","X509_ALGOR_free","SSL_free","SSL_CTX_free","EVP_PKEY_free","EVP_PKEY_free","DSA_free","RSA_free","BN_free","BIO_free","BIO_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 128) {
        // Need minimum data for operations
        return 0;
    }

    // 1. Initialize SSL
    OPENSSL_init_ssl(0, NULL);

    // 2. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Create BIO from FILE*
    BIO *bio_fp = BIO_new_fp(in_file, BIO_NOCLOSE);
    BIO *bio_out = BIO_new_fp(out_file, BIO_NOCLOSE);
    
    // 4. Create SSL context and SSL object
    const SSL_METHOD *meth = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(meth);
    SSL *ssl = SSL_new(ctx);
    
    // 5. Set up tracing callback
    SSL_trace(1, 3, 23, data, size < 100 ? size : 100, ssl, NULL);

    // 6. Create DSA key from input data
    DSA *dsa = DSA_new();
    BIGNUM *p = BN_new();
    BIGNUM *q = BN_new();
    BIGNUM *g = BN_new();
    BIGNUM *pub_key = BN_new();
    BIGNUM *priv_key = BN_new();
    
    // Use portions of input data for DSA parameters
    size_t offset = 0;
    if (size - offset >= 20) {
        BN_bin2bn(data + offset, 20, p);
        offset += 20;
    }
    if (size - offset >= 20) {
        BN_bin2bn(data + offset, 20, q);
        offset += 20;
    }
    if (size - offset >= 20) {
        BN_bin2bn(data + offset, 20, g);
        offset += 20;
    }
    if (size - offset >= 20) {
        BN_bin2bn(data + offset, 20, pub_key);
        offset += 20;
    }
    if (size - offset >= 20) {
        BN_bin2bn(data + offset, 20, priv_key);
        offset += 20;
    }
    
    DSA_set0_pqg(dsa, p, q, g);
    DSA_set0_key(dsa, pub_key, priv_key);
    
    // 7. Create EVP_PKEY from DSA and write to BIO
    EVP_PKEY *pkey_dsa = EVP_PKEY_new();
    EVP_PKEY_set1_DSA(pkey_dsa, dsa);
    
    const EVP_CIPHER *cipher = EVP_aes_256_cbc();
    PEM_write_bio_PrivateKey(bio_out, pkey_dsa, cipher, 
                           (unsigned char*)"password", 8, NULL, NULL);
    
    // 8. Parse TS_REQ from input data
    const unsigned char *ts_data = data + offset;
    long ts_len = size - offset > 100 ? 100 : size - offset;
    TS_REQ *ts_req = d2i_TS_REQ(NULL, &ts_data, ts_len);
    if (ts_req) {
        const ASN1_INTEGER *nonce = TS_REQ_get_nonce(ts_req);
    }
    
    // 9. Create RSA key
    RSA *rsa = RSA_new();
    BIGNUM *e = BN_new();
    BN_set_word(e, 65537);
    RSA_generate_key_ex(rsa, 1024, e, NULL);
    
    EVP_PKEY *pkey_rsa = EVP_PKEY_new();
    EVP_PKEY_set1_RSA(pkey_rsa, rsa);
    PEM_write_bio_PUBKEY(bio_out, pkey_rsa);
    
    // 10. DES encryption
    DES_key_schedule schedule;
    DES_cblock key;
    DES_cblock ivec;
    int num = 0;
    
    if (size - offset >= 8) {
        memcpy(key, data + offset, 8);
        offset += 8;
    }
    if (size - offset >= 8) {
        memcpy(ivec, data + offset, 8);
        offset += 8;
    }
    
    DES_set_key_unchecked(&key, &schedule);
    
    // Encrypt portion of remaining data
    size_t enc_len = size - offset > 64 ? 64 : size - offset;
    unsigned char *enc_out = new unsigned char[enc_len];
    if (enc_len > 0) {
        DES_ofb64_encrypt(data + offset, enc_out, enc_len, 
                         &schedule, &ivec, &num);
    }
    delete[] enc_out;
    
    // 11. Use OSSL_PARAM_allocate_from_text
    OSSL_PARAM param, param_def;
    const char *key_str = "test_key";
    const char *value = "test_value";
    int found = 0;
    
    OSSL_PARAM_allocate_from_text(&param, &param_def, 
                                 key_str, value, strlen(value), &found);
    
    // 12. Create stack and push data
    OPENSSL_STACK *stack = OPENSSL_sk_new_null();
    if (stack && ts_req) {
        OPENSSL_sk_push(stack, (void*)ts_req);
    }
    
    // 13. Create memory BIO
    BIO *mem_bio = BIO_new(BIO_s_mem());
    if (mem_bio && size > 50) {
        BIO_write(mem_bio, data, size > 50 ? 50 : size);
    }
    
    // 14. PKCS7 verify (simplified - would need actual PKCS7 data)
    // Create a minimal PKCS7 structure
    PKCS7 *p7 = PKCS7_new();
    X509_STORE *store = X509_STORE_new();
    
    // Try verification (will likely fail with test data)
    PKCS7_verify(p7, NULL, store, mem_bio, NULL, 0);
    
    // 15. Get DSA size
    int dsa_sig_size = DSA_size(dsa);
    
    // 16. Cleanup
    if (ts_req) TS_REQ_free(ts_req);
    if (stack) OPENSSL_sk_free(stack);
    if (mem_bio) BIO_free(mem_bio);
    if (p7) PKCS7_free(p7);
    if (store) X509_STORE_free(store);
    
    X509_ALGOR *algor = X509_ALGOR_new();
    if (algor) X509_ALGOR_free(algor);
    
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    
    EVP_PKEY_free(pkey_dsa);
    EVP_PKEY_free(pkey_rsa);
    
    DSA_free(dsa);
    RSA_free(rsa);
    BN_free(e);
    
    BIO_free(bio_fp);
    BIO_free(bio_out);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}