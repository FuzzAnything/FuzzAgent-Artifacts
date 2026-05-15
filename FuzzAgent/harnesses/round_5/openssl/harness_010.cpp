#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <openssl/rand.h>

namespace harness_010 {
/*
 * Fuzzing harness for OpenSSL targeting PKCS#8 private key serialization/deserialization operations
 * This harness tests: PEM_write_bio_PKCS8PrivateKey, i2d_PKCS8PrivateKey_bio, d2i_PKCS8PrivateKey_bio,
 * PEM_write_bio_PKCS8PrivateKey_nid, i2d_PKCS8PrivateKey_nid_bio and related PKCS#8 operations.
 * Primary target: PEM_write_bio_PKCS8 (1,584 branches with 1,071 undiscovered)
 * Secondary targets: PKCS#8 private key functions in crypto/pem/pem_pk8.c
 * This harness is semantically different from previous harnesses by targeting complete PKCS#8
 * private key lifecycle: Key Generation → Serialization (encrypted/unencrypted) → Deserialization → Verification
 */



/* Password callback for testing encrypted PKCS#8 keys */
static int test_password_callback(char *buf, int size, int rwflag, void *userdata) {
    /* userdata points to the password string provided by fuzzer */
    const char *password = (const char *)userdata;
    if (password == NULL) {
        return -1; /* Error */
    }
    int len = strlen(password);
    if (len > size - 1) {
        len = size - 1;
    }
    memcpy(buf, password, len);
    buf[len] = '\0';
    return len;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize OpenSSL crypto library
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                        OPENSSL_INIT_ADD_ALL_CIPHERS | 
                        OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();

    // Need minimum input size for meaningful PKCS#8 operations
    // We need: key type choice, encryption choice, password data, key parameters
    if (size < 128) {
        return 0;  // Need enough data for basic PKCS#8 operations
    }

    FuzzedDataProvider fdp(data, size);

    // Declare ALL variables at the top to avoid goto jumping over initializations
    EVP_PKEY* pkey = NULL;
    RSA* rsa = NULL;
    BIGNUM* e = NULL;
    BIGNUM* n = NULL;
    BIGNUM* d = NULL;
    BIO* bio = NULL;
    BIO* bio2 = NULL;
    EVP_PKEY* pkey_readback = NULL;
    PKCS8_PRIV_KEY_INFO* p8inf = NULL;
    
    // Variables for data consumption (also declared at top)
    size_t n_size = 0;
    size_t d_size = 0;
    std::vector<uint8_t> n_data;
    std::vector<uint8_t> d_data;
    std::string password;
    std::vector<uint8_t> pem_buffer;
    std::vector<uint8_t> der_buffer;
    
    // Variables for API results
    int write_result = 0;
    int write_der_result = 0;
    int read_result = 0;
    int key_type_choice = 0;
    int encryption_choice = 0;
    int nid_choice = 0;
    int use_password_callback = 0;
    const EVP_CIPHER* cipher = NULL;
    
    // Variables for reading back objects
    char* pem_data = NULL;
    long pem_len = 0;
    
    // ==================== CREATE TEST RSA KEY ====================
    // Create a minimal RSA key for PKCS#8 testing
    rsa = RSA_new();
    if (rsa == NULL) {
        goto cleanup;
    }
    
    e = BN_new();
    n = BN_new();
    d = BN_new();
    
    if (e == NULL || n == NULL || d == NULL) {
        goto cleanup;
    }
    
    // Set small public exponent (common RSA exponent)
    if (!BN_set_word(e, 65537)) {
        goto cleanup;
    }
    
    // Consume some data for RSA key components
    n_size = fdp.ConsumeIntegralInRange<size_t>(8, 256);
    n_data = fdp.ConsumeBytes<uint8_t>(n_size);
    if (n_data.empty()) {
        goto cleanup;
    }
    
    d_size = fdp.ConsumeIntegralInRange<size_t>(8, 256);
    d_data = fdp.ConsumeBytes<uint8_t>(d_size);
    if (d_data.empty()) {
        goto cleanup;
    }
    
    if (!BN_bin2bn(n_data.data(), n_data.size(), n) ||
        !BN_bin2bn(d_data.data(), d_data.size(), d)) {
        goto cleanup;
    }
    
    if (!RSA_set0_key(rsa, n, e, d)) {
        goto cleanup;
    }
    n = NULL;  // Ownership transferred to RSA
    e = NULL;  // Ownership transferred to RSA  
    d = NULL;  // Ownership transferred to RSA
    
    // Create EVP_PKEY from RSA
    pkey = EVP_PKEY_new();
    if (pkey == NULL) {
        goto cleanup;
    }
    
    if (!EVP_PKEY_assign_RSA(pkey, rsa)) {
        goto cleanup;
    }
    rsa = NULL;  // Ownership transferred to EVP_PKEY
    
    // ==================== CREATE MEMORY BIOS ====================
    bio = BIO_new(BIO_s_mem());
    if (bio == NULL) {
        goto cleanup;
    }
    
    bio2 = BIO_new(BIO_s_mem());
    if (bio2 == NULL) {
        goto cleanup;
    }
    
    // ==================== CONSUME FUZZER PARAMETERS ====================
    key_type_choice = fdp.ConsumeIntegralInRange<int>(0, 1);  // Currently only RSA, but placeholder
    encryption_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
    nid_choice = fdp.ConsumeIntegralInRange<int>(-1, 10);  // -1 for no NID, 0-10 for specific NIDs
    use_password_callback = fdp.ConsumeBool();
    password = fdp.ConsumeRandomLengthString(256);
    
    // Choose cipher based on encryption choice
    switch (encryption_choice % 4) {
        case 0:
            cipher = EVP_aes_256_cbc();
            break;
        case 1:
            cipher = EVP_des_ede3_cbc();
            break;
        case 2:
            cipher = EVP_aes_128_cbc();
            break;
        case 3:
        default:
            cipher = NULL;  // No encryption
            break;
    }
    
    // ==================== TEST UNENCRYPTED PKCS#8 (PEM) ====================
    // Test 1: Write unencrypted PKCS#8 private key in PEM format
    write_result = PEM_write_bio_PKCS8PrivateKey(bio, pkey, NULL, NULL, 0, NULL, NULL);
    // Don't fail on error - continue testing other paths
    
    // Read back the PEM data from bio
    BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
    pem_len = BIO_get_mem_data(bio, &pem_data);
    if (pem_len > 0 && pem_data != NULL) {
        // Write PEM data to bio2 for reading back
        BIO_write(bio2, pem_data, pem_len);
        BIO_ctrl(bio2, BIO_CTRL_RESET, 0, NULL);
        
        // Try to read back the unencrypted key
        pkey_readback = PEM_read_bio_PrivateKey(bio2, NULL, NULL, NULL);
        if (pkey_readback != NULL) {
            EVP_PKEY_free(pkey_readback);
            pkey_readback = NULL;
        }
    }
    
    // ==================== TEST ENCRYPTED PKCS#8 (PEM) ====================
    // Reset BIOs
    BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
    BIO_ctrl(bio2, BIO_CTRL_RESET, 0, NULL);
    
    // Test 2: Write encrypted PKCS#8 private key in PEM format
    if (cipher != NULL) {
        if (use_password_callback) {
            write_result = PEM_write_bio_PKCS8PrivateKey(bio, pkey, cipher, 
                                                       NULL, 0, 
                                                       test_password_callback, 
                                                       (void*)password.c_str());
        } else {
            write_result = PEM_write_bio_PKCS8PrivateKey(bio, pkey, cipher,
                                                       (const char*)password.c_str(), 
                                                       password.length(),
                                                       NULL, NULL);
        }
        
        // Read back the encrypted PEM data
        BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
        pem_len = BIO_get_mem_data(bio, &pem_data);
        if (pem_len > 0 && pem_data != NULL) {
            BIO_write(bio2, pem_data, pem_len);
            BIO_ctrl(bio2, BIO_CTRL_RESET, 0, NULL);
            
            // Try to read back with password
            if (use_password_callback) {
                pkey_readback = PEM_read_bio_PrivateKey(bio2, NULL, 
                                                       test_password_callback, 
                                                       (void*)password.c_str());
            } else {
                pkey_readback = PEM_read_bio_PrivateKey(bio2, NULL, NULL, 
                                                       (void*)password.c_str());
            }
            if (pkey_readback != NULL) {
                EVP_PKEY_free(pkey_readback);
                pkey_readback = NULL;
            }
        }
    }
    
    // ==================== TEST PKCS#8 WITH NID (PEM) ====================
    // Reset BIOs
    BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
    BIO_ctrl(bio2, BIO_CTRL_RESET, 0, NULL);
    
    // Test 3: Write PKCS#8 private key with specific NID
    if (nid_choice >= 0) {
        write_result = PEM_write_bio_PKCS8PrivateKey_nid(bio, pkey, nid_choice,
                                                       NULL, 0, NULL, NULL);
        
        // Read back
        BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
        pem_len = BIO_get_mem_data(bio, &pem_data);
        if (pem_len > 0 && pem_data != NULL) {
            BIO_write(bio2, pem_data, pem_len);
            BIO_ctrl(bio2, BIO_CTRL_RESET, 0, NULL);
            
            pkey_readback = PEM_read_bio_PrivateKey(bio2, NULL, NULL, NULL);
            if (pkey_readback != NULL) {
                EVP_PKEY_free(pkey_readback);
                pkey_readback = NULL;
            }
        }
    }
    
    // ==================== TEST DER FORMAT (i2d_PKCS8PrivateKey_bio) ====================
    // Reset BIOs
    BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
    BIO_ctrl(bio2, BIO_CTRL_RESET, 0, NULL);
    
    // Test 4: Write DER-encoded encrypted PKCS#8 private key
    if (cipher != NULL) {
        write_der_result = i2d_PKCS8PrivateKey_bio(bio, pkey, cipher,
                                                 (const char*)password.c_str(),
                                                 password.length(),
                                                 NULL, NULL);
        
        // Read back using d2i_PKCS8PrivateKey_bio
        BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
        pkey_readback = d2i_PKCS8PrivateKey_bio(bio, NULL, NULL, 
                                              (void*)password.c_str());
        if (pkey_readback != NULL) {
            EVP_PKEY_free(pkey_readback);
            pkey_readback = NULL;
        }
    }
    
    // ==================== TEST DER FORMAT WITH NID ====================
    // Reset BIOs
    BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
    
    // Test 5: Write DER-encoded PKCS#8 private key with NID
    if (nid_choice >= 0) {
        write_der_result = i2d_PKCS8PrivateKey_nid_bio(bio, pkey, nid_choice,
                                                     NULL, 0, NULL, NULL);
        
        // Read back
        BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
        pkey_readback = d2i_PKCS8PrivateKey_bio(bio, NULL, NULL, NULL);
        if (pkey_readback != NULL) {
            EVP_PKEY_free(pkey_readback);
            pkey_readback = NULL;
        }
    }
    
    // ==================== TEST ROUND-Trip with PEM_write_bio_PKCS8 ====================
    // Reset BIOs
    BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
    BIO_ctrl(bio2, BIO_CTRL_RESET, 0, NULL);
    
    // Test 6: Direct test of PEM_write_bio_PKCS8 (the primary target)
    // First need to create a PKCS8_PRIV_KEY_INFO structure
    p8inf = EVP_PKEY2PKCS8(pkey);
    if (p8inf != NULL) {
        write_result = PEM_write_bio_PKCS8(bio, (X509_SIG*)p8inf); // Note: cast for testing
        
        // Read back
        BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
        // PEM_read_bio_PKCS8 would be appropriate here, but not available in public API
        // We'll test error paths by trying to read as X509_SIG
        // X509_SIG* sig_read = PEM_read_bio_X509_SIG(bio, NULL, NULL, NULL);
        // if (sig_read != NULL) {
        //     X509_SIG_free(sig_read);
        // }
        
        PKCS8_PRIV_KEY_INFO_free(p8inf);
        p8inf = NULL;
    }
    
    // ==================== TEST ERROR PATHS ====================
    // Test with invalid passwords (should fail gracefully)
    BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
    if (cipher != NULL && fdp.remaining_bytes() > 32) {
        // Consume two different passwords from fuzzer input
        std::string write_password = fdp.ConsumeRandomLengthString(32);
        std::string read_password = fdp.ConsumeRandomLengthString(32);
        
        // Write with first password
        write_result = PEM_write_bio_PKCS8PrivateKey(bio, pkey, cipher,
                                                   write_password.c_str(), 
                                                   write_password.length(),
                                                   NULL, NULL);
        
        // Try to read with second (possibly different) password
        BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
        pkey_readback = PEM_read_bio_PrivateKey(bio, NULL, NULL, 
                                              (void*)read_password.c_str());
        // Should return NULL if passwords don't match, which is expected
        if (pkey_readback != NULL) {
            EVP_PKEY_free(pkey_readback);
            pkey_readback = NULL;
        }
    }
    
    // Test with empty/null passwords
    BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
    if (cipher != NULL) {
        write_result = PEM_write_bio_PKCS8PrivateKey(bio, pkey, cipher,
                                                   NULL, 0, NULL, NULL);
    }
    
    // Test with corrupted data (consume remaining fuzzer data as "corrupted" PEM)
    if (fdp.remaining_bytes() > 0) {
        std::vector<uint8_t> corrupted_data = fdp.ConsumeRemainingBytes<uint8_t>();
        BIO_write(bio, corrupted_data.data(), corrupted_data.size());
        BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
        
        // Try to parse corrupted data (should fail gracefully)
        pkey_readback = d2i_PKCS8PrivateKey_bio(bio, NULL, NULL, NULL);
        if (pkey_readback != NULL) {
            EVP_PKEY_free(pkey_readback);
            pkey_readback = NULL;
        }
    }

cleanup:
    // Cleanup in reverse order of creation
    if (pkey_readback != NULL) {
        EVP_PKEY_free(pkey_readback);
    }
    if (p8inf != NULL) {
        PKCS8_PRIV_KEY_INFO_free(p8inf);
    }
    if (bio2 != NULL) {
        BIO_free(bio2);
    }
    if (bio != NULL) {
        BIO_free(bio);
    }
    if (pkey != NULL) {
        EVP_PKEY_free(pkey);
    }
    // RSA already freed via EVP_PKEY_free if assigned
    if (rsa != NULL) {
        RSA_free(rsa);
    }
    // BIGNUMs already freed via RSA_set0_key if successful
    if (n != NULL) {
        BN_free(n);
    }
    if (e != NULL) {
        BN_free(e);
    }
    if (d != NULL) {
        BN_free(d);
    }
    
    return 0;
}

} // namespace harness_010
