#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/provider.h>
#include <openssl/core.h>
#include <openssl/evp.h>
#include <openssl/params.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace harness_003 {
/*
 * Fuzzing harness for OpenSSL Provider Infrastructure
 * Targets: Provider management APIs with near-zero coverage (5.64%)
 * Coverage goal: Uncover provider module (42,479 lines with only 5.64% coverage)
 * Semantic differentiation: Provider architecture testing (vs. crypto basics, SSL/TLS, and OCSP in previous harnesses)
 * 
 * APIs targeted:
 * - OSSL_PROVIDER_load / OSSL_PROVIDER_unload - Provider lifecycle management
 * - OSSL_PROVIDER_query_operation - Algorithm discovery
 * - OSSL_PROVIDER_get_params - Provider configuration retrieval
 * - OSSL_PROVIDER_gettable_params - Parameter discovery
 * - OSSL_PROVIDER_available - Provider availability checking
 * - OSSL_PROVIDER_get0_provider_ctx - Context access
 * - Provider-specific operations via algorithm dispatch
 */


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 32) {
        return 0; // Need minimum input for meaningful provider testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                       OPENSSL_INIT_ADD_ALL_CIPHERS | 
                       OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();
    
    // Determine which provider(s) to test based on first byte
    uint8_t provider_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Create a library context for provider testing
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    if (libctx == NULL) {
        return 0;
    }
    
    // Select provider name based on fuzzed input
    const char *provider_name = NULL;
    switch (provider_choice) {
        case 0: provider_name = "default"; break;
        case 1: provider_name = "base"; break;
        case 2: provider_name = "legacy"; break;
        case 3: provider_name = "fips"; break;
        default: provider_name = "default"; break;
    }
    
    // Check if provider is available before attempting to load
    int is_available = OSSL_PROVIDER_available(libctx, provider_name);
    
    // Attempt to load the provider
    OSSL_PROVIDER *provider = OSSL_PROVIDER_load(libctx, provider_name);
    
    if (provider == NULL) {
        // Provider failed to load, clean up and exit
        OSSL_LIB_CTX_free(libctx);
        return 0;
    }
    
    // Test provider parameter discovery and retrieval
    const OSSL_PARAM *gettable_params = OSSL_PROVIDER_gettable_params(provider);
    
    if (gettable_params != NULL) {
        // Try to get some parameters from the provider
        // Consume fuzzed data to create parameter requests
        size_t param_count = 0;
        while (gettable_params[param_count].key != NULL) {
            param_count++;
            if (param_count > 10) break; // Limit to reasonable number
        }
        
        if (param_count > 0) {
            // Allocate parameter array for get_params
            OSSL_PARAM *params = (OSSL_PARAM *)OPENSSL_malloc(sizeof(OSSL_PARAM) * (param_count + 1));
            if (params != NULL) {
                for (size_t i = 0; i < param_count; i++) {
                    params[i].key = gettable_params[i].key;
                    params[i].data_type = gettable_params[i].data_type;
                    params[i].data = NULL;
                    params[i].data_size = 0;
                    params[i].return_size = 0;
                }
                params[param_count].key = NULL;
                
                // Attempt to get parameters
                int get_result = OSSL_PROVIDER_get_params(provider, params);
                
                // Clean up parameter array
                OPENSSL_free(params);
            }
        }
    }
    
    // Test provider operation querying
    // Common operation IDs from OpenSSL (simplified set)
    int operation_ids[] = {
        1,  // OSSL_OP_DIGEST
        2,  // OSSL_OP_CIPHER  
        3,  // OSSL_OP_MAC
        4,  // OSSL_OP_KDF
        5,  // OSSL_OP_KEYMGMT
        6,  // OSSL_OP_KEYEXCH
        7,  // OSSL_OP_SIGNATURE
        8,  // OSSL_OP_ASYM_CIPHER
        9,  // OSSL_OP_KEM
        10, // OSSL_OP_ENCODER
        11, // OSSL_OP_DECODER
        12, // OSSL_OP_STORE
        13, // OSSL_OP_RAND
    };
    
    size_t num_ops = sizeof(operation_ids) / sizeof(operation_ids[0]);
    
    // Query a subset of operations based on fuzzed input
    size_t ops_to_query = fdp.ConsumeIntegralInRange<size_t>(1, num_ops > 5 ? 5 : num_ops);
    
    for (size_t i = 0; i < ops_to_query; i++) {
        // Select operation ID based on fuzzed input
        size_t op_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_ops - 1);
        int operation_id = operation_ids[op_idx];
        int no_cache = 0;
        
        // Query available algorithms for this operation
        const OSSL_ALGORITHM *algs = OSSL_PROVIDER_query_operation(provider, operation_id, &no_cache);
        
        if (algs != NULL) {
            // Count available algorithms
            size_t alg_count = 0;
            while (algs[alg_count].algorithm_names != NULL) {
                alg_count++;
                if (alg_count > 20) break; // Limit scanning
            }
            
            // Unquery the operation (cleanup)
            OSSL_PROVIDER_unquery_operation(provider, operation_id, algs);
        }
    }
    
    // Test provider context access
    void *provider_ctx = OSSL_PROVIDER_get0_provider_ctx(provider);
    // Note: provider_ctx is opaque, we can't do much with it
    
    // Test provider capabilities (if supported)
    // This would require a callback function, simplified for fuzzing
    
    // Test algorithm fetching through EVP interface to exercise provider dispatch
    // Consume algorithm name from fuzzed input
    size_t alg_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
    std::string alg_name = fdp.ConsumeBytesAsString(alg_name_len);
    
    if (!alg_name.empty()) {
        // Try to fetch different types of algorithms
        EVP_MD *digest = EVP_MD_fetch(libctx, alg_name.c_str(), NULL);
        if (digest != NULL) {
            EVP_MD_free(digest);
        }
        
        EVP_CIPHER *cipher = EVP_CIPHER_fetch(libctx, alg_name.c_str(), NULL);
        if (cipher != NULL) {
            EVP_CIPHER_free(cipher);
        }
    }
    
    // Test with algorithm parameters
    // Consume parameter data from fuzzed input
    size_t param_data_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::vector<uint8_t> param_data = fdp.ConsumeBytes<uint8_t>(param_data_len);
    
    if (param_data.size() > 0) {
        // Consume properties string from fuzzed input
        size_t props_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::string props_str = fdp.ConsumeBytesAsString(props_len);
        const char *props = props_str.c_str();
        
        // Consume algorithm name from fuzzed input for property-based fetch
        size_t prop_alg_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
        std::string prop_alg_name = fdp.ConsumeBytesAsString(prop_alg_name_len);
        
        if (!prop_alg_name.empty()) {
            EVP_MD *digest_with_props = EVP_MD_fetch(libctx, prop_alg_name.c_str(), props);
            if (digest_with_props != NULL) {
                EVP_MD_free(digest_with_props);
            }
            
            EVP_CIPHER *cipher_with_props = EVP_CIPHER_fetch(libctx, prop_alg_name.c_str(), props);
            if (cipher_with_props != NULL) {
                EVP_CIPHER_free(cipher_with_props);
            }
        }
    }
    
    // Test provider self-test if available
    // OSSL_PROVIDER_self_test(provider); // May trigger actual self-tests
    
    // Test provider configuration parameters
    // Consume configuration parameter name and value
    size_t conf_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
    size_t conf_value_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
    std::string conf_name = fdp.ConsumeBytesAsString(conf_name_len);
    std::string conf_value = fdp.ConsumeBytesAsString(conf_value_len);
    
    if (!conf_name.empty() && !conf_value.empty()) {
        // Note: OSSL_PROVIDER_add_conf_parameter requires provider not in use
        // by multiple threads, which is true in our single-threaded fuzzer context
        OSSL_PROVIDER_add_conf_parameter((OSSL_PROVIDER *)provider, conf_name.c_str(), conf_value.c_str());
        
        // Try to retrieve configuration parameters
        OSSL_PARAM conf_params[2];
        char conf_value_buf[128];
        
        conf_params[0].key = conf_name.c_str();
        conf_params[0].data_type = OSSL_PARAM_UTF8_PTR;
        conf_params[0].data = conf_value_buf;
        conf_params[0].data_size = sizeof(conf_value_buf);
        conf_params[0].return_size = 0;
        
        conf_params[1].key = NULL;
        
        OSSL_PROVIDER_get_conf_parameters(provider, conf_params);
    }
    
    // Test provider name retrieval
    const char *retrieved_name = OSSL_PROVIDER_get0_name(provider);
    
    // Test provider dispatch table access (advanced)
    const OSSL_DISPATCH *dispatch = OSSL_PROVIDER_get0_dispatch(provider);
    // dispatch is opaque array of function pointers
    
    // Clean up - unload provider
    OSSL_PROVIDER_unload(provider);
    
    // Also test loading/unloading multiple providers in sequence
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 16) {
        // Try loading a second provider
        const char *second_provider_name = "null"; // Try null provider
        
        OSSL_PROVIDER *provider2 = OSSL_PROVIDER_load(libctx, second_provider_name);
        if (provider2 != NULL) {
            // Quick parameter test
            const OSSL_PARAM *params2 = OSSL_PROVIDER_gettable_params(provider2);
            OSSL_PROVIDER_unload(provider2);
        }
    }
    
    // Test provider iteration if supported
    // OSSL_PROVIDER_do_all requires callback, simplified for fuzzing
    
    // Clean up library context
    OSSL_LIB_CTX_free(libctx);
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_003
