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
//<ID> 747
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 2
//<Quality> {"density":7,"unique_branches":{"X509_REVOKED_set_serialNumber":[[168,9,168,18,0,0,4,1],[171,9,171,21,0,0,4,0]]},"library_calls":["SCT_new","SCT_set0_signature","SSL_CTX_new","TLS_method","SSL_new","SSL_get_session","SSL_SESSION_has_ticket","TLS_FEATURE_new","TLS_FEATURE_free","X509_CRL_new","X509_REVOKED_new","ASN1_INTEGER_new","ASN1_INTEGER_set","X509_REVOKED_set_serialNumber","ASN1_INTEGER_free","X509_CRL_add0_revoked","X509_ACERT_new","X509_ACERT_get_attr","SCT_free","SSL_SESSION_free","SSL_free","SSL_CTX_free","X509_CRL_free","X509_REVOKED_free","X509_ACERT_free"],"critical_calls":["SCT_new","SCT_set0_signature","SSL_CTX_new","TLS_method","SSL_new","SSL_get_session","SSL_SESSION_has_ticket","TLS_FEATURE_new","TLS_FEATURE_free","X509_CRL_new","X509_REVOKED_new","ASN1_INTEGER_new","ASN1_INTEGER_set","X509_REVOKED_set_serialNumber","ASN1_INTEGER_free","X509_CRL_add0_revoked","X509_ACERT_new","X509_ACERT_get_attr","SCT_free","SSL_SESSION_free","SSL_free","SSL_CTX_free","X509_CRL_free","X509_ACERT_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Create and use SCT structure
    SCT *sct = SCT_new();
    if (sct) {
        // Use a portion of input data as signature
        size_t sig_len = size > 256 ? 256 : size;
        unsigned char *sig = (unsigned char *)malloc(sig_len);
        if (sig) {
            // Read signature data from input
            fread(sig, 1, sig_len, in_file);
            SCT_set0_signature(sct, sig, sig_len);
        }
    }

    // 2. Create SSL_SESSION and check for ticket
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = SSL_new(ctx);
    SSL_SESSION *session = NULL;
    
    if (ssl) {
        // Try to get session from SSL
        session = SSL_get_session(ssl);
        if (session) {
            int has_ticket = SSL_SESSION_has_ticket(session);
            // Use the result in some way
            (void)has_ticket;
        }
    }

    // 3. Create and use TLS_FEATURE
    TLS_FEATURE *tls_feature = TLS_FEATURE_new();
    if (tls_feature) {
        // We can't directly add data to TLS_FEATURE without additional APIs,
        // but we've created it as required
        TLS_FEATURE_free(tls_feature);
    }

    // 4. Create and populate X509_CRL with revoked certificates
    X509_CRL *crl = X509_CRL_new();
    X509_REVOKED *revoked = X509_REVOKED_new();
    
    if (crl && revoked) {
        // Set some basic fields in the revoked certificate
        ASN1_INTEGER *serial = ASN1_INTEGER_new();
        if (serial) {
            // Use part of input data for serial number
            if (size > 0) {
                ASN1_INTEGER_set(serial, (long)(*data));
            }
            X509_REVOKED_set_serialNumber(revoked, serial);
            ASN1_INTEGER_free(serial);
        }
        
        // Add revoked certificate to CRL
        X509_CRL_add0_revoked(crl, revoked);
    }

    // 5. Create X509_ACERT and get attributes
    X509_ACERT *acert = X509_ACERT_new();
    if (acert) {
        // Try to get an attribute (location 0)
        X509_ATTRIBUTE *attr = X509_ACERT_get_attr(acert, 0);
        // attr may be NULL if no attribute at that location, which is fine
        (void)attr;
    }

    // Cleanup: Release all allocated resources
    if (sct) {
        // Note: SCT_set0_signature transfers ownership, so we shouldn't free sig
        SCT_free(sct);
    }
    
    if (session) {
        SSL_SESSION_free(session);
    }
    
    if (ssl) {
        SSL_free(ssl);
    }
    
    if (ctx) {
        SSL_CTX_free(ctx);
    }
    
    if (crl) {
        // Note: X509_CRL_add0_revoked transfers ownership of revoked
        X509_CRL_free(crl);
    } else if (revoked) {
        // If crl was NULL but revoked was created, free it separately
        X509_REVOKED_free(revoked);
    }
    
    if (acert) {
        X509_ACERT_free(acert);
    }
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}