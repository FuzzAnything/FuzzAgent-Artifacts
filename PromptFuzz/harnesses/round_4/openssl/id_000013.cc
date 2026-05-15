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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 13
//<Quality> {"density":8,"unique_branches":{"SSL_is_stream_local":[[96,6,96,19,1,0,4,0],[1242,29,1242,48,2,0,4,1]],"EVP_EncryptUpdate":[[23,26,23,52,6,0,4,0]],"evp_cipher_init_internal":[[118,13,118,29,0,0,4,0]],"DES_quad_cksum":[[43,9,43,22,0,0,4,1],[50,18,50,25,0,0,4,0],[50,29,50,44,0,0,4,0],[50,29,50,44,0,0,4,1],[53,16,53,21,0,0,4,0],[53,16,53,21,0,0,4,1],[54,17,54,22,0,0,4,0],[54,17,54,22,0,0,4,1],[71,13,71,23,0,0,4,0]]},"library_calls":["DES_quad_cksum","SSL_CTX_new","TLS_method","SSL_new","SSL_is_stream_local","SSL_free","SSL_CTX_free","CRYPTO_THREAD_lock_new","CRYPTO_atomic_load","CRYPTO_THREAD_lock_free","EVP_rc5_32_12_16_ecb","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_CIPHER_CTX_free","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_CMS_RevocationInfoChoice_sk_type","OPENSSL_sk_push","ossl_check_CMS_RevocationInfoChoice_sk_type","ossl_check_CMS_RevocationInfoChoice_type","OPENSSL_sk_free","ossl_check_CMS_RevocationInfoChoice_sk_type","ossl_check_X509_EXTENSION_compfunc_type"],"critical_calls":["DES_quad_cksum","SSL_CTX_new","TLS_method","SSL_new","SSL_is_stream_local","SSL_free","SSL_CTX_free","CRYPTO_THREAD_lock_new","CRYPTO_atomic_load","CRYPTO_THREAD_lock_free","EVP_rc5_32_12_16_ecb","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_EncryptUpdate","EVP_CIPHER_CTX_free","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_CMS_RevocationInfoChoice_sk_type","OPENSSL_sk_push","ossl_check_CMS_RevocationInfoChoice_type","ossl_check_CMS_RevocationInfoChoice_sk_type","OPENSSL_sk_free","ossl_check_CMS_RevocationInfoChoice_sk_type","ossl_check_X509_EXTENSION_compfunc_type"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 64) {
	return 0;
	}
	 // Need minimum data for meaningful processing

    // 1. Process DES checksum
    DES_cblock output, seed;
    memset(&seed, 0, sizeof(DES_cblock));
    DES_quad_cksum(data, &output, size, 1, &seed);

    // 2. Create SSL context and check stream local status
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    if (!ctx) {
	return 0;
	}
	
    
    SSL *ssl = SSL_new(ctx);
    if (ssl) {
        SSL_is_stream_local(ssl);
        SSL_free(ssl);
    }
    SSL_CTX_free(ctx);

    // 3. Atomic load operation
    uint64_t val = 0x1234567890ABCDEF;
    uint64_t ret = 0;
    CRYPTO_RWLOCK *lock = CRYPTO_THREAD_lock_new();
    if (lock) {
        CRYPTO_atomic_load(&val, &ret, lock);
        CRYPTO_THREAD_lock_free(lock);
    }

    // 4. Use RC5 cipher in ECB mode
    const EVP_CIPHER *cipher = EVP_rc5_32_12_16_ecb();
    if (cipher && size >= 16) {
        EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
        if (cipher_ctx) {
            unsigned char key[16];
            unsigned char iv[8];
            unsigned char in[16], out[32];
            int outlen = 0;
            
            // Use first 16 bytes as key, next 8 as IV
            memcpy(key, data, 16);
            memcpy(iv, data + 16, 8);
            memcpy(in, data + 24, 16);
            
            EVP_EncryptInit_ex(cipher_ctx, cipher, NULL, key, iv);
            EVP_EncryptUpdate(cipher_ctx, out, &outlen, in, 16);
            EVP_CIPHER_CTX_free(cipher_ctx);
        }
    }

    // 5. Process CMS revocation info stack
    struct stack_st_CMS_RevocationInfoChoice *rev_stack = sk_CMS_RevocationInfoChoice_new_null();
    if (rev_stack) {
        OPENSSL_STACK *ossl_stack = ossl_check_CMS_RevocationInfoChoice_sk_type(rev_stack);
        // Add dummy entry
        if (size > 0) {
            CMS_RevocationInfoChoice *dummy = NULL;
            sk_CMS_RevocationInfoChoice_push(rev_stack, dummy);
        }
        sk_CMS_RevocationInfoChoice_free(rev_stack);
    }

    // 6. Use X509_EXTENSION comparison function
    sk_X509_EXTENSION_compfunc cmp_func = [](const X509_EXTENSION *const *a, const X509_EXTENSION *const *b) -> int {
        return 0; // Simple dummy comparison
    };
    OPENSSL_sk_compfunc checked_cmp = ossl_check_X509_EXTENSION_compfunc_type(cmp_func);
    
    // 7. Process input via FILE* as requested
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read and process some data
        unsigned char buffer[32];
        fread(buffer, 1, sizeof(buffer), in_file);
        assert_file_closed(&in_file);;
    }

    // 8. Write output to file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(data, 1, size > 1024 ? 1024 : size, out_file);
        assert_file_closed(&out_file);;
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}