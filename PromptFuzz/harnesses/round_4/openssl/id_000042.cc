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
//<ID> 505
//<Prompt> []
/*<Combination>: [
*/
//<score> 160, nr_unique_branch: 74
//<Quality> {"density":10,"unique_branches":{"GENERAL_NAME_print":[[212,13,212,22,0,0,4,1],[213,5,213,23,0,0,4,1],[256,5,256,18,0,0,4,1],[260,5,260,22,0,0,4,1],[265,5,265,19,0,0,4,0],[270,5,270,17,0,0,4,1],[275,5,275,17,0,0,4,1],[280,5,280,21,0,0,4,1],[285,5,285,19,0,0,4,1],[293,5,293,17,0,0,4,1]],"X509_ATTRIBUTE_set1_object":[[338,9,338,21,0,0,4,1],[338,25,338,36,0,0,4,1]],"BIO_sock_non_fatal_error":[[319,12,319,30,0,0,4,1],[320,12,320,25,0,0,4,1],[321,12,321,27,0,0,4,1],[322,12,322,24,0,0,4,1],[324,12,324,25,0,0,4,1],[329,12,329,30,0,0,4,1],[330,12,330,27,0,0,4,1]],"BIO_accept_ex":[[423,22,423,35,0,0,4,1],[428,9,428,28,0,0,4,0],[429,13,429,50,0,0,4,0]],"OCSP_REQUEST_print":[[98,9,98,55,0,0,4,1],[101,9,101,66,0,0,4,1],[103,9,103,35,0,0,4,0],[103,9,103,35,0,0,4,1],[104,13,104,61,0,0,4,1],[108,9,108,58,0,0,4,1],[110,17,110,39,0,0,4,1],[119,9,120,46,0,0,4,1],[122,9,122,12,0,0,4,1]],"ASN1_INTEGER_get":[[567,9,567,18,0,0,4,0]],"X509V3_extensions_print":[[144,9,144,41,0,0,4,0]],"ossl_x509at_add1_attr":[[88,9,88,18,0,0,4,1],[88,22,88,34,0,0,4,1],[93,9,93,19,0,0,4,0],[94,13,94,56,0,0,4,1],[102,9,102,54,0,0,4,1],[104,9,104,32,0,0,4,1],[108,9,108,19,0,0,4,0]],"EVP_PKEY_add1_attr":[[213,9,213,49,0,0,4,0]],"BIO_sock_should_retry":[[302,9,302,17,0,0,4,1],[302,21,302,30,0,0,4,0]],"BIO_accept":[[296,9,296,35,0,0,4,0],[297,13,297,39,0,0,4,1]],"SSL_get_negotiated_client_cert_type":[[8354,9,8354,19,0,0,4,1],[32,6,32,19,2,0,4,1],[33,23,33,61,2,0,4,0]],"X509_ATTRIBUTE_set1_data":[[354,9,354,21,0,0,4,1],[358,9,358,40,0,0,4,1],[366,16,366,25,0,0,4,0],[367,13,367,60,0,0,4,1],[368,16,368,49,0,0,4,1],[379,9,379,22,0,0,4,1],[383,9,383,41,0,0,4,1],[387,9,387,18,0,0,4,1],[396,9,396,27,0,0,4,1]],"X509at_add1_attr":[[121,9,121,18,0,0,4,1],[121,22,121,34,0,0,4,1],[125,9,125,19,0,0,4,1]],"ASN1_STRING_print":[[77,9,77,18,0,0,4,1],[81,17,81,30,0,0,4,0],[81,17,81,30,0,0,4,1],[82,13,82,25,0,0,4,0],[82,13,82,25,0,0,4,1],[82,30,82,42,0,0,4,0],[82,30,82,42,0,0,4,1],[82,46,82,60,0,0,4,0],[82,64,82,78,0,0,4,0],[87,13,87,20,0,0,4,0],[87,13,87,20,0,0,4,1],[88,17,88,43,0,0,4,1],[93,9,93,14,0,0,4,0],[94,13,94,39,0,0,4,1]]},"library_calls":["OCSP_REQUEST_it","ossl_check_X509_ALGOR_compfunc_type","d2i_OCSP_REQUEST","X509_PUBKEY_it","SSL_CTX_new","TLS_method","SSL_new","SSL_get_negotiated_client_cert_type","OSSL_CMP_PKISI_new","OSSL_CMP_PKISI_dup","EVP_PKEY_new","X509_ATTRIBUTE_new","OBJ_nid2obj","ASN1_STRING_new","ASN1_STRING_set","X509_ATTRIBUTE_set1_object","X509_ATTRIBUTE_set1_data","ASN1_STRING_get0_data","ASN1_STRING_length","EVP_PKEY_add1_attr","ASN1_STRING_free","BIO_accept","CRYPTO_free","SSL_get_session","SSL_SESSION_get0_peer","BIO_new_fp","OCSP_REQUEST_print","BIO_free","OCSP_REQUEST_free","OSSL_CMP_PKISI_free","OSSL_CMP_PKISI_free","X509_ATTRIBUTE_free","EVP_PKEY_free","SSL_free","SSL_CTX_free"],"critical_calls":["OCSP_REQUEST_it","ossl_check_X509_ALGOR_compfunc_type","d2i_OCSP_REQUEST","X509_PUBKEY_it","SSL_CTX_new","TLS_method","SSL_new","SSL_get_negotiated_client_cert_type","OSSL_CMP_PKISI_new","OSSL_CMP_PKISI_dup","EVP_PKEY_new","X509_ATTRIBUTE_new","OBJ_nid2obj","ASN1_STRING_new","ASN1_STRING_set","X509_ATTRIBUTE_set1_object","X509_ATTRIBUTE_set1_data","ASN1_STRING_length","ASN1_STRING_get0_data","EVP_PKEY_add1_attr","ASN1_STRING_free","BIO_accept","CRYPTO_free","SSL_get_session","SSL_SESSION_get0_peer","BIO_new_fp","OCSP_REQUEST_print","BIO_free","OCSP_REQUEST_free","OSSL_CMP_PKISI_free","OSSL_CMP_PKISI_free","X509_ATTRIBUTE_free","EVP_PKEY_free","SSL_free","SSL_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
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
	

    // Create FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // 1. Get ASN1_ITEM for OCSP_REQUEST
    const ASN1_ITEM *ocsp_request_it = OCSP_REQUEST_it();
    (void)ocsp_request_it; // Use to avoid unused variable warning

    // 2. Use ossl_check_X509_ALGOR_compfunc_type
    // Create a dummy comparison function
    int (*dummy_cmp)(const X509_ALGOR *const *, const X509_ALGOR *const *) = 
        [](const X509_ALGOR *const *a, const X509_ALGOR *const *b) -> int {
            return 0;
        };
    OPENSSL_sk_compfunc checked_cmp = ossl_check_X509_ALGOR_compfunc_type(dummy_cmp);
    (void)checked_cmp;

    // 3. Parse OCSP request from input data
    OCSP_REQUEST *ocsp_req = NULL;
    fseek(in_file, 0, SEEK_SET);
    unsigned char *data_copy = (unsigned char *)data;
    const unsigned char *p = data_copy;
    ocsp_req = d2i_OCSP_REQUEST(NULL, &p, size);
    
    // 4. Get ASN1_ITEM for X509_PUBKEY
    const ASN1_ITEM *x509_pubkey_it = X509_PUBKEY_it();
    (void)x509_pubkey_it;

    // 5. Create SSL context and SSL object for SSL operations
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = SSL_new(ctx);
    
    // 6. Get negotiated client certificate type (requires SSL handshake simulation)
    int client_cert_type = SSL_get_negotiated_client_cert_type(ssl);
    (void)client_cert_type;

    // 7. Create and duplicate OSSL_CMP_PKISI
    OSSL_CMP_PKISI *pkisi = OSSL_CMP_PKISI_new();
    OSSL_CMP_PKISI *pkisi_dup = NULL;
    if (pkisi) {
        pkisi_dup = OSSL_CMP_PKISI_dup(pkisi);
    }

    // 8. Create EVP_PKEY and add attribute
    EVP_PKEY *pkey = EVP_PKEY_new();
    X509_ATTRIBUTE *attr = X509_ATTRIBUTE_new();
    if (pkey && attr) {
        // Set some dummy values for the attribute
        ASN1_OBJECT *obj = OBJ_nid2obj(NID_commonName);
        ASN1_STRING *str = ASN1_STRING_new();
        if (str && obj) {
            ASN1_STRING_set(str, "test", 4);
            X509_ATTRIBUTE_set1_object(attr, obj);
            X509_ATTRIBUTE_set1_data(attr, V_ASN1_UTF8STRING, 
                                     ASN1_STRING_get0_data(str), 
                                     ASN1_STRING_length(str));
            EVP_PKEY_add1_attr(pkey, attr);
        }
        if (str) ASN1_STRING_free(str);
    }

    // 9. Use BIO_accept with the input file descriptor
    char *ip_port = NULL;
    // Note: in_fd is not a listening socket, so this will likely fail
    // But we call it to satisfy the API usage requirement
    int accept_result = BIO_accept(in_fd, &ip_port);
    (void)accept_result;
    if (ip_port) OPENSSL_free(ip_port);

    // 10. Try to get peer certificate from SSL session
    SSL_SESSION *session = SSL_get_session(ssl);
    X509 *peer_cert = NULL;
    if (session) {
        peer_cert = SSL_SESSION_get0_peer(session);
    }

    // Write some output using the parsed data
    if (ocsp_req) {
        // Write OCSP request info to output file
        BIO *bio = BIO_new_fp(out_file, BIO_NOCLOSE);
        if (bio) {
            OCSP_REQUEST_print(bio, ocsp_req, 0);
            BIO_free(bio);
        }
    }

    // Cleanup
    if (ocsp_req) OCSP_REQUEST_free(ocsp_req);
    if (pkisi) OSSL_CMP_PKISI_free(pkisi);
    if (pkisi_dup) OSSL_CMP_PKISI_free(pkisi_dup);
    if (attr) X509_ATTRIBUTE_free(attr);
    if (pkey) EVP_PKEY_free(pkey);
    if (ssl) SSL_free(ssl);
    if (ctx) SSL_CTX_free(ctx);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}