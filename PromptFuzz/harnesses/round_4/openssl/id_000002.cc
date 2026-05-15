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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 40
//<Quality> {"density":12,"unique_branches":{"SHA512_Update_thunk":[[240,9,240,17,0,0,4,1],[244,9,244,18,0,0,4,1],[250,9,250,20,0,0,4,1],[263,9,263,28,0,0,4,0],[263,9,263,28,0,0,4,1],[276,9,276,17,0,0,4,0],[276,9,276,17,0,0,4,1]],"sha512_224_newctx":[[63,9,63,32,1,0,4,1]],"evp_md_init_internal":[[185,13,185,36,0,0,4,0],[189,13,189,27,0,0,4,1],[23,26,23,52,12,0,4,0],[23,26,23,52,15,0,4,1],[23,46,23,47,15,0,4,1]],"X509_NAME_ENTRY_create_by_OBJ":[[291,9,291,21,0,0,4,1],[291,25,291,38,0,0,4,1],[302,9,302,21,0,0,4,0],[302,25,302,38,0,0,4,1]],"SHA512_Final":[[169,9,169,32,0,0,4,0],[169,9,169,32,0,0,4,1],[188,9,188,20,0,0,4,1],[193,5,193,34,0,0,4,1],[196,5,196,30,0,0,4,1],[199,5,199,30,0,0,4,1],[202,5,202,30,0,0,4,1],[205,5,205,30,0,0,4,0],[222,5,222,12,0,0,4,1],[159,17,159,30,10,0,4,0],[159,17,159,30,10,0,4,1]],"sha512_224_internal_init":[[150,16,150,38,0,0,4,0],[150,42,150,51,0,0,4,0]],"sha512_224_internal_final":[[45,13,45,35,0,0,4,0],[45,39,45,56,0,0,4,0],[45,60,45,73,0,0,4,0]],"X509_NAME_ENTRY_set_data":[[329,9,329,19,0,0,4,1],[329,23,329,45,0,0,4,1],[335,9,335,16,0,0,4,1],[338,9,338,11,0,0,4,1],[340,9,340,29,0,0,4,0],[341,13,341,38,0,0,4,1]],"X509_NAME_ENTRY_create_by_NID":[[275,9,275,20,0,0,4,1]]},"library_calls":["BIO_new_fp","EVP_sha512_224","EVP_MD_CTX_new","EVP_DigestInit_ex","EVP_DigestUpdate","EVP_DigestFinal_ex","EVP_MD_CTX_free","PEM_bytes_read_bio","CRYPTO_free","CRYPTO_free","UI_create_method","UI_method_set_prompt_constructor","UI_destroy_method","d2i_OCSP_REVOKEDINFO","OCSP_REVOKEDINFO_free","SCT_new","i2o_SCT","CRYPTO_free","SCT_free","X509_NAME_ENTRY_new","OBJ_txt2nid","X509_NAME_ENTRY_create_by_NID","X509_NAME_ENTRY_free","DES_set_key_unchecked","DES_set_key_unchecked","DES_set_key_unchecked","DES_decrypt3","BIO_free"],"critical_calls":["BIO_new_fp","EVP_sha512_224","EVP_MD_CTX_new","EVP_DigestInit_ex","EVP_DigestUpdate","EVP_DigestFinal_ex","EVP_MD_CTX_free","PEM_bytes_read_bio","CRYPTO_free","CRYPTO_free","UI_create_method","UI_method_set_prompt_constructor","UI_destroy_method","d2i_OCSP_REVOKEDINFO","OCSP_REVOKEDINFO_free","SCT_new","i2o_SCT","CRYPTO_free","SCT_free","X509_NAME_ENTRY_new","OBJ_txt2nid","X509_NAME_ENTRY_create_by_NID","X509_NAME_ENTRY_free","DES_set_key_unchecked","DES_set_key_unchecked","DES_set_key_unchecked","DES_decrypt3","BIO_free"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified OpenSSL APIs to process input data:

*/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create input FILE* from the data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output FILE* (even if not used by all APIs)
    FILE *out_file = fopen("output_file", "wb");
    
    // 3. Create BIO from input file
    BIO *bio_in = BIO_new_fp(in_file, BIO_NOCLOSE);
    if (!bio_in) {
        assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get EVP_MD for SHA-512/224
    const EVP_MD *md = EVP_sha512_224();
    if (md) {
        // Create digest context and compute hash of input data
        EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
        if (md_ctx) {
            unsigned char hash[EVP_MAX_MD_SIZE];
            unsigned int hash_len = 0;
            EVP_DigestInit_ex(md_ctx, md, NULL);
            EVP_DigestUpdate(md_ctx, data, size);
            EVP_DigestFinal_ex(md_ctx, hash, &hash_len);
            EVP_MD_CTX_free(md_ctx);
        }
    }
    
    // 5. Read PEM data from BIO
    unsigned char *pem_data = nullptr;
    long pem_len = 0;
    char *pem_name = nullptr;
    PEM_bytes_read_bio(&pem_data, &pem_len, &pem_name, "ANY", bio_in, nullptr, nullptr);
    
    // Clean up PEM data
    if (pem_data) OPENSSL_free(pem_data);
    if (pem_name) OPENSSL_free(pem_name);
    
    // 6. Set up UI method with prompt constructor
    UI_METHOD *ui_method = UI_create_method("FuzzerUI");
    if (ui_method) {
        // Create a dummy prompt constructor
        auto dummy_constructor = [](UI *ui, const char *prompt, const char *verify) -> char* {
            return nullptr;
        };
        UI_method_set_prompt_constructor(ui_method, dummy_constructor);
        UI_destroy_method(ui_method);
    }
    
    // 7. Parse OCSP_REVOKEDINFO from input data
    const unsigned char *ptr = data;
    long remaining = size;
    OCSP_REVOKEDINFO *revoked_info = d2i_OCSP_REVOKEDINFO(nullptr, &ptr, remaining);
    if (revoked_info) {
        // Process revoked info if available
        OCSP_REVOKEDINFO_free(revoked_info);
    }
    
    // 8. Create and convert SCT
    SCT *sct = SCT_new();
    if (sct) {
        // Try to convert SCT to octet string
        unsigned char *sct_out = nullptr;
        int sct_len = i2o_SCT(sct, &sct_out);
        if (sct_out) OPENSSL_free(sct_out);
        SCT_free(sct);
    }
    
    // 9. Create X509_NAME_ENTRY
    X509_NAME_ENTRY *name_entry = X509_NAME_ENTRY_new();
    if (name_entry) {
        // Use input data to set the entry (truncate to fit)
        int nid = OBJ_txt2nid("commonName");
        if (size > 0) {
            // Use first byte as type, rest as data (truncated to reasonable size)
            int type = data[0] & 0xFF;
            size_t data_len = (size - 1) > 100 ? 100 : (size - 1);
            X509_NAME_ENTRY_create_by_NID(&name_entry, nid, type, 
                                         data + 1, data_len);
        }
        X509_NAME_ENTRY_free(name_entry);
    }
    
    // 10. Perform triple DES decryption on input data
    if (size >= 8) {  // Need at least 8 bytes for DES block
        // Create dummy key schedules (not secure for real use!)
        DES_key_schedule ks1, ks2, ks3;
        DES_cblock key1 = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
        DES_cblock key2 = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
        DES_cblock key3 = {0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67};
        
        // Set up key schedules
        DES_set_key_unchecked(&key1, &ks1);
        DES_set_key_unchecked(&key2, &ks2);
        DES_set_key_unchecked(&key3, &ks3);
        
        // Use first 8 bytes of input as data
        DES_LONG des_data[2];
        // Copy first 8 bytes to DES_LONG array
        memcpy(des_data, data, (size > 8) ? 8 : size);
        
        // Perform triple DES decryption
        DES_decrypt3(des_data, &ks1, &ks2, &ks3);
    }
    
    // 11. Clean up resources
    BIO_free(bio_in);
    assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}