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
//<ID> 63
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 4
//<Quality> {"density":4,"unique_branches":{"asn1_ex_c2i":[[926,5,926,30,0,0,4,0]],"d2i_PKCS7":[[71,9,71,18,0,0,4,1],[78,9,78,20,0,0,4,0],[78,9,78,20,0,0,4,1]]},"library_calls":["ERR_load_COMP_strings","sk_OCSP_RESPONSE_new","X509_STORE_new","X509_STORE_get1_objects","OPENSSL_sk_free","ossl_check_X509_OBJECT_sk_type","PKCS7_ATTR_VERIFY_it","d2i_CMS_ContentInfo","CMS_get1_certs","OPENSSL_sk_free","ossl_check_X509_sk_type","EC_KEY_new_by_curve_name","EC_KEY_generate_key","i2d_ECPrivateKey","CRYPTO_free","d2i_PKCS7","BIO_new_mem_buf","PKCS7_dataInit","BIO_free","OSSL_CMP_CTX_new","OSSL_CMP_CTX_reset_geninfo_ITAVs","RSA_meth_new","sk_OCSP_RESPONSE_free","X509_STORE_free","CMS_ContentInfo_free","EC_KEY_free","PKCS7_free","BIO_free","OSSL_CMP_CTX_free","RSA_meth_free"],"critical_calls":["ERR_load_COMP_strings","sk_OCSP_RESPONSE_new","X509_STORE_new","X509_STORE_get1_objects","OPENSSL_sk_free","ossl_check_X509_OBJECT_sk_type","PKCS7_ATTR_VERIFY_it","d2i_CMS_ContentInfo","CMS_get1_certs","OPENSSL_sk_free","ossl_check_X509_sk_type","EC_KEY_new_by_curve_name","EC_KEY_generate_key","i2d_ECPrivateKey","CRYPTO_free","d2i_PKCS7","BIO_new_mem_buf","PKCS7_dataInit","BIO_free","OSSL_CMP_CTX_new","OSSL_CMP_CTX_reset_geninfo_ITAVs","RSA_meth_new","sk_OCSP_RESPONSE_free","X509_STORE_free","CMS_ContentInfo_free","EC_KEY_free","PKCS7_free","BIO_free","OSSL_CMP_CTX_free","RSA_meth_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Initialize
    ERR_load_COMP_strings();

    // Split input for different purposes
    size_t part_size = size / 8;
    const uint8_t *current_ptr = data;
    
    // 1. Create OCSP response stack
    struct stack_st_OCSP_RESPONSE *ocsp_stack = sk_OCSP_RESPONSE_new(nullptr);
    
    // 2. Create X509 store and get objects
    X509_STORE *store = X509_STORE_new();
    if (store) {
        struct stack_st_X509_OBJECT *store_objects = X509_STORE_get1_objects(store);
        if (store_objects) {
            sk_X509_OBJECT_free(store_objects);
        }
    }

    // 3. Get PKCS7 attribute verify ASN1 item
    const ASN1_ITEM *attr_verify_item = PKCS7_ATTR_VERIFY_it();

    // 4. Parse CMS from input data
    CMS_ContentInfo *cms = nullptr;
    if (part_size > 0) {
        const unsigned char *cms_ptr = current_ptr;
        cms = d2i_CMS_ContentInfo(nullptr, &cms_ptr, part_size);
        current_ptr += part_size;
    }
    
    // Get certificates from CMS if parsed successfully
    if (cms) {
        struct stack_st_X509 *certs = CMS_get1_certs(cms);
        if (certs) {
            sk_X509_free(certs);
        }
    }

    // 5. Create EC key and convert to DER
    EC_KEY *ec_key = nullptr;
    if (part_size > 0 && current_ptr < data + size) {
        // Create new EC key
        ec_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
        if (ec_key) {
            // Generate key pair
            EC_KEY_generate_key(ec_key);
            
            // Convert to DER
            unsigned char *der_out = nullptr;
            int der_len = i2d_ECPrivateKey(ec_key, &der_out);
            if (der_out) {
                OPENSSL_free(der_out);
            }
        }
    }

    // 6. Parse PKCS7 from input data
    PKCS7 *p7 = nullptr;
    if (part_size > 0 && current_ptr < data + size) {
        const unsigned char *p7_ptr = current_ptr;
        p7 = d2i_PKCS7(nullptr, &p7_ptr, 
                      (current_ptr + part_size <= data + size) ? part_size : 
                      (data + size - current_ptr));
        current_ptr += part_size;
    }
    
    // Initialize BIO for PKCS7 data
    BIO *bio = nullptr;
    if (p7) {
        // Create memory BIO with remaining input data
        bio = BIO_new_mem_buf(current_ptr, 
                             (current_ptr < data + size) ? (data + size - current_ptr) : 0);
        if (bio) {
            BIO *init_bio = PKCS7_dataInit(p7, bio);
            if (init_bio) {
                BIO_free(init_bio);
            }
        }
    }

    // 7. Create CMP context and reset geninfo ITAVs
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(nullptr, nullptr);
    if (cmp_ctx) {
        OSSL_CMP_CTX_reset_geninfo_ITAVs(cmp_ctx);
    }

    // 8. Create RSA method
    RSA_METHOD *rsa_meth = RSA_meth_new("Fuzzer RSA Method", 0);
    
    // Additional processing with remaining data
    if (part_size > 0 && current_ptr < data + size) {
        // Create FILE pointer for reading
        FILE *in_file = fmemopen((void *)current_ptr, 
                                data + size - current_ptr, "rb");
        if (in_file) {
            // Read some data
            unsigned char buffer[256];
            fread(buffer, 1, sizeof(buffer), in_file);
            assert_file_closed(&in_file);;
        }
    }
    
    // Write output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(data, 1, (size > 1024) ? 1024 : size, out_file);
        assert_file_closed(&out_file);;
    }

    // Cleanup
    if (ocsp_stack) sk_OCSP_RESPONSE_free(ocsp_stack);
    if (store) X509_STORE_free(store);
    if (cms) CMS_ContentInfo_free(cms);
    if (ec_key) EC_KEY_free(ec_key);
    if (p7) PKCS7_free(p7);
    if (bio) BIO_free(bio);
    if (cmp_ctx) OSSL_CMP_CTX_free(cmp_ctx);
    if (rsa_meth) RSA_meth_free(rsa_meth);

    assert_file_closed(&out_file);
	return 0;
}