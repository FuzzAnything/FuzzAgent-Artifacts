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
//<ID> 453
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"OSSL_STORE_delete":[[511,9,511,26,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","NOTICEREF_it","SSL_CTX_new","TLS_method","SSL_new","SSL_use_PrivateKey_file","SSL_free","SSL_CTX_free","OSSL_CRMF_MSG_new","OSSL_CRMF_MSG_get0_regCtrl_oldCertID","OSSL_CRMF_MSG_free","EVP_des_ede","X509_TRUST_get_count","ACCESS_DESCRIPTION_new","d2i_ACCESS_DESCRIPTION","i2d_ACCESS_DESCRIPTION","CRYPTO_free","ACCESS_DESCRIPTION_free","i2d_ACCESS_DESCRIPTION","CRYPTO_free","ACCESS_DESCRIPTION_free","OSSL_STORE_delete","EVP_aes_128_cbc_hmac_sha256"],"critical_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","NOTICEREF_it","SSL_CTX_new","TLS_method","SSL_new","SSL_use_PrivateKey_file","SSL_free","SSL_CTX_free","OSSL_CRMF_MSG_new","OSSL_CRMF_MSG_get0_regCtrl_oldCertID","OSSL_CRMF_MSG_free","EVP_des_ede","X509_TRUST_get_count","ACCESS_DESCRIPTION_new","d2i_ACCESS_DESCRIPTION","i2d_ACCESS_DESCRIPTION","CRYPTO_free","ACCESS_DESCRIPTION_free","i2d_ACCESS_DESCRIPTION","CRYPTO_free","ACCESS_DESCRIPTION_free","OSSL_STORE_delete","EVP_aes_128_cbc_hmac_sha256"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS, NULL);
    
    // Create memory FILE for input data
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 1. Call NOTICEREF_it() to get ASN1_ITEM for NOTICEREF
    const ASN1_ITEM *noticeref_item = NOTICEREF_it();
    (void)noticeref_item; // Use to avoid unused variable warning
    
    // 2. Create SSL context and use private key file
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    if (ctx) {
        // Write data to a temporary file for SSL_use_PrivateKey_file
        FILE *tmp_key = fopen("temp_key.pem", "wb");
        if (tmp_key) {
            fwrite(data, 1, size > 1024 ? 1024 : size, tmp_key);
            assert_file_closed(&tmp_key);;
            
            SSL *ssl = SSL_new(ctx);
            if (ssl) {
                // Try to use the private key file (will likely fail with random data)
                SSL_use_PrivateKey_file(ssl, "temp_key.pem", SSL_FILETYPE_PEM);
                SSL_free(ssl);
            }
            remove("temp_key.pem");
        }
        SSL_CTX_free(ctx);
    }
    
    // 3. Create a dummy CRMF message and get oldCertID
    OSSL_CRMF_MSG *crmf_msg = OSSL_CRMF_MSG_new();
    if (crmf_msg) {
        OSSL_CRMF_CERTID *certid = OSSL_CRMF_MSG_get0_regCtrl_oldCertID(crmf_msg);
        (void)certid; // Use to avoid unused variable warning
        OSSL_CRMF_MSG_free(crmf_msg);
    }
    
    // 4. Get DES-EDE cipher
    const EVP_CIPHER *des_ede = EVP_des_ede();
    (void)des_ede; // Use to avoid unused variable warning
    
    // 5. Get X509 trust count
    int trust_count = X509_TRUST_get_count();
    (void)trust_count; // Use to avoid unused variable warning
    
    // 6. Create ACCESS_DESCRIPTION and serialize it
    ACCESS_DESCRIPTION *ad = ACCESS_DESCRIPTION_new();
    if (ad) {
        // Try to parse some data as ACCESS_DESCRIPTION
        const unsigned char *p = data;
        long len = size;
        ACCESS_DESCRIPTION *ad2 = d2i_ACCESS_DESCRIPTION(NULL, &p, len);
        if (ad2) {
            // Serialize it back
            unsigned char *out = NULL;
            int out_len = i2d_ACCESS_DESCRIPTION(ad2, &out);
            if (out) {
                OPENSSL_free(out);
            }
            ACCESS_DESCRIPTION_free(ad2);
        }
        
        // Also serialize our empty ACCESS_DESCRIPTION
        unsigned char *out2 = NULL;
        i2d_ACCESS_DESCRIPTION(ad, &out2);
        if (out2) {
            OPENSSL_free(out2);
        }
        ACCESS_DESCRIPTION_free(ad);
    }
    
    // 7. Try OSSL_STORE_delete with dummy URI (will fail but we call it)
    OSSL_STORE_delete("file://dummy", NULL, NULL, NULL, NULL, NULL);
    
    // 8. Get AES-128-CBC-HMAC-SHA256 cipher
    const EVP_CIPHER *aes_cipher = EVP_aes_128_cbc_hmac_sha256();
    (void)aes_cipher; // Use to avoid unused variable warning
    
    // 9. Additional: Use the input FILE* for reading
    // Try to read some data through the FILE*
    unsigned char buf[1024];
    fread(buf, 1, size > 1024 ? 1024 : size, in_file);
    
    // 10. Additional: Write something to output file
    fwrite(data, 1, size > 1024 ? 1024 : size, out_file);
    
    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}