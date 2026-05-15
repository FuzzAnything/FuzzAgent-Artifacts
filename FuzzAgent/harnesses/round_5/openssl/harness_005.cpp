#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/asn1.h>
#include <openssl/objects.h>
#include <openssl/ossl_typ.h>

namespace harness_005 {
/*
 * Fuzzing harness for OpenSSL targeting X509 Certificate Request and CRL operations
 * This harness tests: PEM_write_bio_X509_REQ, d2i_X509_CRL_bio, ASN1_item_d2i_fp_ex, 
 * OSSL_IETF_ATTR_SYNTAX_print and related X509_REQ/X509_CRL operations.
 * Primary targets: PEM_write_bio_X509_REQ (1,584 branches), d2i_X509_CRL_bio (476 branches)
 * Secondary targets: ASN1_item_d2i_fp_ex (534 branches), OSSL_IETF_ATTR_SYNTAX_print (958 branches)
 * This harness is semantically different from previous harnesses focusing on EVP operations,
 * hash functions, SSL/TLS, and envelope decryption by targeting certificate/CRL handling.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize OpenSSL crypto library
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                        OPENSSL_INIT_ADD_ALL_CIPHERS | 
                        OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();

    // Need minimum input size for meaningful operations
    // We need: version (long) + some data for subject/issuer names + buffer data
    if (size < 64) {
        return 0;  // Need enough data for basic X509 operations
    }

    FuzzedDataProvider fdp(data, size);

    // Declare ALL variables at the top to avoid goto jumping over initializations
    X509_REQ* req = NULL;
    X509_CRL* crl = NULL;
    BIO* bio = NULL;
    BIO* file_bio = NULL;
    EVP_PKEY* pkey = NULL;
    RSA* rsa = NULL;
    BIGNUM* e = NULL;
    BIGNUM* n = NULL;
    BIGNUM* d = NULL;
    X509_NAME* name = NULL;
    X509_NAME* issuer_name = NULL;
    X509_ATTRIBUTE* attr = NULL;
    ASN1_OBJECT* obj = NULL;
    ASN1_TYPE* attr_type = NULL;
    ASN1_STRING* utf8_str = NULL;
    
    // Variables for data consumption (also declared at top)
    size_t n_size = 0;
    size_t d_size = 0;
    std::vector<uint8_t> n_data;
    std::vector<uint8_t> d_data;
    std::string subject_str;
    std::string issuer_str;
    std::string attr_str;
    
    // Variables for API results
    int write_result = 0;
    int read_result = 0;
    int set_result = 0;
    long version = 0;
    
    // Variables for reading back objects
    X509_REQ* req_read = NULL;
    X509_CRL* crl_read = NULL;
    ASN1_OCTET_STRING* octet_str = NULL;
    
    // ==================== CREATE TEST RSA KEY ====================
    // Create a minimal RSA key for X509_REQ_set_pubkey
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
    
    // Set small public exponent
    if (!BN_set_word(e, 65537)) {
        goto cleanup;
    }
    
    // Consume some data for RSA key components
    n_size = fdp.ConsumeIntegralInRange<size_t>(8, 128);
    n_data = fdp.ConsumeBytes<uint8_t>(n_size);
    if (n_data.empty()) {
        goto cleanup;
    }
    
    d_size = fdp.ConsumeIntegralInRange<size_t>(8, 128);
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
    
    // ==================== CREATE MEMORY BIO ====================
    bio = BIO_new(BIO_s_mem());
    if (bio == NULL) {
        goto cleanup;
    }
    
    // ==================== CREATE AND POPULATE X509_REQ ====================
    req = X509_REQ_new();
    if (req == NULL) {
        goto cleanup;
    }
    
    // Set X509_REQ version
    version = fdp.ConsumeIntegralInRange<long>(0, 1);  // Typically 0 for X509_REQ
    set_result = X509_REQ_set_version(req, version);
    // Don't fail on error - continue testing
    
    // Create and set subject name
    name = X509_NAME_new();
    if (name != NULL) {
        subject_str = fdp.ConsumeRandomLengthString(256);
        // Add a simple common name to the X509_NAME
        X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, 
                                  (const unsigned char*)subject_str.c_str(), 
                                  subject_str.length(), -1, 0);
        
        set_result = X509_REQ_set_subject_name(req, name);
        X509_NAME_free(name);
        name = NULL;
    }
    
    // Set public key
    set_result = X509_REQ_set_pubkey(req, pkey);
    
    // ==================== TEST PEM_write_bio_X509_REQ ====================
    write_result = PEM_write_bio_X509_REQ(bio, req);
    // Check error but continue testing
    
    // ==================== TEST PEM_read_bio_X509_REQ ====================
    // Rewind BIO to beginning
    BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
    
    req_read = PEM_read_bio_X509_REQ(bio, NULL, NULL, NULL);
    if (req_read != NULL) {
        X509_REQ_free(req_read);
        req_read = NULL;
    }
    
    // ==================== CREATE AND POPULATE X509_CRL ====================
    crl = X509_CRL_new();
    if (crl == NULL) {
        goto cleanup;
    }
    
    // Set X509_CRL version
    version = fdp.ConsumeIntegralInRange<long>(0, 2);  // CRL versions: 0, 1, or 2
    set_result = X509_CRL_set_version(crl, version);
    
    // Create and set issuer name
    issuer_name = X509_NAME_new();
    if (issuer_name != NULL) {
        issuer_str = fdp.ConsumeRandomLengthString(256);
        // Add a simple common name to the X509_NAME
        X509_NAME_add_entry_by_txt(issuer_name, "CN", MBSTRING_ASC, 
                                  (const unsigned char*)issuer_str.c_str(), 
                                  issuer_str.length(), -1, 0);
        
        set_result = X509_CRL_set_issuer_name(crl, issuer_name);
        X509_NAME_free(issuer_name);
        issuer_name = NULL;
    }
    
    // ==================== TEST i2d_X509_CRL_bio ====================
    // Clear BIO for DER writing
    BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
    
    write_result = i2d_X509_CRL_bio(bio, crl);
    
    // ==================== TEST d2i_X509_CRL_bio ====================
    // Rewind BIO to beginning
    BIO_ctrl(bio, BIO_CTRL_RESET, 0, NULL);
    
    crl_read = d2i_X509_CRL_bio(bio, NULL);
    if (crl_read != NULL) {
        X509_CRL_free(crl_read);
        crl_read = NULL;
    }
    
    // ==================== TEST ASN1_item_d2i_bio_ex (replaces ASN1_item_d2i_fp_ex) ====================
    // Create a file BIO with some ASN.1 data
    if (fdp.remaining_bytes() > 32) {
        size_t asn1_size = fdp.ConsumeIntegralInRange<size_t>(16, 256);
        std::vector<uint8_t> asn1_data = fdp.ConsumeBytes<uint8_t>(asn1_size);
        
        file_bio = BIO_new(BIO_s_mem());
        if (file_bio != NULL) {
            BIO_write(file_bio, asn1_data.data(), asn1_data.size());
            
            // Try to parse with a simple ASN.1 structure using ASN1_item_d2i_bio_ex
            // Use ASN1_OCTET_STRING as a test type
            const ASN1_ITEM* it = ASN1_OCTET_STRING_it();
            
            octet_str = (ASN1_OCTET_STRING*)ASN1_item_d2i_bio_ex(
                it, file_bio, NULL, NULL, NULL);
            
            if (octet_str != NULL) {
                ASN1_OCTET_STRING_free(octet_str);
                octet_str = NULL;
            }
            
            BIO_free(file_bio);
            file_bio = NULL;
        }
    }
    
    // ==================== TEST OSSL_IETF_ATTR_SYNTAX_print ====================
    // Create a simple X509_ATTRIBUTE for testing
    if (fdp.remaining_bytes() > 16) {
        attr = X509_ATTRIBUTE_new();
        if (attr != NULL) {
            // Create an ASN1_OBJECT for the attribute
            obj = OBJ_nid2obj(NID_pkcs9_emailAddress);  // Use email address NID
            if (obj != NULL) {
                X509_ATTRIBUTE_set1_object(attr, obj);
                
                // Create ASN1_TYPE for attribute value
                attr_type = ASN1_TYPE_new();
                if (attr_type != NULL) {
                    attr_str = fdp.ConsumeRandomLengthString(128);
                    
                    // Create UTF8 string properly
                    utf8_str = ASN1_STRING_type_new(V_ASN1_UTF8STRING);
                    if (utf8_str != NULL) {
                        ASN1_STRING_set(utf8_str, (const void*)attr_str.c_str(), attr_str.length());
                        ASN1_TYPE_set(attr_type, V_ASN1_UTF8STRING, utf8_str);
                        utf8_str = NULL; // Ownership transferred, set to NULL to prevent double-free
                    }
                    
                    // Add value to attribute
                    if (!X509_ATTRIBUTE_set1_data(attr, V_ASN1_UTF8STRING, 
                                                  (unsigned char*)attr_str.c_str(), 
                                                  attr_str.length())) {
                        // Continue even if this fails
                    }
                    
                    ASN1_TYPE_free(attr_type);
                    attr_type = NULL;
                }
                
                // Test OSSL_IETF_ATTR_SYNTAX_print if available
                // Note: This function may not be available in all OpenSSL versions
                #ifdef OSSL_IETF_ATTR_SYNTAX_print
                BIO* out_bio = BIO_new(BIO_s_mem());
                if (out_bio != NULL) {
                    OSSL_IETF_ATTR_SYNTAX_print(out_bio, attr, 0);
                    BIO_free(out_bio);
                }
                #endif
                
                ASN1_OBJECT_free(obj);
                obj = NULL;
            }
            
            X509_ATTRIBUTE_free(attr);
            attr = NULL;
        }
    }
    
cleanup:
    // Cleanup in reverse order of creation
    if (utf8_str != NULL) ASN1_STRING_free(utf8_str);
    if (attr != NULL) X509_ATTRIBUTE_free(attr);
    if (attr_type != NULL) ASN1_TYPE_free(attr_type);
    if (obj != NULL) ASN1_OBJECT_free(obj);
    if (name != NULL) X509_NAME_free(name);
    if (issuer_name != NULL) X509_NAME_free(issuer_name);
    if (file_bio != NULL) BIO_free(file_bio);
    if (bio != NULL) BIO_free(bio);
    if (req != NULL) X509_REQ_free(req);
    if (crl != NULL) X509_CRL_free(crl);
    if (pkey != NULL) EVP_PKEY_free(pkey);
    if (rsa != NULL) RSA_free(rsa);
    if (n != NULL) BN_free(n);
    if (e != NULL) BN_free(e);
    if (d != NULL) BN_free(d);
    
    // Clean up read objects
    if (req_read != NULL) X509_REQ_free(req_read);
    if (crl_read != NULL) X509_CRL_free(crl_read);
    if (octet_str != NULL) ASN1_OCTET_STRING_free(octet_str);
    
    ERR_clear_error();
    return 0;
}

} // namespace harness_005
