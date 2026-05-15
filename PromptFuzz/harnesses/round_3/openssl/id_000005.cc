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
//<ID> 143
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["PEM_read_NETSCAPE_CERT_SEQUENCE","PEM_write_NETSCAPE_CERT_SEQUENCE","NETSCAPE_CERT_SEQUENCE_it","EVP_sha256","ASN1_item_digest","OPENSSL_sk_value","ossl_check_const_X509_sk_type","X509_STORE_CTX_new","X509_STORE_CTX_get_lookup_certs","SSL_CTX_new","TLS_method","SSL_CTX_set_purpose","OSSL_CMP_CTX_new","OSSL_CMP_CTX_set1_untrusted","OSSL_CMP_CTX_get0_untrusted","OCSP_BASICRESP_new","OCSP_basic_add1_cert","OCSP_resp_get0_certs","OCSP_BASICRESP_free","OSSL_CMP_CTX_free","SSL_CTX_free","X509_STORE_CTX_free","ossl_check_ASN1_TYPE_compfunc_type","OSSL_FUNC_keymgmt_load","NETSCAPE_CERT_SEQUENCE_free"],"critical_calls":["PEM_read_NETSCAPE_CERT_SEQUENCE","PEM_write_NETSCAPE_CERT_SEQUENCE","NETSCAPE_CERT_SEQUENCE_it","EVP_sha256","ASN1_item_digest","OPENSSL_sk_value","ossl_check_const_X509_sk_type","X509_STORE_CTX_new","X509_STORE_CTX_get_lookup_certs","SSL_CTX_new","TLS_method","SSL_CTX_set_purpose","OSSL_CMP_CTX_new","OSSL_CMP_CTX_set1_untrusted","OSSL_CMP_CTX_get0_untrusted","OCSP_BASICRESP_new","OCSP_basic_add1_cert","OCSP_resp_get0_certs","OCSP_BASICRESP_free","OSSL_CMP_CTX_free","SSL_CTX_free","X509_STORE_CTX_free","ossl_check_ASN1_TYPE_compfunc_type","OSSL_FUNC_keymgmt_load","NETSCAPE_CERT_SEQUENCE_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create FILE* for writing output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Read NETSCAPE_CERT_SEQUENCE from input
    NETSCAPE_CERT_SEQUENCE *seq = PEM_read_NETSCAPE_CERT_SEQUENCE(in_file, NULL, NULL, NULL);
    rewind(in_file);

    if (seq) {
        // 2. Write NETSCAPE_CERT_SEQUENCE to output file
        PEM_write_NETSCAPE_CERT_SEQUENCE(out_file, seq);
        
        // 3. Compute digest of the ASN1 item
        unsigned char md[EVP_MAX_MD_SIZE];
        unsigned int md_len = 0;
        const ASN1_ITEM *item = NETSCAPE_CERT_SEQUENCE_it();
        const EVP_MD *md_type = EVP_sha256();
        
        ASN1_item_digest(item, md_type, seq, md, &md_len);
        
        // 4. Use the certificates from the sequence
        if (seq->certs) {
            // Get the first certificate from the sequence
            X509 *cert = sk_X509_value(seq->certs, 0);
            if (cert) {
                // 5. Create X509_STORE_CTX and get lookup function
                X509_STORE_CTX *store_ctx = X509_STORE_CTX_new();
                if (store_ctx) {
                    X509_STORE_CTX_lookup_certs_fn lookup_fn = 
                        X509_STORE_CTX_get_lookup_certs(store_ctx);
                    
                    // 6. Create SSL_CTX and set purpose
                    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_method());
                    if (ssl_ctx) {
                        SSL_CTX_set_purpose(ssl_ctx, X509_PURPOSE_SSL_CLIENT);
                        
                        // 7. Create OSSL_CMP_CTX and get untrusted certs
                        OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(NULL, NULL);
                        if (cmp_ctx) {
                            // Set untrusted certs from our sequence
                            OSSL_CMP_CTX_set1_untrusted(cmp_ctx, seq->certs);
                            struct stack_st_X509 *untrusted = 
                                OSSL_CMP_CTX_get0_untrusted(cmp_ctx);
                            
                            // 8. Get OCSP certificates (create dummy OCSP response)
                            OCSP_BASICRESP *bs = OCSP_BASICRESP_new();
                            if (bs) {
                                // Add our certificate to OCSP response
                                OCSP_basic_add1_cert(bs, cert);
                                const struct stack_st_X509 *ocsp_certs = 
                                    OCSP_resp_get0_certs(bs);
                                
                                OCSP_BASICRESP_free(bs);
                            }
                            
                            OSSL_CMP_CTX_free(cmp_ctx);
                        }
                        
                        SSL_CTX_free(ssl_ctx);
                    }
                    
                    X509_STORE_CTX_free(store_ctx);
                }
            }
        }
        
        // 9. Use ossl_check_ASN1_TYPE_compfunc_type
        // Create a simple comparison function pointer
        int (*cmp_func)(const ASN1_TYPE *const *, const ASN1_TYPE *const *) = NULL;
        OPENSSL_sk_compfunc compfunc = 
            ossl_check_ASN1_TYPE_compfunc_type(cmp_func);
        
        // 10. Get keymgmt load function from dispatch table
        // Create a minimal dispatch table
        OSSL_DISPATCH dispatch_table[] = {
            {0, NULL}
        };
        OSSL_FUNC_keymgmt_load_fn *load_fn = 
            OSSL_FUNC_keymgmt_load(dispatch_table);
        
        NETSCAPE_CERT_SEQUENCE_free(seq);
    }

    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}