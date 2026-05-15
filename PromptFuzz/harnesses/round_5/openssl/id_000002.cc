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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 6
//<Quality> {"density":12,"unique_branches":{"PKCS12_free":[[34,9,34,20,0,0,4,0],[34,24,34,46,0,0,4,0]],"int_ctx_new":[[122,13,122,28,0,0,4,0]],"PKCS12_mac_present":[[36,12,36,20,0,0,4,1]],"do_sigver_init":[[55,9,55,26,0,0,4,0]],"Camellia_set_key":[[25,9,25,20,0,0,4,1]]},"library_calls":["ERR_load_CRYPTO_strings","d2i_ACCESS_DESCRIPTION","OCSP_REQUEST_new","X509V3_EXT_nconf_nid","OCSP_REQUEST_add_ext","X509_EXTENSION_free","PKCS12_new","PKCS12_mac_present","Camellia_set_key","Camellia_cbc_encrypt","Camellia_cbc_encrypt","SSL_CTX_new","TLS_method","X509_new","X509_set_version","ASN1_INTEGER_set","X509_get_serialNumber","X509_gmtime_adj","X509_getm_notBefore","X509_gmtime_adj","X509_getm_notAfter","SSL_CTX_use_certificate","X509_free","EVP_MD_CTX_new","EVP_PKEY_new","EVP_DigestVerifyInit","EVP_sha256","EVP_PKEY_free","OSSL_TARGET_it","ACCESS_DESCRIPTION_free","OCSP_REQUEST_free","PKCS12_free","SSL_CTX_free","EVP_MD_CTX_free"],"critical_calls":["ERR_load_CRYPTO_strings","d2i_ACCESS_DESCRIPTION","OCSP_REQUEST_new","X509V3_EXT_nconf_nid","OCSP_REQUEST_add_ext","X509_EXTENSION_free","PKCS12_new","PKCS12_mac_present","Camellia_set_key","Camellia_cbc_encrypt","Camellia_cbc_encrypt","SSL_CTX_new","TLS_method","X509_new","X509_set_version","ASN1_INTEGER_set","X509_get_serialNumber","X509_gmtime_adj","X509_getm_notBefore","X509_gmtime_adj","X509_getm_notAfter","SSL_CTX_use_certificate","X509_free","EVP_MD_CTX_new","EVP_PKEY_new","EVP_DigestVerifyInit","EVP_sha256","EVP_PKEY_free","OSSL_TARGET_it","ACCESS_DESCRIPTION_free","OCSP_REQUEST_free","PKCS12_free","SSL_CTX_free","EVP_MD_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // 1. Load CRYPTO error strings
    ERR_load_CRYPTO_strings();

    // 2. Parse ACCESS_DESCRIPTION from input data
    const unsigned char *p = data;
    long remaining = static_cast<long>(size);
    ACCESS_DESCRIPTION *ad = d2i_ACCESS_DESCRIPTION(nullptr, &p, remaining);
    if (ad) {
        remaining -= (p - data);
    }

    // 3. Create OCSP request and add extension
    OCSP_REQUEST *ocsp_req = OCSP_REQUEST_new();
    if (ocsp_req && ad) {
        // Create a basic extension from ACCESS_DESCRIPTION
        // We'll use the ACCESS_DESCRIPTION as extension value (simplified)
        X509_EXTENSION *ext = X509V3_EXT_nconf_nid(nullptr, nullptr, NID_info_access, 
                                                  (char*)"OCSP;URI:http://example.com");
        if (ext) {
            OCSP_REQUEST_add_ext(ocsp_req, ext, -1);
            X509_EXTENSION_free(ext);
        }
    }

    // 4. Check PKCS12 MAC presence (create minimal PKCS12 structure)
    PKCS12 *p12 = PKCS12_new();
    int mac_present = 0;
    if (p12) {
        mac_present = PKCS12_mac_present(p12);
    }

    // 5. Camellia CBC encryption/decryption
    CAMELLIA_KEY key;
    unsigned char iv[CAMELLIA_BLOCK_SIZE];
    unsigned char *encrypted = nullptr;
    unsigned char *decrypted = nullptr;
    
    if (remaining >= 32) { // Need at least key material
        // Use part of input as key and IV
        Camellia_set_key(data, 128, &key);
        memcpy(iv, data + 16, CAMELLIA_BLOCK_SIZE);
        
        size_t data_len = remaining > 64 ? 64 : remaining;
        encrypted = (unsigned char*)malloc(data_len);
        decrypted = (unsigned char*)malloc(data_len);
        
        if (encrypted && decrypted) {
            // Encrypt
            Camellia_cbc_encrypt(data, encrypted, data_len, &key, iv, CAMELLIA_ENCRYPT);
            // Decrypt (reset IV)
            memcpy(iv, data + 16, CAMELLIA_BLOCK_SIZE);
            Camellia_cbc_encrypt(encrypted, decrypted, data_len, &key, iv, CAMELLIA_DECRYPT);
        }
    }

    // 6. SSL_CTX certificate usage
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    if (ctx) {
        // Create minimal X509 certificate from input data
        X509 *x509 = X509_new();
        if (x509) {
            // Set some basic fields (simplified)
            X509_set_version(x509, 2);
            ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
            X509_gmtime_adj(X509_getm_notBefore(x509), 0);
            X509_gmtime_adj(X509_getm_notAfter(x509), 31536000L);
            
            // Try to use certificate (may fail due to incomplete cert)
            SSL_CTX_use_certificate(ctx, x509);
            X509_free(x509);
        }
    }

    // 7. EVP Digest Verify (simplified example)
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    EVP_PKEY_CTX *pkey_ctx = nullptr;
    if (md_ctx) {
        // Create a minimal PKEY for testing
        EVP_PKEY *pkey = EVP_PKEY_new();
        if (pkey) {
            // Try to initialize verification (will likely fail but exercises API)
            EVP_DigestVerifyInit(md_ctx, &pkey_ctx, EVP_sha256(), nullptr, pkey);
            EVP_PKEY_free(pkey);
        }
    }

    // 8. Use OSSL_TARGET_it() (minimal usage)
    const ASN1_ITEM *target_item = OSSL_TARGET_it();
    (void)target_item; // Prevent unused variable warning

    // Cleanup
    if (ad) ACCESS_DESCRIPTION_free(ad);
    if (ocsp_req) OCSP_REQUEST_free(ocsp_req);
    if (p12) PKCS12_free(p12);
    if (encrypted) free(encrypted);
    if (decrypted) free(decrypted);
    if (ctx) SSL_CTX_free(ctx);
    if (md_ctx) EVP_MD_CTX_free(md_ctx);
    
    return 0;
}