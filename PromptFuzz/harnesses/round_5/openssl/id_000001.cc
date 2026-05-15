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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 37.5, nr_unique_branch: 5
//<Quality> {"density":15,"unique_branches":{"bn_sqr_words":[[178,12,178,18,0,0,4,0]],"bn_mul_fixed_top":[[541,13,541,20,0,0,4,0],[542,17,542,43,0,0,4,1]],"bn_mul_normal":[[640,13,640,22,0,0,4,1]],"EVP_PKEY_decapsulate":[[267,13,267,27,0,0,4,1]]},"library_calls":["OPENSSL_init_ssl","ERR_clear_error","SSL_CTX_new","TLS_method","SSL_CTX_set_default_verify_dir","X509_REQ_new","X509_NAME_new","X509_NAME_add_entry_by_txt","X509_REQ_set_subject_name","X509_NAME_free","BN_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","SRP_Calc_B_ex","BN_free","EVP_MD_CTX_new","EVP_MD_CTX_gettable_params","BIO_dgram_non_fatal_error","OCSP_SIGNATURE_it","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OSSL_TARGET_new","OSSL_TARGET_new","sk_OSSL_TARGETS_cmpfunc_thunk","OSSL_TARGET_free","OSSL_TARGET_free","OPENSSL_sk_free","ossl_check_OSSL_TARGET_sk_type","EVP_PKEY_CTX_new_id","EVP_PKEY_decapsulate","BN_free","BN_free","BN_free","BN_free","X509_REQ_free","EVP_MD_CTX_free","EVP_PKEY_CTX_free","SSL_CTX_free"],"critical_calls":["OPENSSL_init_ssl","ERR_clear_error","SSL_CTX_new","TLS_method","SSL_CTX_set_default_verify_dir","X509_REQ_new","X509_NAME_new","X509_NAME_add_entry_by_txt","X509_REQ_set_subject_name","X509_NAME_free","BN_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","BN_bin2bn","SRP_Calc_B_ex","BN_free","EVP_MD_CTX_new","EVP_MD_CTX_gettable_params","BIO_dgram_non_fatal_error","OCSP_SIGNATURE_it","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OSSL_TARGET_new","OSSL_TARGET_new","sk_OSSL_TARGETS_cmpfunc_thunk","OSSL_TARGET_free","OSSL_TARGET_free","OPENSSL_sk_free","ossl_check_OSSL_TARGET_sk_type","EVP_PKEY_CTX_new_id","EVP_PKEY_decapsulate","BN_free","BN_free","BN_free","BN_free","X509_REQ_free","EVP_MD_CTX_free","EVP_PKEY_CTX_free","SSL_CTX_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for operations

    // Initialize OpenSSL
    OPENSSL_init_ssl(0, NULL);
    ERR_clear_error();

    // 1. Create SSL_CTX and set default verify directory
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        SSL_CTX_set_default_verify_dir(ssl_ctx);
    }

    // 2. Create X509_REQ and set subject name
    X509_REQ *req = X509_REQ_new();
    if (req) {
        X509_NAME *name = X509_NAME_new();
        if (name) {
            // Use first 50 bytes as common name
            size_t name_len = size > 50 ? 50 : size;
            X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, 
                                      data, name_len, -1, 0);
            X509_REQ_set_subject_name(req, name);
            X509_NAME_free(name);
        }
    }

    // 3. SRP calculation using data chunks
    BIGNUM *b = BN_new();
    BIGNUM *N = BN_new();
    BIGNUM *g = BN_new();
    BIGNUM *v = BN_new();
    
    if (b && N && g && v && size >= 128) {
        // Use different portions of data for SRP parameters
        size_t chunk = size / 4;
        BN_bin2bn(data, chunk > 64 ? 64 : chunk, b);
        BN_bin2bn(data + chunk, chunk > 64 ? 64 : chunk, N);
        BN_bin2bn(data + 2*chunk, chunk > 64 ? 64 : chunk, g);
        BN_bin2bn(data + 3*chunk, chunk > 64 ? 64 : chunk, v);
        
        BIGNUM *B = SRP_Calc_B_ex(b, N, g, v, NULL, NULL);
        if (B) BN_free(B);
    }

    // 4. Create EVP_MD_CTX and get table parameters
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    if (md_ctx) {
        const OSSL_PARAM *gettable = EVP_MD_CTX_gettable_params(md_ctx);
        // Just access the table, no need to use it
        (void)gettable;
    }

    // 5. Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read some data to simulate usage
        unsigned char buf[100];
        fread(buf, 1, sizeof(buf), in_file);
        assert_file_closed(&in_file);;
    }

    // 6. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some processed data
        if (size > 0) {
            fwrite(data, 1, size > 100 ? 100 : size, out_file);
        }
        assert_file_closed(&out_file);;
    }

    // 7. Get file descriptor and check for non-fatal BIO errors
    if (out_file) {
        int fd = fuzz_fileno(out_file);
        (void)fd;  // Use fd to avoid unused warning
    }
    
    // Simulate a datagram BIO error check
    int error_code = size > 0 ? data[0] : 0;
    BIO_dgram_non_fatal_error(error_code);

    // 8. Get OCSP_SIGNATURE ASN1_ITEM
    const ASN1_ITEM *ocsp_sig_item = OCSP_SIGNATURE_it();
    (void)ocsp_sig_item;  // Use to avoid unused warning

    // 9. Create OSSL_TARGET stack and use comparison function
    struct stack_st_OSSL_TARGET *targets = sk_OSSL_TARGET_new_null();
    if (targets && size >= 20) {
        // Create dummy targets for comparison
        OSSL_TARGET *target1 = OSSL_TARGET_new();
        OSSL_TARGET *target2 = OSSL_TARGET_new();
        
        if (target1 && target2) {
            // Use data for comparison
            int (*cmp_func)(const void *, const void *) = 
                [](const void *a, const void *b) -> int {
                    return memcmp(a, b, 16);
                };
            
            sk_OSSL_TARGETS_cmpfunc_thunk(cmp_func, 
                                          data, 
                                          data + (size > 16 ? 16 : 0));
        }
        
        if (target1) OSSL_TARGET_free(target1);
        if (target2) OSSL_TARGET_free(target2);
        sk_OSSL_TARGET_free(targets);
    }

    // 10. Create EVP_PKEY_CTX for decapsulation (simulated)
    EVP_PKEY_CTX *pkey_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (pkey_ctx && size >= 64) {
        unsigned char unwrapped[256];
        size_t unwrappedlen = sizeof(unwrapped);
        
        // Use middle portion of data as wrapped key
        size_t wrapped_offset = size > 128 ? size/2 - 32 : 0;
        size_t wrappedlen = size > 64 ? 64 : size;
        
        EVP_PKEY_decapsulate(pkey_ctx, 
                            unwrapped, &unwrappedlen,
                            data + wrapped_offset, wrappedlen);
    }

    // Cleanup
    if (b) BN_free(b);
    if (N) BN_free(N);
    if (g) BN_free(g);
    if (v) BN_free(v);
    if (req) X509_REQ_free(req);
    if (md_ctx) EVP_MD_CTX_free(md_ctx);
    if (pkey_ctx) EVP_PKEY_CTX_free(pkey_ctx);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}