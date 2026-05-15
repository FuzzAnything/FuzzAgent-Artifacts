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
//<ID> 299
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["sk_OCSP_RESPONSE_new_null","sk_OCSP_RESPONSE_zero","d2i_OCSP_RESPONSE","sk_OCSP_RESPONSE_push","ossl_check_const_OPENSSL_CSTRING_lh_plain_type","OSSL_LIB_CTX_new","OSSL_LIB_CTX_get_conf_diagnostics","EVP_PKEY_CTX_new_id","EVP_sha256","EVP_PKEY_CTX_set_ecdh_kdf_md","d2i_RSAPrivateKey_fp","RSA_get_multi_prime_extra_count","RSA_free","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","BIO_new_mem_buf","d2i_X509_bio","OPENSSL_sk_push","ossl_check_X509_sk_type","ossl_check_X509_type","BIO_free","OSSL_CMP_ITAV_new_caCerts","OSSL_CMP_ITAV_free","OPENSSL_sk_free","ossl_check_X509_sk_type","ossl_check_SRP_gN_freefunc_type","sk_OCSP_RESPONSE_pop_free","EVP_PKEY_CTX_free","OSSL_LIB_CTX_free"],"critical_calls":["sk_OCSP_RESPONSE_new_null","sk_OCSP_RESPONSE_zero","d2i_OCSP_RESPONSE","sk_OCSP_RESPONSE_push","ossl_check_const_OPENSSL_CSTRING_lh_plain_type","OSSL_LIB_CTX_new","OSSL_LIB_CTX_get_conf_diagnostics","EVP_PKEY_CTX_new_id","EVP_sha256","EVP_PKEY_CTX_set_ecdh_kdf_md","d2i_RSAPrivateKey_fp","RSA_get_multi_prime_extra_count","RSA_free","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","BIO_new_mem_buf","d2i_X509_bio","OPENSSL_sk_push","ossl_check_X509_type","ossl_check_X509_sk_type","BIO_free","OSSL_CMP_ITAV_new_caCerts","OSSL_CMP_ITAV_free","OPENSSL_sk_free","ossl_check_X509_sk_type","ossl_check_SRP_gN_freefunc_type","sk_OCSP_RESPONSE_pop_free","EVP_PKEY_CTX_free","OSSL_LIB_CTX_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Create a stack of OCSP_RESPONSE and zero it
    struct stack_st_OCSP_RESPONSE *ocsp_stack = sk_OCSP_RESPONSE_new_null();
    if (ocsp_stack) {
        sk_OCSP_RESPONSE_zero(ocsp_stack);
        
        // Try to parse OCSP response from input data
        const unsigned char *p = data;
        OCSP_RESPONSE *resp = d2i_OCSP_RESPONSE(NULL, &p, size/2);
        if (resp) {
            sk_OCSP_RESPONSE_push(ocsp_stack, resp);
        }
    }
    
    // 2. Use type-checking function for OPENSSL_CSTRING
    const OPENSSL_CSTRING test_string = "test";
    const OPENSSL_CSTRING *checked_ptr = 
        ossl_check_const_OPENSSL_CSTRING_lh_plain_type(&test_string);
    
    // 3. Get configuration diagnostics from OSSL_LIB_CTX
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    if (libctx) {
        int diag = OSSL_LIB_CTX_get_conf_diagnostics(libctx);
        (void)diag; // Use variable
    }
    
    // 4. Set ECDH KDF message digest in EVP_PKEY_CTX
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
    if (pctx) {
        const EVP_MD *md = EVP_sha256();
        EVP_PKEY_CTX_set_ecdh_kdf_md(pctx, md);
    }
    
    // 5. Get multi-prime extra count from RSA
    // Create RSA key from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        RSA *rsa = d2i_RSAPrivateKey_fp(in_file, NULL);
        if (rsa) {
            int extra_count = RSA_get_multi_prime_extra_count(rsa);
            (void)extra_count; // Use variable
            RSA_free(rsa);
        }
        assert_file_closed(&in_file);;
    }
    
    // 6. Create OSSL_CMP_ITAV with CA certificates
    // Create a stack of X509 certificates
    struct stack_st_X509 *ca_stack = sk_X509_new_null();
    if (ca_stack) {
        // Try to parse a certificate from the remaining data
        const unsigned char *cert_data = data + (size > 50 ? 50 : 0);
        size_t cert_size = size > 50 ? size - 50 : 0;
        
        BIO *bio = BIO_new_mem_buf(cert_data, cert_size);
        if (bio) {
            X509 *cert = d2i_X509_bio(bio, NULL);
            if (cert) {
                sk_X509_push(ca_stack, cert);
            }
            BIO_free(bio);
        }
        
        OSSL_CMP_ITAV *itav = OSSL_CMP_ITAV_new_caCerts(ca_stack);
        if (itav) {
            OSSL_CMP_ITAV_free(itav);
        }
        // Don't free individual certs as they're owned by the stack
        sk_X509_free(ca_stack);
    }
    
    // 7. Type-check SRP_gN free function
    OPENSSL_sk_freefunc srp_freefunc = 
        ossl_check_SRP_gN_freefunc_type((sk_SRP_gN_freefunc)sk_free);
    
    // Clean up resources
    if (ocsp_stack) {
        sk_OCSP_RESPONSE_pop_free(ocsp_stack, OCSP_RESPONSE_free);
    }
    
    if (pctx) {
        EVP_PKEY_CTX_free(pctx);
    }
    
    if (libctx) {
        OSSL_LIB_CTX_free(libctx);
    }
    
    assert_file_closed(&in_file);
	return 0;
}