#include <stddef.h>
#include <stdint.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_002 {
/*
 * Fuzzing harness for OpenSSL SSL/TLS handshake operations
 * Targets: SSL_CTX_new, SSL_new, SSL_do_handshake, SSL_write, SSL_read, SSL_shutdown, SSL_free, SSL_CTX_free
 * Helper APIs: OPENSSL_init_ssl (replaces SSL_library_init, OpenSSL_add_ssl_algorithms, SSL_load_error_strings),
 *              SSL_CTX_set_min_proto_version, SSL_CTX_set_max_proto_version,
 *              SSL_set_bio, SSL_set_connect_state/SSL_set_accept_state, BIO_new(BIO_s_mem())
 * Implements full SSL connection lifecycle: Initialize → Create context → Create SSL → Setup BIOs → 
 * Set mode → Handshake → Write/Read → Shutdown → Cleanup
 * Differentiated from harness_000.cpp (hash/symmetric crypto) and harness_001.cpp (x509 certificate operations)
 * This harness targets network security protocols and aims to break the 0% coverage barrier in SSL module
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize OpenSSL SSL library
    OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    ERR_clear_error();

    // Create SSL context
    SSL_CTX* ctx = SSL_CTX_new(TLS_method());
    if (ctx == NULL) {
        return 0;
    }

    // Configure protocol versions from fuzzer input
    uint8_t version_selector = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (version_selector) {
        case 0:
            SSL_CTX_set_min_proto_version(ctx, TLS1_VERSION);
            SSL_CTX_set_max_proto_version(ctx, TLS1_VERSION);
            break;
        case 1:
            SSL_CTX_set_min_proto_version(ctx, TLS1_1_VERSION);
            SSL_CTX_set_max_proto_version(ctx, TLS1_1_VERSION);
            break;
        case 2:
            SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
            SSL_CTX_set_max_proto_version(ctx, TLS1_2_VERSION);
            break;
        case 3:
            SSL_CTX_set_min_proto_version(ctx, TLS1_3_VERSION);
            SSL_CTX_set_max_proto_version(ctx, TLS1_3_VERSION);
            break;
    }

    // Create SSL structure
    SSL* ssl = SSL_new(ctx);
    if (ssl == NULL) {
        SSL_CTX_free(ctx);
        return 0;
    }

    // Setup memory BIOs for I/O
    BIO* rbio = BIO_new(BIO_s_mem());
    BIO* wbio = BIO_new(BIO_s_mem());
    if (rbio == NULL || wbio == NULL) {
        if (rbio != NULL) BIO_free(rbio);
        if (wbio != NULL) BIO_free(wbio);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return 0;
    }

    // Set BIOs on SSL structure
    SSL_set_bio(ssl, rbio, wbio);

    // Set client mode (simulate client trying to connect)
    SSL_set_connect_state(ssl);

    // Write some initial handshake data to the read BIO
    // In a real scenario, this would come from the server, but for fuzzing we simulate it
    size_t simulated_handshake_size = fdp.ConsumeIntegralInRange<size_t>(0, 128);
    if (simulated_handshake_size > 0 && fdp.remaining_bytes() >= simulated_handshake_size) {
        std::vector<uint8_t> simulated_data = fdp.ConsumeBytes<uint8_t>(simulated_handshake_size);
        BIO_write(rbio, simulated_data.data(), simulated_data.size());
    }

    // Attempt handshake (core target API)
    int handshake_result = SSL_do_handshake(ssl);
    
    // If handshake succeeds (or makes progress), test write/read operations
    if (handshake_result == 1 || SSL_get_error(ssl, handshake_result) == SSL_ERROR_WANT_READ || 
        SSL_get_error(ssl, handshake_result) == SSL_ERROR_WANT_WRITE) {
        
        // Try to write data if we have enough input
        if (fdp.remaining_bytes() >= 16) {
            size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            if (write_size > fdp.remaining_bytes()) {
                write_size = fdp.remaining_bytes();
            }
            
            std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_size);
            int write_result = SSL_write(ssl, write_data.data(), write_data.size());
            
            // If write succeeded, try to read (though in memory BIO setup, we'd need to process the output)
            if (write_result > 0) {
                // Check if there's data in the write BIO (encrypted output)
                char read_buffer[1024];
                int read_result = SSL_read(ssl, read_buffer, sizeof(read_buffer));
                // read_result might be <= 0 (SSL_ERROR_WANT_READ) which is expected
            }
        }
        
        // Attempt graceful shutdown
        int shutdown_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
        int shutdown_result;
        switch (shutdown_mode) {
            case 0:
                shutdown_result = SSL_shutdown(ssl);
                break;
            case 1:
                // Try shutdown with SSL_shutdown, then check if we need to call again
                shutdown_result = SSL_shutdown(ssl);
                if (shutdown_result == 0) {
                    // Need to call SSL_shutdown again
                    shutdown_result = SSL_shutdown(ssl);
                }
                break;
            case 2:
                // Try different shutdown approach
                SSL_set_shutdown(ssl, SSL_SENT_SHUTDOWN | SSL_RECEIVED_SHUTDOWN);
                break;
        }
    }

    // Cleanup
    SSL_free(ssl);  // This also frees the BIOs
    SSL_CTX_free(ctx);
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_002
