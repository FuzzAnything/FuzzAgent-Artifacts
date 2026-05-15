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
//<ID> 237
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 5
//<Quality> {"density":6,"unique_branches":{"BF_set_key":[[38,13,38,21,0,0,4,0],[43,13,43,21,0,0,4,0],[48,13,48,21,0,0,4,0]],"PEM_write_bio":[[653,9,653,23,0,0,4,1],[654,9,654,14,0,0,4,1]]},"library_calls":["d2i_OSSL_CRMF_CERTTEMPLATE","DSA_meth_new","DSA_meth_set_paramgen","OSSL_FUNC_keymgmt_new","OSSL_FUNC_signature_digest_verify_init","COMP_zstd_oneshot","BF_set_key","BF_ecb_encrypt","BIO_new","BIO_s_mem","PEM_write_bio","BIO_free","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_IPAddressFamily_sk_type","OPENSSL_sk_free","ossl_check_IPAddressFamily_sk_type","OSSL_CRMF_CERTTEMPLATE_free","DSA_meth_free"],"critical_calls":["d2i_OSSL_CRMF_CERTTEMPLATE","DSA_meth_new","DSA_meth_set_paramgen","OSSL_FUNC_keymgmt_new","OSSL_FUNC_signature_digest_verify_init","COMP_zstd_oneshot","BF_set_key","BF_ecb_encrypt","BIO_new","BIO_s_mem","PEM_write_bio","BIO_free","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_IPAddressFamily_sk_type","OPENSSL_sk_free","ossl_check_IPAddressFamily_sk_type","OSSL_CRMF_CERTTEMPLATE_free","DSA_meth_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. First, we need to parse an OSSL_CRMF_CERTTEMPLATE from the input data
    OSSL_CRMF_CERTTEMPLATE *cert_template = NULL;
    const unsigned char *p = data;
    cert_template = d2i_OSSL_CRMF_CERTTEMPLATE(NULL, &p, size);
    
    // If parsing failed, we still need to use other APIs
    size_t consumed = p - data;
    size_t remaining = size - consumed;
    
    // 2. Create a DSA method and set paramgen function
    DSA_METHOD *dsa_method = DSA_meth_new("Fuzz DSA Method", 0);
    if (dsa_method) {
        // We need to provide a paramgen function - use a simple one
        DSA_meth_set_paramgen(dsa_method, [](DSA *dsa, int bits, 
                                           const unsigned char *seed, 
                                           int seed_len, int *counter,
                                           unsigned long *h, BN_GENCB *cb) -> int {
            return 0; // Simple stub implementation
        });
    }
    
    // 3. Get function pointers from dispatch table
    // We need to create a minimal dispatch table for the function pointer lookups
    OSSL_DISPATCH dummy_dispatch[] = {
        {0, NULL}
    };
    
    // Call the function pointer getters (even though we don't use the results)
    OSSL_FUNC_keymgmt_new_fn *keymgmt_new = OSSL_FUNC_keymgmt_new(dummy_dispatch);
    OSSL_FUNC_signature_digest_verify_init_fn *sig_digest_verify_init = 
        OSSL_FUNC_signature_digest_verify_init(dummy_dispatch);
    
    // 4. Get Zstd compression method
    COMP_METHOD *zstd_method = COMP_zstd_oneshot();
    
    // 5. Prepare for Blowfish encryption
    BF_KEY bf_key;
    unsigned char bf_in[8] = {0};
    unsigned char bf_out[8] = {0};
    
    // Use some data for Blowfish key setup
    if (remaining >= 8) {
        // Use first 8 bytes of remaining data as input
        memcpy(bf_in, data + consumed, 8);
        
        // Set up a simple key (Blowfish key can be up to 56 bytes)
        unsigned char key_data[16] = {0};
        size_t key_len = remaining > 16 ? 16 : remaining;
        if (key_len > 0) {
            memcpy(key_data, data + consumed, key_len);
        }
        BF_set_key(&bf_key, key_len, key_data);
        
        // Perform ECB encryption
        BF_ecb_encrypt(bf_in, bf_out, &bf_key, BF_ENCRYPT);
    }
    
    // 6. Write encrypted data to BIO in PEM format
    BIO *bio = BIO_new(BIO_s_mem());
    if (bio) {
        // Write the encrypted data as a PEM block
        PEM_write_bio(bio, "ENCRYPTED DATA", NULL, bf_out, sizeof(bf_out));
        BIO_free(bio);
    }
    
    // 7. Use the IPAddressFamily stack type function
    // Create a dummy stack and convert it
    struct stack_st_IPAddressFamily *ip_stack = sk_IPAddressFamily_new_null();
    if (ip_stack) {
        OPENSSL_STACK *ossl_stack = ossl_check_IPAddressFamily_sk_type(ip_stack);
        sk_IPAddressFamily_free(ip_stack);
    }
    
    // 8. Clean up all allocated resources
    if (cert_template) {
        OSSL_CRMF_CERTTEMPLATE_free(cert_template);
    }
    
    if (dsa_method) {
        DSA_meth_free(dsa_method);
    }
    
    // Note: We don't need to free the function pointers or compression method
    // as they're just returned pointers, not allocated resources
    
    return 0;
}