#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/ocsp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/asn1.h>

namespace harness_011 {
/*
 * Fuzzing harness for OpenSSL targeting OCSP (Online Certificate Status Protocol) signing operations
 * This harness tests: OCSP_basic_sign_ctx (primary target with 1412 undiscovered branches)
 * Secondary targets: OCSP_basic_sign (wrapper), OCSP_basic_add1_cert, OCSP_RESPID_set_by_key, 
 * OCSP_RESPID_set_by_name, OCSP_BASICRESP_sign_ctx
 * 
 * Required setup: OCSP_BASICRESP_new, X509_new, EVP_PKEY_new, EVP_MD_CTX_new
 * Required cleanup: OCSP_BASICRESP_free, X509_free, EVP_PKEY_free, EVP_MD_CTX_free
 * 
 * Invocation sequence:
 * 1. Create certificate and key pair (X509_new, EVP_PKEY_new)
 * 2. Create OCSP basic response (OCSP_BASICRESP_new)
 * 3. Add signing certificate (OCSP_basic_add1_cert)
 * 4. Set responder identity (OCSP_RESPID_set_by_key or OCSP_RESPID_set_by_name)
 * 5. Initialize digest context (EVP_MD_CTX_new)
 * 6. Call OCSP_basic_sign_ctx with prepared context
 * 7. Alternatively test OCSP_basic_sign wrapper
 * 8. Cleanup all resources
 * 
 * Focus on error paths and resource exhaustion scenarios for maximum coverage gain.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize OpenSSL crypto library
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                        OPENSSL_INIT_ADD_ALL_CIPHERS | 
                        OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();

    // Need minimum input size for meaningful OCSP operations
    // We need: operation choice, flags, digest choice, key type, etc.
    if (size < 64) {
        return 0;  // Need enough data for basic OCSP operations
    }

    FuzzedDataProvider fdp(data, size);

    // Declare ALL variables at the top to avoid goto jumping over initializations
    X509* cert = NULL;
    EVP_PKEY* pkey = NULL;
    OCSP_BASICRESP* brsp = NULL;
    EVP_MD_CTX* md_ctx = NULL;
    const EVP_MD* digest = NULL;
    STACK_OF(X509)* certs = NULL;
    RSA* rsa = NULL;
    BIGNUM* e = NULL;
    BIGNUM* n = NULL;
    BIGNUM* d = NULL;
    ASN1_INTEGER* serial_num = NULL;
    X509_NAME* name = NULL;
    ASN1_TIME* not_before = NULL;
    ASN1_TIME* not_after = NULL;
    EVP_PKEY* wrong_key = NULL;
    RSA* wrong_rsa = NULL;
    BIGNUM* wrong_e = NULL;
    int result = 0;
    unsigned long flags = 0;
    int operation_choice = 0;
    int digest_choice = 0;
    int respid_choice = 0;
    int use_certs = 0;
    int nid_value = 0;
    
    // Variables for data consumption
    std::vector<uint8_t> cert_data;
    std::vector<uint8_t> key_data;
    std::vector<uint8_t> ocsp_data;
    // ==================== CREATE MINIMAL X509 CERTIFICATE ====================
    // Create a minimal X509 certificate for OCSP signing
    cert = X509_new();
    if (cert == NULL) {
        goto cleanup;
    }
    
    // Set certificate version
    X509_set_version(cert, 2);  // Version 3 (0-indexed)
    
    // Set serial number (random)
    serial_num = ASN1_INTEGER_new();
    if (serial_num) {
        // Generate random serial number
        unsigned char serial_bytes[20];
        RAND_bytes(serial_bytes, sizeof(serial_bytes));
        
        // Convert bytes directly to BIGNUM to avoid signed integer overflow
        BIGNUM* serial_bn = BN_bin2bn(serial_bytes, sizeof(serial_bytes), NULL);
        if (serial_bn) {
            // Convert BIGNUM to ASN1_INTEGER safely
            BN_to_ASN1_INTEGER(serial_bn, serial_num);
            BN_free(serial_bn);
        }
        
        X509_set_serialNumber(cert, serial_num);
        ASN1_INTEGER_free(serial_num);
    }
    // Set issuer and subject names
    name = X509_NAME_new();
    if (name) {
        X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, 
                                  (const unsigned char*)"Test OCSP Responder", -1, -1, 0);
        X509_set_issuer_name(cert, name);
        X509_set_subject_name(cert, name);
        X509_NAME_free(name);
    }
    
    // Set validity period
    not_before = ASN1_TIME_new();
    not_after = ASN1_TIME_new();
    if (not_before && not_after) {
        time_t now = time(NULL);
        ASN1_TIME_set(not_before, now - 86400);  // Yesterday
        ASN1_TIME_set(not_after, now + 86400 * 365);  // One year from now
        X509_set_notBefore(cert, not_before);
        X509_set_notAfter(cert, not_after);
        ASN1_TIME_free(not_before);
        ASN1_TIME_free(not_after);
    }
    
    // ==================== CREATE MINIMAL EVP_PKEY ====================
    // Create an RSA key for signing
    rsa = RSA_new();
    if (rsa == NULL) {
        goto cleanup;
    }
    
    e = BN_new();
    n = BN_new();
    d = BN_new();
    
    if (e && n && d) {
        // Set public exponent
        BN_set_word(e, 65537);
        
        // Generate minimal RSA key components from fuzzer data
        size_t key_size = fdp.ConsumeIntegralInRange<size_t>(64, 512);  // Small for fuzzing
        std::vector<uint8_t> n_bytes = fdp.ConsumeBytes<uint8_t>(key_size);
        std::vector<uint8_t> d_bytes = fdp.ConsumeBytes<uint8_t>(key_size);
        
        if (n_bytes.size() > 0) {
            BN_bin2bn(n_bytes.data(), n_bytes.size(), n);
        }
        if (d_bytes.size() > 0) {
            BN_bin2bn(d_bytes.data(), d_bytes.size(), d);
        }
        // Set RSA key components
        int set_key_result = RSA_set0_key(rsa, n, e, d);
        
        // Create EVP_PKEY from RSA key
        if (set_key_result == 1) {  // RSA_set0_key succeeded, ownership transferred
            pkey = EVP_PKEY_new();
            if (pkey) {
                EVP_PKEY_assign_RSA(pkey, rsa);
                rsa = NULL;  // Ownership transferred
                // BIGNUMs now owned by RSA (via pkey), set pointers to NULL to avoid accidental use
                e = n = d = NULL;
            }
        }
        // If RSA_set0_key failed or EVP_PKEY_new failed, BIGNUMs are still owned by us
    }
    
    // Clean up BIGNUMs only if they weren't transferred to RSA via RSA_set0_key
    // After successful RSA_set0_key, BIGNUMs are owned by RSA and will be freed by RSA_free()
    // or EVP_PKEY_free(pkey) which frees the RSA key internally
    // Note: If e, n, d were set to NULL after successful transfer, BN_free(NULL) is safe
    if (e) BN_free(e);
    if (n) BN_free(n);
    if (d) BN_free(d);
    
    if (rsa) RSA_free(rsa);
    
    if (pkey == NULL) {
        goto cleanup;
    }
    
    // Set public key in certificate
    X509_set_pubkey(cert, pkey);
    
    // ==================== CONSUME FUZZER PARAMETERS ====================
    operation_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
    digest_choice = fdp.ConsumeIntegralInRange<int>(0, 5);
    respid_choice = fdp.ConsumeIntegralInRange<int>(0, 1);
    use_certs = fdp.ConsumeBool();
    nid_value = fdp.ConsumeIntegralInRange<int>(-10, 100);  // Some valid, some invalid NIDs
    
    // Consume flags - test various flag combinations
    if (fdp.ConsumeBool()) flags |= OCSP_NOCERTS;
    if (fdp.ConsumeBool()) flags |= OCSP_NOTIME;
    if (fdp.ConsumeBool()) flags |= OCSP_RESPID_KEY;
    if (fdp.ConsumeBool()) flags |= OCSP_NOSIGS;
    
    // Choose digest algorithm
    switch (digest_choice) {
        case 0: digest = EVP_sha256(); break;
        case 1: digest = EVP_sha1(); break;
        case 2: digest = EVP_sha384(); break;
        case 3: digest = EVP_sha512(); break;
        case 4: digest = EVP_md5(); break;  // Test with weaker digest
        case 5: digest = NULL; break;       // Test NULL digest (error path)
    }
    
    // ==================== CREATE OCSP BASIC RESPONSE ====================
    brsp = OCSP_BASICRESP_new();
    if (brsp == NULL) {
        goto cleanup;
    }
    
    // Add certificate to OCSP response if requested
    if (!(flags & OCSP_NOCERTS)) {
        result = OCSP_basic_add1_cert(brsp, cert);
        // result may be 0 or 1, both are valid test cases
    }
    
    // Note: We don't set responder ID manually here because OCSP_BASICRESP is opaque.
    // The OCSP_basic_sign_ctx function will set responder ID internally based on flags.
    
    // Create certificate stack if needed
    if (use_certs) {
        certs = sk_X509_new_null();
        if (certs) {
            sk_X509_push(certs, cert);
            // Add certificate reference count
            X509_up_ref(cert);
        }
    }
    
    // ==================== TEST OCSP SIGNING OPERATIONS ====================
    
    // Operation 0: Test OCSP_basic_sign_ctx with proper context
    // Operation 1: Test OCSP_basic_sign_ctx with NULL context (error path)
    // Operation 2: Test OCSP_basic_sign wrapper
    // Operation 3: Test additional error scenarios
    
    switch (operation_choice) {
        case 0: {
            // Test OCSP_basic_sign_ctx with proper context
            md_ctx = EVP_MD_CTX_new();
            if (md_ctx && digest && pkey) {
                // Initialize digest context for signing
                EVP_PKEY_CTX* pkey_ctx = NULL;
                if (EVP_DigestSignInit_ex(md_ctx, &pkey_ctx, EVP_MD_get0_name(digest),
                                         NULL, NULL, pkey, NULL)) {
                    result = OCSP_basic_sign_ctx(brsp, cert, md_ctx, certs, flags);
                }
            }
            break;
        }
        
        case 1: {
            // Test OCSP_basic_sign_ctx with NULL context (error path)
            result = OCSP_basic_sign_ctx(brsp, cert, NULL, certs, flags);
            break;
        }
        
        case 2: {
            // Test OCSP_basic_sign wrapper function
            if (digest && pkey) {
                result = OCSP_basic_sign(brsp, cert, pkey, digest, certs, flags);
            }
            break;
        }
        
        case 3: {
            // Test with invalid flags combinations
            unsigned long invalid_flags = flags | 0xFFFFFFFF00000000UL; // Invalid high bits
            if (digest && pkey) {
                result = OCSP_basic_sign(brsp, cert, pkey, digest, certs, invalid_flags);
            }
            break;
        }
    }
    
    // ==================== TEST ERROR PATHS ====================
    // Test with NULL parameters (error paths)
    if (fdp.ConsumeBool()) {
        // Test with NULL brsp
        int err_result = OCSP_basic_sign_ctx(NULL, cert, md_ctx, certs, flags);
        (void)err_result;  // Ignore result
    }
    
    if (fdp.ConsumeBool() && md_ctx) {
        // Test with NULL certificate
        int err_result = OCSP_basic_sign_ctx(brsp, NULL, md_ctx, certs, flags);
        (void)err_result;
    }
    
    // Removed: Testing OCSP_basic_sign with NULL OCSP_BASICRESP pointer is invalid
    // according to API contract and causes NULL pointer dereference in OCSP_basic_add1_cert
    
    // Test with mismatched certificate and key
    if (fdp.ConsumeBool()) {
        // Create another key that doesn't match the certificate
        EVP_PKEY* wrong_key = EVP_PKEY_new();
        if (wrong_key) {
            RSA* wrong_rsa = RSA_new();
            if (wrong_rsa) {
                BIGNUM* wrong_e = BN_new();
                if (wrong_e) {
                    BN_set_word(wrong_e, 65537);
                    RSA_set0_key(wrong_rsa, BN_new(), wrong_e, BN_new());
                    EVP_PKEY_assign_RSA(wrong_key, wrong_rsa);
                    
                    // This should fail (key doesn't match certificate)
                    if (digest) {
                        int err_result = OCSP_basic_sign(brsp, cert, wrong_key, digest, certs, flags);
                        (void)err_result;
                    }
                }
                // Note: RSA and BIGNUMs will be freed when wrong_key is freed
            }
            EVP_PKEY_free(wrong_key);
        }
    }
    
    // Test resource exhaustion scenarios
    if (fdp.ConsumeBool()) {
        // Repeated allocations and frees to test memory management
        for (int i = 0; i < 10 && fdp.remaining_bytes() > 0; i++) {
            OCSP_BASICRESP* temp_brsp = OCSP_BASICRESP_new();
            if (temp_brsp) {
                // Try to sign with minimal setup (likely to fail)
                if (md_ctx && digest && pkey) {
                    int temp_result = OCSP_basic_sign(temp_brsp, cert, pkey, digest, NULL, flags);
                    (void)temp_result;
                }
                OCSP_BASICRESP_free(temp_brsp);
            }
        }
    }

cleanup:
    // ==================== CLEANUP ====================
    if (md_ctx) {
        EVP_MD_CTX_free(md_ctx);
    }
    
    if (certs) {
        sk_X509_pop_free(certs, X509_free);
    }
    
    if (brsp) {
        OCSP_BASICRESP_free(brsp);
    }
    
    if (pkey) {
        EVP_PKEY_free(pkey);
    }
    
    if (cert) {
        X509_free(cert);
    }
    
    // Clear any accumulated errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_011
