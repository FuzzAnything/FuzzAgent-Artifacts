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
//<ID> 127
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 19
//<Quality> {"density":4,"unique_branches":{"OSSL_CRMF_pbmp_new":[[36,9,36,53,0,0,4,1],[44,9,44,46,0,0,4,1],[46,9,46,50,0,0,4,1],[50,9,50,59,0,0,4,1],[58,9,58,76,0,0,4,1],[73,9,73,22,0,0,4,1],[77,9,77,52,0,0,4,1],[82,9,82,62,0,0,4,1],[92,9,92,76,0,0,4,1]],"X509_LOOKUP_meth_free":[[41,9,41,23,0,0,4,0]],"X509_LOOKUP_meth_new":[[26,9,26,23,0,0,4,0],[28,13,28,33,0,0,4,1]],"X509_LOOKUP_free":[[39,9,39,20,0,0,4,1],[41,9,41,30,0,0,4,0],[41,34,41,61,0,0,4,1]],"X509_ALGOR_set0":[[35,9,35,30,0,0,4,1],[44,9,44,30,0,0,4,0]],"X509_LOOKUP_new":[[26,9,26,20,0,0,4,1],[30,9,30,33,0,0,4,1]]},"library_calls":["ERR_load_ERR_strings","BIO_new_mem_buf","BIO_method_type","OSSL_FUNC_provider_random_bytes","OSSL_FUNC_BIO_free","X509_LOOKUP_meth_new","X509_LOOKUP_new","X509_LOOKUP_free","X509_LOOKUP_meth_free","OSSL_CRMF_pbmp_new","OSSL_CRMF_PBMPARAMETER_free","BIO_free","ERR_clear_error"],"critical_calls":["ERR_load_ERR_strings","BIO_new_mem_buf","BIO_method_type","OSSL_FUNC_provider_random_bytes","OSSL_FUNC_BIO_free","X509_LOOKUP_meth_new","X509_LOOKUP_new","X509_LOOKUP_free","X509_LOOKUP_meth_free","OSSL_CRMF_pbmp_new","OSSL_CRMF_PBMPARAMETER_free","BIO_free","ERR_clear_error"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL error strings
    ERR_load_ERR_strings();
    
    // Create a memory BIO from input data
    BIO *mem_bio = BIO_new_mem_buf(data, size);
    if (!mem_bio) {
        return 0;
    }
    
    // Get BIO method type
    int bio_type = BIO_method_type(mem_bio);
    
    // Create a dummy OSSL_DISPATCH structure
    // Note: We're creating minimal structures since we don't have real provider functions
    OSSL_DISPATCH dummy_dispatch[] = {
        {0, NULL}
    };
    
    // Get function pointers from dispatch table (will be NULL in our case)
    OSSL_FUNC_provider_random_bytes_fn *random_bytes_fn = 
        OSSL_FUNC_provider_random_bytes(dummy_dispatch);
    OSSL_FUNC_BIO_free_fn *bio_free_fn = 
        OSSL_FUNC_BIO_free(dummy_dispatch);
    
    // Create X509_LOOKUP - we need a method first
    // Create a simple lookup method structure
    X509_LOOKUP_METHOD *lookup_method = X509_LOOKUP_meth_new("Fuzzer Lookup");
    if (lookup_method) {
        X509_LOOKUP *lookup = X509_LOOKUP_new(lookup_method);
        
        // Clean up lookup
        if (lookup) {
            X509_LOOKUP_free(lookup);
        }
        X509_LOOKUP_meth_free(lookup_method);
    }
    
    // Create a CRMF PBMPARAMETER using input data for salt
    // Use first 32 bytes as salt (or all data if smaller)
    size_t salt_len = size > 32 ? 32 : size;
    int owfnid = 1;  // Default OWF
    size_t iter_count = 1000;
    int macnid = NID_hmac_sha1;
    
    OSSL_CRMF_PBMPARAMETER *pbmp = OSSL_CRMF_pbmp_new(NULL, salt_len, owfnid, iter_count, macnid);
    
    // If we got a valid PBMPARAMETER, we could process it further
    if (pbmp) {
        // In a real scenario, we would use the PBMPARAMETER
        // For fuzzing, we just create and free it
        OSSL_CRMF_PBMPARAMETER_free(pbmp);
    }
    
    // If we had a real bio_free_fn, we could use it
    // But since we don't, use standard BIO_free
    BIO_free(mem_bio);
    
    // Also demonstrate using the OSSL_FUNC_BIO_free if it existed
    if (bio_free_fn) {
        // This would be: bio_free_fn(mem_bio);
        // But we already freed it above
    }
    
    // Clean up any remaining errors
    ERR_clear_error();
    
    return 0;
}