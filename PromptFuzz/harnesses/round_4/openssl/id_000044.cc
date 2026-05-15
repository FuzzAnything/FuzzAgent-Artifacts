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
//<ID> 542
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 20
//<Quality> {"density":5,"unique_branches":{"OCSP_resp_get0_id":[[179,9,179,40,0,0,4,1],[182,16,182,46,0,0,4,1]],"RC5_32_set_key":[[41,9,41,11,0,0,4,0],[54,9,54,16,0,0,4,1],[24,17,24,18,7,0,4,1],[25,9,25,15,7,0,4,1],[28,9,28,15,7,0,4,0],[28,9,28,15,7,0,4,1],[31,9,31,15,7,0,4,0],[31,9,31,15,7,0,4,1],[34,9,34,15,7,0,4,0],[34,9,34,15,7,0,4,1],[37,9,37,15,7,0,4,0],[37,9,37,15,7,0,4,1],[40,9,40,15,7,0,4,0],[40,9,40,15,7,0,4,1],[43,9,43,15,7,0,4,0],[43,9,43,15,7,0,4,1],[46,9,46,15,7,0,4,0],[46,9,46,15,7,0,4,1]]},"library_calls":["OSSL_CRMF_CERTTEMPLATE_new","OSSL_CRMF_CERTTEMPLATE_free","RC5_32_set_key","OCSP_BASICRESP_new","OCSP_resp_get0_id","IPAddressOrRange_new","ossl_check_IPAddressOrRange_type","EC_GROUP_new_by_curve_name","BN_new","BN_CTX_new","EC_GROUP_get_cofactor","TS_MSG_IMPRINT_new","X509_ALGOR_new","OBJ_nid2obj","X509_ALGOR_set0","TS_MSG_IMPRINT_set_algo","SSL_CTX_new","TLS_method","SSL_CTX_set_security_callback","UI_new","UI_construct_prompt","CRYPTO_free","UI_free","SSL_CTX_free","X509_ALGOR_free","TS_MSG_IMPRINT_free","BN_free","BN_CTX_free","EC_GROUP_free","IPAddressOrRange_free","OCSP_BASICRESP_free"],"critical_calls":["OSSL_CRMF_CERTTEMPLATE_new","OSSL_CRMF_CERTTEMPLATE_free","RC5_32_set_key","OCSP_BASICRESP_new","OCSP_resp_get0_id","IPAddressOrRange_new","ossl_check_IPAddressOrRange_type","EC_GROUP_new_by_curve_name","BN_new","BN_CTX_new","EC_GROUP_get_cofactor","TS_MSG_IMPRINT_new","X509_ALGOR_new","OBJ_nid2obj","X509_ALGOR_set0","TS_MSG_IMPRINT_set_algo","SSL_CTX_new","TLS_method","SSL_CTX_set_security_callback","UI_new","UI_construct_prompt","CRYPTO_free","UI_free","SSL_CTX_free","X509_ALGOR_free","TS_MSG_IMPRINT_free","BN_free","BN_CTX_free","EC_GROUP_free","IPAddressOrRange_free","OCSP_BASICRESP_free"],"visited":0}
/**/


// Security callback function
static int  id_000044_security_callback(const SSL *s, const SSL_CTX *ctx, int op, int bits,
                             int nid, void *other, void *ex) {
    // Simple callback that just returns 1 (success)
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize variables
    OSSL_CRMF_CERTTEMPLATE *crmf_template = NULL;
    RC5_32_KEY rc5_key;
    OCSP_BASICRESP *ocsp_resp = NULL;
    IPAddressOrRange *ip_addr_or_range = NULL;
    EC_GROUP *ec_group = NULL;
    TS_MSG_IMPRINT *ts_imprint = NULL;
    SSL_CTX *ssl_ctx = NULL;
    UI *ui = NULL;
    BIGNUM *cofactor = NULL;
    BN_CTX *bn_ctx = NULL;
    X509_ALGOR *algor = NULL;
    const ASN1_OCTET_STRING *ocsp_id = NULL;
    const X509_NAME *ocsp_name = NULL;
    char *prompt = NULL;
    
    // 1. Create and free OSSL_CRMF_CERTTEMPLATE
    crmf_template = OSSL_CRMF_CERTTEMPLATE_new();
    if (crmf_template) {
        OSSL_CRMF_CERTTEMPLATE_free(crmf_template);
    }
    
    // 2. Set RC5 key using input data
    // Ensure we have enough data for RC5 key (at least 1 byte)
    if (size > 0) {
        int key_len = (size > 255) ? 255 : (int)size;
        int rounds = 12; // Default RC5 rounds
        RC5_32_set_key(&rc5_key, key_len, data, rounds);
    }
    
    // 3. Create OCSP_BASICRESP and get responder ID
    ocsp_resp = OCSP_BASICRESP_new();
    if (ocsp_resp) {
        OCSP_resp_get0_id(ocsp_resp, &ocsp_id, &ocsp_name);
    }
    
    // 4. Create IPAddressOrRange and check type
    ip_addr_or_range = IPAddressOrRange_new();
    if (ip_addr_or_range) {
        IPAddressOrRange *checked = ossl_check_IPAddressOrRange_type(ip_addr_or_range);
        // Use the checked pointer
        (void)checked;
    }
    
    // 5. Create EC group and get cofactor
    ec_group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_group) {
        cofactor = BN_new();
        bn_ctx = BN_CTX_new();
        if (cofactor && bn_ctx) {
            EC_GROUP_get_cofactor(ec_group, cofactor, bn_ctx);
        }
    }
    
    // 6. Create TS_MSG_IMPRINT and set algorithm
    ts_imprint = TS_MSG_IMPRINT_new();
    if (ts_imprint) {
        algor = X509_ALGOR_new();
        if (algor) {
            // Set algorithm to SHA256
            ASN1_OBJECT *obj = OBJ_nid2obj(NID_sha256);
            X509_ALGOR_set0(algor, obj, V_ASN1_NULL, NULL);
            TS_MSG_IMPRINT_set_algo(ts_imprint, algor);
        }
    }
    
    // 7. Create SSL_CTX and set security callback
    ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx) {
        SSL_CTX_set_security_callback(ssl_ctx,  id_000044_security_callback);
    }
    
    // 8. Create UI and construct prompt
    ui = UI_new();
    if (ui) {
        prompt = UI_construct_prompt(ui, "Enter password", "fuzzer object");
        // Use the prompt
        (void)prompt;
    }
    
    // Create a FILE* from input data for additional processing
    if (size > 0) {
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            // Read some data from the file
            unsigned char buffer[256];
            size_t read_bytes = fread(buffer, 1, sizeof(buffer) < size ? sizeof(buffer) : size, in_file);
            (void)read_bytes; // Use the data
            assert_file_closed(&in_file);;
        }
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data to output file
        const char *test_data = "Fuzzer output";
        fwrite(test_data, 1, strlen(test_data), out_file);
        assert_file_closed(&out_file);;
    }
    
    // Clean up all allocated resources
    if (prompt) OPENSSL_free(prompt);
    if (ui) UI_free(ui);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);
    if (algor) X509_ALGOR_free(algor);
    if (ts_imprint) TS_MSG_IMPRINT_free(ts_imprint);
    if (cofactor) BN_free(cofactor);
    if (bn_ctx) BN_CTX_free(bn_ctx);
    if (ec_group) EC_GROUP_free(ec_group);
    if (ip_addr_or_range) IPAddressOrRange_free(ip_addr_or_range);
    if (ocsp_resp) OCSP_BASICRESP_free(ocsp_resp);
    
    assert_file_closed(&out_file);
	return 0;
}