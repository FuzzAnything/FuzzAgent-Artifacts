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
//<ID> 167
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 4
//<Quality> {"density":9,"unique_branches":{"EVP_DigestUpdate":[[274,9,274,28,0,0,4,0]],"EVP_DigestFinal_ex":[[23,26,23,52,1,0,4,0]],"ri_cb":[[182,9,182,39,0,0,4,0],[182,9,182,39,0,0,4,1]]},"library_calls":["PKCS7_ENVELOPE_it","ASN1_item_d2i_fp","OSSL_trace_get_category_name","OSSL_FUNC_kem_decapsulate_init","OSSL_CMP_CTX_new","OSSL_CMP_CTX_set_http_cb","d2i_OCSP_BASICRESP","OCSP_resp_get0_id","ASN1_STRING_length","EVP_whirlpool","EVP_MD_CTX_new","EVP_DigestInit_ex","EVP_DigestUpdate","EVP_DigestFinal_ex","EVP_MD_CTX_free","ASN1_item_i2d_fp","ASN1_item_free","OCSP_BASICRESP_free","OSSL_CMP_CTX_free"],"critical_calls":["PKCS7_ENVELOPE_it","ASN1_item_d2i_fp","OSSL_trace_get_category_name","OSSL_FUNC_kem_decapsulate_init","OSSL_CMP_CTX_new","OSSL_CMP_CTX_set_http_cb","d2i_OCSP_BASICRESP","OCSP_resp_get0_id","ASN1_STRING_length","EVP_whirlpool","EVP_MD_CTX_new","EVP_DigestInit_ex","EVP_DigestUpdate","EVP_DigestFinal_ex","EVP_MD_CTX_free","ASN1_item_i2d_fp","ASN1_item_free","OCSP_BASICRESP_free","OSSL_CMP_CTX_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Open input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Get PKCS7 envelope ASN.1 item
    const ASN1_ITEM *envelope_item = PKCS7_ENVELOPE_it();
    if (!envelope_item) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Use input data to parse PKCS7 envelope
    PKCS7_ENVELOPE *env = NULL;
    env = (PKCS7_ENVELOPE *)ASN1_item_d2i_fp(envelope_item, in_file, NULL);
    assert_file_closed(&in_file);;
    
    // 3. Get trace category name
    const char *trace_name = OSSL_trace_get_category_name(0);  // Try category 0
    if (trace_name) {
        // Could use trace_name if needed
    }

    // 4. Create a mock OSSL_DISPATCH table for KEM decapsulation
    // We'll create a minimal dispatch structure
    OSSL_DISPATCH dispatch_table[2] = {{0, NULL}};
    OSSL_FUNC_kem_decapsulate_init_fn *kem_decaps_init = 
        OSSL_FUNC_kem_decapsulate_init(dispatch_table);
    
    // 5. Create and setup OSSL_CMP context with HTTP callback
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(NULL, NULL);
    if (cmp_ctx) {
        // Define a simple HTTP BIO callback
        OSSL_HTTP_bio_cb_t http_callback = [](BIO *bio, void *arg, int oper, int i) -> BIO* {
            // Simple callback that returns the BIO unchanged
            return bio;
        };
        OSSL_CMP_CTX_set_http_cb(cmp_ctx, http_callback);
    }

    // 6. Parse OCSP response from input data (if enough data)
    const unsigned char *p = data;
    OCSP_BASICRESP *br = d2i_OCSP_BASICRESP(NULL, &p, size);
    if (br) {
        const ASN1_OCTET_STRING *id = NULL;
        const X509_NAME *name = NULL;
        OCSP_resp_get0_id(br, &id, &name);
        
        // If we got an ID, we could process it further
        if (id) {
            // Example: get ID length
            int id_len = ASN1_STRING_length((ASN1_STRING*)id);
        }
    }

    // 7. Get Whirlpool digest and hash some data
    const EVP_MD *whirlpool_md = EVP_whirlpool();
    if (whirlpool_md && size > 0) {
        EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
        if (md_ctx) {
            unsigned char hash[EVP_MAX_MD_SIZE];
            unsigned int hash_len = 0;
            
            EVP_DigestInit_ex(md_ctx, whirlpool_md, NULL);
            EVP_DigestUpdate(md_ctx, data, size);
            EVP_DigestFinal_ex(md_ctx, hash, &hash_len);
            
            EVP_MD_CTX_free(md_ctx);
        }
    }

    // 8. Create output file for any potential output
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // If we had parsed a PKCS7 envelope, we could write it back
        if (env) {
            ASN1_item_i2d_fp(envelope_item, out_file, env);
        }
        assert_file_closed(&out_file);;
    }

    // 9. Clean up all allocated resources
    if (env) {
        ASN1_item_free((ASN1_VALUE*)env, envelope_item);
    }
    
    if (br) {
        OCSP_BASICRESP_free(br);
    }
    
    if (cmp_ctx) {
        OSSL_CMP_CTX_free(cmp_ctx);
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}