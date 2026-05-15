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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"d2i_PKCS12_bio":[[253,9,253,20,0,0,4,1]]},"library_calls":["ERR_load_CRYPTO_strings","d2i_ACCESS_DESCRIPTION","OCSP_REQUEST_new","X509V3_EXT_nconf_nid","OCSP_REQUEST_add_ext","X509_EXTENSION_free","PKCS12_mac_present","Camellia_set_key","Camellia_cbc_encrypt","Camellia_cbc_encrypt","SSL_CTX_new","TLS_method","SSL_CTX_use_certificate","X509_free","EVP_MD_CTX_new","EVP_PKEY_new","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_assign","BN_free","EVP_DigestVerifyInit","EVP_sha256","EVP_PKEY_free","OSSL_TARGET_it","ACCESS_DESCRIPTION_free","OCSP_REQUEST_free","PKCS12_free","SSL_CTX_free","EVP_MD_CTX_free"],"critical_calls":["ERR_load_CRYPTO_strings","d2i_ACCESS_DESCRIPTION","OCSP_REQUEST_new","X509V3_EXT_nconf_nid","OCSP_REQUEST_add_ext","X509_EXTENSION_free","PKCS12_mac_present","Camellia_set_key","Camellia_cbc_encrypt","Camellia_cbc_encrypt","SSL_CTX_new","TLS_method","SSL_CTX_use_certificate","X509_free","EVP_MD_CTX_new","EVP_PKEY_new","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_assign","BN_free","EVP_DigestVerifyInit","EVP_sha256","EVP_PKEY_free","OSSL_TARGET_it","ACCESS_DESCRIPTION_free","OCSP_REQUEST_free","PKCS12_free","SSL_CTX_free","EVP_MD_CTX_free"],"visited":1}
/**/


// Function to parse an X509 certificate from memory
X509 * id_000004_parse_x509_from_data(const uint8_t *data, size_t size) {
    BIO *bio = BIO_new_mem_buf(data, size);
    if (!bio) return nullptr;
    
    X509 *cert = d2i_X509_bio(bio, nullptr);
    BIO_free(bio);
    return cert;
}

// Function to parse a PKCS12 structure from memory
PKCS12 * id_000004_parse_pkcs12_from_data(const uint8_t *data, size_t size) {
    BIO *bio = BIO_new_mem_buf(data, size);
    if (!bio) return nullptr;
    
    PKCS12 *p12 = d2i_PKCS12_bio(bio, nullptr);
    BIO_free(bio);
    return p12;
}

// Function to parse an OCSP request from memory
OCSP_REQUEST * id_000004_parse_ocsp_request_from_data(const uint8_t *data, size_t size) {
    const unsigned char *p = data;
    return d2i_OCSP_REQUEST(nullptr, &p, size);
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Load CRYPTO error strings
    ERR_load_CRYPTO_strings();
    
    // 2. Parse ACCESS_DESCRIPTION from input data
    const unsigned char *p = data;
    ACCESS_DESCRIPTION *ad = d2i_ACCESS_DESCRIPTION(nullptr, &p, size);
    size_t consumed = p - data;
    
    // 3. Create and use OCSP_REQUEST with extension
    OCSP_REQUEST *req = OCSP_REQUEST_new();
    if (req && ad) {
        // Create a basic extension from ACCESS_DESCRIPTION
        X509_EXTENSION *ext = X509V3_EXT_nconf_nid(nullptr, nullptr, 
                                                  NID_info_access, 
                                                  "OCSP;URI:http://example.com");
        if (ext) {
            OCSP_REQUEST_add_ext(req, ext, -1);
            X509_EXTENSION_free(ext);
        }
    }
    
    // 4. Check for MAC in PKCS12 structure
    PKCS12 *p12 =  id_000004_parse_pkcs12_from_data(data + consumed, size - consumed);
    if (p12) {
        int mac_present = PKCS12_mac_present(p12);
        // Use the result to avoid optimization
        if (mac_present) {
            // Do nothing, just using the result
        }
    }
    
    // 5. CAMELLIA CBC encryption/decryption
    if (size - consumed >= 48) { // Need at least key + IV + some data
        CAMELLIA_KEY key;
        unsigned char ivec[CAMELLIA_BLOCK_SIZE];
        unsigned char out[256];
        
        // Use first 32 bytes as key, next 16 as IV
        const unsigned char *key_data = data + consumed;
        const unsigned char *iv_data = key_data + 32;
        const unsigned char *in_data = iv_data + 16;
        size_t data_len = size - consumed - 48;
        
        if (data_len > 0 && data_len <= 256) {
            // Set up key (simplified - in real use would use proper key setup)
            Camellia_set_key(key_data, 256, &key);
            memcpy(ivec, iv_data, CAMELLIA_BLOCK_SIZE);
            
            // Encrypt
            Camellia_cbc_encrypt(in_data, out, data_len, &key, ivec, 1);
            
            // Decrypt back
            memcpy(ivec, iv_data, CAMELLIA_BLOCK_SIZE); // Reset IV
            unsigned char dec_out[256];
            Camellia_cbc_encrypt(out, dec_out, data_len, &key, ivec, 0);
        }
    }
    
    // 6. SSL_CTX with certificate
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    if (ctx) {
        // Try to parse a certificate from remaining data
        size_t cert_offset = consumed + 48;
        if (cert_offset < size) {
            X509 *cert =  id_000004_parse_x509_from_data(data + cert_offset, size - cert_offset);
            if (cert) {
                SSL_CTX_use_certificate(ctx, cert);
                X509_free(cert);
            }
        }
    }
    
    // 7. EVP Digest Verify Init
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    EVP_PKEY_CTX *pkey_ctx = nullptr;
    
    if (md_ctx) {
        // Create a dummy key for verification
        EVP_PKEY *pkey = EVP_PKEY_new();
        if (pkey) {
            // Use RSA with minimal parameters
            RSA *rsa = RSA_new();
            if (rsa) {
                BIGNUM *e = BN_new();
                BN_set_word(e, RSA_F4);
                RSA_generate_key_ex(rsa, 512, e, nullptr);
                EVP_PKEY_assign_RSA(pkey, rsa);
                BN_free(e);
            }
            
            // Initialize digest verification
            EVP_DigestVerifyInit(md_ctx, &pkey_ctx, EVP_sha256(), nullptr, pkey);
            
            EVP_PKEY_free(pkey);
        }
    }
    
    // 8. Call OSSL_TARGET_it (just to use the API)
    const ASN1_ITEM *target_item = OSSL_TARGET_it();
    if (target_item) {
        // Just using the API, no action needed
    }
    
    // Cleanup
    if (ad) ACCESS_DESCRIPTION_free(ad);
    if (req) OCSP_REQUEST_free(req);
    if (p12) PKCS12_free(p12);
    if (ctx) SSL_CTX_free(ctx);
    if (md_ctx) EVP_MD_CTX_free(md_ctx);
    
    return 0;
}