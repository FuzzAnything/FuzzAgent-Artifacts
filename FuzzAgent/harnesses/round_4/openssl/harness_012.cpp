#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/hpke.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_012 {
/*
 * Fuzzing harness for OpenSSL HPKE (Hybrid Public Key Encryption) operations (RFC 9180)
 * Primary target: OSSL_HPKE_export with 377 undiscovered branches (634 accumulated)
 * Coverage goal: Exercise HPKE module for modern hybrid public-key encryption
 * 
 * Target APIs:
 * - OSSL_HPKE_export (Primary target: secret export)
 * - OSSL_HPKE_encap (Key encapsulation)
 * - OSSL_HPKE_seal (Encryption)
 * - OSSL_HPKE_decap (Key decapsulation) 
 * - OSSL_HPKE_open (Decryption)
 * - OSSL_HPKE_keygen (Key generation)
 * - OSSL_HPKE_CTX_new (Context creation)
 * - OSSL_HPKE_CTX_free (Context cleanup)
 * - OSSL_HPKE_CTX_set1_psk (Optional: PSK mode)
 * - OSSL_HPKE_CTX_set1_authpriv (Optional: Auth mode)
 * - OSSL_HPKE_suite_check (Suite validation)
 * 
 * Invocation sequence:
 * 1. Validate HPKE suite parameters with OSSL_HPKE_suite_check
 * 2. Generate key pair with OSSL_HPKE_keygen
 * 3. Create sender context with OSSL_HPKE_CTX_new
 * 4. Optionally set PSK/auth modes with OSSL_HPKE_CTX_set1_psk or OSSL_HPKE_CTX_set1_authpriv
 * 5. Perform key encapsulation with OSSL_HPKE_encap
 * 6. Encrypt test data with OSSL_HPKE_seal
 * 7. Create receiver context with OSSL_HPKE_CTX_new
 * 8. Perform key decapsulation with OSSL_HPKE_decap
 * 9. Decrypt ciphertext with OSSL_HPKE_open
 * 10. Derive exporter secrets with OSSL_HPKE_export (primary target)
 * 11. Cleanup contexts with OSSL_HPKE_CTX_free
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
 * - harness_012.cpp: HPKE (Hybrid Public Key Encryption) RFC 9180 (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for suite selection, mode, and some data
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL error strings
    ERR_load_crypto_strings();
    
    // Consume HPKE mode: 0 = BASE, 1 = PSK, 2 = AUTH, 3 = PSKAUTH
    uint8_t mode_val = fdp.ConsumeIntegral<uint8_t>() % 4;
    int mode;
    switch (mode_val) {
        case 0: mode = OSSL_HPKE_MODE_BASE; break;
        case 1: mode = OSSL_HPKE_MODE_PSK; break;
        case 2: mode = OSSL_HPKE_MODE_AUTH; break;
        case 3: mode = OSSL_HPKE_MODE_PSKAUTH; break;
        default: mode = OSSL_HPKE_MODE_BASE; break;
    }
    
    // Consume KEM ID: choose between available KEMs
    uint8_t kem_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint16_t kem_id;
    switch (kem_choice) {
        case 0: kem_id = OSSL_HPKE_KEM_ID_X25519; break;
        case 1: kem_id = OSSL_HPKE_KEM_ID_P256; break;
        case 2: kem_id = OSSL_HPKE_KEM_ID_P384; break;
        case 3: kem_id = OSSL_HPKE_KEM_ID_P521; break;
        default: kem_id = OSSL_HPKE_KEM_ID_X25519; break;
    }
    
    // Consume KDF ID
    uint8_t kdf_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    uint16_t kdf_id;
    switch (kdf_choice) {
        case 0: kdf_id = OSSL_HPKE_KDF_ID_HKDF_SHA256; break;
        case 1: kdf_id = OSSL_HPKE_KDF_ID_HKDF_SHA384; break;
        case 2: kdf_id = OSSL_HPKE_KDF_ID_HKDF_SHA512; break;
        default: kdf_id = OSSL_HPKE_KDF_ID_HKDF_SHA256; break;
    }
    
    // Consume AEAD ID
    uint8_t aead_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    uint16_t aead_id;
    switch (aead_choice) {
        case 0: aead_id = OSSL_HPKE_AEAD_ID_AES_GCM_128; break;
        case 1: aead_id = OSSL_HPKE_AEAD_ID_AES_GCM_256; break;
        case 2: aead_id = OSSL_HPKE_AEAD_ID_CHACHA_POLY1305; break;
        default: aead_id = OSSL_HPKE_AEAD_ID_AES_GCM_128; break;
    }
    
    // Create HPKE suite
    OSSL_HPKE_SUITE suite = {kem_id, kdf_id, aead_id};
    
    // Validate suite with OSSL_HPKE_suite_check
    if (OSSL_HPKE_suite_check(suite) != 1) {
        // Suite not supported, skip this iteration
        return 0;
    }
    
    // Consume IKM (Input Keying Material) for key generation
    size_t ikm_len = fdp.ConsumeIntegralInRange<size_t>(0, OSSL_HPKE_MAX_PARMLEN);
    std::vector<unsigned char> ikm = fdp.ConsumeBytes<unsigned char>(ikm_len);
    
    // Buffer for public key
    size_t pub_len = 256; // Maximum reasonable size for public keys
    std::vector<unsigned char> pub(pub_len);
    size_t actual_pub_len = pub_len;
    
    // Generate key pair with OSSL_HPKE_keygen
    EVP_PKEY* priv_key = NULL;
    if (OSSL_HPKE_keygen(suite, pub.data(), &actual_pub_len, &priv_key, 
                         ikm.empty() ? NULL : ikm.data(), ikm.size(),
                         NULL, NULL) != 1) {
        // Key generation failed, skip
        return 0;
    }
    
    // Create sender context with OSSL_HPKE_CTX_new
    OSSL_HPKE_CTX* sender_ctx = OSSL_HPKE_CTX_new(mode, suite, OSSL_HPKE_ROLE_SENDER, NULL, NULL);
    if (sender_ctx == NULL) {
        EVP_PKEY_free(priv_key);
        return 0;
    }
    
    // Optional: Set PSK if mode requires it
    if (mode == OSSL_HPKE_MODE_PSK || mode == OSSL_HPKE_MODE_PSKAUTH) {
        // Consume PSK ID and PSK
        std::string pskid = fdp.ConsumeRandomLengthString(OSSL_HPKE_MAX_PARMLEN);
        size_t psk_len = fdp.ConsumeIntegralInRange<size_t>(OSSL_HPKE_MIN_PSKLEN, OSSL_HPKE_MAX_PARMLEN);
        std::vector<unsigned char> psk = fdp.ConsumeBytes<unsigned char>(psk_len);
        
        if (!psk.empty()) {
            OSSL_HPKE_CTX_set1_psk(sender_ctx, pskid.c_str(), psk.data(), psk.size());
        }
    }
    
    // Optional: Set auth private key if mode requires it
    if (mode == OSSL_HPKE_MODE_AUTH || mode == OSSL_HPKE_MODE_PSKAUTH) {
        // For simplicity, we reuse the same private key for auth
        OSSL_HPKE_CTX_set1_authpriv(sender_ctx, priv_key);
    }
    
    // Buffer for encapsulated key
    size_t enc_len = OSSL_HPKE_get_public_encap_size(suite);
    std::vector<unsigned char> enc(enc_len);
    size_t actual_enc_len = enc_len;
    
    // Consume info parameter for encapsulation
    size_t info_len = fdp.ConsumeIntegralInRange<size_t>(0, OSSL_HPKE_MAX_INFOLEN);
    std::vector<unsigned char> info = fdp.ConsumeBytes<unsigned char>(info_len);
    
    // Perform key encapsulation with OSSL_HPKE_encap
    if (OSSL_HPKE_encap(sender_ctx, enc.data(), &actual_enc_len,
                        pub.data(), actual_pub_len,
                        info.empty() ? NULL : info.data(), info.size()) != 1) {
        OSSL_HPKE_CTX_free(sender_ctx);
        EVP_PKEY_free(priv_key);
        return 0;
    }
    
    // Consume plaintext for encryption
    size_t pt_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::vector<unsigned char> pt = fdp.ConsumeBytes<unsigned char>(pt_len);
    
    // Consume AAD (Additional Authenticated Data)
    size_t aad_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::vector<unsigned char> aad = fdp.ConsumeBytes<unsigned char>(aad_len);
    
    // Buffer for ciphertext
    size_t ct_len = OSSL_HPKE_get_ciphertext_size(suite, pt_len);
    std::vector<unsigned char> ct(ct_len);
    size_t actual_ct_len = ct_len;
    
    // Encrypt plaintext with OSSL_HPKE_seal
    if (OSSL_HPKE_seal(sender_ctx, ct.data(), &actual_ct_len,
                       aad.empty() ? NULL : aad.data(), aad.size(),
                       pt.empty() ? NULL : pt.data(), pt.size()) != 1) {
        OSSL_HPKE_CTX_free(sender_ctx);
        EVP_PKEY_free(priv_key);
        return 0;
    }
    
    // Create receiver context with OSSL_HPKE_CTX_new
    OSSL_HPKE_CTX* receiver_ctx = OSSL_HPKE_CTX_new(mode, suite, OSSL_HPKE_ROLE_RECEIVER, NULL, NULL);
    if (receiver_ctx == NULL) {
        OSSL_HPKE_CTX_free(sender_ctx);
        EVP_PKEY_free(priv_key);
        return 0;
    }
    
    // Optional: Set PSK on receiver context if mode requires it
    if (mode == OSSL_HPKE_MODE_PSK || mode == OSSL_HPKE_MODE_PSKAUTH) {
        // Use same PSK parameters as sender
        std::string pskid = fdp.ConsumeRandomLengthString(OSSL_HPKE_MAX_PARMLEN);
        size_t psk_len = fdp.ConsumeIntegralInRange<size_t>(OSSL_HPKE_MIN_PSKLEN, OSSL_HPKE_MAX_PARMLEN);
        std::vector<unsigned char> psk = fdp.ConsumeBytes<unsigned char>(psk_len);
        
        if (!psk.empty()) {
            OSSL_HPKE_CTX_set1_psk(receiver_ctx, pskid.c_str(), psk.data(), psk.size());
        }
    }
    
    // Perform key decapsulation with OSSL_HPKE_decap
    if (OSSL_HPKE_decap(receiver_ctx, enc.data(), actual_enc_len,
                        priv_key,
                        info.empty() ? NULL : info.data(), info.size()) != 1) {
        OSSL_HPKE_CTX_free(sender_ctx);
        OSSL_HPKE_CTX_free(receiver_ctx);
        EVP_PKEY_free(priv_key);
        return 0;
    }
    
    // Buffer for decrypted plaintext
    std::vector<unsigned char> pt_decrypted(pt_len);
    size_t actual_pt_len = pt_len;
    
    // Decrypt ciphertext with OSSL_HPKE_open
    if (OSSL_HPKE_open(receiver_ctx, pt_decrypted.data(), &actual_pt_len,
                       aad.empty() ? NULL : aad.data(), aad.size(),
                       ct.data(), actual_ct_len) != 1) {
        OSSL_HPKE_CTX_free(sender_ctx);
        OSSL_HPKE_CTX_free(receiver_ctx);
        EVP_PKEY_free(priv_key);
        return 0;
    }
    
    // Primary target: Derive exporter secrets with OSSL_HPKE_export
    // Try multiple export calls with different labels and lengths
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
        // Consume export label
        size_t label_len = fdp.ConsumeIntegralInRange<size_t>(0, OSSL_HPKE_MAX_PARMLEN);
        std::vector<unsigned char> label = fdp.ConsumeBytes<unsigned char>(label_len);
        
        // Consume export secret length
        size_t secret_len = fdp.ConsumeIntegralInRange<size_t>(0, 64);
        std::vector<unsigned char> secret(secret_len);
        
        if (!label.empty() && secret_len > 0) {
            // Try export from sender context
            OSSL_HPKE_export(sender_ctx, secret.data(), secret_len,
                             label.data(), label.size());
            
            // Try export from receiver context
            OSSL_HPKE_export(receiver_ctx, secret.data(), secret_len,
                             label.data(), label.size());
        }
    }
    
    // Cleanup
    OSSL_HPKE_CTX_free(sender_ctx);
    OSSL_HPKE_CTX_free(receiver_ctx);
    EVP_PKEY_free(priv_key);
    
    return 0;
}

} // namespace harness_012
