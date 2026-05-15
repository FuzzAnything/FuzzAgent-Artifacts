#include <stddef.h>
#include <stdint.h>
#include <ctime>
#include <openssl/cms.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/asn1.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_012 {
/*
 * Fuzzing harness for OpenSSL CMS encryption functions
 * Primary target: CMS_encrypt_ex with 2930 undiscovered accumulated branches
 * Targets: CMS_encrypt_ex, CMS_EnvelopedData_create_ex, CMS_AuthEnvelopedData_create_ex,
 *          CMS_add1_recipient_cert, CMS_final, CMS_set_detached, CMS_ContentInfo_free,
 *          CMS_get0_RecipientInfos, CMS_RecipientInfo_type, CMS_verify
 * Helper APIs: X509_new, X509_free, STACK_OF(X509) operations, BIO_new_mem_buf,
 *              BIO_new, BIO_free_all, OpenSSL_add_all_algorithms, ERR_clear_error,
 *              EVP_CIPHER initialization
 * Cleanup: CMS_ContentInfo_free, X509_free, sk_X509_free, BIO_free_all
 * Implements CMS encryption lifecycle: Create recipient certificates → 
 * Choose encryption cipher → Set encryption flags → Create CMS encrypted content → 
 * Add recipients → Finalize encryption → Verify structure → Cleanup
 * Differentiated from harness_004.cpp (PKCS7/CMS signature verification),
 * harness_007.cpp (CMS EncryptedData operations), and harness_008.cpp (CMS signature verification) -
 * this harness specifically targets CMS encryption (enveloped data) operations
 * which are critical for S/MIME message encryption and secure data transmission.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0;  // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_clear_error();

    // Determine operation type based on fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;

    // Create data to encrypt from fuzzer input
    size_t data_len = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    if (data_len > fdp.remaining_bytes()) {
        data_len = fdp.remaining_bytes();
    }
    std::vector<uint8_t> plain_data = fdp.ConsumeBytes<uint8_t>(data_len);

    // Create BIO for data input
    BIO* data_bio = BIO_new_mem_buf(plain_data.data(), plain_data.size());
    if (data_bio == NULL) {
        return 0;
    }

    // Determine cipher algorithm based on fuzzer input
    uint8_t cipher_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
    const EVP_CIPHER* cipher = NULL;
    
    switch (cipher_choice) {
        case 0:
            cipher = EVP_aes_128_cbc();
            break;
        case 1:
            cipher = EVP_aes_256_cbc();
            break;
        case 2:
            cipher = EVP_aes_128_gcm();
            break;
        case 3:
            cipher = EVP_aes_256_gcm();
            break;
        case 4:
            cipher = EVP_des_ede3_cbc();  // Triple DES
            break;
        case 5:
            cipher = EVP_camellia_128_cbc();
            break;
        case 6:
            cipher = EVP_camellia_256_cbc();
            break;
        case 7:
            cipher = EVP_aria_128_gcm();
            break;
    }

    // Determine encryption flags
    unsigned int flags = 0;
    uint8_t flags_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
    switch (flags_choice) {
        case 0:
            flags = CMS_DETACHED;
            break;
        case 1:
            flags = CMS_BINARY;
            break;
        case 2:
            flags = CMS_STREAM;
            break;
        case 3:
            flags = CMS_PARTIAL;
            break;
        case 4:
            flags = CMS_DETACHED | CMS_BINARY;
            break;
        case 5:
            flags = CMS_STREAM | CMS_PARTIAL;
            break;
        case 6:
            flags = 0;  // Default flags
            break;
        case 7:
            // Random flags combination
            if (fdp.ConsumeBool()) flags |= CMS_DETACHED;
            if (fdp.ConsumeBool()) flags |= CMS_BINARY;
            if (fdp.ConsumeBool()) flags |= CMS_STREAM;
            if (fdp.ConsumeBool()) flags |= CMS_PARTIAL;
            break;
    }

    // Create recipient certificates stack
    STACK_OF(X509)* certs = sk_X509_new_null();
    if (certs == NULL) {
        BIO_free(data_bio);
        return 0;
    }

    // Create some dummy certificates for recipients (simplified for fuzzing)
    size_t num_recipients = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    for (size_t i = 0; i < num_recipients && fdp.remaining_bytes() > 32; i++) {
        X509* cert = X509_new();
        if (cert != NULL) {
            // Set basic certificate fields (minimal for testing)
            X509_set_version(cert, 2); // X509_VERSION_3
            
            // Set a random serial number
            ASN1_INTEGER* serial = ASN1_INTEGER_new();
            if (serial != NULL) {
                uint32_t serial_val = fdp.ConsumeIntegral<uint32_t>();
                ASN1_INTEGER_set(serial, serial_val);
                X509_set_serialNumber(cert, serial);
                ASN1_INTEGER_free(serial);
            }
            
            // Set subject and issuer names (minimal)
            X509_NAME* name = X509_NAME_new();
            if (name != NULL) {
                // Add a simple Common Name entry
                size_t cn_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                if (cn_len > fdp.remaining_bytes()) {
                    cn_len = fdp.remaining_bytes();
                }
                std::string cn_value = fdp.ConsumeBytesAsString(cn_len);
                X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC,
                                          (const unsigned char*)cn_value.c_str(), -1, -1, 0);
                X509_set_subject_name(cert, name);
                X509_set_issuer_name(cert, name);
                X509_NAME_free(name);
            }
            
            // Set public key (dummy)
            EVP_PKEY* pkey = EVP_PKEY_new();
            if (pkey != NULL) {
                // Use a simple RSA key (simplified for fuzzing)
                RSA* rsa = RSA_new();
                if (rsa != NULL) {
                    EVP_PKEY_assign_RSA(pkey, rsa);
                    X509_set_pubkey(cert, pkey);
                }
                EVP_PKEY_free(pkey);
            }
            
            // Set validity period (current time +/- some random offset)
            ASN1_TIME* asn1_time = ASN1_TIME_new();
            if (asn1_time != NULL) {
                time_t now = ::time(NULL);
                time_t not_before = now - fdp.ConsumeIntegralInRange<time_t>(0, 86400*365); // Up to 1 year ago
                time_t not_after = now + fdp.ConsumeIntegralInRange<time_t>(0, 86400*365*2); // Up to 2 years future
                ASN1_TIME_set(asn1_time, not_before);
                X509_set_notBefore(cert, asn1_time);
                ASN1_TIME_set(asn1_time, not_after);
                X509_set_notAfter(cert, asn1_time);
                ASN1_TIME_free(asn1_time);
            }
            
            sk_X509_push(certs, cert);
        }
    }

    // Main CMS structures
    CMS_ContentInfo* cms = NULL;
    BIO* out_bio = NULL;
    int ret = 0;

    switch (operation) {
        case 0: {
            // Test 1: Basic CMS_encrypt_ex with various parameters
            cms = CMS_encrypt_ex(certs, data_bio, cipher, flags, NULL, NULL);
            
            if (cms != NULL) {
                // Test additional operations on the encrypted content
                // Get recipient infos
                STACK_OF(CMS_RecipientInfo)* ris = CMS_get0_RecipientInfos(cms);
                if (ris != NULL) {
                    int num_ri = sk_CMS_RecipientInfo_num(ris);
                    for (int i = 0; i < num_ri; i++) {
                        CMS_RecipientInfo* ri = sk_CMS_RecipientInfo_value(ris, i);
                        int ri_type = CMS_RecipientInfo_type(ri);
                        // Just access the type to exercise code
                    }
                }
                
                // Try to create output BIO and write CMS structure
                out_bio = BIO_new(BIO_s_mem());
                if (out_bio != NULL) {
                    i2d_CMS_bio(out_bio, cms);
                    BIO_free(out_bio);
                    out_bio = NULL;
                }
            }
            break;
        }

        case 1: {
            // Test 2: Create enveloped data directly and add recipients
            if (EVP_CIPHER_get_flags(cipher) & EVP_CIPH_FLAG_AEAD_CIPHER) {
                cms = CMS_AuthEnvelopedData_create_ex(cipher, NULL, NULL);
            } else {
                cms = CMS_EnvelopedData_create_ex(cipher, NULL, NULL);
            }
            
            if (cms != NULL) {
                // Add recipients one by one
                for (int i = 0; i < sk_X509_num(certs); i++) {
                    X509* cert = sk_X509_value(certs, i);
                    CMS_add1_recipient_cert(cms, cert, flags);
                }
                
                // Set detached flag if requested
                if (flags & CMS_DETACHED) {
                    CMS_set_detached(cms, 1);
                }
                
                // Finalize the CMS structure
                if (flags & (CMS_STREAM | CMS_PARTIAL)) {
                    // Partial/stream mode - don't finalize
                } else {
                    CMS_final(cms, data_bio, NULL, flags);
                }
            }
            break;
        }

        case 2: {
            // Test 3: Test with empty recipient list (should fail)
            STACK_OF(X509)* empty_certs = sk_X509_new_null();
            cms = CMS_encrypt_ex(empty_certs, data_bio, cipher, flags, NULL, NULL);
            sk_X509_free(empty_certs);
            
            // If CMS created (unlikely with empty certs), try operations
            if (cms != NULL) {
                // Try to verify (should fail but exercises code)
                BIO* verify_bio = BIO_new(BIO_s_mem());
                if (verify_bio != NULL) {
                    CMS_verify(cms, NULL, NULL, data_bio, verify_bio, 0);
                    BIO_free(verify_bio);
                }
            }
            break;
        }

        case 3: {
            // Test 4: Test with different library context and property query
            cms = CMS_encrypt_ex(certs, data_bio, cipher, flags, NULL, "provider=default");
            
            if (cms != NULL) {
                // Test content info operations
                const ASN1_OBJECT* obj = CMS_get0_type(cms);
                int nid = OBJ_obj2nid(obj);
                
                // Try to get content
                ASN1_OCTET_STRING** pos = CMS_get0_content(cms);
                if (pos != NULL && *pos != NULL) {
                    // Access content length
                    int content_len = ASN1_STRING_length(*pos);
                }
            }
            break;
        }

        case 4: {
            // Test 5: Test with varying data BIO configurations
            // Create new BIO with different configuration
            BIO* alt_data_bio = BIO_new(BIO_s_mem());
            if (alt_data_bio != NULL) {
                // Write fuzzer data to the BIO
                size_t alt_data_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                if (alt_data_len > fdp.remaining_bytes()) {
                    alt_data_len = fdp.remaining_bytes();
                }
                std::vector<uint8_t> alt_data = fdp.ConsumeBytes<uint8_t>(alt_data_len);
                BIO_write(alt_data_bio, alt_data.data(), alt_data.size());
                
                cms = CMS_encrypt_ex(certs, alt_data_bio, cipher, flags, NULL, NULL);
                
                BIO_free(alt_data_bio);
            }
            break;
        }
    }

    // Cleanup
    if (cms != NULL) {
        CMS_ContentInfo_free(cms);
    }
    
    // Free certificates stack
    for (int i = 0; i < sk_X509_num(certs); i++) {
        X509* cert = sk_X509_value(certs, i);
        X509_free(cert);
    }
    sk_X509_free(certs);
    
    BIO_free(data_bio);
    
    // Clear any remaining errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_012
