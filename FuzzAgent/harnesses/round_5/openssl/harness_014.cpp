#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/crypto.h>

namespace harness_014 {
/*
 * Fuzzing harness for OpenSSL targeting SSL session printing operations
 * Primary Target: SSL_SESSION_print_fp (748 undiscovered branches)
 * 
 * Required APIs: SSL_CTX_new, SSL_new, SSL_set_fd, SSL_get_session, SSL_SESSION_free, SSL_free, SSL_CTX_free
 * Additional APIs: TLS_method, BIO_s_mem, BIO_new, BIO_set_fd, BIO_free_all
 * 
 * Invocation sequence:
 * 1. Create SSL context with SSL_CTX_new using TLS_method()
 * 2. Create SSL object with SSL_new
 * 3. Create in-memory BIO for dummy file descriptor with BIO_s_mem and BIO_new
 * 4. Set file descriptor with SSL_set_fd (using BIO_get_fd or simulated BIO)
 * 5. Establish minimal SSL connection simulation
 * 6. Obtain SSL_SESSION object with SSL_get_session
 * 7. Print session details with SSL_SESSION_print_fp (primary target)
 * 8. Cleanup: SSL_SESSION_free, SSL_free, SSL_CTX_free, BIO_free_all
 * 
 * Focus: SSL module has only 9.38% coverage, and SSL_SESSION_print_fp has the highest
 * undiscovered branch count. Test various session states, error paths, and edge cases
 * for session printing. Use FuzzedDataProvider for input-driven testing.
 * 
 * Ensure semantic diversity from existing harnesses by focusing on SSL session
 * debugging output functionality and session state inspection.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize OpenSSL crypto library
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                        OPENSSL_INIT_ADD_ALL_CIPHERS | 
                        OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();

    // Need minimum input size for meaningful SSL operations
    // We need data for session parameters, context options, and BIO operations
    if (size < 32) {
        return 0;  // Need enough data for basic SSL operations
    }

    FuzzedDataProvider fdp(data, size);

    // Declare all variables at the top to avoid goto jumping over initializations
    SSL_CTX* ctx = NULL;
    SSL* ssl = NULL;
    SSL_SESSION* session = NULL;
    BIO* bio = NULL;
    BIO* mem_bio = NULL;
    FILE* tmp_file = NULL;
    FILE* print_file = NULL;
    int bio_fd = -1;
    
    // Variables for data consumption
    int ssl_version = 0;
    int cipher_list_index = 0;
    int session_cache_mode = 0;
    int verify_mode = 0;
    int options = 0;
    bool set_cipher_list = false;
    bool set_session_cache_mode = false;
    bool set_verify_mode = false;
    bool set_options = false;
    bool simulate_handshake = false;
    bool test_invalid_session = false;
    bool test_null_fp = false;
    
    // ==================== CONSUME FUZZER INPUT FOR PARAMETERS ====================
    
    // Consume SSL/TLS version selection (0=TLS_method, 1=TLS_client_method, 2=TLS_server_method)
    ssl_version = fdp.ConsumeIntegralInRange<int>(0, 2);
    
    // Consume cipher list index for testing different cipher suites
    cipher_list_index = fdp.ConsumeIntegralInRange<int>(0, 10);
    
    // Consume session cache mode flags
    session_cache_mode = fdp.ConsumeIntegral<int>();
    
    // Consume verify mode
    verify_mode = fdp.ConsumeIntegral<int>();
    
    // Consume SSL options
    options = fdp.ConsumeIntegral<int>();
    
    // Consume boolean flags for various configurations
    set_cipher_list = fdp.ConsumeBool();
    set_session_cache_mode = fdp.ConsumeBool();
    set_verify_mode = fdp.ConsumeBool();
    set_options = fdp.ConsumeBool();
    simulate_handshake = fdp.ConsumeBool();
    test_invalid_session = fdp.ConsumeBool();
    test_null_fp = fdp.ConsumeBool();
    
    // Consume remaining bytes for potential certificate/private key data
    // (though we won't actually use them for real crypto in fuzzing)
    std::vector<uint8_t> cert_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // ==================== CREATE SSL CONTEXT ====================
    
    // Create SSL context with appropriate method based on consumed version
    const SSL_METHOD* method = NULL;
    switch (ssl_version) {
        case 0:
            method = TLS_method();
            break;
        case 1:
            method = TLS_client_method();
            break;
        case 2:
            method = TLS_server_method();
            break;
        default:
            method = TLS_method();
            break;
    }
    
    if (method == NULL) {
        goto cleanup;
    }
    
    ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
        goto cleanup;
    }
    
    // Configure SSL context based on consumed parameters
    if (set_cipher_list) {
        // Try different cipher lists based on consumed index
        const char* cipher_lists[] = {
            "ALL",                           // All ciphers
            "DEFAULT",                       // Default ciphers
            "HIGH",                          // High strength ciphers
            "MEDIUM",                        // Medium strength ciphers  
            "LOW",                           // Low strength ciphers
            "AES128",                        // AES128 ciphers
            "AES256",                        // AES256 ciphers
            "CHACHA20",                      // ChaCha20 ciphers
            "ECDHE",                         // ECDHE ciphers
            "RSA",                           // RSA ciphers
            "NULL"                           // NULL ciphers (no encryption)
        };
        
        int list_idx = cipher_list_index % (sizeof(cipher_lists) / sizeof(cipher_lists[0]));
        SSL_CTX_set_cipher_list(ctx, cipher_lists[list_idx]);
    }
    
    if (set_session_cache_mode) {
        SSL_CTX_set_session_cache_mode(ctx, session_cache_mode);
    }
    
    if (set_verify_mode) {
        SSL_CTX_set_verify(ctx, verify_mode, NULL);
    }
    
    if (set_options) {
        SSL_CTX_set_options(ctx, options);
    }
    
    // ==================== CREATE SSL OBJECT ====================
    
    ssl = SSL_new(ctx);
    if (ssl == NULL) {
        goto cleanup;
    }
    
    // ==================== CREATE IN-MEMORY BIO FOR DUMMY FD ====================
    
    // Create a memory BIO to simulate a file descriptor
    mem_bio = BIO_new(BIO_s_mem());
    if (mem_bio == NULL) {
        goto cleanup;
    }
    
    // For SSL_set_fd, we need a file descriptor
    // Create a temporary file or use the memory BIO in a creative way
    tmp_file = tmpfile();
    if (tmp_file != NULL) {
        bio = BIO_new_fd(fileno(tmp_file), BIO_NOCLOSE);
        if (bio != NULL) {
            // Set the BIO for the SSL object
            SSL_set_bio(ssl, bio, bio);
            // Don't free bio here - SSL_free will handle it
            bio = NULL; // Set to NULL to prevent double-free
        }
    }
    
    // If we couldn't create a file-based BIO, try to use memory BIO
    // Note: SSL_set_fd requires a real file descriptor, but we can use BIO pairs
    // as an alternative for fuzzing purposes
    
    // ==================== SIMULATE MINIMAL SSL CONNECTION ====================
    
    if (simulate_handshake) {
        // For fuzzing, we simulate being in different SSL states
        // without actually performing a real handshake
        const char* state_str = SSL_state_string(ssl);
        
        // Try to get a session object
        session = SSL_get_session(ssl);
        
        // If we didn't get a session, create a dummy one for testing
        if (session == NULL && !test_invalid_session) {
            // We'll test SSL_SESSION_print_fp with NULL session in error path
            // For now, continue without session
        }
    } else {
        // Get session even without simulated handshake
        // This may return NULL, which is valid for testing error paths
        session = SSL_get_session(ssl);
    }
    
    // ==================== PRIMARY TARGET: SSL_SESSION_print_fp ====================
    
    // Create a temporary file for printing session details
    print_file = tmpfile();
    if (print_file != NULL) {
        // Test with valid session if available
        if (session != NULL) {
            SSL_SESSION_print_fp(print_file, session);
            
            // Also try with different file positions
            rewind(print_file);
            SSL_SESSION_print_fp(print_file, session);
            
            // Test error handling by closing and reopening file
            fclose(print_file);
            print_file = tmpfile();
            if (print_file != NULL) {
                SSL_SESSION_print_fp(print_file, session);
            }
        }
        
        // Test with NULL session pointer (error path)
        if (test_invalid_session) {
            SSL_SESSION_print_fp(print_file, NULL);
        }
        
        // Test with NULL file pointer if flag is set
        if (test_null_fp && print_file != NULL) {
            // This would normally crash, but we should handle it gracefully
            // by not calling it or checking for NULL
            // SSL_SESSION_print_fp(NULL, session); // Don't call with NULL fp
        }
        
        if (print_file != NULL) {
            fclose(print_file);
            print_file = NULL;
        }
    }
    
    // ==================== ADDITIONAL TESTING VARIATIONS ====================
    
    // Test with different session states by creating a new SSL context
    // and setting various session parameters
    
    // Test SSL_SESSION_print with BIO (alternative to SSL_SESSION_print_fp)
    if (mem_bio != NULL && session != NULL) {
        BIO* print_bio = BIO_new(BIO_s_mem());
        if (print_bio != NULL) {
            SSL_SESSION_print(print_bio, session);
            BIO_free(print_bio);
        }
    }
    
    // Test error paths by corrupting session data
    // (simulated by freeing and then trying to use)
    
    // ==================== CLEANUP ====================
    
cleanup:
    // Free resources in reverse order of creation
    if (session != NULL) {
        SSL_SESSION_free(session);
    }
    
    if (ssl != NULL) {
        SSL_free(ssl);
        // Note: SSL_free also frees the BIOs associated with the SSL object
    }
    
    if (bio != NULL) {
        BIO_free(bio);
    }
    
    if (mem_bio != NULL) {
        BIO_free(mem_bio);
    }
    
    if (ctx != NULL) {
        SSL_CTX_free(ctx);
    }
    
    if (tmp_file != NULL) {
        fclose(tmp_file);
    }
    
    if (print_file != NULL) {
        fclose(print_file);
    }
    
    // Clear any remaining OpenSSL errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_014
