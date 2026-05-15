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
//<ID> 471
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 2
//<Quality> {"density":5,"unique_branches":{"ASN1_GENERALIZEDTIME_set_string":[[54,9,54,18,0,0,4,1]],"UI_add_user_data":[[400,9,400,45,0,0,4,1]]},"library_calls":["OPENSSL_init_ssl","OSSL_CRMF_PKIPUBLICATIONINFO_it","OCSP_BASICRESP_it","UI_new","UI_add_user_data","UI_process","BIO_f_buffer","BIO_new_fp","BIO_new","BIO_push","d2i_OCSP_BASICRESP","d2i_OCSP_BASICRESP","ASN1_UTF8STRING_new","ASN1_STRING_set","i2d_ASN1_UTF8STRING","CRYPTO_free","PKEY_USAGE_PERIOD_new","ASN1_GENERALIZEDTIME_set_string","ASN1_GENERALIZEDTIME_set_string","PKEY_USAGE_PERIOD_free","BIO_ADDR_new","BIO_ADDR_new","BIO_ADDR_copy","d2i_OSSL_CRMF_PKIPUBLICATIONINFO","ASN1_UTF8STRING_free","PKEY_USAGE_PERIOD_free","OSSL_CRMF_PKIPUBLICATIONINFO_free","OCSP_BASICRESP_free","UI_free","BIO_ADDR_free","BIO_ADDR_free","BIO_free_all","BIO_free_all","ERR_clear_error"],"critical_calls":["OPENSSL_init_ssl","OSSL_CRMF_PKIPUBLICATIONINFO_it","OCSP_BASICRESP_it","UI_new","UI_add_user_data","UI_process","BIO_f_buffer","BIO_new_fp","BIO_new","BIO_push","d2i_OCSP_BASICRESP","d2i_OCSP_BASICRESP","ASN1_UTF8STRING_new","ASN1_STRING_set","i2d_ASN1_UTF8STRING","CRYPTO_free","PKEY_USAGE_PERIOD_new","ASN1_GENERALIZEDTIME_set_string","ASN1_GENERALIZEDTIME_set_string","PKEY_USAGE_PERIOD_free","BIO_ADDR_new","BIO_ADDR_new","BIO_ADDR_copy","d2i_OSSL_CRMF_PKIPUBLICATIONINFO","ASN1_UTF8STRING_free","PKEY_USAGE_PERIOD_free","OSSL_CRMF_PKIPUBLICATIONINFO_free","OCSP_BASICRESP_free","UI_free","BIO_ADDR_free","BIO_ADDR_free","BIO_free_all","ERR_clear_error"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // Initialize OpenSSL
    OPENSSL_init_ssl(0, NULL);
    
    FILE *in_file = NULL;
    FILE *out_file = NULL;
    BIO *bio = NULL;
    BIO *buffer_bio = NULL;
    BIO_ADDR *bio_addr1 = NULL;
    BIO_ADDR *bio_addr2 = NULL;
    UI *ui = NULL;
    ASN1_UTF8STRING *utf8_string = NULL;
    PKEY_USAGE_PERIOD *usage_period = NULL;
    OSSL_CRMF_PKIPUBLICATIONINFO *pub_info = NULL;
    OCSP_BASICRESP *basic_resp = NULL;
    const ASN1_ITEM *item = NULL;
    const BIO_METHOD *buffer_method = NULL;
    
    unsigned char *der_data = NULL;
    int der_len = 0;
    const unsigned char *p = data;
    
    // 1. Get ASN1_ITEM structures
    item = OSSL_CRMF_PKIPUBLICATIONINFO_it();
    if (!item) goto cleanup;
    
    item = OCSP_BASICRESP_it();
    if (!item) goto cleanup;
    
    // 2. Create and use UI
    ui = UI_new();
    if (ui) {
        // Add a dummy method for processing
        UI_add_user_data(ui, (void*)"test");
        UI_process(ui);
    }
    
    // 3. Create file for input data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) goto cleanup;
    
    // 4. Create file for output data
    out_file = fopen("output_file", "wb");
    if (!out_file) goto cleanup;
    
    // 5. Create BIOs and use BIO_f_buffer
    buffer_method = BIO_f_buffer();
    if (!buffer_method) goto cleanup;
    
    bio = BIO_new_fp(in_file, BIO_NOCLOSE);
    if (!bio) goto cleanup;
    
    buffer_bio = BIO_new(buffer_method);
    if (buffer_bio) {
        BIO_push(buffer_bio, bio);
        bio = buffer_bio;
    }
    
    // 6. Parse OCSP_BASICRESP from input data
    basic_resp = d2i_OCSP_BASICRESP(NULL, &p, size);
    if (!basic_resp && size > 50) {
        // Try with smaller amount if first attempt fails
        p = data;
        basic_resp = d2i_OCSP_BASICRESP(NULL, &p, size / 2);
    }
    
    // 7. Create and use ASN1_UTF8STRING
    utf8_string = ASN1_UTF8STRING_new();
    if (utf8_string && size > 20) {
        // Use part of input data as UTF8 string content
        size_t str_len = size > 100 ? 100 : size;
        ASN1_STRING_set(utf8_string, data, str_len);
        
        // Convert to DER format
        der_len = i2d_ASN1_UTF8STRING(utf8_string, &der_data);
        if (der_data && der_len > 0) {
            // Write DER data to output file
            fwrite(der_data, 1, der_len, out_file);
            OPENSSL_free(der_data);
            der_data = NULL;
        }
    }
    
    // 8. Create and use PKEY_USAGE_PERIOD
    usage_period = PKEY_USAGE_PERIOD_new();
    if (usage_period) {
        // Set some dummy values
        if (size > 30) {
            ASN1_GENERALIZEDTIME_set_string(usage_period->notBefore, "20240101000000Z");
            ASN1_GENERALIZEDTIME_set_string(usage_period->notAfter, "20251231000000Z");
        }
        // Free it (as required by API usage)
        PKEY_USAGE_PERIOD_free(usage_period);
        usage_period = NULL;
    }
    
    // 9. Create and use BIO_ADDR
    bio_addr1 = BIO_ADDR_new();
    bio_addr2 = BIO_ADDR_new();
    if (bio_addr1 && bio_addr2) {
        // Try to copy addresses (dummy operation since we don't have real addresses)
        BIO_ADDR_copy(bio_addr2, bio_addr1);
    }
    
    // 10. Try to parse OSSL_CRMF_PKIPUBLICATIONINFO if we have enough data
    if (size > 100) {
        const unsigned char *p2 = data + (size / 3);
        size_t remaining = size - (size / 3);
        pub_info = d2i_OSSL_CRMF_PKIPUBLICATIONINFO(NULL, &p2, remaining);
    }
    
    // 11. Write some data to output file using file descriptor
    if (out_file) {
        int fd = fuzz_fileno(out_file);
        if (fd >= 0 && size > 10) {
            // Write first 10 bytes to output
            fwrite(data, 1, size > 100 ? 100 : size, out_file);
        }
    }

cleanup:
    // Release all allocated resources
    if (utf8_string) ASN1_UTF8STRING_free(utf8_string);
    if (usage_period) PKEY_USAGE_PERIOD_free(usage_period);
    if (pub_info) OSSL_CRMF_PKIPUBLICATIONINFO_free(pub_info);
    if (basic_resp) OCSP_BASICRESP_free(basic_resp);
    if (ui) UI_free(ui);
    if (bio_addr1) BIO_ADDR_free(bio_addr1);
    if (bio_addr2) BIO_ADDR_free(bio_addr2);
    
    // Clean up BIO chain properly
    if (buffer_bio && bio == buffer_bio) {
        // bio points to buffer_bio in the chain
        BIO_free_all(bio);
    } else if (bio) {
        BIO_free_all(bio);
    }
    
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    // Clean up OpenSSL
    ERR_clear_error();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}