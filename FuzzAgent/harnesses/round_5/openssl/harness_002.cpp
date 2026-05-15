#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <openssl/srp.h>

namespace harness_002 {
/*
 * Fuzzing harness for OpenSSL targeting SSL/TLS Secure Remote Password (SRP) authentication
 * This harness tests: SSL context initialization, SRP context setup, and SRP parameter configuration
 * Primary target: SSL_SRP_CTX_init with 184 undiscovered branches
 * Secondary target: SSL_set_srp_server_param for SRP parameter configuration
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize OpenSSL SSL library
    // Note: SSL_library_init() is a macro that expands to OPENSSL_init_ssl(0, NULL)
    OPENSSL_init_ssl(0, NULL);
    ERR_clear_error();

    // Need minimum input size for meaningful SRP operations
    // We need at least: 4 ints for BIGNUM sizes + some data for BIGNUMs and strings
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    SSL_CTX* ctx = NULL;
    SSL* ssl = NULL;
    BIGNUM* N = NULL;
    BIGNUM* g = NULL;
    BIGNUM* sa = NULL;
    BIGNUM* v = NULL;
    char* info = NULL;
    int srp_init_result = 0;
    
    // Declare variables that will be initialized later
    std::string info_str;
    std::vector<uint8_t> N_data, g_data, sa_data, v_data;
    size_t N_size = 0, g_size = 0, sa_size = 0, v_size = 0;
    // ==================== SSL CONTEXT AND CONNECTION SETUP ====================
    // 1. Create SSL context with TLS method
    ctx = SSL_CTX_new(TLS_method());
    if (ctx == NULL) {
        goto cleanup;
    }

    // 2. Create SSL connection structure
    ssl = SSL_new(ctx);
    if (ssl == NULL) {
        goto cleanup;
    }

    // ==================== SRP CONTEXT INITIALIZATION ====================
    // 3. Initialize SRP context (PRIMARY TARGET: SSL_SRP_CTX_init)
    srp_init_result = SSL_SRP_CTX_init(ssl);
    // Note: SSL_SRP_CTX_init returns 1 on success, 0 on failure
    // We continue even if it fails to explore error paths

    // ==================== SRP PARAMETER CONFIGURATION ====================
    // 4. Set SRP server parameters using fuzzer input
    
    // Create BIGNUMs from fuzzer data for SRP parameters
    // Consume data for BIGNUM N (modulus)
    N_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    N_data = fdp.ConsumeBytes<uint8_t>(N_size);
    if (N_data.size() > 0) {
        N = BN_bin2bn(N_data.data(), N_data.size(), NULL);
    }
    
    // Consume data for BIGNUM g (generator)
    g_size = fdp.ConsumeIntegralInRange<size_t>(1, 64);
    g_data = fdp.ConsumeBytes<uint8_t>(g_size);
    if (g_data.size() > 0) {
        g = BN_bin2bn(g_data.data(), g_data.size(), NULL);
    }
    
    // Consume data for BIGNUM sa (salt)
    sa_size = fdp.ConsumeIntegralInRange<size_t>(1, 32);
    sa_data = fdp.ConsumeBytes<uint8_t>(sa_size);
    if (sa_data.size() > 0) {
        sa = BN_bin2bn(sa_data.data(), sa_data.size(), NULL);
    }
    
    // Consume data for BIGNUM v (verifier)
    v_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    v_data = fdp.ConsumeBytes<uint8_t>(v_size);
    if (v_data.size() > 0) {
        v = BN_bin2bn(v_data.data(), v_data.size(), NULL);
    }
    
    // Consume string for info parameter
    info_str = fdp.ConsumeRandomLengthString(64);
    if (!info_str.empty()) {
        info = strdup(info_str.c_str());
    }

    // 5. Call SSL_set_srp_server_param with generated parameters
    // This is the secondary target function
    if (N != NULL && g != NULL && sa != NULL && v != NULL && info != NULL) {
        int set_param_result = SSL_set_srp_server_param(ssl, N, g, sa, v, info);
        // Result is 1 on success, 0 on failure - we don't need to check
    }

    // ==================== OPTIONAL SRP OPERATIONS ====================
    // Try to get SRP parameters back (if available)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        BIGNUM* retrieved_g = SSL_get_srp_g(ssl);
        BIGNUM* retrieved_N = SSL_get_srp_N(ssl);
        char* retrieved_username = SSL_get_srp_username(ssl);
        char* retrieved_userinfo = SSL_get_srp_userinfo(ssl);
        
        // Note: SSL_get_srp_g and SSL_get_srp_N return pointers to internal BIGNUMs that shouldn't be freed
        // SSL_get_srp_username and SSL_get_srp_userinfo return pointers to internal SSL structure data
        // that should NOT be freed by the caller (they will be freed by SSL_free())
    }

cleanup:
    // ==================== CLEANUP ====================
    // Clean up BIGNUMs
    if (N != NULL) {
        BN_free(N);
    }
    if (g != NULL) {
        BN_free(g);
    }
    if (sa != NULL) {
        BN_free(sa);
    }
    if (v != NULL) {
        BN_free(v);
    }
    
    // Clean up string
    if (info != NULL) {
        free(info);
    }
    
    // Clean up SSL and SSL_CTX
    if (ssl != NULL) {
        SSL_free(ssl);
    }
    if (ctx != NULL) {
        SSL_CTX_free(ctx);
    }
    
    // Global cleanup
    OPENSSL_cleanup();
    
    return 0;
}

} // namespace harness_002
