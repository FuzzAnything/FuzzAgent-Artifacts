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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["ERR_load_COMP_strings","RSA_meth_new","sk_OCSP_RESPONSE_new","RSA_meth_free","X509_STORE_new","X509_STORE_get1_objects","PKCS7_ATTR_VERIFY_it","OPENSSL_sk_pop_free","ossl_check_X509_OBJECT_sk_type","ossl_check_X509_OBJECT_freefunc_type","X509_STORE_free","OSSL_CMP_CTX_new","OSSL_CMP_CTX_reset_geninfo_ITAVs","OSSL_CMP_CTX_free","d2i_PKCS7_fp","BIO_new_mem_buf","PKCS7_dataInit","BIO_read","BIO_free_all","BIO_free","CMS_ContentInfo_new","CMS_get1_certs","OPENSSL_sk_pop_free","ossl_check_X509_sk_type","ossl_check_X509_freefunc_type","CMS_ContentInfo_free","PKCS7_free","EC_KEY_new_by_curve_name","EC_KEY_generate_key","i2d_ECPrivateKey","CRYPTO_free","EC_KEY_free","sk_OCSP_RESPONSE_pop_free","RSA_meth_free"],"critical_calls":["ERR_load_COMP_strings","RSA_meth_new","sk_OCSP_RESPONSE_new","X509_STORE_new","X509_STORE_get1_objects","PKCS7_ATTR_VERIFY_it","OPENSSL_sk_pop_free","ossl_check_X509_OBJECT_freefunc_type","ossl_check_X509_OBJECT_sk_type","X509_STORE_free","OSSL_CMP_CTX_new","OSSL_CMP_CTX_reset_geninfo_ITAVs","OSSL_CMP_CTX_free","d2i_PKCS7_fp","BIO_new_mem_buf","PKCS7_dataInit","BIO_read","BIO_free_all","BIO_free","CMS_ContentInfo_new","CMS_get1_certs","OPENSSL_sk_pop_free","ossl_check_X509_freefunc_type","ossl_check_X509_sk_type","CMS_ContentInfo_free","PKCS7_free","EC_KEY_new_by_curve_name","EC_KEY_generate_key","i2d_ECPrivateKey","CRYPTO_free","EC_KEY_free","sk_OCSP_RESPONSE_pop_free","RSA_meth_free"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified OpenSSL APIs in a coherent sequence:

*/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations

    // Initialize error strings
    ERR_load_COMP_strings();

    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output FILE*
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create RSA method
    RSA_METHOD *rsa_meth = RSA_meth_new("FuzzRSA", 0);
    if (!rsa_meth) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create OCSP response stack
    struct stack_st_OCSP_RESPONSE *ocsp_stack = sk_OCSP_RESPONSE_new(NULL);
    if (!ocsp_stack) {
        RSA_meth_free(rsa_meth);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create and initialize X509 store
    X509_STORE *store = X509_STORE_new();
    if (store) {
        // Get objects from store
        struct stack_st_X509_OBJECT *objects = X509_STORE_get1_objects(store);
        
        // Use PKCS7 attribute verify item
        const ASN1_ITEM *attr_verify_it = PKCS7_ATTR_VERIFY_it();
        (void)attr_verify_it; // Use it to avoid unused variable warning
        
        if (objects) {
            sk_X509_OBJECT_pop_free(objects, X509_OBJECT_free);
        }
        X509_STORE_free(store);
    }

    // Create CMP context and reset geninfo ITAVs
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(NULL, NULL);
    if (cmp_ctx) {
        OSSL_CMP_CTX_reset_geninfo_ITAVs(cmp_ctx);
        OSSL_CMP_CTX_free(cmp_ctx);
    }

    // Try to parse input as PKCS7 structure
    fseek(in_file, 0, SEEK_SET);
    PKCS7 *p7 = d2i_PKCS7_fp(in_file, NULL);
    if (p7) {
        // Create BIO from input data for PKCS7_dataInit
        BIO *bio_in = BIO_new_mem_buf(data, size);
        if (bio_in) {
            BIO *p7bio = PKCS7_dataInit(p7, bio_in);
            if (p7bio) {
                // Process some data through the pipeline
                char buffer[1024];
                BIO_read(p7bio, buffer, sizeof(buffer) > size ? size : sizeof(buffer));
                BIO_free_all(p7bio);
            }
            BIO_free(bio_in);
        }
        
        // Try to get CMS structure from PKCS7 (if possible)
        CMS_ContentInfo *cms = CMS_ContentInfo_new();
        if (cms) {
            struct stack_st_X509 *certs = CMS_get1_certs(cms);
            if (certs) {
                sk_X509_pop_free(certs, X509_free);
            }
            CMS_ContentInfo_free(cms);
        }
        
        PKCS7_free(p7);
    }

    // Create EC key and serialize it
    EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_key) {
        // Generate key (ignoring failure since we just need to test serialization)
        EC_KEY_generate_key(ec_key);
        
        // Serialize EC private key
        unsigned char *ec_buf = NULL;
        int ec_len = i2d_ECPrivateKey(ec_key, &ec_buf);
        if (ec_len > 0 && ec_buf) {
            // Write serialized key to output file
            fwrite(ec_buf, 1, ec_len, out_file);
            OPENSSL_free(ec_buf);
        }
        EC_KEY_free(ec_key);
    }

    // Cleanup OCSP stack
    sk_OCSP_RESPONSE_pop_free(ocsp_stack, OCSP_RESPONSE_free);
    
    // Cleanup RSA method
    RSA_meth_free(rsa_meth);
    
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