#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/dh.h>
#include <openssl/dsa.h>
#include <openssl/ec.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace harness_013 {
/*
 * Fuzzing harness for OpenSSL PEM key parameter operations
 * Targets: PEM_read_bio_Parameters (1391+ undiscovered branches), PEM parameter parsing,
 *          key parameter extraction, and related PEM operations for cryptographic parameters
 * Specifically targets PEM-encoded key parameter parsing for DH, DSA, EC parameters
 * This harness focuses on PEM parameter parsing specifically, differentiating from:
 * - harness_003: PEM/X509 certificate handling (private keys and certificates)
 * - harness_012: Direct DH operations (not PEM parsing)
 * Coverage target: PEM parameter parsing infrastructure with various parameter formats
 */


// Initialize OpenSSL once
static bool openssl_initialized = false;

static void initialize_openssl() {
    if (!openssl_initialized) {
        OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                           OPENSSL_INIT_ADD_ALL_CIPHERS |
                           OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
        ERR_clear_error();
        openssl_initialized = true;
    }
}

// Test PEM parameter parsing with various parameter types
static void test_pem_parameters_parsing(FuzzedDataProvider& fdp, const std::vector<uint8_t>& pem_data) {
    if (pem_data.empty()) {
        return;
    }
    
    BIO* bio = BIO_new_mem_buf(pem_data.data(), pem_data.size());
    if (bio == NULL) {
        return;
    }
    
    // Clear any previous errors
    ERR_clear_error();
    
    // Test PEM_read_bio_Parameters - primary target
    EVP_PKEY* params = NULL;
    
    // Try the basic version
    params = PEM_read_bio_Parameters(bio, NULL);
    if (params != NULL) {
        // Test parameter properties if we got valid parameters
        int key_type = EVP_PKEY_get_id(params);
        
        // Try to get parameter bits
        int param_bits = EVP_PKEY_bits(params);
        (void)param_bits; // Use result to avoid unused variable warning
        
        // Test parameter validation based on type
        switch (key_type) {
            case EVP_PKEY_DH:
            case EVP_PKEY_DHX: {
                const DH* dh = EVP_PKEY_get0_DH(params);
                if (dh != NULL) {
                    // Test DH parameter validation
                    int codes = 0;
                    DH_check(dh, &codes);
                    (void)codes; // Use to avoid warnings
                    
                    // Test DH parameter bits
                    int dh_bits = DH_bits(dh);
                    (void)dh_bits;
                }
                break;
            }
            case EVP_PKEY_DSA: {
                const DSA* dsa = EVP_PKEY_get0_DSA(params);
                if (dsa != NULL) {
                    // Test DSA parameter access (no DSA_check function in OpenSSL 3.x)
                    const BIGNUM* p = DSA_get0_p(dsa);
                    const BIGNUM* q = DSA_get0_q(dsa);
                    const BIGNUM* g = DSA_get0_g(dsa);
                    (void)p; (void)q; (void)g; // Use to avoid warnings
                    
                    // Test DSA parameter bits (DSA_bits may not exist, use alternative)
                    int dsa_bits = 0;
                    if (p != NULL) {
                        dsa_bits = BN_num_bits(p);
                    }
                    (void)dsa_bits;
                }
                break;
            }
            case EVP_PKEY_EC: {
                const EC_KEY* ec = EVP_PKEY_get0_EC_KEY(params);
                if (ec != NULL) {
                    // Test EC parameter validation
                    const EC_GROUP* group = EC_KEY_get0_group(ec);
                    if (group != NULL) {
                        int degree = EC_GROUP_get_degree(group);
                        (void)degree;
                        
                        // Test curve name
                        int nid = EC_GROUP_get_curve_name(group);
                        (void)nid;
                    }
                }
                break;
            }
            default:
                // Unknown parameter type - still test basic operations
                break;
        }
        
        // Try to write parameters back to BIO for round-trip testing
        BIO* out_bio = BIO_new(BIO_s_mem());
        if (out_bio != NULL) {
            PEM_write_bio_Parameters(out_bio, params);
            BIO_free(out_bio);
        }
        
        EVP_PKEY_free(params);
        params = NULL;
    }
    
    // Reset BIO position for next test
    BIO_reset(bio);
    
    // Test PEM_read_bio_Parameters_ex with library context
    params = PEM_read_bio_Parameters_ex(bio, NULL, NULL, NULL);
    if (params != NULL) {
        EVP_PKEY_free(params);
        params = NULL;
    }
    
    // Reset BIO position again
    BIO_reset(bio);
    
    // Test with non-NULL output parameter
    EVP_PKEY* out_params = NULL;
    params = PEM_read_bio_Parameters(bio, &out_params);
    if (params != NULL) {
        EVP_PKEY_free(params);
    }
    if (out_params != NULL) {
        EVP_PKEY_free(out_params);
    }
    
    BIO_free(bio);
}

// Test with artificially constructed PEM data for different parameter types
static void test_parameter_type_specific(FuzzedDataProvider& fdp, int param_type) {
    // Create a simple BIO with test data
    BIO* bio = BIO_new(BIO_s_mem());
    if (bio == NULL) {
        return;
    }
    
    // Write different PEM headers based on parameter type
    switch (param_type) {
        case 0: // DH parameters
            BIO_printf(bio, "-----BEGIN DH PARAMETERS-----\n");
            break;
        case 1: // DSA parameters  
            BIO_printf(bio, "-----BEGIN DSA PARAMETERS-----\n");
            break;
        case 2: // EC parameters
            BIO_printf(bio, "-----BEGIN EC PARAMETERS-----\n");
            break;
        case 3: // Generic parameters
            BIO_printf(bio, "-----BEGIN PARAMETERS-----\n");
            break;
        default:
            BIO_free(bio);
            return;
    }
    
    // Add some base64-encoded data from fuzzer input
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(10, 200);
    std::vector<uint8_t> b64_data = fdp.ConsumeBytes<uint8_t>(data_size);
    
    // Write the data in base64-like format (not actually base64, but that's okay for fuzzing)
    for (size_t i = 0; i < b64_data.size(); i++) {
        char c = 'A' + (b64_data[i] % 26);
        BIO_write(bio, &c, 1);
        if ((i + 1) % 64 == 0) {
            BIO_write(bio, "\n", 1);
        }
    }
    
    if (b64_data.size() % 64 != 0) {
        BIO_write(bio, "\n", 1);
    }
    
    // Write PEM footer
    switch (param_type) {
        case 0:
            BIO_printf(bio, "-----END DH PARAMETERS-----\n");
            break;
        case 1:
            BIO_printf(bio, "-----END DSA PARAMETERS-----\n");
            break;
        case 2:
            BIO_printf(bio, "-----END EC PARAMETERS-----\n");
            break;
        case 3:
            BIO_printf(bio, "-----END PARAMETERS-----\n");
            break;
    }
    
    // Get the buffer contents
    BUF_MEM* buf_ptr;
    BIO_get_mem_ptr(bio, &buf_ptr);
    
    if (buf_ptr != NULL && buf_ptr->data != NULL && buf_ptr->length > 0) {
        std::vector<uint8_t> pem_data(buf_ptr->data, buf_ptr->data + buf_ptr->length);
        test_pem_parameters_parsing(fdp, pem_data);
    }
    
    BIO_free(bio);
}

// Test error cases and edge conditions
static void test_pem_parameter_edge_cases(FuzzedDataProvider& fdp) {
    // Test with empty data
    std::vector<uint8_t> empty_data;
    test_pem_parameters_parsing(fdp, empty_data);
    
    // Test with very small data
    std::vector<uint8_t> small_data = fdp.ConsumeBytes<uint8_t>(10);
    test_pem_parameters_parsing(fdp, small_data);
    
    // Test with malformed PEM data (no headers)
    std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(100);
    test_pem_parameters_parsing(fdp, random_data);
    
    // Test with truncated PEM data
    std::vector<uint8_t> truncated_data = fdp.ConsumeBytes<uint8_t>(50);
    // Add a partial PEM header
    std::string partial_header = "-----BEGIN ";
    truncated_data.insert(truncated_data.begin(), partial_header.begin(), partial_header.end());
    test_pem_parameters_parsing(fdp, truncated_data);
    
    // Test with invalid base64 data in PEM
    std::vector<uint8_t> invalid_b64_data = fdp.ConsumeBytes<uint8_t>(80);
    // Create a PEM-like structure with invalid characters
    std::string pem_template = "-----BEGIN PARAMETERS-----\n"
                               "!!!INVALID@@@BASE64^^^DATA***\n"
                               "-----END PARAMETERS-----\n";
    std::vector<uint8_t> invalid_pem(pem_template.begin(), pem_template.end());
    test_pem_parameters_parsing(fdp, invalid_pem);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Too small for meaningful PEM parameter testing
    }
    
    initialize_openssl();
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data for test configuration
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    size_t max_pem_size = fdp.ConsumeIntegralInRange<size_t>(64, 2048);
    
    // Ensure max_pem_size is at least 64 before using as upper bound
    if (max_pem_size < 64) max_pem_size = 64;
    
    // Split input into multiple parts for different tests
    size_t pem_data_size = fdp.ConsumeIntegralInRange<size_t>(64, max_pem_size);
    // Ensure pem_data_size doesn't exceed remaining bytes
    if (pem_data_size > fdp.remaining_bytes()) pem_data_size = fdp.remaining_bytes();
    std::vector<uint8_t> pem_data = fdp.ConsumeBytes<uint8_t>(pem_data_size);
    
    // Clear any previous errors before starting tests
    ERR_clear_error();
    
    // Select test type based on fuzzer input
    switch (test_type) {
        case 0:
            // Primary target: PEM_read_bio_Parameters with raw fuzzer data
            test_pem_parameters_parsing(fdp, pem_data);
            break;
            
        case 1:
            // Test with DH parameter format
            test_parameter_type_specific(fdp, 0); // DH
            break;
            
        case 2:
            // Test with DSA parameter format
            test_parameter_type_specific(fdp, 1); // DSA
            break;
            
        case 3:
            // Test with EC parameter format
            test_parameter_type_specific(fdp, 2); // EC
            break;
            
        case 4:
            // Test edge cases and error conditions
            test_pem_parameter_edge_cases(fdp);
            break;
            
        case 5:
            // Combined test: multiple parameter types
            test_pem_parameters_parsing(fdp, pem_data);
            test_parameter_type_specific(fdp, fdp.ConsumeIntegral<uint8_t>() % 4);
            break;
    }
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_013
