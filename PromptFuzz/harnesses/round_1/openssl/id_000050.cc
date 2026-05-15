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
//<ID> 598
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 19
//<Quality> {"density":4,"unique_branches":{"evp_pkey_type2name":[[1026,13,1026,50,0,0,4,1]],"ERR_clear_last_mark":[[91,15,91,22,0,0,4,1]],"old_ec_priv_decode":[[374,9,374,60,0,0,4,0]],"d2i_ECPrivateKey":[[935,9,935,62,0,0,4,0],[935,9,935,62,0,0,4,1],[938,9,938,18,0,0,4,0],[939,13,939,41,0,0,4,1],[946,9,946,29,0,0,4,1],[954,9,954,27,0,0,4,0],[1015,9,1015,18,0,0,4,0]],"old_dsa_priv_decode":[[360,9,360,62,0,0,4,0]],"dsa_cb":[[28,9,28,37,0,0,4,0],[30,13,30,26,0,0,4,0],[33,16,33,45,0,0,4,0]],"EC_KEY_free":[[72,9,72,18,0,0,4,0],[84,9,84,17,0,0,4,1]],"EC_GROUP_free":[[125,9,125,15,0,0,4,0]],"ossl_d2i_PrivateKey_legacy":[[120,13,120,44,0,0,4,1],[121,16,121,50,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","OPENSSL_issetugid","OPENSSL_version_pre_release","TLS_method","SSL_CTX_new","SSL_CTX_free","CRYPTO_aligned_alloc_array","d2i_OSSL_ATTRIBUTE_MAPPING","GENERAL_NAME_new","ASN1_IA5STRING_new","ASN1_STRING_set","ADMISSION_SYNTAX_new","ADMISSION_SYNTAX_set0_admissionAuthority","OCSP_SINGLERESP_new","OCSP_SINGLERESP_get_ext_by_critical","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set1_ip_asc","X509_VERIFY_PARAM_free","SSL_CTX_use_PrivateKey_ASN1","EVP_sha3_256","OSSL_TIME_PERIOD_new","ossl_check_OSSL_TIME_PERIOD_type","OCSP_SINGLERESP_free","ADMISSION_SYNTAX_free","GENERAL_NAME_free","OSSL_ATTRIBUTE_MAPPING_free","OSSL_TIME_PERIOD_free","CRYPTO_secure_clear_free","SSL_CTX_free"],"critical_calls":["OPENSSL_init_ssl","OPENSSL_issetugid","OPENSSL_version_pre_release","TLS_method","SSL_CTX_new","CRYPTO_aligned_alloc_array","d2i_OSSL_ATTRIBUTE_MAPPING","GENERAL_NAME_new","ASN1_IA5STRING_new","ASN1_STRING_set","ADMISSION_SYNTAX_new","ADMISSION_SYNTAX_set0_admissionAuthority","OCSP_SINGLERESP_new","OCSP_SINGLERESP_get_ext_by_critical","X509_VERIFY_PARAM_new","X509_VERIFY_PARAM_set1_ip_asc","X509_VERIFY_PARAM_free","SSL_CTX_use_PrivateKey_ASN1","EVP_sha3_256","OSSL_TIME_PERIOD_new","ossl_check_OSSL_TIME_PERIOD_type","OCSP_SINGLERESP_free","ADMISSION_SYNTAX_free","OSSL_ATTRIBUTE_MAPPING_free","OSSL_TIME_PERIOD_free","CRYPTO_secure_clear_free","SSL_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize OpenSSL
    uint64_t opts = OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS;
    OPENSSL_init_ssl(opts, nullptr);

    // Check if running with elevated privileges
    int is_setugid = OPENSSL_issetugid();

    // Get OpenSSL version info
    const char *pre_release = OPENSSL_version_pre_release();

    // Create SSL context
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx) {
        return 0;
    }

    // Create file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        SSL_CTX_free(ctx);
        assert_file_closed(&in_file);
	return 0;
    }

    // Allocate aligned memory
    void *freeptr = nullptr;
    size_t align = 64;
    size_t num = 10;
    size_t elem_size = 32;
    void *aligned_mem = CRYPTO_aligned_alloc_array(num, elem_size, align, &freeptr, __FILE__, __LINE__);
    
    if (aligned_mem) {
        // Use some data to initialize memory
        size_t copy_size = size < num * elem_size ? size : num * elem_size;
        memcpy(aligned_mem, data, copy_size);
    }

    // Parse OSSL_ATTRIBUTE_MAPPING from input data
    const unsigned char *p = data;
    long remaining = static_cast<long>(size);
    OSSL_ATTRIBUTE_MAPPING *attr_mapping = d2i_OSSL_ATTRIBUTE_MAPPING(nullptr, &p, remaining);
    
    if (attr_mapping) {
        // Update remaining bytes
        remaining -= (p - data);
    }

    // Set up admission syntax structure
    ADMISSION_SYNTAX *admission_syntax = nullptr;
    GENERAL_NAME *gen_name = GENERAL_NAME_new();
    if (gen_name) {
        // Create a simple general name (DNS)
        gen_name->type = GEN_DNS;
        ASN1_IA5STRING *dns_name = ASN1_IA5STRING_new();
        if (dns_name && remaining > 0) {
            size_t dns_len = remaining > 100 ? 100 : remaining;
            ASN1_STRING_set(dns_name, p, static_cast<int>(dns_len));
            p += dns_len;
            remaining -= dns_len;
            gen_name->d.dNSName = dns_name;
        }
        
        admission_syntax = ADMISSION_SYNTAX_new();
        if (admission_syntax) {
            ADMISSION_SYNTAX_set0_admissionAuthority(admission_syntax, gen_name);
        }
    }

    // Create OCSP single response and check extensions
    OCSP_SINGLERESP *single_resp = OCSP_SINGLERESP_new();
    if (single_resp) {
        // Add some dummy extensions if we have data
        if (remaining > 0) {
            OCSP_SINGLERESP_get_ext_by_critical(single_resp, 0, -1);
        }
    }

    // Set up X509 verify parameter
    X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
    if (param) {
        // Use a sample IP address
        X509_VERIFY_PARAM_set1_ip_asc(param, "127.0.0.1");
        X509_VERIFY_PARAM_free(param);
    }

    // Try to use private key from remaining input data
    if (remaining > 0) {
        const unsigned char *key_data = p;
        long key_len = remaining;
        
        // Try different key types
        int key_types[] = {EVP_PKEY_RSA, EVP_PKEY_EC, EVP_PKEY_DSA};
        for (int type : key_types) {
            SSL_CTX_use_PrivateKey_ASN1(type, ctx, key_data, key_len);
        }
    }

    // Get SHA3-256 digest
    const EVP_MD *sha3_256 = EVP_sha3_256();

    // Create OSSL_TIME_PERIOD structure
    OSSL_TIME_PERIOD *time_period = OSSL_TIME_PERIOD_new();
    if (time_period) {
        // Use the cast function
        OSSL_TIME_PERIOD *checked = ossl_check_OSSL_TIME_PERIOD_type(time_period);
        (void)checked;  // Avoid unused variable warning
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data if we have aligned memory
        if (aligned_mem && size > 0) {
            size_t write_size = size > 100 ? 100 : size;
            fwrite(aligned_mem, 1, write_size, out_file);
        }
        assert_file_closed(&out_file);;
    }

    // Cleanup
    assert_file_closed(&in_file);;
    
    if (single_resp) {
        OCSP_SINGLERESP_free(single_resp);
    }
    
    if (admission_syntax) {
        // Note: gen_name is owned by admission_syntax after set0
        ADMISSION_SYNTAX_free(admission_syntax);
    } else if (gen_name) {
        GENERAL_NAME_free(gen_name);
    }
    
    if (attr_mapping) {
        OSSL_ATTRIBUTE_MAPPING_free(attr_mapping);
    }
    
    if (time_period) {
        OSSL_TIME_PERIOD_free(time_period);
    }
    
    // Securely clear and free aligned memory
    if (aligned_mem) {
        CRYPTO_secure_clear_free(aligned_mem, num * elem_size, __FILE__, __LINE__);
    }
    
    SSL_CTX_free(ctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}