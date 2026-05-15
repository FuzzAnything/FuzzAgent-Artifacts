#include <stddef.h>
#include <stdint.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/quic.h>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_003 {
/*
 * Fuzzing harness for OpenSSL QUIC stream operations
 * Targets: SSL_new_stream (1570 branches, 1140 undiscovered), SSL_stream_conclude,
 *          SSL_get0_connection, SSL_get_stream_type, SSL_get_stream_id
 * Helper APIs: SSL_CTX_new, SSL_new, OSSL_QUIC_client_method/OSSL_QUIC_server_method,
 *              SSL_free, SSL_CTX_free
 * Implements full QUIC stream lifecycle: Initialize → Create context → Create SSL connection →
 * Enable QUIC → Create stream → Stream operations → Query properties → Conclude → Cleanup
 * Differentiated from harness_000.cpp (hash/symmetric crypto), harness_001.cpp (x509 certificate operations),
 * and harness_002.cpp (SSL/TLS handshake). This harness targets modern QUIC protocol implementation.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize OpenSSL SSL library
    OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    ERR_clear_error();

    // Determine whether to test client or server mode based on fuzzer input
    bool is_client = fdp.ConsumeBool();
    
    // Create SSL context with appropriate QUIC method
    SSL_CTX* ctx = NULL;
    if (is_client) {
        ctx = SSL_CTX_new(OSSL_QUIC_client_method());
    } else {
        ctx = SSL_CTX_new(OSSL_QUIC_server_method());
    }
    
    if (ctx == NULL) {
        return 0;
    }

    // Create SSL connection object
    SSL* ssl = SSL_new(ctx);
    if (ssl == NULL) {
        SSL_CTX_free(ctx);
        return 0;
    }

    // Setup memory BIOs for I/O (required for QUIC)
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

    // Set appropriate state based on client/server mode
    if (is_client) {
        SSL_set_connect_state(ssl);
    } else {
        SSL_set_accept_state(ssl);
    }

    // Write some simulated data to the read BIO to simulate incoming packets
    size_t simulated_data_size = fdp.ConsumeIntegralInRange<size_t>(0, 128);
    if (simulated_data_size > 0 && fdp.remaining_bytes() >= simulated_data_size) {
        std::vector<uint8_t> simulated_data = fdp.ConsumeBytes<uint8_t>(simulated_data_size);
        BIO_write(rbio, simulated_data.data(), simulated_data.size());
    }

    // Attempt to create streams with various flags based on fuzzer input
    uint8_t stream_operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    SSL* stream = NULL;
    
    switch (stream_operation) {
        case 0: {
            // Create stream with default flags (0)
            stream = SSL_new_stream(ssl, 0);
            break;
        }
        case 1: {
            // Create stream with UNIDIRECTIONAL flag if available
            // Note: Need to check actual flag definition
            uint64_t flags = 0;
            // Try to create stream with flags from input
            if (fdp.remaining_bytes() >= sizeof(uint64_t)) {
                flags = fdp.ConsumeIntegral<uint64_t>();
            }
            stream = SSL_new_stream(ssl, flags);
            break;
        }
        case 2: {
            // Create stream with BIDIRECTIONAL flag if available
            uint64_t flags = 0;
            // Try different flag combinations
            if (fdp.remaining_bytes() >= sizeof(uint64_t)) {
                flags = fdp.ConsumeIntegral<uint64_t>();
                // Clear high bits to keep reasonable values
                flags &= 0xFF;
            }
            stream = SSL_new_stream(ssl, flags);
            break;
        }
        case 3: {
            // Try to create multiple streams
            uint8_t num_streams = fdp.ConsumeIntegralInRange<uint8_t>(1, 3);
            for (uint8_t i = 0; i < num_streams && fdp.remaining_bytes() > 0; i++) {
                uint64_t flags = 0;
                if (fdp.remaining_bytes() >= sizeof(uint64_t)) {
                    flags = fdp.ConsumeIntegral<uint64_t>();
                    flags &= 0xFF;  // Keep reasonable values
                }
                SSL* temp_stream = SSL_new_stream(ssl, flags);
                if (temp_stream != NULL) {
                    // Store first valid stream
                    if (stream == NULL) {
                        stream = temp_stream;
                    } else {
                        SSL_free(temp_stream);
                    }
                }
            }
            break;
        }
    }

    // If we successfully created a stream, perform stream operations
    if (stream != NULL) {
        // Query stream properties (target APIs)
        SSL* connection = SSL_get0_connection(stream);
        int stream_type = SSL_get_stream_type(stream);
        uint64_t stream_id = SSL_get_stream_id(stream);
        
        // Try to conclude the stream if we have enough input
        if (fdp.remaining_bytes() >= sizeof(uint64_t)) {
            uint64_t conclude_flags = fdp.ConsumeIntegral<uint64_t>();
            conclude_flags &= 0xFF;  // Keep reasonable values
            SSL_stream_conclude(stream, conclude_flags);
        }
        
        // Free the stream
        SSL_free(stream);
    }

    // Test edge cases with remaining input
    if (fdp.remaining_bytes() > 0) {
        // Try to get connection from main SSL object
        SSL* conn_check = SSL_get0_connection(ssl);
        
        // Try to get stream type from main SSL object (may not be valid)
        int ssl_stream_type = SSL_get_stream_type(ssl);
        
        // Try to get stream ID from main SSL object (may not be valid)
        uint64_t ssl_stream_id = SSL_get_stream_id(ssl);
    }

    // Clean up
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_003
