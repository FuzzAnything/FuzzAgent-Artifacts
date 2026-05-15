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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 50, nr_unique_branch: 13
//<Quality> {"density":10,"unique_branches":{"GENERAL_NAME_set0_value":[[178,13,178,17,0,0,4,1],[179,5,179,18,0,0,4,1],[183,5,183,22,0,0,4,1],[187,5,187,23,0,0,4,1],[191,5,191,19,0,0,4,1],[192,5,192,17,0,0,4,0],[193,5,193,17,0,0,4,1],[197,5,197,21,0,0,4,1],[201,5,201,19,0,0,4,1],[205,5,205,17,0,0,4,1]],"CMS_sign_ex":[[545,9,545,32,0,0,4,0]],"X509_VERIFY_PARAM_set1":[[499,9,499,19,0,0,4,1]],"UI_method_get_data_destructor":[[769,9,769,23,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","X509_STORE_new","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set_flags","X509_STORE_set1_param","GENERAL_NAME_new","ASN1_IA5STRING_new","ASN1_STRING_set","GENERAL_NAME_set0_value","ASN1_IA5STRING_free","ASN1_TYPE_new","ASN1_TYPE_set_octetstring","UI_OpenSSL","UI_method_get_data_destructor","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","CMS_sign","CMS_add1_recipient_cert","OPENSSL_sk_push","ossl_check_CMS_RecipientInfo_sk_type","ossl_check_CMS_RecipientInfo_type","OPENSSL_sk_pop_free","ossl_check_CMS_RecipientInfo_sk_type","ossl_check_CMS_RecipientInfo_freefunc_type","CMS_ContentInfo_free","PEM_read_X509","d2i_X509_fp","BIO_new_fp","X509_print","BIO_free","X509_free","GENERAL_NAME_free","ASN1_TYPE_free","X509_STORE_free","X509_VERIFY_PARAM_free","UI_OpenSSL","UI_destroy_method"],"critical_calls":["OPENSSL_init_ssl","X509_STORE_new","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set_flags","X509_STORE_set1_param","GENERAL_NAME_new","ASN1_IA5STRING_new","ASN1_STRING_set","GENERAL_NAME_set0_value","ASN1_TYPE_new","ASN1_TYPE_set_octetstring","UI_OpenSSL","UI_method_get_data_destructor","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","CMS_sign","CMS_add1_recipient_cert","OPENSSL_sk_push","ossl_check_CMS_RecipientInfo_type","ossl_check_CMS_RecipientInfo_sk_type","OPENSSL_sk_pop_free","ossl_check_CMS_RecipientInfo_freefunc_type","ossl_check_CMS_RecipientInfo_sk_type","CMS_ContentInfo_free","PEM_read_X509","d2i_X509_fp","BIO_new_fp","X509_print","BIO_free","X509_free","GENERAL_NAME_free","ASN1_TYPE_free","X509_STORE_free","X509_VERIFY_PARAM_free","UI_OpenSSL","UI_destroy_method"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL if needed
    OPENSSL_init_ssl(0, NULL);
    
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
    
    // 1. X509_STORE_set1_param: Set verification parameters for certificate store
    X509_STORE *store = X509_STORE_new();
    X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
    if (store && param) {
        // Set some basic parameters
        X509_VERIFY_PARAM_set_flags(param, X509_V_FLAG_ALLOW_PROXY_CERTS);
        X509_STORE_set1_param(store, param);
    }
    
    // 2. GENERAL_NAME_set0_value: Create and set value for GENERAL_NAME
    GENERAL_NAME *gen_name = GENERAL_NAME_new();
    if (gen_name) {
        // Create an ASN1_IA5STRING from input data (truncated to safe size)
        size_t name_len = size > 100 ? 100 : size;
        ASN1_IA5STRING *ia5 = ASN1_IA5STRING_new();
        if (ia5 && name_len > 0) {
            ASN1_STRING_set(ia5, data, name_len);
            // Set the GENERAL_NAME to use this IA5STRING as DNS name
            GENERAL_NAME_set0_value(gen_name, GEN_DNS, ia5);
        } else if (ia5) {
            ASN1_IA5STRING_free(ia5);
        }
    }
    
    // 3. ASN1_TYPE_set_octetstring: Set ASN1_TYPE to octet string
    ASN1_TYPE *asn1_type = ASN1_TYPE_new();
    if (asn1_type && size > 0) {
        // Use a portion of input data for octet string
        size_t octet_len = size > 50 ? 50 : size;
        unsigned char *octet_data = (unsigned char *)malloc(octet_len);
        if (octet_data) {
            memcpy(octet_data, data, octet_len);
            ASN1_TYPE_set_octetstring(asn1_type, octet_data, octet_len);
            free(octet_data);
        }
    }
    
    // 4. UI_method_get_data_destructor: Get UI method destructor
    UI_METHOD *ui_method = UI_OpenSSL();
    if (ui_method) {
        // The actual function returns a function pointer, we just call it
        // to demonstrate usage
        UI_method_get_data_destructor(ui_method);
    }
    
    // 5. sk_CMS_RecipientInfo_freefunc_thunk: Free function for CMS RecipientInfo stack
    // Create a dummy stack to demonstrate the free function
    STACK_OF(CMS_RecipientInfo) *sk = sk_CMS_RecipientInfo_new_null();
    if (sk) {
        // Add a dummy recipient info if possible
        CMS_ContentInfo *cms = CMS_sign(NULL, NULL, NULL, NULL, 0);
        if (cms) {
            CMS_RecipientInfo *ri = CMS_add1_recipient_cert(cms, NULL, 0);
            if (ri) {
                sk_CMS_RecipientInfo_push(sk, ri);
            }
        }
        
        // Use the thunk function to free stack elements
        sk_CMS_RecipientInfo_pop_free(sk, (void (*)(CMS_RecipientInfo *))sk_CMS_RecipientInfo_freefunc_thunk);
        
        if (cms) {
            CMS_ContentInfo_free(cms);
        }
    }
    
    // Consume input data by reading from the FILE*
    // Try to parse as X509 certificate from input
    X509 *x509 = PEM_read_X509(in_file, NULL, NULL, NULL);
    if (!x509) {
        // Reset file pointer and try different format
        fseek(in_file, 0, SEEK_SET);
        x509 = d2i_X509_fp(in_file, NULL);
    }
    
    // If we got a certificate, print it to output file
    if (x509) {
        BIO *bio = BIO_new_fp(out_file, BIO_NOCLOSE);
        if (bio) {
            X509_print(bio, x509);
            BIO_free(bio);
        }
        X509_free(x509);
    }
    
    // Clean up all allocated resources
    if (gen_name) {
        GENERAL_NAME_free(gen_name);
    }
    
    if (asn1_type) {
        ASN1_TYPE_free(asn1_type);
    }
    
    if (store) {
        X509_STORE_free(store);
    }
    
    if (param) {
        X509_VERIFY_PARAM_free(param);
    }
    
    // Clean up UI method if we created one
    if (ui_method && ui_method != UI_OpenSSL()) {
        UI_destroy_method(ui_method);
    }
    
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