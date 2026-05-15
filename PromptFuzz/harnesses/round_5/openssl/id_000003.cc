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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["ERR_load_CRYPTO_strings","d2i_OCSP_REQUEST","X509_EXTENSION_new","OCSP_REQUEST_add_ext","X509_EXTENSION_free","d2i_ACCESS_DESCRIPTION","OSSL_TARGET_it","d2i_PKCS12","PKCS12_mac_present","Camellia_set_key","Camellia_cbc_encrypt","d2i_X509","SSL_CTX_new","TLS_method","SSL_CTX_use_certificate","SSL_CTX_free","EVP_MD_CTX_new","RSA_new","EVP_PKEY_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_assign","EVP_DigestVerifyInit","EVP_sha256","BN_free","EVP_PKEY_free","RSA_free","EVP_PKEY_free","OCSP_REQUEST_free","ACCESS_DESCRIPTION_free","PKCS12_free","X509_free","EVP_MD_CTX_free"],"critical_calls":["ERR_load_CRYPTO_strings","d2i_OCSP_REQUEST","X509_EXTENSION_new","OCSP_REQUEST_add_ext","X509_EXTENSION_free","d2i_ACCESS_DESCRIPTION","OSSL_TARGET_it","d2i_PKCS12","PKCS12_mac_present","Camellia_set_key","Camellia_cbc_encrypt","d2i_X509","SSL_CTX_new","TLS_method","SSL_CTX_use_certificate","SSL_CTX_free","EVP_MD_CTX_new","RSA_new","EVP_PKEY_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_assign","EVP_DigestVerifyInit","EVP_sha256","BN_free","EVP_PKEY_free","OCSP_REQUEST_free","ACCESS_DESCRIPTION_free","PKCS12_free","X509_free","EVP_MD_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL error strings
    ERR_load_CRYPTO_strings();
    
    // Parse OCSP request from input data
    OCSP_REQUEST *ocsp_req = NULL;
    const unsigned char *p = data;
    ocsp_req = d2i_OCSP_REQUEST(NULL, &p, size);
    
    if (ocsp_req) {
        // Create a dummy X509 extension
        X509_EXTENSION *ex = X509_EXTENSION_new();
        if (ex) {
            // Add extension to OCSP request
            OCSP_REQUEST_add_ext(ocsp_req, ex, -1);
            X509_EXTENSION_free(ex);
        }
    }
    
    // Parse ACCESS_DESCRIPTION from input data
    ACCESS_DESCRIPTION *ad = NULL;
    p = data;
    long remaining = size;
    ad = d2i_ACCESS_DESCRIPTION(NULL, &p, remaining);
    
    // Get OSSL_TARGET ASN1_ITEM
    const ASN1_ITEM *target_item = OSSL_TARGET_it();
    
    // Parse PKCS12 structure from input data
    PKCS12 *p12 = NULL;
    p = data;
    p12 = d2i_PKCS12(NULL, &p, size);
    
    if (p12) {
        // Check if MAC is present
        PKCS12_mac_present(p12);
    }
    
    // Camellia CBC encryption/decryption
    if (size >= 48) { // Need enough data for key, IV, and some plaintext
        CAMELLIA_KEY key;
        unsigned char iv[16];
        unsigned char *out = (unsigned char *)malloc(size - 32);
        
        // Use first 32 bytes as key, next 16 as IV
        Camellia_set_key(data, 256, &key); // 256-bit key
        memcpy(iv, data + 32, 16);
        
        // Encrypt the remaining data
        if (out && (size - 48) > 0) {
            Camellia_cbc_encrypt(data + 48, out, size - 48, &key, iv, CAMELLIA_ENCRYPT);
        }
        
        free(out);
    }
    
    // Parse X509 certificate from input data
    X509 *cert = NULL;
    p = data;
    cert = d2i_X509(NULL, &p, size);
    
    if (cert) {
        // Create SSL context and use certificate
        SSL_CTX *ctx = SSL_CTX_new(SSLv23_method());
        if (ctx) {
            SSL_CTX_use_certificate(ctx, cert);
            SSL_CTX_free(ctx);
        }
    }
    
    // Setup EVP digest verification
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    EVP_PKEY_CTX *pkey_ctx = NULL;
    
    if (md_ctx) {
        // Create a dummy RSA key for verification
        RSA *rsa = RSA_new();
        EVP_PKEY *pkey = EVP_PKEY_new();
        
        if (rsa && pkey) {
            // Generate a minimal RSA key (not secure, for fuzzing only)
            BIGNUM *e = BN_new();
            BN_set_word(e, 65537);
            RSA_generate_key_ex(rsa, 512, e, NULL);
            EVP_PKEY_assign_RSA(pkey, rsa);
            
            // Initialize digest verification context
            EVP_DigestVerifyInit(md_ctx, &pkey_ctx, EVP_sha256(), NULL, pkey);
            
            BN_free(e);
            // Note: rsa is freed when pkey is freed
            EVP_PKEY_free(pkey);
        } else {
            if (rsa) RSA_free(rsa);
            if (pkey) EVP_PKEY_free(pkey);
        }
    }
    
    // Cleanup
    if (ocsp_req) OCSP_REQUEST_free(ocsp_req);
    if (ad) ACCESS_DESCRIPTION_free(ad);
    if (p12) PKCS12_free(p12);
    if (cert) X509_free(cert);
    if (md_ctx) EVP_MD_CTX_free(md_ctx);
    
    return 0;
}