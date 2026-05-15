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
//<ID> 352
//<Prompt> []
/*<Combination>: [
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["OPENSSL_init_crypto","d2i_PKCS12_fp","EC_GROUP_new_by_curve_name","BN_CTX_new","EVP_PKEY_CTX_new_id","EVP_PKEY_encrypt_init","EVP_PKEY_CTX_set_rsa_padding","EVP_PKEY_CTX_set0_rsa_oaep_label","EVP_aes_128_cbc","EVP_PKEY_new_CMAC_key","EVP_PKEY_CTX_new","EVP_PKEY_sign_init","OSSL_CMP_PKIHEADER_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","OSSL_CMP_HDR_get0_recipNonce","OSSL_LIB_CTX_new","EVP_RAND_fetch","EVP_RAND_names_do_all","BIO_new_fp","BIO_free","EVP_RAND_free","OSSL_LIB_CTX_free","ASN1_OCTET_STRING_free","OSSL_CMP_PKIHEADER_free","EVP_PKEY_CTX_free","EVP_PKEY_free","EVP_PKEY_CTX_free","EC_GROUP_free","BN_CTX_free","PKCS12_free"],"critical_calls":["OPENSSL_init_crypto","d2i_PKCS12_fp","EC_GROUP_new_by_curve_name","BN_CTX_new","EVP_PKEY_CTX_new_id","EVP_PKEY_encrypt_init","EVP_PKEY_CTX_set_rsa_padding","EVP_PKEY_CTX_set0_rsa_oaep_label","EVP_aes_128_cbc","EVP_PKEY_new_CMAC_key","EVP_PKEY_CTX_new","EVP_PKEY_sign_init","OSSL_CMP_PKIHEADER_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","OSSL_CMP_HDR_get0_recipNonce","OSSL_LIB_CTX_new","EVP_RAND_fetch","EVP_RAND_names_do_all","BIO_new_fp","BIO_free","EVP_RAND_free","OSSL_LIB_CTX_free","ASN1_OCTET_STRING_free","OSSL_CMP_PKIHEADER_free","EVP_PKEY_CTX_free","EVP_PKEY_free","EVP_PKEY_CTX_free","EC_GROUP_free","BN_CTX_free","PKCS12_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize OpenSSL
    if (!OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, nullptr)) {
        return 0;
    }

    // 2. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Read PKCS12 from file
    PKCS12 *p12 = d2i_PKCS12_fp(in_file, nullptr);
    assert_file_closed(&in_file);;

    // 4. Create EC_GROUP and BN_CTX (for demonstration)
    EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    BN_CTX *bn_ctx = BN_CTX_new();

    // 5. Create EVP_PKEY context for RSA encryption
    EVP_PKEY_CTX *encrypt_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    if (encrypt_ctx) {
        EVP_PKEY_encrypt_init(encrypt_ctx);
        EVP_PKEY_CTX_set_rsa_padding(encrypt_ctx, RSA_PKCS1_OAEP_PADDING);
        
        // Create dummy label for OAEP
        unsigned char label[] = "test";
        EVP_PKEY_CTX_set0_rsa_oaep_label(encrypt_ctx, label, sizeof(label)-1);
    }

    // 6. Create CMAC key using AES-128-CBC
    const EVP_CIPHER *cipher = EVP_aes_128_cbc();
    EVP_PKEY *cmac_key = nullptr;
    if (size > 0) {
        cmac_key = EVP_PKEY_new_CMAC_key(nullptr, data, 
                                        size > 16 ? 16 : size, // Use up to 16 bytes
                                        cipher);
    }

    // 7. Create EVP_PKEY context for signing with CMAC key
    EVP_PKEY_CTX *sign_ctx = nullptr;
    if (cmac_key) {
        sign_ctx = EVP_PKEY_CTX_new(cmac_key, nullptr);
        if (sign_ctx) {
            EVP_PKEY_sign_init(sign_ctx);
        }
    }

    // 8. Create CMP PKI header and octet string
    OSSL_CMP_PKIHEADER *header = OSSL_CMP_PKIHEADER_new();
    ASN1_OCTET_STRING *octet_str = ASN1_OCTET_STRING_new();
    
    // Set octet string with input data (limited to 100 bytes for safety)
    if (octet_str && size > 0) {
        int len = size > 100 ? 100 : (int)size;
        ASN1_OCTET_STRING_set(octet_str, data, len);
    }
    
    // Get recipient nonce (will be NULL as we didn't set it)
    const ASN1_OCTET_STRING *recip_nonce = OSSL_CMP_HDR_get0_recipNonce(header);

    // 9. Fetch EVP_RAND and list names
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    EVP_RAND *rand = EVP_RAND_fetch(libctx, "CTR-DRBG", nullptr);
    if (rand) {
        // Dummy callback for name enumeration
        auto name_callback = [](const char *name, void *data) {
            // Do nothing in fuzzer
        };
        EVP_RAND_names_do_all(rand, name_callback, nullptr);
    }

    // 10. Create BIO for output (demonstration)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        BIO *out_bio = BIO_new_fp(out_file, BIO_NOCLOSE);
        if (out_bio) {
            // Could write something here if needed
            BIO_free(out_bio);
        }
        assert_file_closed(&out_file);;
    }

    // 11. Cleanup all resources
    if (rand) EVP_RAND_free(rand);
    if (libctx) OSSL_LIB_CTX_free(libctx);
    if (octet_str) ASN1_OCTET_STRING_free(octet_str);
    if (header) OSSL_CMP_PKIHEADER_free(header);
    if (sign_ctx) EVP_PKEY_CTX_free(sign_ctx);
    if (cmac_key) EVP_PKEY_free(cmac_key);
    if (encrypt_ctx) EVP_PKEY_CTX_free(encrypt_ctx);
    if (ec_group) EC_GROUP_free(ec_group);
    if (bn_ctx) BN_CTX_free(bn_ctx);
    if (p12) PKCS12_free(p12);

    // Note: OSSL_FUNC_keymgmt_free is a function pointer type,
    // not a function we can call directly without a dispatch table
    // So we skip calling it as we don't have OSSL_DISPATCH

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}