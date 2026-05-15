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
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 296, nr_unique_branch: 422
//<Quality> {"density":4,"unique_branches":{"_CONF_add_string":[[53,9,53,28,0,0,4,1],[57,9,57,18,0,0,4,0],[57,9,57,18,0,0,4,1]],"OPENSSL_LH_strhash":[[366,9,366,20,0,0,4,0],[366,24,366,36,0,0,4,0]],"ASN1_item_ex_i2d":[[129,13,129,21,0,0,4,0],[129,25,129,41,0,0,4,0]],"NCONF_free_data":[[213,9,213,21,0,0,4,1]],"unreserve_loader_store":[[128,9,128,22,0,0,4,0],[129,12,129,64,0,0,4,1]],"pkey_cb":[[23,13,23,22,0,0,4,0],[23,13,23,22,0,0,4,1],[24,5,24,26,0,0,4,0],[24,5,24,26,0,0,4,1],[27,13,27,22,0,0,4,0],[30,5,30,26,0,0,4,1]],"OPENSSL_sk_delete_ptr":[[321,9,321,19,0,0,4,1],[324,17,324,28,0,0,4,0],[325,13,325,29,0,0,4,0],[325,13,325,29,0,0,4,1]],"CONF_set_nconf":[[28,9,28,36,0,0,4,0]],"eat_alpha_numeric":[[937,21,937,40,0,0,4,1],[33,32,33,61,1,0,4,0],[33,32,33,61,1,0,4,1],[53,29,53,53,2,0,4,0],[53,29,53,53,2,0,4,1],[37,32,37,69,3,0,4,0],[37,32,37,69,3,0,4,1]],"put_loader_in_store":[[167,9,167,66,0,0,4,1],[168,12,168,62,0,0,4,1],[171,9,171,22,0,0,4,0],[171,26,171,78,0,0,4,1]],"CT_POLICY_EVAL_CTX_free":[[60,9,60,20,0,0,4,1]],"TS_RESP_CTX_free":[[122,9,122,13,0,0,4,1]],"scan_quote":[[948,12,948,31,0,0,4,0],[948,12,948,31,0,0,4,1],[948,35,948,44,0,0,4,0],[948,35,948,44,0,0,4,1],[956,9,956,16,0,0,4,0],[956,9,956,16,0,0,4,1],[33,32,33,61,1,0,4,0],[33,32,33,61,1,0,4,1],[32,32,32,61,2,0,4,0],[32,32,32,61,2,0,4,1]],"OSSL_STORE_LOADER_up_ref":[[23,9,23,29,0,0,4,0]],"OPENSSL_sk_pop":[[443,9,443,19,0,0,4,0]],"deflt_query":[[769,5,769,23,0,0,4,0]],"get_loader_from_store":[[143,9,143,40,0,0,4,0],[149,9,149,22,0,0,4,0],[150,12,150,64,0,0,4,1],[153,9,153,80,0,0,4,1]],"ASN1_dup":[[23,9,23,18,0,0,4,1],[27,9,27,15,0,0,4,0],[27,9,27,15,0,0,4,1],[31,9,31,18,0,0,4,1]],"OPENSSL_LH_delete":[[156,9,156,20,0,0,4,1],[166,9,166,36,0,0,4,1]],"OPENSSL_sk_insert":[[286,13,286,29,0,0,4,0],[287,17,287,24,0,0,4,0],[289,21,289,32,0,0,4,1],[292,17,292,34,0,0,4,1]],"asn1_i2d_ex_primitive":[[499,9,499,18,0,0,4,1]],"value_free_hash":[[160,9,160,24,0,0,4,0],[160,9,160,24,0,0,4,1]],"eat_ws":[[911,31,911,50,0,0,4,0],[35,32,35,60,1,0,4,0],[35,32,35,60,1,0,4,1]],"clear_comments":[[611,13,611,19,0,0,4,0],[611,13,611,19,0,0,4,1],[31,32,31,66,1,0,4,1],[30,32,30,65,3,0,4,0],[30,32,30,65,3,0,4,1],[39,32,39,64,4,0,4,1],[38,32,38,63,5,0,4,0],[38,32,38,63,5,0,4,1],[33,32,33,61,6,0,4,0],[33,32,33,61,6,0,4,1],[53,29,53,53,7,0,4,0],[53,29,53,53,7,0,4,1],[32,32,32,61,8,0,4,0],[32,32,32,61,8,0,4,1]],"internal_delete":[[312,32,312,44,0,0,4,1]],"asn1_ex_i2c":[[608,5,608,26,0,0,4,0]],"SSL_client_hello_get0_random":[[6992,9,6992,19,0,0,4,1],[6995,9,6995,32,0,0,4,0],[32,6,32,19,3,0,4,1],[33,23,33,61,3,0,4,0]],"is_keytype":[[901,9,901,18,0,0,4,0],[901,9,901,18,0,0,4,1],[906,12,906,34,0,0,4,0],[906,12,906,34,0,0,4,1]],"stack_to_property_list":[[331,17,331,22,0,0,4,0],[331,26,331,68,0,0,4,1]],"ASN1_d2i_fp":[[27,9,27,44,0,0,4,1]],"BIO_ADDR_rawaddress":[[176,9,176,36,0,0,4,0],[193,9,193,24,0,0,4,1],[196,9,196,18,0,0,4,0],[196,9,196,18,0,0,4,1],[199,9,199,18,0,0,4,0]],"TS_CONF_set_ess_cert_id_digest":[[447,9,447,19,0,0,4,0],[451,9,451,24,0,0,4,1],[456,9,456,58,0,0,4,1]],"new_loader":[[50,9,50,59,0,0,4,1],[51,12,51,47,0,0,4,1],[52,12,52,39,0,0,4,1]],"BUF_MEM_grow":[[76,9,76,27,0,0,4,0],[80,9,80,24,0,0,4,0],[81,13,81,30,0,0,4,0]],"parse_unquoted":[[234,53,234,62,0,0,4,1],[241,30,241,40,0,0,4,0],[241,44,241,53,0,0,4,1]],"TS_RESP_CTX_new_ex":[[97,9,97,53,0,0,4,1],[100,9,100,22,0,0,4,1]],"EC_KEY_set_method":[[63,9,63,23,0,0,4,1],[67,9,67,27,0,0,4,1]],"_CONF_new_data":[[122,9,122,21,0,0,4,1],[124,9,124,27,0,0,4,0],[126,13,126,31,0,0,4,1]],"BIO_get_host_ip":[[50,9,50,29,0,0,4,1],[53,9,53,77,0,0,4,0],[56,13,56,48,0,0,4,1],[58,20,58,76,0,0,4,0],[52,24,53,24,2,0,4,0]],"OSSL_STORE_LOADER_settable_ctx_params":[[508,9,508,23,0,0,4,0],[508,27,508,64,0,0,4,0]],"def_load":[[177,9,177,19,0,0,4,1]],"ossl_init_async":[[207,9,207,22,0,0,4,1]],"def_load_bio":[[232,9,232,39,0,0,4,1],[238,9,238,24,0,0,4,1],[241,9,241,34,0,0,4,1],[247,9,247,19,0,0,4,1],[255,13,255,54,0,0,4,1],[262,13,262,23,0,0,4,0],[262,27,262,63,0,0,4,1],[266,13,266,23,0,0,4,0],[266,13,266,23,0,0,4,1],[271,17,271,23,0,0,4,0],[271,17,271,23,0,0,4,1],[271,27,271,54,0,0,4,0],[271,27,271,54,0,0,4,1],[279,13,279,19,0,0,4,0],[279,13,279,19,0,0,4,1],[279,23,279,29,0,0,4,0],[279,23,279,29,0,0,4,1],[285,17,285,31,0,0,4,1],[299,17,299,53,0,0,4,0],[309,16,309,21,0,0,4,0],[309,16,309,21,0,0,4,1],[310,17,310,35,0,0,4,0],[310,17,310,35,0,0,4,1],[310,39,310,57,0,0,4,0],[310,39,310,57,0,0,4,1],[318,13,318,15,0,0,4,0],[318,13,318,15,0,0,4,1],[318,19,318,26,0,0,4,0],[318,19,318,26,0,0,4,1],[332,13,332,19,0,0,4,0],[332,13,332,19,0,0,4,1],[332,23,332,34,0,0,4,0],[332,23,332,34,0,0,4,1],[338,40,338,53,0,0,4,0],[338,40,338,53,0,0,4,1],[338,57,338,64,0,0,4,0],[338,57,338,64,0,0,4,1],[343,13,343,18,0,0,4,0],[343,13,343,18,0,0,4,1],[352,13,352,22,0,0,4,0],[352,13,352,22,0,0,4,1],[361,17,361,26,0,0,4,0],[361,17,361,26,0,0,4,1],[362,21,362,31,0,0,4,0],[362,21,362,31,0,0,4,1],[362,35,362,42,0,0,4,0],[362,35,362,42,0,0,4,1],[370,17,370,55,0,0,4,1],[372,17,372,64,0,0,4,0],[372,17,372,64,0,0,4,1],[374,17,374,27,0,0,4,1],[382,17,382,32,0,0,4,0],[382,17,382,32,0,0,4,1],[382,36,382,51,0,0,4,0],[382,36,382,51,0,0,4,1],[393,21,393,31,0,0,4,0],[393,21,393,31,0,0,4,1],[393,35,393,44,0,0,4,0],[393,35,393,44,0,0,4,1],[396,21,396,30,0,0,4,0],[396,21,396,30,0,0,4,1],[404,21,404,33,0,0,4,0],[404,21,404,33,0,0,4,1],[404,37,404,46,0,0,4,0],[404,37,404,46,0,0,4,1],[404,50,404,65,0,0,4,0],[404,50,404,65,0,0,4,1],[420,21,420,47,0,0,4,1],[423,28,423,53,0,0,4,1],[426,28,426,56,0,0,4,1],[437,21,437,31,0,0,4,0],[437,21,437,31,0,0,4,1],[437,35,437,44,0,0,4,0],[437,35,437,44,0,0,4,1],[453,21,453,39,0,0,4,1],[457,21,457,40,0,0,4,0],[460,21,460,30,0,0,4,0],[460,21,460,30,0,0,4,1],[465,21,465,59,0,0,4,0],[465,21,465,59,0,0,4,1],[468,21,468,40,0,0,4,1],[486,21,486,39,0,0,4,1],[496,21,496,44,0,0,4,0],[505,21,505,33,0,0,4,1],[523,24,523,33,0,0,4,0],[523,24,523,33,0,0,4,1],[533,17,533,57,0,0,4,1],[537,17,537,32,0,0,4,1],[539,17,539,62,0,0,4,0],[539,17,539,62,0,0,4,1],[542,17,542,47,0,0,4,1],[553,17,553,51,0,0,4,1],[577,12,577,33,0,0,4,1],[585,9,585,23,0,0,4,1],[588,9,588,21,0,0,4,1],[592,9,592,24,0,0,4,0],[596,9,596,18,0,0,4,0],[596,9,596,18,0,0,4,1],[33,32,33,61,12,0,4,0],[33,32,33,61,12,0,4,1],[32,32,32,61,14,0,4,0],[32,32,32,61,14,0,4,1],[61,5,61,63,17,0,4,0],[61,5,61,63,17,0,4,1],[61,5,61,63,21,0,4,0],[61,5,61,63,21,0,4,1],[58,30,58,74,62,0,4,0],[58,30,58,74,62,0,4,1],[58,30,58,74,68,0,4,0],[58,30,58,74,68,0,4,1]],"str_copy":[[647,9,647,38,0,0,4,1],[650,9,650,45,0,0,4,1],[657,20,657,27,0,0,4,0],[657,20,657,27,0,0,4,1],[657,44,657,56,0,0,4,0],[657,44,657,56,0,0,4,1],[665,17,665,27,0,0,4,0],[665,17,665,27,0,0,4,1],[687,22,687,30,0,0,4,0],[687,22,687,30,0,0,4,1],[689,22,689,30,0,0,4,0],[689,22,689,30,0,0,4,1],[691,22,691,30,0,0,4,0],[691,22,691,30,0,0,4,1],[693,22,693,30,0,0,4,0],[693,22,693,30,0,0,4,1],[698,18,698,30,0,0,4,0],[698,18,698,30,0,0,4,1],[699,17,699,37,0,0,4,0],[707,17,707,26,0,0,4,1],[709,22,709,31,0,0,4,0],[709,22,709,31,0,0,4,1],[714,17,714,18,0,0,4,0],[714,17,714,18,0,0,4,1],[719,21,719,40,0,0,4,1],[721,17,721,30,0,0,4,0],[721,17,721,30,0,0,4,1],[721,34,721,47,0,0,4,0],[721,34,721,47,0,0,4,1],[729,25,729,44,0,0,4,1],[735,17,735,18,0,0,4,0],[735,17,735,18,0,0,4,1],[736,21,736,27,0,0,4,0],[753,17,753,28,0,0,4,0],[753,17,753,28,0,0,4,1],[756,17,756,26,0,0,4,0],[756,17,756,26,0,0,4,1],[761,17,761,48,0,0,4,1],[765,17,765,50,0,0,4,1],[769,20,769,22,0,0,4,0],[769,20,769,22,0,0,4,1],[38,32,38,63,1,0,4,0],[38,32,38,63,1,0,4,1],[33,32,33,61,2,0,4,0],[33,32,33,61,2,0,4,1],[32,32,32,61,3,0,4,0],[32,32,32,61,3,0,4,1],[39,32,39,64,5,0,4,1],[33,32,33,61,7,0,4,0],[33,32,33,61,7,0,4,1],[32,32,32,61,8,0,4,0],[32,32,32,61,8,0,4,1],[32,32,32,61,9,0,4,0],[32,32,32,61,9,0,4,1],[36,32,36,63,10,0,4,0],[36,32,36,63,10,0,4,1],[36,32,36,63,12,0,4,1]],"value_free_stack_doall":[[170,9,170,24,0,0,4,1],[174,41,174,47,0,0,4,0],[174,41,174,47,0,0,4,1]],"ossl_time_now":[[38,9,38,35,0,0,4,1],[43,9,43,22,0,0,4,1]],"OSSL_STORE_LOADER_free":[[30,9,30,23,0,0,4,0],[30,27,30,47,0,0,4,0],[34,13,34,18,0,0,4,0]],"pd_compare":[[293,9,293,38,0,0,4,0]],"BIO_ADDRINFO_family":[[389,9,389,20,0,0,4,0]],"process_include":[[805,9,805,31,0,0,4,0]],"OPENSSL_init_crypto":[[479,9,479,36,0,0,4,0],[480,12,480,21,0,0,4,1],[130,6,130,48,30,0,4,0]],"_CONF_free_data":[[142,9,142,27,0,0,4,1]],"CT_POLICY_EVAL_CTX_new_ex":[[34,9,34,20,0,0,4,1],[38,9,38,22,0,0,4,1]],"reserve_loader_store":[[117,9,117,22,0,0,4,0],[118,12,118,64,0,0,4,1]],"ERR_add_error_vdata":[[811,9,811,49,0,0,4,0],[837,13,837,24,0,0,4,0],[842,17,842,26,0,0,4,1],[52,24,53,24,3,0,4,0]],"BIO_ADDRINFO_free":[[455,9,455,20,0,0,4,1],[460,15,460,41,1,0,4,0]],"loader_from_algorithm":[[184,9,184,44,0,0,4,1],[190,12,190,33,0,0,4,0],[190,12,190,33,0,0,4,1],[191,17,191,33,0,0,4,1],[192,9,192,34,0,0,4,0],[192,9,192,34,0,0,4,1],[193,17,193,39,0,0,4,0],[196,9,196,36,0,0,4,0],[196,9,196,36,0,0,4,1],[197,17,197,41,0,0,4,0],[200,9,200,49,0,0,4,0],[200,9,200,49,0,0,4,1],[201,17,201,54,0,0,4,0],[204,9,204,44,0,0,4,0],[204,9,204,44,0,0,4,1],[205,17,205,49,0,0,4,0],[208,9,208,34,0,0,4,0],[208,9,208,34,0,0,4,1],[209,17,209,39,0,0,4,0],[212,9,212,33,0,0,4,0],[212,9,212,33,0,0,4,1],[213,17,213,38,0,0,4,0],[216,9,216,35,0,0,4,0],[216,9,216,35,0,0,4,1],[217,17,217,40,0,0,4,0],[220,9,220,43,0,0,4,1],[224,9,224,36,0,0,4,1],[228,9,228,37,0,0,4,1],[235,10,235,32,0,0,4,1],[236,12,236,34,0,0,4,1],[237,12,237,33,0,0,4,1],[238,12,238,35,0,0,4,1]],"BIO_lookup_ex":[[686,5,686,17,0,0,4,0],[688,5,688,18,0,0,4,1],[691,5,691,17,0,0,4,1],[694,5,694,19,0,0,4,1],[697,5,697,12,0,0,4,1],[703,9,703,26,0,0,4,1],[712,9,712,29,0,0,4,1],[727,13,727,25,0,0,4,0],[727,29,727,48,0,0,4,1],[732,13,732,45,0,0,4,1],[743,9,743,24,0,0,4,1],[750,9,750,24,0,0,4,1],[755,9,755,15,0,0,4,0],[758,9,758,16,0,0,4,1]],"dealloc_tmp_loader_store":[[103,9,103,22,0,0,4,1]],"ASYNC_set_mem_functions":[[58,9,58,50,0,0,4,1],[60,9,60,25,0,0,4,1],[66,9,66,25,0,0,4,0],[68,9,68,24,0,0,4,0]],"inner_loader_fetch":[[295,31,295,49,0,0,4,1],[299,9,299,22,0,0,4,1],[299,26,299,41,0,0,4,1],[313,9,313,16,0,0,4,0],[330,13,333,20,0,0,4,0],[339,17,339,24,0,0,4,0],[352,10,352,17,0,0,4,0],[352,40,352,54,0,0,4,1]],"_CONF_get_string":[[72,9,72,21,0,0,4,1],[74,9,74,21,0,0,4,1],[76,9,76,27,0,0,4,1],[78,9,78,24,0,0,4,0],[82,13,82,22,0,0,4,0],[82,13,82,22,0,0,4,1],[84,13,84,40,0,0,4,1],[93,9,93,18,0,0,4,0],[93,9,93,18,0,0,4,1]],"conf_value_cmp":[[107,9,107,33,0,0,4,0],[107,9,107,33,0,0,4,1],[109,13,109,19,0,0,4,1],[113,9,113,24,0,0,4,0],[113,9,113,24,0,0,4,1],[113,28,113,43,0,0,4,0],[115,9,115,27,0,0,4,0],[115,9,115,27,0,0,4,1],[117,12,117,29,0,0,4,0]],"NCONF_get_string":[[309,9,309,10,0,0,4,1],[312,9,312,21,0,0,4,1],[317,29,317,42,0,0,4,0]],"asn1_item_clear":[[197,5,197,27,0,0,4,0]],"EC_KEY_free":[[84,9,84,17,0,0,4,1]],"TS_TST_INFO_set_tsa":[[276,9,276,22,0,0,4,1],[279,9,279,24,0,0,4,0],[279,9,279,24,0,0,4,1]],"_CONF_new_section":[[191,9,191,48,0,0,4,1],[193,9,193,49,0,0,4,1],[196,9,196,49,0,0,4,1],[204,9,204,19,0,0,4,1],[204,23,204,58,0,0,4,1]],"cmp_with_thunk":[[208,12,208,35,0,0,4,1]],"BIO_ADDRINFO_address":[[448,9,448,20,0,0,4,0]],"construct_loader":[[268,9,268,16,0,0,4,0],[276,9,276,23,0,0,4,1]],"trim_ws":[[920,12,920,19,0,0,4,0],[920,12,920,19,0,0,4,1],[923,12,923,24,0,0,4,0],[923,12,923,24,0,0,4,1],[35,32,35,60,2,0,4,0],[35,32,35,60,2,0,4,1]],"_CONF_get_section":[[27,9,27,21,0,0,4,1],[27,25,27,40,0,0,4,1],[31,12,31,30,0,0,4,0]]},"library_calls":["OPENSSL_init_ssl","ERR_clear_error","BIO_get_host_ip","OSSL_STORE_LOADER_fetch","OSSL_STORE_LOADER_settable_ctx_params","SSL_CTX_new","TLS_method","SSL_new","SSL_client_hello_get0_random","d2i_PKCS8_PRIV_KEY_INFO_fp","PKCS12_SAFEBAG_create0_p8inf","CT_POLICY_EVAL_CTX_new","CT_POLICY_EVAL_CTX_get0_issuer","TS_TST_INFO_new","GENERAL_NAME_new","ASN1_IA5STRING_new","ASN1_STRING_set","TS_TST_INFO_set_tsa","EC_KEY_new","EC_KEY_get_default_method","EC_KEY_set_method","NCONF_new","TS_RESP_CTX_new","NCONF_load","TS_CONF_set_ess_cert_id_digest","ASYNC_set_mem_functions","i2d_PKCS12_SAFEBAG","CRYPTO_free","PKCS12_SAFEBAG_free","PKCS8_PRIV_KEY_INFO_free","SSL_free","SSL_CTX_free","OSSL_STORE_LOADER_free","CT_POLICY_EVAL_CTX_free","TS_TST_INFO_free","GENERAL_NAME_free","EC_KEY_free","NCONF_free","TS_RESP_CTX_free"],"critical_calls":["OPENSSL_init_ssl","ERR_clear_error","BIO_get_host_ip","OSSL_STORE_LOADER_fetch","OSSL_STORE_LOADER_settable_ctx_params","SSL_CTX_new","TLS_method","SSL_new","SSL_client_hello_get0_random","d2i_PKCS8_PRIV_KEY_INFO_fp","PKCS12_SAFEBAG_create0_p8inf","CT_POLICY_EVAL_CTX_new","CT_POLICY_EVAL_CTX_get0_issuer","TS_TST_INFO_new","GENERAL_NAME_new","ASN1_IA5STRING_new","ASN1_STRING_set","TS_TST_INFO_set_tsa","EC_KEY_new","EC_KEY_get_default_method","EC_KEY_set_method","NCONF_new","TS_RESP_CTX_new","NCONF_load","TS_CONF_set_ess_cert_id_digest","ASYNC_set_mem_functions","i2d_PKCS12_SAFEBAG","CRYPTO_free","PKCS12_SAFEBAG_free","PKCS8_PRIV_KEY_INFO_free","SSL_free","SSL_CTX_free","OSSL_STORE_LOADER_free","CT_POLICY_EVAL_CTX_free","TS_TST_INFO_free","GENERAL_NAME_free","EC_KEY_free","NCONF_free","TS_RESP_CTX_free"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified OpenSSL APIs in a meaningful sequence:

*/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
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

    // Create memory file for input data
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

    // Variables for API calls
    unsigned char ip[4];
    const char *hostname = "localhost";
    OSSL_STORE_LOADER *loader = NULL;
    SSL *ssl = NULL;
    SSL_CTX *ctx = NULL;
    PKCS8_PRIV_KEY_INFO *p8 = NULL;
    PKCS12_SAFEBAG *bag = NULL;
    CT_POLICY_EVAL_CTX *ct_ctx = NULL;
    X509 *issuer = NULL;
    TS_TST_INFO *tst_info = NULL;
    GENERAL_NAME *tsa_name = NULL;
    EC_KEY *ec_key = NULL;
    const EC_KEY_METHOD *ec_method = NULL;
    CONF *conf = NULL;
    TS_RESP_CTX *ts_resp_ctx = NULL;
    
    // 1. BIO_get_host_ip - Convert hostname to IP
    int ip_result = BIO_get_host_ip(hostname, ip);
    
    // 2. OSSL_STORE_LOADER_settable_ctx_params - Get loader params
    loader = OSSL_STORE_LOADER_fetch(NULL, "file", NULL);
    if (loader) {
        const OSSL_PARAM *params = OSSL_STORE_LOADER_settable_ctx_params(loader);
    }
    
    // 3. SSL_client_hello_get0_random - Setup SSL and get random
    ctx = SSL_CTX_new(TLS_method());
    if (ctx) {
        ssl = SSL_new(ctx);
        if (ssl) {
            const unsigned char *random_data;
            size_t random_len = SSL_client_hello_get0_random(ssl, &random_data);
        }
    }
    
    // 4. PKCS12_SAFEBAG_create0_p8inf - Create PKCS12 bag from private key
    // Read PKCS8 private key from input
    fseek(in_file, 0, SEEK_SET);
    p8 = d2i_PKCS8_PRIV_KEY_INFO_fp(in_file, NULL);
    if (p8) {
        bag = PKCS12_SAFEBAG_create0_p8inf(p8);
    }
    
    // 5. CT_POLICY_EVAL_CTX_get0_issuer - Get issuer from CT context
    ct_ctx = CT_POLICY_EVAL_CTX_new();
    if (ct_ctx) {
        issuer = CT_POLICY_EVAL_CTX_get0_issuer(ct_ctx);
    }
    
    // 6. TS_TST_INFO_set_tsa - Set TSA in timestamp info
    tst_info = TS_TST_INFO_new();
    tsa_name = GENERAL_NAME_new();
    if (tst_info && tsa_name) {
        // Set some data for TSA name (using first 100 bytes from input)
        if (size > 100) {
            tsa_name->type = GEN_DNS;
            tsa_name->d.dNSName = ASN1_IA5STRING_new();
            if (tsa_name->d.dNSName) {
                ASN1_STRING_set(tsa_name->d.dNSName, data, size > 100 ? 100 : size);
            }
        }
        int ts_result = TS_TST_INFO_set_tsa(tst_info, tsa_name);
    }
    
    // 7. EC_KEY_set_method - Set method for EC key
    ec_key = EC_KEY_new();
    ec_method = EC_KEY_get_default_method();
    if (ec_key && ec_method) {
        int ec_result = EC_KEY_set_method(ec_key, ec_method);
    }
    
    // 8. TS_CONF_set_ess_cert_id_digest - Configure ESS cert ID digest
    conf = NCONF_new(NULL);
    ts_resp_ctx = TS_RESP_CTX_new();
    if (conf && ts_resp_ctx) {
        // Write config data to file
        FILE *conf_file = fopen("temp_conf.cnf", "wb");
        if (conf_file) {
            fwrite(data, 1, size > 200 ? 200 : size, conf_file);
            assert_file_closed(&conf_file);;
            
            // Load config
            if (NCONF_load(conf, "temp_conf.cnf", NULL) > 0) {
                int ts_conf_result = TS_CONF_set_ess_cert_id_digest(conf, "default", ts_resp_ctx);
            }
            remove("temp_conf.cnf");
        }
    }
    
    // 9. ASYNC_set_mem_functions - Set async memory functions
    // Use simple wrapper functions
    auto alloc_fn = [](size_t *size) -> void* {
        *size = 1024;
        return malloc(*size);
    };
    
    auto free_fn = [](void *ptr) -> void {
        free(ptr);
    };
    
    int async_result = ASYNC_set_mem_functions(
        (ASYNC_stack_alloc_fn)alloc_fn,
        (ASYNC_stack_free_fn)free_fn
    );
    
    // Write some output using the data
    if (out_file) {
        // Write processed data to output
        fwrite(data, 1, size > 50 ? 50 : size, out_file);
        
        // If we have PKCS12 bag, try to write it
        if (bag) {
            unsigned char *bag_data = NULL;
            int bag_len = i2d_PKCS12_SAFEBAG(bag, &bag_data);
            if (bag_data && bag_len > 0) {
                fwrite(bag_data, 1, bag_len, out_file);
                OPENSSL_free(bag_data);
            }
        }
    }
    
    // Cleanup
    if (bag) PKCS12_SAFEBAG_free(bag);
    if (p8) PKCS8_PRIV_KEY_INFO_free(p8);
    if (ssl) SSL_free(ssl);
    if (ctx) SSL_CTX_free(ctx);
    if (loader) OSSL_STORE_LOADER_free(loader);
    if (ct_ctx) CT_POLICY_EVAL_CTX_free(ct_ctx);
    if (tst_info) TS_TST_INFO_free(tst_info);
    if (tsa_name) GENERAL_NAME_free(tsa_name);
    if (ec_key) EC_KEY_free(ec_key);
    if (conf) NCONF_free(conf);
    if (ts_resp_ctx) TS_RESP_CTX_free(ts_resp_ctx);
    
    assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}