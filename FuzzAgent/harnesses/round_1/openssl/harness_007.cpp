#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace harness_007 {
/*
 * Fuzzing harness for OpenSSL SSL_accept with memory BIOs
 * Targets: SSL_accept (1076 undiscovered branches), SSL handshake acceptance,
 *          memory BIO operations, SSL_connect, SSL_read, SSL_write
 * Specifically targets SSL handshake acceptance with memory BIOs for deeper coverage
 * Focus: SSL_accept with various protocol versions, cipher suites, and handshake scenarios
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

// Create a basic SSL context with certificate and key for testing
static SSL_CTX* create_test_ssl_ctx(FuzzedDataProvider& fdp) {
    SSL_CTX* ctx = SSL_CTX_new(TLS_method());
    if (ctx == NULL) {
        return NULL;
    }
    
    // Set minimum and maximum protocol versions based on fuzzer input
    int min_version = fdp.ConsumeIntegral<uint8_t>() % 5;
    int max_version = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Map to actual protocol versions
    switch (min_version) {
        case 0: SSL_CTX_set_min_proto_version(ctx, TLS1_VERSION); break;
        case 1: SSL_CTX_set_min_proto_version(ctx, TLS1_1_VERSION); break;
        case 2: SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION); break;
        case 3: SSL_CTX_set_min_proto_version(ctx, TLS1_3_VERSION); break;
        default: SSL_CTX_set_min_proto_version(ctx, 0); break; // Any version
    }
    
    switch (max_version) {
        case 0: SSL_CTX_set_max_proto_version(ctx, TLS1_VERSION); break;
        case 1: SSL_CTX_set_max_proto_version(ctx, TLS1_1_VERSION); break;
        case 2: SSL_CTX_set_max_proto_version(ctx, TLS1_2_VERSION); break;
        case 3: SSL_CTX_set_max_proto_version(ctx, TLS1_3_VERSION); break;
        default: SSL_CTX_set_max_proto_version(ctx, 0); break; // Any version
    }
    
    // Set cipher list based on fuzzer input
    const char* cipher_lists[] = {
        "ALL:eNULL:@SECLEVEL=0",
        "HIGH:!aNULL:!MD5",
        "ECDHE-RSA-AES128-GCM-SHA256",
        "AES128-SHA",
        NULL
    };
    
    int cipher_idx = fdp.ConsumeIntegralInRange<int>(0, 3);
    if (cipher_lists[cipher_idx] != NULL) {
        SSL_CTX_set_cipher_list(ctx, cipher_lists[cipher_idx]);
    }
    
    // Generate a temporary self-signed certificate for testing
    EVP_PKEY* pkey = EVP_PKEY_new();
    if (pkey != NULL) {
        RSA* rsa = RSA_generate_key(512, RSA_F4, NULL, NULL);
        if (rsa != NULL) {
            EVP_PKEY_assign_RSA(pkey, rsa);
            
            X509* x509 = X509_new();
            if (x509 != NULL) {
                ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
                X509_gmtime_adj(X509_getm_notBefore(x509), 0);
                X509_gmtime_adj(X509_getm_notAfter(x509), 31536000L); // 1 year
                X509_set_pubkey(x509, pkey);
                
                const X509_NAME* name = X509_get_subject_name(x509);
                if (name != NULL) {
                    // Create a copy of the name to modify
                    X509_NAME* name_copy = X509_NAME_dup(name);
                    if (name_copy != NULL) {
                        X509_NAME_add_entry_by_txt(name_copy, "C", MBSTRING_ASC, 
                                                  (unsigned char*)"US", -1, -1, 0);
                        X509_NAME_add_entry_by_txt(name_copy, "CN", MBSTRING_ASC,
                                                  (unsigned char*)"Test Certificate", -1, -1, 0);
                        
                        X509_set_issuer_name(x509, name_copy);
                        X509_set_subject_name(x509, name_copy);
                        X509_NAME_free(name_copy);
                    }
                }
                
                X509_sign(x509, pkey, EVP_sha256());
                
                // Use the certificate and key in SSL context
                SSL_CTX_use_certificate(ctx, x509);
                SSL_CTX_use_PrivateKey(ctx, pkey);
                
                X509_free(x509);
            }
            // RSA freed by EVP_PKEY_free
        }
        EVP_PKEY_free(pkey);
    }
    
    return ctx;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Too small for meaningful SSL testing
    }
    
    initialize_openssl();
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test configuration from fuzzer input
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t use_blocking = fdp.ConsumeBool() ? 1 : 0;
    uint8_t handshake_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume handshake data (simulated client hello)
    size_t handshake_data_size = fdp.ConsumeIntegralInRange<size_t>(1, size - 16);
    std::vector<uint8_t> handshake_data = fdp.ConsumeBytes<uint8_t>(handshake_data_size);
    
    // Consume application data for read/write tests
    std::vector<uint8_t> app_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Clear any previous errors
    ERR_clear_error();
    
    SSL_CTX* ctx = create_test_ssl_ctx(fdp);
    if (ctx == NULL) {
        return 0;
    }
    
    SSL* ssl = SSL_new(ctx);
    if (ssl == NULL) {
        SSL_CTX_free(ctx);
        return 0;
    }
    
    // Create memory BIOs for testing
    BIO* rbio = BIO_new(BIO_s_mem());
    BIO* wbio = BIO_new(BIO_s_mem());
    
    if (rbio == NULL || wbio == NULL) {
        if (rbio != NULL) BIO_free(rbio);
        if (wbio != NULL) BIO_free(wbio);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return 0;
    }
    
    // Set BIOs to SSL object (SSL takes ownership)
    SSL_set_bio(ssl, rbio, wbio);
    
    // Set SSL to accept state (server mode)
    SSL_set_accept_state(ssl);
    
    // Write handshake data to read BIO (simulating client hello)
    BIO_write(rbio, handshake_data.data(), handshake_data.size());
    
    int ret = 0;
    
    switch (test_scenario) {
        case 0: {
            // Test SSL_accept directly
            ret = SSL_accept(ssl);
            
            // Check error if accept failed
            if (ret <= 0) {
                int ssl_err = SSL_get_error(ssl, ret);
                (void)ssl_err; // Use to avoid warning
                
                // For non-blocking scenarios, we might get WANT_READ/WANT_WRITE
                if (ssl_err == SSL_ERROR_WANT_READ || ssl_err == SSL_ERROR_WANT_WRITE) {
                    // Try to provide more data if available
                    if (!app_data.empty()) {
                        BIO_write(rbio, app_data.data(), app_data.size());
                        ret = SSL_accept(ssl);
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test SSL_do_handshake (alternative to SSL_accept)
            ret = SSL_do_handshake(ssl);
            
            if (ret <= 0) {
                int ssl_err = SSL_get_error(ssl, ret);
                (void)ssl_err;
                
                // Try reading/writing based on error
                if (ssl_err == SSL_ERROR_WANT_READ) {
                    // Check if there's data in write BIO
                    char buf[1024];
                    int bytes = BIO_read(wbio, buf, sizeof(buf));
                    (void)bytes; // Use to avoid warning
                }
            }
            break;
        }
        
        case 2: {
            // Test mixed SSL_accept and SSL_read/SSL_write operations
            ret = SSL_accept(ssl);
            
            if (ret > 0) {
                // Handshake successful, try read/write operations
                if (!app_data.empty()) {
                    // Try to write application data
                    char read_buf[1024];
                    int read_ret = SSL_read(ssl, read_buf, sizeof(read_buf));
                    (void)read_ret;
                    
                    // Try to write some data
                    if (app_data.size() > 0) {
                        int write_ret = SSL_write(ssl, app_data.data(), 
                                                 app_data.size() > 1024 ? 1024 : app_data.size());
                        (void)write_ret;
                    }
                }
            } else {
                // Handshake failed, try to read error queue
                unsigned long err = ERR_get_error();
                (void)err;
            }
            break;
        }
        
        case 3: {
            // Test SSL_connect after SSL_accept (client/server role reversal)
            // First do partial accept
            ret = SSL_accept(ssl);
            
            if (ret <= 0) {
                // Get data from write BIO (server response)
                char server_response[2048];
                int response_len = BIO_read(wbio, server_response, sizeof(server_response));
                
                if (response_len > 0) {
                    // Create a new SSL object for client testing
                    SSL* client_ssl = SSL_new(ctx);
                    if (client_ssl != NULL) {
                        BIO* client_rbio = BIO_new(BIO_s_mem());
                        BIO* client_wbio = BIO_new(BIO_s_mem());
                        
                        if (client_rbio != NULL && client_wbio != NULL) {
                            SSL_set_bio(client_ssl, client_rbio, client_wbio);
                            SSL_set_connect_state(client_ssl);
                            
                            // Write server response to client read BIO
                            BIO_write(client_rbio, server_response, response_len);
                            
                            // Try client handshake
                            int client_ret = SSL_connect(client_ssl);
                            (void)client_ret;
                            
                            // Clean up client SSL
                            SSL_free(client_ssl);
                        } else {
                            if (client_rbio != NULL) BIO_free(client_rbio);
                            if (client_wbio != NULL) BIO_free(client_wbio);
                            SSL_free(client_ssl);
                        }
                    }
                }
            }
            break;
        }
    }
    
    // Additional SSL state inspection
    int state = SSL_get_state(ssl);
    (void)state;
    
    const char* cipher = SSL_get_cipher_name(ssl);
    (void)cipher;
    
    // Check for any pending data in write BIO
    if (BIO_pending(wbio) > 0) {
        char pending_buf[4096];
        int pending_len = BIO_read(wbio, pending_buf, sizeof(pending_buf));
        (void)pending_len;
    }
    
    // SSL_free will free the BIOs that were transferred to SSL
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    
    // Clear error queue
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_007
