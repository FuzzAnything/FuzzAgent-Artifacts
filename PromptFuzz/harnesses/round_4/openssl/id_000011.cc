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
//<ID> 81
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 3
//<Quality> {"density":10,"unique_branches":{"ossl_dh_compute_key":[[95,12,95,42,0,0,4,0]],"asn1_put_uint64":[[252,14,252,21,0,0,4,0]],"asn1_string_set_int64":[[360,9,360,14,0,0,4,0]]},"library_calls":["d2i_PKCS8_PRIV_KEY_INFO_fp","PKCS8_pkey_get0_attrs","TS_REQ_new","TS_REQ_set_version","DH_new","DH_generate_parameters_ex","DH_generate_key","BN_bin2bn","DH_compute_key","BN_free","DH_free","RIPEMD160_Init","RIPEMD160_Update","RIPEMD160_Final","DIST_POINT_NAME_it","i2d_PKCS8_PRIV_KEY_INFO_fp","i2d_TS_REQ_fp","PKCS8_PRIV_KEY_INFO_free","TS_REQ_free"],"critical_calls":["d2i_PKCS8_PRIV_KEY_INFO_fp","PKCS8_pkey_get0_attrs","TS_REQ_new","TS_REQ_set_version","DH_new","DH_generate_parameters_ex","DH_generate_key","BN_bin2bn","DH_compute_key","BN_free","DH_free","RIPEMD160_Init","RIPEMD160_Update","RIPEMD160_Final","DIST_POINT_NAME_it","i2d_PKCS8_PRIV_KEY_INFO_fp","i2d_TS_REQ_fp","PKCS8_PRIV_KEY_INFO_free","TS_REQ_free"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified APIs to process various OpenSSL data structures:

*/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for various operations

    // 1. Process PKCS8_PRIV_KEY_INFO structure
    PKCS8_PRIV_KEY_INFO *p8 = NULL;
    {
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            p8 = d2i_PKCS8_PRIV_KEY_INFO_fp(in_file, NULL);
            if (p8) {
                // Use PKCS8_pkey_get0_attrs API
                const struct stack_st_X509_ATTRIBUTE *attrs = PKCS8_pkey_get0_attrs(p8);
                (void)attrs;  // Use the result
            }
            assert_file_closed(&in_file);;
        }
    }

    // 2. Process TS_REQ structure
    TS_REQ *ts_req = NULL;
    {
        // Create TS_REQ from remaining data
        ts_req = TS_REQ_new();
        if (ts_req) {
            // Use TS_REQ_set_version API with version from input
            long version = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
            TS_REQ_set_version(ts_req, version);
        }
    }

    // 3. DH operations
    {
        DH *dh = DH_new();
        if (dh) {
            // Generate DH parameters
            DH_generate_parameters_ex(dh, 512, 2, NULL);
            DH_generate_key(dh);
            
            // Create public key from input data
            BIGNUM *pub_key = BN_bin2bn(data + 4, 64, NULL);
            if (pub_key) {
                // Use DH_compute_key API
                unsigned char key[128];
                int key_len = DH_compute_key(key, pub_key, dh);
                (void)key_len;  // Use the result
                BN_free(pub_key);
            }
            DH_free(dh);
        }
    }

    // 4. RIPEMD160 operations
    {
        RIPEMD160_CTX ripemd_ctx;
        RIPEMD160_Init(&ripemd_ctx);
        
        // Update with input data
        size_t hash_size = size > 1000 ? 1000 : size;
        RIPEMD160_Update(&ripemd_ctx, data, hash_size);
        
        // Use RIPEMD160_Final API
        unsigned char ripemd_hash[RIPEMD160_DIGEST_LENGTH];
        RIPEMD160_Final(ripemd_hash, &ripemd_ctx);
    }

    // 5. Use DIST_POINT_NAME_it API
    {
        const ASN1_ITEM *dist_point_item = DIST_POINT_NAME_it();
        (void)dist_point_item;  // Use the result
    }

    // 6. Additional processing with output file
    {
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            // Write processed data to output file
            if (p8) {
                i2d_PKCS8_PRIV_KEY_INFO_fp(out_file, p8);
            }
            if (ts_req) {
                i2d_TS_REQ_fp(out_file, ts_req);
            }
            assert_file_closed(&out_file);;
        }
    }

    // 7. Clean up allocated resources
    if (p8) {
        PKCS8_PRIV_KEY_INFO_free(p8);
    }
    if (ts_req) {
        TS_REQ_free(ts_req);
    }

    return 0;
}