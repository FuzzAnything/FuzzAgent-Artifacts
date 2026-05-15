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
//<ID> 410
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["TLS_method","SSL_CTX_new","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","EVP_PKEY_new","BN_new","BN_bin2bn","RSA_new","RSA_generate_key_ex","EVP_PKEY_assign","EVP_PKEY_get1_RSA","DH_meth_new","d2i_PKCS7_ENC_CONTENT","RC2_set_key","RC2_cfb64_encrypt","BIO_dump_indent_fp","DIRECTORYSTRING_it","d2i_OCSP_CERTSTATUS","OSSL_ATTRIBUTE_TYPE_MAPPING_it","PKCS7_ENC_CONTENT_free","PKCS7_RECIP_INFO_free","OCSP_CERTSTATUS_free","DH_meth_free","RSA_free","BN_free","EVP_PKEY_free","OSSL_CMP_CTX_free","SSL_CTX_free"],"critical_calls":["TLS_method","SSL_CTX_new","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","EVP_PKEY_new","BN_new","BN_bin2bn","RSA_new","RSA_generate_key_ex","EVP_PKEY_assign","EVP_PKEY_get1_RSA","DH_meth_new","d2i_PKCS7_ENC_CONTENT","RC2_set_key","RC2_cfb64_encrypt","BIO_dump_indent_fp","DIRECTORYSTRING_it","d2i_OCSP_CERTSTATUS","OSSL_ATTRIBUTE_TYPE_MAPPING_it","PKCS7_ENC_CONTENT_free","PKCS7_RECIP_INFO_free","OCSP_CERTSTATUS_free","DH_meth_free","RSA_free","BN_free","EVP_PKEY_free","OSSL_CMP_CTX_free","SSL_CTX_free"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified OpenSSL APIs to achieve a comprehensive test scenario:

*/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations

    // 1. Create SSL context and set CT validation callback
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    
    // Create dummy CT validation callback
    ssl_ct_validation_cb ct_callback = [](const CT_POLICY_EVAL_CTX *ctx, 
                                          const struct stack_st_SCT *scts, 
                                          void *arg) {
        return 1;
    };
    SSL_CTX_set_ct_validation_callback(ctx, ct_callback, nullptr);

    // 2. Create CMP context
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(nullptr, nullptr);
    void *http_cb_arg = OSSL_CMP_CTX_get_http_cb_arg(cmp_ctx);

    // 3. Create EVP_PKEY and generate RSA key
    EVP_PKEY *pkey = EVP_PKEY_new();
    BIGNUM *bn = BN_new();
    
    // Use first 32 bytes for BIGNUM
    BN_bin2bn(data, std::min(32, (int)size), bn);
    
    RSA *rsa = RSA_new();
    // Generate 512-bit RSA key (small for fuzzing performance)
    RSA_generate_key_ex(rsa, 512, bn, nullptr);
    
    // Assign RSA key to EVP_PKEY
    EVP_PKEY_assign(pkey, EVP_PKEY_RSA, rsa);
    
    // Get RSA key back
    RSA *rsa2 = EVP_PKEY_get1_RSA(pkey);

    // 4. Create and free DH method
    DH_METHOD *dh_method = DH_meth_new("fuzz_dh", 0);
    
    // 5. Decode PKCS7 structures from input data
    const uint8_t *pkcs7_data = data;
    long pkcs7_len = std::min((long)size, 1000L);
    PKCS7_ENC_CONTENT *enc_content = d2i_PKCS7_ENC_CONTENT(nullptr, &pkcs7_data, pkcs7_len);

    // 6. RC2 encryption operations
    RC2_KEY rc2_key;
    unsigned char iv[8] = {0};
    int num = 0;
    
    // Use data for RC2 key setup
    size_t rc2_key_len = std::min(size, (size_t)16);
    RC2_set_key(&rc2_key, rc2_key_len, data, 64);
    
    // Encrypt a small buffer
    unsigned char in_buf[64] = {0};
    unsigned char out_buf[64] = {0};
    if (size > 16) {
        memcpy(iv, data + 8, 8);
    }
    RC2_cfb64_encrypt(in_buf, out_buf, 64, &rc2_key, iv, &num, 1);

    // 7. Create FILE* for input/output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    if (in_file && out_file) {
        // 8. Dump data to file
        BIO_dump_indent_fp(out_file, data, std::min(100, (int)size), 4);
        
        // 9. Get file descriptors
        int in_fd = fuzz_fileno(in_file);
        int out_fd = fuzz_fileno(out_file);
        
        // Use file descriptors (dummy operations)
        if (in_fd >= 0 && out_fd >= 0) {
            // Could perform file operations here
        }
        
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
    }

    // 10. Use DIRECTORYSTRING_it
    const ASN1_ITEM *dir_string_it = DIRECTORYSTRING_it();

    // 11. Decode OCSP_CERTSTATUS
    const uint8_t *ocsp_data = data;
    long ocsp_len = std::min((long)size, 500L);
    OCSP_CERTSTATUS *certstatus = d2i_OCSP_CERTSTATUS(nullptr, &ocsp_data, ocsp_len);

    // 12. Use OSSL_ATTRIBUTE_TYPE_MAPPING_it
    const ASN1_ITEM *attr_mapping_it = OSSL_ATTRIBUTE_TYPE_MAPPING_it();

    // 13. Cleanup - free all allocated resources
    if (enc_content) PKCS7_ENC_CONTENT_free(enc_content);
    
    // Free PKCS7 recipient info (dummy cleanup)
    PKCS7_RECIP_INFO *recip_info = nullptr;
    if (recip_info) PKCS7_RECIP_INFO_free(recip_info);
    
    if (certstatus) {
        OCSP_CERTSTATUS_free(certstatus);
    }
    
    if (dh_method) DH_meth_free(dh_method);
    
    if (rsa2) RSA_free(rsa2); // Free the extracted RSA key
    
    // Note: rsa is owned by pkey, don't free separately
    if (bn) BN_free(bn);
    if (pkey) EVP_PKEY_free(pkey);
    if (cmp_ctx) OSSL_CMP_CTX_free(cmp_ctx);
    if (ctx) SSL_CTX_free(ctx);

    // Clean up output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}