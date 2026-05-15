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
//<ID> 430
//<Prompt> []
/*<Combination>: [
*/
//<score> 80, nr_unique_branch: 11
//<Quality> {"density":10,"unique_branches":{"scrypt_alg":[[501,9,501,29,0,0,4,0]],"EVP_PBE_scrypt_ex":[[83,9,83,55,0,0,4,0]],"kdf_scrypt_set_ctx_params":[[237,16,237,30,0,0,4,0],[238,16,238,43,0,0,4,0],[244,56,244,69,0,0,4,0],[250,56,250,69,0,0,4,0]],"X509_REVOKED_set_serialNumber":[[168,9,168,18,0,0,4,1],[171,9,171,21,0,0,4,0]],"kdf_scrypt_derive":[[193,36,193,75,0,0,4,0]],"PKCS5_pbe2_set_scrypt":[[60,9,60,67,0,0,4,0]],"i2c_ibuf":[[107,21,107,30,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","ERR_clear_error","BIO_new_fp","BIO_new_fp","X509_STORE_new","X509_STORE_get1_objects","OPENSSL_sk_pop_free","ossl_check_X509_OBJECT_sk_type","ossl_check_X509_OBJECT_freefunc_type","X509_STORE_set_lookup_crls","X509_STORE_free","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_is_a","EVP_PKEY_CTX_free","X509_LOOKUP_file","X509_LOOKUP_meth_get_init","EVP_aes_256_cbc","PKCS5_pbe2_set_scrypt","X509_ALGOR_free","ASN1_TBOOLEAN_it","d2i_X509_fp","X509_ocspid_print","X509_free","X509_REVOKED_new","ASN1_INTEGER_new","ASN1_INTEGER_set","X509_REVOKED_set_serialNumber","ASN1_INTEGER_free","X509_REVOKED_get_ext_d2i","X509_REVOKED_free","BIO_free","BIO_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_ssl","ERR_clear_error","BIO_new_fp","BIO_new_fp","X509_STORE_new","X509_STORE_get1_objects","OPENSSL_sk_pop_free","ossl_check_X509_OBJECT_freefunc_type","ossl_check_X509_OBJECT_sk_type","X509_STORE_set_lookup_crls","X509_STORE_free","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_is_a","EVP_PKEY_CTX_free","X509_LOOKUP_file","X509_LOOKUP_meth_get_init","EVP_aes_256_cbc","PKCS5_pbe2_set_scrypt","X509_ALGOR_free","ASN1_TBOOLEAN_it","d2i_X509_fp","X509_ocspid_print","X509_free","X509_REVOKED_new","ASN1_INTEGER_new","ASN1_INTEGER_set","X509_REVOKED_set_serialNumber","ASN1_INTEGER_free","X509_REVOKED_get_ext_d2i","X509_REVOKED_free","BIO_free","BIO_free","ERR_clear_error"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations

    // Initialize OpenSSL
    OPENSSL_init_ssl(0, NULL);
    ERR_clear_error();

    // Create file from input data
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

    // Create BIOs for file operations
    BIO *bio_in = BIO_new_fp(in_file, BIO_NOCLOSE);
    BIO *bio_out = BIO_new_fp(out_file, BIO_NOCLOSE);
    
    // 1. Create and use X509_STORE
    X509_STORE *store = X509_STORE_new();
    if (store) {
        // Get objects from store
        struct stack_st_X509_OBJECT *objs = X509_STORE_get1_objects(store);
        if (objs) {
            sk_X509_OBJECT_pop_free(objs, X509_OBJECT_free);
        }
        
        // Set lookup CRLs callback
        X509_STORE_set_lookup_crls(store, NULL);
        
        X509_STORE_free(store);
    }

    // 2. Create EVP_PKEY_CTX and check key type
    EVP_PKEY_CTX *pkey_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (pkey_ctx) {
        EVP_PKEY_CTX_is_a(pkey_ctx, "RSA");
        EVP_PKEY_CTX_free(pkey_ctx);
    }

    // 3. Get X509_LOOKUP_METHOD init function
    X509_LOOKUP_METHOD *lookup_meth = X509_LOOKUP_file();
    if (lookup_meth) {
        X509_LOOKUP_meth_get_init(lookup_meth);
    }

    // 4. Create PBE2 with scrypt parameters using data
    const EVP_CIPHER *cipher = EVP_aes_256_cbc();
    unsigned char salt[16];
    unsigned char aiv[16];
    
    // Use first 32 bytes of data for salt and aiv
    size_t copy_len = size > 32 ? 32 : size;
    memcpy(salt, data, copy_len > 16 ? 16 : copy_len);
    if (copy_len > 16) {
        memcpy(aiv, data + 16, copy_len - 16 > 16 ? 16 : copy_len - 16);
    }
    
    X509_ALGOR *algor = PKCS5_pbe2_set_scrypt(cipher, salt, 16, aiv, 
                                             (uint64_t)data[0] * 1024, 
                                             (uint64_t)data[1] * 8, 
                                             (uint64_t)data[2]);
    if (algor) {
        X509_ALGOR_free(algor);
    }

    // 5. Get ASN1 boolean item
    const ASN1_ITEM *bool_item = ASN1_TBOOLEAN_it();

    // 6. Create and print X509 certificate OCSP ID
    // Try to read certificate from input
    fseek(in_file, 0, SEEK_SET);
    X509 *cert = d2i_X509_fp(in_file, NULL);
    if (cert) {
        X509_ocspid_print(bio_out, cert);
        X509_free(cert);
    }

    // 7. Create revoked certificate and get extension
    X509_REVOKED *revoked = X509_REVOKED_new();
    if (revoked) {
        // Add serial number
        ASN1_INTEGER *serial = ASN1_INTEGER_new();
        if (serial) {
            // Use some data for serial number
            ASN1_INTEGER_set(serial, (long)(data[0] << 8 | data[1]));
            X509_REVOKED_set_serialNumber(revoked, serial);
            ASN1_INTEGER_free(serial);
        }
        
        // Try to get extension (NID_crl_reason = 141)
        int crit = 0, idx = 0;
        void *ext_data = X509_REVOKED_get_ext_d2i(revoked, 141, &crit, &idx);
        
        X509_REVOKED_free(revoked);
    }

    // Cleanup
    BIO_free(bio_in);
    BIO_free(bio_out);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clear any remaining errors
    ERR_clear_error();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}