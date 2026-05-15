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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 10
//<Quality> {"density":6,"unique_branches":{"RSA_padding_add_X931":[[55,9,55,14,0,0,4,1],[63,9,63,15,0,0,4,1],[67,13,67,18,0,0,4,0]],"aes_128_cfb_newctx":[[223,13,223,24,0,0,4,0],[205,9,205,32,1,0,4,1]],"ossl_err_load_PKCS12_strings":[[60,9,60,69,0,0,4,1]],"aes_cfb128_vaes_encdec_wrapper":[[45,9,45,16,0,0,4,1],[51,9,51,17,0,0,4,0]],"ossl_prov_cipher_hw_aes_cfb128":[[98,9,98,42,1,0,4,0],[185,23,185,64,2,0,4,0]]},"library_calls":["ERR_load_PKCS12_strings","ADMISSION_SYNTAX_new","d2i_NAMING_AUTHORITY","EVP_aes_128_cfb128","ADMISSION_SYNTAX_free","NAMING_AUTHORITY_free","EVP_CIPHER_CTX_new","ADMISSION_SYNTAX_free","NAMING_AUTHORITY_free","EVP_CipherInit_ex","EVP_CIPHER_CTX_free","ADMISSION_SYNTAX_free","NAMING_AUTHORITY_free","EVP_CipherUpdate","RSA_padding_add_X931","EVP_CIPHER_CTX_free","ADMISSION_SYNTAX_free","NAMING_AUTHORITY_free"],"critical_calls":["ERR_load_PKCS12_strings","ADMISSION_SYNTAX_new","d2i_NAMING_AUTHORITY","EVP_aes_128_cfb128","EVP_CIPHER_CTX_new","EVP_CipherInit_ex","EVP_CipherUpdate","RSA_padding_add_X931","EVP_CIPHER_CTX_free","ADMISSION_SYNTAX_free","NAMING_AUTHORITY_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need sufficient data for operations
    
    // Step 1: Load PKCS12 error strings
    ERR_load_PKCS12_strings();
    
    // Step 2: Create ADMISSION_SYNTAX structure
    ADMISSION_SYNTAX *admission = ADMISSION_SYNTAX_new();
    if (!admission) {
	return 0;
	}
	
    
    // Step 3: Decode NAMING_AUTHORITY from input data
    const unsigned char *p = data;
    long len = static_cast<long>(size);
    NAMING_AUTHORITY *naming_auth = d2i_NAMING_AUTHORITY(NULL, &p, len);
    
    if (naming_auth) {
        // Update remaining data pointer and length
        len -= (p - data);
    }
    
    // Step 4: Use AES-128-CFB128 cipher
    const EVP_CIPHER *cipher = EVP_aes_128_cfb128();
    if (!cipher) {
        ADMISSION_SYNTAX_free(admission);
        if (naming_auth) NAMING_AUTHORITY_free(naming_auth);
        return 0;
    }
    
    // Create cipher context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        ADMISSION_SYNTAX_free(admission);
        if (naming_auth) NAMING_AUTHORITY_free(naming_auth);
        return 0;
    }
    
    // Set up key and IV from remaining data
    unsigned char key[16] = {0};
    unsigned char iv[16] = {0};
    
    if (len >= 32) {
        memcpy(key, p, 16);
        memcpy(iv, p + 16, 16);
        p += 32;
        len -= 32;
    } else if (len > 0) {
        size_t copy_len = len > 16 ? 16 : len;
        memcpy(key, p, copy_len);
        if (len > 16) {
            memcpy(iv, p + 16, len - 16);
        }
    }
    
    // Initialize cipher context
    if (EVP_CipherInit_ex(ctx, cipher, NULL, key, iv, 1) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        ADMISSION_SYNTAX_free(admission);
        if (naming_auth) NAMING_AUTHORITY_free(naming_auth);
        return 0;
    }
    
    // Encrypt some data (remaining input)
    size_t encrypt_len = len > 64 ? 64 : len;
    unsigned char plaintext[64] = {0};
    unsigned char ciphertext[80] = {0}; // Extra space for padding
    
    if (encrypt_len > 0) {
        memcpy(plaintext, p, encrypt_len);
        int outlen = 0;
        EVP_CipherUpdate(ctx, ciphertext, &outlen, plaintext, static_cast<int>(encrypt_len));
    }
    
    // Step 5: Use RSA X.931 padding
    // Prepare data for padding (use first 32 bytes of ciphertext or plaintext)
    unsigned char padded_data[256] = {0};
    int padding_result = 0;
    
    // We need at least some data to pad
    unsigned char source_data[32] = {0};
    size_t source_len = encrypt_len > 32 ? 32 : encrypt_len;
    
    if (source_len > 0) {
        memcpy(source_data, plaintext, source_len);
        // Pad for 2048-bit RSA (256 bytes)
        padding_result = RSA_padding_add_X931(padded_data, 256, source_data, static_cast<int>(source_len));
    }
    
    // Cleanup
    EVP_CIPHER_CTX_free(ctx);
    ADMISSION_SYNTAX_free(admission);
    if (naming_auth) NAMING_AUTHORITY_free(naming_auth);
    
    return 0;
}