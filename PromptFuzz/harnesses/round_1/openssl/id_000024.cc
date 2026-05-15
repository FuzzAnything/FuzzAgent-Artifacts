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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 49
//<Quality> {"density":12,"unique_branches":{"ASN1_GENERALIZEDTIME_set_string":[[54,9,54,18,0,0,4,0],[54,22,54,46,0,0,4,1]],"ossl_asn1_utctime_to_tm":[[23,9,23,34,0,0,4,1]],"leap_year":[[39,9,39,24,0,0,4,0],[39,29,39,44,0,0,4,1],[39,48,39,61,0,0,4,0]],"ASN1_UTCTIME_set_string":[[39,9,39,39,0,0,4,1],[46,9,46,32,0,0,4,0],[46,9,46,32,0,0,4,1],[49,9,49,18,0,0,4,0],[49,22,49,46,0,0,4,1]],"ASN1_TIME_set_string":[[363,9,363,40,0,0,4,0],[363,9,363,40,0,0,4,1]],"ASN1_TIME_set_string_X509":[[388,9,388,29,0,0,4,1],[401,9,401,39,0,0,4,0],[401,9,401,39,0,0,4,1],[404,9,404,18,0,0,4,0],[412,13,412,31,0,0,4,0],[412,13,412,31,0,0,4,1],[412,35,412,44,0,0,4,0],[412,35,412,44,0,0,4,1]],"is_utc":[[32,9,32,19,0,0,4,1],[32,23,32,34,0,0,4,1]],"ossl_ascii_isdigit":[[265,28,265,37,1,0,4,1],[265,41,265,50,1,0,4,1]],"ossl_asn1_time_to_tm":[[90,9,90,34,0,0,4,0],[114,28,114,45,0,0,4,0],[114,49,114,63,0,0,4,0],[114,67,114,82,0,0,4,0],[118,13,118,38,0,0,4,0],[125,13,125,38,0,0,4,0],[132,14,132,41,0,0,4,0],[134,13,134,26,0,0,4,0],[134,30,134,43,0,0,4,0],[142,17,142,42,0,0,4,0],[143,31,143,37,0,0,4,0],[143,31,143,37,0,0,4,1],[152,17,152,32,0,0,4,0],[179,9,179,42,0,0,4,1],[198,9,198,24,0,0,4,1],[200,17,200,31,0,0,4,0],[200,17,200,31,0,0,4,1],[200,35,200,50,0,0,4,0],[200,35,200,50,0,0,4,1],[201,23,201,36,0,0,4,0],[201,23,201,36,0,0,4,1],[211,13,211,23,0,0,4,0],[239,9,239,15,0,0,4,1],[241,13,241,23,0,0,4,0]]},"library_calls":["ERR_load_SSL_strings","BN_CTX_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_set_word","BN_nist_mod_224","ASN1_TIME_new","ASN1_TIME_set_string_X509","SSL_CTX_new","TLS_method","SSL_new","SSL_get_rpoll_descriptor","BIO_fd_non_fatal_error","X509_new","X509_check_ip","IPAddressChoice_it","ASN1_TIME_free","BN_free","BN_free","BN_free","BN_CTX_free","SSL_free","SSL_CTX_free","X509_free"],"critical_calls":["ERR_load_SSL_strings","BN_CTX_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_nist_mod_224","ASN1_TIME_new","ASN1_TIME_set_string_X509","SSL_CTX_new","TLS_method","SSL_new","SSL_get_rpoll_descriptor","BIO_fd_non_fatal_error","X509_new","X509_check_ip","IPAddressChoice_it","ASN1_TIME_free","BN_free","BN_free","BN_free","BN_CTX_free","SSL_free","SSL_CTX_free","X509_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize OpenSSL
    ERR_load_SSL_strings();
    
    // Create FILE* from input data
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Initialize BIGNUM and BN_CTX for BN_nist_mod_224
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *r = BN_new();
    BIGNUM *a = BN_new();
    BIGNUM *p = BN_new();
    
    // Create a BIGNUM from part of input data
    size_t bn_size = size > 32 ? 32 : size;
    BN_bin2bn(data, bn_size, a);
    
    // Set p to a prime value (using part of input data)
    if (size > 64) {
        BN_bin2bn(data + 32, 32, p);
    } else {
        // Use a default prime if not enough data
        BN_set_word(p, 65537);
    }
    
    // Call BN_nist_mod_224
    BN_nist_mod_224(r, a, p, ctx);
    
    // Create ASN1_TIME and call ASN1_TIME_set_string_X509
    ASN1_TIME *time = ASN1_TIME_new();
    // Use first 20 bytes or less as string
    size_t str_len = size > 20 ? 20 : size;
    char *time_str = (char *)malloc(str_len + 1);
    if (time_str) {
        memcpy(time_str, data, str_len);
        time_str[str_len] = '\0';
        ASN1_TIME_set_string_X509(time, time_str);
        free(time_str);
    }
    
    // Create SSL context and SSL object for SSL_get_rpoll_descriptor
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = SSL_new(ssl_ctx);
    
    // Call SSL_get_rpoll_descriptor
    BIO_POLL_DESCRIPTOR desc;
    SSL_get_rpoll_descriptor(ssl, &desc);
    
    // Check for non-fatal errors on input file descriptor
    int error = ferror(in_file);
    BIO_fd_non_fatal_error(error);
    
    // Create a dummy X509 certificate for X509_check_ip
    X509 *x509 = X509_new();
    
    // Call X509_check_ip with remaining data (if any)
    if (size > 64) {
        size_t ip_len = size - 64;
        const unsigned char *ip_data = data + 64;
        X509_check_ip(x509, ip_data, ip_len > 16 ? 16 : ip_len, 0);
    }
    
    // Get IPAddressChoice ASN1 item
    const ASN1_ITEM *item = IPAddressChoice_it();
    
    // Cleanup
    if (time) ASN1_TIME_free(time);
    if (r) BN_free(r);
    if (a) BN_free(a);
    if (p) BN_free(p);
    if (ctx) BN_CTX_free(ctx);
    if (ssl) SSL_free(ssl);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    if (x509) X509_free(x509);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}