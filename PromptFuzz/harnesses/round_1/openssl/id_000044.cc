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
//<ID> 514
//<Prompt> []
/*<Combination>: [
*/
//<score> 160, nr_unique_branch: 18
//<Quality> {"density":16,"unique_branches":{"bn_sqr_fixed_top":[[36,9,36,16,0,0,4,0]],"evp_get_global_properties_str":[[591,9,591,20,0,0,4,1],[595,9,595,16,0,0,4,1],[601,9,601,24,0,0,4,1],[603,9,603,69,0,0,4,1]],"PKCS5_pbkdf2_set_ex":[[204,9,204,21,0,0,4,1],[211,9,211,13,0,0,4,0],[218,9,218,18,0,0,4,1]],"ossl_lib_ctx_is_global_default":[[613,9,613,63,0,0,4,1]],"EC_POINT_set_affine_coordinates":[[904,9,904,53,0,0,4,0]],"ossl_property_list_to_string":[[707,9,707,21,0,0,4,0],[708,13,708,24,0,0,4,0],[708,13,708,24,0,0,4,1]],"bn_sqr_normal":[[122,9,122,16,0,0,4,0],[128,21,128,26,0,0,4,0]],"bn_sqr_words":[[189,13,189,21,0,0,4,1],[192,13,192,21,0,0,4,1]],"PKCS12_SAFEBAG_get0_p8inf":[[36,9,36,50,0,0,4,0]]},"library_calls":["OSSL_LIB_CTX_new","EVP_get1_default_properties","d2i_TS_REQ","TS_REQ_get_exts","SSL_CTX_new","TLS_method","SSL_CTX_get_info_callback","ossl_check_ADMISSIONS_freefunc_type","EC_GROUP_new_by_curve_name","EC_GROUP_set_curve_name","EC_POINT_new","BN_CTX_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","EC_POINT_set_affine_coordinates_GF2m","BN_free","BN_free","BN_CTX_free","EC_POINT_free","PKCS12_SAFEBAG_new","PKCS12_SAFEBAG_get0_p8inf","EVP_aes_128_cbc","PKCS5_pbe2_set","OPENSSL_sk_pop_free","ossl_check_X509_EXTENSION_sk_type","ossl_check_X509_EXTENSION_freefunc_type","TS_REQ_free","SSL_CTX_free","EC_GROUP_free","PKCS12_SAFEBAG_free","X509_ALGOR_free","CRYPTO_free","OSSL_LIB_CTX_free"],"critical_calls":["OSSL_LIB_CTX_new","EVP_get1_default_properties","d2i_TS_REQ","TS_REQ_get_exts","SSL_CTX_new","TLS_method","SSL_CTX_get_info_callback","ossl_check_ADMISSIONS_freefunc_type","EC_GROUP_new_by_curve_name","EC_GROUP_set_curve_name","EC_POINT_new","BN_CTX_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","EC_POINT_set_affine_coordinates_GF2m","BN_free","BN_free","BN_CTX_free","EC_POINT_free","PKCS12_SAFEBAG_new","PKCS12_SAFEBAG_get0_p8inf","EVP_aes_128_cbc","PKCS5_pbe2_set","OPENSSL_sk_pop_free","ossl_check_X509_EXTENSION_freefunc_type","ossl_check_X509_EXTENSION_sk_type","TS_REQ_free","SSL_CTX_free","EC_GROUP_free","PKCS12_SAFEBAG_free","X509_ALGOR_free","CRYPTO_free","OSSL_LIB_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE* from input data for potential reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for potential writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 1. Get default properties from library context
    OSSL_LIB_CTX *libctx = OSSL_LIB_CTX_new();
    char *props = EVP_get1_default_properties(libctx);
    
    // 2. Parse TS_REQ from input data and get extensions
    TS_REQ *ts_req = NULL;
    if (size > 0) {
        const unsigned char *p = data;
        ts_req = d2i_TS_REQ(NULL, &p, size);
    }
    struct stack_st_X509_EXTENSION *exts = NULL;
    if (ts_req) {
        exts = TS_REQ_get_exts(ts_req);
    }
    
    // 3. Get SSL_CTX info callback
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    void (*info_cb)(const SSL *ssl, int type, int val) = SSL_CTX_get_info_callback(ssl_ctx);
    
    // 4. ADMISSIONS stack free function (type conversion)
    OPENSSL_sk_freefunc free_admissions = ossl_check_ADMISSIONS_freefunc_type((sk_ADMISSIONS_freefunc)sk_free);
    
    // 5. Create EC group and set curve name
    EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_group) {
        EC_GROUP_set_curve_name(ec_group, NID_secp384r1);
        
        // 7. Set affine coordinates for EC point (using some data)
        EC_POINT *point = EC_POINT_new(ec_group);
        BN_CTX *bn_ctx = BN_CTX_new();
        BIGNUM *x = BN_new();
        BIGNUM *y = BN_new();
        
        // Use some data for coordinates if available
        if (size >= 32) {
            BN_bin2bn(data, 16, x);
            BN_bin2bn(data + 16, 16, y);
            EC_POINT_set_affine_coordinates_GF2m(ec_group, point, x, y, bn_ctx);
        }
        
        BN_free(x);
        BN_free(y);
        BN_CTX_free(bn_ctx);
        EC_POINT_free(point);
    }
    
    // 6. Create PKCS12_SAFEBAG and get PKCS8 private key info
    PKCS12_SAFEBAG *bag = PKCS12_SAFEBAG_new();
    const PKCS8_PRIV_KEY_INFO *p8inf = NULL;
    if (bag) {
        p8inf = PKCS12_SAFEBAG_get0_p8inf(bag);
    }
    
    // 8. Set up PBE2 encryption
    const EVP_CIPHER *cipher = EVP_aes_128_cbc();
    X509_ALGOR *pbe2 = NULL;
    if (size >= 8) {
        // Use first 8 bytes as salt if available
        pbe2 = PKCS5_pbe2_set(cipher, 1000, (unsigned char*)data, 8);
    }
    
    // Clean up
    
    // Free TS_REQ extensions if any
    if (exts) {
        sk_X509_EXTENSION_pop_free(exts, X509_EXTENSION_free);
    }
    
    // Free TS_REQ
    if (ts_req) {
        TS_REQ_free(ts_req);
    }
    
    // Free SSL_CTX
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
    }
    
    // Free EC_GROUP
    if (ec_group) {
        EC_GROUP_free(ec_group);
    }
    
    // Free PKCS12_SAFEBAG
    if (bag) {
        PKCS12_SAFEBAG_free(bag);
    }
    
    // Free PBE2
    if (pbe2) {
        X509_ALGOR_free(pbe2);
    }
    
    // Free properties string
    if (props) {
        OPENSSL_free(props);
    }
    
    // Free library context
    if (libctx) {
        OSSL_LIB_CTX_free(libctx);
    }
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}