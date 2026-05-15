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
//<ID> 329
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 16
//<Quality> {"density":8,"unique_branches":{"mem_write":[[226,9,226,17,0,0,4,0]],"EVP_DecodeUpdate":[[618,29,618,32,0,0,4,0]],"mem_gets":[[356,9,356,41,0,0,4,0],[359,9,359,29,0,0,4,0],[360,9,360,23,0,0,4,0],[360,9,360,23,0,0,4,1],[362,9,362,15,0,0,4,0],[362,9,362,15,0,0,4,1],[367,17,367,22,0,0,4,0],[367,17,367,22,0,0,4,1],[368,13,368,25,0,0,4,0],[368,13,368,25,0,0,4,1],[380,9,380,14,0,0,4,0]],"mem_read":[[198,9,198,40,0,0,4,0]],"BIO_puts":[[539,9,539,16,0,0,4,1],[549,9,549,16,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","ERR_clear_error","BIO_new_mem_buf","UI_null","BIO_free","PEM_read_bio_EC_PUBKEY","BIO_free","BIO_new_mem_buf","EC_KEY_free","X509_CRL_new","X509_CRL_get0_extensions","OCSP_SINGLERESP_new","OCSP_SINGLERESP_get0_id","ossl_check_X509_LOOKUP_compfunc_type","BIO_new_fp","PEM_read_bio_X509","X509_free","PEM_read_bio_EC_PUBKEY","EC_KEY_free","BIO_free","EC_KEY_get0_group","EC_GROUP_get_curve_name","EC_KEY_get0_public_key","EC_KEY_get0_private_key","BIO_new_fp","PEM_write_bio_EC_PUBKEY","BIO_free","EC_KEY_free","BIO_free","X509_CRL_free","OCSP_SINGLERESP_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_crypto","ERR_clear_error","BIO_new_mem_buf","UI_null","PEM_read_bio_EC_PUBKEY","BIO_free","BIO_new_mem_buf","X509_CRL_new","X509_CRL_get0_extensions","OCSP_SINGLERESP_new","OCSP_SINGLERESP_get0_id","ossl_check_X509_LOOKUP_compfunc_type","BIO_new_fp","PEM_read_bio_X509","X509_free","PEM_read_bio_EC_PUBKEY","EC_KEY_free","BIO_free","EC_KEY_get0_group","EC_GROUP_get_curve_name","EC_KEY_get0_public_key","EC_KEY_get0_private_key","BIO_new_fp","PEM_write_bio_EC_PUBKEY","BIO_free","EC_KEY_free","BIO_free","X509_CRL_free","OCSP_SINGLERESP_free","ERR_clear_error"],"visited":0}
/*Looking at the provided APIs, I can create a fuzz driver that simulates parsing an EC public key from PEM data, examining certificate revocation information, and using UI callbacks. Here's a complete implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL if needed
    OpenSSL_add_all_algorithms();
    ERR_clear_error();

    // Create a BIO from the input data
    BIO* bio = BIO_new_mem_buf(data, size);
    if (!bio) {
        return 0;
    }

    // 1. Use UI_null() for password callback
    const UI_METHOD* ui_method = UI_null();
    if (!ui_method) {
        BIO_free(bio);
        return 0;
    }

    // 2. Read EC public key from PEM data
    EC_KEY* ec_key = nullptr;
    ec_key = PEM_read_bio_EC_PUBKEY(bio, &ec_key, nullptr, nullptr);
    
    // Reset BIO position for potential reuse
    BIO_free(bio);
    bio = BIO_new_mem_buf(data, size);
    if (!bio) {
        if (ec_key) EC_KEY_free(ec_key);
        return 0;
    }

    // 3. Create a dummy X509_CRL and get its extensions
    X509_CRL* crl = X509_CRL_new();
    if (crl) {
        const struct stack_st_X509_EXTENSION* extensions = X509_CRL_get0_extensions(crl);
        // Just reference it to ensure the API is called
        (void)extensions;
    }

    // 4. Create a dummy OCSP_SINGLERESP and get its cert ID
    OCSP_SINGLERESP* single_resp = OCSP_SINGLERESP_new();
    if (single_resp) {
        const OCSP_CERTID* cert_id = OCSP_SINGLERESP_get0_id(single_resp);
        // Just reference it to ensure the API is called
        (void)cert_id;
    }

    // 5. Use the X509_LOOKUP comparison function casting
    // Create a dummy comparison function
    auto dummy_cmp = [](const X509_LOOKUP* const* a, const X509_LOOKUP* const* b) -> int {
        return 0;
    };
    
    // Cast the comparison function using the provided API
    OPENSSL_sk_compfunc cmp_func = ossl_check_X509_LOOKUP_compfunc_type(dummy_cmp);
    // Just reference it to ensure the API is called
    (void)cmp_func;

    // 6. Try to parse the input as various formats using FILE* operations
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Try to read as PEM using FILE operations
        BIO* file_bio = BIO_new_fp(in_file, BIO_NOCLOSE);
        if (file_bio) {
            // Try reading different formats
            X509* x509 = PEM_read_bio_X509(file_bio, nullptr, nullptr, nullptr);
            if (x509) {
                X509_free(x509);
            }
            
            // Try reading EC key again
            EC_KEY* ec_key2 = PEM_read_bio_EC_PUBKEY(file_bio, nullptr, nullptr, nullptr);
            if (ec_key2) {
                EC_KEY_free(ec_key2);
            }
            
            BIO_free(file_bio);
        }
        assert_file_closed(&in_file);;
    }

    // 7. Create output file and write some debug info
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write whether we successfully parsed an EC key
        if (ec_key) {
            fprintf(out_file, "Successfully parsed EC key\n");
            
            // Get the group from EC key
            const EC_GROUP* group = EC_KEY_get0_group(ec_key);
            if (group) {
                // Write some info about the curve
                int nid = EC_GROUP_get_curve_name(group);
                if (nid != NID_undef) {
                    fprintf(out_file, "Curve NID: %d\n", nid);
                }
            }
        } else {
            fprintf(out_file, "Failed to parse EC key\n");
        }
        
        // Write input size
        fprintf(out_file, "Input size: %zu\n", size);
        
        assert_file_closed(&out_file);;
    }

    // 8. Try using file descriptor operations
    if (out_file) {
        int fd = fuzz_fileno(out_file);
        (void)fd; // Mark as used
    }

    // 9. Try more operations with the parsed data
    if (ec_key) {
        // Check if key has public components
        const EC_POINT* pub_point = EC_KEY_get0_public_key(ec_key);
        const BIGNUM* priv_key = EC_KEY_get0_private_key(ec_key);
        
        // Create a new BIO to test writing
        BIO* out_bio = BIO_new_fp(stdout, BIO_NOCLOSE);
        if (out_bio && pub_point) {
            // Try to write the public key
            PEM_write_bio_EC_PUBKEY(out_bio, ec_key);
            BIO_free(out_bio);
        }
    }

    // Clean up all allocated resources
    if (ec_key) {
        EC_KEY_free(ec_key);
    }
    if (bio) {
        BIO_free(bio);
    }
    if (crl) {
        X509_CRL_free(crl);
    }
    if (single_resp) {
        OCSP_SINGLERESP_free(single_resp);
    }

    // Clean up OpenSSL error stack
    ERR_clear_error();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}