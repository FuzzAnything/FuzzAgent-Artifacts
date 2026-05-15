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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 4.5, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["SSL_CTX_new","TLS_method","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","EVP_PKEY_new","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_assign","EVP_PKEY_get1_RSA","RSA_free","BN_free","DH_meth_new","d2i_PKCS7_ENC_CONTENT","PKCS7_RECIP_INFO_new","PKCS7_RECIP_INFO_free","RC2_set_key","RC2_cfb64_encrypt","BIO_dump_indent_fp","BIO_dump_indent_fp","PKCS7_ENC_CONTENT_free","DH_meth_free","EVP_PKEY_free","RSA_free","OSSL_CMP_CTX_free","SSL_CTX_free"],"critical_calls":["SSL_CTX_new","TLS_method","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","EVP_PKEY_new","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_assign","EVP_PKEY_get1_RSA","RSA_free","BN_free","DH_meth_new","d2i_PKCS7_ENC_CONTENT","PKCS7_RECIP_INFO_new","PKCS7_RECIP_INFO_free","RC2_set_key","RC2_cfb64_encrypt","BIO_dump_indent_fp","PKCS7_ENC_CONTENT_free","DH_meth_free","EVP_PKEY_free","OSSL_CMP_CTX_free","SSL_CTX_free"],"visited":1}
/*To create a comprehensive fuzzer that uses all the specified OpenSSL APIs together, I'll design an event that processes certificate-related data with encryption and validation. Here's the implementation:

*/


// Custom CT validation callback
static int  id_000000_ct_validation_callback(const CT_POLICY_EVAL_CTX *ctx, 
                                 const struct stack_st_SCT *scts, 
                                 void *arg) {
    // Simple validation that always accepts
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    // 1. Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create FILE* for writing output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create SSL_CTX and set CT validation callback
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        SSL_CTX_set_ct_validation_callback(ssl_ctx,  id_000000_ct_validation_callback, NULL);
    }
    
    // 4. Create OSSL_CMP_CTX and get HTTP callback argument
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(NULL, NULL);
    void *http_cb_arg = NULL;
    if (cmp_ctx) {
        http_cb_arg = OSSL_CMP_CTX_get_http_cb_arg(cmp_ctx);
        // Use the argument (though it's typically NULL in default context)
        if (http_cb_arg) {
            // If there's an argument, we could use it
        }
    }
    
    // 5. Create EVP_PKEY with RSA and extract it
    EVP_PKEY *pkey = EVP_PKEY_new();
    RSA *rsa = RSA_new();
    if (pkey && rsa) {
        // Create a minimal RSA key for testing
        BIGNUM *e = BN_new();
        BN_set_word(e, 65537);
        RSA_generate_key_ex(rsa, 512, e, NULL);
        EVP_PKEY_assign_RSA(pkey, rsa);
        
        // Extract RSA from EVP_PKEY
        struct rsa_st *extracted_rsa = EVP_PKEY_get1_RSA(pkey);
        if (extracted_rsa) {
            RSA_free(extracted_rsa);
        }
        BN_free(e);
    }
    
    // 6. Create new DH method
    DH_METHOD *dh_method = DH_meth_new("TestDHMethod", 0);
    
    // 7. Parse PKCS7_ENC_CONTENT from input data
    // Use first portion of data for PKCS7 parsing
    const unsigned char *p7_data = data;
    long p7_len = (size > 500) ? 500 : size; // Use first 500 bytes for PKCS7
    PKCS7_ENC_CONTENT *p7_enc = d2i_PKCS7_ENC_CONTENT(NULL, &p7_data, p7_len);
    
    // 8. Create and free PKCS7_RECIP_INFO
    PKCS7_RECIP_INFO *ri = PKCS7_RECIP_INFO_new();
    if (ri) {
        PKCS7_RECIP_INFO_free(ri);
    }
    
    // 9. Use RC2 encryption on remaining data
    if (size > p7_len) {
        size_t rc2_data_len = size - p7_len;
        const unsigned char *rc2_input = data + p7_len;
        
        // Prepare RC2 key and IV
        RC2_KEY rc2_key;
        unsigned char rc2_iv[8] = {0};
        unsigned char *rc2_out = new unsigned char[rc2_data_len];
        int num = 0;
        
        // Set a simple key for testing
        unsigned char rc2_key_data[16] = {0};
        if (rc2_data_len > 16) {
            memcpy(rc2_key_data, rc2_input, 16);
        }
        RC2_set_key(&rc2_key, 16, rc2_key_data, 128);
        
        // Encrypt the data
        RC2_cfb64_encrypt(rc2_input, rc2_out, rc2_data_len, 
                         &rc2_key, rc2_iv, &num, RC2_ENCRYPT);
        
        // 10. Dump encrypted data to file
        BIO_dump_indent_fp(out_file, rc2_out, 
                          (rc2_data_len > 100) ? 100 : rc2_data_len, 4);
        
        delete[] rc2_out;
    } else {
        // If no data left for RC2, dump the PKCS7 parsing result
        if (p7_enc) {
            BIO_dump_indent_fp(out_file, data, 
                              (size > 100) ? 100 : size, 4);
        }
    }
    
    // 11. Clean up all allocated resources
    if (p7_enc) {
        PKCS7_ENC_CONTENT_free(p7_enc);
    }
    
    if (dh_method) {
        DH_meth_free(dh_method);
    }
    
    if (pkey) {
        EVP_PKEY_free(pkey);
    } else if (rsa) {
        RSA_free(rsa);
    }
    
    if (cmp_ctx) {
        OSSL_CMP_CTX_free(cmp_ctx);
    }
    
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
    }
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove the output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}