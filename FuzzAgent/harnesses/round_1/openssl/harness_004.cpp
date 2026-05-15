#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/pkcs12.h>
#include <openssl/x509.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/dsa.h>
#include <openssl/ec.h>
#include <openssl/rand.h>
#include <openssl/asn1.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace harness_004 {
/*
 * Fuzzing harness for OpenSSL PKCS12 operations
 * Targets: PKCS12 parsing, key derivation, encryption/decryption of PKCS12 containers
 * Specifically targets PKCS12 operations with 1000+ undiscovered branches
 * Focus on symmetric encryption, hash functions, and ASN.1 parsing through PKCS12 operations
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

// Password callback that uses fuzzer input
static int pkcs12_password_callback(char *buf, int size, int rwflag, void *userdata) {
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

// Test PKCS12 parsing with different passwords and operations
static void test_pkcs12_parsing(FuzzedDataProvider& fdp, const std::vector<uint8_t>& pkcs12_data) {
    if (pkcs12_data.empty()) {
        return;
    }
    
    BIO* bio = BIO_new_mem_buf(pkcs12_data.data(), pkcs12_data.size());
    if (bio == NULL) {
        return;
    }
    
    // Clear any previous errors
    ERR_clear_error();
    
    // Parse PKCS12 structure
    PKCS12* p12 = d2i_PKCS12_bio(bio, NULL);
    BIO_free(bio);
    
    if (p12 == NULL) {
        return;
    }
    
    // Test various PKCS12 operations
    
    // 1. Test MAC verification with different passwords
    if (PKCS12_mac_present(p12)) {
        // Test with NULL password
        PKCS12_verify_mac(p12, NULL, 0);
        
        // Test with empty password
        PKCS12_verify_mac(p12, "", 0);
        
        // Test with fuzzer-provided password
        if (fdp.remaining_bytes() > 0) {
            std::string password = fdp.ConsumeRandomLengthString(64);
            PKCS12_verify_mac(p12, password.c_str(), password.length());
        }
    }
    
    // 2. Test parsing with different passwords
    EVP_PKEY* pkey = NULL;
    X509* cert = NULL;
    STACK_OF(X509)* ca = NULL;
    
    // Parse with NULL password
    PKCS12_parse(p12, NULL, &pkey, &cert, &ca);
    EVP_PKEY_free(pkey);
    X509_free(cert);
    OSSL_STACK_OF_X509_free(ca);
    
    // Parse with empty password
    pkey = NULL;
    cert = NULL;
    ca = NULL;
    PKCS12_parse(p12, "", &pkey, &cert, &ca);
    EVP_PKEY_free(pkey);
    X509_free(cert);
    OSSL_STACK_OF_X509_free(ca);
    
    // Parse with fuzzer-provided password
    if (fdp.remaining_bytes() > 0) {
        std::string password = fdp.ConsumeRandomLengthString(64);
        pkey = NULL;
        cert = NULL;
        ca = NULL;
        PKCS12_parse(p12, password.c_str(), &pkey, &cert, &ca);
        EVP_PKEY_free(pkey);
        X509_free(cert);
        OSSL_STACK_OF_X509_free(ca);
    }
    
    // 3. Test PKCS12 creation and manipulation
    if (fdp.remaining_bytes() > 100) {
        // Create a new PKCS12 structure
        PKCS12* new_p12 = PKCS12_init(NID_pkcs7_data);
        if (new_p12 != NULL) {
            // Generate some random key material
            size_t key_size = fdp.ConsumeIntegralInRange<size_t>(16, 256);
            std::vector<uint8_t> key_data = fdp.ConsumeBytes<uint8_t>(key_size);
            
            // Create a simple RSA key for testing
            EVP_PKEY* test_key = EVP_RSA_gen(2048);
            if (test_key != NULL) {
                // Create a self-signed certificate
                X509* test_cert = X509_new();
                if (test_cert != NULL) {
                    // Set basic certificate fields
                    ASN1_INTEGER_set(X509_get_serialNumber(test_cert), 1);
                    X509_gmtime_adj(X509_get_notBefore(test_cert), 0);
                    X509_gmtime_adj(X509_get_notAfter(test_cert), 31536000L); // 1 year
                    X509_set_pubkey(test_cert, test_key);
                    
                    // Create PKCS12 with test key and certificate
                    std::string pass = fdp.ConsumeRandomLengthString(32);
                    STACK_OF(X509)* certs = sk_X509_new_null();
                    if (certs != NULL) {
                        PKCS12* created_p12 = PKCS12_create(
                            pass.c_str(), "Test Certificate", test_key, test_cert, certs,
                            NID_pbe_WithSHA1And3_Key_TripleDES_CBC,
                            NID_pbe_WithSHA1And3_Key_TripleDES_CBC,
                            1, 100, 0);
                        
                        if (created_p12 != NULL) {
                            // Test operations on created PKCS12
                            PKCS12_verify_mac(created_p12, pass.c_str(), pass.length());
                            
                            // Try to parse it back
                            EVP_PKEY* parsed_key = NULL;
                            X509* parsed_cert = NULL;
                            STACK_OF(X509)* parsed_ca = NULL;
                            PKCS12_parse(created_p12, pass.c_str(), &parsed_key, &parsed_cert, &parsed_ca);
                            EVP_PKEY_free(parsed_key);
                            X509_free(parsed_cert);
                            OSSL_STACK_OF_X509_free(parsed_ca);
                            
                            PKCS12_free(created_p12);
                        }
                        sk_X509_free(certs);
                    }
                    X509_free(test_cert);
                }
                EVP_PKEY_free(test_key);
            }
            
            PKCS12_free(new_p12);
        }
    }
    
    // 4. Test PKCS12 attribute operations
    if (PKCS12_mac_present(p12)) {
        const ASN1_OCTET_STRING* mac = NULL;
        const X509_ALGOR* macalgid = NULL;
        const ASN1_OCTET_STRING* salt = NULL;
        const ASN1_INTEGER* maciter = NULL;
        
        PKCS12_get0_mac(&mac, &macalgid, &salt, &maciter, p12);
        
        // Test MAC generation with different parameters
        if (fdp.remaining_bytes() > 50) {
            std::string mac_pass = fdp.ConsumeRandomLengthString(32);
            unsigned char mac_buf[EVP_MAX_MD_SIZE];
            unsigned int mac_len;
            
            // Test PKCS12_gen_mac
            PKCS12_gen_mac(p12, mac_pass.c_str(), mac_pass.length(), mac_buf, &mac_len);
            
            // Test PKCS12_set_mac
            size_t salt_len = fdp.ConsumeIntegralInRange<size_t>(8, 64);
            std::vector<uint8_t> salt_data = fdp.ConsumeBytes<uint8_t>(salt_len);
            int iter = fdp.ConsumeIntegralInRange<int>(1, 10000);
            const EVP_MD* md = EVP_sha256();
            
            PKCS12_set_mac(p12, mac_pass.c_str(), mac_pass.length(), 
                          salt_data.empty() ? NULL : salt_data.data(), 
                          salt_data.size(), iter, md);
        }
    }
    
    // 5. Test PKCS12 key derivation functions
    if (fdp.remaining_bytes() > 100) {
        std::string pass = fdp.ConsumeRandomLengthString(32);
        size_t salt_len = fdp.ConsumeIntegralInRange<size_t>(8, 64);
        std::vector<uint8_t> salt = fdp.ConsumeBytes<uint8_t>(salt_len);
        int iter = fdp.ConsumeIntegralInRange<int>(1, 10000);
        int keylen = fdp.ConsumeIntegralInRange<int>(16, 64);
        int id = fdp.ConsumeIntegralInRange<int>(1, 3); // PKCS12_KEY_ID, PKCS12_IV_ID, PKCS12_MAC_ID
        
        std::vector<unsigned char> key(keylen);
        
        // Test different key generation functions
        PKCS12_key_gen_asc(pass.c_str(), pass.length(), 
                          salt.empty() ? NULL : salt.data(), salt.size(),
                          id, iter, keylen, key.data(), EVP_sha256());
        
        PKCS12_key_gen_utf8(pass.c_str(), pass.length(),
                           salt.empty() ? NULL : salt.data(), salt.size(),
                           id, iter, keylen, key.data(), EVP_sha256());
    }
    
    // 6. Test PKCS12 PBE operations
    if (fdp.remaining_bytes() > 50) {
        std::string pass = fdp.ConsumeRandomLengthString(32);
        size_t data_len = fdp.ConsumeIntegralInRange<size_t>(16, 256);
        std::vector<uint8_t> data = fdp.ConsumeBytes<uint8_t>(data_len);
        
        // Create a simple PBE algorithm structure
        X509_ALGOR* algor = X509_ALGOR_new();
        if (algor != NULL && !data.empty()) {
            // Test PKCS12_pbe_crypt
            unsigned char* out = NULL;
            int outlen;
            unsigned char* encrypted = PKCS12_pbe_crypt(algor, pass.c_str(), pass.length(),
                                                       data.data(), data.size(), &out, &outlen, 1);
            if (encrypted != NULL) {
                // Try to decrypt it back
                unsigned char* decrypted_out = NULL;
                int decrypted_len;
                unsigned char* decrypted = PKCS12_pbe_crypt(algor, pass.c_str(), pass.length(),
                                                           encrypted, outlen, &decrypted_out, &decrypted_len, 0);
                OPENSSL_free(encrypted);
                OPENSSL_free(decrypted_out);
            }
            X509_ALGOR_free(algor);

        }
    }
    
    PKCS12_free(p12);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Too small for meaningful PKCS12 testing
    }
    
    initialize_openssl();
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume variable-size PKCS12 data
    size_t pkcs12_data_size = fdp.ConsumeIntegralInRange<size_t>(0, size - 16);
    std::vector<uint8_t> pkcs12_data = fdp.ConsumeBytes<uint8_t>(pkcs12_data_size);
    
    // Clear any previous errors
    ERR_clear_error();
    
    switch (test_type) {
        case 0: {
            // Test basic PKCS12 parsing and operations
            test_pkcs12_parsing(fdp, pkcs12_data);
            break;
        }
        
        case 1: {
            // Test PKCS12 creation and manipulation
            if (fdp.remaining_bytes() > 100) {
                // Try to create a PKCS12 from scratch
                std::string password = fdp.ConsumeRandomLengthString(32);
                std::string friendly_name = fdp.ConsumeRandomLengthString(64);
                
                // Generate a simple RSA key
                EVP_PKEY* pkey = EVP_RSA_gen(1024); // Small key for speed
                if (pkey != NULL) {
                    // Create a minimal certificate
                    X509* cert = X509_new();
                    if (cert != NULL) {
                        // Set basic fields
                        ASN1_INTEGER_set(X509_get_serialNumber(cert), 1);
                        X509_gmtime_adj(X509_get_notBefore(cert), 0);
                        X509_gmtime_adj(X509_get_notAfter(cert), 31536000L);
                        X509_set_pubkey(cert, pkey);
                        
                        // Create empty CA stack
                        STACK_OF(X509)* ca = sk_X509_new_null();
                        
                        // Create PKCS12
                        PKCS12* p12 = PKCS12_create(
                            password.c_str(), friendly_name.c_str(), pkey, cert, ca,
                            NID_pbe_WithSHA1And3_Key_TripleDES_CBC,
                            NID_pbe_WithSHA1And3_Key_TripleDES_CBC,
                            1, 100, 0);
                        
                        if (p12 != NULL) {
                            // Test operations on created PKCS12
                            test_pkcs12_parsing(fdp, pkcs12_data); // Reuse parsing test
                            PKCS12_free(p12);
                        }
                        
                        sk_X509_free(ca);
                        X509_free(cert);
                    }
                    EVP_PKEY_free(pkey);
                }
            }
            break;
        }
        
        case 2: {
            // Test PKCS12 encryption/decryption operations
            if (!pkcs12_data.empty()) {
                BIO* bio = BIO_new_mem_buf(pkcs12_data.data(), pkcs12_data.size());
                if (bio != NULL) {
                    PKCS12* p12 = d2i_PKCS12_bio(bio, NULL);
                    BIO_free(bio);
                    
                    if (p12 != NULL) {
                        // Test password change operation
                        if (fdp.remaining_bytes() > 64) {
                            std::string old_pass = fdp.ConsumeRandomLengthString(32);
                            std::string new_pass = fdp.ConsumeRandomLengthString(32);
                            
                            // Try to change password (may fail if MAC doesn't verify)
                            PKCS12_newpass(p12, old_pass.c_str(), new_pass.c_str());
                        }
                        
                        PKCS12_free(p12);
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Test PKCS12 key derivation and PBE operations
            if (fdp.remaining_bytes() > 128) {
                // Test various PBE algorithms
                std::string pass = fdp.ConsumeRandomLengthString(32);
                size_t salt_len = fdp.ConsumeIntegralInRange<size_t>(8, 64);
                std::vector<uint8_t> salt = fdp.ConsumeBytes<uint8_t>(salt_len);
                int iter = fdp.ConsumeIntegralInRange<int>(1, 1000);
                
                // Test with different hash algorithms
                const EVP_MD* md = NULL;
                switch (fdp.ConsumeIntegral<uint8_t>() % 5) {
                    case 0: md = EVP_sha1(); break;
                    case 1: md = EVP_sha256(); break;
                    case 2: md = EVP_sha384(); break;
                    case 3: md = EVP_sha512(); break;
                    case 4: md = EVP_md5(); break;
                }
                
                if (md != NULL) {
                    // Create encryption context
                    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
                    if (ctx != NULL) {
                        // Test PKCS12 PBE key initialization
                        const EVP_CIPHER* cipher = EVP_aes_128_cbc();
                        int en_de = fdp.ConsumeBool() ? 1 : 0; // 0 for decrypt, 1 for encrypt
                        PKCS12_PBE_keyivgen(ctx, pass.c_str(), pass.length(),
                                           NULL, cipher, md, en_de);
                        
                        EVP_CIPHER_CTX_free(ctx);
                    }
                }
            }
            break;
        }
    }
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_004
