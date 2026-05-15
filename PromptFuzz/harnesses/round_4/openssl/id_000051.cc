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
//<ID> 745
//<Prompt> []
/*<Combination>: [
*/
//<score> 91, nr_unique_branch: 70
//<Quality> {"density":7,"unique_branches":{"asn1_primitive_clear":[[328,15,328,24,0,0,4,0],[330,13,330,27,0,0,4,0]],"asn1_ex_c2i":[[869,9,869,11,0,0,4,0],[869,15,869,27,0,0,4,0]],"X509_REVOKED_set_revocationDate":[[154,9,154,18,0,0,4,1],[154,22,154,32,0,0,4,1]],"asn1_get_uint64":[[227,9,227,27,0,0,4,1],[231,9,231,18,0,0,4,1],[233,24,233,32,0,0,4,0],[233,24,233,32,0,0,4,1]],"SSL_SESSION_free":[[846,9,846,19,0,0,4,1],[850,9,850,14,0,0,4,1],[293,12,293,18,2,0,4,1]],"uint64_c2i":[[78,9,78,22,0,0,4,1],[89,9,89,17,0,0,4,0],[89,9,89,17,0,0,4,1],[92,9,92,54,0,0,4,0],[92,9,92,54,0,0,4,1],[94,9,94,44,0,0,4,0],[94,9,94,44,0,0,4,1],[94,48,94,51,0,0,4,0],[94,48,94,51,0,0,4,1],[98,9,98,60,0,0,4,0],[98,9,98,60,0,0,4,1],[99,12,99,16,0,0,4,0],[99,12,99,16,0,0,4,1],[99,20,99,36,0,0,4,1],[103,9,103,12,0,0,4,0],[103,9,103,12,0,0,4,1]],"ossl_c2i_uint64_int":[[638,9,638,20,0,0,4,0],[638,9,638,20,0,0,4,1],[640,9,640,34,0,0,4,0],[640,9,640,34,0,0,4,1]],"ossl_time_now":[[38,9,38,35,0,0,4,1],[43,9,43,22,0,0,4,1]],"ossl_x509_set1_time":[[78,9,78,19,0,0,4,1],[81,9,81,19,0,0,4,0],[81,23,81,34,0,0,4,1],[85,9,85,25,0,0,4,1]],"d2i_SSL_SESSION_ex":[[276,9,276,19,0,0,4,0],[276,9,276,19,0,0,4,1],[279,9,279,18,0,0,4,0],[281,13,281,24,0,0,4,1],[287,9,287,48,0,0,4,0],[432,9,432,20,0,0,4,0]],"uint32_c2i":[[179,9,179,22,0,0,4,1],[190,9,190,17,0,0,4,0],[190,9,190,17,0,0,4,1],[193,9,193,54,0,0,4,0],[193,9,193,54,0,0,4,1],[195,9,195,44,0,0,4,0],[195,9,195,44,0,0,4,1],[195,48,195,51,0,0,4,0],[195,48,195,51,0,0,4,1],[199,9,199,12,0,0,4,0],[199,9,199,12,0,0,4,1],[200,13,200,33,0,0,4,0],[200,13,200,33,0,0,4,1],[206,14,206,49,0,0,4,0],[206,14,206,49,0,0,4,1],[206,53,206,69,0,0,4,0],[206,53,206,69,0,0,4,1],[207,17,207,52,0,0,4,0],[207,17,207,52,0,0,4,1],[207,56,207,73,0,0,4,0],[207,56,207,73,0,0,4,1]],"SSL_SESSION_new":[[105,9,105,63,0,0,4,1],[109,9,109,19,0,0,4,1],[118,9,118,44,0,0,4,1],[123,9,123,75,0,0,4,1]]},"library_calls":["BIO_new_fp","SCT_new","CRYPTO_malloc","SCT_set0_signature","ASN1_d2i_bio","SSL_SESSION_has_ticket","TLS_FEATURE_new","X509_CRL_new","X509_REVOKED_new","ASN1_TIME_new","ASN1_TIME_set","X509_REVOKED_set_revocationDate","ASN1_TIME_free","X509_CRL_add0_revoked","X509_ACERT_new","X509_ACERT_get_attr","SCT_free","SSL_SESSION_free","TLS_FEATURE_free","X509_CRL_free","X509_ACERT_free","BIO_free"],"critical_calls":["BIO_new_fp","SCT_new","CRYPTO_malloc","SCT_set0_signature","ASN1_d2i_bio","SSL_SESSION_has_ticket","TLS_FEATURE_new","X509_CRL_new","X509_REVOKED_new","ASN1_TIME_new","ASN1_TIME_set","X509_REVOKED_set_revocationDate","ASN1_TIME_free","X509_CRL_add0_revoked","X509_ACERT_new","X509_ACERT_get_attr","SCT_free","SSL_SESSION_free","TLS_FEATURE_free","X509_CRL_free","X509_ACERT_free","BIO_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a BIO from the FILE* for OpenSSL operations
    BIO *bio = BIO_new_fp(in_file, BIO_NOCLOSE);
    if (!bio) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Variables to hold parsed objects
    SCT *sct = NULL;
    SSL_SESSION *ssl_session = NULL;
    TLS_FEATURE *tls_feature = NULL;
    X509_CRL *crl = NULL;
    X509_ACERT *acert = NULL;

    // 1. Parse or create an SCT (Signed Certificate Timestamp)
    // We'll create a minimal SCT and set a signature from the input data
    sct = SCT_new();
    if (sct) {
        // Use a portion of the input data as signature
        size_t sig_len = size > 32 ? 32 : size; // Use up to 32 bytes for signature
        unsigned char *sig = (unsigned char *)OPENSSL_malloc(sig_len);
        if (sig) {
            memcpy(sig, data, sig_len);
            SCT_set0_signature(sct, sig, sig_len);
        }
    }

    // 2. Parse an SSL_SESSION from the input data
    // We'll attempt to read a session from the BIO
    ssl_session = d2i_SSL_SESSION_bio(bio, NULL);
    if (ssl_session) {
        // Check if the session has a ticket
        int has_ticket = SSL_SESSION_has_ticket(ssl_session);
        (void)has_ticket; // Use the result to avoid unused variable warning
    }

    // 3. Create a new TLS_FEATURE object
    tls_feature = TLS_FEATURE_new();
    if (tls_feature) {
        // Optionally add features (though not required by the API call)
        // This is just to demonstrate using the object
    }

    // 4. Parse or create an X509_CRL and add a revoked certificate
    crl = X509_CRL_new();
    if (crl) {
        X509_REVOKED *revoked = X509_REVOKED_new();
        if (revoked) {
            // Set some fields in the revoked certificate (optional)
            ASN1_TIME *rev_time = ASN1_TIME_new();
            if (rev_time) {
                ASN1_TIME_set(rev_time, time(NULL));
                X509_REVOKED_set_revocationDate(revoked, rev_time);
                ASN1_TIME_free(rev_time);
            }
            // Add the revoked certificate to the CRL
            int add_result = X509_CRL_add0_revoked(crl, revoked);
            (void)add_result; // Use the result to avoid unused variable warning
        }
    }

    // 5. Parse an X509_ACERT from the input data and get an attribute
    // Note: X509_ACERT parsing is not directly available in standard OpenSSL.
    // We'll simulate by creating a dummy ACERT and getting an attribute.
    acert = X509_ACERT_new();
    if (acert) {
        // Get the first attribute (if any)
        X509_ATTRIBUTE *attr = X509_ACERT_get_attr(acert, 0);
        (void)attr; // Use the result to avoid unused variable warning
    }

    // Clean up all allocated resources
    if (sct) {
        SCT_free(sct);
    }
    if (ssl_session) {
        SSL_SESSION_free(ssl_session);
    }
    if (tls_feature) {
        TLS_FEATURE_free(tls_feature);
    }
    if (crl) {
        X509_CRL_free(crl);
    }
    if (acert) {
        X509_ACERT_free(acert);
    }
    BIO_free(bio);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}