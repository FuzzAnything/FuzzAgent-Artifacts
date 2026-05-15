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
//<ID> 241
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 13
//<Quality> {"density":9,"unique_branches":{"BN_mod_exp_mont":[[309,6,309,15,8,0,4,0]],"generate_key":[[343,21,343,61,0,0,4,1]],"dh_builtin_genparams":[[192,9,192,36,0,0,4,1],[198,16,198,43,0,0,4,1],[210,13,210,33,0,0,4,1],[212,13,212,33,0,0,4,1]],"calc_trial_divisions":[[76,9,76,20,0,0,4,1],[78,14,78,26,0,0,4,0]],"bn_sqr_words":[[178,12,178,18,0,0,4,0],[187,9,187,10,0,0,4,1],[192,13,192,21,0,0,4,0]],"bn_mod_exp_mont_fixed_top":[[723,58,723,79,0,0,4,0]],"bn_mul_normal":[[640,13,640,22,0,0,4,1]]},"library_calls":["d2i_PKCS8_PRIV_KEY_INFO_fp","ERR_peek_error_data","PKCS8_pkey_get0_attrs","TS_REQ_new","TS_REQ_set_version","DH_new","DH_generate_parameters_ex","DH_generate_key","BN_bin2bn","DH_compute_key","BN_free","OSSL_CMP_ATAVS_new","RIPEMD160_Init","RIPEMD160_Update","RIPEMD160_Final","DIST_POINT_NAME_it","i2d_PKCS8_PRIV_KEY_INFO_fp","i2d_TS_REQ_fp","PKCS8_PRIV_KEY_INFO_free","TS_REQ_free","DH_free","OSSL_CMP_ATAVS_free"],"critical_calls":["d2i_PKCS8_PRIV_KEY_INFO_fp","ERR_peek_error_data","PKCS8_pkey_get0_attrs","TS_REQ_new","TS_REQ_set_version","DH_new","DH_generate_parameters_ex","DH_generate_key","BN_bin2bn","DH_compute_key","BN_free","OSSL_CMP_ATAVS_new","RIPEMD160_Init","RIPEMD160_Update","RIPEMD160_Final","DIST_POINT_NAME_it","i2d_PKCS8_PRIV_KEY_INFO_fp","i2d_TS_REQ_fp","PKCS8_PRIV_KEY_INFO_free","TS_REQ_free","DH_free","OSSL_CMP_ATAVS_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	 // Minimum size for meaningful operations

    // 1. Read PKCS8 private key from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    PKCS8_PRIV_KEY_INFO *p8inf = NULL;
    p8inf = d2i_PKCS8_PRIV_KEY_INFO_fp(in_file, &p8inf);
    
    // Check for errors
    const char *err_data;
    int err_flags;
    ERR_peek_error_data(&err_data, &err_flags);
    
    // Get attributes from PKCS8 if available
    if (p8inf) {
        const struct stack_st_X509_ATTRIBUTE *attrs = PKCS8_pkey_get0_attrs(p8inf);
        (void)attrs; // Use in some way
    }
    
    // 2. Create and configure a timestamp request
    TS_REQ *ts_req = TS_REQ_new();
    if (ts_req) {
        // Use first 4 bytes for version (but ensure it's valid)
        long version = (data[0] % 10) + 1; // Keep in reasonable range
        TS_REQ_set_version(ts_req, version);
    }
    
    // 3. DH key generation and computation
    DH *dh = DH_new();
    if (dh) {
        // Generate DH parameters (using small values for fuzzing speed)
        int prime_len = 512 + (data[1] % 512); // 512-1024 bits
        int generator = 2 + (data[2] % 3); // 2, 3, or 5
        DH_generate_parameters_ex(dh, prime_len, generator, NULL);
        
        // Generate DH key pair
        DH_generate_key(dh);
        
        // Create a BIGNUM from input data for public key computation
        size_t bn_len = size > 256 ? 256 : size;
        BIGNUM *pub_key = BN_bin2bn(data, bn_len, NULL);
        if (pub_key) {
            unsigned char shared_key[1024];
            DH_compute_key(shared_key, pub_key, dh);
            BN_free(pub_key);
        }
    }
    
    // 4. Create OSSL_CMP attribute value set
    OSSL_CMP_ATAVS *atavs = OSSL_CMP_ATAVS_new();
    
    // 5. RIPEMD160 hashing of input data
    RIPEMD160_CTX ripemd_ctx;
    RIPEMD160_Init(&ripemd_ctx);
    size_t hash_len = size > 1024 ? 1024 : size;
    RIPEMD160_Update(&ripemd_ctx, data, hash_len);
    unsigned char ripemd_hash[RIPEMD160_DIGEST_LENGTH];
    RIPEMD160_Final(ripemd_hash, &ripemd_ctx);
    
    // 6. Get DIST_POINT_NAME ASN1 item (for completeness)
    const ASN1_ITEM *dist_point_item = DIST_POINT_NAME_it();
    (void)dist_point_item;
    
    // 7. Write PKCS8 private key to output file if we read one
    FILE *out_pkcs8 = fopen("output_file_pkcs8", "wb");
    if (out_pkcs8 && p8inf) {
        i2d_PKCS8_PRIV_KEY_INFO_fp(out_pkcs8, p8inf);
        assert_file_closed(&out_pkcs8);;
    } else if (out_pkcs8) {
        assert_file_closed(&out_pkcs8);;
    }
    
    // 8. Write TS_REQ to output file if created
    FILE *out_ts = fopen("output_file_ts", "wb");
    if (out_ts && ts_req) {
        i2d_TS_REQ_fp(out_ts, ts_req);
        assert_file_closed(&out_ts);;
    } else if (out_ts) {
        assert_file_closed(&out_ts);;
    }
    
    // 9. Cleanup
    if (p8inf) PKCS8_PRIV_KEY_INFO_free(p8inf);
    if (ts_req) TS_REQ_free(ts_req);
    if (dh) DH_free(dh);
    if (atavs) OSSL_CMP_ATAVS_free(atavs);
    
    assert_file_closed(&in_file);;
    
    // Remove output files to avoid accumulation
    remove("output_file_pkcs8");
    remove("output_file_ts");
    
    assert_file_closed(&out_ts);
	assert_file_closed(&out_pkcs8);
	assert_file_closed(&in_file);
	return 0;
}