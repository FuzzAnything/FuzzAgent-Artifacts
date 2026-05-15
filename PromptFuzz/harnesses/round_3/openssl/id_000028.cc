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
//<ID> 431
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"ossl_sa_set":[[210,9,210,20,0,0,4,0],[210,24,210,34,0,0,4,0]]},"library_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","X509_STORE_new","X509_STORE_CTX_new","X509_STORE_free","d2i_X509_fp","PEM_read_X509","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_is_a","X509_LOOKUP_file","X509_LOOKUP_meth_get_init","EVP_aes_256_cbc","PKCS5_pbe2_set_scrypt","ASN1_TBOOLEAN_it","BIO_new","BIO_s_file","BIO_ctrl","X509_ocspid_print","BIO_free","X509_STORE_set_lookup_crls","X509_STORE_get1_objects","X509_REVOKED_new","X509_REVOKED_get_ext_d2i","AUTHORITY_KEYID_free","OPENSSL_sk_pop_free","ossl_check_X509_OBJECT_sk_type","ossl_check_X509_OBJECT_freefunc_type","X509_REVOKED_free","X509_ALGOR_free","X509_free","EVP_PKEY_CTX_free","X509_STORE_CTX_free","X509_STORE_free"],"critical_calls":["OPENSSL_init_crypto","OPENSSL_init_ssl","X509_STORE_new","X509_STORE_CTX_new","d2i_X509_fp","PEM_read_X509","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_is_a","X509_LOOKUP_file","X509_LOOKUP_meth_get_init","EVP_aes_256_cbc","PKCS5_pbe2_set_scrypt","ASN1_TBOOLEAN_it","BIO_new","BIO_s_file","BIO_ctrl","X509_ocspid_print","BIO_free","X509_STORE_set_lookup_crls","X509_STORE_get1_objects","X509_REVOKED_new","X509_REVOKED_get_ext_d2i","AUTHORITY_KEYID_free","OPENSSL_sk_pop_free","ossl_check_X509_OBJECT_freefunc_type","ossl_check_X509_OBJECT_sk_type","X509_REVOKED_free","X509_ALGOR_free","X509_free","EVP_PKEY_CTX_free","X509_STORE_CTX_free","X509_STORE_free"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified OpenSSL APIs to achieve a coherent event:

*/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize OpenSSL if needed
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    OPENSSL_init_ssl(0, NULL);

    // Create X509_STORE for certificate storage
    X509_STORE *store = X509_STORE_new();
    if (!store) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create X509_STORE_CTX for certificate verification context
    X509_STORE_CTX *store_ctx = X509_STORE_CTX_new();
    if (!store_ctx) {
        X509_STORE_free(store);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Try to load certificates from input data
    X509 *cert = NULL;
    if (size > 0) {
        // Try to read as DER certificate
        fseek(in_file, 0, SEEK_SET);
        cert = d2i_X509_fp(in_file, NULL);
        
        // If that fails, try as PEM
        if (!cert) {
            fseek(in_file, 0, SEEK_SET);
            cert = PEM_read_X509(in_file, NULL, NULL, NULL);
        }
    }

    // Create EVP_PKEY context for key operations
    EVP_PKEY_CTX *pkey_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    
    // Use EVP_PKEY_CTX_is_a to check key type
    if (pkey_ctx) {
        EVP_PKEY_CTX_is_a(pkey_ctx, "RSA");
    }

    // Create X509_LOOKUP_METHOD and get its init function
    X509_LOOKUP_METHOD *lookup_method = X509_LOOKUP_file();
    if (lookup_method) {
        X509_LOOKUP_meth_get_init(lookup_method);
    }

    // Create PBE with scrypt parameters using data as salt
    X509_ALGOR *pbe_algor = NULL;
    if (size >= 16) {
        unsigned char salt[16];
        unsigned char aiv[16];
        
        // Use first 16 bytes as salt, next 16 as aiv if available
        memcpy(salt, data, size > 16 ? 16 : size);
        if (size >= 32) {
            memcpy(aiv, data + 16, 16);
        } else {
            memset(aiv, 0, 16);
        }
        
        const EVP_CIPHER *cipher = EVP_aes_256_cbc();
        pbe_algor = PKCS5_pbe2_set_scrypt(cipher, salt, 16, aiv, 
                                         2048,  // N
                                         8,     // r
                                         1);    // p
    }

    // Get ASN1 boolean item
    const ASN1_ITEM *bool_item = ASN1_TBOOLEAN_it();

    // Create BIO for output operations
    BIO *bio = BIO_new(BIO_s_file());
    if (bio) {
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            BIO_set_fp(bio, out_file, BIO_NOCLOSE);
            
            // Try to print OCSP ID if we have a certificate
            if (cert) {
                X509_ocspid_print(bio, cert);
            }
            
            assert_file_closed(&out_file);;
        }
        BIO_free(bio);
    }

    // Set lookup CRLs function for store
    X509_STORE_set_lookup_crls(store, 
        [](const X509_STORE_CTX *ctx, const X509_NAME *nm) -> struct stack_st_X509_CRL* {
            return NULL;  // Return empty stack for fuzzing
        });

    // Get objects from store
    struct stack_st_X509_OBJECT *objects = X509_STORE_get1_objects(store);

    // Create X509_REVOKED and try to get extension
    X509_REVOKED *revoked = X509_REVOKED_new();
    if (revoked && size > 0) {
        int crit = 0, idx = 0;
        // Try to get authority key identifier extension
        void *ext_data = X509_REVOKED_get_ext_d2i(revoked, NID_authority_key_identifier, 
                                                 &crit, &idx);
        if (ext_data) {
            // If we got extension data, free it
            // Type depends on extension, but we don't know it here
            // For authority key identifier, it's AUTHORITY_KEYID
            AUTHORITY_KEYID_free((AUTHORITY_KEYID*)ext_data);
        }
    }

    // Cleanup
    if (objects) {
        sk_X509_OBJECT_pop_free(objects, X509_OBJECT_free);
    }
    
    if (revoked) {
        X509_REVOKED_free(revoked);
    }
    
    if (pbe_algor) {
        X509_ALGOR_free(pbe_algor);
    }
    
    if (cert) {
        X509_free(cert);
    }
    
    if (pkey_ctx) {
        EVP_PKEY_CTX_free(pkey_ctx);
    }
    
    X509_STORE_CTX_free(store_ctx);
    X509_STORE_free(store);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}