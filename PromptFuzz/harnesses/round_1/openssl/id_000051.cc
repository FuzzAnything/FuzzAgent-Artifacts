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
//<ID> 599
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["OPENSSL_init_ssl","OPENSSL_issetugid","TLS_method","SSL_CTX_new","SSL_CTX_use_PrivateKey_ASN1","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set1_ip_asc","X509_VERIFY_PARAM_free","d2i_ADMISSION_SYNTAX","GENERAL_NAME_new","ADMISSION_SYNTAX_set0_admissionAuthority","ADMISSION_SYNTAX_free","d2i_OSSL_ATTRIBUTE_MAPPING","OSSL_ATTRIBUTE_MAPPING_free","OPENSSL_version_pre_release","OCSP_SINGLERESP_new","OCSP_SINGLERESP_get_ext_by_critical","OCSP_SINGLERESP_free","EVP_sha3_256","ossl_check_OSSL_TIME_PERIOD_type","CRYPTO_aligned_alloc_array","CRYPTO_secure_clear_free","SSL_CTX_free"],"critical_calls":["OPENSSL_init_ssl","OPENSSL_issetugid","TLS_method","SSL_CTX_new","SSL_CTX_use_PrivateKey_ASN1","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set1_ip_asc","X509_VERIFY_PARAM_free","d2i_ADMISSION_SYNTAX","GENERAL_NAME_new","ADMISSION_SYNTAX_set0_admissionAuthority","ADMISSION_SYNTAX_free","d2i_OSSL_ATTRIBUTE_MAPPING","OSSL_ATTRIBUTE_MAPPING_free","OPENSSL_version_pre_release","OCSP_SINGLERESP_new","OCSP_SINGLERESP_get_ext_by_critical","OCSP_SINGLERESP_free","EVP_sha3_256","ossl_check_OSSL_TIME_PERIOD_type","CRYPTO_aligned_alloc_array","CRYPTO_secure_clear_free","SSL_CTX_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // 1. Initialize OpenSSL SSL library
    OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, nullptr);

    // 2. Check if running with elevated privileges
    OPENSSL_issetugid();

    // 3. Create SSL context with TLS method
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx) {
	return 0;
	}
	

    // 4. Try to use input as private key (first 512 bytes)
    size_t key_len = size > 512 ? 512 : size;
    SSL_CTX_use_PrivateKey_ASN1(EVP_PKEY_RSA, ctx, data, static_cast<long>(key_len));

    // 5. Create X509 verification parameter and set IP address
    X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
    if (param) {
        // Use portion of input as IP string (max 64 bytes)
        size_t ip_len = size > 64 ? 64 : size;
        char ip_str[65] = {0};
        memcpy(ip_str, data, ip_len);
        X509_VERIFY_PARAM_set1_ip_asc(param, ip_str);
        X509_VERIFY_PARAM_free(param);
    }

    // 6. Parse ADMISSION_SYNTAX from input
    const unsigned char *p = data;
    long remaining = static_cast<long>(size);
    ADMISSION_SYNTAX *as = d2i_ADMISSION_SYNTAX(nullptr, &p, remaining);
    if (as) {
        // Create minimal GENERAL_NAME for admission authority
        GENERAL_NAME *gn = GENERAL_NAME_new();
        if (gn) {
            gn->type = GEN_OTHERNAME;
            ADMISSION_SYNTAX_set0_admissionAuthority(as, gn);
            // Note: gn is now owned by as, don't free separately
        }
        ADMISSION_SYNTAX_free(as);
    }

    // 7. Parse OSSL_ATTRIBUTE_MAPPING from input
    p = data;
    remaining = static_cast<long>(size);
    OSSL_ATTRIBUTE_MAPPING *attr = d2i_OSSL_ATTRIBUTE_MAPPING(nullptr, &p, remaining);
    if (attr) {
        OSSL_ATTRIBUTE_MAPPING_free(attr);
    }

    // 8. Get OpenSSL version info
    const char *version = OPENSSL_version_pre_release();

    // 9. Create OCSP single response and check extensions
    OCSP_SINGLERESP *sr = OCSP_SINGLERESP_new();
    if (sr) {
        // Add dummy extension for testing
        OCSP_SINGLERESP_get_ext_by_critical(sr, 0, -1);
        OCSP_SINGLERESP_free(sr);
    }

    // 10. Get SHA3-256 digest
    const EVP_MD *sha3_256 = EVP_sha3_256();

    // 11. Use type checking function
    OSSL_TIME_PERIOD dummy_period;
    OSSL_TIME_PERIOD *checked = ossl_check_OSSL_TIME_PERIOD_type(&dummy_period);

    // 12. Allocate aligned memory using portion of input for size
    void *freeptr = nullptr;
    size_t alloc_size = size % 1024 + 1;  // Ensure non-zero
    void *aligned_mem = CRYPTO_aligned_alloc_array(
        1, alloc_size, 64, &freeptr, __FILE__, __LINE__);
    
    if (aligned_mem) {
        // 13. Securely clear and free allocated memory
        CRYPTO_secure_clear_free(aligned_mem, alloc_size, __FILE__, __LINE__);
    }

    // 14. Create FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        
        // Read some data from the file
        char buf[128];
        fread(buf, 1, sizeof(buf), in_file);
        
        assert_file_closed(&in_file);;
    }

    // 15. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write processed data
        if (size > 0) {
            fwrite(data, 1, size > 1024 ? 1024 : size, out_file);
        }
        
        // Get file descriptor
        int fd_out = fuzz_fileno(out_file);
        
        assert_file_closed(&out_file);;
    }

    // 16. Clean up SSL context
    SSL_CTX_free(ctx);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}