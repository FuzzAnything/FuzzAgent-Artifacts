#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/dsa.h>
#include <openssl/ec.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace harness_003 {
/*
 * Fuzzing harness for OpenSSL PEM/X509 certificate handling infrastructure
 * Targets: PEM_read_bio_PrivateKey (1597+ undiscovered branches), X509 certificate parsing,
 *          certificate loading, certificate chain validation, and related operations
 * Specifically targets PEM format parsing, X509 certificate handling, private key loading
 */

// Initialize OpenSSL once
static bool openssl_initialized = false;

static void initialize_openssl() {
    if (!openssl_initialized) {
        OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                           OPENSSL_INIT_ADD_ALL_CIPHERS |
                           OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
        ERR_clear_error();
        openssl_initialized = true;
    }
}

// Simple password callback that either returns no password or uses fuzzer input
static int pem_password_callback(char *buf, int size, int rwflag, void *userdata) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(userdata);
    if (fdp == nullptr || fdp->remaining_bytes() == 0) {
        return -1; // No password available
    }
    
    // Sometimes return no password, sometimes provide one
    if (fdp->ConsumeBool()) {
        return -1;
    }
    
    // Provide a password from fuzzer input
    std::string password = fdp->ConsumeRandomLengthString(32);
    if (password.length() > static_cast<size_t>(size - 1)) {
        password = password.substr(0, size - 1);
    }
    memcpy(buf, password.c_str(), password.length());
    buf[password.length()] = '\0';
    return static_cast<int>(password.length());
}

// Test PEM private key parsing with various key types
static void test_pem_private_key_parsing(FuzzedDataProvider& fdp, const std::vector<uint8_t>& pem_data) {
    if (pem_data.empty()) {
        return;
    }
    
    BIO* bio = BIO_new_mem_buf(pem_data.data(), pem_data.size());
    if (bio == NULL) {
        return;
    }
    
    // Clear any previous errors
    ERR_clear_error();
    
    // Test PEM_read_bio_PrivateKey with different parameters
    EVP_PKEY* pkey = NULL;
    
    // Try with password callback
    pkey = PEM_read_bio_PrivateKey(bio, NULL, pem_password_callback, &fdp);
    if (pkey != NULL) {
        // Test key properties if we got a valid key
        int key_type = EVP_PKEY_get_id(pkey);
        
        // Try to get key size
        int key_bits = EVP_PKEY_bits(pkey);
        (void)key_bits; // Use result to avoid unused variable warning
        
        // Test key operations based on type
        switch (key_type) {
            case EVP_PKEY_RSA:
            case EVP_PKEY_RSA_PSS: {
                const RSA* rsa = EVP_PKEY_get0_RSA(pkey);
                if (rsa != NULL) {
                    // Test RSA operations
                    const BIGNUM* n = RSA_get0_n(rsa);
                    const BIGNUM* e = RSA_get0_e(rsa);
                    (void)n; (void)e; // Use to avoid warnings
                }
                break;
            }
            case EVP_PKEY_DSA: {
                const DSA* dsa = EVP_PKEY_get0_DSA(pkey);
                if (dsa != NULL) {
                    // Test DSA operations
                    const BIGNUM* p = DSA_get0_p(dsa);
                    const BIGNUM* q = DSA_get0_q(dsa);
                    (void)p; (void)q; // Use to avoid warnings
                }
                break;
            }
            case EVP_PKEY_EC: {
                const EC_KEY* ec = EVP_PKEY_get0_EC_KEY(pkey);
                if (ec != NULL) {
                    // Test EC operations
                    const EC_GROUP* group = EC_KEY_get0_group(ec);
                    (void)group; // Use to avoid warnings
                }
                break;
            }
        }
        
        EVP_PKEY_free(pkey);
        pkey = NULL;
    }
    
    // Reset BIO for next test
    BIO_free(bio);
    bio = BIO_new_mem_buf(pem_data.data(), pem_data.size());
    if (bio == NULL) {
        return;
    }
    
    // Test PEM_read_bio_PrivateKey with NULL password callback
    pkey = PEM_read_bio_PrivateKey(bio, NULL, NULL, NULL);
    if (pkey != NULL) {
        EVP_PKEY_free(pkey);
    }
    
    BIO_free(bio);
}

// Test X509 certificate parsing
static void test_x509_certificate_parsing(FuzzedDataProvider& fdp, const std::vector<uint8_t>& cert_data) {
    if (cert_data.empty()) {
        return;
    }
    
    BIO* bio = BIO_new_mem_buf(cert_data.data(), cert_data.size());
    if (bio == NULL) {
        return;
    }
    
    // Clear any previous errors
    ERR_clear_error();
    
    // Test PEM certificate parsing
    X509* x509 = PEM_read_bio_X509(bio, NULL, NULL, NULL);
    if (x509 != NULL) {
        // Exercise various X509 functions
        X509_check_purpose(x509, -1, -1);
        
        // Get certificate properties
        EVP_PKEY* pubkey = X509_get_pubkey(x509);
        if (pubkey != NULL) {
            EVP_PKEY_free(pubkey);
        }
        // Get subject and issuer
        const X509_NAME* subject = X509_get_subject_name(x509);
        const X509_NAME* issuer = X509_get_issuer_name(x509);

        
        // Get serial number
        const ASN1_INTEGER* serial = X509_get0_serialNumber(x509);
        (void)serial; // Use to avoid warnings
        
        // Get validity period
        const ASN1_TIME* not_before = X509_get0_notBefore(x509);
        const ASN1_TIME* not_after = X509_get0_notAfter(x509);
        (void)not_before; (void)not_after; // Use to avoid warnings
        
        X509_free(x509);
    }
    
    // Reset BIO for DER parsing
    BIO_free(bio);
    bio = BIO_new_mem_buf(cert_data.data(), cert_data.size());
    if (bio == NULL) {
        return;
    }
    
    // Test DER certificate parsing
    const unsigned char* der_data = cert_data.data();
    x509 = d2i_X509_bio(bio, NULL);
    if (x509 != NULL) {
        X509_free(x509);
    }
    
    BIO_free(bio);
}

// Test certificate chain and store operations
static void test_certificate_chain_operations(FuzzedDataProvider& fdp, 
                                              const std::vector<uint8_t>& cert_data1,
                                              const std::vector<uint8_t>& cert_data2) {
    if (cert_data1.empty()) {
        return;
    }
    
    // Parse first certificate
    BIO* bio1 = BIO_new_mem_buf(cert_data1.data(), cert_data1.size());
    if (bio1 == NULL) {
        return;
    }
    
    X509* cert1 = PEM_read_bio_X509(bio1, NULL, NULL, NULL);
    BIO_free(bio1);
    
    if (cert1 == NULL) {
        return;
    }
    
    // Create X509 store and add certificate
    X509_STORE* store = X509_STORE_new();
    if (store != NULL) {
        X509_STORE_add_cert(store, cert1);
        
        // Create store context
        X509_STORE_CTX* ctx = X509_STORE_CTX_new();
        if (ctx != NULL) {
            // Initialize store context
            X509_STORE_CTX_init(ctx, store, cert1, NULL);
            
            // Try verification (will likely fail but exercises code)
            X509_verify_cert(ctx);
            
            X509_STORE_CTX_free(ctx);
        }
        
        X509_STORE_free(store);
    }
    
    // If we have a second certificate, try chain operations
    if (!cert_data2.empty()) {
        BIO* bio2 = BIO_new_mem_buf(cert_data2.data(), cert_data2.size());
        if (bio2 != NULL) {
            X509* cert2 = PEM_read_bio_X509(bio2, NULL, NULL, NULL);
            BIO_free(bio2);
            
            if (cert2 != NULL) {
                // Create a stack of certificates
                STACK_OF(X509)* chain = sk_X509_new_null();
                if (chain != NULL) {
                    sk_X509_push(chain, cert1);
                    sk_X509_push(chain, cert2);
                    
                    // Test store with chain
                    store = X509_STORE_new();
                    if (store != NULL) {
                        X509_STORE_add_cert(store, cert1);
                        
                        X509_STORE_CTX* ctx = X509_STORE_CTX_new();
                        if (ctx != NULL) {
                            X509_STORE_CTX_init(ctx, store, cert1, chain);
                            X509_verify_cert(ctx);
                            X509_STORE_CTX_free(ctx);
                        }
                        
                        X509_STORE_free(store);
                    }
                    
                    sk_X509_free(chain);
                }
                
                X509_free(cert2);
            }
        }
    }
    
    X509_free(cert1);
}

// Test PKCS8 private key parsing
static void test_pkcs8_parsing(FuzzedDataProvider& fdp, const std::vector<uint8_t>& pkcs8_data) {
    if (pkcs8_data.empty()) {
        return;
    }
    
    BIO* bio = BIO_new_mem_buf(pkcs8_data.data(), pkcs8_data.size());
    if (bio == NULL) {
        return;
    }
    
    // Clear any previous errors
    ERR_clear_error();
    
    // Test PKCS8 parsing
    PKCS8_PRIV_KEY_INFO* p8inf = PEM_read_bio_PKCS8_PRIV_KEY_INFO(bio, NULL, NULL, NULL);
    if (p8inf != NULL) {
        // Convert to EVP_PKEY
        EVP_PKEY* pkey = EVP_PKCS82PKEY(p8inf);
        if (pkey != NULL) {
            EVP_PKEY_free(pkey);
        }
        PKCS8_PRIV_KEY_INFO_free(p8inf);
    }
    
    BIO_free(bio);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0;  // Too small for meaningful PEM/X509 testing
    }
    
    initialize_openssl();
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data for test configuration
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool use_password = fdp.ConsumeBool();
    size_t max_pem_size = fdp.ConsumeIntegralInRange<size_t>(128, 4096);
    
    // Fix: Ensure max_pem_size is at least 64 before using as upper bound
    if (max_pem_size < 64) max_pem_size = 64;
    
    // Split input into multiple parts for different tests
    size_t pem_data_size = fdp.ConsumeIntegralInRange<size_t>(64, max_pem_size);
    // Fix: Ensure pem_data_size doesn't exceed remaining bytes
    if (pem_data_size > fdp.remaining_bytes()) pem_data_size = fdp.remaining_bytes();
    std::vector<uint8_t> pem_data = fdp.ConsumeBytes<uint8_t>(pem_data_size);
    
    // Fix: Ensure we don't underflow when calculating cert_data_size
    size_t remaining_after_pem = size - pem_data_size;
    size_t max_cert_size = (remaining_after_pem > 64) ? remaining_after_pem : 64;
    size_t cert_data_size = fdp.ConsumeIntegralInRange<size_t>(64, max_cert_size);
    // Fix: Ensure cert_data_size doesn't exceed remaining bytes
    if (cert_data_size > fdp.remaining_bytes()) cert_data_size = fdp.remaining_bytes();
    std::vector<uint8_t> cert_data = fdp.ConsumeBytes<uint8_t>(cert_data_size);
    
    size_t cert2_data_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
    std::vector<uint8_t> cert2_data = fdp.ConsumeBytes<uint8_t>(cert2_data_size);
    
    size_t pkcs8_data_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
    std::vector<uint8_t> pkcs8_data = fdp.ConsumeBytes<uint8_t>(pkcs8_data_size);
    
    // Clear any previous errors before starting tests
    ERR_clear_error();
    
    // Select test type based on fuzzer input

    switch (test_type) {
        case 0:
            // Primary target: PEM_read_bio_PrivateKey
            test_pem_private_key_parsing(fdp, pem_data);
            break;
            
        case 1:
            // X509 certificate parsing
            test_x509_certificate_parsing(fdp, cert_data);
            break;
            
        case 2:
            // Certificate chain operations
            test_certificate_chain_operations(fdp, cert_data, cert2_data);
            break;
            
        case 3:
            // PKCS8 parsing
            test_pkcs8_parsing(fdp, pkcs8_data);
            break;
            
        case 4:
            // Combined test: private key then certificate
            test_pem_private_key_parsing(fdp, pem_data);
            test_x509_certificate_parsing(fdp, cert_data);
            break;
            
        case 5:
            // Test with password callback
            test_pem_private_key_parsing(fdp, pem_data);
            break;
            
        case 6:
            // Multiple certificate parsing
            test_x509_certificate_parsing(fdp, cert_data);
            if (!cert2_data.empty()) {
                test_x509_certificate_parsing(fdp, cert2_data);
            }
            break;
            
        case 7:
            // All operations
            test_pem_private_key_parsing(fdp, pem_data);
            test_x509_certificate_parsing(fdp, cert_data);
            test_certificate_chain_operations(fdp, cert_data, cert2_data);
            test_pkcs8_parsing(fdp, pkcs8_data);
            break;
    }
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_003
