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
//<ID> 283
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 10
//<Quality> {"density":8,"unique_branches":{"ssl_set_new_record_layer":[[1372,9,1372,18,0,0,4,1],[1375,16,1375,26,0,0,4,0],[1375,30,1375,70,0,0,4,0],[1375,30,1375,70,0,0,4,1]],"ossl_statem_set_in_init":[[207,9,207,36,0,0,4,0],[207,40,207,80,0,0,4,1]],"SSL_set_connect_state":[[96,6,96,19,2,0,4,0],[17,6,17,19,3,0,4,1],[18,23,18,61,3,0,4,0],[1242,29,1242,48,4,0,4,1]]},"library_calls":["d2i_X509_REQ_fp","X509_REQ_get_pubkey","EVP_PKEY_get1_RSA","RSA_print_fp","RSA_free","EVP_PKEY_free","X509_REQ_free","d2i_X509_NAME_ENTRY","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_cmp","ASN1_OCTET_STRING_free","ASN1_OCTET_STRING_free","X509_NAME_ENTRY_free","SSL_CTX_new","TLS_method","SSL_new","SSL_set_connect_state","SSL_client_hello_get0_ciphers","SSL_is_quic","SSL_free","SSL_CTX_free","EVP_PKEY_CTX_new_id","EVP_PKEY_derive_init","EVP_PKEY_CTX_set_scrypt_N","EVP_PKEY_CTX_free","EC_KEY_new_by_curve_name","EC_KEY_generate_key","EC_KEY_decoded_from_explicit_params","EC_KEY_free"],"critical_calls":["d2i_X509_REQ_fp","X509_REQ_get_pubkey","EVP_PKEY_get1_RSA","RSA_print_fp","RSA_free","EVP_PKEY_free","X509_REQ_free","d2i_X509_NAME_ENTRY","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_set","ASN1_OCTET_STRING_cmp","ASN1_OCTET_STRING_free","ASN1_OCTET_STRING_free","X509_NAME_ENTRY_free","SSL_CTX_new","TLS_method","SSL_new","SSL_set_connect_state","SSL_client_hello_get0_ciphers","SSL_is_quic","SSL_free","SSL_CTX_free","EVP_PKEY_CTX_new_id","EVP_PKEY_derive_init","EVP_PKEY_CTX_set_scrypt_N","EVP_PKEY_CTX_free","EC_KEY_new_by_curve_name","EC_KEY_generate_key","EC_KEY_decoded_from_explicit_params","EC_KEY_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
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

    // 1. Decode X509_REQ from input file
    X509_REQ *req = NULL;
    req = d2i_X509_REQ_fp(in_file, &req);
    
    // 2. If X509_REQ was decoded, extract RSA key and print it
    if (req) {
        EVP_PKEY *pkey = X509_REQ_get_pubkey(req);
        if (pkey) {
            RSA *rsa = EVP_PKEY_get1_RSA(pkey);
            if (rsa) {
                RSA_print_fp(out_file, rsa, 0);
                RSA_free(rsa);
            }
            EVP_PKEY_free(pkey);
        }
        X509_REQ_free(req);
    }

    // 3. Decode X509_NAME_ENTRY from remaining data
    rewind(in_file);
    const unsigned char *p = data;
    X509_NAME_ENTRY *name_entry = NULL;
    name_entry = d2i_X509_NAME_ENTRY(&name_entry, &p, size);
    
    // 4. Create and compare ASN1_OCTET_STRINGs
    ASN1_OCTET_STRING *str1 = ASN1_OCTET_STRING_new();
    ASN1_OCTET_STRING *str2 = ASN1_OCTET_STRING_new();
    
    if (str1 && str2) {
        // Use first half of data for str1, second half for str2
        size_t half = size / 2;
        if (half > 0) {
            ASN1_OCTET_STRING_set(str1, data, half);
            ASN1_OCTET_STRING_set(str2, data + half, size - half);
            ASN1_OCTET_STRING_cmp(str1, str2);
        }
    }
    
    if (str1) ASN1_OCTET_STRING_free(str1);
    if (str2) ASN1_OCTET_STRING_free(str2);
    if (name_entry) X509_NAME_ENTRY_free(name_entry);

    // 5. Create SSL context and check QUIC status
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    if (ctx) {
        SSL *ssl = SSL_new(ctx);
        if (ssl) {
            // Try to set up SSL for client hello inspection
            SSL_set_connect_state(ssl);
            
            // Get ciphers from client hello (will be NULL/0 as no handshake)
            const unsigned char *ciphers;
            size_t cipher_len = SSL_client_hello_get0_ciphers(ssl, &ciphers);
            
            // Check if SSL is QUIC
            SSL_is_quic(ssl);
            
            SSL_free(ssl);
        }
        SSL_CTX_free(ctx);
    }

    // 6. Create EVP_PKEY_CTX for scrypt and set N parameter
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_SCRYPT, NULL);
    if (pctx) {
        if (EVP_PKEY_derive_init(pctx) > 0) {
            // Use first 8 bytes of data as N value if available
            uint64_t n = 0;
            if (size >= sizeof(uint64_t)) {
                memcpy(&n, data, sizeof(uint64_t));
                // Ensure N is a reasonable value for scrypt
                if (n == 0) n = 1024;
                EVP_PKEY_CTX_set_scrypt_N(pctx, n);
            }
        }
        EVP_PKEY_CTX_free(pctx);
    }

    // 7. Create EC_KEY and check if decoded from explicit params
    EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (ec_key) {
        // Generate a key pair
        if (EC_KEY_generate_key(ec_key) == 1) {
            EC_KEY_decoded_from_explicit_params(ec_key);
        }
        EC_KEY_free(ec_key);
    }

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}