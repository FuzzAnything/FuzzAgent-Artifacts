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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 70, nr_unique_branch: 55
//<Quality> {"density":7,"unique_branches":{"CONF_module_add":[[585,9,585,45,0,0,4,0]],"nist_cp_bn":[[279,17,279,24,0,0,4,0],[279,17,279,24,0,0,4,1]],"DES_ede3_cbc_encrypt":[[219,9,219,12,0,0,4,0],[222,22,222,28,0,0,4,0],[222,22,222,28,0,0,4,1],[237,13,237,20,0,0,4,0],[237,13,237,20,0,0,4,1],[39,17,39,18,7,0,4,1],[40,9,40,15,7,0,4,1],[43,9,43,15,7,0,4,0],[43,9,43,15,7,0,4,1],[46,9,46,15,7,0,4,0],[46,9,46,15,7,0,4,1],[49,9,49,15,7,0,4,0],[49,9,49,15,7,0,4,1],[52,9,52,15,7,0,4,0],[52,9,52,15,7,0,4,1],[55,9,55,15,7,0,4,0],[55,9,55,15,7,0,4,1],[58,9,58,15,7,0,4,0],[58,9,58,15,7,0,4,1],[61,9,61,15,7,0,4,0],[61,9,61,15,7,0,4,1]],"ssl3_ctx_ctrl":[[4545,5,4545,43,0,0,4,0],[4559,5,4559,34,0,0,4,1]],"bn2binpad":[[591,9,591,18,0,0,4,0],[592,13,592,23,0,0,4,1]],"X509_VERIFY_PARAM_set1":[[499,9,499,19,0,0,4,1]],"module_add":[[303,9,303,18,0,0,4,1],[310,9,310,28,0,0,4,0],[315,9,315,28,0,0,4,1],[318,9,318,55,0,0,4,1],[325,9,325,27,0,0,4,1],[328,9,328,48,0,0,4,1],[130,6,130,48,3,0,4,0]],"BN_nist_mod_224":[[506,9,506,26,0,0,4,1],[506,30,506,74,0,0,4,1],[510,9,510,15,0,0,4,1],[513,16,513,21,0,0,4,0],[513,16,513,21,0,0,4,1],[514,16,514,24,0,0,4,1],[516,9,516,15,0,0,4,0],[517,13,517,44,0,0,4,1],[611,9,611,18,0,0,4,0],[611,9,611,18,0,0,4,1],[617,16,617,25,0,0,4,1],[632,12,632,64,0,0,4,0],[632,12,632,64,0,0,4,1],[632,68,632,73,0,0,4,0],[269,22,269,30,7,0,4,0],[269,22,269,30,7,0,4,1],[271,16,271,24,7,0,4,0],[271,16,271,24,7,0,4,1]],"do_init_module_list_lock":[[67,9,67,33,0,0,4,1]]},"library_calls":["SSL_CTX_new","TLS_method","X509_STORE_new","X509_VERIFY_PARAM_new","BN_new","BN_new","BN_new","BN_CTX_new","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_PKCS7_RECIP_INFO_sk_type","X509_ATTRIBUTE_new","X509_STORE_set1_param","ASN1_add_oid_module","BN_bin2bn","BN_bin2bn","BN_nist_mod_224","SSL_CTX_set_srp_strength","DH_OpenSSL","DES_set_key_unchecked","DES_set_key_unchecked","DES_set_key_unchecked","DES_ede3_cbc_encrypt","OCSP_SINGLERESP_new","BN_num_bits","BN_bn2bin","OCSP_SINGLERESP_free","X509_ATTRIBUTE_free","OPENSSL_sk_free","ossl_check_PKCS7_RECIP_INFO_sk_type","SSL_CTX_free","X509_STORE_free","X509_VERIFY_PARAM_free","BN_free","BN_free","BN_free","BN_CTX_free"],"critical_calls":["SSL_CTX_new","TLS_method","X509_STORE_new","X509_VERIFY_PARAM_new","BN_new","BN_new","BN_new","BN_CTX_new","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_PKCS7_RECIP_INFO_sk_type","X509_ATTRIBUTE_new","X509_STORE_set1_param","ASN1_add_oid_module","BN_bin2bn","BN_bin2bn","BN_nist_mod_224","SSL_CTX_set_srp_strength","DH_OpenSSL","DES_set_key_unchecked","DES_set_key_unchecked","DES_set_key_unchecked","DES_ede3_cbc_encrypt","OCSP_SINGLERESP_new","BN_num_bits","BN_bn2bin","OCSP_SINGLERESP_free","X509_ATTRIBUTE_free","OPENSSL_sk_free","ossl_check_PKCS7_RECIP_INFO_sk_type","SSL_CTX_free","X509_STORE_free","X509_VERIFY_PARAM_free","BN_free","BN_free","BN_free","BN_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Initialize OpenSSL structures
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    X509_STORE *store = X509_STORE_new();
    X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
    BIGNUM *bn_a = BN_new();
    BIGNUM *bn_p = BN_new();
    BIGNUM *bn_r = BN_new();
    BN_CTX *bn_ctx = BN_CTX_new();
    DES_key_schedule ks1, ks2, ks3;
    DES_cblock ivec;
    
    // 3. Use ossl_check_PKCS7_RECIP_INFO_sk_type
    // Create a dummy stack to use with the function
    struct stack_st_PKCS7_RECIP_INFO *recip_stack = sk_PKCS7_RECIP_INFO_new_null();
    if (recip_stack) {
        OPENSSL_STACK *checked_stack = ossl_check_PKCS7_RECIP_INFO_sk_type(recip_stack);
        // Use the checked stack (dummy operation)
        if (checked_stack) {
            // Just to ensure the stack is used
        }
    }

    // 4. Use X509_ATTRIBUTE_new
    X509_ATTRIBUTE *attr = X509_ATTRIBUTE_new();
    
    // 5. Use X509_STORE_set1_param
    if (store && param) {
        X509_STORE_set1_param(store, param);
    }

    // 6. Use ASN1_add_oid_module
    ASN1_add_oid_module();

    // 7. Use BN_nist_mod_224
    // Read BIGNUM data from input file
    if (bn_a && bn_p && bn_r && bn_ctx) {
        // Read some data for BIGNUMs (simplified - in reality would need proper DER parsing)
        unsigned char buf[256];
        size_t read_len = fread(buf, 1, sizeof(buf), in_file);
        if (read_len > 0) {
            BN_bin2bn(buf, std::min(read_len, (size_t)32), bn_a);
            if (read_len > 32) {
                BN_bin2bn(buf + 32, std::min(read_len - 32, (size_t)32), bn_p);
            }
            BN_nist_mod_224(bn_r, bn_a, bn_p, bn_ctx);
        }
    }

    // 8. Use SSL_CTX_set_srp_strength
    if (ctx) {
        // Use a value from input data for strength
        int strength = (size > 0) ? (data[0] % 3) : 0; // 0, 1, or 2
        SSL_CTX_set_srp_strength(ctx, strength);
    }

    // 9. Use DH_OpenSSL
    const DH_METHOD *dh_method = DH_OpenSSL();
    if (dh_method) {
        // Dummy use of the method
    }

    // 10. Use DES_ede3_cbc_encrypt
    // Prepare DES keys and IV from input data
    rewind(in_file);
    unsigned char des_key1[8], des_key2[8], des_key3[8], des_iv[8];
    size_t des_data_len = fread(des_key1, 1, 8, in_file);
    des_data_len += fread(des_key2, 1, 8, in_file);
    des_data_len += fread(des_key3, 1, 8, in_file);
    des_data_len += fread(des_iv, 1, 8, in_file);
    
    if (des_data_len >= 32) {
        DES_set_key_unchecked((DES_cblock *)des_key1, &ks1);
        DES_set_key_unchecked((DES_cblock *)des_key2, &ks2);
        DES_set_key_unchecked((DES_cblock *)des_key3, &ks3);
        
        // Read more data for encryption
        unsigned char input_buf[1024], output_buf[1024];
        size_t encrypt_len = fread(input_buf, 1, sizeof(input_buf), in_file);
        if (encrypt_len > 0) {
            memcpy(ivec, des_iv, 8);
            DES_ede3_cbc_encrypt(input_buf, output_buf, encrypt_len, 
                                &ks1, &ks2, &ks3, &ivec, 1); // Encrypt
        }
    }

    // 11. Use OCSP_SINGLERESP_new
    OCSP_SINGLERESP *ocsp_resp = OCSP_SINGLERESP_new();

    // 12. Create output file and write some results
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write the result of BN_nist_mod_224 if available
        if (bn_r) {
            unsigned char *bn_buf = (unsigned char *)malloc(BN_num_bytes(bn_r));
            if (bn_buf) {
                int len = BN_bn2bin(bn_r, bn_buf);
                fwrite(bn_buf, 1, len, out_file);
                free(bn_buf);
            }
        }
        assert_file_closed(&out_file);;
    }

    // 13. Clean up all allocated resources
    if (ocsp_resp) OCSP_SINGLERESP_free(ocsp_resp);
    if (attr) X509_ATTRIBUTE_free(attr);
    if (recip_stack) sk_PKCS7_RECIP_INFO_free(recip_stack);
    if (ctx) SSL_CTX_free(ctx);
    if (store) X509_STORE_free(store);
    if (param) X509_VERIFY_PARAM_free(param);
    if (bn_a) BN_free(bn_a);
    if (bn_p) BN_free(bn_p);
    if (bn_r) BN_free(bn_r);
    if (bn_ctx) BN_CTX_free(bn_ctx);
    
    // Clear DES key schedules (no explicit free function, just clear memory)
    memset(&ks1, 0, sizeof(ks1));
    memset(&ks2, 0, sizeof(ks2));
    memset(&ks3, 0, sizeof(ks3));
    memset(&ivec, 0, sizeof(ivec));
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}