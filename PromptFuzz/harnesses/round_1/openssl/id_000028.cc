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
//<ID> 377
//<Prompt> []
/*<Combination>: [
*/
//<score> 69.333336, nr_unique_branch: 56
//<Quality> {"density":13,"unique_branches":{"OPENSSL_gmtime_adj":[[226,9,226,46,0,0,4,1],[235,9,235,23,0,0,4,0],[235,27,235,62,0,0,4,1],[237,9,237,23,0,0,4,1],[241,9,241,45,0,0,4,1]],"OSSL_HTTP_proxy_connect":[[1456,13,1456,28,0,0,4,0],[1456,32,1456,44,0,0,4,1]],"ASN1_TIME_adj":[[323,23,323,33,0,0,4,0],[324,13,324,60,0,0,4,1]],"X509_time_adj_ex":[[2490,9,2490,18,0,0,4,0],[2490,22,2490,64,0,0,4,1]],"x509_name_ex_i2d":[[229,9,229,20,0,0,4,0]],"OBJ_NAME_new_index":[[107,9,107,26,0,0,4,0],[109,9,109,25,0,0,4,0],[111,9,111,26,0,0,4,0]],"is_valid_time":[[74,12,74,22,0,0,4,0],[74,26,74,38,0,0,4,0],[74,42,74,54,0,0,4,0],[74,58,74,69,0,0,4,0],[74,73,74,86,0,0,4,0],[74,90,74,103,0,0,4,0]],"ossl_asn1_time_from_tm":[[267,9,267,18,0,0,4,1]],"add_ca_name":[[687,9,687,18,0,0,4,1],[689,9,689,20,0,0,4,0],[689,24,689,65,0,0,4,1],[692,9,692,65,0,0,4,1],[695,9,695,27,0,0,4,1]],"DH_size":[[184,9,184,29,0,0,4,1]],"SSL_add_client_CA":[[725,9,725,19,0,0,4,1],[32,6,32,19,2,0,4,1],[33,23,33,61,2,0,4,0]],"ASN1_item_dup":[[63,9,63,41,0,0,4,1],[63,45,63,75,0,0,4,1],[64,12,64,49,0,0,4,1]],"dh_builtin_genparams":[[168,9,168,40,0,0,4,0]],"is_valid_date":[[38,9,38,16,0,0,4,1],[38,20,38,28,0,0,4,1],[38,32,38,43,0,0,4,1],[41,5,41,11,0,0,4,1],[42,5,42,11,0,0,4,1],[43,5,43,11,0,0,4,1],[44,5,44,11,0,0,4,1],[45,5,45,11,0,0,4,1],[46,5,46,12,0,0,4,1],[47,5,47,12,0,0,4,1],[50,5,50,11,0,0,4,0],[51,5,51,11,0,0,4,1],[52,5,52,11,0,0,4,1],[53,5,53,12,0,0,4,1],[56,5,56,11,0,0,4,1],[62,5,62,12,0,0,4,1]],"posix_time_from_utc":[[99,9,99,41,0,0,4,1],[99,45,99,84,0,0,4,1],[101,9,101,19,0,0,4,1],[105,12,105,21,0,0,4,0],[107,27,107,36,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","ERR_clear_error","OBJ_NAME_new_index","DH_new","DH_generate_parameters_ex","DH_size","EC_KEY_METHOD_new","EC_KEY_METHOD_get_verify","SSL_CTX_new","TLS_method","SSL_new","X509_new","ASN1_INTEGER_set","X509_get_serialNumber","X509_gmtime_adj","X509_getm_notBefore","X509_gmtime_adj","X509_getm_notAfter","SSL_add_client_CA","SSL_client_version","X509_free","BIO_ADDR_new","BIO_ADDR_new","BIO_ADDR_copy","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","ASN1_item_pack","ASN1_OCTET_STRING_it","BIO_new","BIO_s_mem","BIO_new_fp","OSSL_HTTP_proxy_connect","ASN1_OCTET_STRING_free","ASN1_OCTET_STRING_free","BIO_ADDR_free","BIO_ADDR_free","DH_free","EC_KEY_METHOD_free","SSL_free","SSL_CTX_free","BIO_free","BIO_free"],"critical_calls":["OPENSSL_init_crypto","ERR_clear_error","OBJ_NAME_new_index","DH_new","DH_generate_parameters_ex","DH_size","EC_KEY_METHOD_new","EC_KEY_METHOD_get_verify","SSL_CTX_new","TLS_method","SSL_new","X509_new","ASN1_INTEGER_set","X509_get_serialNumber","X509_gmtime_adj","X509_getm_notBefore","X509_gmtime_adj","X509_getm_notAfter","SSL_add_client_CA","SSL_client_version","X509_free","BIO_ADDR_new","BIO_ADDR_new","BIO_ADDR_copy","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","ASN1_item_pack","ASN1_OCTET_STRING_it","BIO_new","BIO_s_mem","BIO_new_fp","OSSL_HTTP_proxy_connect","ASN1_OCTET_STRING_free","ASN1_OCTET_STRING_free","BIO_ADDR_free","BIO_ADDR_free","DH_free","EC_KEY_METHOD_free","SSL_free","SSL_CTX_free","BIO_free","BIO_free"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_clear_error();
    
    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 1: Use OBJ_NAME_new_index to create a new object name index
    unsigned long (*hash_func)(const char *) = [](const char *name) -> unsigned long {
        return 0; // Simple hash for testing
    };
    int (*cmp_func)(const char *, const char *) = [](const char *a, const char *b) -> int {
        return strcmp(a, b);
    };
    void (*free_func)(const char *, int, const char *) = [](const char *name, int type, const char *data) {};
    
    int name_index = OBJ_NAME_new_index(hash_func, cmp_func, free_func);
    
    // Step 2: Create and use DH objects
    DH *dh = DH_new();
    if (dh) {
        // Generate DH parameters (using predefined group for simplicity)
        DH_generate_parameters_ex(dh, 64, DH_GENERATOR_2, NULL);
        int dh_size = DH_size(dh);
        // Use dh_size in some way
        if (out_file) {
            fprintf(out_file, "DH size: %d\n", dh_size);
        }
    }
    
    // Step 3: Create EC_KEY_METHOD and get verify function
    EC_KEY_METHOD *ec_method = EC_KEY_METHOD_new(NULL);
    if (ec_method) {
        int (*pverify)(int, const unsigned char *, int, const unsigned char *, int, EC_KEY *) = NULL;
        int (*pverify_sig)(const unsigned char *, int, const ECDSA_SIG *, EC_KEY *) = NULL;
        EC_KEY_METHOD_get_verify(ec_method, &pverify, &pverify_sig);
    }
    
    // Step 4: SSL related operations
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = NULL;
    if (ctx) {
        ssl = SSL_new(ctx);
        if (ssl) {
            // Create a dummy X509 certificate for SSL_add_client_CA
            X509 *x509 = X509_new();
            if (x509) {
                // Set some basic fields in the certificate
                ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
                X509_gmtime_adj(X509_get_notBefore(x509), 0);
                X509_gmtime_adj(X509_get_notAfter(x509), 31536000L); // 1 year
                
                // Add client CA
                int ret = SSL_add_client_CA(ssl, x509);
                if (out_file) {
                    fprintf(out_file, "SSL_add_client_CA returned: %d\n", ret);
                }
                
                // Get client version
                int client_ver = SSL_client_version(ssl);
                if (out_file) {
                    fprintf(out_file, "SSL client version: %d\n", client_ver);
                }
                
                X509_free(x509);
            }
        }
    }
    
    // Step 5: BIO_ADDR operations
    BIO_ADDR *addr1 = BIO_ADDR_new();
    BIO_ADDR *addr2 = BIO_ADDR_new();
    if (addr1 && addr2) {
        // Try to copy address (though addresses are empty)
        int copy_result = BIO_ADDR_copy(addr2, addr1);
        if (out_file) {
            fprintf(out_file, "BIO_ADDR_copy returned: %d\n", copy_result);
        }
    }
    
    // Step 6: ASN1_item_pack operation
    // Create a simple ASN1 object to pack
    ASN1_OCTET_STRING *octet_str = ASN1_OCTET_STRING_new();
    ASN1_OCTET_STRING *packed_oct = NULL;
    if (octet_str) {
        // Use a small portion of input data for the octet string
        size_t data_len = size > 100 ? 100 : size;
        ASN1_OCTET_STRING_set(octet_str, data, data_len);
        
        // Pack the object
        ASN1_STRING *packed = ASN1_item_pack(octet_str, ASN1_OCTET_STRING_it(), 
                                            (ASN1_OCTET_STRING **)&packed_oct);
        if (out_file && packed) {
            fprintf(out_file, "ASN1_item_pack succeeded\n");
        }
    }
    
    // Step 7: OSSL_HTTP_proxy_connect (simplified - won't actually connect)
    BIO *bio = BIO_new(BIO_s_mem());
    BIO *bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);
    if (bio && bio_err) {
        // This won't actually connect but will exercise the API
        int connect_result = OSSL_HTTP_proxy_connect(
            bio, 
            "localhost", 
            "8080", 
            NULL, 
            NULL, 
            5000, 
            bio_err, 
            "fuzzer"
        );
        if (out_file) {
            fprintf(out_file, "OSSL_HTTP_proxy_connect returned: %d\n", connect_result);
        }
    }
    
    // Clean up resources
    if (octet_str) ASN1_OCTET_STRING_free(octet_str);
    if (packed_oct) ASN1_OCTET_STRING_free(packed_oct);
    if (addr1) BIO_ADDR_free(addr1);
    if (addr2) BIO_ADDR_free(addr2);
    if (dh) DH_free(dh);
    if (ec_method) EC_KEY_METHOD_free(ec_method);
    if (ssl) SSL_free(ssl);
    if (ctx) SSL_CTX_free(ctx);
    if (bio) BIO_free(bio);
    if (bio_err) BIO_free(bio_err);
    
    // Close files
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    // Clean up OpenSSL
    ERR_free_strings();
    EVP_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}