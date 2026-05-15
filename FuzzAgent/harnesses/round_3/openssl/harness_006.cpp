#include <stddef.h>
#include <stdint.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_006 {
/*
 * Fuzzing harness for OpenSSL SSL/TLS event handling operations
 * Primary target: SSL_handle_events (12 branches, 1111 undiscovered accumulated branches)
 * Targets: SSL_handle_events, SSL_new, SSL_CTX_new, SSL_set_bio, SSL_connect/SSL_accept, 
 *          SSL_read/SSL_write, SSL_shutdown, SSL_free, SSL_CTX_free
 * Helper APIs: OPENSSL_init_ssl, BIO_new(BIO_s_mem()), SSL_set_connect_state/SSL_set_accept_state
 * Implements SSL event handling lifecycle: Initialize → Create context → Create SSL → Setup BIOs → 
 * Set mode → Connect/Accept → Handle events → Write/Read → Shutdown → Cleanup
 * Differentiated from harness_002.cpp (SSL/TLS handshake) - this harness focuses specifically on event handling operations
 * SSL_handle_events is critical for asynchronous SSL/TLS operations (DTLS timeout handling, QUIC event processing)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize OpenSSL SSL library
    OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    ERR_clear_error();

    // Choose between DTLS and TLS for event handling testing
    // SSL_handle_events is primarily useful for DTLS (timeout handling) and QUIC
    uint8_t protocol_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    const SSL_METHOD* method = NULL;
    
    switch (protocol_choice) {
        case 0:
            method = DTLS_method();  // DTLS supports SSL_handle_events for timeout handling
            break;
        case 1:
            method = TLS_client_method();  // TLS client
            break;
        case 2:
            method = TLS_server_method();  // TLS server
            break;
    }
    
    if (method == NULL) {
        return 0;
    }

    // Create SSL context
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
        return 0;
    }

    // Configure protocol versions from fuzzer input for TLS/DTLS
    if (protocol_choice == 0) {
        // DTLS version configuration
        uint8_t dtls_version = fdp.ConsumeIntegral<uint8_t>() % 2;
        if (dtls_version == 0) {
            SSL_CTX_set_min_proto_version(ctx, DTLS1_VERSION);
            SSL_CTX_set_max_proto_version(ctx, DTLS1_VERSION);
        } else {
            SSL_CTX_set_min_proto_version(ctx, DTLS1_2_VERSION);
            SSL_CTX_set_max_proto_version(ctx, DTLS1_2_VERSION);
        }
    } else {
        // TLS version configuration
        uint8_t tls_version = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (tls_version) {
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
    }

    // Create SSL structure
    SSL* ssl = SSL_new(ctx);
    if (ssl == NULL) {
        SSL_CTX_free(ctx);
        return 0;
    }

    // Setup memory BIOs for I/O (non-blocking memory BIOs)
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

    // Set client or server mode based on fuzzer input
    uint8_t mode_selector = fdp.ConsumeIntegral<uint8_t>() % 2;
    if (mode_selector == 0) {
        SSL_set_connect_state(ssl);  // Client mode
    } else {
        SSL_set_accept_state(ssl);   // Server mode
    }

    // Write some initial data to the read BIO to simulate network input
    size_t simulated_input_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    if (simulated_input_size > 0 && fdp.remaining_bytes() >= simulated_input_size) {
        std::vector<uint8_t> simulated_data = fdp.ConsumeBytes<uint8_t>(simulated_input_size);
        BIO_write(rbio, simulated_data.data(), simulated_data.size());
    }

    // Attempt connection establishment (SSL_connect or SSL_accept)
    int connect_result;
    if (mode_selector == 0) {
        connect_result = SSL_connect(ssl);
    } else {
        connect_result = SSL_accept(ssl);
    }

    // PRIMARY TARGET: SSL_handle_events - process pending events
    // This is the core API we're targeting for coverage
    int events_result = SSL_handle_events(ssl);
    // Note: SSL_handle_events returns 1 on success, 0 on failure
    // For TLS connections, it's a no-op but still returns success

    // If connection made progress or SSL_handle_events was called, test data exchange
    if (connect_result == 1 || SSL_get_error(ssl, connect_result) == SSL_ERROR_WANT_READ || 
        SSL_get_error(ssl, connect_result) == SSL_ERROR_WANT_WRITE || events_result == 1) {
        
        // Try to write data if we have enough input
        if (fdp.remaining_bytes() >= 16) {
            size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            if (write_size > fdp.remaining_bytes()) {
                write_size = fdp.remaining_bytes();
            }
            
            std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_size);
            int write_result = SSL_write(ssl, write_data.data(), write_data.size());
            
            // Call SSL_handle_events again after write to process any new events
            SSL_handle_events(ssl);
            
            // Try to read if write succeeded or made progress
            if (write_result > 0 || SSL_get_error(ssl, write_result) == SSL_ERROR_WANT_READ || 
                SSL_get_error(ssl, write_result) == SSL_ERROR_WANT_WRITE) {
                
                // Check if there's data in the write BIO (encrypted output)
                char read_buffer[1024];
                int read_result = SSL_read(ssl, read_buffer, sizeof(read_buffer));
                
                // Call SSL_handle_events after read attempt
                SSL_handle_events(ssl);
                
                // read_result might be <= 0 (SSL_ERROR_WANT_READ/WANT_WRITE) which is expected
            }
        }
        
        // Test multiple calls to SSL_handle_events with different timing scenarios
        // This helps explore the state machine for event processing
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
            // Consume a small amount of data to vary the timing
            uint8_t timing_value = fdp.ConsumeIntegral<uint8_t>();
            (void)timing_value; // Use value to affect fuzzer exploration
            
            // Call SSL_handle_events multiple times to test event accumulation/processing
            SSL_handle_events(ssl);
            
            // Simulate some data being available for reading
            if (fdp.remaining_bytes() > 10) {
                size_t additional_data = fdp.ConsumeIntegralInRange<size_t>(0, 64);
                if (additional_data > 0 && fdp.remaining_bytes() >= additional_data) {
                    std::vector<uint8_t> additional_bytes = fdp.ConsumeBytes<uint8_t>(additional_data);
                    BIO_write(rbio, additional_bytes.data(), additional_bytes.size());
                    
                    // Process events with new data available
                    SSL_handle_events(ssl);
                }
            }
        }
        // Attempt graceful shutdown
        int shutdown_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (shutdown_mode) {
            case 0:
                // Standard shutdown
                SSL_shutdown(ssl);
                SSL_handle_events(ssl);  // Process events during shutdown
                break;
            case 1: {
                // Two-step shutdown
                int shutdown_result = SSL_shutdown(ssl);
                SSL_handle_events(ssl);
                if (shutdown_result == 0) {
                    // Need to call SSL_shutdown again
                    SSL_shutdown(ssl);
                    SSL_handle_events(ssl);
                }
                break;
            }
            case 2:
                // Immediate shutdown with event handling
                SSL_set_shutdown(ssl, SSL_SENT_SHUTDOWN | SSL_RECEIVED_SHUTDOWN);
                SSL_handle_events(ssl);
                break;
        }
    }

    // Final cleanup with event handling
    SSL_handle_events(ssl);  // One last event processing before cleanup
    
    // Cleanup
    SSL_free(ssl);  // This also frees the BIOs
    SSL_CTX_free(ctx);
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_006
