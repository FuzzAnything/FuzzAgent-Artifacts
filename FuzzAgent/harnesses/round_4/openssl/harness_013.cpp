#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/pkcs7.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_013 {
/*
 * Fuzzing harness for OpenSSL PKCS7 S/MIME operations
 * Primary target: SMIME_read_PKCS7 with 370 undiscovered branches (690 accumulated)
 * Coverage goal: Exercise PKCS7 S/MIME parsing, manipulation, and verification lifecycle
 * 
 * Target APIs:
 * - SMIME_read_PKCS7 (Primary target: 690 accumulated branches, 370 undiscovered)
 * - SMIME_read_PKCS7_ex (Extended version with context support)
 * - SMIME_write_PKCS7 (Related write operation from same file)
 * - PKCS7_new (Required Helper: Initialization)
 * - PKCS7_free (Required Helper: Cleanup)
 * - PKCS7_sign (Likely needed for creating test data)
 * - PKCS7_verify (Complementary verification function)
 * - PKCS7_new_ex (Extended initialization with context)
 * - PKCS7_get_signer_info (Get signer information)
 * - PKCS7_cert_from_signer_info (Get certificate from signer info)
 * - PKCS7_get_smimecap (Get S/MIME capabilities)
 * - BIO_new_mem_buf (Helper for creating BIO from fuzzer input)
 * - BIO_free (Cleanup for BIO objects)
 * 
 * Invocation sequence:
 * 1. Create BIO objects from fuzzer input using BIO_new_mem_buf
 * 2. Call SMIME_read_PKCS7/SMIME_read_PKCS7_ex to parse S/MIME data from BIO
 * 3. If parsing succeeds, test various PKCS7 operations:
 *    - Get signer information with PKCS7_get_signer_info
 *    - Extract certificates with PKCS7_cert_from_signer_info
 *    - Get S/MIME capabilities with PKCS7_get_smimecap
 * 4. Optionally create new PKCS7 structures with PKCS7_new/PKCS7_new_ex
 * 5. Test SMIME_write_PKCS7 for serialization
 * 6. Test PKCS7_verify for verification of parsed content
 * 7. Clean up all resources
 * 
 * Alternative test flows:
 * - Test SMIME_read_PKCS7_ex with provided PKCS7 context
 * - Test error paths with malformed/malicious S/MIME data
 * - Test with different PKCS7 content types (signed, enveloped, etc.)
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
 * - harness_009.cpp: Time-Stamp Protocol (RFC 3161) operations
 * - harness_010.cpp: Hash functions (SHA family)
 * - harness_011.cpp: DH key exchange and DSA digital signatures
 * - harness_012.cpp: HPKE (Hybrid Public Key Encryption) RFC 9180
 * - harness_013.cpp: PKCS7 S/MIME operations (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need some data for BIO and operation selection
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL error strings
    ERR_load_crypto_strings();
    
    // Consume operation mode: determines which PKCS7 functions to test
    // 0 = Parse only (SMIME_read_PKCS7)
    // 1 = Parse with context (SMIME_read_PKCS7_ex)
    // 2 = Parse and verify (SMIME_read_PKCS7 + PKCS7_verify)
    // 3 = Create new PKCS7 and write (PKCS7_new + SMIME_write_PKCS7)
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume flags for S/MIME operations (0-255 range)
    int smime_flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Create BIO from remaining fuzzer input for S/MIME data
    std::vector<uint8_t> smime_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (smime_data.empty()) {
        return 0;
    }
    
    BIO* bio = BIO_new_mem_buf(smime_data.data(), smime_data.size());
    if (bio == NULL) {
        return 0;
    }
    
    PKCS7* p7 = NULL;
    BIO* bcont = NULL;  // Content BIO (output parameter for SMIME_read_PKCS7)
    
    switch (operation_mode) {
        case 0: {
            // Test SMIME_read_PKCS7 (basic parsing)
            p7 = SMIME_read_PKCS7(bio, &bcont);
            break;
        }
        
        case 1: {
            // Test SMIME_read_PKCS7_ex with optional context
            PKCS7* existing_p7 = NULL;
            // Small chance to provide existing PKCS7 context
            if (fdp.ConsumeBool() && smime_data.size() > 100) {
                // Use first part of data as context, rest as S/MIME
                size_t split_point = smime_data.size() / 2;
                std::vector<uint8_t> context_data(smime_data.begin(), smime_data.begin() + split_point);
                std::vector<uint8_t> actual_smime(smime_data.begin() + split_point, smime_data.end());
                
                BIO_free(bio);
                bio = BIO_new_mem_buf(actual_smime.data(), actual_smime.size());
                
                BIO* context_bio = BIO_new_mem_buf(context_data.data(), context_data.size());
                if (context_bio) {
                    existing_p7 = SMIME_read_PKCS7(context_bio, NULL);
                    BIO_free(context_bio);
                }
            }
            
            p7 = SMIME_read_PKCS7_ex(bio, &bcont, existing_p7 ? &existing_p7 : NULL);
            
            if (existing_p7) {
                PKCS7_free(existing_p7);
            }
            break;
        }
        
        case 2: {
            // Parse and attempt verification
            p7 = SMIME_read_PKCS7(bio, &bcont);
            
            if (p7 != NULL) {
                // Try to verify the parsed PKCS7 structure
                // Note: PKCS7_verify needs certificates and store, which we don't have
                // We'll call it with NULL parameters to exercise error paths
                PKCS7_verify(p7, NULL, NULL, NULL, NULL, 0);
                
                // Also test getting signer information
                STACK_OF(PKCS7_SIGNER_INFO)* signers = PKCS7_get_signer_info(p7);
                if (signers != NULL) {
                    int num_signers = sk_PKCS7_SIGNER_INFO_num(signers);
                    for (int i = 0; i < num_signers && i < 5; i++) {  // Limit to 5 signers
                        PKCS7_SIGNER_INFO* si = sk_PKCS7_SIGNER_INFO_value(signers, i);
                        
                        // Try to get certificate from signer info
                        X509* cert = PKCS7_cert_from_signer_info(p7, si);
                        if (cert != NULL) {
                            X509_free(cert);
                        }
                        
                        // Get S/MIME capabilities
                        STACK_OF(X509_ALGOR)* caps = PKCS7_get_smimecap(si);
                        if (caps != NULL) {
                            sk_X509_ALGOR_pop_free(caps, X509_ALGOR_free);
                        }
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Create new PKCS7 structure and test SMIME_write_PKCS7
            p7 = PKCS7_new();
            if (p7 == NULL) {
                BIO_free(bio);
                return 0;
            }
            
            // Try to set a basic type (signed data)
            PKCS7_set_type(p7, NID_pkcs7_signed);
            
            // Create a dummy data BIO for writing
            BIO* data_bio = BIO_new_mem_buf("test data", 9);
            if (data_bio != NULL) {
                // Try to write the PKCS7 structure
                // This may fail because the PKCS7 structure isn't fully initialized,
                // but it will exercise the API
                SMIME_write_PKCS7(bio, p7, data_bio, smime_flags);
                BIO_free(data_bio);
            }
            
            // Also test PKCS7_new_ex for extended version
            PKCS7* p7_ex = PKCS7_new_ex(NULL, NULL);
            if (p7_ex != NULL) {
                PKCS7_free(p7_ex);
            }
            break;
        }
    }
    
    // If we have a PKCS7 structure from parsing, explore it more
    if (p7 != NULL) {
        // Test various PKCS7 content type checks
        int is_other = PKCS7_type_is_other(p7);
        int type = OBJ_obj2nid(p7->type);
        
        // Try to get issuer and serial (may fail for non-signed types)
        PKCS7_ISSUER_AND_SERIAL* ias = PKCS7_get_issuer_and_serial(p7, 0);
        if (ias != NULL) {
            // ias is owned by p7, don't free it
        }
        
        // Try to get octet string
        ASN1_OCTET_STRING* os = PKCS7_get_octet_string(p7);
        if (os != NULL) {
            // os is owned by p7, don't free it
        }
        
        // Clean up the PKCS7 structure
        PKCS7_free(p7);
    }
    
    // Clean up BIO objects
    if (bcont != NULL) {
        BIO_free(bcont);
    }
    BIO_free(bio);
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_013
