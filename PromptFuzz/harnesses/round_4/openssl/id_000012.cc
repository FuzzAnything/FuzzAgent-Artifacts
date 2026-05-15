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
//<ID> 83
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 2
//<Quality> {"density":24,"unique_branches":{"bin2bn":[[457,9,457,17,0,0,4,0]],"ossl_dh_compute_key":[[72,9,72,29,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","ERR_clear_error","d2i_PKCS8_PRIV_KEY_INFO_fp","PKCS8_pkey_get0_attrs","PKCS8_PRIV_KEY_INFO_free","d2i_TS_REQ_fp","TS_REQ_new","TS_REQ_set_version","TS_REQ_set_version","DH_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_set_word","DH_set0_pqg","BN_bin2bn","DH_size","DH_compute_key","BN_free","BN_free","BN_free","BN_free","DH_free","RIPEMD160_Init","RIPEMD160_Update","RIPEMD160_Final","DIST_POINT_NAME_it","ASN1_item_d2i","ASN1_item_free","DIST_POINT_NAME_new","ASN1_item_i2d","CRYPTO_free","DIST_POINT_NAME_free","TS_REQ_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_crypto","ERR_clear_error","d2i_PKCS8_PRIV_KEY_INFO_fp","PKCS8_pkey_get0_attrs","PKCS8_PRIV_KEY_INFO_free","d2i_TS_REQ_fp","TS_REQ_new","TS_REQ_set_version","DH_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_set_word","DH_set0_pqg","BN_bin2bn","DH_size","DH_compute_key","BN_free","DH_free","RIPEMD160_Init","RIPEMD160_Update","RIPEMD160_Final","DIST_POINT_NAME_it","ASN1_item_d2i","DIST_POINT_NAME_new","ASN1_item_i2d","CRYPTO_free","DIST_POINT_NAME_free","TS_REQ_free","ERR_clear_error"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize OpenSSL
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    ERR_clear_error();

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Use PKCS8_pkey_get0_attrs with PKCS8_PRIV_KEY_INFO
    PKCS8_PRIV_KEY_INFO *p8 = d2i_PKCS8_PRIV_KEY_INFO_fp(in_file, NULL);
    if (p8) {
        const struct stack_st_X509_ATTRIBUTE *attrs = PKCS8_pkey_get0_attrs(p8);
        // We can't do much with attrs but at least we called the function
        (void)attrs;
        PKCS8_PRIV_KEY_INFO_free(p8);
    }
    
    // Reset file pointer for next operation
    rewind(in_file);

    // 2. Use TS_REQ_set_version with TS_REQ
    TS_REQ *ts_req = d2i_TS_REQ_fp(in_file, NULL);
    if (!ts_req) {
        // If parsing failed, create a new TS_REQ
        ts_req = TS_REQ_new();
        if (ts_req) {
            // Use first byte as version (masked to reasonable range)
            long version = (size > 0) ? (data[0] & 0x7F) : 1;
            TS_REQ_set_version(ts_req, version);
        }
    } else {
        // Modify existing TS_REQ version
        long version = (size > 0) ? (data[0] & 0x7F) : 1;
        TS_REQ_set_version(ts_req, version);
    }
    
    rewind(in_file);

    // 3. Use DH_compute_key with DH
    DH *dh = DH_new();
    if (dh) {
        // Generate DH parameters (using small values for fuzzing)
        BIGNUM *p = BN_new();
        BIGNUM *g = BN_new();
        BIGNUM *pub_key = BN_new();
        
        if (p && g && pub_key) {
            // Use input data to generate DH parameters
            BN_bin2bn(data, size < 128 ? size : 128, p);
            BN_set_word(g, 2); // Common generator
            
            DH_set0_pqg(dh, p, NULL, g);
            
            // Generate a public key from input data
            BN_bin2bn(data + (size > 128 ? 128 : 0), 
                     size > 128 ? (size - 128 > 128 ? 128 : size - 128) : 0, 
                     pub_key);
            
            // Compute shared key
            unsigned char *shared_key = (unsigned char *)malloc(DH_size(dh));
            if (shared_key) {
                DH_compute_key(shared_key, pub_key, dh);
                free(shared_key);
            }
            
            BN_free(pub_key);
        } else {
            if (p) BN_free(p);
            if (g) BN_free(g);
            if (pub_key) BN_free(pub_key);
        }
        DH_free(dh);
    }
    
    rewind(in_file);

    // 4. Use RIPEMD160_Final with RIPEMD160_CTX
    RIPEMD160_CTX *ripemd_ctx = (RIPEMD160_CTX *)malloc(sizeof(RIPEMD160_CTX));
    if (ripemd_ctx) {
        RIPEMD160_Init(ripemd_ctx);
        
        // Update with input data
        size_t chunk_size = 64;
        for (size_t i = 0; i < size; i += chunk_size) {
            size_t remaining = size - i;
            size_t current_chunk = remaining < chunk_size ? remaining : chunk_size;
            RIPEMD160_Update(ripemd_ctx, data + i, current_chunk);
        }
        
        unsigned char ripemd_result[RIPEMD160_DIGEST_LENGTH];
        RIPEMD160_Final(ripemd_result, ripemd_ctx);
        
        free(ripemd_ctx);
    }
    
    rewind(in_file);

    // 5. Use DIST_POINT_NAME_it for ASN1 operations
    const ASN1_ITEM *dist_point_name_it = DIST_POINT_NAME_it();
    
    // Try to parse DIST_POINT_NAME from input
    const unsigned char *p = data;
    DIST_POINT_NAME *dist_point = (DIST_POINT_NAME *)ASN1_item_d2i(
        NULL, &p, size, dist_point_name_it);
    
    if (dist_point) {
        // We have a valid DIST_POINT_NAME
        ASN1_item_free((ASN1_VALUE *)dist_point, dist_point_name_it);
    } else {
        // Try to create and encode a simple DIST_POINT_NAME
        DIST_POINT_NAME *new_dist = DIST_POINT_NAME_new();
        if (new_dist) {
            unsigned char *buf = NULL;
            int len = ASN1_item_i2d((ASN1_VALUE *)new_dist, &buf, dist_point_name_it);
            if (buf) {
                OPENSSL_free(buf);
            }
            DIST_POINT_NAME_free(new_dist);
        }
    }

    // Cleanup TS_REQ if created
    if (ts_req) {
        TS_REQ_free(ts_req);
    }

    // Close file and cleanup
    assert_file_closed(&in_file);;
    
    // Clear any remaining OpenSSL errors
    ERR_clear_error();
    
    assert_file_closed(&in_file);
	return 0;
}