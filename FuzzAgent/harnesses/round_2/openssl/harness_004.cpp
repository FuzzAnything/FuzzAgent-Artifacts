#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace harness_004 {
/*
 * Fuzzing harness for OpenSSL QUIC/HTTP3 protocol functions
 * Targets: QUIC protocol stack with 0% coverage
 * Coverage goal: Uncover QUIC module (3000+ lines with 0% coverage)
 * Semantic differentiation: QUIC protocol testing (vs. crypto basics, SSL/TLS, OCSP, and providers in previous harnesses)
 * 
 * APIs targeted:
 * - SSL_new_from_listener - QUIC server connection creation
 * - SSL_get_event_timeout - QUIC event timing management
 * - SSL_has_pending - QUIC pending data checking
 * - SSL_stream_conclude - QUIC stream lifecycle management
 * - SSL_get_value_uint - QUIC parameter retrieval
 * - SSL_new, SSL_free - Connection lifecycle
 * - SSL_set_accept_state/SSL_set_connect_state - Connection state
 * 
 * QUIC is a modern transport protocol for HTTP/3 with different state machine
 * and packet handling than traditional TLS over TCP.
 */


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 64) {
        return 0; // Need minimum input for meaningful QUIC testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL SSL library
    OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | 
                    OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    ERR_clear_error();
    
    // Determine QUIC operation mode: 0 = client, 1 = server, 2 = listener mode
    uint8_t mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Create SSL context with QUIC method
    SSL_CTX *ctx = NULL;
    if (mode == 0) {
        // Client mode
        ctx = SSL_CTX_new(OSSL_QUIC_client_method());
    } else {
        // Server or listener mode
        ctx = SSL_CTX_new(OSSL_QUIC_server_method());
    }
    
    if (ctx == NULL) {
        return 0;
    }
    
    // Create memory BIOs for fuzzing QUIC datagram transport
    BIO *net_rbio = BIO_new(BIO_s_dgram_mem());
    BIO *net_wbio = BIO_new(BIO_s_dgram_mem());
    
    if (net_rbio == NULL || net_wbio == NULL) {
        if (net_rbio) BIO_free(net_rbio);
        if (net_wbio) BIO_free(net_wbio);
        SSL_CTX_free(ctx);
        return 0;
    }
    
    SSL *ssl = NULL;
    SSL *listener = NULL;
    
    switch (mode) {
        case 0: { // Client mode
            ssl = SSL_new(ctx);
            if (ssl == NULL) {
                break;
            }
            
            // Set BIOs for QUIC transport
            SSL_set_bio(ssl, net_rbio, net_wbio);
            SSL_set_connect_state(ssl);
            
            // Write some fuzzed data to simulate incoming QUIC packets
            size_t quic_packet_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            std::vector<uint8_t> quic_packet = fdp.ConsumeBytes<uint8_t>(quic_packet_len);
            if (!quic_packet.empty()) {
                BIO_write(net_rbio, quic_packet.data(), quic_packet.size());
            }
            
            // Test QUIC-specific APIs
            struct timeval tv;
            int is_infinite = 0;
            SSL_get_event_timeout(ssl, &tv, &is_infinite);
            
            int has_pending = SSL_has_pending(ssl);
            
            // Try to get some QUIC parameter values
            uint64_t quic_value = 0;
            SSL_get_value_uint(ssl, SSL_VALUE_CLASS_GENERIC, 0, &quic_value);
            
            // Attempt handshake (will fail without proper server, but exercises code)
            SSL_do_handshake(ssl);
            
            // Check pending again after handshake attempt
            has_pending = SSL_has_pending(ssl);
            
            // Try to create a stream if connection seems established
            if (SSL_is_init_finished(ssl)) {
                // Create a bidirectional stream
                SSL *stream = SSL_new_stream(ssl, 0);
                if (stream != NULL) {
                    // Try to conclude the stream
                    SSL_stream_conclude(stream, 0);
                    SSL_free(stream);
                }
            }
            
            SSL_free(ssl);
            break;
        }
        
        case 1: { // Server mode (regular connection)
            ssl = SSL_new(ctx);
            if (ssl == NULL) {
                break;
            }
            
            SSL_set_bio(ssl, net_rbio, net_wbio);
            SSL_set_accept_state(ssl);
            
            // Write fuzzed data to simulate client QUIC packets
            size_t quic_packet_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            std::vector<uint8_t> quic_packet = fdp.ConsumeBytes<uint8_t>(quic_packet_len);
            if (!quic_packet.empty()) {
                BIO_write(net_rbio, quic_packet.data(), quic_packet.size());
            }
            
            // Test QUIC event timeout
            struct timeval tv;
            int is_infinite = 0;
            SSL_get_event_timeout(ssl, &tv, &is_infinite);
            
            // Test pending check
            int has_pending = SSL_has_pending(ssl);
            
            // Try to handle events
            SSL_handle_events(ssl);
            
            // Try to accept a stream if listener
            SSL *accepted_stream = SSL_accept_stream(ssl, 0);
            if (accepted_stream != NULL) {
                // Test stream operations
                uint64_t stream_id = SSL_get_stream_id(accepted_stream);
                int stream_type = SSL_get_stream_type(accepted_stream);
                
                // Try to conclude stream
                SSL_stream_conclude(accepted_stream, 0);
                
                SSL_free(accepted_stream);
            }
            
            SSL_free(ssl);
            break;
        }
        
        case 2: { // Listener mode (SSL_new_from_listener)
            // First create a listener
            listener = SSL_new_listener(ctx, 0);
            if (listener == NULL) {
                break;
            }
            
            SSL_set_bio(listener, net_rbio, net_wbio);
            
            // Write fuzzed data to simulate incoming connection attempts
            size_t quic_packet_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            std::vector<uint8_t> quic_packet = fdp.ConsumeBytes<uint8_t>(quic_packet_len);
            if (!quic_packet.empty()) {
                BIO_write(net_rbio, quic_packet.data(), quic_packet.size());
            }
            
            // Test listener-specific APIs
            int is_listener = SSL_is_listener(listener);
            SSL *connection_from_listener = SSL_new_from_listener(listener, 0);
            
            if (connection_from_listener != NULL) {
                // Test QUIC APIs on the new connection
                struct timeval tv;
                int is_infinite = 0;
                SSL_get_event_timeout(connection_from_listener, &tv, &is_infinite);
                
                int has_pending = SSL_has_pending(connection_from_listener);
                
                // Try to get QUIC parameters
                uint64_t quic_value = 0;
                SSL_get_value_uint(connection_from_listener, SSL_VALUE_CLASS_GENERIC, 1, &quic_value);
                
                // Try to accept a stream
                SSL *stream = SSL_accept_stream(connection_from_listener, 0);
                if (stream != NULL) {
                    // Test stream conclusion
                    SSL_stream_conclude(stream, 0);
                    SSL_free(stream);
                }
                
                SSL_free(connection_from_listener);
            }
            
            // Test accept_connection API
            SSL *accepted_conn = SSL_accept_connection(listener, 0);
            if (accepted_conn != NULL) {
                SSL_free(accepted_conn);
            }
            
            SSL_free(listener);
            break;
        }
    }
    
    // Cleanup
    SSL_CTX_free(ctx);
    
    return 0;
}

} // namespace harness_004
