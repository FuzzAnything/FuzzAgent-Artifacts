#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_002 {
/*
 * Fuzzing harness for OpenSSL SSL/TLS protocol stack
 * Targets: SSL/TLS connection lifecycle with in-memory BIO pair for fuzzing
 * Coverage goal: Exercise 61k+ lines and 3k+ functions in SSL module (currently 0% covered)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: enough for basic SSL operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL SSL library
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    
    // Consume SSL method type (client vs server)
    uint8_t method_choice = fdp.ConsumeIntegral<uint8_t>() % 2;
    const SSL_METHOD* method = nullptr;
    
    if (method_choice == 0) {
        method = TLS_method();  // Generic TLS method
    } else {
        method = TLS_client_method();  // Client method
    }
    
    if (method == nullptr) {
        return 0;
    }
    
    // Create SSL context
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (ctx == nullptr) {
        return 0;
    }
    
    // Set minimum TLS version for testing
    SSL_CTX_set_min_proto_version(ctx, TLS1_VERSION);
    SSL_CTX_set_max_proto_version(ctx, TLS1_3_VERSION);
    
    // Create SSL object
    SSL* ssl = SSL_new(ctx);
    if (ssl == nullptr) {
        SSL_CTX_free(ctx);
        return 0;
    }
    
    // Create in-memory BIO pair for fuzzing
    BIO* bio_read = nullptr;
    BIO* bio_write = nullptr;
    
    // Create memory BIOs for fuzzing input/output
    bio_read = BIO_new(BIO_s_mem());
    bio_write = BIO_new(BIO_s_mem());
    
    if (bio_read == nullptr || bio_write == nullptr) {
        if (bio_read) BIO_free(bio_read);
        if (bio_write) BIO_free(bio_write);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return 0;
    }
    
    // Set up BIO pair for SSL
    SSL_set_bio(ssl, bio_read, bio_write);
    
    // Determine whether to act as client or server
    uint8_t role = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume some data to write to the BIO for handshake simulation
    size_t handshake_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    std::vector<uint8_t> handshake_data = fdp.ConsumeBytes<uint8_t>(handshake_data_size);
    
    if (handshake_data.size() > 0) {
        // Write fuzzed data to read BIO to simulate incoming handshake data
        BIO_write(bio_read, handshake_data.data(), handshake_data.size());
    }
    
    // Try to perform handshake based on role
    int handshake_result = 0;
    if (role == 0) {
        // Act as client - try to connect
        handshake_result = SSL_connect(ssl);
    } else {
        // Act as server - try to accept
        handshake_result = SSL_accept(ssl);
    }
    
    // Consume more data for application data testing
    size_t app_data_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    std::vector<uint8_t> app_data = fdp.ConsumeBytes<uint8_t>(app_data_size);
    
    if (app_data.size() > 0) {
        // Test SSL_write if we have data
        if (handshake_result > 0) {
            // SSL connection is established (or attempted)
            // Write some application data
            char write_buffer[1024];
            size_t write_size = app_data.size() < sizeof(write_buffer) ? 
                               app_data.size() : sizeof(write_buffer);
            memcpy(write_buffer, app_data.data(), write_size);
            
            int write_result = SSL_write(ssl, write_buffer, write_size);
            (void)write_result;  // Result not critical for fuzzing
            
            // Try to read (might fail if no data available)
            char read_buffer[1024];
            int read_result = SSL_read(ssl, read_buffer, sizeof(read_buffer));
            (void)read_result;  // Result not critical for fuzzing
        }
    }
    
    // Test SSL_pending and SSL_has_pending
    int pending = SSL_pending(ssl);
    int has_pending = SSL_has_pending(ssl);
    (void)pending;
    (void)has_pending;
    
    // Test SSL_get_error to check error states
    int ssl_error = SSL_get_error(ssl, 0);
    (void)ssl_error;
    
    // Test SSL_shutdown (attempt graceful closure)
    int shutdown_result = SSL_shutdown(ssl);
    (void)shutdown_result;
    
    // Test SSL_get_version
    const char* ssl_version = SSL_get_version(ssl);
    (void)ssl_version;
    
    // Test SSL_get_cipher
    const SSL_CIPHER* cipher = SSL_get_current_cipher(ssl);
    if (cipher != nullptr) {
        const char* cipher_name = SSL_CIPHER_get_name(cipher);
        (void)cipher_name;
    }
    
    // Clean up
    // Note: bio_read and bio_write are freed by SSL_free via SSL_set_bio
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    
    // Clean up global OpenSSL state
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
    
    return 0;
}

} // namespace harness_002
