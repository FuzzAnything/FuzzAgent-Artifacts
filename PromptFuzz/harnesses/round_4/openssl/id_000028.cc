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
//<ID> 391
//<Prompt> []
/*<Combination>: [
*/
//<score> 81, nr_unique_branch: 18
//<Quality> {"density":27,"unique_branches":{"ossl_dh_compute_key":[[52,9,52,29,0,0,4,0],[53,12,53,67,0,0,4,1]],"ossl_ffc_params_set0_pqg":[[52,9,52,18,0,0,4,0],[52,22,52,31,0,0,4,0]],"asn1_set_seq_out":[[410,13,410,44,0,0,4,1],[415,17,415,31,0,0,4,1],[418,17,418,31,0,0,4,1],[423,9,423,17,0,0,4,1],[433,32,433,63,0,0,4,0],[433,32,433,63,0,0,4,1],[444,32,444,63,0,0,4,0],[444,32,444,63,0,0,4,1],[450,9,450,21,0,0,4,1]],"asn1_template_ex_i2d":[[289,13,289,25,0,0,4,0],[289,29,289,56,0,0,4,0]],"der_cmp":[[389,14,389,39,0,0,4,0],[389,14,389,39,0,0,4,1],[391,9,391,10,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","ERR_clear_error","d2i_PKCS8_PRIV_KEY_INFO_fp","PKCS8_pkey_get0_attrs","d2i_TS_REQ_fp","TS_REQ_new","TS_REQ_set_version","DH_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_set_word","BN_bin2bn","DH_set0_pqg","DH_size","BN_new","BN_bin2bn","DH_compute_key","RIPEMD160_Init","RIPEMD160_Update","RIPEMD160_Final","ASN1_item_d2i","DIST_POINT_NAME_it","ASN1_item_i2d","DIST_POINT_NAME_it","CRYPTO_free","DIST_POINT_NAME_new","ASN1_item_i2d","DIST_POINT_NAME_it","CRYPTO_free","PKCS8_PRIV_KEY_INFO_free","TS_REQ_free","DH_free","BN_free","BN_free","BN_free","BN_free","DIST_POINT_NAME_free","ASN1_item_free","DIST_POINT_NAME_it","X509_PURPOSE_cleanup","ERR_clear_error"],"critical_calls":["OPENSSL_init_crypto","ERR_clear_error","d2i_PKCS8_PRIV_KEY_INFO_fp","PKCS8_pkey_get0_attrs","d2i_TS_REQ_fp","TS_REQ_new","TS_REQ_set_version","DH_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_set_word","BN_bin2bn","DH_set0_pqg","DH_size","BN_new","BN_bin2bn","DH_compute_key","RIPEMD160_Init","RIPEMD160_Update","RIPEMD160_Final","ASN1_item_d2i","DIST_POINT_NAME_it","ASN1_item_i2d","DIST_POINT_NAME_it","CRYPTO_free","DIST_POINT_NAME_new","ASN1_item_i2d","DIST_POINT_NAME_it","CRYPTO_free","PKCS8_PRIV_KEY_INFO_free","TS_REQ_free","DH_free","BN_free","BN_free","BN_free","BN_free","DIST_POINT_NAME_free","ASN1_item_free","DIST_POINT_NAME_it","X509_PURPOSE_cleanup","ERR_clear_error"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize OpenSSL
    if (!OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, nullptr)) {
        return 0;
    }
    
    // Clear any existing errors
    ERR_clear_error();
    
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    PKCS8_PRIV_KEY_INFO *p8inf = nullptr;
    TS_REQ *ts_req = nullptr;
    DH *dh = nullptr;
    BIGNUM *p = nullptr, *q = nullptr, *g = nullptr;
    BIGNUM *pub_key = nullptr;
    RIPEMD160_CTX ripemd_ctx;
    DIST_POINT_NAME *dpn = nullptr;
    ASN1_VALUE *asn1_val = nullptr;
    unsigned char *der_out = nullptr;
    int der_len = 0;
    unsigned char dh_key[1024];
    unsigned char ripemd_hash[RIPEMD160_DIGEST_LENGTH];
    
    // 2. Read PKCS8 private key info from input data
    in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        p8inf = d2i_PKCS8_PRIV_KEY_INFO_fp(in_file, nullptr);
        if (p8inf) {
            // Get attributes from PKCS8
            const struct stack_st_X509_ATTRIBUTE *attrs = PKCS8_pkey_get0_attrs(p8inf);
            (void)attrs; // Use attrs to avoid unused variable warning
        }
        assert_file_closed(&in_file);;
        in_file = nullptr;
    }
    
    // 3. Read TS_REQ from input data
    in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        ts_req = d2i_TS_REQ_fp(in_file, nullptr);
        assert_file_closed(&in_file);;
        in_file = nullptr;
    }
    
    // If TS_REQ read failed, create a new one
    if (!ts_req) {
        ts_req = TS_REQ_new();
        if (ts_req) {
            TS_REQ_set_version(ts_req, 1); // Set version to 1
        }
    }
    
    // 4. Create and setup DH parameters
    dh = DH_new();
    if (dh) {
        p = BN_new();
        q = BN_new();
        g = BN_new();
        
        if (p && q && g) {
            // Convert input data to BIGNUMs
            if (size > 0) {
                BN_bin2bn(data, std::min((int)size, 32), p);
                BN_set_word(q, 2); // Use small prime for q
                BN_bin2bn(data, std::min((int)size, 32), g);
            }
            
            // Set DH parameters
            DH_set0_pqg(dh, p, q, g);
            p = q = g = nullptr; // DH now owns these
            
            // Get DH size
            int dh_size = DH_size(dh);
            
            // Create a fake public key for DH computation
            pub_key = BN_new();
            if (pub_key && size > 0) {
                BN_bin2bn(data, std::min((int)size, 32), pub_key);
                
                // Compute DH key
                DH_compute_key(dh_key, pub_key, dh);
            }
        }
    }
    
    // 5. Compute RIPEMD160 hash of input data
    if (size > 0) {
        RIPEMD160_Init(&ripemd_ctx);
        RIPEMD160_Update(&ripemd_ctx, data, size);
        RIPEMD160_Final(ripemd_hash, &ripemd_ctx);
    }
    
    // 6. Decode and encode DIST_POINT_NAME
    if (size > 0) {
        const unsigned char *p_data = data;
        long data_len = (long)size;
        
        // Decode DIST_POINT_NAME from input data
        asn1_val = ASN1_item_d2i(nullptr, &p_data, data_len, DIST_POINT_NAME_it());
        
        if (asn1_val) {
            // Encode it back to DER
            der_len = ASN1_item_i2d(asn1_val, &der_out, DIST_POINT_NAME_it());
            
            // Free the DER output
            if (der_out) {
                CRYPTO_free(der_out, __FILE__, __LINE__);
                der_out = nullptr;
            }
        }
        
        // Also test with a new DIST_POINT_NAME
        dpn = DIST_POINT_NAME_new();
        if (dpn) {
            // Encode the empty DIST_POINT_NAME
            der_len = ASN1_item_i2d((ASN1_VALUE*)dpn, &der_out, DIST_POINT_NAME_it());
            
            if (der_out) {
                CRYPTO_free(der_out, __FILE__, __LINE__);
                der_out = nullptr;
            }
        }
    }
    
    // 7. Open output file and write some data (for APIs that need FILE*)
    out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write the RIPEMD160 hash to file
        if (size > 0) {
            fwrite(ripemd_hash, 1, RIPEMD160_DIGEST_LENGTH, out_file);
        }
        assert_file_closed(&out_file);;
        out_file = nullptr;
    }
    
    // 8. Cleanup - free all allocated resources
    if (p8inf) PKCS8_PRIV_KEY_INFO_free(p8inf);
    if (ts_req) TS_REQ_free(ts_req);
    if (dh) DH_free(dh);
    if (p) BN_free(p);
    if (q) BN_free(q);
    if (g) BN_free(g);
    if (pub_key) BN_free(pub_key);
    if (dpn) DIST_POINT_NAME_free(dpn);
    if (asn1_val) ASN1_item_free(asn1_val, DIST_POINT_NAME_it());
    
    // Cleanup X509 purpose
    X509_PURPOSE_cleanup();
    
    // Clear any remaining errors
    ERR_clear_error();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
}