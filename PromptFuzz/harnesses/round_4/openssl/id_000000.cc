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
//<ID> 14
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 10
//<Quality> {"density":6,"unique_branches":{"ossl_ffc_params_FIPS186_2_gen_verify":[[883,9,883,24,0,0,4,1]],"DSA_generate_parameters_ex":[[55,9,55,24,0,0,4,1]],"cipher_hw_aesni_initkey":[[27,44,27,74,0,0,4,0],[36,13,36,43,0,0,4,0]],"ASN1_dup":[[23,9,23,18,0,0,4,1],[27,9,27,15,0,0,4,1],[31,9,31,18,0,0,4,1]],"ossl_prov_cipher_hw_aes_cbc":[[185,23,185,64,2,0,4,0]],"aes_128_cbc_newctx":[[223,13,223,24,0,0,4,0],[205,9,205,32,1,0,4,1]]},"library_calls":["CMS_ContentInfo_it","ASN1_item_d2i_fp","GENERAL_NAME_new","X509_NAME_new","X509_NAME_add_entry_by_txt","GENERAL_NAME_dup","GENERAL_NAME_free","GENERAL_NAME_free","DSA_new","DSA_generate_parameters_ex","DSA_get_method","DSA_free","OPENSSL_sk_new_null","CRYPTO_free","CRYPTO_free","sk_IPAddressOrRange_freefunc_thunk","OSSL_QUIC_method","OSSL_CRMF_ENCRYPTEDVALUE_new","i2d_OSSL_CRMF_ENCRYPTEDVALUE","CRYPTO_free","OSSL_CRMF_ENCRYPTEDVALUE_free","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_aes_128_cbc","EVP_CIPHER_CTX_free","BIO_new_file","BIO_new_mem_buf","PEM_write_bio_CMS_stream","BIO_free","BIO_free","CMS_ContentInfo_free","ERR_clear_error"],"critical_calls":["CMS_ContentInfo_it","ASN1_item_d2i_fp","GENERAL_NAME_new","X509_NAME_new","X509_NAME_add_entry_by_txt","GENERAL_NAME_dup","GENERAL_NAME_free","GENERAL_NAME_free","DSA_new","DSA_generate_parameters_ex","DSA_get_method","DSA_free","OPENSSL_sk_new_null","CRYPTO_free","CRYPTO_free","sk_IPAddressOrRange_freefunc_thunk","OSSL_QUIC_method","OSSL_CRMF_ENCRYPTEDVALUE_new","i2d_OSSL_CRMF_ENCRYPTEDVALUE","CRYPTO_free","OSSL_CRMF_ENCRYPTEDVALUE_free","EVP_CIPHER_CTX_new","EVP_EncryptInit_ex","EVP_aes_128_cbc","EVP_CIPHER_CTX_free","BIO_new_file","BIO_new_mem_buf","PEM_write_bio_CMS_stream","BIO_free","BIO_free","CMS_ContentInfo_free","ERR_clear_error"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 64) {
	return 0;
	}
	 // Need minimum data for keys/IVs

    // 1. Read ASN.1 structure from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Try to read a CMS_ContentInfo structure
    const ASN1_ITEM *cms_it = CMS_ContentInfo_it();
    CMS_ContentInfo *cms = (CMS_ContentInfo *)ASN1_item_d2i_fp(cms_it, in_file, NULL);
    assert_file_closed(&in_file);;
    
    // 2. Create and duplicate a GENERAL_NAME
    GENERAL_NAME *gen_name = GENERAL_NAME_new();
    if (gen_name) {
        gen_name->type = GEN_DIRNAME;
        X509_NAME *name = X509_NAME_new();
        if (name) {
            // Add a dummy entry
            X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, 
                                      (const unsigned char*)"fuzz", 4, -1, 0);
            gen_name->d.dirn = name;
        }
        GENERAL_NAME *gen_name_dup = GENERAL_NAME_dup(gen_name);
        if (gen_name_dup) GENERAL_NAME_free(gen_name_dup);
        GENERAL_NAME_free(gen_name);
    }
    
    // 3. Create DSA key and get its method
    DSA *dsa = DSA_new();
    if (dsa) {
        // Generate DSA parameters (fuzzing-friendly size)
        DSA_generate_parameters_ex(dsa, 512, NULL, 0, NULL, NULL, NULL);
        const DSA_METHOD *dsa_method = DSA_get_method(dsa);
        (void)dsa_method; // Use the method pointer
        DSA_free(dsa);
    }
    
    // 4. Create and free IPAddressOrRange stack (simulated)
    // Note: We don't have actual IPAddressOrRange type, so we simulate the thunk call
    OPENSSL_STACK *dummy_stack = OPENSSL_sk_new_null();
    if (dummy_stack) {
        // Cast the free function to match thunk signature
        void (*freefunc)(void *) = [](void *ptr) { OPENSSL_free(ptr); };
        sk_IPAddressOrRange_freefunc_thunk(freefunc, dummy_stack);
    }
    
    // 5. Get QUIC method
    const SSL_METHOD *quic_method = OSSL_QUIC_method();
    (void)quic_method;
    
    // 6. Create and serialize OSSL_CRMF_ENCRYPTEDVALUE
    OSSL_CRMF_ENCRYPTEDVALUE *enc_val = OSSL_CRMF_ENCRYPTEDVALUE_new();
    if (enc_val) {
        unsigned char *der = NULL;
        int der_len = i2d_OSSL_CRMF_ENCRYPTEDVALUE(enc_val, &der);
        if (der) OPENSSL_free(der);
        OSSL_CRMF_ENCRYPTEDVALUE_free(enc_val);
    }
    
    // 7. Initialize encryption context using input data for key/IV
    EVP_CIPHER_CTX *enc_ctx = EVP_CIPHER_CTX_new();
    if (enc_ctx) {
        // Use first 16 bytes as key, next 16 as IV
        const unsigned char *key = data;
        const unsigned char *iv = data + 16;
        size_t key_iv_len = 16;
        
        if (size >= 32) {
            EVP_EncryptInit_ex(enc_ctx, EVP_aes_128_cbc(), NULL, key, iv);
        }
        EVP_CIPHER_CTX_free(enc_ctx);
    }
    
    // 8. Write CMS to BIO stream if we successfully read one
    if (cms) {
        BIO *out_bio = BIO_new_file("output_file", "wb");
        BIO *in_bio = BIO_new_mem_buf(data, size);
        
        if (out_bio && in_bio) {
            // Write CMS in PEM format with streaming
            PEM_write_bio_CMS_stream(out_bio, cms, in_bio, 0);
        }
        
        if (out_bio) BIO_free(out_bio);
        if (in_bio) BIO_free(in_bio);
        
        CMS_ContentInfo_free(cms);
    }
    
    // Clean up ERR stack
    ERR_clear_error();
    
    assert_file_closed(&in_file);
	return 0;
}