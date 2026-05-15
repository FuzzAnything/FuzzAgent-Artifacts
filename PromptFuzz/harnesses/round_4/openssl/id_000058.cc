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
//<ID> 831
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 7
//<Quality> {"density":10,"unique_branches":{"i2c_ibuf":[[114,24,114,32,0,0,4,0],[119,38,119,46,0,0,4,1],[121,22,121,30,0,0,4,1]],"asn1_ex_i2c":[[574,5,574,24,0,0,4,0],[576,13,576,25,0,0,4,1],[578,13,578,36,0,0,4,1],[597,5,597,27,0,0,4,0]]},"library_calls":["BIO_new_fp","BIO_new_fp","BIO_free","d2i_CMS_bio","i2d_CMS_bio","BIO_ctrl","X509_SIG_it","ASN1_item_new","ASN1_item_d2i","CMS_ContentInfo_new","i2d_CMS_bio","ASN1_item_free","X509_SIG_it","ASN1_item_new","ASN1_item_free","OSSL_FUNC_core_thread_start","EVP_sha256","EVP_MD_gettable_ctx_params","OSSL_IETF_ATTR_SYNTAX_new","OSSL_IETF_ATTR_SYNTAX_get_value_num","OSSL_IETF_ATTR_SYNTAX_free","X509_SIG_it","CMS_ContentInfo_free","BIO_free","BIO_free"],"critical_calls":["BIO_new_fp","BIO_new_fp","d2i_CMS_bio","BIO_ctrl","X509_SIG_it","ASN1_item_new","ASN1_item_d2i","CMS_ContentInfo_new","i2d_CMS_bio","ASN1_item_free","X509_SIG_it","ASN1_item_new","ASN1_item_free","OSSL_FUNC_core_thread_start","EVP_sha256","EVP_MD_gettable_ctx_params","OSSL_IETF_ATTR_SYNTAX_new","OSSL_IETF_ATTR_SYNTAX_get_value_num","OSSL_IETF_ATTR_SYNTAX_free","X509_SIG_it","CMS_ContentInfo_free","BIO_free","BIO_free"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified OpenSSL APIs to achieve the event of parsing and writing CMS (Cryptographic Message Syntax) data:

*/


extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE pointer for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create a BIO for reading
    BIO *in_bio = BIO_new_fp(in_file, BIO_NOCLOSE);
    if (!in_bio) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create a BIO for writing
    BIO *out_bio = BIO_new_fp(out_file, BIO_NOCLOSE);
    if (!out_bio) {
        BIO_free(in_bio);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Event: Parse and write CMS data from input stream
    // Step 1: Try to read CMS data from input BIO
    CMS_ContentInfo *cms = d2i_CMS_bio(in_bio, NULL);
    
    // If we successfully read CMS data, write it back using i2d_CMS_bio
    if (cms) {
        // Use i2d_CMS_bio to write CMS data to output BIO
        i2d_CMS_bio(out_bio, cms);
    } else {
        // If reading CMS failed, try alternative parsing
        // Rewind the input BIO
        BIO_seek(in_bio, 0);
        
        // Create an ASN1_VALUE using X509_SIG_it()
        const ASN1_ITEM *sig_it = X509_SIG_it();
        ASN1_VALUE *asn1_val = ASN1_item_new(sig_it);
        
        if (asn1_val) {
            // Try to parse the input as X509_SIG
            const unsigned char *p = data;
            ASN1_VALUE *parsed = ASN1_item_d2i(&asn1_val, &p, size, sig_it);
            
            if (parsed) {
                // Create a dummy CMS structure for demonstration
                cms = CMS_ContentInfo_new();
                if (cms) {
                    // Write dummy CMS structure
                    i2d_CMS_bio(out_bio, cms);
                }
            }
            
            ASN1_item_free(asn1_val, sig_it);
        }
    }

    // Step 2: Create a new ASN1_VALUE using ASN1_item_new
    const ASN1_ITEM *test_item = X509_SIG_it();
    ASN1_VALUE *new_asn1 = ASN1_item_new(test_item);
    if (new_asn1) {
        // Clean up the created ASN1_VALUE
        ASN1_item_free(new_asn1, test_item);
    }

    // Step 3: Demonstrate OSSL_FUNC_core_thread_start_fn usage
    // Create a dummy OSSL_DISPATCH structure
    OSSL_DISPATCH dummy_dispatch[] = {
        {0, NULL}
    };
    
    // Get the thread start function pointer (will be NULL in our case)
    OSSL_FUNC_core_thread_start_fn *thread_start = OSSL_FUNC_core_thread_start(dummy_dispatch);
    // Note: In real usage, this would be used to start threads in providers

    // Step 4: Get table of context parameters for a message digest
    const EVP_MD *md = EVP_sha256();
    const OSSL_PARAM *gettable_params = EVP_MD_gettable_ctx_params(md);
    
    // Count the number of parameters available
    int param_count = 0;
    if (gettable_params) {
        while (gettable_params[param_count].key != NULL) {
            param_count++;
        }
    }

    // Step 5: Create a dummy OSSL_IETF_ATTR_SYNTAX and get value count
    OSSL_IETF_ATTR_SYNTAX *attr_syntax = OSSL_IETF_ATTR_SYNTAX_new();
    if (attr_syntax) {
        // Get number of values in attribute syntax
        int value_num = OSSL_IETF_ATTR_SYNTAX_get_value_num(attr_syntax);
        (void)value_num; // Use value to avoid unused variable warning
        
        OSSL_IETF_ATTR_SYNTAX_free(attr_syntax);
    }

    // Step 6: Get X509_SIG ASN1_ITEM (already done above, but demonstrate usage)
    const ASN1_ITEM *sig_item = X509_SIG_it();
    (void)sig_item; // Use item to avoid unused variable warning

    // Clean up
    if (cms) {
        CMS_ContentInfo_free(cms);
    }
    
    BIO_free(in_bio);
    BIO_free(out_bio);
    
    // Note: in_file and out_file are closed automatically by BIO_free with BIO_NOCLOSE
    // But we need to close the FILE pointers since we used BIO_NOCLOSE
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}