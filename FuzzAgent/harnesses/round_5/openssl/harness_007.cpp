#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pkcs12.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/objects.h>
#include <openssl/ossl_typ.h>

namespace harness_007 {
/*
 * Fuzzing harness for OpenSSL targeting PKCS#12 certificate and key storage operations
 * This harness tests: PKCS12_parse (primary target - 2118 undiscovered branches), 
 * PKCS12_create, PKCS12_add_cert, PKCS12_add_key, PKCS12_verify_mac, PKCS12_set_mac,
 * d2i_PKCS12_bio, i2d_PKCS12_bio, PKCS12_free, X509_new, EVP_PKEY_new, BIO_new_mem_buf.
 * This harness is semantically different from previous harnesses by targeting the complete
 * PKCS#12 lifecycle: creation → population → protection → serialization → parsing → verification.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize OpenSSL crypto library
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                        OPENSSL_INIT_ADD_ALL_CIPHERS | 
                        OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();

    // Need minimum input size for meaningful PKCS12 operations
    // We need: password data + RSA key components + certificate data
    if (size < 128) {
        return 0;  // Need enough data for basic PKCS12 operations
    }

    FuzzedDataProvider fdp(data, size);

    // Declare ALL variables at the top to avoid goto jumping over initializations
    PKCS12* p12 = NULL;
    PKCS12* parsed_p12 = NULL;
    X509* cert = NULL;
    EVP_PKEY* pkey = NULL;
    EVP_PKEY* parsed_pkey = NULL;
    X509* parsed_cert = NULL;
    STACK_OF(X509)* ca = NULL;
    STACK_OF(X509)* parsed_ca = NULL;
    RSA* rsa = NULL;
    BIGNUM* e = NULL;
    BIGNUM* n = NULL;
    BIGNUM* d = NULL;
    X509_NAME* name = NULL;
    BIO* bio = NULL;
    BIO* mem_bio = NULL;
    ASN1_INTEGER* serial = NULL;
    STACK_OF(PKCS12_SAFEBAG)* bags = NULL;
    PKCS12_SAFEBAG* cert_bag = NULL;
    PKCS12_SAFEBAG* key_bag = NULL;
    BUF_MEM* buf_mem = NULL;
    
    // Variables for data consumption
    std::vector<uint8_t> password_data;
    std::string password_str;
    std::string friendly_name;
    size_t n_size = 0;
    size_t d_size = 0;
    std::vector<uint8_t> n_data;
    std::vector<uint8_t> d_data;
    std::string subject_str;
    std::vector<uint8_t> salt_data;
    unsigned int iter_count = 0;
    int key_nid = 0;
    int cert_nid = 0;
    int keytype = 0;
    int mac_iter = 0;
    uint32_t serial_num = 0;
    
    // Variables for API results
    int result = 0;
    int parse_result = 0;
    int verify_result = 0;
    int set_mac_result = 0;
    int add_cert_result = 0;
    int add_key_result = 0;
    
    // ==================== CREATE MINIMAL X509 CERTIFICATE ====================
    // Create a minimal RSA key for the certificate
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
    
    // Set RSA key components (minimal values for testing)
    if (!BN_bin2bn(n_data.data(), n_data.size(), n) ||
        !BN_bin2bn(d_data.data(), d_data.size(), d)) {
        goto cleanup;
    }
    
    // Set RSA components
    RSA_set0_key(rsa, n, e, d);
    n = e = d = NULL;  // These are now owned by rsa
    
    // Create EVP_PKEY from RSA key
    pkey = EVP_PKEY_new();
    if (pkey == NULL || !EVP_PKEY_assign_RSA(pkey, rsa)) {
        goto cleanup;
    }
    rsa = NULL;  // rsa is now owned by pkey
    
    // Create minimal X509 certificate
    cert = X509_new();
    if (cert == NULL) {
        goto cleanup;
    }
    
    // Set certificate version
    if (!X509_set_version(cert, 2)) {  // X509v3
        goto cleanup;
    }
    
    // Generate a random serial number
    serial = ASN1_INTEGER_new();
    if (serial != NULL) {
        serial_num = fdp.ConsumeIntegral<uint32_t>();
        if (!ASN1_INTEGER_set(serial, serial_num)) {
            ASN1_INTEGER_free(serial);
            serial = NULL;
        }
    }
    
    if (serial != NULL) {
        X509_set_serialNumber(cert, serial);
        ASN1_INTEGER_free(serial);
        serial = NULL;
    }
    
    // Create and set subject name
    name = X509_NAME_new();
    if (name != NULL) {
        subject_str = fdp.ConsumeRandomLengthString(64);
        if (!subject_str.empty()) {
            X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC,
                                      (const unsigned char*)subject_str.c_str(),
                                      subject_str.length(), -1, 0);
        }
        X509_set_subject_name(cert, name);
        X509_set_issuer_name(cert, name);
        X509_NAME_free(name);
        name = NULL;
    }
    
    // Set certificate validity period
    if (!X509_gmtime_adj(X509_getm_notBefore(cert), -3600*24*365) ||
        !X509_gmtime_adj(X509_getm_notAfter(cert), 3600*24*365)) {
        goto cleanup;
    }
    
    // Set public key in certificate
    if (!X509_set_pubkey(cert, pkey)) {
        goto cleanup;
    }
    
    // Self-sign the certificate
    if (!X509_sign(cert, pkey, EVP_sha256())) {
        goto cleanup;
    }
    
    // ==================== PKCS12 CREATION AND MANIPULATION ====================
    // Consume password for PKCS12 operations
    password_str = fdp.ConsumeRandomLengthString(64);
    friendly_name = fdp.ConsumeRandomLengthString(32);
    
    // Consume algorithm parameters
    key_nid = fdp.ConsumeIntegralInRange<int>(NID_pbe_WithSHA1And3_Key_TripleDES_CBC,
                                             NID_pbe_WithSHA1And40BitRC2_CBC);
    cert_nid = fdp.ConsumeIntegralInRange<int>(NID_pbe_WithSHA1And3_Key_TripleDES_CBC,
                                              NID_pbe_WithSHA1And40BitRC2_CBC);
    keytype = fdp.ConsumeIntegralInRange<int>(0, 3);
    iter_count = fdp.ConsumeIntegralInRange<unsigned int>(1, 1000);
    mac_iter = fdp.ConsumeIntegralInRange<int>(1, 1000);
    
    // Create PKCS12 structure using PKCS12_create
    p12 = PKCS12_create(password_str.c_str(), friendly_name.c_str(),
                        pkey, cert, NULL,  // No CA certificates
                        key_nid, cert_nid, iter_count, mac_iter, keytype);
    
    if (p12 == NULL) {
        // If PKCS12_create fails, try alternative approach with PKCS12_init and manual setup
        p12 = PKCS12_init(NID_pkcs7_data);
    }
    
    if (p12 == NULL) {
        goto cleanup;
    }
    
    // Try to add certificate and key using PKCS12_add_cert and PKCS12_add_key
    // First we need to create a stack of PKCS12_SAFEBAGs
    bags = sk_PKCS12_SAFEBAG_new_null();
    if (bags != NULL) {
        // Add certificate
        cert_bag = PKCS12_add_cert(&bags, cert);
        if (cert_bag != NULL) {
            // Try to add friendly name to certificate bag
            PKCS12_add_friendlyname_utf8(cert_bag, friendly_name.c_str(),
                                        friendly_name.length());
        }
        
        // Add private key
        key_bag = PKCS12_add_key(&bags, pkey, keytype, iter_count,
                                key_nid, password_str.c_str());
        if (key_bag != NULL) {
            // Try to add friendly name to key bag
            PKCS12_add_friendlyname_utf8(key_bag, friendly_name.c_str(),
                                        friendly_name.length());
        }
        
        // We would normally add these bags to a safe and then to the PKCS12 structure,
        // but for fuzzing purposes, we'll focus on the main PKCS12 APIs.
        sk_PKCS12_SAFEBAG_pop_free(bags, PKCS12_SAFEBAG_free);
        bags = NULL;
    }
    
    // Set MAC (password protection) on the PKCS12 structure
    salt_data = fdp.ConsumeBytes<uint8_t>(16);  // 16 bytes salt
    if (salt_data.size() >= 8) {  // Minimum salt size
        set_mac_result = PKCS12_set_mac(p12, password_str.c_str(), password_str.length(),
                                       salt_data.data(), salt_data.size(),
                                       mac_iter, EVP_sha256());
    }
    
    // Verify MAC (password protection)
    verify_result = PKCS12_verify_mac(p12, password_str.c_str(), password_str.length());
    
    // Also test with empty password
    PKCS12_verify_mac(p12, NULL, 0);
    PKCS12_verify_mac(p12, "", 0);
    
    // ==================== SERIALIZATION AND PARSING ====================
    // Create memory BIO for serialization
    mem_bio = BIO_new(BIO_s_mem());
    if (mem_bio != NULL) {
        // Serialize PKCS12 to BIO
        result = i2d_PKCS12_bio(mem_bio, p12);
        
        if (result > 0) {
            // Get the serialized data
            buf_mem = NULL;
            BIO_get_mem_ptr(mem_bio, &buf_mem);
            
            if (buf_mem != NULL && buf_mem->length > 0) {
                // Create another BIO with the serialized data
                bio = BIO_new_mem_buf(buf_mem->data, buf_mem->length);
                if (bio != NULL) {
                    // Parse the PKCS12 back from the BIO
                    parsed_p12 = d2i_PKCS12_bio(bio, NULL);
                    
                    if (parsed_p12 != NULL) {
                        // Verify MAC on parsed PKCS12
                        PKCS12_verify_mac(parsed_p12, password_str.c_str(), password_str.length());
                        
                        // Parse the PKCS12 structure to extract certificate and key
                        parsed_pkey = NULL;
                        parsed_cert = NULL;
                        parsed_ca = NULL;
                        
                        parse_result = PKCS12_parse(parsed_p12, password_str.c_str(),
                                                   &parsed_pkey, &parsed_cert, &parsed_ca);
                        
                        // Also test parsing with empty password
                        EVP_PKEY_free(parsed_pkey);
                        X509_free(parsed_cert);
                        OSSL_STACK_OF_X509_free(parsed_ca);
                        
                        parsed_pkey = NULL;
                        parsed_cert = NULL;
                        parsed_ca = NULL;
                        
                        PKCS12_parse(parsed_p12, NULL, &parsed_pkey, &parsed_cert, &parsed_ca);
                        
                        // Test with empty string password
                        EVP_PKEY_free(parsed_pkey);
                        X509_free(parsed_cert);
                        OSSL_STACK_OF_X509_free(parsed_ca);
                        
                        parsed_pkey = NULL;
                        parsed_cert = NULL;
                        parsed_ca = NULL;
                        
                        PKCS12_parse(parsed_p12, "", &parsed_pkey, &parsed_cert, &parsed_ca);
                    }
                }
            }
        }
    }
    
cleanup:
    // Cleanup all allocated resources
    EVP_PKEY_free(parsed_pkey);
    X509_free(parsed_cert);
    OSSL_STACK_OF_X509_free(parsed_ca);
    
    PKCS12_free(parsed_p12);
    PKCS12_free(p12);
    
    EVP_PKEY_free(pkey);
    X509_free(cert);
    
    RSA_free(rsa);
    BN_free(e);
    BN_free(n);
    BN_free(d);
    
    X509_NAME_free(name);
    BIO_free(bio);
    BIO_free(mem_bio);
    
    ASN1_INTEGER_free(serial);
    sk_PKCS12_SAFEBAG_pop_free(bags, PKCS12_SAFEBAG_free);
    
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_007
