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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 26
//<Quality> {"density":11,"unique_branches":{"BIO_puts":[[539,9,539,16,0,0,4,1],[549,9,549,16,0,0,4,1]],"mem_write":[[226,9,226,17,0,0,4,0]],"get_header_and_data":[[864,52,864,81,0,0,4,1]],"check_pem":[[136,9,136,47,0,0,4,1],[156,9,156,49,0,0,4,1],[174,9,174,46,0,0,4,1],[180,9,180,45,0,0,4,1],[184,9,184,49,0,0,4,1],[189,9,189,41,0,0,4,1],[193,9,193,45,0,0,4,1],[198,9,198,41,0,0,4,1],[202,9,202,49,0,0,4,1],[207,9,207,41,0,0,4,1],[211,9,211,42,0,0,4,1]],"mem_gets":[[356,9,356,41,0,0,4,0],[359,9,359,29,0,0,4,0],[360,9,360,23,0,0,4,0],[360,9,360,23,0,0,4,1],[362,9,362,15,0,0,4,0],[362,9,362,15,0,0,4,1],[367,17,367,22,0,0,4,0],[367,17,367,22,0,0,4,1],[368,13,368,25,0,0,4,0],[368,13,368,25,0,0,4,1],[380,9,380,14,0,0,4,0]]},"library_calls":["EVP_sha512_224","BIO_new_mem_buf","PEM_bytes_read_bio","UI_create_method","UI_method_set_prompt_constructor","UI_destroy_method","d2i_OCSP_REVOKEDINFO","SCT_new","i2o_SCT","CRYPTO_free","SCT_free","X509_NAME_ENTRY_new","DES_set_key_unchecked","DES_set_key_unchecked","DES_set_key_unchecked","DES_decrypt3","CRYPTO_free","CRYPTO_free","X509_NAME_ENTRY_free","OCSP_REVOKEDINFO_free","BIO_free"],"critical_calls":["EVP_sha512_224","BIO_new_mem_buf","PEM_bytes_read_bio","UI_create_method","UI_method_set_prompt_constructor","UI_destroy_method","d2i_OCSP_REVOKEDINFO","SCT_new","i2o_SCT","CRYPTO_free","SCT_free","X509_NAME_ENTRY_new","DES_set_key_unchecked","DES_set_key_unchecked","DES_set_key_unchecked","DES_decrypt3","CRYPTO_free","CRYPTO_free","X509_NAME_ENTRY_free","OCSP_REVOKEDINFO_free","BIO_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get SHA-512/224 digest method
    const EVP_MD *sha512_224 = EVP_sha512_224();
    if (!sha512_224) {
	return 0;
	}
	
    
    // 2. Create BIO from input data for PEM reading
    BIO *bio = BIO_new_mem_buf(data, size);
    if (!bio) {
	return 0;
	}
	
    
    // Read PEM data from BIO
    unsigned char *pem_data = nullptr;
    long pem_len = 0;
    char *pem_name = nullptr;
    int pem_ret = PEM_bytes_read_bio(&pem_data, &pem_len, &pem_name, 
                                     "CERTIFICATE", bio, nullptr, nullptr);
    
    // 3. Create and configure UI method
    UI_METHOD *ui_method = UI_create_method("FuzzUI");
    if (ui_method) {
        UI_method_set_prompt_constructor(ui_method, 
            [](UI *ui, const char *prompt_info, const char *object_name) -> char* {
                return strdup("fuzz-prompt");
            });
        UI_destroy_method(ui_method);
    }
    
    // 4. Parse OCSP revoked info from input data
    const uint8_t *ocsp_ptr = data;
    long ocsp_len = size;
    OCSP_REVOKEDINFO *revoked_info = d2i_OCSP_REVOKEDINFO(nullptr, &ocsp_ptr, ocsp_len);
    
    // 5. Create SCT and convert to output
    SCT *sct = SCT_new();
    if (sct) {
        unsigned char *sct_out = nullptr;
        int sct_len = i2o_SCT(sct, &sct_out);
        OPENSSL_free(sct_out);
        SCT_free(sct);
    }
    
    // 6. Create X509 name entry
    X509_NAME_ENTRY *name_entry = X509_NAME_ENTRY_new();
    
    // 7. Prepare DES decryption (needs 24 bytes for 3 keys)
    if (size >= 24 + 8) {  // 24 bytes for keys, 8 bytes for data
        DES_key_schedule ks1, ks2, ks3;
        DES_LONG des_data[2];
        
        // Use first 24 bytes for key schedules (8 bytes each)
        DES_set_key_unchecked((const_DES_cblock*)data, &ks1);
        DES_set_key_unchecked((const_DES_cblock*)(data + 8), &ks2);
        DES_set_key_unchecked((const_DES_cblock*)(data + 16), &ks3);
        
        // Use next 8 bytes for data
        memcpy(des_data, data + 24, 8);
        
        // Perform DES decryption
        DES_decrypt3(des_data, &ks1, &ks2, &ks3);
    }
    
    // Cleanup
    if (pem_data) OPENSSL_free(pem_data);
    if (pem_name) OPENSSL_free(pem_name);
    if (name_entry) X509_NAME_ENTRY_free(name_entry);
    if (revoked_info) OCSP_REVOKEDINFO_free(revoked_info);
    BIO_free(bio);
    
    return 0;
}