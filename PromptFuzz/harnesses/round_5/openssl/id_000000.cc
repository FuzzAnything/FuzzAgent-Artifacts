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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 108, nr_unique_branch: 23
//<Quality> {"density":9,"unique_branches":{"EVP_PKEY_Q_keygen":[[1025,9,1025,45,0,0,4,1],[1028,16,1028,51,0,0,4,0]],"BN_mul_word":[[186,9,186,15,0,0,4,1]],"bn_mul_fixed_top":[[550,9,550,36,0,0,4,0],[550,40,550,67,0,0,4,1]],"BN_mod_exp_mont_word":[[1204,13,1204,33,0,0,4,0],[1212,9,1212,15,0,0,4,0]],"ossl_ec_curve_name2nid":[[138,13,138,66,0,0,4,0]],"EVP_PKEY_CTX_new":[[257,9,257,21,0,0,4,1]],"int_ctx_new":[[78,13,78,25,0,0,4,1],[78,29,78,50,0,0,4,1],[81,17,81,29,0,0,4,0],[81,17,81,29,0,0,4,1],[86,17,86,32,0,0,4,0],[88,21,88,36,0,0,4,1],[114,29,114,50,0,0,4,0],[115,17,115,51,0,0,4,1]],"ASN1_mbstring_ncopy":[[110,26,110,43,0,0,4,0]],"evp_pkey_keygen":[[1005,9,1005,20,0,0,4,0],[1006,12,1006,41,0,0,4,0],[1007,12,1007,48,0,0,4,0]],"ossl_ec_curve_nist2nid_int":[[184,13,184,51,0,0,4,0]],"ASN1_STRING_set_by_NID":[[93,13,93,43,0,0,4,1]]},"library_calls":["SSL_CTX_new","TLS_method","SSL_CTX_set_default_verify_dir","X509_REQ_new","X509_NAME_new","X509_NAME_add_entry_by_txt","X509_REQ_set_subject_name","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","SRP_Calc_B_ex","EVP_MD_CTX_new","EVP_MD_CTX_gettable_params","EVP_PKEY_Q_keygen","EVP_PKEY_CTX_new","EVP_PKEY_decapsulate","OCSP_SIGNATURE_it","BIO_dgram_non_fatal_error","sk_OSSL_TARGETS_cmpfunc_thunk","EVP_PKEY_free","EVP_PKEY_CTX_free","EVP_MD_CTX_free","SSL_CTX_free","X509_REQ_free","X509_NAME_free","BN_free","BN_free","BN_free","BN_free","BN_free"],"critical_calls":["SSL_CTX_new","TLS_method","SSL_CTX_set_default_verify_dir","X509_REQ_new","X509_NAME_new","X509_NAME_add_entry_by_txt","X509_REQ_set_subject_name","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","SRP_Calc_B_ex","EVP_MD_CTX_new","EVP_MD_CTX_gettable_params","EVP_PKEY_Q_keygen","EVP_PKEY_CTX_new","EVP_PKEY_decapsulate","OCSP_SIGNATURE_it","BIO_dgram_non_fatal_error","sk_OSSL_TARGETS_cmpfunc_thunk","EVP_PKEY_free","EVP_PKEY_CTX_free","EVP_MD_CTX_free","SSL_CTX_free","X509_REQ_free","X509_NAME_free","BN_free","BN_free","BN_free","BN_free","BN_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize variables
    SSL_CTX *ssl_ctx = nullptr;
    EVP_PKEY_CTX *pkey_ctx = nullptr;
    EVP_MD_CTX *md_ctx = nullptr;
    X509_REQ *req = nullptr;
    X509_NAME *name = nullptr;
    BIGNUM *b = nullptr, *N = nullptr, *g = nullptr, *v = nullptr, *B = nullptr;
    OSSL_LIB_CTX *libctx = nullptr;
    const OSSL_PARAM *gettable_params = nullptr;
    
    // 1. Create SSL_CTX and set default verify directory
    ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        SSL_CTX_set_default_verify_dir(ssl_ctx);
    }
    
    // 2. Create X509_REQ and set subject name
    req = X509_REQ_new();
    if (req && size > 0) {
        // Create a simple X509_NAME from first byte of data
        name = X509_NAME_new();
        if (name) {
            // Use first byte as country code if printable
            if (data[0] >= 32 && data[0] <= 126) {
                char country[2] = {(char)data[0], '\0'};
                X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, 
                                          (const unsigned char*)country, -1, -1, 0);
            }
            X509_REQ_set_subject_name(req, name);
        }
    }
    
    // 3. Calculate SRP B value using data as parameters
    if (size >= 4) {
        // Use data segments for SRP parameters
        b = BN_bin2bn(data, size/4 > 32 ? 32 : size/4, nullptr);
        N = BN_bin2bn(data + size/4, size/4 > 32 ? 32 : size/4, nullptr);
        g = BN_bin2bn(data + 2*(size/4), size/4 > 32 ? 32 : size/4, nullptr);
        v = BN_bin2bn(data + 3*(size/4), size - 3*(size/4), nullptr);
        
        if (b && N && g && v) {
            B = SRP_Calc_B_ex(b, N, g, v, libctx, nullptr);
        }
    }
    
    // 4. Create EVP_MD_CTX and get table parameters
    md_ctx = EVP_MD_CTX_new();
    if (md_ctx) {
        gettable_params = EVP_MD_CTX_gettable_params(md_ctx);
    }
    
    // 5. Create PKEY context for decapsulation
    // Generate a temporary EC key for testing
    EVP_PKEY *pkey = EVP_EC_gen("P-256");
    if (pkey) {
        pkey_ctx = EVP_PKEY_CTX_new(pkey, nullptr);
        if (pkey_ctx && size > 0) {
            unsigned char *unwrapped = nullptr;
            size_t unwrappedlen = 0;
            // Try decapsulation with data as wrapped key
            EVP_PKEY_decapsulate(pkey_ctx, unwrapped, &unwrappedlen, 
                               data, size > 256 ? 256 : size);
            free(unwrapped);
        }
    }
    
    // 6. Use OCSP_SIGNATURE_it
    const ASN1_ITEM *ocsp_sig_it = OCSP_SIGNATURE_it();
    (void)ocsp_sig_it; // Prevent unused variable warning
    
    // 7. Use BIO_dgram_non_fatal_error with simulated error
    int error = size > 0 ? data[0] : EINVAL;
    BIO_dgram_non_fatal_error(error);
    
    // 8. Use sk_OSSL_TARGETS_cmpfunc_thunk with dummy comparison
    if (size >= 2) {
        // Create dummy comparison function
        int (*dummy_cmp)(const void *, const void *) = [](const void *a, const void *b) {
            return 0;
        };
        sk_OSSL_TARGETS_cmpfunc_thunk(dummy_cmp, data, data + 1);
    }
    
    // Cleanup
    if (pkey) EVP_PKEY_free(pkey);
    if (pkey_ctx) EVP_PKEY_CTX_free(pkey_ctx);
    if (md_ctx) EVP_MD_CTX_free(md_ctx);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    if (req) X509_REQ_free(req);
    if (name) X509_NAME_free(name);
    if (b) BN_free(b);
    if (N) BN_free(N);
    if (g) BN_free(g);
    if (v) BN_free(v);
    if (B) BN_free(B);
    
    return 0;
}