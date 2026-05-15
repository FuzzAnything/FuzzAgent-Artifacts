#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pkcs7.h>
#include <openssl/cms.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/asn1.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_004 {
/*
 * Fuzzing harness for OpenSSL X.509 certificate parsing and CMS operations
 * Targets: X.509 certificate/CRL manipulation and Cryptographic Message Syntax (CMS)
 * Coverage goal: Exercise 1900+ accumulated branch complexity with 1100+ undiscovered branches
 * 
 * Target APIs:
 * - X509_CRL_set_issuer_name (Target: 558 accumulated branches, 317 undiscovered)
 * - d2i_PKCS7_bio (Target: 494 accumulated branches, 298 undiscovered)
 * - CMS_EnvelopedData_create (Target: 296 accumulated branches, 172 undiscovered)
 * - X509_OBJECT_set1_X509_CRL (Target: 260 accumulated branches, 150 undiscovered)
 * - i2d_X509_ATTRIBUTE (Target: 344 accumulated branches, 180 undiscovered)
 * 
 * Required helper APIs:
 * - X509_new / X509_free (Certificate initialization/cleanup)
 * - d2i_X509 / i2d_X509 (Certificate parsing/encoding)
 * - BIO_new / BIO_free (I/O operations)
 * 
 * Invocation sequence:
 * 1. Create BIO objects for input/output using BIO_new
 * 2. Parse X509 certificate using d2i_X509_bio
 * 3. Parse X509 CRL using d2i_X509_CRL_bio
 * 4. Manipulate CRL using X509_CRL_set_issuer_name
 * 5. Create X509_OBJECT and set CRL using X509_OBJECT_set1_X509_CRL
 * 6. Parse PKCS7 structure using d2i_PKCS7_bio
 * 7. Create CMS envelope using CMS_EnvelopedData_create
 * 8. Encode X509 attributes using i2d_X509_ATTRIBUTE
 * 9. Clean up all objects
 * 
 * Differentiation from existing harnesses:
 * - harness_000.cpp: AES cryptographic operations
 * - harness_001.cpp: RSA cryptographic operations
 * - harness_002.cpp: SSL/TLS protocol stack
 * - harness_003.cpp: CMP/CRMF certificate management
 * - harness_004.cpp: X.509 certificate parsing and CMS operations (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for X.509 and CMS operations
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL libraries
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    
    // 1. Create BIO objects for input/output
    BIO* bio_in = BIO_new(BIO_s_mem());
    BIO* bio_out = BIO_new(BIO_s_mem());
    if (bio_in == NULL || bio_out == NULL) {
        if (bio_in) BIO_free(bio_in);
        if (bio_out) BIO_free(bio_out);
        return 0;
    }
    
    // Write some fuzzed data to the input BIO for parsing
    size_t bio_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 512);
    std::vector<uint8_t> bio_data = fdp.ConsumeBytes<uint8_t>(bio_data_size);
    
    if (bio_data.size() > 0) {
        BIO_write(bio_in, bio_data.data(), bio_data.size());
        BIO_seek(bio_in, 0);  // Reset position for reading
    }
    
    // 2. Try to parse X509 certificate from BIO
    X509* cert = d2i_X509_bio(bio_in, NULL);
    
    // Reset BIO for next parsing operation
    BIO_seek(bio_in, 0);
    
    // 3. Try to parse X509 CRL from BIO
    X509_CRL* crl = d2i_X509_CRL_bio(bio_in, NULL);
    
    // If CRL parsing succeeded, manipulate it
    if (crl != NULL) {
        // 4. Manipulate CRL using X509_CRL_set_issuer_name
        // Create a minimal X509_NAME for testing
        X509_NAME* name = X509_NAME_new();
        if (name != NULL) {
            // Add some entries to the name using fuzzed data
            size_t name_entries = fdp.ConsumeIntegralInRange<size_t>(1, 5);
            for (size_t i = 0; i < name_entries && fdp.remaining_bytes() > 10; i++) {
                std::string field = fdp.ConsumeRandomLengthString(20);
                std::string value = fdp.ConsumeRandomLengthString(50);
                
                // Try to add name entry (ignore failures)
                X509_NAME_add_entry_by_txt(name, field.c_str(), MBSTRING_UTF8,
                                          (const unsigned char*)value.c_str(), value.length(),
                                          -1, 0);
            }
            
            // Set the issuer name on the CRL
            X509_CRL_set_issuer_name(crl, name);
            X509_NAME_free(name);
        }
        
        // 5. Create X509_OBJECT and set CRL
        X509_OBJECT* obj = X509_OBJECT_new();
        if (obj != NULL) {
            X509_OBJECT_set1_X509_CRL(obj, crl);
            X509_OBJECT_free(obj);
        }
    }
    
    // Reset BIO for PKCS7 parsing
    BIO_seek(bio_in, 0);
    
    // 6. Try to parse PKCS7 structure from BIO
    PKCS7* pkcs7 = d2i_PKCS7_bio(bio_in, NULL);
    
    // 7. Create CMS envelope using CMS_EnvelopedData_create
    // Choose a cipher algorithm based on fuzzer input
    uint8_t cipher_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    const EVP_CIPHER* cipher = NULL;
    
    switch (cipher_choice) {
        case 0:
            cipher = EVP_aes_128_cbc();
            break;
        case 1:
            cipher = EVP_aes_256_cbc();
            break;
        case 2:
            cipher = EVP_des_ede3_cbc();
            break;
    }
    
    CMS_ContentInfo* cms = NULL;
    if (cipher != NULL) {
        cms = CMS_EnvelopedData_create(cipher);
    }
    
    // 8. Encode X509 attributes using i2d_X509_ATTRIBUTE
    // Create a test X509_ATTRIBUTE
    X509_ATTRIBUTE* attr = X509_ATTRIBUTE_new();
    if (attr != NULL) {
        // Set up a simple attribute
        ASN1_OBJECT* obj_id = OBJ_nid2obj(NID_pkcs9_emailAddress);
        if (obj_id != NULL) {
            ASN1_STRING* str = ASN1_STRING_new();
            if (str != NULL) {
                std::string email = fdp.ConsumeRandomLengthString(100);
                ASN1_STRING_set(str, email.c_str(), email.length());
                
                // Add the attribute value
                X509_ATTRIBUTE_set1_object(attr, obj_id);
                X509_ATTRIBUTE_set1_data(attr, V_ASN1_UTF8STRING, 
                                        (unsigned char*)email.c_str(), email.length());
                
                // Encode the attribute
                unsigned char* encoded_data = NULL;
                int encoded_len = i2d_X509_ATTRIBUTE(attr, &encoded_data);
                
                if (encoded_data != NULL) {
                    OPENSSL_free(encoded_data);
                }
                
                ASN1_STRING_free(str);
            }
        }
        
        // Also test X509 encoding/decoding
        if (cert != NULL) {
            // Try to encode certificate
            unsigned char* cert_der = NULL;
            int cert_len = i2d_X509(cert, &cert_der);
            
            if (cert_der != NULL) {
                // Try to decode it back
                const unsigned char* p = cert_der;
                X509* cert2 = d2i_X509(NULL, &p, cert_len);
                if (cert2 != NULL) {
                    X509_free(cert2);
                }
                OPENSSL_free(cert_der);
            }
        }
        
        X509_ATTRIBUTE_free(attr);
    }
    
    // 9. Clean up all objects
    if (cert != NULL) X509_free(cert);
    if (crl != NULL) X509_CRL_free(crl);
    if (pkcs7 != NULL) PKCS7_free(pkcs7);
    if (cms != NULL) CMS_ContentInfo_free(cms);
    
    BIO_free(bio_in);
    BIO_free(bio_out);
    
    return 0;
}

} // namespace harness_004
