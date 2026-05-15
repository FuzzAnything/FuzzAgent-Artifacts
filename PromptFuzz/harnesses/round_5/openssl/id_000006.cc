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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 136, nr_unique_branch: 61
//<Quality> {"density":17,"unique_branches":{"SHA512_Update_thunk":[[263,9,263,28,0,0,4,0],[276,9,276,17,0,0,4,1]],"sha512_internal_init":[[150,16,150,38,0,0,4,0],[150,42,150,51,0,0,4,0]],"EVP_Q_digest":[[573,9,573,23,0,0,4,0],[577,9,577,22,0,0,4,1]],"sha512_newctx":[[63,9,63,32,1,0,4,1]],"ASN1_item_ex_i2d":[[117,5,117,27,0,0,4,0],[122,13,122,22,0,0,4,1],[126,13,126,20,0,0,4,1],[129,13,129,21,0,0,4,0],[129,25,129,41,0,0,4,0]],"SHA512_Final":[[169,9,169,32,0,0,4,0],[169,9,169,32,0,0,4,1],[188,9,188,20,0,0,4,1],[193,5,193,34,0,0,4,1],[196,5,196,30,0,0,4,1],[199,5,199,30,0,0,4,1],[202,5,202,30,0,0,4,0],[205,5,205,30,0,0,4,1],[222,5,222,12,0,0,4,1],[159,17,159,30,8,0,4,0],[159,17,159,30,8,0,4,1]],"ossl_ffc_params_set0_pqg":[[48,9,48,18,0,0,4,0],[48,22,48,31,0,0,4,0],[52,9,52,18,0,0,4,0],[52,22,52,31,0,0,4,0],[56,9,56,18,0,0,4,0],[56,22,56,31,0,0,4,0]],"BF_cfb64_encrypt":[[36,9,36,16,0,0,4,0],[36,9,36,16,0,0,4,1],[37,16,37,19,0,0,4,0],[37,16,37,19,0,0,4,1],[38,17,38,23,0,0,4,0],[38,17,38,23,0,0,4,1],[57,16,57,19,0,0,4,0],[57,16,57,19,0,0,4,1],[58,17,58,23,0,0,4,0],[58,17,58,23,0,0,4,1]],"SHA512":[[78,9,78,19,0,0,4,1],[80,12,80,62,0,0,4,0]],"asn1_template_ex_i2d":[[235,9,235,35,0,0,4,0],[237,13,237,22,0,0,4,1],[344,9,344,33,0,0,4,0],[348,13,348,19,0,0,4,1],[357,13,357,16,0,0,4,0],[357,13,357,16,0,0,4,1],[357,20,357,29,0,0,4,0],[361,17,361,26,0,0,4,1]],"DSA_set0_pqg":[[247,10,247,29,0,0,4,0],[247,33,247,42,0,0,4,1],[248,13,248,32,0,0,4,0],[248,36,248,45,0,0,4,1],[249,13,249,32,0,0,4,0],[249,36,249,45,0,0,4,1]],"asn1_ex_i2c":[[608,5,608,26,0,0,4,0]],"bn2binpad":[[592,13,592,23,0,0,4,1]],"sha512_internal_final":[[45,13,45,35,0,0,4,0],[45,39,45,56,0,0,4,0],[45,60,45,73,0,0,4,0]],"asn1_i2d_ex_primitive":[[499,9,499,18,0,0,4,1]]},"library_calls":["d2i_ACCESS_DESCRIPTION","i2d_ACCESS_DESCRIPTION","CRYPTO_free","d2i_ISSUING_DIST_POINT","ISSUING_DIST_POINT_free","DSA_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","DSA_set0_pqg","DSA_get0_q","BN_bn2bin","BN_free","BN_free","BN_free","DSA_free","ASN1_STRING_new","ASN1_STRING_set","DISPLAYTEXT_free","ACCESS_DESCRIPTION_free","BF_set_key","BF_cfb64_encrypt","BF_cfb64_encrypt","SHA512","BIO_meth_new","BIO_meth_set_write","BIO_meth_free","SHA512"],"critical_calls":["d2i_ACCESS_DESCRIPTION","i2d_ACCESS_DESCRIPTION","CRYPTO_free","d2i_ISSUING_DIST_POINT","ISSUING_DIST_POINT_free","DSA_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_bin2bn","DSA_set0_pqg","DSA_get0_q","BN_bn2bin","DSA_free","ASN1_STRING_new","ASN1_STRING_set","DISPLAYTEXT_free","ACCESS_DESCRIPTION_free","BF_set_key","BF_cfb64_encrypt","BF_cfb64_encrypt","SHA512","BIO_meth_new","BIO_meth_set_write","BIO_meth_free","SHA512"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 64) {
        return 0; // Need minimum data for operations
    }

    // 1. Parse ACCESS_DESCRIPTION from input data
    const unsigned char *p = data;
    ACCESS_DESCRIPTION *ad = d2i_ACCESS_DESCRIPTION(nullptr, &p, size / 4);
    if (ad) {
        // Convert back to DER and free
        unsigned char *der = nullptr;
        int der_len = i2d_ACCESS_DESCRIPTION(ad, &der);
        if (der) {
            OPENSSL_free(der);
        }
        // Note: We'll free AD after creating DISPLAYTEXT
    }

    // 2. Parse ISSUING_DIST_POINT from next part of input
    p = data + size / 4;
    size_t remaining = size - size / 4;
    ISSUING_DIST_POINT *idp = d2i_ISSUING_DIST_POINT(nullptr, &p, 
                                                    remaining > size / 4 ? size / 4 : remaining);
    if (idp) {
        // Process and free
        ISSUING_DIST_POINT_free(idp);
    }

    // 3. Create and use DSA structure
    DSA *dsa = DSA_new();
    if (dsa) {
        // Generate DSA parameters (simplified - in real fuzzing we'd parse from input)
        BIGNUM *p = BN_new();
        BIGNUM *q = BN_new();
        BIGNUM *g = BN_new();
        
        if (p && q && g && size >= 128) {
            // Use parts of input data as "random" numbers for DSA params
            BN_bin2bn(data + 32, 64, p);
            BN_bin2bn(data + 96, 32, q);
            BN_bin2bn(data, 32, g);
            
            DSA_set0_pqg(dsa, p, q, g);
            
            // Get q parameter
            const BIGNUM *q_param = DSA_get0_q(dsa);
            if (q_param) {
                // Use q parameter in some way
                unsigned char q_buf[32];
                BN_bn2bin(q_param, q_buf);
            }
        } else {
            if (p) BN_free(p);
            if (q) BN_free(q);
            if (g) BN_free(g);
        }
        
        DSA_free(dsa);
    }

    // 4. Create DISPLAYTEXT and free it
    if (ad) {
        // ACCESS_DESCRIPTION contains GENERAL_NAME which may have ASN1_STRING
        // Create a standalone DISPLAYTEXT (ASN1_STRING) for demonstration
        ASN1_STRING *display_text = ASN1_STRING_new();
        if (display_text && size >= 48) {
            ASN1_STRING_set(display_text, data + 16, 32);
            DISPLAYTEXT_free(display_text); // DISPLAYTEXT is typedef of ASN1_STRING
        }
        
        // Free the ACCESS_DESCRIPTION
        ACCESS_DESCRIPTION_free(ad);
    }

    // 5. Blowfish CFB encryption/decryption
    if (size >= 80) {
        BF_KEY bf_key;
        unsigned char ivec[8] = {0};
        int num = 0;
        
        // Initialize Blowfish key from input data
        BF_set_key(&bf_key, 16, data + 48); // Use 16 bytes from input as key
        
        // Prepare input and output buffers
        size_t bf_len = size - 64;
        if (bf_len > 256) bf_len = 256; // Limit size
        
        unsigned char *in_buf = (unsigned char *)malloc(bf_len);
        unsigned char *out_buf = (unsigned char *)malloc(bf_len);
        
        if (in_buf && out_buf) {
            memcpy(in_buf, data + 64, bf_len);
            
            // Encrypt
            BF_cfb64_encrypt(in_buf, out_buf, bf_len, &bf_key, ivec, &num, BF_ENCRYPT);
            
            // Reset IV and num for decryption
            memset(ivec, 0, sizeof(ivec));
            num = 0;
            
            // Decrypt back
            unsigned char *dec_buf = (unsigned char *)malloc(bf_len);
            if (dec_buf) {
                BF_cfb64_encrypt(out_buf, dec_buf, bf_len, &bf_key, ivec, &num, BF_DECRYPT);
                free(dec_buf);
            }
        }
        
        if (in_buf) free(in_buf);
        if (out_buf) free(out_buf);
    }

    // 6. SHA512 hash calculation
    if (size > 0) {
        unsigned char sha512_hash[SHA512_DIGEST_LENGTH];
        SHA512(data, size > 1024 ? 1024 : size, sha512_hash);
        // Hash is computed, can be used for further processing
    }

    // 7. BIO_METHOD creation and freeing
    BIO_METHOD *biom = BIO_meth_new(BIO_TYPE_SOURCE_SINK, "fuzz_bio");
    if (biom) {
        // Set up some methods (simplified)
        BIO_meth_set_write(biom, [](BIO *b, const char *buf, int len) -> int {
            return len; // Simplified write
        });
        
        BIO_meth_free(biom);
    }

    // 8. File operations using the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read some data from the memory file
        unsigned char read_buf[64];
        fread(read_buf, 1, sizeof(read_buf), in_file);
        assert_file_closed(&in_file);;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write processed data (hash in this case)
        unsigned char sha512_hash[SHA512_DIGEST_LENGTH];
        SHA512(data, size > 1024 ? 1024 : size, sha512_hash);
        fwrite(sha512_hash, 1, SHA512_DIGEST_LENGTH, out_file);
        assert_file_closed(&out_file);;
        
        // Clean up output file
        remove("output_file");
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}