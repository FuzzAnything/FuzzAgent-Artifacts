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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 96
//<Quality> {"density":8,"unique_branches":{"rsa_keygen":[[635,32,635,57,0,0,4,1]],"ossl_rsa_multip_cap":[[102,9,102,20,0,0,4,1],[104,14,104,25,0,0,4,0]],"bn_sqr_words":[[187,9,187,10,0,0,4,1]],"RSAZ_512_mod_exp":[[287,21,287,31,0,0,4,0],[287,21,287,31,0,0,4,1],[297,22,297,32,0,0,4,0],[297,22,297,32,0,0,4,1]],"ASN1_item_d2i_ex":[[166,9,166,21,0,0,4,1]],"bn_mul_normal":[[624,9,624,16,0,0,4,0]],"bn_mul_part_recursive":[[319,9,319,14,0,0,4,1],[328,13,328,24,0,0,4,1],[329,5,329,12,0,0,4,1],[333,5,333,12,0,0,4,1],[334,5,334,12,0,0,4,1],[339,5,339,12,0,0,4,1],[340,5,340,11,0,0,4,1],[341,5,341,11,0,0,4,1],[342,5,342,11,0,0,4,0],[347,5,347,11,0,0,4,1],[348,5,348,11,0,0,4,1],[365,9,365,15,0,0,4,1],[378,13,378,22,0,0,4,1],[382,13,382,19,0,0,4,1],[386,20,386,25,0,0,4,1],[394,17,394,51,0,0,4,0],[395,20,395,54,0,0,4,0],[428,9,428,12,0,0,4,0],[442,9,442,11,0,0,4,1]],"ossl_ifc_ffc_compute_security_bits":[[310,13,310,14,0,0,4,0],[311,5,311,14,0,0,4,1],[333,9,333,20,0,0,4,1],[335,9,335,14,0,0,4,1],[343,9,343,18,0,0,4,0],[355,9,355,16,0,0,4,1]],"calc_trial_divisions":[[78,14,78,26,0,0,4,1],[80,14,80,26,0,0,4,0]],"icbrt64":[[238,18,238,24,0,0,4,0],[238,18,238,24,0,0,4,1],[241,13,241,26,0,0,4,0],[241,13,241,26,0,0,4,1]],"bn_sub_part_words":[[36,9,36,16,0,0,4,1],[43,9,43,15,0,0,4,1],[78,16,78,17,0,0,4,0],[78,16,78,17,0,0,4,1],[81,17,81,23,0,0,4,0],[83,17,83,26,0,0,4,1],[88,17,88,23,0,0,4,0],[90,17,90,26,0,0,4,1],[95,17,95,23,0,0,4,0],[97,17,97,26,0,0,4,1],[102,17,102,23,0,0,4,0],[104,17,104,26,0,0,4,1],[111,13,111,19,0,0,4,0],[112,17,112,29,0,0,4,1],[133,13,133,19,0,0,4,0],[136,21,136,30,0,0,4,0],[136,21,136,30,0,0,4,1],[139,21,139,30,0,0,4,0],[139,21,139,30,0,0,4,1],[142,21,142,30,0,0,4,0],[142,21,142,30,0,0,4,1],[145,21,145,30,0,0,4,0],[145,21,145,30,0,0,4,1]],"bn_sqr_fixed_top":[[59,16,59,23,0,0,4,0],[81,17,81,24,0,0,4,1],[86,21,86,49,0,0,4,1]],"bn_mod_exp_mont_fixed_top":[[678,27,678,41,0,0,4,1],[678,45,678,69,0,0,4,1],[689,16,689,29,0,0,4,0],[689,33,689,46,0,0,4,0],[689,50,689,73,0,0,4,0],[689,50,689,73,0,0,4,1],[690,13,690,38,0,0,4,1],[1138,9,1138,25,0,0,4,1]],"ilog_e":[[267,12,267,26,0,0,4,0],[267,12,267,26,0,0,4,1],[271,25,271,31,0,0,4,0],[271,25,271,31,0,0,4,1],[273,13,273,27,0,0,4,0],[273,13,273,27,0,0,4,1]],"bn_cmp_part_words":[[920,9,920,15,0,0,4,0],[921,22,921,27,0,0,4,0],[922,17,922,30,0,0,4,0],[926,9,926,15,0,0,4,0],[927,22,927,27,0,0,4,0],[928,17,928,30,0,0,4,0]],"rsa_multiprime_keygen":[[289,9,289,36,0,0,4,0]],"bn_mul_fixed_top":[[541,13,541,20,0,0,4,0],[542,17,542,43,0,0,4,1],[556,17,556,23,0,0,4,1],[559,17,559,24,0,0,4,0],[568,17,568,23,0,0,4,0],[568,27,568,33,0,0,4,0],[569,21,569,49,0,0,4,1],[571,21,571,50,0,0,4,1]]},"library_calls":["TLS_method","SSL_CTX_new","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","EVP_PKEY_new","BN_new","BN_new","BN_bin2bn","BN_rand","RSA_new","RSA_generate_key_ex","RSA_generate_key_ex","EVP_PKEY_assign","EVP_PKEY_get1_RSA","DH_meth_new","d2i_PKCS7_ENC_CONTENT","PKCS7_RECIP_INFO_new","RC2_set_key","RC2_cfb64_encrypt","BIO_dump_indent_fp","PKCS7_RECIP_INFO_free","PKCS7_ENC_CONTENT_free","DH_meth_free","RSA_free","BN_free","BN_free","EVP_PKEY_free","OSSL_CMP_CTX_free","SSL_CTX_free"],"critical_calls":["TLS_method","SSL_CTX_new","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","EVP_PKEY_new","BN_new","BN_new","BN_bin2bn","RSA_new","RSA_generate_key_ex","EVP_PKEY_assign","EVP_PKEY_get1_RSA","DH_meth_new","d2i_PKCS7_ENC_CONTENT","PKCS7_RECIP_INFO_new","RC2_set_key","RC2_cfb64_encrypt","BIO_dump_indent_fp","PKCS7_RECIP_INFO_free","PKCS7_ENC_CONTENT_free","DH_meth_free","RSA_free","BN_free","BN_free","EVP_PKEY_free","OSSL_CMP_CTX_free","SSL_CTX_free"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create SSL context with TLS method
    const SSL_METHOD *meth = TLS_method();
    SSL_CTX *ssl_ctx = SSL_CTX_new(meth);
    
    // 2. Set CT validation callback (dummy callback)
    ssl_ct_validation_cb dummy_cb = [](const CT_POLICY_EVAL_CTX *, const struct stack_st_SCT *, void *) { return 1; };
    SSL_CTX_set_ct_validation_callback(ssl_ctx, dummy_cb, nullptr);
    
    // 3. Create CMP context
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(nullptr, nullptr);
    void *http_arg = OSSL_CMP_CTX_get_http_cb_arg(cmp_ctx);
    (void)http_arg; // Use to avoid unused variable warning
    
    // 4. Create EVP_PKEY and generate RSA key
    EVP_PKEY *pkey = EVP_PKEY_new();
    BIGNUM *bn_e = BN_new();
    BIGNUM *bn_rand = BN_new();
    
    // Use input data for BN_bin2bn if we have enough data
    if (size >= 8) {
        BN_bin2bn(data, 8, bn_e);
    } else {
        // Generate random exponent if input too small
        BN_rand(bn_e, 16, 0, 0);
    }
    
    // Generate RSA key
    RSA *rsa = RSA_new();
    if (size >= 4) {
        int bits = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        bits = bits % 2048 + 512; // Ensure reasonable key size
        RSA_generate_key_ex(rsa, bits, bn_e, nullptr);
    } else {
        RSA_generate_key_ex(rsa, 1024, bn_e, nullptr);
    }
    
    EVP_PKEY_assign(pkey, EVP_PKEY_RSA, rsa);
    
    // Extract RSA key back (just for demonstration)
    RSA *extracted_rsa = EVP_PKEY_get1_RSA(pkey);
    
    // 5. Create DH method
    DH_METHOD *dh_meth = DH_meth_new("fuzz_dh", 0);
    
    // 6. Parse PKCS7 encrypted content from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    PKCS7_ENC_CONTENT *enc_content = nullptr;
    if (in_file) {
        // Try to read and parse PKCS7_ENC_CONTENT
        fseek(in_file, 0, SEEK_END);
        long file_size = ftell(in_file);
        fseek(in_file, 0, SEEK_SET);
        
        if (file_size > 0) {
            unsigned char *buf = (unsigned char *)malloc(file_size);
            if (buf) {
                fread(buf, 1, file_size, in_file);
                const unsigned char *p = buf;
                d2i_PKCS7_ENC_CONTENT(&enc_content, &p, file_size);
                free(buf);
            }
        }
        assert_file_closed(&in_file);;
    }
    
    // 7. Create and free PKCS7 recipient info
    PKCS7_RECIP_INFO *recip_info = PKCS7_RECIP_INFO_new();
    
    // 8. RC2 encryption/decryption using input data
    if (size >= 24) { // Need at least 16 bytes key + 8 bytes IV
        RC2_KEY rc2_key;
        unsigned char ivec[8];
        int num = 0;
        
        // Use first 16 bytes as key, next 8 as IV
        RC2_set_key(&rc2_key, 16, data, 64);
        for (int i = 0; i < 8; i++) {
            ivec[i] = data[16 + i];
        }
        
        // Encrypt/decrypt remaining data
        size_t rc2_data_len = size - 24;
        if (rc2_data_len > 0) {
            unsigned char *out = (unsigned char *)malloc(rc2_data_len);
            if (out) {
                RC2_cfb64_encrypt(data + 24, out, rc2_data_len, &rc2_key, ivec, &num, 1);
                free(out);
            }
        }
    }
    
    // 9. Dump input data to file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (size > 0) {
            BIO_dump_indent_fp(out_file, data, size < 256 ? size : 256, 4);
        }
        assert_file_closed(&out_file);;
    }
    
    // Cleanup - free all allocated resources
    if (recip_info) PKCS7_RECIP_INFO_free(recip_info);
    if (enc_content) PKCS7_ENC_CONTENT_free(enc_content);
    if (dh_meth) DH_meth_free(dh_meth);
    if (extracted_rsa) RSA_free(extracted_rsa);
    if (bn_rand) BN_free(bn_rand);
    if (bn_e) BN_free(bn_e);
    if (pkey) EVP_PKEY_free(pkey);
    if (cmp_ctx) OSSL_CMP_CTX_free(cmp_ctx);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}