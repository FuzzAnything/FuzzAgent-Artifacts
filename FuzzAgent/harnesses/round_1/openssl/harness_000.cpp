#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/dsa.h>
#include <openssl/ec.h>
#include <openssl/dh.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rc4.h>
#include <openssl/bn.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace harness_000 {
/*
 * Fuzzing harness for OpenSSL core SSL/TLS and cryptographic operations
 * Targets: SSL handshake functions, certificate parsing, encryption/decryption
 */


// Initialize OpenSSL once
static bool openssl_initialized = false;

static void initialize_openssl() {
    if (!openssl_initialized) {
        OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                           OPENSSL_INIT_ADD_ALL_CIPHERS |
                           OPENSSL_INIT_ADD_ALL_DIGESTS |
                           OPENSSL_INIT_ASYNC, NULL);
        OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS, NULL);
        ERR_clear_error();
        openssl_initialized = true;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Too small for meaningful testing
    }
    
    initialize_openssl();
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    uint32_t flags = fdp.ConsumeIntegral<uint32_t>();
    int key_size = fdp.ConsumeIntegralInRange<int>(64, 4096);
    
    // Consume variable-size data
    std::vector<uint8_t> cert_data = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(0, size/2));
    std::vector<uint8_t> ssl_data = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(0, size/2));
    std::vector<uint8_t> crypto_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Clear any previous errors
    ERR_clear_error();
    
    switch (test_type) {
        case 0: {
            // Test X509 certificate parsing
            if (!cert_data.empty()) {
                const unsigned char* p = cert_data.data();
                X509* x509 = d2i_X509(NULL, &p, cert_data.size());
                if (x509 != NULL) {
                    // Exercise various X509 functions
                    X509_free(x509);
                }
                
                // Try PEM parsing
                BIO* bio = BIO_new_mem_buf(cert_data.data(), cert_data.size());
                if (bio != NULL) {
                    X509* pem_x509 = PEM_read_bio_X509(bio, NULL, NULL, NULL);
                    if (pem_x509 != NULL) {
                        X509_free(pem_x509);
                    }
                    BIO_free(bio);
                }
            }
            break;
        }
        
        case 1: {
            // Test SSL/TLS client operations
            if (!ssl_data.empty()) {
                SSL_CTX* ctx = SSL_CTX_new(TLS_method());
                if (ctx != NULL) {
                    SSL* ssl = SSL_new(ctx);
                    if (ssl != NULL) {
                        BIO* in = BIO_new(BIO_s_mem());
                        BIO* out = BIO_new(BIO_s_mem());
                        
                        if (in != NULL && out != NULL) {
                            // SSL_set_bio transfers ownership of BIOs to SSL
                            SSL_set_bio(ssl, in, out);
                            SSL_set_connect_state(ssl);
                            
                            // Write fuzzer data to input BIO
                            BIO_write(in, ssl_data.data(), ssl_data.size());
                            
                            // Try handshake (may fail, that's OK)
                            SSL_do_handshake(ssl);
                            
                            // Try reading (may fail, that's OK)
                            uint8_t buf[1024];
                            SSL_read(ssl, buf, sizeof(buf));
                            
                            // SSL_free will free the BIOs that were transferred to SSL
                        } else {
                            // Clean up any BIOs that were created but not transferred to SSL
                            if (in != NULL) BIO_free(in);
                            if (out != NULL) BIO_free(out);
                        }
                        
                        SSL_free(ssl);
                    }
                    SSL_CTX_free(ctx);
                }
            }
            break;
        }
        
        case 2: {
            // Test hash functions
            if (!crypto_data.empty()) {
                EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
                if (mdctx != NULL) {
                    // Test different hash algorithms
                    const EVP_MD* md = NULL;
                    uint8_t hash_type = fdp.ConsumeIntegral<uint8_t>() % 6;
                    
                    switch (hash_type) {
                        case 0: md = EVP_md5(); break;
                        case 1: md = EVP_sha1(); break;
                        case 2: md = EVP_sha256(); break;
                        case 3: md = EVP_sha384(); break;
                        case 4: md = EVP_sha512(); break;
                        default: md = EVP_sha224(); break;
                    }
                    
                    if (md != NULL) {
                        EVP_DigestInit_ex(mdctx, md, NULL);
                        EVP_DigestUpdate(mdctx, crypto_data.data(), crypto_data.size());
                        unsigned char md_value[EVP_MAX_MD_SIZE];
                        unsigned int md_len;
                        EVP_DigestFinal_ex(mdctx, md_value, &md_len);
                    }
                    
                    EVP_MD_CTX_free(mdctx);
                }
            }
            break;
        }
        
        case 3: {
            // Test symmetric encryption
            if (!crypto_data.empty() && crypto_data.size() >= 32) {
                EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
                if (ctx != NULL) {
                    // Use first 16 bytes as key, next 16 as IV
                    unsigned char key[16], iv[16];
                    memcpy(key, crypto_data.data(), 16);
                    memcpy(iv, crypto_data.data() + 16, 16);
                    
                    const EVP_CIPHER* cipher = NULL;
                    uint8_t cipher_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                    
                    switch (cipher_type) {
                        case 0: cipher = EVP_aes_128_cbc(); break;
                        case 1: cipher = EVP_aes_128_ecb(); break;
                        case 2: cipher = EVP_aes_128_ctr(); break;
                    }
                    
                    if (cipher != NULL) {
                        // Try encryption
                        EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv);
                        
                        size_t data_len = crypto_data.size() - 32;
                        if (data_len > 0) {
                            unsigned char* out_buf = (unsigned char*)malloc(data_len + EVP_MAX_BLOCK_LENGTH);
                            if (out_buf != NULL) {
                                int out_len;
                                EVP_EncryptUpdate(ctx, out_buf, &out_len, 
                                                 crypto_data.data() + 32, data_len);
                                free(out_buf);
                            }
                        }
                    }
                    
                    EVP_CIPHER_CTX_free(ctx);
                }
            }
            break;
        }
        
        case 4: {
            // Test HMAC
            if (!crypto_data.empty() && crypto_data.size() >= 32) {
                // Use first 16 bytes as key
                unsigned char key[16];
                memcpy(key, crypto_data.data(), 16);
                
                const EVP_MD* md = EVP_sha256();
                unsigned char hmac_value[EVP_MAX_MD_SIZE];
                unsigned int hmac_len;
                
                HMAC(md, key, 16, crypto_data.data() + 16, crypto_data.size() - 16,
                     hmac_value, &hmac_len);
            }
            break;
        }
        
        case 5: {
            // Test BIO operations
            if (!crypto_data.empty()) {
                BIO* bio = BIO_new(BIO_s_mem());
                if (bio != NULL) {
                    BIO_write(bio, crypto_data.data(), crypto_data.size());
                    
                    // Try reading back
                    uint8_t read_buf[1024];
                    BIO_read(bio, read_buf, sizeof(read_buf));
                    
                    BIO_free(bio);
                }
                
                // Test file BIO (null device)
                BIO* null_bio = BIO_new(BIO_s_null());
                if (null_bio != NULL) {
                    BIO_write(null_bio, crypto_data.data(), crypto_data.size());
                    BIO_free(null_bio);
                }
            }
            break;
        }
        
        case 6: {
            // Test random number generation
            if (!crypto_data.empty()) {
                uint8_t rand_buf[256];
                RAND_bytes(rand_buf, sizeof(rand_buf));
                
                // Seed RNG with fuzzer data
                RAND_seed(crypto_data.data(), crypto_data.size());
                RAND_bytes(rand_buf, sizeof(rand_buf));
            }
            break;
        }
        
        case 7: {
            // Test ASN.1 parsing (via X509) - simpler approach
            if (!cert_data.empty()) {
                BIO* bio = BIO_new_mem_buf(cert_data.data(), cert_data.size());
                if (bio != NULL) {
                    // Try to parse as X509 certificate
                    X509* x509 = d2i_X509_bio(bio, NULL);
                    if (x509 != NULL) {
                        X509_free(x509);
                    }
                    BIO_free(bio);
                }
            }
            break;
        }
    }
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_000
