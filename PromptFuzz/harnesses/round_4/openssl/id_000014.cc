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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 45.333332, nr_unique_branch: 12
//<Quality> {"density":17,"unique_branches":{"X509_NAME_ENTRY_set_data":[[330,16,332,44,0,0,4,1]],"ASN1_STRING_set_by_NID":[[101,9,101,17,0,0,4,0]],"EVP_DigestUpdate":[[23,26,23,52,1,0,4,0]],"X509_NAME_ENTRY_create_by_OBJ":[[299,9,299,57,0,0,4,0],[306,9,306,21,0,0,4,0]],"X509_NAME_hash_ex":[[303,9,303,19,0,0,4,0],[305,9,305,21,0,0,4,0],[305,25,305,37,0,0,4,0],[306,12,306,75,0,0,4,0],[308,13,308,23,0,0,4,0]],"ASN1_mbstring_ncopy":[[116,26,116,43,0,0,4,0]],"X509_NAME_add_entry_by_txt":[[182,9,182,12,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","X509_NAME_new","X509_NAME_add_entry_by_txt","i2d_X509_NAME","CRYPTO_free","OSSL_LIB_CTX_new","X509_NAME_hash_ex","OSSL_LIB_CTX_free","X509_NAME_free","CRYPTO_malloc","OSSL_FUNC_signature_dupctx","OSSL_FUNC_digest_copyctx","CRYPTO_free","ossl_check_IPAddressOrRange_compfunc_type","NAMING_AUTHORITY_new","ASN1_IA5STRING_new","ASN1_STRING_set","NAMING_AUTHORITY_set0_authorityURL","NAMING_AUTHORITY_free","CRYPTO_new_ex_data","CRYPTO_malloc","CRYPTO_set_ex_data","CRYPTO_free","CRYPTO_free_ex_data","OSSL_ATTRIBUTE_TYPE_MAPPING_new","OBJ_nid2obj","OBJ_nid2obj","OBJ_dup","OBJ_dup","OSSL_ATTRIBUTE_TYPE_MAPPING_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_crypto","X509_NAME_new","X509_NAME_add_entry_by_txt","i2d_X509_NAME","CRYPTO_free","OSSL_LIB_CTX_new","X509_NAME_hash_ex","OSSL_LIB_CTX_free","X509_NAME_free","CRYPTO_malloc","OSSL_FUNC_signature_dupctx","OSSL_FUNC_digest_copyctx","CRYPTO_free","ossl_check_IPAddressOrRange_compfunc_type","NAMING_AUTHORITY_new","ASN1_IA5STRING_new","ASN1_STRING_set","NAMING_AUTHORITY_set0_authorityURL","NAMING_AUTHORITY_free","CRYPTO_new_ex_data","CRYPTO_malloc","CRYPTO_set_ex_data","CRYPTO_free","CRYPTO_free_ex_data","OSSL_ATTRIBUTE_TYPE_MAPPING_new","OBJ_nid2obj","OBJ_nid2obj","OBJ_dup","OBJ_dup","OSSL_ATTRIBUTE_TYPE_MAPPING_free","ERR_clear_error"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL library
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    
    // Create a temporary file for reading input
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create an output file
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
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used
    
    // 1. Create and use X509_NAME object
    X509_NAME *name = X509_NAME_new();
    if (name) {
        // Create a simple X509_NAME entry using the first part of data
        if (size > 0) {
            size_t len = size > 100 ? 100 : size;
            X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC,
                                       (const unsigned char*)data, len, -1, 0);
        }
        
        // Use i2d_X509_NAME - serialize X509_NAME to DER
        unsigned char *der = NULL;
        int der_len = i2d_X509_NAME(name, &der);
        if (der_len > 0 && der) {
            // Write DER to output file
            fwrite(der, 1, der_len, out_file);
            OPENSSL_free(der);
        }
        
        // Use X509_NAME_hash_ex
        OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
        int ok = 0;
        if (libctx) {
            unsigned long hash = X509_NAME_hash_ex(name, libctx, NULL, &ok);
            (void)hash; // Mark as used
            OSSL_LIB_CTX_free(libctx);
        }
        
        X509_NAME_free(name);
    }
    
    // 2. Create and use OSSL_DISPATCH structure for function pointers
    // Note: OSSL_DISPATCH is typically provided by providers, we'll create a minimal one
    OSSL_DISPATCH *dispatch = (OSSL_DISPATCH*)OPENSSL_malloc(sizeof(OSSL_DISPATCH));
    if (dispatch) {
        // These function pointers would normally be provided by providers
        // For fuzzing, we'll use NULL or dummy functions
        dispatch->function_id = 0;
        dispatch->function = NULL;
        
        // Get function pointers (though they'll be NULL in our case)
        OSSL_FUNC_signature_dupctx_fn *sig_dup = OSSL_FUNC_signature_dupctx(dispatch);
        OSSL_FUNC_digest_copyctx_fn *digest_copy = OSSL_FUNC_digest_copyctx(dispatch);
        (void)sig_dup;      // Mark as used
        (void)digest_copy;  // Mark as used
        
        OPENSSL_free(dispatch);
    }
    
    // 3. Create and use IPAddressOrRange comparison function
    // First we need to create the actual comparison function
    sk_IPAddressOrRange_compfunc cmp_func = [](const IPAddressOrRange* const *a,
                                              const IPAddressOrRange* const *b) -> int {
        // Simple comparison - in real usage this would compare IP addresses
        return 0;
    };
    
    // Get the OPENSSL_sk_compfunc wrapper
    OPENSSL_sk_compfunc ossl_cmp = ossl_check_IPAddressOrRange_compfunc_type(cmp_func);
    (void)ossl_cmp; // Mark as used
    
    // 4. Create and use NAMING_AUTHORITY
    NAMING_AUTHORITY *na = NAMING_AUTHORITY_new();
    if (na) {
        // Create an ASN1_IA5STRING from part of the input
        ASN1_IA5STRING *url = ASN1_IA5STRING_new();
        if (url && size > 0) {
            size_t url_len = size > 50 ? 50 : size;
            ASN1_STRING_set(url, data, url_len);
            NAMING_AUTHORITY_set0_authorityURL(na, url);
        }
        
        NAMING_AUTHORITY_free(na);
    }
    
    // 5. Create and use CRYPTO_EX_DATA
    CRYPTO_EX_DATA ex_data;
    CRYPTO_new_ex_data(0, NULL, &ex_data);
    
    // Set some data in the ex_data
    if (size > 0) {
        void *val = OPENSSL_malloc(1);
        if (val) {
            // Use the first byte of input as a simple value
            *(unsigned char*)val = data[0];
            CRYPTO_set_ex_data(&ex_data, 0, val);
            OPENSSL_free(val);
        }
    }
    
    CRYPTO_free_ex_data(0, NULL, &ex_data);
    
    // 6. Create and free OSSL_ATTRIBUTE_TYPE_MAPPING
    OSSL_ATTRIBUTE_TYPE_MAPPING *mapping = OSSL_ATTRIBUTE_TYPE_MAPPING_new();
    if (mapping) {
        // Set some values if we have enough data
        if (size >= 2) {
            // Create simple ASN1_OBJECTs from the input
            ASN1_OBJECT *local_obj = OBJ_nid2obj(data[0] % 100);
            ASN1_OBJECT *remote_obj = OBJ_nid2obj(data[1] % 100);
            
            if (local_obj && remote_obj) {
                mapping->local = OBJ_dup(local_obj);
                mapping->remote = OBJ_dup(remote_obj);
            }
        }
        
        // Free the mapping
        OSSL_ATTRIBUTE_TYPE_MAPPING_free(mapping);
    }
    
    // 7. Additional operations using the input data
    
    // Write some of the input data to output file
    if (size > 0) {
        size_t write_len = size > 1024 ? 1024 : size;
        fwrite(data, 1, write_len, out_file);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up OpenSSL error stack
    ERR_clear_error();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}