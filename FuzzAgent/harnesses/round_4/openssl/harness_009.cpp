#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/ts.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/asn1.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <vector>
#include <string>

namespace harness_009 {
/*
 * Fuzzing harness for OpenSSL Time-Stamp Protocol (RFC 3161) operations
 * Targets: TS (Time-Stamp) module operations for timestamp request/response creation and verification
 * Coverage goal: Exercise Time-Stamp Protocol with 2102+ undiscovered branches in TS_CONF_load_certs
 * 
 * Target APIs:
 * - TS_CONF_load_certs (Target: 2102 undiscovered branches, 0% coverage)
 * - TS_REQ_new (Target: create timestamp requests)
 * - TS_RESP_new (Target: create timestamp responses)
 * - TS_RESP_verify (Target: verify timestamp responses)
 * 
 * Required helper APIs:
 * - TS_REQ_free (Cleanup)
 * - TS_RESP_free (Cleanup)
 * - TS_VERIFY_CTX_new (Verification context)
 * - TS_VERIFY_CTX_free (Cleanup)
 * 
 * Invocation sequence:
 * 1. Initialize TS configuration context and load certificates for verification
 * 2. Create timestamp request with message imprint from fuzzer input
 * 3. Create timestamp response (or attempt to parse fuzzer input as TS_RESP)
 * 4. Verify timestamp response using loaded certificates
 * 5. Call cleanup functions to prevent memory leaks
 * 
 * Differentiation from existing harnesses:
 * - harness_000.cpp: AES cryptographic operations
 * - harness_001.cpp: RSA cryptographic operations
 * - harness_002.cpp: SSL/TLS protocol stack
 * - harness_003.cpp: CMP/CRMF certificate management
 * - harness_004.cpp: X.509 certificate parsing and CMS operations
 * - harness_005.cpp: PKCS12 parsing and encryption
 * - harness_006.cpp: DES and 3DES cryptographic operations
 * - harness_007.cpp: EC (Elliptic Curve) cryptographic operations
 * - harness_008.cpp: PEM format parsing of cryptographic keys
 * - harness_009.cpp: Time-Stamp Protocol (RFC 3161) operations (NEW)
 */

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough data for meaningful testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL libraries
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    // 1. Create a simple PEM certificate for TS_CONF_load_certs testing
    // Since TS_CONF_load_certs requires a file, we'll create a minimal PEM certificate
    std::string pem_cert =
        "-----BEGIN CERTIFICATE-----\n"
        "MIICUTCCAfugAwIBAgIBADANBgkqhkiG9w0BAQQFADBXMQswCQYDVQQGEwJDTjEL\n"
        "MAkGA1UECBMCUE4xCzAJBgNVBAcTAkNOMQswCQYDVQQKEwJPTjELMAkGA1UECxMC\n"
        "VU4xFDASBgNVBAMTC0hlcm9uZyBZYW5nMB4XDTk1MDQyNTA2MDAwMFoXDTk1MDQy\n"
        "NjA2MDAwMFowVzELMAkGA1UEBhMCQ04xCzAJBgNVBAgTAlBOMQswCQYDVQQHEwJD\n"
        "TjELMAkGA1UEChMCT04xCzAJBgNVBAoTAlVOMRQwEgYDVQQDEwtIZXJvbmcgWWFu\n"
        "ZzBcMA0GCSqGSIb3DQEBAQUAA0sAMEgCQQCpZBZrXkFFZ5R+mz7cJWp8NwCv6x8F\n"
        "GQJ7u5QN6q7JQ8jJN7KvJz8L8y9J8K8K8J8K8K8J8K8K8K8K8K8K8K8K8K8K8K8K\n"
        "8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K8K\n"
        "-----END CERTIFICATE-----\n";
    
    // Modify the certificate with fuzzer input to create variation
    // Use first few bytes to modify parts of the PEM string
    if (size > 10) {
        // Replace some characters in the PEM string with fuzzer data
        size_t mod_pos = fdp.ConsumeIntegralInRange<size_t>(0, pem_cert.length() - 1);
        char mod_char = fdp.ConsumeIntegral<char>();
        if (mod_pos < pem_cert.length()) {
            pem_cert[mod_pos] = mod_char;
        }
    }
    
    // Write PEM certificate to a temporary file
    // Create a temporary file with a unique name
    char temp_filename[] = "/tmp/fuzz_ts_cert_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return 0;
    }
    
    // Write the PEM certificate to the temporary file
    FILE* cert_file = fdopen(fd, "w");
    if (cert_file == NULL) {
        close(fd);
        return 0;
    }
    
    fwrite(pem_cert.c_str(), 1, pem_cert.length(), cert_file);
    fclose(cert_file);
    
    // 1. Call TS_CONF_load_certs to load certificates for verification
    STACK_OF(X509)* certs = TS_CONF_load_certs(temp_filename);
    
    // Clean up temporary file
    remove(temp_filename);
    
    if (certs != NULL) {
        // 2. Create timestamp request with message imprint from fuzzer input
        TS_REQ* ts_req = TS_REQ_new();
        if (ts_req != NULL) {
            // Create message imprint
            TS_MSG_IMPRINT* msg_imprint = TS_MSG_IMPRINT_new();
            if (msg_imprint != NULL) {
                // Set hash algorithm (use SHA256)
                X509_ALGOR* algo = X509_ALGOR_new();
                if (algo != NULL) {
                    ASN1_OBJECT* sha256_obj = OBJ_nid2obj(NID_sha256);
                    X509_ALGOR_set0(algo, sha256_obj, V_ASN1_NULL, NULL);
                    TS_MSG_IMPRINT_set_algo(msg_imprint, algo);
                }
                
                // Set message data from fuzzer input
                size_t imprint_size = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                std::vector<uint8_t> imprint_data = fdp.ConsumeBytes<uint8_t>(imprint_size);
                if (!imprint_data.empty()) {
                    TS_MSG_IMPRINT_set_msg(msg_imprint, 
                                         const_cast<unsigned char*>(imprint_data.data()), 
                                         imprint_data.size());
                }
                
                // Set message imprint in request
                TS_REQ_set_msg_imprint(ts_req, msg_imprint);
                
                // Set other request properties from fuzzer input
                TS_REQ_set_version(ts_req, fdp.ConsumeIntegral<long>());
                
                // Set nonce if available
                if (fdp.remaining_bytes() >= 4) {
                    uint32_t nonce_val = fdp.ConsumeIntegral<uint32_t>();
                    ASN1_INTEGER* nonce = ASN1_INTEGER_new();
                    if (nonce != NULL) {
                        ASN1_INTEGER_set(nonce, nonce_val);
                        TS_REQ_set_nonce(ts_req, nonce);
                        ASN1_INTEGER_free(nonce);
                    }
                }
                
                // Set cert request flag
                TS_REQ_set_cert_req(ts_req, fdp.ConsumeBool() ? 1 : 0);
                
                // Clean up message imprint
                TS_MSG_IMPRINT_free(msg_imprint);
            }
            
            // 3. Create or parse timestamp response
            TS_RESP* ts_resp = NULL;
            
            // Try to parse remaining fuzzer input as a TS_RESP structure
            size_t remaining_size = fdp.remaining_bytes();
            if (remaining_size > 0) {
                std::vector<uint8_t> resp_data = fdp.ConsumeRemainingBytes<uint8_t>();
                
                // Create BIO from response data
                BIO* bio = BIO_new_mem_buf(resp_data.data(), resp_data.size());
                if (bio != NULL) {
                    // Try to parse as TS_RESP
                    ts_resp = d2i_TS_RESP_bio(bio, NULL);
                    BIO_free(bio);
                }
            }
            
            // If parsing failed, create a new empty response
            if (ts_resp == NULL) {
                ts_resp = TS_RESP_new();
            }
            
            if (ts_resp != NULL) {
                // 4. Verify timestamp response using loaded certificates
                TS_VERIFY_CTX* verify_ctx = TS_VERIFY_CTX_new();
                if (verify_ctx != NULL) {
                    // Set verification flags
                    int flags = TS_VFY_SIGNATURE | TS_VFY_VERSION;
                    if (fdp.ConsumeBool()) {
                        flags |= TS_VFY_POLICY;
                    }
                    if (fdp.ConsumeBool()) {
                        flags |= TS_VFY_NONCE;
                    }
                    TS_VERIFY_CTX_set_flags(verify_ctx, flags);
                    
                    // Set certificates for verification
                    if (certs != NULL) {
                        // Note: TS_VERIFY_CTX doesn't have a direct set_certs function
                        // Verification typically uses X509_STORE
                        // For this harness, we'll call TS_RESP_verify_response
                        // which uses the context
                        TS_RESP_verify_response(verify_ctx, ts_resp);
                    }
                    
                    // Clean up verification context
                    TS_VERIFY_CTX_free(verify_ctx);
                }
                
                // Clean up response
                TS_RESP_free(ts_resp);
            }
            
            // Clean up request
            TS_REQ_free(ts_req);
        }
        
        // Clean up certificates
        sk_X509_pop_free(certs, X509_free);
    }
    
    // Clean up OpenSSL
    ERR_free_strings();
    EVP_cleanup();
    
    return 0;
}

} // namespace harness_009
