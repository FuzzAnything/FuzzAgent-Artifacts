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
//<ID> 267
//<Prompt> []
/*<Combination>: [
*/
//<score> 120, nr_unique_branch: 127
//<Quality> {"density":16,"unique_branches":{"aria_192_ctr_newctx":[[223,13,223,24,0,0,4,0],[205,9,205,32,1,0,4,1]],"OSSL_PARAM_set_octet_ptr":[[1536,9,1536,18,0,0,4,1]],"evp_cipher_param_to_asn1_ex":[[87,9,87,18,0,0,4,1],[87,22,87,39,0,0,4,1],[102,9,102,72,0,0,4,0],[104,9,104,32,0,0,4,1],[110,9,110,31,0,0,4,1],[114,9,114,31,0,0,4,1],[115,9,115,31,0,0,4,1],[116,9,116,31,0,0,4,1],[120,9,120,16,0,0,4,0],[136,9,136,18,0,0,4,1],[138,14,138,22,0,0,4,1],[140,9,140,17,0,0,4,1]],"EVP_CIPHER_set_asn1_iv":[[72,9,72,21,0,0,4,0],[477,12,477,15,1,0,4,0]],"crl_cb":[[222,5,222,25,0,0,4,0],[223,13,223,32,0,0,4,1]],"EVP_CIPHER_CTX_original_iv":[[539,12,539,19,0,0,4,0]],"i2c_ibuf":[[104,9,104,18,0,0,4,0],[104,22,104,26,0,0,4,0],[107,13,107,17,0,0,4,0],[107,21,107,30,0,0,4,1],[110,20,110,23,0,0,4,1]],"ASN1_TYPE_set_octetstring":[[20,9,20,47,0,0,4,1],[22,9,22,46,0,0,4,1]],"EVP_RAND_CTX_get_params":[[418,9,418,28,0,0,4,1]],"EVP_CIPHER_CTX_ctrl":[[934,9,934,20,0,0,4,1],[934,24,934,43,0,0,4,1],[939,9,939,34,0,0,4,1],[945,5,945,33,0,0,4,1],[954,5,954,27,0,0,4,1],[960,5,960,23,0,0,4,1],[969,5,969,43,0,0,4,1],[970,5,970,12,0,0,4,0],[972,5,972,33,0,0,4,1],[981,5,981,28,0,0,4,1],[988,5,988,36,0,0,4,1],[992,5,992,29,0,0,4,1],[999,5,999,33,0,0,4,1],[1005,5,1005,33,0,0,4,1],[1007,5,1007,33,0,0,4,1],[1013,5,1013,28,0,0,4,1],[1019,5,1019,31,0,0,4,1],[1021,5,1021,31,0,0,4,1],[1025,5,1025,32,0,0,4,1],[1040,5,1040,35,0,0,4,1],[1042,5,1042,35,0,0,4,1],[1047,5,1047,48,0,0,4,1],[1061,5,1061,40,0,0,4,1],[1085,5,1085,44,0,0,4,1],[1108,5,1108,35,0,0,4,1],[1122,9,1122,40,0,0,4,0]],"SMIME_read_ASN1_ex":[[428,9,428,14,0,0,4,1]],"ASN1_item_pack":[[22,9,22,20,0,0,4,0],[23,13,23,48,0,0,4,1],[33,9,33,68,0,0,4,1],[37,9,37,28,0,0,4,1],[42,9,42,20,0,0,4,1]],"drbg_ctr_get_ctx_params_decoder":[[312,21,312,29,0,0,4,0],[323,21,323,29,0,0,4,1],[22,24,22,50,60,0,4,0],[23,26,23,52,61,0,4,1]],"ossl_drbg_get_ctx_params":[[885,9,885,25,0,0,4,0],[885,29,885,71,0,0,4,1],[888,9,888,23,0,0,4,1]],"ASN1_TYPE_pack_sequence":[[115,9,115,20,0,0,4,1],[118,9,118,10,0,0,4,0],[118,14,118,16,0,0,4,1],[122,13,122,23,0,0,4,1],[126,13,126,14,0,0,4,0]],"ossl_X509_ALGOR_from_nid":[[57,9,57,21,0,0,4,1],[59,9,59,41,0,0,4,1],[61,9,61,48,0,0,4,0]],"PKCS5_pbkdf2_set_ex":[[188,9,188,42,0,0,4,1],[192,9,192,50,0,0,4,1],[200,9,200,20,0,0,4,1],[204,9,204,21,0,0,4,1],[206,9,206,56,0,0,4,1],[211,9,211,13,0,0,4,0],[218,9,218,18,0,0,4,1],[221,9,221,43,0,0,4,1],[228,9,228,19,0,0,4,1],[240,9,240,20,0,0,4,0],[240,24,240,51,0,0,4,0],[242,13,242,29,0,0,4,1],[251,9,251,24,0,0,4,1],[260,9,261,33,0,0,4,1]],"OSSL_PARAM_set_octet_string_or_ptr":[[1711,9,1711,18,0,0,4,1],[1716,9,1716,48,0,0,4,1],[1718,14,1718,50,0,0,4,0]],"RAND_status":[[332,9,332,21,0,0,4,0],[332,25,332,47,0,0,4,1],[336,9,336,49,0,0,4,1]],"PKCS5_pbe2_set_iv_ex":[[63,9,63,29,0,0,4,1],[68,9,68,41,0,0,4,1],[76,9,76,54,0,0,4,1],[83,9,83,18,0,0,4,0],[84,13,84,16,0,0,4,1],[86,18,86,58,0,0,4,1],[91,9,91,20,0,0,4,1],[97,9,97,59,0,0,4,1],[99,9,99,62,0,0,4,1],[108,9,108,24,0,0,4,0],[108,28,108,92,0,0,4,0],[117,9,117,31,0,0,4,1],[129,9,129,30,0,0,4,1],[136,9,136,41,0,0,4,1],[145,9,146,29,0,0,4,1]],"OPENSSL_sk_sort":[[512,9,512,19,0,0,4,1]],"evp_cipher_init_internal":[[84,13,84,16,0,0,4,1],[204,9,204,12,0,0,4,1],[231,9,231,35,0,0,4,1],[249,9,249,20,0,0,4,0],[252,9,252,19,0,0,4,1]],"set_ptr_internal":[[1512,9,1512,29,0,0,4,1],[1517,9,1517,24,0,0,4,0]],"cipher_generic_get_ctx_params_decoder":[[222,21,222,29,0,0,4,1],[233,21,233,30,0,0,4,0],[23,26,23,52,6,0,4,1]],"ossl_cipher_common_get_ctx_params":[[570,9,570,22,0,0,4,0],[571,12,571,76,0,0,4,1]],"X509_CRL_sort":[[66,17,66,40,0,0,4,1]],"EVP_RAND_get_state":[[673,9,673,46,0,0,4,1]],"asn1_ex_i2c":[[569,5,569,21,0,0,4,0]],"cipher_generic_init_internal":[[159,16,159,19,0,0,4,1]]},"library_calls":["OPENSSL_init_ssl","OSSL_CRMF_MSG_new","EVP_aria_192_ctr","PKCS5_pbe2_set","RAND_status","PKCS7_SIGN_ENVELOPE_new","X509_CRL_new","PEM_read_X509_CRL","X509_CRL_free","d2i_X509_CRL_fp","X509_CRL_free","X509_CRL_sort","SSL_CTX_new","TLS_method","SSL_CTX_set_new_pending_conn_cb","BIO_new_fp","SMIME_read_PKCS7","BIO_new_fp","PEM_write_bio_PKCS7","BIO_free","PKCS7_free","BIO_free","PEM_read_X509","d2i_X509_fp","BIO_new_fp","X509_print","BIO_free","X509_free","OSSL_CRMF_MSG_free","X509_ALGOR_free","PKCS7_SIGN_ENVELOPE_free","X509_CRL_free","SSL_CTX_free"],"critical_calls":["OPENSSL_init_ssl","OSSL_CRMF_MSG_new","EVP_aria_192_ctr","PKCS5_pbe2_set","RAND_status","PKCS7_SIGN_ENVELOPE_new","X509_CRL_new","PEM_read_X509_CRL","d2i_X509_CRL_fp","X509_CRL_free","X509_CRL_sort","SSL_CTX_new","TLS_method","SSL_CTX_set_new_pending_conn_cb","BIO_new_fp","SMIME_read_PKCS7","BIO_new_fp","PEM_write_bio_PKCS7","BIO_free","PKCS7_free","BIO_free","PEM_read_X509","d2i_X509_fp","BIO_new_fp","X509_print","BIO_free","X509_free","OSSL_CRMF_MSG_free","X509_ALGOR_free","PKCS7_SIGN_ENVELOPE_free","X509_CRL_free","SSL_CTX_free"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL if needed
    OPENSSL_init_ssl(0, NULL);
    
    // Create a FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create new CRMF message
    OSSL_CRMF_MSG *crmf_msg = OSSL_CRMF_MSG_new();
    
    // 2. Create PBE2 algorithm using input data as salt
    // Use first 16 bytes as salt if available, otherwise use what we have
    int saltlen = size > 16 ? 16 : (int)size;
    unsigned char *salt = (unsigned char *)data;
    const EVP_CIPHER *cipher = EVP_aria_192_ctr(); // Using available cipher
    X509_ALGOR *pbe_algor = PKCS5_pbe2_set(cipher, 1000, salt, saltlen);
    
    // 3. Check random number generator status
    int rand_status = RAND_status();
    
    // 4. Create new PKCS7 signed and enveloped structure
    PKCS7_SIGN_ENVELOPE *p7_se = PKCS7_SIGN_ENVELOPE_new();
    
    // 5. Create and sort a CRL
    X509_CRL *crl = X509_CRL_new();
    if (crl) {
        // Try to parse CRL from input data if it looks like a CRL
        // First rewind the file
        fseek(in_file, 0, SEEK_SET);
        
        // Try to read as PEM CRL
        X509_CRL *parsed_crl = PEM_read_X509_CRL(in_file, NULL, NULL, NULL);
        if (parsed_crl) {
            X509_CRL_free(crl);
            crl = parsed_crl;
        } else {
            // If PEM read failed, rewind and try DER
            fseek(in_file, 0, SEEK_SET);
            parsed_crl = d2i_X509_CRL_fp(in_file, NULL);
            if (parsed_crl) {
                X509_CRL_free(crl);
                crl = parsed_crl;
            }
        }
        
        // Sort the CRL (even if empty or invalid)
        X509_CRL_sort(crl);
    }
    
    // 6. Create SSL context and set new pending connection callback
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        // Define a simple callback function
        auto new_pending_conn_cb = [](SSL_CTX *ctx, SSL *ssl, void *arg) -> int {
            // Simple callback that just returns 1
            return 1;
        };
        
        // Use the input data pointer as callback argument
        SSL_CTX_set_new_pending_conn_cb(ssl_ctx, new_pending_conn_cb, (void *)data);
    }
    
    // Try to use the input data with various OpenSSL functions
    // Rewind input file
    fseek(in_file, 0, SEEK_SET);
    
    // Try to read PKCS7 structure from input
    BIO *in_bio = BIO_new_fp(in_file, BIO_NOCLOSE);
    if (in_bio) {
        PKCS7 *p7 = SMIME_read_PKCS7(in_bio, NULL);
        if (p7) {
            // Write PKCS7 to output file
            BIO *out_bio = BIO_new_fp(out_file, BIO_NOCLOSE);
            if (out_bio) {
                PEM_write_bio_PKCS7(out_bio, p7);
                BIO_free(out_bio);
            }
            PKCS7_free(p7);
        }
        BIO_free(in_bio);
    }
    
    // Try to parse as X509 certificate
    fseek(in_file, 0, SEEK_SET);
    X509 *x509 = PEM_read_X509(in_file, NULL, NULL, NULL);
    if (!x509) {
        fseek(in_file, 0, SEEK_SET);
        x509 = d2i_X509_fp(in_file, NULL);
    }
    
    if (x509) {
        // Print certificate to output
        BIO *bio = BIO_new_fp(out_file, BIO_NOCLOSE);
        if (bio) {
            X509_print(bio, x509);
            BIO_free(bio);
        }
        X509_free(x509);
    }
    
    // Clean up all allocated resources
    if (crmf_msg) OSSL_CRMF_MSG_free(crmf_msg);
    if (pbe_algor) X509_ALGOR_free(pbe_algor);
    if (p7_se) PKCS7_SIGN_ENVELOPE_free(p7_se);
    if (crl) X509_CRL_free(crl);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}