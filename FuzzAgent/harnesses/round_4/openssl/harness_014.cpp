#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <unistd.h>

namespace harness_014 {
/*
 * Fuzzing harness for OpenSSL SSL socket operations
 * Primary target: SSL_set_rfd, SSL_set_wfd, SSL_set_fd with 0% coverage and 1000+ undiscovered branches
 * Coverage goal: Exercise SSL socket file descriptor lifecycle for network I/O functionality
 * 
 * Target APIs:
 * - SSL_set_rfd (Target: SSL socket read file descriptor setup, 0% coverage)
 * - SSL_set_wfd (Target: SSL socket write file descriptor setup, 0% coverage) 
 * - SSL_set_fd (Target: SSL socket general file descriptor setup, 0% coverage)
 * - SSL_new (Required Helper: SSL context creation)
 * - SSL_free (Required Helper: SSL context cleanup)
 * - SSL_set_connect_state (Required Helper: Set client mode)
 * - SSL_set_accept_state (Required Helper: Set server mode)
 * - SSL_set_bio (Required Helper: BIO attachment)
 * - SSL_get_rfd (Complementary getter)
 * - SSL_get_wfd (Complementary getter)
 * - SSL_get_fd (Complementary getter)
 * 
 * Invocation sequence:
 * 1. Call SSL_new to create SSL context.
 * 2. Call SSL_set_connect_state or SSL_set_accept_state to set mode.
 * 3. Create simulated socket file descriptors using pipe() for local testing.
 * 4. Call SSL_set_fd to set socket descriptor.
 * 5. Call SSL_set_rfd and SSL_set_wfd for separate read/write descriptors.
 * 6. Call SSL_set_bio to attach BIO objects.
 * 7. Test SSL_get_fd, SSL_get_rfd, SSL_get_wfd getters.
 * 8. Call SSL_free to clean up.
 * 
 * Alternative test flows:
 * - Test with different SSL methods (TLS_method, TLS_client_method, TLS_server_method)
 * - Test error paths with invalid file descriptors
 * - Test with different socket BIO configurations
 * 
 * Differentiation from existing harnesses:
 * - harness_000.cpp: AES cryptographic operations
 * - harness_001.cpp: RSA cryptographic operations
 * - harness_002.cpp: SSL/TLS protocol stack with memory BIOs (NOT socket FDs)
 * - harness_003.cpp: CMP/CRMF certificate management
 * - harness_004.cpp: X.509 certificate parsing and CMS operations
 * - harness_005.cpp: PKCS12 parsing and encryption
 * - harness_006.cpp: DES and 3DES cryptographic operations
 * - harness_007.cpp: EC (Elliptic Curve) cryptographic operations
 * - harness_008.cpp: PEM format parsing of cryptographic keys
 * - harness_009.cpp: Time-Stamp Protocol (RFC 3161) operations
 * - harness_010.cpp: Hash functions (SHA family)
 * - harness_011.cpp: DH key exchange and DSA digital signatures
 * - harness_012.cpp: HPKE (Hybrid Public Key Encryption) RFC 9180
 * - harness_013.cpp: PKCS7 S/MIME operations
 * - harness_014.cpp: SSL socket file descriptor operations (NEW - targets 0% covered APIs)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for operation selection and basic parameters
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL SSL library
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    
    // Consume SSL method type based on input
    uint8_t method_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    const SSL_METHOD* method = nullptr;
    
    switch (method_choice) {
        case 0:
            method = TLS_method();           // Generic TLS method
            break;
        case 1:
            method = TLS_client_method();    // Client method
            break;
        case 2:
            method = TLS_server_method();    // Server method
            break;
        default:
            method = TLS_method();
            break;
    }
    
    if (method == nullptr) {
        return 0;
    }
    
    // Create SSL context
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (ctx == nullptr) {
        return 0;
    }
    
    // Set reasonable TLS version bounds for testing
    SSL_CTX_set_min_proto_version(ctx, TLS1_VERSION);
    SSL_CTX_set_max_proto_version(ctx, TLS1_3_VERSION);
    
    // Create SSL object
    SSL* ssl = SSL_new(ctx);
    if (ssl == nullptr) {
        SSL_CTX_free(ctx);
        return 0;
    }
    
    // Consume operation mode: which socket functions to test
    // 0 = Test SSL_set_fd with combined read/write
    // 1 = Test SSL_set_rfd and SSL_set_wfd separately
    // 2 = Test SSL_set_bio after setting descriptors
    // 3 = Test error paths with invalid descriptors
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Create pipe file descriptors to simulate sockets for testing
    int pipe_fds[2];
    if (pipe(pipe_fds) != 0) {
        // If pipe fails, we can still test some error paths
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return 0;
    }
    
    int read_fd = pipe_fds[0];
    int write_fd = pipe_fds[1];
    
    // Consume role: client or server mode
    uint8_t role = fdp.ConsumeIntegral<uint8_t>() % 2;
    if (role == 0) {
        SSL_set_connect_state(ssl);  // Client mode
    } else {
        SSL_set_accept_state(ssl);   // Server mode
    }
    
    int test_result = 0;
    
    switch (operation_mode) {
        case 0: {
            // Test SSL_set_fd for combined read/write descriptor
            // Use write_fd as the combined descriptor for testing
            test_result = SSL_set_fd(ssl, write_fd);
            
            // Test getters
            int retrieved_fd = SSL_get_fd(ssl);
            int retrieved_rfd = SSL_get_rfd(ssl);
            int retrieved_wfd = SSL_get_wfd(ssl);
            
            // The getters should return valid descriptors or -1
            // We don't assert specific values since implementation may vary
            (void)retrieved_fd;
            (void)retrieved_rfd;
            (void)retrieved_wfd;
            break;
        }
        
        case 1: {
            // Test SSL_set_rfd and SSL_set_wfd separately
            int rfd_result = SSL_set_rfd(ssl, read_fd);
            int wfd_result = SSL_set_wfd(ssl, write_fd);
            
            test_result = (rfd_result == 1 && wfd_result == 1) ? 1 : 0;
            
            // Test getters for separate descriptors
            int retrieved_rfd = SSL_get_rfd(ssl);
            int retrieved_wfd = SSL_get_wfd(ssl);
            
            (void)retrieved_rfd;
            (void)retrieved_wfd;
            break;
        }
        
        case 2: {
            // Test SSL_set_bio with manually created BIOs
            // First set file descriptors
            if (SSL_set_fd(ssl, write_fd) != 1) {
                break;
            }
            
            // Get the existing BIOs
            BIO* rbio = SSL_get_rbio(ssl);
            BIO* wbio = SSL_get_wbio(ssl);
            
            // Test setting BIOs explicitly (should be same as current)
            SSL_set_bio(ssl, rbio, wbio);
            
            test_result = 1;
            break;
        }
        
        case 3: {
            // Test error paths with invalid descriptors
            int invalid_fd = -1;
            
            // Try setting invalid file descriptors
            int invalid_result = SSL_set_fd(ssl, invalid_fd);
            
            // These should fail but exercise error handling code
            (void)invalid_result;
            
            // Also test with closed descriptors
            close(read_fd);
            close(write_fd);
            
            int closed_result = SSL_set_fd(ssl, read_fd);  // Already closed
            
            (void)closed_result;
            
            // Re-create pipes for cleanup (to avoid double-close)
            pipe_fds[0] = -1;
            pipe_fds[1] = -1;
            read_fd = -1;
            write_fd = -1;
            
            test_result = 1;  // Consider error path testing successful
            break;
        }
    }
    
    // Clean up file descriptors if they're still valid
    if (read_fd >= 0) {
        close(read_fd);
    }
    if (write_fd >= 0) {
        close(write_fd);
    }
    
    // Clean up SSL resources
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    
    // Return 0 regardless of test outcomes - fuzzer should continue
    return 0;
}

} // namespace harness_014
