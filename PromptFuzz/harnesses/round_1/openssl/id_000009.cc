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
//<ID> 174
//<Prompt> []
/*<Combination>: [
*/
//<score> 150, nr_unique_branch: 47
//<Quality> {"density":10,"unique_branches":{"X509_NAME_ENTRY_set_data":[[327,9,327,21,0,0,4,1],[327,26,327,41,0,0,4,1],[329,9,329,19,0,0,4,0],[329,23,329,45,0,0,4,0],[330,16,332,44,0,0,4,0]],"OPENSSL_gmtime_diff":[[252,9,252,43,0,0,4,0]],"asn1_item_clear":[[178,5,178,27,0,0,4,0],[180,13,180,15,0,0,4,0],[180,19,180,36,0,0,4,1]],"ASN1_STRING_TABLE_get":[[132,9,132,17,0,0,4,1],[143,9,143,23,0,0,4,1]],"ASN1_mbstring_ncopy":[[56,9,56,18,0,0,4,0],[59,13,59,29,0,0,4,1],[110,9,110,22,0,0,4,0],[110,26,110,43,0,0,4,1],[116,9,116,22,0,0,4,0],[116,26,116,43,0,0,4,1]],"X509_NAME_ENTRY_create_by_OBJ":[[291,9,291,21,0,0,4,0],[292,13,292,50,0,0,4,1],[297,9,297,46,0,0,4,1],[299,9,299,57,0,0,4,1],[302,9,302,21,0,0,4,1]],"EC_POINT_copy":[[803,13,803,48,0,0,4,0],[804,16,804,37,0,0,4,0],[805,16,805,36,0,0,4,1]],"posix_time_from_utc":[[99,9,99,41,0,0,4,0]],"X509_NAME_ENTRY_create_by_txt":[[256,9,256,20,0,0,4,1]],"ASN1_STRING_set_by_NID":[[88,9,88,20,0,0,4,1],[91,9,91,20,0,0,4,0],[93,13,93,43,0,0,4,0],[101,9,101,17,0,0,4,1]],"X509_NAME_add_entry":[[200,9,200,21,0,0,4,1],[204,9,204,16,0,0,4,1],[206,14,206,21,0,0,4,0],[211,9,211,18,0,0,4,1],[220,13,220,21,0,0,4,0],[221,17,221,25,0,0,4,1],[229,9,229,53,0,0,4,1],[232,9,232,35,0,0,4,1],[236,9,236,12,0,0,4,0],[238,27,238,32,0,0,4,1]],"X509_NAME_ENTRY_set_object":[[313,9,313,21,0,0,4,1],[313,25,313,38,0,0,4,1],[319,13,319,33,0,0,4,1]],"ASN1_item_dup":[[63,9,63,41,0,0,4,0],[67,19,67,30,0,0,4,1]],"is_valid_date":[[38,9,38,16,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","OPENSSL_init_crypto","OPENSSL_gmtime_diff","EC_KEY_new_by_curve_name","EC_KEY_get0_group","EC_POINT_new","EC_POINT_new","EC_POINT_copy","EC_GROUP_get0_generator","EC_POINT_copy","EC_POINT_free","EC_POINT_free","EC_KEY_free","OSSL_FUNC_asym_cipher_settable_ctx_params","X509_NAME_new","X509_NAME_ENTRY_create_by_txt","X509_NAME_add_entry","ossl_check_X509_NAME_ENTRY_copyfunc_type","X509_NAME_ENTRY_free","X509_NAME_free","BN_CTX_new","BN_CTX_start","BN_CTX_get","BN_bin2bn","BN_CTX_end","BN_CTX_free","POLICYINFO_new","ossl_check_POLICYINFO_compfunc_type","POLICYINFO_free","d2i_X509_fp","PEM_read_X509","OCSP_RESPID_new","OCSP_RESPID_set_by_key","OCSP_RESPID_free","X509_free","OSSL_CRMF_MSG_new","ossl_check_OSSL_CRMF_MSG_copyfunc_type","OSSL_CRMF_MSG_free"],"critical_calls":["OPENSSL_init_crypto","OPENSSL_init_crypto","OPENSSL_gmtime_diff","EC_KEY_new_by_curve_name","EC_KEY_get0_group","EC_POINT_new","EC_POINT_new","EC_POINT_copy","EC_GROUP_get0_generator","EC_POINT_copy","EC_POINT_free","EC_POINT_free","EC_KEY_free","OSSL_FUNC_asym_cipher_settable_ctx_params","X509_NAME_new","X509_NAME_ENTRY_create_by_txt","X509_NAME_add_entry","ossl_check_X509_NAME_ENTRY_copyfunc_type","X509_NAME_ENTRY_free","X509_NAME_free","BN_CTX_new","BN_CTX_start","BN_CTX_get","BN_bin2bn","BN_CTX_end","BN_CTX_free","POLICYINFO_new","ossl_check_POLICYINFO_compfunc_type","POLICYINFO_free","d2i_X509_fp","PEM_read_X509","OCSP_RESPID_new","OCSP_RESPID_set_by_key","OCSP_RESPID_free","X509_free","OSSL_CRMF_MSG_new","ossl_check_OSSL_CRMF_MSG_copyfunc_type","OSSL_CRMF_MSG_free"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified OpenSSL APIs:

*/

#ifdef OPENSSL_NO_EC
#error EC is not supported
#endif


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize OpenSSL error strings
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    // 1. Use OPENSSL_gmtime_diff
    {
        struct tm from_tm = {0};
        struct tm to_tm = {0};
        
        // Parse some data into tm structures (simplified)
        if (size >= 24) {
            memcpy(&from_tm, data, sizeof(from_tm) > 12 ? 12 : sizeof(from_tm));
            memcpy(&to_tm, data + 12, sizeof(to_tm) > 12 ? 12 : sizeof(to_tm));
            
            int pday, psec;
            OPENSSL_gmtime_diff(&pday, &psec, &from_tm, &to_tm);
        }
    }

    // 2. Use EC_POINT_copy - Create EC_POINT and copy it
    {
        EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
        if (ec_key) {
            const EC_GROUP *group = EC_KEY_get0_group(ec_key);
            EC_POINT *point1 = EC_POINT_new(group);
            EC_POINT *point2 = EC_POINT_new(group);
            
            if (point1 && point2) {
                // Set point1 to generator
                EC_POINT_copy(point1, EC_GROUP_get0_generator(group));
                // Copy point1 to point2
                EC_POINT_copy(point2, point1);
            }
            
            EC_POINT_free(point1);
            EC_POINT_free(point2);
            EC_KEY_free(ec_key);
        }
    }

    // 3. Use OSSL_FUNC_asym_cipher_settable_ctx_params
    {
        // Create a dummy dispatch table
        static const OSSL_DISPATCH dummy_dispatch[] = {
            { OSSL_FUNC_ASYM_CIPHER_ENCRYPT_INIT, NULL },
            { 0, NULL }
        };
        
        OSSL_FUNC_asym_cipher_settable_ctx_params_fn *fn = 
            OSSL_FUNC_asym_cipher_settable_ctx_params(dummy_dispatch);
        // fn might be NULL, but we've called the API
    }

    // 4. Use ossl_check_X509_NAME_ENTRY_copyfunc_type
    {
        X509_NAME *name = X509_NAME_new();
        if (name) {
            // Create a simple name entry
            X509_NAME_ENTRY *entry = X509_NAME_ENTRY_create_by_txt(NULL, "CN", 
                MBSTRING_ASC, (const unsigned char*)"Test", -1);
            if (entry) {
                X509_NAME_add_entry(name, entry, -1, 0);
                
                // Get the copy function type (though we don't execute it)
                sk_X509_NAME_ENTRY_copyfunc copy_func = NULL;
                ossl_check_X509_NAME_ENTRY_copyfunc_type(copy_func);
                
                X509_NAME_ENTRY_free(entry);
            }
            X509_NAME_free(name);
        }
    }

    // 5. Use BN_CTX_start
    {
        BN_CTX *ctx = BN_CTX_new();
        if (ctx) {
            BN_CTX_start(ctx);
            // Use the context briefly
            BIGNUM *bn = BN_CTX_get(ctx);
            if (bn && size > 20) {
                BN_bin2bn(data, size > 32 ? 32 : size, bn);
            }
            BN_CTX_end(ctx);
            BN_CTX_free(ctx);
        }
    }

    // 6. Use ossl_check_POLICYINFO_compfunc_type
    {
        POLICYINFO *policy = POLICYINFO_new();
        if (policy) {
            sk_POLICYINFO_compfunc comp_func = NULL;
            ossl_check_POLICYINFO_compfunc_type(comp_func);
            POLICYINFO_free(policy);
        }
    }

    // 7. Use OCSP_RESPID_set_by_key
    {
        // Try to parse input as X509 certificate
        fseek(in_file, 0, SEEK_SET);
        X509 *cert = d2i_X509_fp(in_file, NULL);
        if (!cert) {
            // Try PEM format
            fseek(in_file, 0, SEEK_SET);
            cert = PEM_read_X509(in_file, NULL, NULL, NULL);
        }
        
        if (cert) {
            OCSP_RESPID *respid = OCSP_RESPID_new();
            if (respid) {
                OCSP_RESPID_set_by_key(respid, cert);
                OCSP_RESPID_free(respid);
            }
            X509_free(cert);
        }
    }

    // 8. Use ossl_check_OSSL_CRMF_MSG_copyfunc_type
    {
        OSSL_CRMF_MSG *crmf_msg = OSSL_CRMF_MSG_new();
        if (crmf_msg) {
            sk_OSSL_CRMF_MSG_copyfunc copy_func = NULL;
            ossl_check_OSSL_CRMF_MSG_copyfunc_type(copy_func);
            OSSL_CRMF_MSG_free(crmf_msg);
        }
    }

    // Write some data to output file using the input
    if (size > 0) {
        fwrite(data, 1, size > 1024 ? 1024 : size, out_file);
    }

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");

    // Clean up OpenSSL
    ERR_free_strings();
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}