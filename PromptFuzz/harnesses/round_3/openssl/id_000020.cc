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
//<ID> 334
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 0
//<Quality> {"density":28,"unique_branches":{},"library_calls":["PEM_read_NETSCAPE_CERT_SEQUENCE","PEM_write_NETSCAPE_CERT_SEQUENCE","NETSCAPE_CERT_SEQUENCE_it","EVP_sha256","ASN1_item_digest","TLS_method","SSL_CTX_new","SSL_CTX_set_purpose","SSL_new","SSL_rstate_string","SSL_free","OSSL_CMP_CTX_new","ossl_check_const_X509_sk_type","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OPENSSL_sk_num","ossl_check_const_X509_sk_type","OPENSSL_sk_value","OPENSSL_sk_push","ossl_check_X509_sk_type","ossl_check_X509_type","X509_dup","OSSL_CMP_CTX_set1_untrusted","OPENSSL_sk_pop_free","ossl_check_X509_sk_type","ossl_check_X509_freefunc_type","OSSL_CMP_CTX_get0_untrusted","OCSP_BASICRESP_new","OPENSSL_sk_num","ossl_check_const_X509_sk_type","OPENSSL_sk_value","ossl_check_const_X509_sk_type","OCSP_basic_add1_cert","OCSP_resp_get0_certs","TS_VERIFY_CTX_new","X509_STORE_new","TS_VERIFY_CTX_set0_store","X509_STORE_CTX_new","X509_STORE_CTX_get_lookup_certs","EC_KEY_new_by_curve_name","EC_KEY_generate_key","i2o_ECPublicKey","CRYPTO_free","ossl_check_ASN1_TYPE_compfunc_type","OSSL_FUNC_keymgmt_load","NETSCAPE_CERT_SEQUENCE_free","OCSP_BASICRESP_free","OSSL_CMP_CTX_free","SSL_CTX_free","X509_STORE_CTX_free","TS_VERIFY_CTX_free","X509_STORE_free","EC_KEY_free"],"critical_calls":["PEM_read_NETSCAPE_CERT_SEQUENCE","PEM_write_NETSCAPE_CERT_SEQUENCE","NETSCAPE_CERT_SEQUENCE_it","EVP_sha256","ASN1_item_digest","TLS_method","SSL_CTX_new","SSL_CTX_set_purpose","SSL_new","SSL_rstate_string","SSL_free","OSSL_CMP_CTX_new","ossl_check_const_X509_sk_type","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OPENSSL_sk_num","ossl_check_const_X509_sk_type","OPENSSL_sk_value","OPENSSL_sk_push","ossl_check_X509_type","X509_dup","ossl_check_X509_sk_type","OSSL_CMP_CTX_set1_untrusted","OPENSSL_sk_pop_free","ossl_check_X509_freefunc_type","ossl_check_X509_sk_type","OSSL_CMP_CTX_get0_untrusted","OCSP_BASICRESP_new","OPENSSL_sk_num","ossl_check_const_X509_sk_type","OPENSSL_sk_value","ossl_check_const_X509_sk_type","OCSP_basic_add1_cert","OCSP_resp_get0_certs","TS_VERIFY_CTX_new","X509_STORE_new","TS_VERIFY_CTX_set0_store","X509_STORE_CTX_new","X509_STORE_CTX_get_lookup_certs","EC_KEY_new_by_curve_name","EC_KEY_generate_key","i2o_ECPublicKey","CRYPTO_free","ossl_check_ASN1_TYPE_compfunc_type","OSSL_FUNC_keymgmt_load","NETSCAPE_CERT_SEQUENCE_free","OCSP_BASICRESP_free","OSSL_CMP_CTX_free","SSL_CTX_free","X509_STORE_CTX_free","TS_VERIFY_CTX_free","EC_KEY_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE pointer for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Read NETSCAPE_CERT_SEQUENCE from input file
    NETSCAPE_CERT_SEQUENCE *seq = NULL;
    seq = PEM_read_NETSCAPE_CERT_SEQUENCE(in_file, &seq, NULL, NULL);
    
    // 2. Write NETSCAPE_CERT_SEQUENCE to output file
    if (seq) {
        PEM_write_NETSCAPE_CERT_SEQUENCE(out_file, seq);
    }

    // 3. Get ASN1_ITEM for NETSCAPE_CERT_SEQUENCE
    const ASN1_ITEM *item = NETSCAPE_CERT_SEQUENCE_it();

    // 4. Compute digest of NETSCAPE_CERT_SEQUENCE using SHA256
    unsigned char md[EVP_MAX_MD_SIZE];
    unsigned int md_len = 0;
    const EVP_MD *sha256 = EVP_sha256();
    if (seq && item && sha256) {
        ASN1_item_digest(item, sha256, (void *)seq, md, &md_len);
    }

    // 5. Create SSL context and get SSL rstate string
    const SSL_METHOD *meth = TLS_method();
    SSL_CTX *ssl_ctx = SSL_CTX_new(meth);
    if (ssl_ctx) {
        SSL_CTX_set_purpose(ssl_ctx, X509_PURPOSE_SSL_SERVER);
        SSL *ssl = SSL_new(ssl_ctx);
        if (ssl) {
            const char *rstate = SSL_rstate_string(ssl);
            (void)rstate; // Use variable to avoid unused warning
            SSL_free(ssl);
        }
    }

    // 6. Create OSSL_CMP_CTX and handle untrusted certificates
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(NULL, NULL);
    if (cmp_ctx && seq && seq->certs) {
        // Get certificates from NETSCAPE_CERT_SEQUENCE
        const OPENSSL_STACK *certs_stack = ossl_check_const_X509_sk_type(seq->certs);
        if (certs_stack) {
            // Convert to stack_st_X509 for OSSL_CMP_CTX
            struct stack_st_X509 *untrusted = sk_X509_new_null();
            if (untrusted) {
                for (int i = 0; i < sk_X509_num(seq->certs); i++) {
                    X509 *cert = (X509 *)OPENSSL_sk_value(certs_stack, i);
                    if (cert) {
                        sk_X509_push(untrusted, X509_dup(cert));
                    }
                }
                OSSL_CMP_CTX_set1_untrusted(cmp_ctx, untrusted);
                sk_X509_pop_free(untrusted, X509_free);
            }
        }
        // Get untrusted certificates back
        struct stack_st_X509 *retrieved = OSSL_CMP_CTX_get0_untrusted(cmp_ctx);
        (void)retrieved; // Use variable
    }

    // 7. Create and populate OCSP_BASICRESP
    OCSP_BASICRESP *ocsp_resp = OCSP_BASICRESP_new();
    if (ocsp_resp && seq && seq->certs && sk_X509_num(seq->certs) > 0) {
        // Add first certificate from sequence to OCSP response
        X509 *first_cert = sk_X509_value(seq->certs, 0);
        if (first_cert) {
            OCSP_basic_add1_cert(ocsp_resp, first_cert);
        }
        // Get certificates from OCSP response
        const struct stack_st_X509 *ocsp_certs = OCSP_resp_get0_certs(ocsp_resp);
        (void)ocsp_certs; // Use variable
    }

    // 8. Create TS_VERIFY_CTX and X509_STORE_CTX
    TS_VERIFY_CTX *ts_ctx = TS_VERIFY_CTX_new();
    X509_STORE *store = X509_STORE_new();
    if (ts_ctx && store) {
        TS_VERIFY_CTX_set0_store(ts_ctx, store);
    }
    
    X509_STORE_CTX *store_ctx = X509_STORE_CTX_new();
    if (store_ctx) {
        // Get lookup function
        X509_STORE_CTX_lookup_certs_fn lookup = X509_STORE_CTX_get_lookup_certs(store_ctx);
        (void)lookup; // Use variable
    }

    // 9. Create EC_KEY and convert to octet string
    EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_key) {
        // Generate key (will fail without proper parameters, but we just need the function call)
        EC_KEY_generate_key(ec_key);
        unsigned char *out = NULL;
        int out_len = i2o_ECPublicKey(ec_key, &out);
        if (out) {
            OPENSSL_free(out);
        }
    }

    // 10. Use other required functions
    // ossl_check_ASN1_TYPE_compfunc_type
    OPENSSL_sk_compfunc compfunc = ossl_check_ASN1_TYPE_compfunc_type(NULL);
    (void)compfunc; // Use variable

    // OSSL_FUNC_keymgmt_load (requires OSSL_DISPATCH, create dummy)
    OSSL_DISPATCH dummy_dispatch[] = {{0, NULL}};
    OSSL_FUNC_keymgmt_load_fn *load_fn = OSSL_FUNC_keymgmt_load(dummy_dispatch);
    (void)load_fn; // Use variable

    // Clean up all allocated resources
    if (seq) {
        NETSCAPE_CERT_SEQUENCE_free(seq);
    }
    if (ocsp_resp) {
        OCSP_BASICRESP_free(ocsp_resp);
    }
    if (cmp_ctx) {
        OSSL_CMP_CTX_free(cmp_ctx);
    }
    if (ssl_ctx) {
        SSL_CTX_free(ssl_ctx);
    }
    if (store_ctx) {
        X509_STORE_CTX_free(store_ctx);
    }
    if (ts_ctx) {
        if (store) {
            // store is now owned by ts_ctx, don't free separately
        }
        TS_VERIFY_CTX_free(ts_ctx);
    } else if (store) {
        X509_STORE_free(store);
    }
    if (ec_key) {
        EC_KEY_free(ec_key);
    }

    // Close file pointers
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}