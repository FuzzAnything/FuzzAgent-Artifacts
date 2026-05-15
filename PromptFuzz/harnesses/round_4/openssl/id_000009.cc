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
//<ID> 78
//<Prompt> []
/*<Combination>: [
*/
//<score> 76.5, nr_unique_branch: 32
//<Quality> {"density":9,"unique_branches":{"evp_pkey_asn1_find_str":[[91,16,91,66,0,0,4,0]],"ec_query_operation_name":[[86,13,86,25,0,0,4,1],[87,5,87,25,0,0,4,0],[89,5,89,27,0,0,4,1]],"EC_GROUP_free":[[125,9,125,15,0,0,4,0]],"ecdh_init":[[116,9,116,32,0,0,4,1],[117,12,117,28,0,0,4,1],[118,12,118,25,0,0,4,1],[119,12,119,46,0,0,4,0]],"EC_KEY_free":[[84,9,84,17,0,0,4,1]],"ecdh_newctx":[[91,9,91,32,0,0,4,1],[101,9,101,22,0,0,4,1]],"EVP_PKEY_derive_set_peer_ex":[[388,9,388,47,0,0,4,1],[393,9,393,22,0,0,4,1],[416,9,416,28,0,0,4,1],[426,9,426,24,0,0,4,0]],"pkey_set_type":[[1538,9,1538,21,0,0,4,1],[1567,17,1567,38,0,0,4,0]],"evp_keymgmt_util_query_operation_name":[[583,13,583,50,0,0,4,0],[585,13,585,25,0,0,4,1]],"evp_keymgmt_newdata":[[381,9,381,32,0,0,4,1]],"COMP_get_name":[[49,9,49,21,0,0,4,0]],"EVP_PKEY_set_type_by_keymgmt":[[1604,31,1604,45,4,0,4,1]],"EVP_PKEY_derive_init_ex":[[362,12,362,15,0,0,4,1]],"evp_pkey_export_to_provider":[[1822,22,1822,41,0,0,4,0],[1823,9,1823,14,0,0,4,0]],"ec_newdata_ex":[[290,9,290,32,0,0,4,1],[296,9,296,23,0,0,4,1],[304,9,304,18,0,0,4,1],[316,9,316,22,0,0,4,0]],"find_ameth":[[1588,9,1589,18,0,0,4,0],[1590,13,1590,27,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","TLS_method","SSL_CTX_new","SSL_new","SSL_CTX_free","SSL_set_default_read_buffer_len","SSL_new_listener_from","SSL_free","TS_TST_INFO_new","TS_TST_INFO_free","d2i_OCSP_CERTID","OCSP_id_get0_info","OCSP_CERTID_free","EVP_PKEY_CTX_new_id","EVP_PKEY_derive_init","EVP_PKEY_new","EVP_PKEY_new","EVP_PKEY_derive_set_peer_ex","EVP_PKEY_free","EVP_PKEY_free","EVP_PKEY_CTX_free","COMP_get_name","ossl_check_const_IPAddressOrRange_sk_type","ossl_check_OCSP_SINGLERESP_compfunc_type","ERR_peek_last_error_all","SSL_free","SSL_CTX_free"],"critical_calls":["OPENSSL_init_ssl","TLS_method","SSL_CTX_new","SSL_new","SSL_set_default_read_buffer_len","SSL_new_listener_from","SSL_free","TS_TST_INFO_new","TS_TST_INFO_free","d2i_OCSP_CERTID","OCSP_id_get0_info","OCSP_CERTID_free","EVP_PKEY_CTX_new_id","EVP_PKEY_derive_init","EVP_PKEY_new","EVP_PKEY_new","EVP_PKEY_derive_set_peer_ex","EVP_PKEY_free","EVP_PKEY_free","EVP_PKEY_CTX_free","COMP_get_name","ossl_check_const_IPAddressOrRange_sk_type","ossl_check_OCSP_SINGLERESP_compfunc_type","ERR_peek_last_error_all","SSL_free","SSL_CTX_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL library
    OPENSSL_init_ssl(0, NULL);
    
    // Create SSL context and SSL object
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx) {
	return 0;
	}
	
    
    SSL *ssl = SSL_new(ctx);
    if (!ssl) {
        SSL_CTX_free(ctx);
        return 0;
    }
    
    // Set default read buffer length
    SSL_set_default_read_buffer_len(ssl, 1024);
    
    // Create listener from SSL (though typically used for server side)
    SSL *listener = SSL_new_listener_from(ssl, 0);
    if (listener) {
        SSL_free(listener);
    }
    
    // Create and free TS_TST_INFO object
    TS_TST_INFO *ts_info = TS_TST_INFO_new();
    if (ts_info) {
        TS_TST_INFO_free(ts_info);
    }
    
    // Try to parse input as OCSP_CERTID
    OCSP_CERTID *cert_id = NULL;
    const uint8_t *p = data;
    cert_id = d2i_OCSP_CERTID(NULL, &p, size);
    
    if (cert_id) {
        // Get OCSP certificate ID information
        ASN1_OCTET_STRING *nameHash = NULL;
        ASN1_OBJECT *md = NULL;
        ASN1_OCTET_STRING *keyHash = NULL;
        ASN1_INTEGER *serial = NULL;
        
        OCSP_id_get0_info(&nameHash, &md, &keyHash, &serial, cert_id);
        
        // Free the certificate ID
        OCSP_CERTID_free(cert_id);
    }
    
    // Create EVP_PKEY context for key derivation
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
    if (pctx) {
        EVP_PKEY_derive_init(pctx);
        
        // Create temporary EVP_PKEY objects
        EVP_PKEY *pkey = EVP_PKEY_new();
        EVP_PKEY *peer_pkey = EVP_PKEY_new();
        
        if (pkey && peer_pkey) {
            // Try to set peer (this will likely fail but exercises the API)
            EVP_PKEY_derive_set_peer_ex(pctx, peer_pkey, 0);
        }
        
        // Clean up
        if (pkey) EVP_PKEY_free(pkey);
        if (peer_pkey) EVP_PKEY_free(peer_pkey);
        EVP_PKEY_CTX_free(pctx);
    }
    
    // Get compression method name (NULL is valid input)
    COMP_get_name(NULL);
    
    // Type checking functions (these are typically used internally)
    const struct stack_st_IPAddressOrRange *ip_stack = NULL;
    ossl_check_const_IPAddressOrRange_sk_type(ip_stack);
    
    sk_OCSP_SINGLERESP_compfunc cmp_func = NULL;
    ossl_check_OCSP_SINGLERESP_compfunc_type(cmp_func);
    
    // Check for any accumulated errors
    const char *file, *func, *data_str;
    int line, flags;
    ERR_peek_last_error_all(&file, &line, &func, &data_str, &flags);
    
    // Clean up SSL objects
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    
    return 0;
}