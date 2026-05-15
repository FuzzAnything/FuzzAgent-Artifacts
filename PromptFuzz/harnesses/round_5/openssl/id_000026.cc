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
//<ID> 239
//<Prompt> []
/*<Combination>: [
*/
//<score> 56, nr_unique_branch: 34
//<Quality> {"density":8,"unique_branches":{"EVP_EncodeFinal":[[458,9,458,22,0,0,4,1]],"x509_pubkey_ex_d2i_ex":[[193,13,193,39,0,0,4,0],[195,17,195,31,0,0,4,1]],"PEM_write_bio":[[667,19,667,44,0,0,4,0],[672,13,672,19,0,0,4,0],[672,23,672,65,0,0,4,1],[681,9,681,19,0,0,4,1]],"encode_base64_avx2":[[487,18,487,31,0,0,4,1],[489,26,489,37,0,0,4,0],[490,12,490,63,0,0,4,1],[496,12,496,29,0,0,4,0],[496,12,496,29,0,0,4,1],[544,13,544,20,0,0,4,1],[557,13,557,24,0,0,4,1],[570,20,570,32,0,0,4,0],[640,9,640,20,0,0,4,1],[663,9,663,21,0,0,4,0],[663,25,663,43,0,0,4,1]],"evp_encodeblock_int":[[231,21,231,77,0,0,4,0],[232,24,232,72,0,0,4,0],[242,5,242,11,0,0,4,0],[244,5,244,11,0,0,4,0],[253,13,253,24,0,0,4,0],[254,17,254,73,0,0,4,1],[263,5,263,11,0,0,4,1]],"EVP_EncodeUpdate":[[384,9,384,47,0,0,4,1],[389,9,389,22,0,0,4,0],[389,9,389,22,0,0,4,1],[415,13,415,52,0,0,4,0],[416,34,416,76,0,0,4,0],[435,9,435,55,0,0,4,0],[440,9,440,24,0,0,4,1],[445,9,445,17,0,0,4,0],[445,9,445,17,0,0,4,1]]},"library_calls":["d2i_OSSL_CRMF_CERTTEMPLATE","DSA_meth_new","DSA_meth_set_paramgen","BF_set_key","BF_ecb_encrypt","BIO_new","BIO_s_mem","PEM_write_bio","COMP_zstd_oneshot","OSSL_FUNC_keymgmt_new","OSSL_FUNC_signature_digest_verify_init","ossl_check_IPAddressFamily_sk_type","PEM_write_bio","BIO_ctrl","BIO_free","OSSL_CRMF_CERTTEMPLATE_free","DSA_meth_free"],"critical_calls":["d2i_OSSL_CRMF_CERTTEMPLATE","DSA_meth_new","DSA_meth_set_paramgen","BF_set_key","BF_ecb_encrypt","BIO_new","BIO_s_mem","PEM_write_bio","COMP_zstd_oneshot","OSSL_FUNC_keymgmt_new","OSSL_FUNC_signature_digest_verify_init","ossl_check_IPAddressFamily_sk_type","PEM_write_bio","BIO_ctrl","BIO_free","OSSL_CRMF_CERTTEMPLATE_free","DSA_meth_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 64) {
	return 0;
	}
	  // Minimal size for meaningful operations
    
    // 1. Parse OSSL_CRMF_CERTTEMPLATE from input data
    const unsigned char *p = data;
    OSSL_CRMF_CERTTEMPLATE *cert_tmpl = d2i_OSSL_CRMF_CERTTEMPLATE(NULL, &p, size);
    if (cert_tmpl) {
        // Cleanup will be done at the end
    }
    
    // 2. Create and setup DSA method with paramgen
    DSA_METHOD *dsa_method = DSA_meth_new("Fuzzer DSA Method", 0);
    if (dsa_method) {
        DSA_meth_set_paramgen(dsa_method, 
            [](DSA *dsa, int bits, const unsigned char *seed, int seed_len,
               int *counter, unsigned long *h, BN_GENCB *cb) -> int {
                return 1;  // Simplified paramgen
            });
        
        // Cleanup will be done at the end
    }
    
    // 3. Setup Blowfish ECB encryption
    BF_KEY bf_key;
    // Initialize Blowfish key with part of input data
    size_t key_len = size > 56 ? 56 : size;
    BF_set_key(&bf_key, key_len, data);
    
    // Prepare input/output buffers for Blowfish
    unsigned char bf_in[8] = {0};
    unsigned char bf_out[8] = {0};
    if (size >= 8) {
        memcpy(bf_in, data + (size - 8), 8);
        BF_ecb_encrypt(bf_in, bf_out, &bf_key, BF_ENCRYPT);
    }
    
    // 4. Create BIO for PEM writing
    BIO *bio = BIO_new(BIO_s_mem());
    if (bio) {
        // Write encrypted Blowfish data as PEM
        PEM_write_bio(bio, "BLOWFISH ENCRYPTED DATA", 
                     "-----BEGIN ENCRYPTED DATA-----\n",
                     bf_out, sizeof(bf_out));
        
        // 5. Get ZSTD compression method
        COMP_METHOD *zstd_method = COMP_zstd_oneshot();
        if (zstd_method) {
            // Method available, though we don't use it further in this example
        }
        
        // 6. Get OSSL function pointers (simulated usage)
        // These would normally come from provider dispatch tables
        OSSL_DISPATCH dummy_dispatch[] = {
            {0, NULL}
        };
        
        OSSL_FUNC_keymgmt_new_fn *keymgmt_new = 
            OSSL_FUNC_keymgmt_new(dummy_dispatch);
        OSSL_FUNC_signature_digest_verify_init_fn *sig_digest_verify_init = 
            OSSL_FUNC_signature_digest_verify_init(dummy_dispatch);
        
        // 7. Handle IPAddressFamily stack type
        struct stack_st_IPAddressFamily *ip_stack = NULL;
        OPENSSL_STACK *checked_stack = ossl_check_IPAddressFamily_sk_type(ip_stack);
        // Stack operations would normally go here
        
        // 8. Write additional data to BIO
        if (size > 16) {
            // Use remaining data for additional PEM write
            size_t pem_data_len = size - 16;
            const unsigned char *pem_data = data + 16;
            PEM_write_bio(bio, "FUZZ DATA", 
                         "-----BEGIN FUZZ DATA-----\n",
                         pem_data, 
                         pem_data_len > LONG_MAX ? LONG_MAX : (long)pem_data_len);
        }
        
        // Read back from BIO to consume output
        char *bio_data;
        long bio_len = BIO_get_mem_data(bio, &bio_data);
        if (bio_len > 0) {
            // Simulate processing of BIO output
            volatile size_t dummy = bio_len;
            (void)dummy;
        }
        
        BIO_free(bio);
    }
    
    // 9. Cleanup all allocated resources
    if (cert_tmpl) {
        OSSL_CRMF_CERTTEMPLATE_free(cert_tmpl);
    }
    
    if (dsa_method) {
        DSA_meth_free(dsa_method);
    }
    
    // Clear sensitive data
    memset(&bf_key, 0, sizeof(bf_key));
    memset(bf_in, 0, sizeof(bf_in));
    memset(bf_out, 0, sizeof(bf_out));
    
    return 0;
}