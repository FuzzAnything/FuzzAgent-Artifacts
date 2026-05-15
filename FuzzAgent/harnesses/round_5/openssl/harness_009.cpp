#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/bio.h>
#include <openssl/rand.h>

namespace harness_009 {
/*
 * Fuzzing harness for OpenSSL targeting SSL/TLS handshake operations with focus on DTLS and block padding functionality.
 * This harness tests: SSL_set_block_padding (primary target - 0% coverage, 24 branches), SSL_new, SSL_CTX_new, 
 * SSL_set_fd/SSL_set_bio, SSL_connect/SSL_accept, SSL_read/SSL_write, SSL_shutdown, SSL_free, SSL_CTX_free, 
 * DTLSv1_listen, SSL_get_error.
 * This harness is semantically different from previous harnesses by targeting the complete SSL/TLS handshake lifecycle:
 * Context Init → Object Creation → I/O Setup → Configuration (Block Padding) → Handshake → Data Transfer → Cleanup
 * Includes both TLS and DTLS protocols, with specific focus on DTLSv1_listen for DTLS server functionality.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize OpenSSL crypto library
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                        OPENSSL_INIT_ADD_ALL_CIPHERS | 
                        OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();

    // Need minimum input size for meaningful SSL operations
    // We need: protocol choice, block padding size, handshake mode, and data for read/write
    if (size < 32) {
        return 0;  // Need enough data for basic SSL operations
    }

    FuzzedDataProvider fdp(data, size);

    // Declare ALL variables at the top to avoid goto jumping over initializations
    SSL_CTX* ctx = NULL;
    SSL* ssl = NULL;
    BIO* bio1 = NULL;
    BIO* bio2 = NULL;
    BIO* rbio = NULL;
    BIO* wbio = NULL;
    
    // Variables for data consumption
    int protocol_choice = 0;
    int handshake_mode = 0;
    size_t block_padding_size = 0;
    int use_bio = 0;
    int dtls_listen_test = 0;
    size_t read_write_size = 0;
    std::vector<uint8_t> read_write_data;
    std::vector<uint8_t> read_buffer;
    int ssl_error = 0;
    
    // Variables for API results
    int ssl_set_padding_result = 0;
    int ssl_connect_result = 0;
    int ssl_accept_result = 0;
    int ssl_read_result = 0;
    int ssl_write_result = 0;
    int ssl_shutdown_result = 0;
    int dtls_listen_result = 0;
    
    // Variables for test_retvals array (must be declared before any goto)
    int test_retvals[] = {0, 1, -1, 100};
    // Other temporary variables
    const SSL_METHOD* method = NULL;
    int dummy_fd = -1;
    size_t app_block_size = 0;
    
    // Choose protocol: TLS_method() or DTLS_method()
    protocol_choice = fdp.ConsumeIntegralInRange<int>(0, 1);
    
    if (protocol_choice == 0) {
        method = TLS_method();      // TLS 1.0, 1.1, 1.2, 1.3
    } else {
        method = DTLS_method();     // DTLS 1.0 and 1.2
    }
    
    if (method == NULL) {
        return 0;
    }
    ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
        goto cleanup;
    }
    
    // Set minimal security options to avoid handshake failures
    SSL_CTX_set_min_proto_version(ctx, TLS1_VERSION);
    SSL_CTX_set_max_proto_version(ctx, TLS1_3_VERSION);
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);

    // ==================== STEP 2: CREATE SSL OBJECT ====================
    ssl = SSL_new(ctx);
    if (ssl == NULL) {
        goto cleanup;
    }

    // ==================== STEP 3: SET UP I/O ====================
    // Choose between SSL_set_fd (file descriptor) or SSL_set_bio (BIO)
    use_bio = fdp.ConsumeBool();
    
    if (use_bio) {
        // Create memory BIOs for testing
        bio1 = BIO_new(BIO_s_mem());
        bio2 = BIO_new(BIO_s_mem());
        
        if (bio1 == NULL || bio2 == NULL) {
            goto cleanup;
        }
        
        // SSL_set_bio expects rbio and wbio
        SSL_set_bio(ssl, bio1, bio2);
        
        // We'll keep the BIOs for later use
        rbio = bio1;
        wbio = bio2;
    } else {
        // For SSL_set_fd, we would need a real file descriptor
        // Since we're fuzzing, we'll create a socket pair or use dummy fd
        // For simplicity, we'll fall back to BIO approach in fuzzing context
        // but still call SSL_set_fd with a dummy value
        int dummy_fd = -1;  // Dummy file descriptor
        SSL_set_fd(ssl, dummy_fd);
    }

    // ==================== STEP 4: CONFIGURE BLOCK PADDING (PRIMARY TARGET) ====================
    // SSL_set_block_padding - primary target with 0% coverage, 24 branches
    block_padding_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    ssl_set_padding_result = SSL_set_block_padding(ssl, block_padding_size);
    
    // Also test SSL_set_block_padding_ex if available
    // Note: SSL_set_block_padding_ex is defined in the header, we'll test it conditionally
    #ifdef SSL_set_block_padding_ex
    if (fdp.ConsumeBool()) {
        size_t app_block_size = fdp.ConsumeIntegralInRange<size_t>(0, 2048);
        SSL_set_block_padding_ex(ssl, app_block_size, 0);  // Third parameter unclear from header
    }
    #endif

    // ==================== STEP 5: HANDSHAKE OPERATIONS ====================
    handshake_mode = fdp.ConsumeIntegralInRange<int>(0, 2);  // 0=client, 1=server, 2=both
    
    // For testing handshake, we need to simulate both sides
    // Since we're fuzzing, we'll attempt handshake but expect it to fail
    // (no real network connection, no certificates configured)
    
    if (handshake_mode == 0 || handshake_mode == 2) {
        // Client-side handshake attempt
        ssl_connect_result = SSL_connect(ssl);
        ssl_error = SSL_get_error(ssl, ssl_connect_result);
    }
    
    if (handshake_mode == 1 || handshake_mode == 2) {
        // Server-side handshake attempt
        ssl_accept_result = SSL_accept(ssl);
        ssl_error = SSL_get_error(ssl, ssl_accept_result);
    }

    // ==================== STEP 6: DTLS-SPECIFIC TESTING ====================
    if (protocol_choice == 1) {  // DTLS protocol
        // Test DTLSv1_listen - DTLS-specific functionality
        dtls_listen_test = fdp.ConsumeBool();
        if (dtls_listen_test) {
            // DTLSv1_listen expects BIO_ADDR* client parameter
            // Since we don't have real network, we'll pass NULL
            // The function should handle NULL parameter gracefully
            dtls_listen_result = DTLSv1_listen(ssl, NULL);
            ssl_error = SSL_get_error(ssl, dtls_listen_result);
        }
    }

    // ==================== STEP 7: DATA EXCHANGE ====================
    // Test SSL_read and SSL_write even if handshake didn't complete
    // These functions will fail but exercise error paths
    
    // Prepare some application data for read/write testing
    read_write_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    if (read_write_size > 0 && fdp.remaining_bytes() >= read_write_size) {
        read_write_data = fdp.ConsumeBytes<uint8_t>(read_write_size);
        
        if (read_write_data.size() > 0) {
            // Attempt SSL_write
            ssl_write_result = SSL_write(ssl, read_write_data.data(), read_write_data.size());
            ssl_error = SSL_get_error(ssl, ssl_write_result);
            
            // Attempt SSL_read (should fail or read 0 bytes since no data sent)
            std::vector<uint8_t> read_buffer(4096);
            ssl_read_result = SSL_read(ssl, read_buffer.data(), read_buffer.size());
            ssl_error = SSL_get_error(ssl, ssl_read_result);
        }
    }

    // ==================== STEP 8: CONNECTION TERMINATION ====================
    // Test SSL_shutdown
    ssl_shutdown_result = SSL_shutdown(ssl);
    ssl_error = SSL_get_error(ssl, ssl_shutdown_result);
    
    // Test SSL_shutdown with different modes
    if (fdp.ConsumeBool()) {
        // Try shutdown again (idempotent)
        SSL_shutdown(ssl);
    }

    // ==================== STEP 9: ERROR HANDLING ====================
    // Exercise SSL_get_error with various scenarios
    // Get error for the last operation
    ssl_error = SSL_get_error(ssl, -1);  // -1 for testing error retrieval
    
    // Test SSL_get_error with different return values
    // test_retvals is already declared at line 66
    for (int i = 0; i < 4 && fdp.ConsumeBool(); i++) {
        SSL_get_error(ssl, test_retvals[i]);
    }

cleanup:
    // ==================== STEP 10: CLEANUP ====================
    // Free SSL object
    if (ssl != NULL) {
        SSL_free(ssl);
    }
    
    // Free SSL context
    if (ctx != NULL) {
        SSL_CTX_free(ctx);
    }
    
    // Note: BIOs are freed by SSL_free when using SSL_set_bio
    // If we created BIOs but didn't set them with SSL_set_bio, free them
    if (bio1 != NULL && !use_bio) {
        BIO_free(bio1);
    }
    if (bio2 != NULL && !use_bio) {
        BIO_free(bio2);
    }
    
    // Clear any remaining OpenSSL errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_009
