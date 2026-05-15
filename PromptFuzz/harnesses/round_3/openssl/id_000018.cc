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
//<ID> 327
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 6
//<Quality> {"density":9,"unique_branches":{"evp_pkey_get0_RSA_int":[[42,9,42,35,0,0,4,0],[42,39,42,69,0,0,4,0]],"ECDSA_SIG_free":[[1211,9,1211,20,0,0,4,1]],"evp_pkey_free_legacy":[[1707,9,1707,22,0,0,4,0],[1707,26,1707,58,0,0,4,1],[1710,9,1710,22,0,0,4,1]]},"library_calls":["TLS_method","SSL_CTX_new","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","EVP_PKEY_new","BN_new","RSA_new","BN_bin2bn","RSA_generate_key_ex","BN_free","EVP_PKEY_assign","EVP_PKEY_get1_RSA","RSA_free","DH_meth_new","d2i_PKCS7_ENC_CONTENT","PKCS7_RECIP_INFO_new","RC2_set_key","RC2_cfb64_encrypt","BIO_dump_indent_fp","ECDSA_SIG_new","ECDSA_SIG_get0_s","ECDSA_SIG_free","PKCS7_RECIP_INFO_free","PKCS7_ENC_CONTENT_free","DH_meth_free","EVP_PKEY_get0_RSA","RSA_free","BN_free","EVP_PKEY_free","OSSL_CMP_CTX_free","SSL_CTX_free"],"critical_calls":["TLS_method","SSL_CTX_new","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","EVP_PKEY_new","BN_new","RSA_new","BN_bin2bn","RSA_generate_key_ex","BN_free","EVP_PKEY_assign","EVP_PKEY_get1_RSA","RSA_free","DH_meth_new","d2i_PKCS7_ENC_CONTENT","PKCS7_RECIP_INFO_new","RC2_set_key","RC2_cfb64_encrypt","BIO_dump_indent_fp","ECDSA_SIG_new","ECDSA_SIG_get0_s","ECDSA_SIG_free","PKCS7_RECIP_INFO_free","PKCS7_ENC_CONTENT_free","DH_meth_free","EVP_PKEY_get0_RSA","RSA_free","BN_free","EVP_PKEY_free","OSSL_CMP_CTX_free","SSL_CTX_free"],"visited":0}
/**/


// Dummy CT validation callback
static int  id_000018_dummy_ct_validation_cb(const CT_POLICY_EVAL_CTX *ctx,
                                  const struct stack_st_SCT *scts,
                                  void *arg) {
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. SSL Context with CT validation
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ssl_ctx = SSL_CTX_new(method);
    if (ssl_ctx) {
        SSL_CTX_set_ct_validation_callback(ssl_ctx,  id_000018_dummy_ct_validation_cb, NULL);
    }

    // 2. CMP Context
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(NULL, NULL);
    if (cmp_ctx) {
        void *http_arg = OSSL_CMP_CTX_get_http_cb_arg(cmp_ctx);
        (void)http_arg; // Use to avoid unused warning
    }

    // 3. Create EVP_PKEY and RSA key
    EVP_PKEY *pkey = EVP_PKEY_new();
    BIGNUM *bn = BN_new();
    RSA *rsa = RSA_new();
    
    // Use input data for RSA generation if enough bytes
    if (size >= 8 && bn && rsa) {
        // Use first 4 bytes for exponent
        BIGNUM *e = BN_bin2bn(data, 4, NULL);
        if (e) {
            // Use next 4 bytes for bit count (ensure reasonable range)
            int bits = 512 + (data[4] % 1536); // 512-2048 bits
            RSA_generate_key_ex(rsa, bits, e, NULL);
            BN_free(e);
        }
        
        // Assign RSA to EVP_PKEY
        if (rsa) {
            EVP_PKEY_assign(pkey, EVP_PKEY_RSA, rsa);
            // Get it back
            RSA *rsa2 = EVP_PKEY_get1_RSA(pkey);
            if (rsa2) RSA_free(rsa2);
        }
    }

    // 4. DH method
    DH_METHOD *dh_meth = DH_meth_new("fuzz", 0);
    
    // 5. PKCS7 decoding (use part of input data)
    const unsigned char *p = data;
    long remaining = size;
    PKCS7_ENC_CONTENT *enc_content = d2i_PKCS7_ENC_CONTENT(NULL, &p, remaining);
    if (enc_content) {
        remaining -= (p - data);
    }
    
    // 6. PKCS7 recipient info
    PKCS7_RECIP_INFO *recip = PKCS7_RECIP_INFO_new();
    
    // 7. RC2 encryption (use remaining data)
    if (remaining >= 32) {
        RC2_KEY rc2_key;
        unsigned char ivec[8] = {0};
        int num = 0;
        
        // Set key using first 16 bytes of remaining data
        RC2_set_key(&rc2_key, 16, p, 128);
        
        // Encrypt next block (up to 64 bytes)
        size_t enc_len = (remaining - 16 > 64) ? 64 : (remaining - 16);
        if (enc_len > 0) {
            unsigned char out[64];
            unsigned char iv[8];
            memcpy(iv, ivec, 8);
            RC2_cfb64_encrypt(p + 16, out, enc_len, &rc2_key, iv, &num, 1);
        }
    }
    
    // 8. Get EVP_RAND params (dummy - we don't have EVP_RAND_CTX)
    // const OSSL_PARAM *rand_params = EVP_RAND_CTX_gettable_params(NULL);
    
    // 9. Dump data to file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file && size > 0) {
        BIO_dump_indent_fp(out_file, data, (size > 256) ? 256 : size, 4);
        assert_file_closed(&out_file);;
    }
    
    // 10. ECDSA signature (simulated)
    if (size >= 32) {
        // Create dummy ECDSA_SIG
        ECDSA_SIG *sig = ECDSA_SIG_new();
        if (sig) {
            // Get 's' component (dummy - actual sig would have values)
            const BIGNUM *s = ECDSA_SIG_get0_s(sig);
            (void)s; // Use to avoid warning
            ECDSA_SIG_free(sig);
        }
    }
    
    // 11. Cleanup
    if (recip) PKCS7_RECIP_INFO_free(recip);
    if (enc_content) PKCS7_ENC_CONTENT_free(enc_content);
    if (dh_meth) DH_meth_free(dh_meth);
    if (rsa && !EVP_PKEY_get0_RSA(pkey)) RSA_free(rsa); // Free if not assigned to pkey
    if (bn) BN_free(bn);
    if (pkey) EVP_PKEY_free(pkey);
    if (cmp_ctx) OSSL_CMP_CTX_free(cmp_ctx);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    
    // 12. Free function thunk (simulated usage)
    void (*freefunc)(void*) = [](void *ptr) { free(ptr); };
    void *dummy_ptr = malloc(1);
    if (dummy_ptr) {
        freefunc(dummy_ptr);
    }
    
    assert_file_closed(&out_file);
	return 0;
}