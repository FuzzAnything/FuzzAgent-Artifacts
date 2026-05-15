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
//<ID> 562
//<Prompt> []
/*<Combination>: [
*/
//<score> 143, nr_unique_branch: 51
//<Quality> {"density":11,"unique_branches":{"bio_write_intern":[[354,9,354,18,0,0,4,0]],"ossl_asn1_time_from_tm":[[267,9,267,18,0,0,4,0]],"X509_time_adj_ex":[[2490,9,2490,18,0,0,4,1]],"dh_builtin_genparams":[[168,9,168,40,0,0,4,1],[174,9,174,20,0,0,4,1],[179,9,179,19,0,0,4,1],[183,9,183,30,0,0,4,0],[183,34,183,70,0,0,4,1],[185,9,185,30,0,0,4,0],[185,34,185,70,0,0,4,1],[188,9,188,23,0,0,4,1],[192,9,192,36,0,0,4,0],[193,13,193,33,0,0,4,1],[195,13,195,33,0,0,4,1],[217,9,217,77,0,0,4,0],[230,9,230,17,0,0,4,0]],"BN_generate_prime_ex2":[[137,16,137,27,0,0,4,1],[157,9,157,20,0,0,4,1],[161,13,161,69,0,0,4,1]],"OBJ_NAME_new_index":[[107,9,107,26,0,0,4,1],[109,9,109,25,0,0,4,1],[111,9,111,26,0,0,4,1]],"probable_prime_dh":[[556,9,556,39,0,0,4,1],[559,9,559,47,0,0,4,1],[563,9,563,76,0,0,4,1],[568,9,568,16,0,0,4,1],[570,9,570,30,0,0,4,1],[572,9,572,20,0,0,4,1],[576,13,576,35,0,0,4,1],[580,9,580,32,0,0,4,1],[581,12,581,47,0,0,4,1],[581,32,581,36,0,0,4,0],[587,17,587,36,0,0,4,0],[587,17,587,36,0,0,4,1],[589,13,589,32,0,0,4,1],[595,17,595,36,0,0,4,0],[595,17,595,36,0,0,4,1],[597,13,597,23,0,0,4,1],[601,13,601,17,0,0,4,0],[601,13,602,54,0,0,4,0],[601,13,602,54,0,0,4,1],[604,17,604,33,0,0,4,1],[609,9,609,33,0,0,4,1]],"BN_get_word":[[406,9,406,19,0,0,4,0]],"DH_size":[[184,9,184,29,0,0,4,0]],"calc_trial_divisions":[[76,9,76,20,0,0,4,1],[78,14,78,26,0,0,4,0]],"OSSL_HTTP_proxy_connect":[[1456,13,1456,28,0,0,4,1],[1533,13,1533,46,0,0,4,0]],"bnrand":[[50,9,50,23,0,0,4,0],[75,13,75,16,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","ERR_clear_error","OBJ_NAME_new_index","DH_new","BN_GENCB_new","DH_generate_parameters_ex","BN_GENCB_free","DH_size","EC_KEY_METHOD_new","EC_KEY_METHOD_get_verify","TLS_method","SSL_CTX_new","SSL_new","SSL_client_version","SSL_free","SSL_CTX_free","X509_new","X509_get_serialNumber","ASN1_INTEGER_set","X509_gmtime_adj","ASN1_STRING_free","X509_free","EVP_aes_192_ctr","UI_new","UI_free","BIO_ADDR_new","BIO_ADDR_new","BIO_ADDR_copy","BIO_ADDR_free","BIO_ADDR_free","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","ASN1_item_pack","ASN1_OCTET_STRING_it","ASN1_OCTET_STRING_free","ASN1_OCTET_STRING_free","BIO_s_mem","BIO_new","BIO_write","BIO_new_fp","OSSL_HTTP_proxy_connect","BIO_free","BIO_free","DH_free","EC_KEY_METHOD_free"],"critical_calls":["OPENSSL_init_crypto","ERR_clear_error","OBJ_NAME_new_index","DH_new","BN_GENCB_new","DH_generate_parameters_ex","BN_GENCB_free","DH_size","EC_KEY_METHOD_new","EC_KEY_METHOD_get_verify","TLS_method","SSL_CTX_new","SSL_new","SSL_client_version","SSL_free","SSL_CTX_free","X509_new","X509_get_serialNumber","ASN1_INTEGER_set","X509_gmtime_adj","ASN1_STRING_free","X509_free","EVP_aes_192_ctr","UI_new","UI_free","BIO_ADDR_new","BIO_ADDR_new","BIO_ADDR_copy","BIO_ADDR_free","BIO_ADDR_free","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","ASN1_item_pack","ASN1_OCTET_STRING_it","ASN1_OCTET_STRING_free","ASN1_OCTET_STRING_free","BIO_s_mem","BIO_new","BIO_write","BIO_new_fp","OSSL_HTTP_proxy_connect","BIO_free","BIO_free","DH_free","EC_KEY_METHOD_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | OPENSSL_INIT_ADD_ALL_CIPHERS | OPENSSL_INIT_ADD_ALL_DIGESTS, nullptr);
    ERR_clear_error();

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create FILE* for output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. OBJ_NAME index creation
    OBJ_NAME_new_index(nullptr, nullptr, nullptr);

    // 2. DH operations
    DH *dh = DH_new();
    if (dh) {
        BN_GENCB *cb = BN_GENCB_new();
        if (cb) {
            // Use a small prime length for fuzzing efficiency
            int prime_len = (size > 0) ? (data[0] % 512 + 512) : 512;
            DH_generate_parameters_ex(dh, prime_len, 2, cb);
            BN_GENCB_free(cb);
        }
        // Get DH size
        DH_size(dh);
    }

    // 3. EC_KEY_METHOD operations
    EC_KEY_METHOD *ec_method = EC_KEY_METHOD_new(nullptr);
    if (ec_method) {
        int (*verify_func)(int, const unsigned char *, int, const unsigned char *, int, EC_KEY *) = nullptr;
        int (*verify_sig_func)(const unsigned char *, int, const ECDSA_SIG *, EC_KEY *) = nullptr;
        EC_KEY_METHOD_get_verify(ec_method, &verify_func, &verify_sig_func);
    }

    // 4. SSL/TLS operations
    const SSL_METHOD *meth = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(meth);
    if (ctx) {
        SSL *ssl = SSL_new(ctx);
        if (ssl) {
            // Get client version
            SSL_client_version(ssl);
            SSL_free(ssl);
        }
        SSL_CTX_free(ctx);
    }

    // 5. X509 certificate operations
    X509 *x509 = X509_new();
    if (x509) {
        // Set serial number using data
        ASN1_INTEGER *serial = X509_get_serialNumber(x509);
        if (serial && size > 0) {
            long serial_value = (data[0] << 8) | (size > 1 ? data[1] : 0);
            ASN1_INTEGER_set(serial, serial_value);
        }
        
        // Adjust time
        ASN1_TIME *time = X509_gmtime_adj(nullptr, 0);
        if (time) {
            ASN1_STRING_free(time);
        }
        
        X509_free(x509);
    }

    // 6. AES cipher reference
    const EVP_CIPHER *cipher = EVP_aes_192_ctr();
    (void)cipher; // Prevent unused variable warning

    // 7. UI operations
    UI *ui = UI_new();
    if (ui) {
        UI_free(ui);
    }

    // 8. BIO_ADDR operations
    BIO_ADDR *addr = BIO_ADDR_new();
    if (addr) {
        BIO_ADDR *addr_copy = BIO_ADDR_new();
        if (addr_copy) {
            BIO_ADDR_copy(addr_copy, addr);
            BIO_ADDR_free(addr_copy);
        }
        BIO_ADDR_free(addr);
    }

    // 9. ASN1_OCTET_STRING operations using input data
    ASN1_OCTET_STRING *octet_str = ASN1_OCTET_STRING_new();
    if (octet_str && size > 0) {
        // Limit the length to what ASN1_OCTET_STRING_set can handle
        int copy_len = (size < INT_MAX) ? (int)size : 1000;
        ASN1_OCTET_STRING_set(octet_str, data, copy_len);
        
        // Pack into ASN1 item
        ASN1_OCTET_STRING *packed = nullptr;
        ASN1_item_pack(octet_str, ASN1_OCTET_STRING_it(), &packed);
        if (packed) {
            ASN1_OCTET_STRING_free(packed);
        }
        
        ASN1_OCTET_STRING_free(octet_str);
    }

    // 10. BIO operations
    const BIO_METHOD *mem_method = BIO_s_mem();
    BIO *mem_bio = BIO_new(mem_method);
    if (mem_bio) {
        // Write some data to memory BIO
        if (size > 0) {
            BIO_write(mem_bio, data, (int)(size > 1024 ? 1024 : size));
        }
        
        // Create file BIO
        BIO *file_bio = BIO_new_fp(out_file, BIO_NOCLOSE);
        if (file_bio) {
            // Try HTTP proxy connect (will likely fail but we call the API)
            OSSL_HTTP_proxy_connect(mem_bio, "localhost", "8080", 
                                   nullptr, nullptr, 5000, nullptr, "fuzzer");
            BIO_free(file_bio);
        }
        BIO_free(mem_bio);
    }

    // 11. Cleanup remaining resources
    if (dh) {
        DH_free(dh);
    }
    if (ec_method) {
        EC_KEY_METHOD_free(ec_method);
    }

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}