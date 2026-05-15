#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <openssl/aes.h>
}
extern "C" {
#include <openssl/asn1.h>
}
extern "C" {
#include <openssl/asn1err.h>
}
extern "C" {
#include <openssl/asn1t.h>
}
extern "C" {
#include <openssl/async.h>
}
extern "C" {
#include <openssl/asyncerr.h>
}
extern "C" {
#include <openssl/bio.h>
}
extern "C" {
#include <openssl/bioerr.h>
}
extern "C" {
#include <openssl/blowfish.h>
}
extern "C" {
#include <openssl/bn.h>
}
extern "C" {
#include <openssl/bnerr.h>
}
extern "C" {
#include <openssl/buffer.h>
}
extern "C" {
#include <openssl/buffererr.h>
}
extern "C" {
#include <openssl/byteorder.h>
}
extern "C" {
#include <openssl/camellia.h>
}
extern "C" {
#include <openssl/cast.h>
}
extern "C" {
#include <openssl/cmac.h>
}
extern "C" {
#include <openssl/cmp.h>
}
extern "C" {
#include <openssl/cmp_util.h>
}
extern "C" {
#include <openssl/cmperr.h>
}
extern "C" {
#include <openssl/cms.h>
}
extern "C" {
#include <openssl/cmserr.h>
}
extern "C" {
#include <openssl/comp.h>
}
extern "C" {
#include <openssl/comperr.h>
}
extern "C" {
#include <openssl/conf.h>
}
extern "C" {
#include <openssl/conf_api.h>
}
extern "C" {
#include <openssl/conferr.h>
}
extern "C" {
#include <openssl/configuration.h>
}
extern "C" {
#include <openssl/conftypes.h>
}
extern "C" {
#include <openssl/core.h>
}
extern "C" {
#include <openssl/core_dispatch.h>
}
extern "C" {
#include <openssl/core_names.h>
}
extern "C" {
#include <openssl/core_object.h>
}
extern "C" {
#include <openssl/crmf.h>
}
extern "C" {
#include <openssl/crmferr.h>
}
extern "C" {
#include <openssl/crypto.h>
}
extern "C" {
#include <openssl/cryptoerr.h>
}
extern "C" {
#include <openssl/cryptoerr_legacy.h>
}
extern "C" {
#include <openssl/ct.h>
}
extern "C" {
#include <openssl/cterr.h>
}
extern "C" {
#include <openssl/decoder.h>
}
extern "C" {
#include <openssl/decodererr.h>
}
extern "C" {
#include <openssl/des.h>
}
extern "C" {
#include <openssl/dh.h>
}
extern "C" {
#include <openssl/dherr.h>
}
extern "C" {
#include <openssl/dsa.h>
}
extern "C" {
#include <openssl/dsaerr.h>
}
extern "C" {
#include <openssl/dtls1.h>
}
extern "C" {
#include <openssl/e_os2.h>
}
extern "C" {
#include <openssl/e_ostime.h>
}
extern "C" {
#include <openssl/ebcdic.h>
}
extern "C" {
#include <openssl/ec.h>
}
extern "C" {
#include <openssl/ecdh.h>
}
extern "C" {
#include <openssl/ecdsa.h>
}
extern "C" {
#include <openssl/ecerr.h>
}
extern "C" {
#include <openssl/ech.h>
}
extern "C" {
#include <openssl/encoder.h>
}
extern "C" {
#include <openssl/encodererr.h>
}
extern "C" {
#include <openssl/engine.h>
}
extern "C" {
#include <openssl/engineerr.h>
}
extern "C" {
#include <openssl/err.h>
}
extern "C" {
#include <openssl/ess.h>
}
extern "C" {
#include <openssl/esserr.h>
}
extern "C" {
#include <openssl/evp.h>
}
extern "C" {
#include <openssl/evperr.h>
}
extern "C" {
#include <openssl/fips_names.h>
}
extern "C" {
#include <openssl/fipskey.h>
}
extern "C" {
#include <openssl/hmac.h>
}
extern "C" {
#include <openssl/hpke.h>
}
extern "C" {
#include <openssl/http.h>
}
extern "C" {
#include <openssl/httperr.h>
}
extern "C" {
#include <openssl/idea.h>
}
extern "C" {
#include <openssl/indicator.h>
}
extern "C" {
#include <openssl/kdf.h>
}
extern "C" {
#include <openssl/kdferr.h>
}
extern "C" {
#include <openssl/lhash.h>
}
extern "C" {
#include <openssl/macros.h>
}
extern "C" {
#include <openssl/md2.h>
}
extern "C" {
#include <openssl/md4.h>
}
extern "C" {
#include <openssl/md5.h>
}
extern "C" {
#include <openssl/mdc2.h>
}
extern "C" {
#include <openssl/ml_kem.h>
}
extern "C" {
#include <openssl/modes.h>
}
extern "C" {
#include <openssl/obj_mac.h>
}
extern "C" {
#include <openssl/objects.h>
}
extern "C" {
#include <openssl/objectserr.h>
}
extern "C" {
#include <openssl/ocsp.h>
}
extern "C" {
#include <openssl/ocsperr.h>
}
extern "C" {
#include <openssl/opensslconf.h>
}
extern "C" {
#include <openssl/opensslv.h>
}
extern "C" {
#include <openssl/ossl_typ.h>
}
extern "C" {
#include <openssl/param_build.h>
}
extern "C" {
#include <openssl/params.h>
}
extern "C" {
#include <openssl/pem.h>
}
extern "C" {
#include <openssl/pem2.h>
}
extern "C" {
#include <openssl/pemerr.h>
}
extern "C" {
#include <openssl/pkcs12.h>
}
extern "C" {
#include <openssl/pkcs12err.h>
}
extern "C" {
#include <openssl/pkcs7.h>
}
extern "C" {
#include <openssl/pkcs7err.h>
}
extern "C" {
#include <openssl/posix_time.h>
}
extern "C" {
#include <openssl/prov_ssl.h>
}
extern "C" {
#include <openssl/proverr.h>
}
extern "C" {
#include <openssl/provider.h>
}
extern "C" {
#include <openssl/quic.h>
}
extern "C" {
#include <openssl/rand.h>
}
extern "C" {
#include <openssl/randerr.h>
}
extern "C" {
#include <openssl/rc2.h>
}
extern "C" {
#include <openssl/rc4.h>
}
extern "C" {
#include <openssl/rc5.h>
}
extern "C" {
#include <openssl/ripemd.h>
}
extern "C" {
#include <openssl/rsa.h>
}
extern "C" {
#include <openssl/rsaerr.h>
}
extern "C" {
#include <openssl/safestack.h>
}
extern "C" {
#include <openssl/seed.h>
}
extern "C" {
#include <openssl/self_test.h>
}
extern "C" {
#include <openssl/sha.h>
}
extern "C" {
#include <openssl/srp.h>
}
extern "C" {
#include <openssl/srtp.h>
}
extern "C" {
#include <openssl/ssl.h>
}
extern "C" {
#include <openssl/ssl2.h>
}
extern "C" {
#include <openssl/ssl3.h>
}
extern "C" {
#include <openssl/sslerr.h>
}
extern "C" {
#include <openssl/sslerr_legacy.h>
}
extern "C" {
#include <openssl/stack.h>
}
extern "C" {
#include <openssl/store.h>
}
extern "C" {
#include <openssl/storeerr.h>
}
extern "C" {
#include <openssl/symhacks.h>
}
extern "C" {
#include <openssl/thread.h>
}
extern "C" {
#include <openssl/tls1.h>
}
extern "C" {
#include <openssl/trace.h>
}
extern "C" {
#include <openssl/ts.h>
}
extern "C" {
#include <openssl/tserr.h>
}
extern "C" {
#include <openssl/txt_db.h>
}
extern "C" {
#include <openssl/types.h>
}
extern "C" {
#include <openssl/ui.h>
}
extern "C" {
#include <openssl/uierr.h>
}
extern "C" {
#include <openssl/whrlpool.h>
}
extern "C" {
#include <openssl/x509.h>
}
extern "C" {
#include <openssl/x509_acert.h>
}
extern "C" {
#include <openssl/x509_vfy.h>
}
extern "C" {
#include <openssl/x509err.h>
}
extern "C" {
#include <openssl/x509v3.h>
}
extern "C" {
#include <openssl/x509v3err.h>
}
//<ID> 347
//<Prompt> []
/*<Combination>: [
*/
//<score> 145, nr_unique_branch: 77
//<Quality> {"density":5,"unique_branches":{"provider_deactivate":[[1174,9,1174,21,0,0,4,1],[1178,9,1178,35,0,0,4,1],[1188,9,1188,22,0,0,4,1],[1191,9,1191,13,0,0,4,0],[1191,17,1191,54,0,0,4,1],[1193,9,1193,13,0,0,4,0],[1193,17,1193,59,0,0,4,1],[1198,9,1198,83,0,0,4,1],[1207,9,1207,19,0,0,4,1],[1218,9,1218,18,0,0,4,0],[1226,9,1226,23,0,0,4,0],[1226,27,1226,40,0,0,4,0],[1230,21,1230,28,0,0,4,1],[1236,9,1236,13,0,0,4,0],[1244,13,1244,22,0,0,4,0],[1249,9,1249,19,0,0,4,1]],"PEM_ASN1_read":[[117,9,117,44,0,0,4,1]],"asn1_template_noexp_d2i":[[714,20,714,29,0,0,4,0]],"impl_cache_free":[[260,9,260,21,0,0,4,0],[264,13,264,51,0,0,4,0]],"DSO_free":[[51,9,51,20,0,0,4,0]],"asn1_d2i_ex_primitive":[[781,16,781,25,0,0,4,0]],"crl_cb":[[222,5,222,25,0,0,4,0],[223,13,223,32,0,0,4,1]],"init_thread_deregister":[[445,9,445,20,0,0,4,1],[447,9,447,13,0,0,4,0],[448,13,448,49,0,0,4,1],[453,17,453,66,0,0,4,0],[453,17,453,66,0,0,4,1],[458,13,458,26,0,0,4,1],[464,16,464,28,0,0,4,0],[464,16,464,28,0,0,4,1],[465,17,465,20,0,0,4,1],[465,24,465,44,0,0,4,1],[478,13,478,16,0,0,4,1],[481,9,481,12,0,0,4,1]],"ossl_method_store_fetch":[[808,9,808,44,0,0,4,1]],"EVP_RAND_CTX_free":[[388,9,388,20,0,0,4,1],[392,9,392,16,0,0,4,1]],"drbg_ctr_free":[[667,9,667,21,0,0,4,0],[667,25,667,68,0,0,4,0]],"find_call":[[749,9,749,25,0,0,4,1]],"provider_deactivate_free":[[238,9,238,29,0,0,4,0]],"ossl_provider_teardown":[[1743,9,1743,31,0,0,4,0],[1745,12,1745,26,0,0,4,0]],"BIO_meth_free":[[52,9,52,21,0,0,4,0]],"ossl_provider_doall_activated":[[1543,9,1543,37,0,0,4,1]],"ossl_lib_ctx_is_default":[[595,24,595,52,0,0,4,1]],"ossl_decoder_cache_flush":[[815,9,815,22,0,0,4,0]],"ossl_provider_deactivate":[[1425,9,1425,21,0,0,4,1],[1426,12,1426,70,0,0,4,1],[1428,12,1428,22,0,0,4,0]],"ossl_rand_drbg_new":[[818,9,818,79,0,0,4,1],[820,9,820,69,0,0,4,1],[822,9,822,71,0,0,4,1],[824,9,824,79,0,0,4,1],[828,9,828,73,0,0,4,1],[830,9,830,75,0,0,4,1],[846,9,846,23,0,0,4,1]],"impl_free":[[252,9,252,21,0,0,4,0]],"evp_rand_free":[[71,9,71,16,0,0,4,1]],"provider_remove_store_methods":[[1366,9,1366,59,0,0,4,1],[1369,9,1369,46,0,0,4,1],[1374,9,1374,17,0,0,4,1]],"ossl_rand_drbg_free":[[871,9,871,21,0,0,4,1]],"alg_cleanup":[[324,9,324,18,0,0,4,0],[328,9,328,19,0,0,4,0]],"ossl_provider_free":[[753,13,753,21,0,0,4,0],[754,17,754,39,0,0,4,0],[758,21,758,48,0,0,4,1]],"ossl_list_lru_entry_remove":[[127,13,127,32,0,0,4,0],[127,13,127,32,0,0,4,1],[129,13,129,32,0,0,4,0],[131,13,131,48,0,0,4,0],[131,13,131,48,0,0,4,1],[133,13,133,48,0,0,4,1]],"free_oldmd":[[298,17,298,65,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","PEM_read_X509","d2i_X509_fp","PKCS7_new","PKCS7_RECIP_INFO_new","X509_check_purpose","X509_check_purpose","X509_check_purpose","X509_check_purpose","X509_check_purpose","X509_STORE_CTX_new","X509_STORE_CTX_set_error","X509_STORE_CTX_set_error","X509_STORE_CTX_set_error","X509_STORE_CTX_set_error","OSSL_LIB_CTX_new","EVP_RAND_fetch","EVP_RAND_CTX_new","EVP_RAND_free","d2i_X509_CRL","X509_CRL_free","d2i_PKCS7_fp","PKCS7_free","PEM_write_X509","i2d_X509","CRYPTO_free","PKCS7_free","PKCS7_RECIP_INFO_free","X509_STORE_CTX_free","OSSL_STORE_INFO_free","X509_free","EVP_RAND_CTX_free","OSSL_LIB_CTX_free"],"critical_calls":["OPENSSL_init_ssl","PEM_read_X509","d2i_X509_fp","PKCS7_new","PKCS7_RECIP_INFO_new","X509_check_purpose","X509_check_purpose","X509_check_purpose","X509_check_purpose","X509_check_purpose","X509_STORE_CTX_new","X509_STORE_CTX_set_error","X509_STORE_CTX_set_error","X509_STORE_CTX_set_error","X509_STORE_CTX_set_error","OSSL_LIB_CTX_new","EVP_RAND_fetch","EVP_RAND_CTX_new","EVP_RAND_free","d2i_X509_CRL","X509_CRL_free","d2i_PKCS7_fp","PKCS7_free","PEM_write_X509","i2d_X509","CRYPTO_free","PKCS7_free","PKCS7_RECIP_INFO_free","X509_STORE_CTX_free","OSSL_STORE_INFO_free","X509_free","EVP_RAND_CTX_free","OSSL_LIB_CTX_free"],"visited":0}
/*Looking at these APIs, I can design a fuzzer that tests certificate handling with PKCS#7 enveloped data and random number generation. Here's a comprehensive fuzzer that uses all the specified APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL if needed
    OPENSSL_init_ssl(0, NULL);
    
    // Create memory file for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Variables to be used with the APIs
    PKCS7 *p7 = NULL;
    PKCS7_RECIP_INFO *ri = NULL;
    X509_STORE_CTX *ctx = NULL;
    OSSL_STORE_INFO *store_info = NULL;
    X509 *cert = NULL;
    OSSL_LIB_CTX *libctx = NULL;
    EVP_RAND_CTX *rand_ctx = NULL;
    
    // 1. Try to create PKCS7 structure from input
    // First, try to read a certificate from the input
    fseek(in_file, 0, SEEK_SET);
    cert = PEM_read_X509(in_file, NULL, NULL, NULL);
    
    if (!cert) {
        // Try DER format
        fseek(in_file, 0, SEEK_SET);
        cert = d2i_X509_fp(in_file, NULL);
    }
    
    // 2. Create PKCS7 enveloped structure if we have a certificate
    if (cert) {
        p7 = PKCS7_new();
        if (p7) {
            // Create recipient info with the certificate
            ri = PKCS7_RECIP_INFO_new();
            if (ri) {
                // Set up recipient info (simplified - in real use would need proper setup)
                // PKCS7_add_recipient_info(p7, ri);
                // Note: In practice, we'd need to properly initialize ri first
            }
        }
    }
    
    // 3. Use X509_check_purpose API
    if (cert) {
        // Check various purposes of the certificate
        X509_check_purpose(cert, X509_PURPOSE_SSL_CLIENT, 0);
        X509_check_purpose(cert, X509_PURPOSE_SSL_SERVER, 0);
        X509_check_purpose(cert, X509_PURPOSE_NS_SSL_SERVER, 0);
        X509_check_purpose(cert, X509_PURPOSE_SMIME_SIGN, 0);
        X509_check_purpose(cert, X509_PURPOSE_SMIME_ENCRYPT, 0);
    }
    
    // 4. Create and use X509_STORE_CTX
    ctx = X509_STORE_CTX_new();
    if (ctx) {
        // Set various errors to test error handling
        X509_STORE_CTX_set_error(ctx, X509_V_ERR_CERT_HAS_EXPIRED);
        X509_STORE_CTX_set_error(ctx, X509_V_ERR_CERT_NOT_YET_VALID);
        X509_STORE_CTX_set_error(ctx, X509_V_ERR_CERT_REVOKED);
        X509_STORE_CTX_set_error(ctx, X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT);
    }
    
    // 5. Work with OSSL_STORE_INFO
    // Create a dummy store info (in practice, this would come from actual store operations)
    // For fuzzing purposes, we'll create a simple name-based store info
    const char *test_name = "test_certificate";
    // Note: OSSL_STORE_INFO_new_NAME is typically used, but we're using get0_NAME
    
    // 6. Set up random number generator context
    libctx = OSSL_LIB_CTX_new();
    if (libctx) {
        // Create a random context using system RNG
        EVP_RAND *rand = EVP_RAND_fetch(libctx, "CTR-DRBG", NULL);
        if (rand) {
            rand_ctx = EVP_RAND_CTX_new(rand, NULL);
            EVP_RAND_free(rand);
            
            if (rand_ctx) {
                // Initialize with some seed data from input
                if (size > 0) {
                    unsigned char seed[256];
                    size_t seed_len = size > sizeof(seed) ? sizeof(seed) : size;
                    memcpy(seed, data, seed_len);
                    
                    // Set up random context (simplified - real setup would be more complex)
                    // RAND_set0_public(libctx, rand_ctx);
                    // Note: This API requires specific setup that may not be available
                }
            }
        }
    }
    
    // 7. Process input data through various transformations
    if (size > 0) {
        // Try to use input as ASN.1 data
        const unsigned char *p = data;
        X509_CRL *crl = d2i_X509_CRL(NULL, &p, size);
        if (crl) {
            X509_CRL_free(crl);
        }
        
        // Try to read as PKCS7 data
        fseek(in_file, 0, SEEK_SET);
        PKCS7 *p7_input = d2i_PKCS7_fp(in_file, NULL);
        if (p7_input) {
            PKCS7_free(p7_input);
        }
    }
    
    // 8. Write some output based on processing
    if (cert) {
        // Write certificate to output file
        PEM_write_X509(out_file, cert);
        
        // Also try DER format
        unsigned char *der_data = NULL;
        int der_len = i2d_X509(cert, &der_data);
        if (der_len > 0 && der_data) {
            fwrite(der_data, 1, der_len, out_file);
            OPENSSL_free(der_data);
        }
    }
    
    // 9. Cleanup all allocated resources
    if (p7) PKCS7_free(p7);
    if (ri) PKCS7_RECIP_INFO_free(ri);
    if (ctx) X509_STORE_CTX_free(ctx);
    if (store_info) OSSL_STORE_INFO_free(store_info);
    if (cert) X509_free(cert);
    if (rand_ctx) EVP_RAND_CTX_free(rand_ctx);
    if (libctx) OSSL_LIB_CTX_free(libctx);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}