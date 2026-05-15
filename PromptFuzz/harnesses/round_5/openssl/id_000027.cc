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
//<ID> 249
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 5
//<Quality> {"density":6,"unique_branches":{"OSSL_STORE_SEARCH_by_name":[[949,9,949,23,0,0,4,1]],"ASYNC_WAIT_CTX_get_callback":[[200,9,200,30,0,0,4,0]],"CMS_ContentInfo_new_ex":[[60,9,60,19,0,0,4,0],[63,13,63,26,0,0,4,1]],"ASN1_item_new_ex":[[41,9,41,60,0,0,4,0]]},"library_calls":["d2i_DIST_POINT_NAME","CMS_ContentInfo_new_ex","PKCS7_ENC_CONTENT_new","PKCS12_SAFEBAG_new","PKCS12_SAFEBAG_get0_type","PKCS12_SAFEBAG_free","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_get_callback","ossl_check_SSL_COMP_copyfunc_type","X509_SIG_new","PEM_write_PKCS8","X509_NAME_new","X509_NAME_add_entry_by_txt","OSSL_STORE_SEARCH_by_name","OSSL_STORE_SEARCH_get0_name","OSSL_STORE_SEARCH_free","X509_NAME_free","DIST_POINT_NAME_free","CMS_ContentInfo_free","PKCS7_ENC_CONTENT_free","X509_SIG_free","ASYNC_WAIT_CTX_free"],"critical_calls":["d2i_DIST_POINT_NAME","CMS_ContentInfo_new_ex","PKCS7_ENC_CONTENT_new","PKCS12_SAFEBAG_new","PKCS12_SAFEBAG_get0_type","PKCS12_SAFEBAG_free","ASYNC_WAIT_CTX_new","ASYNC_WAIT_CTX_get_callback","ossl_check_SSL_COMP_copyfunc_type","X509_SIG_new","PEM_write_PKCS8","X509_NAME_new","X509_NAME_add_entry_by_txt","OSSL_STORE_SEARCH_by_name","OSSL_STORE_SEARCH_get0_name","OSSL_STORE_SEARCH_free","X509_NAME_free","DIST_POINT_NAME_free","CMS_ContentInfo_free","PKCS7_ENC_CONTENT_free","X509_SIG_free","ASYNC_WAIT_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for writing output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Variables for API calls
    DIST_POINT_NAME *dist_point = NULL;
    CMS_ContentInfo *cms = NULL;
    PKCS7_ENC_CONTENT *p7_enc = NULL;
    OSSL_STORE_SEARCH *search = NULL;
    X509_SIG *p8 = NULL;
    ASYNC_WAIT_CTX *wait_ctx = NULL;
    const unsigned char *p = data;
    long len = size;
    
    // 1. Decode DIST_POINT_NAME from input data
    dist_point = d2i_DIST_POINT_NAME(NULL, &p, len);
    
    // 2. Create new CMS_ContentInfo with explicit library context
    cms = CMS_ContentInfo_new_ex(NULL, NULL);
    
    // 3. Create new PKCS7_ENC_CONTENT
    p7_enc = PKCS7_ENC_CONTENT_new();
    
    // 4. Get type from PKCS12_SAFEBAG (simulated - create minimal bag)
    // Note: We need to create a minimal PKCS12_SAFEBAG for this API
    PKCS12_SAFEBAG *bag = PKCS12_SAFEBAG_new();
    if (bag) {
        const ASN1_OBJECT *bag_type = PKCS12_SAFEBAG_get0_type(bag);
        PKCS12_SAFEBAG_free(bag);
    }
    
    // 5. Get callback from ASYNC_WAIT_CTX
    ASYNC_callback_fn callback = NULL;
    void *callback_arg = NULL;
    wait_ctx = ASYNC_WAIT_CTX_new();
    if (wait_ctx) {
        ASYNC_WAIT_CTX_get_callback(wait_ctx, &callback, &callback_arg);
    }
    
    // 6. Use OPENSSL_sk_copyfunc for SSL_COMP
    // Create a dummy copy function and use the type-checking macro
    sk_SSL_COMP_copyfunc dummy_copy = [](const SSL_COMP *comp) -> SSL_COMP* {
        return NULL;
    };
    OPENSSL_sk_copyfunc checked_copy = ossl_check_SSL_COMP_copyfunc_type(dummy_copy);
    
    // 7. Write PKCS8 to file (simulated - create minimal X509_SIG)
    p8 = X509_SIG_new();
    if (p8) {
        // Write to file - this may fail but we call the API
        PEM_write_PKCS8(out_file, p8);
    }
    
    // 8. Get name from OSSL_STORE_SEARCH
    // Create a simple name search criterion
    X509_NAME *name = X509_NAME_new();
    if (name) {
        // Add a dummy entry to avoid empty name
        X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, 
                                  (unsigned char*)"test", -1, -1, 0);
        search = OSSL_STORE_SEARCH_by_name(name);
        if (search) {
            const X509_NAME *search_name = OSSL_STORE_SEARCH_get0_name(search);
            OSSL_STORE_SEARCH_free(search);
        }
        X509_NAME_free(name);
    }
    
    // Clean up allocated resources
    if (dist_point) DIST_POINT_NAME_free(dist_point);
    if (cms) CMS_ContentInfo_free(cms);
    if (p7_enc) PKCS7_ENC_CONTENT_free(p7_enc);
    if (p8) X509_SIG_free(p8);
    if (wait_ctx) ASYNC_WAIT_CTX_free(wait_ctx);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}