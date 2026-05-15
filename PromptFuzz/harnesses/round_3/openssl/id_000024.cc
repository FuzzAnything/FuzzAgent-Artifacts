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
//<ID> 412
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"ossl_asn1_primitive_free":[[199,5,199,21,0,0,4,0]]},"library_calls":["TLS_method","SSL_CTX_new","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","EVP_PKEY_new","BN_new","RSA_new","BN_bin2bn","RSA_generate_key_ex","EVP_PKEY_assign","EVP_PKEY_get1_RSA","DH_meth_new","d2i_PKCS7_ENC_CONTENT","RC2_set_key","RC2_cfb64_encrypt","RC2_cfb64_encrypt","BIO_dump_indent_fp","d2i_OCSP_CERTSTATUS","DIRECTORYSTRING_it","OSSL_ATTRIBUTE_TYPE_MAPPING_it","PKCS7_ENC_CONTENT_free","OCSP_CERTSTATUS_free","DH_meth_free","RSA_free","BN_free","EVP_PKEY_free","OSSL_CMP_CTX_free","SSL_CTX_free"],"critical_calls":["TLS_method","SSL_CTX_new","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","EVP_PKEY_new","BN_new","RSA_new","BN_bin2bn","RSA_generate_key_ex","EVP_PKEY_assign","EVP_PKEY_get1_RSA","DH_meth_new","d2i_PKCS7_ENC_CONTENT","RC2_set_key","RC2_cfb64_encrypt","RC2_cfb64_encrypt","BIO_dump_indent_fp","d2i_OCSP_CERTSTATUS","DIRECTORYSTRING_it","OSSL_ATTRIBUTE_TYPE_MAPPING_it","PKCS7_ENC_CONTENT_free","OCSP_CERTSTATUS_free","DH_meth_free","RSA_free","BN_free","EVP_PKEY_free","OSSL_CMP_CTX_free","SSL_CTX_free"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified OpenSSL APIs to achieve a comprehensive event of setting up SSL/TLS context with Certificate Transparency validation, generating RSA keys, performing RC2 encryption, and parsing various cryptographic structures:

*/


// Dummy CT validation callback function
static int  id_000024_dummy_ct_validation_cb(const CT_POLICY_EVAL_CTX *ctx, 
                                 const struct stack_st_SCT *scts, 
                                 void *arg) {
    return 1; // Always accept for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 100) {
	return 0;
	}
	
    
    // 1. Create SSL context with TLS method
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    
    // 2. Set CT validation callback
    SSL_CTX_set_ct_validation_callback(ctx,  id_000024_dummy_ct_validation_cb, NULL);
    
    // 3. Create CMP context
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(NULL, NULL);
    
    // 4. Get HTTP callback argument from CMP context
    void *http_cb_arg = OSSL_CMP_CTX_get_http_cb_arg(cmp_ctx);
    
    // 5. Create EVP_PKEY and RSA key generation
    EVP_PKEY *pkey = EVP_PKEY_new();
    BIGNUM *bn = BN_new();
    RSA *rsa = RSA_new();
    
    // Use input data for RSA exponent
    const unsigned char *exponent_data = data;
    int exponent_len = size > 4 ? 4 : size;
    BN_bin2bn(exponent_data, exponent_len, bn);
    
    // Generate RSA key (512 bits for speed)
    RSA_generate_key_ex(rsa, 512, bn, NULL);
    
    // Assign RSA key to EVP_PKEY
    EVP_PKEY_assign(pkey, EVP_PKEY_RSA, rsa);
    
    // 6. Get RSA from EVP_PKEY
    RSA *rsa2 = EVP_PKEY_get1_RSA(pkey);
    
    // 7. Create DH method
    DH_METHOD *dh_method = DH_meth_new("fuzz_dh", 0);
    
    // 8. Parse PKCS7_ENC_CONTENT from input data
    const unsigned char *pkcs7_data = data + exponent_len;
    long pkcs7_len = size - exponent_len;
    PKCS7_ENC_CONTENT *pkcs7_enc = d2i_PKCS7_ENC_CONTENT(NULL, &pkcs7_data, pkcs7_len);
    
    // 9. RC2 encryption/decryption using input data
    RC2_KEY rc2_key;
    unsigned char iv[8] = {0};
    int num = 0;
    
    // Use portion of input for RC2 key
    const unsigned char *rc2_key_data = data;
    int rc2_key_len = size > 16 ? 16 : size;
    RC2_set_key(&rc2_key, rc2_key_len, rc2_key_data, 64);
    
    // Allocate buffers for encryption
    size_t rc2_data_len = size > 64 ? 64 : size;
    unsigned char *in_buf = (unsigned char *)malloc(rc2_data_len);
    unsigned char *out_buf = (unsigned char *)malloc(rc2_data_len);
    
    if (in_buf && out_buf) {
        memcpy(in_buf, data, rc2_data_len);
        
        // Encrypt
        RC2_cfb64_encrypt(in_buf, out_buf, rc2_data_len, &rc2_key, iv, &num, 1);
        
        // Decrypt
        num = 0;
        memset(iv, 0, sizeof(iv));
        RC2_cfb64_encrypt(out_buf, in_buf, rc2_data_len, &rc2_key, iv, &num, 0);
    }
    
    // 10. Create FILE for dumping data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Dump some data
        BIO_dump_indent_fp(out_file, data, size > 256 ? 256 : size, 4);
        assert_file_closed(&out_file);;
    }
    
    // 11. Parse OCSP_CERTSTATUS from remaining data
    const unsigned char *ocsp_data = data + (size / 2);
    long ocsp_len = size / 2;
    OCSP_CERTSTATUS *ocsp_status = d2i_OCSP_CERTSTATUS(NULL, &ocsp_data, ocsp_len);
    
    // 12. Get ASN1 items for various structures
    const ASN1_ITEM *directory_string_it = DIRECTORYSTRING_it();
    const ASN1_ITEM *attr_mapping_it = OSSL_ATTRIBUTE_TYPE_MAPPING_it();
    
    // Cleanup - free all allocated resources
    if (pkcs7_enc) PKCS7_ENC_CONTENT_free(pkcs7_enc);
    if (ocsp_status) OCSP_CERTSTATUS_free(ocsp_status);
    
    if (dh_method) DH_meth_free(dh_method);
    
    if (rsa2) RSA_free(rsa2); // This is a separate reference from EVP_PKEY_get1_RSA
    
    // Note: rsa was assigned to pkey, so don't free separately
    if (bn) BN_free(bn);
    if (pkey) EVP_PKEY_free(pkey);
    
    if (cmp_ctx) OSSL_CMP_CTX_free(cmp_ctx);
    if (ctx) SSL_CTX_free(ctx);
    
    free(in_buf);
    free(out_buf);
    
    // Clean up output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	return 0;
}