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
//<ID> 424
//<Prompt> []
/*<Combination>: [
*/
//<score> 180, nr_unique_branch: 51
//<Quality> {"density":10,"unique_branches":{"X509_ATTRIBUTE_set1_object":[[338,9,338,21,0,0,4,1],[338,25,338,36,0,0,4,1]],"SSL_client_hello_get_extension_order":[[7090,9,7090,19,0,0,4,1],[7093,9,7093,32,0,0,4,0],[32,6,32,19,2,0,4,1],[33,23,33,61,2,0,4,0]],"ossl_asn1_do_adb":[[246,9,246,22,0,0,4,0],[247,13,247,33,0,0,4,0],[287,9,287,16,0,0,4,1]],"PKCS12_add_CSPName_asc":[[76,9,78,16,0,0,4,0]],"ossl_x509at_add1_attr_by_NID":[[180,9,180,21,0,0,4,1]],"SSL_get_stream_type":[[96,6,96,19,1,0,4,0],[1242,29,1242,48,3,0,4,1]],"ASN1_mbstring_ncopy":[[141,14,141,37,0,0,4,0],[153,9,153,13,0,0,4,1],[161,13,161,25,0,0,4,1],[187,5,187,22,0,0,4,0],[197,5,197,23,0,0,4,1]],"asn1_ex_i2c":[[562,13,562,25,0,0,4,0],[615,5,615,26,0,0,4,0]],"X509at_add1_attr_by_NID":[[193,9,193,18,0,0,4,1],[197,9,197,19,0,0,4,1]],"type_str":[[329,9,329,35,0,0,4,0],[329,39,329,55,0,0,4,1],[331,9,331,36,0,0,4,1]],"ossl_x509at_add1_attr":[[88,9,88,18,0,0,4,1],[88,22,88,34,0,0,4,1],[93,9,93,19,0,0,4,0],[94,13,94,56,0,0,4,1],[102,9,102,54,0,0,4,1],[104,9,104,32,0,0,4,1],[108,9,108,19,0,0,4,0]],"ossl_asn1_item_embed_free":[[111,17,111,23,0,0,4,0]],"X509_ATTRIBUTE_create_by_OBJ":[[295,9,295,21,0,0,4,0],[296,13,296,49,0,0,4,1],[304,9,304,46,0,0,4,1],[306,9,306,59,0,0,4,1],[309,9,309,21,0,0,4,1]],"ASN1_STRING_set_by_NID":[[88,9,88,20,0,0,4,0],[93,13,93,43,0,0,4,1]],"X509_ATTRIBUTE_create_by_NID":[[278,9,278,20,0,0,4,1],[283,9,283,20,0,0,4,1]],"asn1_template_noexp_d2i":[[643,13,643,25,0,0,4,1],[651,20,651,48,0,0,4,1]],"X509_ATTRIBUTE_set1_data":[[354,9,354,21,0,0,4,1],[358,9,358,40,0,0,4,0],[361,13,361,25,0,0,4,1],[379,9,379,22,0,0,4,1],[383,9,383,41,0,0,4,1],[387,9,387,18,0,0,4,1],[396,9,396,27,0,0,4,1]]},"library_calls":["SSL_CTX_new","TLS_method","SSL_new","SSL_free","SSL_CTX_free","SSL_CTX_set_recv_max_early_data","SSL_get_stream_type","PKCS12_SAFEBAG_new","PKCS12_add_CSPName_asc","SSL_client_hello_get_extension_order","ASN1_item_new","OSSL_ATAV_it","i2d_OSSL_ATAV","CRYPTO_free","ASN1_item_free","OSSL_ATAV_it","OCSP_ONEREQ_new","OCSP_ONEREQ_new","sk_OCSP_ONEREQ_cmpfunc_thunk","OCSP_ONEREQ_free","OCSP_ONEREQ_free","PKCS12_SAFEBAG_free","SSL_free","SSL_CTX_free"],"critical_calls":["SSL_CTX_new","TLS_method","SSL_new","SSL_CTX_set_recv_max_early_data","SSL_get_stream_type","PKCS12_SAFEBAG_new","PKCS12_add_CSPName_asc","SSL_client_hello_get_extension_order","ASN1_item_new","OSSL_ATAV_it","i2d_OSSL_ATAV","CRYPTO_free","ASN1_item_free","OSSL_ATAV_it","OCSP_ONEREQ_new","OCSP_ONEREQ_new","sk_OCSP_ONEREQ_cmpfunc_thunk","OCSP_ONEREQ_free","OCSP_ONEREQ_free","PKCS12_SAFEBAG_free","SSL_free","SSL_CTX_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create SSL_CTX and SSL objects
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = SSL_new(ctx);
    if (!ctx || !ssl) {
        if (ssl) SSL_free(ssl);
        if (ctx) SSL_CTX_free(ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Set receive max early data
    SSL_CTX_set_recv_max_early_data(ctx, 1024);
    
    // 4. Get stream type
    SSL_get_stream_type(ssl);
    
    // 5. Create PKCS12_SAFEBAG
    PKCS12_SAFEBAG *bag = PKCS12_SAFEBAG_new();
    if (bag) {
        // Add CSP name (use first 100 bytes or size if smaller)
        int namelen = size > 100 ? 100 : size;
        PKCS12_add_CSPName_asc(bag, (const char*)data, namelen);
    }
    
    // 6. Get client hello extension order (if in client hello state)
    uint16_t exts[10];
    size_t num_exts = 10;
    SSL_client_hello_get_extension_order(ssl, exts, &num_exts);
    
    // 7. Create and encode OSSL_ATAV structure
    OSSL_ATAV *atav = (OSSL_ATAV*)ASN1_item_new((ASN1_ITEM*)OSSL_ATAV_it());
    if (atav) {
        // Use input data for encoding
        unsigned char *out = NULL;
        int len = i2d_OSSL_ATAV(atav, &out);
        if (out) OPENSSL_free(out);
        ASN1_item_free((ASN1_VALUE*)atav, (ASN1_ITEM*)OSSL_ATAV_it());
    }
    
    // 8. Create OCSP_ONEREQ stack and use comparison function
    OCSP_ONEREQ *one1 = OCSP_ONEREQ_new();
    OCSP_ONEREQ *one2 = OCSP_ONEREQ_new();
    if (one1 && one2) {
        // Create comparison function thunk
        int (*cmp_func)(const void*, const void*) = [](const void *a, const void *b) -> int {
            return memcmp(a, b, sizeof(OCSP_ONEREQ*));
        };
        sk_OCSP_ONEREQ_cmpfunc_thunk(cmp_func, one1, one2);
    }
    
    // 9. Cleanup
    if (one1) OCSP_ONEREQ_free(one1);
    if (one2) OCSP_ONEREQ_free(one2);
    if (bag) PKCS12_SAFEBAG_free(bag);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}