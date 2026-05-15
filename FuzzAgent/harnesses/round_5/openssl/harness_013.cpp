#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/hpke.h>
#include <openssl/core_names.h>

namespace harness_013 {
/*
 * Fuzzing harness for OpenSSL targeting HPKE (Hybrid Public Key Encryption) operations
 * This harness tests: OSSL_HPKE_decap (primary target with 615 undiscovered branches)
 * Additional HPKE APIs to target: OSSL_HPKE_encap, OSSL_HPKE_seal, OSSL_HPKE_open,
 * OSSL_HPKE_keygen, OSSL_HPKE_CTX_new, OSSL_HPKE_CTX_free
 * 
 * Optional APIs: OSSL_HPKE_export, OSSL_HPKE_CTX_set1_psk, OSSL_HPKE_CTX_set1_authpriv
 * 
 * Invocation sequence:
 * 1. Generate key pair using OSSL_HPKE_keygen
 * 2. Create sender context with OSSL_HPKE_CTX_new (role = SENDER)
 * 3. Create receiver context with OSSL_HPKE_CTX_new (role = RECEIVER)
 * 4. Optionally setup PSK or auth parameters if fuzz data provides them
 * 5. Sender calls OSSL_HPKE_encap to generate encapsulated key
 * 6. Sender calls OSSL_HPKE_seal to encrypt plaintext
 * 7. Receiver calls OSSL_HPKE_decap to decapsulate key (primary target)
 * 8. Receiver calls OSSL_HPKE_open to decrypt ciphertext
 * 9. Optionally test OSSL_HPKE_export for secret exporting
 * 10. Cleanup with OSSL_HPKE_CTX_free for both contexts
 * 
 * Focus: HPKE is RFC9180 implementation for post-quantum cryptography with 
 * extremely low coverage (0.19%). Test both sender and receiver roles, 
 * error paths, and various modes (PSK, auth). Ensure semantic diversity 
 * from existing harnesses by focusing on modern cryptographic protocol.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize OpenSSL crypto library
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                        OPENSSL_INIT_ADD_ALL_CIPHERS | 
                        OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();

    // Need minimum input size for meaningful HPKE operations
    // We need data for keys, modes, suites, plaintext, etc.
    if (size < 64) {
        return 0;  // Need enough data for basic HPKE operations
    }

    FuzzedDataProvider fdp(data, size);

    // Declare all variables at the top to avoid goto jumping over initializations
    OSSL_HPKE_CTX* sealctx = NULL;
    OSSL_HPKE_CTX* openctx = NULL;
    EVP_PKEY* sender_priv = NULL;
    EVP_PKEY* receiver_priv = NULL;
    EVP_PKEY* auth_priv = NULL;
    unsigned char sender_pub[256];
    unsigned char receiver_pub[256];
    unsigned char auth_pub[256];
    size_t sender_publen = sizeof(sender_pub);
    size_t receiver_publen = sizeof(receiver_pub);
    size_t auth_publen = sizeof(auth_pub);
    unsigned char enc[256];
    size_t enclen = sizeof(enc);
    unsigned char ciphertext[512];
    size_t ciphertextlen = sizeof(ciphertext);
    unsigned char plaintext[256];
    unsigned char decrypted[256];
    size_t decryptedlen = sizeof(decrypted);
    uint64_t seq_num = 0;
    
    // Variables for data consumption
    int mode = 0;
    OSSL_HPKE_SUITE suite;
    int role_sender = OSSL_HPKE_ROLE_SENDER;
    int role_receiver = OSSL_HPKE_ROLE_RECEIVER;
    bool use_psk = false;
    bool use_auth = false;
    bool test_export = false;
    size_t plaintext_size = 0;
    size_t aad_size = 0;
    size_t info_size = 0;
    size_t export_label_size = 0;
    std::vector<uint8_t> ikmE;
    std::vector<uint8_t> ikmR;
    std::vector<uint8_t> ikmAuth;
    std::vector<uint8_t> psk_data;
    std::vector<uint8_t> pskid_data;
    std::vector<uint8_t> info_data;
    std::vector<uint8_t> aad_data;
    std::vector<uint8_t> plaintext_data;
    std::vector<uint8_t> export_label_data;
    // ==================== CONSUME FUZZER INPUT FOR PARAMETERS ====================
    // Consume mode (0-3 as defined in hpke.h)
    mode = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Consume suite components (kem_id, kdf_id, aead_id)
    uint16_t kem_id = fdp.ConsumeIntegral<uint16_t>();
    uint16_t kdf_id = fdp.ConsumeIntegral<uint16_t>();
    uint16_t aead_id = fdp.ConsumeIntegral<uint16_t>();
    
    // Create HPKE suite
    suite.kem_id = kem_id;
    suite.kdf_id = kdf_id;
    suite.aead_id = aead_id;
    
    // Determine if we should use PSK or auth based on mode and input
    use_psk = fdp.ConsumeBool();
    use_auth = fdp.ConsumeBool();
    test_export = fdp.ConsumeBool();
    
    // Consume sizes for various data elements
    plaintext_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    aad_size = fdp.ConsumeIntegralInRange<size_t>(0, 64);
    info_size = fdp.ConsumeIntegralInRange<size_t>(0, 64);
    export_label_size = fdp.ConsumeIntegralInRange<size_t>(0, 32);
    
    // Consume actual data
    ikmE = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(0, 66));  // OSSL_HPKE_MAX_PARMLEN
    ikmR = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(0, 66));
    ikmAuth = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(0, 66));
    psk_data = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(0, 66));
    pskid_data = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(0, 32));
    info_data = fdp.ConsumeBytes<uint8_t>(info_size);
    aad_data = fdp.ConsumeBytes<uint8_t>(aad_size);
    plaintext_data = fdp.ConsumeBytes<uint8_t>(plaintext_size);
    export_label_data = fdp.ConsumeBytes<uint8_t>(export_label_size);
    
    // Ensure we have at least some data for plaintext
    if (plaintext_data.empty()) {
        return 0;
    }
    
    // ==================== KEY GENERATION ====================
    // Generate sender key pair
    if (OSSL_HPKE_keygen(suite, sender_pub, &sender_publen, &sender_priv,
                        ikmE.empty() ? NULL : ikmE.data(), ikmE.size(),
                        NULL, NULL) != 1) {
        // Key generation failed, clean up and return
        goto cleanup;
    }
    
    // Generate receiver key pair
    if (OSSL_HPKE_keygen(suite, receiver_pub, &receiver_publen, &receiver_priv,
                        ikmR.empty() ? NULL : ikmR.data(), ikmR.size(),
                        NULL, NULL) != 1) {
        goto cleanup;
    }
    
    // Generate authentication key pair if needed
    if (use_auth) {
        if (OSSL_HPKE_keygen(suite, auth_pub, &auth_publen, &auth_priv,
                            ikmAuth.empty() ? NULL : ikmAuth.data(), ikmAuth.size(),
                            NULL, NULL) != 1) {
            // Authentication key generation failed, continue without auth
            use_auth = false;
        }
    }
    
    // ==================== CREATE CONTEXTS ====================
    // Create sender context
    sealctx = OSSL_HPKE_CTX_new(mode, suite, role_sender, NULL, NULL);
    if (sealctx == NULL) {
        goto cleanup;
    }
    
    // Create receiver context
    openctx = OSSL_HPKE_CTX_new(mode, suite, role_receiver, NULL, NULL);
    if (openctx == NULL) {
        goto cleanup;
    }
    
    // ==================== SETUP OPTIONAL PARAMETERS ====================
    // Set PSK if mode requires it and we have PSK data
    if ((mode == OSSL_HPKE_MODE_PSK || mode == OSSL_HPKE_MODE_PSKAUTH) && use_psk) {
        if (!psk_data.empty() && !pskid_data.empty()) {
            // Convert pskid_data to null-terminated string
            std::string pskid_str(reinterpret_cast<const char*>(pskid_data.data()),
                                 std::min(pskid_data.size(), (size_t)31));
            pskid_str += '\0';  // Ensure null termination
            
            OSSL_HPKE_CTX_set1_psk(sealctx, pskid_str.c_str(), 
                                  psk_data.data(), psk_data.size());
            OSSL_HPKE_CTX_set1_psk(openctx, pskid_str.c_str(),
                                  psk_data.data(), psk_data.size());
        }
    }
    
    // Set authentication private key if needed
    if ((mode == OSSL_HPKE_MODE_AUTH || mode == OSSL_HPKE_MODE_PSKAUTH) && use_auth && auth_priv != NULL) {
        OSSL_HPKE_CTX_set1_authpriv(sealctx, auth_priv);
        
        // For receiver context, we need to set the auth public key
        OSSL_HPKE_CTX_set1_authpub(openctx, auth_pub, auth_publen);
    }
    
    // Set IKM for sender context if available
    if (!ikmE.empty()) {
        OSSL_HPKE_CTX_set1_ikme(sealctx, ikmE.data(), ikmE.size());
    }
    
    // ==================== ENCAPSULATION (SENDER SIDE) ====================
    // Generate encapsulated key
    if (OSSL_HPKE_encap(sealctx, enc, &enclen,
                       receiver_pub, receiver_publen,
                       info_data.empty() ? NULL : info_data.data(), info_data.size()) != 1) {
        // Encapsulation failed, but we can still test error paths
        // Continue to test other operations if possible
    }
    
    // ==================== SEAL (ENCRYPT) ====================
    // Encrypt plaintext
    ciphertextlen = sizeof(ciphertext);
    if (OSSL_HPKE_seal(sealctx, ciphertext, &ciphertextlen,
                      aad_data.empty() ? NULL : aad_data.data(), aad_data.size(),
                      plaintext_data.data(), plaintext_data.size()) != 1) {
        // Seal failed, but continue to test decap/open error paths
    }
    
    // ==================== DECAPSULATION (PRIMARY TARGET) ====================
    // This is the primary target with 615 undiscovered branches
    if (OSSL_HPKE_decap(openctx, enc, enclen, receiver_priv,
                       info_data.empty() ? NULL : info_data.data(), info_data.size()) != 1) {
        // Decapsulation failed - this is expected for many error paths
        // Test is still valuable for coverage
    }
    
    // ==================== OPEN (DECRYPT) ====================
    // Try to decrypt if we have ciphertext
    if (ciphertextlen > 0 && ciphertextlen <= sizeof(ciphertext)) {
        decryptedlen = sizeof(decrypted);
        if (OSSL_HPKE_open(openctx, decrypted, &decryptedlen,
                          aad_data.empty() ? NULL : aad_data.data(), aad_data.size(),
                          ciphertext, ciphertextlen) != 1) {
            // Decryption failed - expected for many test cases
        }
    }
    
    // ==================== TEST EXPORT FUNCTION ====================
    if (test_export && !export_label_data.empty()) {
        unsigned char exported_secret[32];
        size_t secretlen = sizeof(exported_secret);
        
        // Try to export secret from both contexts
        OSSL_HPKE_export(sealctx, exported_secret, secretlen,
                        export_label_data.data(), export_label_data.size());
        OSSL_HPKE_export(openctx, exported_secret, secretlen,
                        export_label_data.data(), export_label_data.size());
    }
    
    // ==================== TEST SEQUENCE NUMBER OPERATIONS ====================
    // Test sequence number get/set operations
    OSSL_HPKE_CTX_get_seq(sealctx, &seq_num);
    OSSL_HPKE_CTX_set_seq(sealctx, seq_num + 1);
    
    OSSL_HPKE_CTX_get_seq(openctx, &seq_num);
    OSSL_HPKE_CTX_set_seq(openctx, seq_num + 1);
    // ==================== TEST ERROR PATHS ====================
    // Test with NULL parameters to exercise error handling
    if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
        // Test with invalid suite
        OSSL_HPKE_SUITE invalid_suite = {0, 0, 0};
        OSSL_HPKE_suite_check(invalid_suite);
        
        // Test with NULL contexts
        OSSL_HPKE_encap(NULL, enc, &enclen, receiver_pub, receiver_publen, NULL, 0);
        OSSL_HPKE_decap(NULL, enc, enclen, receiver_priv, NULL, 0);
        OSSL_HPKE_seal(NULL, ciphertext, &ciphertextlen, NULL, 0, plaintext_data.data(), plaintext_data.size());
        OSSL_HPKE_open(NULL, decrypted, &decryptedlen, NULL, 0, ciphertext, ciphertextlen);
        
        // Test export with NULL
        unsigned char dummy[16];
        OSSL_HPKE_export(NULL, dummy, sizeof(dummy), NULL, 0);
    }

cleanup:
    // ==================== CLEANUP ====================
    // Free contexts
    OSSL_HPKE_CTX_free(sealctx);
    OSSL_HPKE_CTX_free(openctx);
    
    // Free keys
    EVP_PKEY_free(sender_priv);
    EVP_PKEY_free(receiver_priv);
    EVP_PKEY_free(auth_priv);
    
    return 0;
}

} // namespace harness_013
