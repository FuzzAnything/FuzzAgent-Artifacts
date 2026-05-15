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
//<ID> 444
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"int_ctx_new":[[122,13,122,28,0,0,4,0]]},"library_calls":["OSSL_LIB_CTX_new","EVP_RAND_fetch","EVP_RAND_names_do_all","BIO_new_fp","OSSL_CMP_PKIHEADER_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","OSSL_CMP_HDR_get0_recipNonce","BN_CTX_new","EVP_PKEY_new","EVP_PKEY_new","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_new","EVP_PKEY_derive_set_peer_ex","EVP_PKEY_encrypt_init","EVP_PKEY_CTX_set_rsa_padding","EVP_PKEY_CTX_set0_rsa_oaep_label","EVP_PKEY_sign_init","EVP_aes_128_cbc","EVP_PKEY_new_CMAC_key","EVP_PKEY_free","EVP_PKEY_CTX_free","EVP_PKEY_CTX_free","EVP_PKEY_free","EVP_PKEY_free","BN_CTX_free","ASN1_OCTET_STRING_free","OSSL_CMP_PKIHEADER_free","BIO_free","EVP_RAND_free","OSSL_LIB_CTX_free"],"critical_calls":["OSSL_LIB_CTX_new","EVP_RAND_fetch","EVP_RAND_names_do_all","BIO_new_fp","OSSL_CMP_PKIHEADER_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","OSSL_CMP_HDR_get0_recipNonce","BN_CTX_new","EVP_PKEY_new","EVP_PKEY_new","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_new","EVP_PKEY_derive_set_peer_ex","EVP_PKEY_encrypt_init","EVP_PKEY_CTX_set_rsa_padding","EVP_PKEY_CTX_set0_rsa_oaep_label","EVP_PKEY_sign_init","EVP_aes_128_cbc","EVP_PKEY_new_CMAC_key","EVP_PKEY_free","EVP_PKEY_CTX_free","EVP_PKEY_CTX_free","EVP_PKEY_free","EVP_PKEY_free","BN_CTX_free","ASN1_OCTET_STRING_free","OSSL_CMP_PKIHEADER_free","BIO_free","EVP_RAND_free","OSSL_LIB_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Create file pointer from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 1. Create OSSL_LIB_CTX and fetch EVP_RAND
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    EVP_RAND *rand = EVP_RAND_fetch(libctx, "CTR-DRBG", NULL);
    
    // 2. List RAND algorithm names (callback function)
    EVP_RAND_names_do_all(rand, [](const char *name, void *data) {}, nullptr);
    
    // 3. Create BIO from file pointer
    BIO *bio = BIO_new_fp(in_file, BIO_CLOSE);
    
    // 4. Create CMP PKI header and ASN1 octet string
    OSSL_CMP_PKIHEADER *hdr = OSSL_CMP_PKIHEADER_new();
    ASN1_OCTET_STRING *octet = ASN1_OCTET_STRING_new();
    
    // Use input data for octet string (first min(100, size) bytes)
    int len = size > 100 ? 100 : size;
    ASN1_OCTET_STRING_set(octet, data, len);
    
    // 5. Get recipient nonce from header (will be NULL since not set)
    OSSL_CMP_HDR_get0_recipNonce(hdr);
    
    // 6. Create BN_CTX for BIGNUM operations
    BN_CTX *bn_ctx = BN_CTX_new();
    
    // 7. Create EVP_PKEY contexts for various operations
    EVP_PKEY *pkey1 = EVP_PKEY_new();
    EVP_PKEY *pkey2 = EVP_PKEY_new();
    
    // Create PKEY context by ID (RSA)
    EVP_PKEY_CTX *ctx_id = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    
    // Create PKEY context from pkey
    EVP_PKEY_CTX *ctx_pkey = EVP_PKEY_CTX_new(pkey1, NULL);
    
    // 8. Set up key derivation with peer
    if (ctx_pkey) {
        EVP_PKEY_derive_set_peer_ex(ctx_pkey, pkey2, 0);
    }
    
    // 9. Initialize encryption context
    if (ctx_id) {
        EVP_PKEY_encrypt_init(ctx_id);
        EVP_PKEY_CTX_set_rsa_padding(ctx_id, RSA_PKCS1_OAEP_PADDING);
        
        // Set OAEP label using a small buffer
        unsigned char label[] = {0x6c, 0x61, 0x62, 0x65, 0x6c}; // "label"
        EVP_PKEY_CTX_set0_rsa_oaep_label(ctx_id, label, sizeof(label));
    }
    
    // 10. Initialize signing context
    if (ctx_pkey) {
        EVP_PKEY_sign_init(ctx_pkey);
    }
    
    // 11. Create CMAC key using AES-128-CBC
    const EVP_CIPHER *cipher = EVP_aes_128_cbc();
    // Use part of input data as key material
    const unsigned char *key_data = data;
    size_t key_len = size > 16 ? 16 : size;
    EVP_PKEY *cmac_key = EVP_PKEY_new_CMAC_key(NULL, key_data, key_len, cipher);
    
    // 12. Create output file (dummy operation)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // 13. Clean up all resources
    if (cmac_key) EVP_PKEY_free(cmac_key);
    if (ctx_pkey) EVP_PKEY_CTX_free(ctx_pkey);
    if (ctx_id) EVP_PKEY_CTX_free(ctx_id);
    if (pkey2) EVP_PKEY_free(pkey2);
    if (pkey1) EVP_PKEY_free(pkey1);
    if (bn_ctx) BN_CTX_free(bn_ctx);
    if (octet) ASN1_OCTET_STRING_free(octet);
    if (hdr) OSSL_CMP_PKIHEADER_free(hdr);
    if (bio) BIO_free(bio);
    // Note: in_file is closed by BIO_free with BIO_CLOSE flag
    if (rand) EVP_RAND_free(rand);
    if (libctx) OSSL_LIB_CTX_free(libctx);
    
    // Note: EC_GROUP_free and PKCS12_free cannot be called without creating
    // EC_GROUP or PKCS12 objects first, which we don't have APIs for in the list
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}