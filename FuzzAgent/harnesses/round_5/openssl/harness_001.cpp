#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/asn1.h>
#include <openssl/crypto.h>

namespace harness_001 {
/*
 * Fuzzing harness for OpenSSL targeting X509 certificate parsing and printing operations
 * This harness tests: X509 certificate parsing, printing, CRL manipulation, ASN1 object parsing, and digest operations
 * Primary target: X509_print with 1425 undiscovered branches
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                        OPENSSL_INIT_ADD_ALL_CIPHERS | 
                        OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();

    if (size < 64) {
        return 0;  // Need minimum input size for meaningful X509 operations
    }

    FuzzedDataProvider fdp(data, size);

    // ==================== X509 CERTIFICATE PARSING AND PRINTING ====================
    // Primary target: X509_print with 1425 undiscovered branches
    
    X509* x509 = NULL;
    BIO* bio = NULL;
    X509_CRL* crl = NULL;
    EXTENDED_KEY_USAGE* eku = NULL;
    ASN1_OBJECT* asn1_obj = NULL;
    EVP_MD_CTX* md_ctx = NULL;

    // 1. Create or parse X509 certificate
    // First approach: Try to parse DER-encoded X509 certificate from input
    size_t cert_data_len = fdp.ConsumeIntegralInRange<size_t>(1, size / 2);
    std::vector<uint8_t> cert_data = fdp.ConsumeBytes<uint8_t>(cert_data_len);
    
    if (cert_data.size() > 0) {
        const unsigned char* p = cert_data.data();
        x509 = d2i_X509(NULL, &p, cert_data.size());
    }
    
    // If parsing failed, create a new X509 certificate
    if (x509 == NULL) {
        x509 = X509_new();
    }

    if (x509 == NULL) {
        return 0;  // Cannot proceed without X509 object
    }

    // 2. Create BIO for output
    bio = BIO_new(BIO_s_null());
    if (bio == NULL) {
        X509_free(x509);
        return 0;
    }

    // 3. Call X509_print with certificate and BIO (PRIMARY TARGET)
    X509_print(bio, x509);

    // 4. Optionally manipulate certificate with X509_CRL_set1_nextUpdate
    // First create a CRL if we have enough data
    size_t crl_data_len = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes() / 2);
    if (crl_data_len > 0) {
        std::vector<uint8_t> crl_data = fdp.ConsumeBytes<uint8_t>(crl_data_len);
        const unsigned char* crl_p = crl_data.data();
        crl = d2i_X509_CRL(NULL, &crl_p, crl_data.size());
        
        if (crl != NULL) {
            // Create an ASN1_TIME for next update
            ASN1_TIME* next_update = ASN1_TIME_new();
            if (next_update != NULL) {
                // Set a random time (year 2030-2050)
                int year = fdp.ConsumeIntegralInRange<int>(2030, 2050);
                int month = fdp.ConsumeIntegralInRange<int>(1, 12);
                int day = fdp.ConsumeIntegralInRange<int>(1, 28);
                
                ASN1_TIME_set(next_update, 0);  // Set to epoch
                // Note: X509_CRL_set1_nextUpdate expects ASN1_TIME*
                // We'll try to use it if available
                X509_CRL_set1_nextUpdate(crl, next_update);
                ASN1_TIME_free(next_update);
            }
        }
    }

    // 5. Parse ASN1 objects using d2i_ASN1_OBJECT and d2i_EXTENDED_KEY_USAGE
    // Parse ASN1_OBJECT
    size_t asn1_data_len = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes() / 2);
    if (asn1_data_len > 0) {
        std::vector<uint8_t> asn1_data = fdp.ConsumeBytes<uint8_t>(asn1_data_len);
        const unsigned char* asn1_p = asn1_data.data();
        asn1_obj = d2i_ASN1_OBJECT(NULL, &asn1_p, asn1_data.size());
    }

    // Parse EXTENDED_KEY_USAGE
    size_t eku_data_len = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes() / 2);
    if (eku_data_len > 0) {
        std::vector<uint8_t> eku_data = fdp.ConsumeBytes<uint8_t>(eku_data_len);
        const unsigned char* eku_p = eku_data.data();
        eku = d2i_EXTENDED_KEY_USAGE(NULL, &eku_p, eku_data.size());
    }

    // 6. Perform digest operations with EVP_DigestFinal
    // Create a digest context and test EVP_DigestFinal
    md_ctx = EVP_MD_CTX_new();
    if (md_ctx != NULL) {
        // Choose a digest algorithm based on input
        int digest_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
        const EVP_MD* md = NULL;
        
        switch (digest_choice) {
            case 0: md = EVP_sha256(); break;
            case 1: md = EVP_sha1(); break;
            case 2: md = EVP_md5(); break;
            case 3: md = EVP_sha512(); break;
        }
        
        if (md != NULL) {
            // Consume some data for hashing
            size_t hash_data_len = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
            std::vector<uint8_t> hash_data = fdp.ConsumeBytes<uint8_t>(hash_data_len);
            
            unsigned char hash_value[EVP_MAX_MD_SIZE];
            unsigned int hash_len;
            
            if (EVP_DigestInit_ex(md_ctx, md, NULL)) {
                EVP_DigestUpdate(md_ctx, hash_data.data(), hash_data.size());
                EVP_DigestFinal_ex(md_ctx, hash_value, &hash_len);
            }
        }
    }

    // ==================== CLEANUP ====================
    // Clean up all resources in reverse order of creation
    if (md_ctx != NULL) {
        EVP_MD_CTX_free(md_ctx);
    }
    
    if (eku != NULL) {
        EXTENDED_KEY_USAGE_free(eku);
    }
    
    if (asn1_obj != NULL) {
        ASN1_OBJECT_free(asn1_obj);
    }
    
    if (crl != NULL) {
        X509_CRL_free(crl);
    }
    
    if (bio != NULL) {
        BIO_free(bio);
    }
    
    if (x509 != NULL) {
        X509_free(x509);
    }

    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_001
