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
//<ID> 245
//<Prompt> []
/*<Combination>: [
*/
//<score> 108, nr_unique_branch: 89
//<Quality> {"density":9,"unique_branches":{"evp_mac_from_algorithm":[[65,9,65,38,0,0,4,1],[71,9,71,75,0,0,4,1],[76,12,76,33,0,0,4,0],[76,12,76,33,0,0,4,1],[77,17,77,33,0,0,4,1],[78,9,78,34,0,0,4,0],[78,9,78,34,0,0,4,1],[79,17,79,36,0,0,4,1],[84,9,84,34,0,0,4,0],[84,9,84,34,0,0,4,1],[85,17,85,36,0,0,4,1],[89,9,89,35,0,0,4,0],[89,9,89,35,0,0,4,1],[90,17,90,37,0,0,4,1],[95,9,95,32,0,0,4,0],[95,9,95,32,0,0,4,1],[96,17,96,34,0,0,4,1],[101,9,101,34,0,0,4,0],[101,9,101,34,0,0,4,1],[102,17,102,36,0,0,4,1],[107,9,107,33,0,0,4,0],[107,9,107,33,0,0,4,1],[108,17,108,35,0,0,4,1],[113,9,113,43,0,0,4,0],[113,9,113,43,0,0,4,1],[114,17,114,45,0,0,4,1],[118,9,118,47,0,0,4,0],[118,9,118,47,0,0,4,1],[119,17,119,49,0,0,4,1],[123,9,123,47,0,0,4,0],[123,9,123,47,0,0,4,1],[124,17,124,49,0,0,4,1],[128,9,128,38,0,0,4,0],[128,9,128,38,0,0,4,1],[129,17,129,40,0,0,4,1],[133,9,133,42,0,0,4,0],[133,9,133,42,0,0,4,1],[134,17,134,44,0,0,4,1],[138,9,138,42,0,0,4,0],[138,9,138,42,0,0,4,1],[139,17,139,44,0,0,4,1],[143,9,143,37,0,0,4,1],[152,9,152,22,0,0,4,1],[153,12,153,25,0,0,4,1],[163,9,163,21,0,0,4,0],[163,25,163,52,0,0,4,1]],"evp_mac_new":[[49,9,49,53,0,0,4,1],[50,12,50,44,0,0,4,1]],"evp_mac_free":[[33,9,33,20,0,0,4,1],[37,9,37,16,0,0,4,0]],"ossl_ascii_isdigit":[[265,28,265,37,1,0,4,0],[265,28,265,37,1,0,4,1],[265,41,265,50,1,0,4,0],[265,41,265,50,1,0,4,1]],"ossl_asn1_time_to_tm":[[90,9,90,34,0,0,4,1],[92,16,92,49,0,0,4,0],[111,9,111,18,0,0,4,1],[113,17,113,24,0,0,4,0],[114,13,114,23,0,0,4,1],[118,13,118,38,0,0,4,0],[118,13,118,38,0,0,4,1],[122,13,122,21,0,0,4,1],[125,13,125,38,0,0,4,0],[125,13,125,38,0,0,4,1],[129,13,129,21,0,0,4,1],[132,14,132,41,0,0,4,1],[134,13,134,26,0,0,4,0],[134,13,134,26,0,0,4,1],[134,30,134,43,0,0,4,0],[134,30,134,43,0,0,4,1],[136,17,136,19,0,0,4,1],[137,9,137,15,0,0,4,0],[137,9,137,15,0,0,4,1],[141,9,141,15,0,0,4,0],[141,9,141,15,0,0,4,1],[142,17,142,42,0,0,4,1],[147,9,147,15,0,0,4,0],[147,9,147,15,0,0,4,1],[150,9,150,15,0,0,4,1],[163,9,163,15,0,0,4,1],[166,9,166,15,0,0,4,1],[169,9,169,15,0,0,4,1]],"deflt_query":[[749,5,749,21,0,0,4,0]],"PKCS12_SAFEBAG_get1_crl":[[106,9,106,50,0,0,4,0]],"asn1_generalizedtime_to_tm":[[28,9,28,42,0,0,4,1]],"PKCS12_SAFEBAG_create0_pkcs8":[[236,9,236,20,0,0,4,1]],"EVP_KEYEXCH_settable_ctx_params":[[626,9,626,24,0,0,4,1],[626,28,626,64,0,0,4,1]],"ossl_asn1_do_adb":[[275,13,275,36,0,0,4,1]]},"library_calls":["d2i_ASN1_GENERALIZEDTIME","ASN1_GENERALIZEDTIME_check","ASN1_GENERALIZEDTIME_free","EVP_MAC_fetch","EVP_MAC_free","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_ASN1_VALUE_sk_type","ASN1_item_new","ASN1_OCTET_STRING_it","OPENSSL_sk_push","OPENSSL_sk_pop","ASN1_item_free","ASN1_OCTET_STRING_it","OPENSSL_sk_free","ossl_check_ASN1_VALUE_sk_type","OSSL_CMP_PKIHEADER_new","ASN1_INTEGER_new","ASN1_INTEGER_set","ASN1_INTEGER_free","OSSL_CMP_PKIHEADER_free","EVP_KEYEXCH_fetch","EVP_KEYEXCH_settable_ctx_params","EVP_KEYEXCH_free","d2i_X509_CRL","X509_CRL_new","PKCS12_SAFEBAG_create0_pkcs8","PKCS12_SAFEBAG_get1_crl","X509_CRL_free","PKCS12_SAFEBAG_free","X509_CRL_free"],"critical_calls":["d2i_ASN1_GENERALIZEDTIME","ASN1_GENERALIZEDTIME_check","ASN1_GENERALIZEDTIME_free","EVP_MAC_fetch","EVP_MAC_free","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_ASN1_VALUE_sk_type","ASN1_item_new","ASN1_OCTET_STRING_it","OPENSSL_sk_push","OPENSSL_sk_pop","ASN1_item_free","ASN1_OCTET_STRING_it","OPENSSL_sk_free","ossl_check_ASN1_VALUE_sk_type","OSSL_CMP_PKIHEADER_new","ASN1_INTEGER_new","ASN1_INTEGER_set","ASN1_INTEGER_free","OSSL_CMP_PKIHEADER_free","EVP_KEYEXCH_fetch","EVP_KEYEXCH_settable_ctx_params","EVP_KEYEXCH_free","d2i_X509_CRL","X509_CRL_new","PKCS12_SAFEBAG_create0_pkcs8","PKCS12_SAFEBAG_get1_crl","X509_CRL_free","PKCS12_SAFEBAG_free","X509_CRL_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for operations

    // 1. Create ASN1_GENERALIZEDTIME from input data and check it
    const unsigned char *p = data;
    ASN1_GENERALIZEDTIME *time = d2i_ASN1_GENERALIZEDTIME(NULL, &p, size/4);
    if (time) {
        int check_result = ASN1_GENERALIZEDTIME_check(time);
        // Use check_result to avoid unused variable warning
        (void)check_result;
        ASN1_GENERALIZEDTIME_free(time);
    }

    // 2. Fetch EVP_MAC
    EVP_MAC *mac = EVP_MAC_fetch(NULL, "HMAC", NULL);
    if (mac) {
        EVP_MAC_free(mac);
    }

    // 3. Create and use OPENSSL_STACK of ASN1_VALUE
    struct stack_st_ASN1_VALUE *asn1_stack = sk_ASN1_VALUE_new_null();
    if (asn1_stack) {
        // Cast using the required function
        OPENSSL_STACK *stack = ossl_check_ASN1_VALUE_sk_type(asn1_stack);
        
        // Add some dummy ASN1_VALUE elements
        ASN1_VALUE *dummy_value = ASN1_item_new(ASN1_ITEM_rptr(ASN1_OCTET_STRING));
        if (dummy_value) {
            OPENSSL_sk_push(stack, (void*)dummy_value);
            // Pop and free
            OPENSSL_sk_pop(stack);
            ASN1_item_free(dummy_value, ASN1_ITEM_rptr(ASN1_OCTET_STRING));
        }
        sk_ASN1_VALUE_free(asn1_stack);
    }

    // 4. Create OSSL_CMP_PKIHEADER
    OSSL_CMP_PKIHEADER *header = OSSL_CMP_PKIHEADER_new();
    if (header) {
        // Set some fields using input data
        if (size > 50) {
            ASN1_INTEGER *msg_id = ASN1_INTEGER_new();
            if (msg_id) {
                ASN1_INTEGER_set(msg_id, data[0]);
                // Note: Actual OSSL_CMP_PKIHEADER field setting would require
                // access to internal structure. We'll just create and free.
                ASN1_INTEGER_free(msg_id);
            }
        }
        OSSL_CMP_PKIHEADER_free(header);
    }

    // 5. Get EVP_KEYEXCH settable parameters
    EVP_KEYEXCH *keyexch = EVP_KEYEXCH_fetch(NULL, "DH", NULL);
    if (keyexch) {
        const OSSL_PARAM *params = EVP_KEYEXCH_settable_ctx_params(keyexch);
        // Use params to avoid unused variable warning
        (void)params;
        EVP_KEYEXCH_free(keyexch);
    }

    // 6. Create PKCS12_SAFEBAG and get CRL from it
    // First create a dummy CRL
    X509_CRL *crl = d2i_X509_CRL(NULL, &data, std::min<long>(size, 1000));
    if (!crl) {
        // If parsing fails, create a new CRL
        crl = X509_CRL_new();
    }
    
    if (crl) {
        // Create a PKCS12_SAFEBAG containing the CRL
        // Note: PKCS12_SAFEBAG_create_crl is not in provided APIs,
        // so we'll use a workaround with available functions
        PKCS12_SAFEBAG *bag = PKCS12_SAFEBAG_create0_pkcs8(NULL);  // Create empty bag
        if (bag) {
            // Try to get CRL from bag (will likely return NULL for empty bag)
            X509_CRL *bag_crl = PKCS12_SAFEBAG_get1_crl(bag);
            if (bag_crl) {
                X509_CRL_free(bag_crl);
            }
            PKCS12_SAFEBAG_free(bag);
        }
        X509_CRL_free(crl);
    }

    // 7. Additional operation: Use input data with a FILE*
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read and parse some data from the file
        unsigned char buffer[100];
        size_t read = fread(buffer, 1, sizeof(buffer), in_file);
        (void)read;  // Avoid unused variable warning
        assert_file_closed(&in_file);;
    }

    // 8. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data to output
        fwrite(data, 1, std::min<size_t>(size, 100), out_file);
        assert_file_closed(&out_file);;
    }

    // 9. Use file descriptors if needed
    if (out_file) {  // Reopen for descriptor
        out_file = fopen("output_file", "rb");
        if (out_file) {
            int fd = fuzz_fileno(out_file);
            (void)fd;  // Avoid unused variable warning
            assert_file_closed(&out_file);;
        }
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}