#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>
#include <openssl/pkcs7.h>
#include <openssl/cms.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/objects.h>
#include <openssl/ossl_typ.h>

namespace harness_008 {
/*
 * Fuzzing harness for OpenSSL targeting X.509 certificate chain validation and 
 * PKCS7/CMS signature verification operations.
 * This harness tests: X509_STORE_CTX_verify (primary target), X509_verify_cert, 
 * PKCS7_verify, CMS_verify, X509_STORE_CTX_new, X509_STORE_new, X509_new, 
 * X509_free, X509_STORE_add_cert, PKCS7_new, PKCS7_free, CMS_ContentInfo_new, 
 * CMS_ContentInfo_free.
 * Primary target: verify_chain function with 4399 undiscovered branches (reachable
 * through X509_STORE_CTX_verify, X509_verify_cert, PKCS7_verify, CMS_verify).
 * This harness is semantically different from previous harnesses by focusing on
 * certificate chain validation and signed data verification, which are critical
 * security functions in TLS/SSL, code signing, document signing, and PKI systems.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize OpenSSL crypto library
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | 
                        OPENSSL_INIT_ADD_ALL_CIPHERS | 
                        OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    ERR_clear_error();

    // Need minimum input size for meaningful certificate chain validation
    // We need: data for certificates, store setup, and validation parameters
    if (size < 128) {
        return 0;  // Need enough data for basic certificate operations
    }

    FuzzedDataProvider fdp(data, size);

    // Declare ALL variables at the top to avoid goto jumping over initializations
    X509_STORE* store = NULL;
    X509_STORE_CTX* ctx = NULL;
    X509* leaf_cert = NULL;
    X509* ca_cert = NULL;
    X509* intermediate_cert = NULL;
    STACK_OF(X509)* untrusted_chain = NULL;
    EVP_PKEY* leaf_pkey = NULL;
    EVP_PKEY* ca_pkey = NULL;
    RSA* leaf_rsa = NULL;
    RSA* ca_rsa = NULL;
    BIGNUM* e = NULL;
    BIGNUM* n = NULL;
    BIGNUM* d = NULL;
    BIGNUM* ca_e = NULL;
    BIGNUM* ca_n = NULL;
    BIGNUM* ca_d = NULL;
    X509_NAME* leaf_name = NULL;
    X509_NAME* ca_name = NULL;
    X509_NAME* intermediate_name = NULL;
    BIO* bio = NULL;
    
    // Variables for PKCS7/CMS testing
    PKCS7* pkcs7 = NULL;
    CMS_ContentInfo* cms = NULL;
    
    // Variables for extensions
    X509_EXTENSION* ext = NULL;
    BASIC_CONSTRAINTS* bc = NULL;
    
    // Variables for data consumption
    std::vector<uint8_t> leaf_n_data;
    std::vector<uint8_t> leaf_d_data;
    std::vector<uint8_t> ca_n_data;
    std::vector<uint8_t> ca_d_data;
    std::string leaf_subject_str;
    std::string ca_subject_str;
    std::string intermediate_subject;
    std::string issuer_str;
    size_t leaf_n_size = 0;
    size_t leaf_d_size = 0;
    size_t ca_n_size = 0;
    size_t ca_d_size = 0;
    
    // Variables for API results
    int verify_result = 0;
    int store_add_result = 0;
    int ctx_init_result = 0;
    int pkcs7_verify_result = 0;
    int cms_verify_result = 0;
    long serial_num = 0;
    long ca_serial_num = 0;
    long intermediate_serial_num = 0;
    
    // ==================== CREATE CERTIFICATE STORE AND CONTEXT ====================
    // 1. Create X509_STORE for trusted certificates
    store = X509_STORE_new();
    if (store == NULL) {
        goto cleanup;
    }
    
    // 2. Create X509_STORE_CTX for certificate validation
    ctx = X509_STORE_CTX_new();
    if (ctx == NULL) {
        goto cleanup;
    }
    
    // ==================== CREATE LEAF (END-ENTITY) CERTIFICATE ====================
    // Create a minimal RSA key for the leaf certificate
    leaf_rsa = RSA_new();
    if (leaf_rsa == NULL) {
        goto cleanup;
    }
    
    e = BN_new();
    n = BN_new();
    d = BN_new();
    
    if (e == NULL || n == NULL || d == NULL) {
        goto cleanup;
    }
    
    // Set small public exponent for leaf certificate
    if (!BN_set_word(e, 65537)) {
        goto cleanup;
    }
    
    // Consume data for RSA key components (leaf certificate)
    leaf_n_size = fdp.ConsumeIntegralInRange<size_t>(8, 128);
    leaf_n_data = fdp.ConsumeBytes<uint8_t>(leaf_n_size);
    if (!leaf_n_data.empty()) {
        if (!BN_bin2bn(leaf_n_data.data(), leaf_n_data.size(), n)) {
            goto cleanup;
        }
    } else {
        // Set a default small value if no data
        BN_set_word(n, 0x10001);
    }
    
    leaf_d_size = fdp.ConsumeIntegralInRange<size_t>(8, 128);
    leaf_d_data = fdp.ConsumeBytes<uint8_t>(leaf_d_size);
    if (!leaf_d_data.empty()) {
        if (!BN_bin2bn(leaf_d_data.data(), leaf_d_data.size(), d)) {
            goto cleanup;
        }
    } else {
        // Set a default small value if no data
        BN_set_word(d, 0x10003);
    }
    
    // Set RSA key components
    RSA_set0_key(leaf_rsa, n, e, d);
    // Mark components as owned by RSA key (don't free separately)
    n = NULL;
    e = NULL;
    d = NULL;
    
    // Create EVP_PKEY from RSA key
    leaf_pkey = EVP_PKEY_new();
    if (leaf_pkey == NULL) {
        goto cleanup;
    }
    
    if (!EVP_PKEY_assign_RSA(leaf_pkey, leaf_rsa)) {
        goto cleanup;
    }
    // RSA key is now owned by EVP_PKEY, don't free separately
    leaf_rsa = NULL;
    
    // Create leaf X509 certificate
    leaf_cert = X509_new();
    if (leaf_cert == NULL) {
        goto cleanup;
    }
    
    // Set certificate version (X509v3)
    if (!X509_set_version(leaf_cert, 2)) {  // 2 means X509v3
        goto cleanup;
    }
    
    // Set serial number from fuzzer input
    serial_num = fdp.ConsumeIntegral<long>();
    if (!ASN1_INTEGER_set(X509_get_serialNumber(leaf_cert), serial_num)) {
        goto cleanup;
    }
    
    // Set subject name
    leaf_name = X509_NAME_new();
    if (leaf_name != NULL) {
        leaf_subject_str = fdp.ConsumeRandomLengthString(64);
        if (!leaf_subject_str.empty()) {
            X509_NAME_add_entry_by_txt(leaf_name, "CN", MBSTRING_ASC, 
                                      (const unsigned char*)leaf_subject_str.c_str(), 
                                      leaf_subject_str.length(), -1, 0);
        }
        X509_set_subject_name(leaf_cert, leaf_name);
    }
    
    // Set issuer name (self-signed for now)
    if (leaf_name != NULL) {
        X509_set_issuer_name(leaf_cert, leaf_name);
    }
    
    // Set validity period
    if (!X509_gmtime_adj(X509_get_notBefore(leaf_cert), 0) ||
        !X509_gmtime_adj(X509_get_notAfter(leaf_cert), 31536000L)) {  // 1 year
        goto cleanup;
    }
    
    // Set public key
    if (!X509_set_pubkey(leaf_cert, leaf_pkey)) {
        goto cleanup;
    }
    
    // Sign the certificate (self-signed)
    if (!X509_sign(leaf_cert, leaf_pkey, EVP_sha256())) {
        goto cleanup;
    }
    
    // ==================== CREATE CA CERTIFICATE ====================
    // Create a minimal RSA key for the CA certificate
    ca_rsa = RSA_new();
    if (ca_rsa == NULL) {
        goto cleanup;
    }
    
    ca_e = BN_new();
    ca_n = BN_new();
    ca_d = BN_new();
    
    if (ca_e == NULL || ca_n == NULL || ca_d == NULL) {
        goto cleanup;
    }
    
    // Set small public exponent for CA certificate
    if (!BN_set_word(ca_e, 65537)) {
        goto cleanup;
    }
    
    // Consume data for RSA key components (CA certificate)
    ca_n_size = fdp.ConsumeIntegralInRange<size_t>(8, 128);
    ca_n_data = fdp.ConsumeBytes<uint8_t>(ca_n_size);
    if (!ca_n_data.empty()) {
        if (!BN_bin2bn(ca_n_data.data(), ca_n_data.size(), ca_n)) {
            goto cleanup;
        }
    } else {
        // Set a default small value if no data
        BN_set_word(ca_n, 0x10005);
    }
    
    ca_d_size = fdp.ConsumeIntegralInRange<size_t>(8, 128);
    ca_d_data = fdp.ConsumeBytes<uint8_t>(ca_d_size);
    if (!ca_d_data.empty()) {
        if (!BN_bin2bn(ca_d_data.data(), ca_d_data.size(), ca_d)) {
            goto cleanup;
        }
    } else {
        // Set a default small value if no data
        BN_set_word(ca_d, 0x10007);
    }
    
    // Set RSA key components for CA
    RSA_set0_key(ca_rsa, ca_n, ca_e, ca_d);
    // Mark components as owned by RSA key (don't free separately)
    ca_n = NULL;
    ca_e = NULL;
    ca_d = NULL;
    
    // Create EVP_PKEY from RSA key for CA
    ca_pkey = EVP_PKEY_new();
    if (ca_pkey == NULL) {
        goto cleanup;
    }
    
    if (!EVP_PKEY_assign_RSA(ca_pkey, ca_rsa)) {
        goto cleanup;
    }
    // RSA key is now owned by EVP_PKEY, don't free separately
    ca_rsa = NULL;
    
    // Create CA X509 certificate
    ca_cert = X509_new();
    if (ca_cert == NULL) {
        goto cleanup;
    }
    
    // Set certificate version (X509v3)
    if (!X509_set_version(ca_cert, 2)) {
        goto cleanup;
    }
    
    // Set serial number from fuzzer input
    ca_serial_num = fdp.ConsumeIntegral<long>();
    if (!ASN1_INTEGER_set(X509_get_serialNumber(ca_cert), ca_serial_num)) {
        goto cleanup;
    }
    
    // Set subject name for CA
    ca_name = X509_NAME_new();
    if (ca_name != NULL) {
        ca_subject_str = fdp.ConsumeRandomLengthString(64);
        if (!ca_subject_str.empty()) {
            X509_NAME_add_entry_by_txt(ca_name, "CN", MBSTRING_ASC, 
                                      (const unsigned char*)ca_subject_str.c_str(), 
                                      ca_subject_str.length(), -1, 0);
        }
        X509_set_subject_name(ca_cert, ca_name);
        // CA is self-signed
        X509_set_issuer_name(ca_cert, ca_name);
    }
    
    // Set validity period for CA
    if (!X509_gmtime_adj(X509_get_notBefore(ca_cert), 0) ||
        !X509_gmtime_adj(X509_get_notAfter(ca_cert), 63072000L)) {  // 2 years
        goto cleanup;
    }
    
    // Set public key for CA
    if (!X509_set_pubkey(ca_cert, ca_pkey)) {
        goto cleanup;
    }
    
    // Add basic constraints extension for CA
    bc = BASIC_CONSTRAINTS_new();
    if (bc != NULL) {
        bc->ca = 1;  // This is a CA certificate
        bc->pathlen = NULL;  // No path length constraint
        ext = X509V3_EXT_i2d(NID_basic_constraints, 0, bc);
        BASIC_CONSTRAINTS_free(bc);
        bc = NULL;
        if (ext != NULL) {
            X509_add_ext(ca_cert, ext, -1);
            X509_EXTENSION_free(ext);
            ext = NULL;
        }
    }
    
    // Sign the CA certificate (self-signed)
    if (!X509_sign(ca_cert, ca_pkey, EVP_sha256())) {
        goto cleanup;
    }
    
    // ==================== SET UP CERTIFICATE STORE AND CHAIN ====================
    // Add CA certificate to the trusted store
    store_add_result = X509_STORE_add_cert(store, ca_cert);
    // Note: store_add_result is 1 on success, 0 on failure
    
    // Create untrusted chain (leaf certificate)
    untrusted_chain = sk_X509_new_null();
    if (untrusted_chain != NULL) {
        sk_X509_push(untrusted_chain, leaf_cert);
        
        // Try to add an intermediate certificate if we have enough data
        if (fdp.remaining_bytes() > 64) {
            intermediate_cert = X509_new();
            if (intermediate_cert != NULL) {
                // Minimal setup for intermediate certificate
                X509_set_version(intermediate_cert, 2);
                intermediate_serial_num = fdp.ConsumeIntegral<long>();
                ASN1_INTEGER_set(X509_get_serialNumber(intermediate_cert), 
                                intermediate_serial_num);
                
                // Set subject and issuer names
                intermediate_name = X509_NAME_new();
                if (intermediate_name != NULL) {
                    intermediate_subject = fdp.ConsumeRandomLengthString(32);
                    if (!intermediate_subject.empty()) {
                        X509_NAME_add_entry_by_txt(intermediate_name, "CN", MBSTRING_ASC,
                                                  (const unsigned char*)intermediate_subject.c_str(),
                                                  intermediate_subject.length(), -1, 0);
                    }
                    X509_set_subject_name(intermediate_cert, intermediate_name);
                    X509_set_issuer_name(intermediate_cert, ca_name);  // Issued by CA
                }
                
                // Set validity
                X509_gmtime_adj(X509_get_notBefore(intermediate_cert), 0);
                X509_gmtime_adj(X509_get_notAfter(intermediate_cert), 15768000L);  // 6 months
                
                // Set public key (reuse leaf key for simplicity)
                X509_set_pubkey(intermediate_cert, leaf_pkey);
                
                // Sign by CA
                X509_sign(intermediate_cert, ca_pkey, EVP_sha256());
                
                sk_X509_push(untrusted_chain, intermediate_cert);
            }
        }
    }
    
    // ==================== CERTIFICATE CHAIN VALIDATION ====================
    // Initialize store context with certificate and chain
    ctx_init_result = X509_STORE_CTX_init(ctx, store, leaf_cert, untrusted_chain);
    
    if (ctx_init_result == 1) {
        // PRIMARY TARGET: X509_STORE_CTX_verify to exercise verify_chain function
        verify_result = X509_STORE_CTX_verify(ctx);
        // Note: verify_result is 1 on success, 0 on failure
        
        // Also test X509_verify_cert (alternative entry point)
        int verify_cert_result = X509_verify_cert(ctx);
        // Note: verify_cert_result is 1 on success, 0 on failure, <0 on internal error
    }
    
    // ==================== PKCS7 VERIFICATION TESTING ====================
    // Create a minimal PKCS7 structure for verification testing
    pkcs7 = PKCS7_new();
    if (pkcs7 != NULL) {
        // Set PKCS7 type to signed data
        PKCS7_set_type(pkcs7, NID_pkcs7_signed);
        
        // Try PKCS7_verify with our certificates
        // Note: PKCS7_verify requires more complete PKCS7 structure, but we can
        // at least call it to hit the API and error paths
        STACK_OF(X509)* certs = sk_X509_new_null();
        if (certs != NULL) {
            sk_X509_push(certs, leaf_cert);
            sk_X509_push(certs, ca_cert);
            
            bio = BIO_new(BIO_s_null());
            if (bio != NULL) {
                // PKCS7_verify returns 1 on success, 0 on failure
                pkcs7_verify_result = PKCS7_verify(pkcs7, certs, store, bio, NULL, 0);
                BIO_free(bio);
                bio = NULL;
            }
            
            sk_X509_free(certs);
        }
    }
    
    // ==================== CMS VERIFICATION TESTING ====================
    // Create a minimal CMS structure for verification testing
    cms = CMS_ContentInfo_new();
    if (cms != NULL) {
        // Note: CMS_ContentInfo_new creates a CMS structure of type NID_pkcs7_signed
        // by default, so we don't need to set type explicitly
        
        // Try CMS_verify with our certificates
        STACK_OF(X509)* cms_certs = sk_X509_new_null();
        if (cms_certs != NULL) {
            sk_X509_push(cms_certs, leaf_cert);
            sk_X509_push(cms_certs, ca_cert);
            
            bio = BIO_new(BIO_s_null());
            if (bio != NULL) {
                // CMS_verify returns 1 on success, 0 on failure
                cms_verify_result = CMS_verify(cms, cms_certs, store, bio, NULL, 0);
                BIO_free(bio);
                bio = NULL;
            }
            
            sk_X509_free(cms_certs);
        }
    }
    
cleanup:
    // Cleanup in reverse order of creation
    
    // PKCS7/CMS cleanup
    if (cms != NULL) {
        CMS_ContentInfo_free(cms);
    }
    
    if (pkcs7 != NULL) {
        PKCS7_free(pkcs7);
    }
    
    // Certificate chain cleanup
    if (untrusted_chain != NULL) {
        sk_X509_pop_free(untrusted_chain, X509_free);
        // Note: leaf_cert and intermediate_cert are in the stack, 
        // so they will be freed by sk_X509_pop_free
        leaf_cert = NULL;
        intermediate_cert = NULL;
    }
    
    // Individual certificate cleanup (if not already freed by stack)
    if (leaf_cert != NULL) {
        X509_free(leaf_cert);
    }
    
    if (intermediate_cert != NULL) {
        X509_free(intermediate_cert);
    }
    
    if (ca_cert != NULL) {
        X509_free(ca_cert);
    }
    
    // Key cleanup
    if (leaf_pkey != NULL) {
        EVP_PKEY_free(leaf_pkey);
    }
    
    if (ca_pkey != NULL) {
        EVP_PKEY_free(ca_pkey);
    }
    
    // RSA key cleanup (if not transferred to EVP_PKEY)
    if (leaf_rsa != NULL) {
        RSA_free(leaf_rsa);
    }
    
    if (ca_rsa != NULL) {
        RSA_free(ca_rsa);
    }
    
    // BIGNUM cleanup (if not transferred to RSA)
    if (n != NULL) {
        BN_free(n);
    }
    
    if (e != NULL) {
        BN_free(e);
    }
    
    if (d != NULL) {
        BN_free(d);
    }
    
    if (ca_n != NULL) {
        BN_free(ca_n);
    }
    
    if (ca_e != NULL) {
        BN_free(ca_e);
    }
    
    if (ca_d != NULL) {
        BN_free(ca_d);
    }
    
    // Name cleanup
    if (leaf_name != NULL) {
        X509_NAME_free(leaf_name);
    }
    
    if (ca_name != NULL) {
        X509_NAME_free(ca_name);
    }
    
    if (intermediate_name != NULL) {
        X509_NAME_free(intermediate_name);
    }
    
    // Extension cleanup
    if (ext != NULL) {
        X509_EXTENSION_free(ext);
    }
    
    if (bc != NULL) {
        BASIC_CONSTRAINTS_free(bc);
    }
    
    // Store and context cleanup
    if (ctx != NULL) {
        X509_STORE_CTX_free(ctx);
    }
    
    if (store != NULL) {
        X509_STORE_free(store);
    }
    
    // BIO cleanup
    if (bio != NULL) {
        BIO_free(bio);
    }
    
    // Clear any remaining errors
    ERR_clear_error();
    
    return 0;
}

} // namespace harness_008
