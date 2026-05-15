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
//<ID> 69
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 8
//<Quality> {"density":5,"unique_branches":{"ASIdentifierChoice_canonize":[[361,9,361,23,0,0,4,0]],"internal_find":[[404,12,404,21,0,0,4,0]],"X509v3_asid_canonize":[[489,13,489,25,0,0,4,1],[489,30,489,70,0,0,4,0],[489,74,489,112,0,0,4,0]],"ossl_provider_find":[[432,13,432,71,0,0,4,1],[435,13,435,25,0,0,4,1]],"OSSL_PROVIDER_available":[[1676,9,1676,21,0,0,4,1]]},"library_calls":["OPENSSL_init_ssl","d2i_X509_REQ_fp","X509_REQ_new","ASN1_BIT_STRING_new","ASN1_STRING_set","X509_REQ_set0_signature","UI_new","UI_OpenSSL","UI_set_method","CRYPTO_get_ex_new_index","UI_set_ex_data","X509_STORE_CTX_new","X509_STORE_new","X509_STORE_CTX_init","X509_STORE_CTX_verify","X509_STORE_free","ASIdentifiers_new","X509v3_asid_canonize","CRYPTO_malloc","CRYPTO_realloc","OSSL_LIB_CTX_new","OSSL_PROVIDER_available","X509_REQ_free","ASN1_BIT_STRING_free","UI_free","X509_STORE_CTX_free","ASIdentifiers_free","CRYPTO_free","OSSL_LIB_CTX_free"],"critical_calls":["OPENSSL_init_ssl","d2i_X509_REQ_fp","X509_REQ_new","ASN1_BIT_STRING_new","ASN1_STRING_set","X509_REQ_set0_signature","UI_new","UI_OpenSSL","UI_set_method","CRYPTO_get_ex_new_index","UI_set_ex_data","X509_STORE_CTX_new","X509_STORE_new","X509_STORE_CTX_init","X509_STORE_CTX_verify","X509_STORE_free","ASIdentifiers_new","X509v3_asid_canonize","CRYPTO_malloc","CRYPTO_realloc","OSSL_LIB_CTX_new","OSSL_PROVIDER_available","X509_REQ_free","ASN1_BIT_STRING_free","UI_free","X509_STORE_CTX_free","ASIdentifiers_free","CRYPTO_free","OSSL_LIB_CTX_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL if needed
    OPENSSL_init_ssl(0, NULL);
    
    // Create FILE* for input data
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
    
    // Create X509_REQ from input data
    X509_REQ *req = d2i_X509_REQ_fp(in_file, NULL);
    if (!req) {
        // If parsing fails, create empty X509_REQ
        req = X509_REQ_new();
    }
    
    // Create ASN1_BIT_STRING for signature
    ASN1_BIT_STRING *psig = ASN1_BIT_STRING_new();
    if (psig && size > 0) {
        // Use first min(128, size) bytes for signature data
        size_t sig_len = size > 128 ? 128 : size;
        ASN1_STRING_set(psig, data, sig_len);
        
        // Call X509_REQ_set0_signature
        X509_REQ_set0_signature(req, psig);
        // Note: req now owns psig, don't free separately
        psig = NULL;
    }
    
    // Create UI object
    UI *ui = UI_new();
    if (ui) {
        // Set UI method (simplified)
        const UI_METHOD *method = UI_OpenSSL();
        UI_set_method(ui, method);
        
        // Call UI_set_ex_data with some index
        static int ui_ex_data_index = -1;
        if (ui_ex_data_index < 0) {
            ui_ex_data_index = UI_get_ex_new_index(0, NULL, NULL, NULL, NULL);
        }
        if (ui_ex_data_index >= 0) {
            // Use a pointer to part of input data
            void *ex_data = (void*)(data + (size > 16 ? 16 : 0));
            UI_set_ex_data(ui, ui_ex_data_index, ex_data);
        }
    }
    
    // Create X509_STORE_CTX and verify
    X509_STORE_CTX *ctx = X509_STORE_CTX_new();
    if (ctx) {
        X509_STORE *store = X509_STORE_new();
        if (store) {
            X509_STORE_CTX_init(ctx, store, NULL, NULL);
            // Call X509_STORE_CTX_verify
            X509_STORE_CTX_verify(ctx);
            X509_STORE_free(store);
        }
    }
    
    // Create and canonize ASIdentifiers
    ASIdentifiers *asid = ASIdentifiers_new();
    if (asid) {
        // Initialize with some data from input
        if (size > 32) {
            // Try to parse minimal ASIdentifiers structure
            // This is simplified - real usage would need proper DER encoding
            X509v3_asid_canonize(asid);
        }
    }
    
    // Use CRYPTO_realloc for memory management
    void *allocated_mem = OPENSSL_malloc(16);
    if (allocated_mem && size > 0) {
        // Copy some data into allocated memory
        size_t copy_len = size > 16 ? 16 : size;
        memcpy(allocated_mem, data, copy_len);
        
        // Reallocate to larger size
        void *reallocated_mem = CRYPTO_realloc(allocated_mem, 32, __FILE__, __LINE__);
        if (reallocated_mem) {
            allocated_mem = reallocated_mem;
            // Use the reallocated memory
            if (size > 32) {
                memcpy((char*)allocated_mem + 16, data + 16, 16);
            }
        }
    }
    
    // Check provider availability
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    if (libctx) {
        // Use part of input as provider name (null-terminated)
        char provider_name[64] = {0};
        size_t name_len = size > 63 ? 63 : size;
        if (name_len > 0) {
            memcpy(provider_name, data, name_len);
            provider_name[name_len] = '\0';
            
            // Call OSSL_PROVIDER_available
            OSSL_PROVIDER_available(libctx, provider_name);
        }
    }
    
    // Write some output using the output file
    if (size > 0 && out_file) {
        // Write processed data to output file
        fwrite(data, 1, size > 1024 ? 1024 : size, out_file);
    }
    
    // Cleanup - release all allocated resources
    if (req) X509_REQ_free(req);
    if (psig) ASN1_BIT_STRING_free(psig); // Only if not transferred to req
    if (ui) UI_free(ui);
    if (ctx) X509_STORE_CTX_free(ctx);
    if (asid) ASIdentifiers_free(asid);
    if (allocated_mem) OPENSSL_free(allocated_mem);
    if (libctx) OSSL_LIB_CTX_free(libctx);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}