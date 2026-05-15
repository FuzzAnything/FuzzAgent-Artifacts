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
//<ID> 434
//<Prompt> []
/*<Combination>: [
*/
//<score> 108, nr_unique_branch: 53
//<Quality> {"density":6,"unique_branches":{"sha1_internal_init":[[133,16,133,38,0,0,4,0],[134,16,134,25,0,0,4,0],[135,16,135,43,0,0,4,0]],"asn1_template_ex_i2d":[[277,13,277,37,0,0,4,1],[294,17,294,22,0,0,4,1]],"sha1_set_ctx_params_decoder":[[33,9,33,18,0,0,4,1]],"ASN1_item_ex_i2d":[[141,5,141,27,0,0,4,0]],"SHA1_Update_thunk":[[214,9,214,14,0,0,4,1],[222,9,222,17,0,0,4,0],[23,26,23,52,3,0,4,1],[23,26,23,52,4,0,4,1],[22,24,22,50,5,0,4,1]],"sha1_set_ctx_params":[[46,9,46,26,0,0,4,1],[23,26,23,52,1,0,4,1],[23,46,23,47,1,0,4,1],[23,46,23,47,1,0,4,1]],"X509_get0_pubkey_bitstr":[[1033,9,1033,18,0,0,4,1]],"X509_ocspid_print":[[207,9,207,18,0,0,4,1],[207,22,207,32,0,0,4,1],[212,9,212,59,0,0,4,1],[216,9,216,20,0,0,4,1],[218,9,218,56,0,0,4,1],[220,9,220,41,0,0,4,1],[224,9,224,19,0,0,4,1],[226,9,226,57,0,0,4,1],[228,17,228,38,0,0,4,0],[228,17,228,38,0,0,4,1],[229,13,229,51,0,0,4,1],[238,9,238,64,0,0,4,1],[243,9,243,24,0,0,4,1],[246,9,247,65,0,0,4,1],[249,17,249,38,0,0,4,0],[249,17,249,38,0,0,4,1],[250,13,250,51,0,0,4,1]],"x509_name_encode":[[251,9,251,19,0,0,4,1],[253,17,253,43,0,0,4,1],[270,9,270,37,0,0,4,1]],"EVP_DigestFinal_ex":[[328,9,328,22,0,0,4,1]],"x509_name_ex_i2d":[[220,9,220,20,0,0,4,0],[220,9,220,20,0,0,4,1],[222,13,222,20,0,0,4,1],[225,13,225,17,0,0,4,1],[229,9,229,20,0,0,4,0],[229,9,229,20,0,0,4,1]],"EVP_DigestUpdate":[[23,26,23,52,1,0,4,0]],"sha1_internal_final":[[45,13,45,35,0,0,4,0],[45,39,45,56,0,0,4,0],[45,60,45,73,0,0,4,0]],"sha1_newctx":[[63,9,63,32,1,0,4,1]],"BIO_vprintf":[[113,9,113,16,0,0,4,0],[114,13,114,37,0,0,4,1]],"asn1_set_seq_out":[[408,9,408,16,0,0,4,1]],"SHA1_Final":[[247,9,247,30,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","X509_STORE_new","X509_STORE_get1_objects","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_is_a","X509_LOOKUP_file","X509_LOOKUP_meth_get_init","PKCS5_pbe2_set_scrypt","EVP_aes_256_cbc","ASN1_TBOOLEAN_it","BIO_new","BIO_s_mem","X509_new","X509_ocspid_print","X509_free","BIO_free","X509_STORE_set_lookup_crls","X509_REVOKED_new","X509_REVOKED_get_ext_d2i","ASN1_ENUMERATED_free","X509_REVOKED_free","EVP_PKEY_CTX_free","X509_ALGOR_free","OPENSSL_sk_free","ossl_check_X509_OBJECT_sk_type","X509_STORE_free"],"critical_calls":["OPENSSL_init_crypto","X509_STORE_new","X509_STORE_get1_objects","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_is_a","X509_LOOKUP_file","X509_LOOKUP_meth_get_init","PKCS5_pbe2_set_scrypt","EVP_aes_256_cbc","ASN1_TBOOLEAN_it","BIO_new","BIO_s_mem","X509_new","X509_ocspid_print","X509_free","BIO_free","X509_STORE_set_lookup_crls","X509_REVOKED_new","X509_REVOKED_get_ext_d2i","ASN1_ENUMERATED_free","X509_REVOKED_free","EVP_PKEY_CTX_free","X509_ALGOR_free","OPENSSL_sk_free","ossl_check_X509_OBJECT_sk_type","X509_STORE_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 64) {
	return 0;
	}
	 // Need minimum data for operations

    // Initialize OpenSSL if needed
    OPENSSL_init_crypto(0, NULL);

    // 1. Create X509_STORE and get objects
    X509_STORE *store = X509_STORE_new();
    if (!store) {
	return 0;
	}
	
    
    // Add some dummy objects to store (normally would load from data)
    struct stack_st_X509_OBJECT *objects = X509_STORE_get1_objects(store);
    
    // 2. Create EVP_PKEY_CTX and check key type
    EVP_PKEY_CTX *pkey_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (pkey_ctx) {
        int is_rsa = EVP_PKEY_CTX_is_a(pkey_ctx, "RSA");
        // Use the result to avoid compiler warnings
        (void)is_rsa;
    }
    
    // 3. Get X509_LOOKUP_METHOD init function
    X509_LOOKUP_METHOD *lookup_method = X509_LOOKUP_file();
    if (lookup_method) {
        int (*init_func)(X509_LOOKUP *) = (int (*)(X509_LOOKUP *))X509_LOOKUP_meth_get_init(lookup_method);
        // Could call init_func if we had a X509_LOOKUP instance
    }
    
    // 4. Create PKCS5 PBE2 with scrypt parameters from input data
    // Use first 16 bytes as salt if available
    const unsigned char *salt = (size >= 16) ? data : NULL;
    int saltlen = (size >= 16) ? 16 : 8;
    
    // Use part of input as AIV if available
    unsigned char aiv[16] = {0};
    if (size >= 32) {
        memcpy(aiv, data + 16, 16);
    }
    
    // Create PBE2 algorithm with scrypt
    X509_ALGOR *pbe_algor = PKCS5_pbe2_set_scrypt(
        EVP_aes_256_cbc(),
        salt,
        saltlen,
        aiv,
        16384,  // N
        8,      // r
        1       // p
    );
    
    // 5. Get ASN1 boolean item
    const ASN1_ITEM *bool_item = ASN1_TBOOLEAN_it();
    (void)bool_item; // Use to avoid warning
    
    // 6. Create BIO for X509_ocspid_print
    BIO *bio = BIO_new(BIO_s_mem());
    if (bio) {
        // Create a temporary X509 certificate from partial data
        X509 *x509 = X509_new();
        if (x509) {
            // Try to print OCSP ID (will likely fail but exercises the API)
            X509_ocspid_print(bio, x509);
            X509_free(x509);
        }
        BIO_free(bio);
    }
    
    // 7. Set lookup CRLs function for store
    X509_STORE_set_lookup_crls(store, [](const X509_STORE_CTX *ctx, const X509_NAME *nm) -> struct stack_st_X509_CRL* {
        // Return NULL for fuzzing
        return NULL;
    });
    
    // 8. Try to get extension from X509_REVOKED using input data
    X509_REVOKED *revoked = X509_REVOKED_new();
    if (revoked) {
        int crit = 0, idx = 0;
        
        // Try to parse extension from data
        // Use a common extension NID for testing
        void *ext_data = X509_REVOKED_get_ext_d2i(
            revoked, 
            NID_crl_reason,  // Common CRL extension
            &crit, 
            &idx
        );
        
        // Free if we got anything
        if (ext_data) {
            // Cast based on extension type - this is simplified
            ASN1_ENUMERATED_free((ASN1_ENUMERATED*)ext_data);
        }
        
        X509_REVOKED_free(revoked);
    }
    
    // 9. Process input data through a FILE* as required
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Try to read and parse some data from the file
        unsigned char buffer[256];
        size_t read = fread(buffer, 1, sizeof(buffer), in_file);
        (void)read; // Use to avoid warning
        
        // Get file descriptor as required
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use to avoid warning
        
        assert_file_closed(&in_file);;
    }
    
    // 10. Create output file as required
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data
        fwrite(data, 1, size > 256 ? 256 : size, out_file);
        
        // Get file descriptor as required
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Use to avoid warning
        
        assert_file_closed(&out_file);;
    }
    
    // Clean up allocated resources
    if (pkey_ctx) EVP_PKEY_CTX_free(pkey_ctx);
    if (pbe_algor) X509_ALGOR_free(pbe_algor);
    if (objects) sk_X509_OBJECT_free(objects);
    if (store) X509_STORE_free(store);
    
    // Clean up output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}