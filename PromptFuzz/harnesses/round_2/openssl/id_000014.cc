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
//<ID> 322
//<Prompt> []
/*<Combination>: [
*/
//<score> 280, nr_unique_branch: 381
//<Quality> {"density":5,"unique_branches":{"_CONF_new_section":[[191,9,191,48,0,0,4,1],[193,9,193,49,0,0,4,1],[196,9,196,49,0,0,4,1],[204,9,204,19,0,0,4,1],[204,23,204,58,0,0,4,1]],"_CONF_free_data":[[142,9,142,27,0,0,4,1]],"_CONF_add_string":[[53,9,53,28,0,0,4,1],[57,9,57,18,0,0,4,0],[57,9,57,18,0,0,4,1]],"file_free":[[123,13,123,22,0,0,4,0],[123,26,123,42,0,0,4,0],[124,17,124,53,0,0,4,1]],"OPENSSL_DIR_read":[[89,9,89,20,0,0,4,1],[89,24,89,41,0,0,4,1],[95,9,95,21,0,0,4,0],[95,9,95,21,0,0,4,1],[97,13,97,25,0,0,4,1],[113,13,113,32,0,0,4,1],[130,9,130,25,0,0,4,1]],"ASN1_item_new_ex":[[41,9,41,60,0,0,4,0]],"_CONF_get_section":[[27,9,27,21,0,0,4,1],[27,25,27,40,0,0,4,1],[31,12,31,30,0,0,4,0]],"BUF_MEM_grow":[[76,9,76,27,0,0,4,0],[80,9,80,24,0,0,4,0],[81,13,81,30,0,0,4,0]],"ossl_asn1_do_adb":[[275,13,275,36,0,0,4,0]],"is_keytype":[[901,9,901,18,0,0,4,0],[901,9,901,18,0,0,4,1],[906,12,906,34,0,0,4,0],[906,12,906,34,0,0,4,1]],"value_free_hash":[[160,9,160,24,0,0,4,0],[160,9,160,24,0,0,4,1]],"SSL_CTX_set_session_id_context":[[1038,9,1038,45,0,0,4,1]],"cms_sd_set_version":[[71,17,71,68,0,0,4,1],[85,17,85,52,0,0,4,1],[93,9,93,76,0,0,4,1],[97,17,97,42,0,0,4,1],[109,9,109,24,0,0,4,1]],"process_include":[[805,9,805,31,0,0,4,0],[805,9,805,31,0,0,4,1],[812,13,812,28,0,0,4,0],[812,13,812,28,0,0,4,1],[818,13,818,60,0,0,4,0]],"OSSL_CRMF_CERTTEMPLATE_get0_issuer":[[590,12,590,24,0,0,4,0]],"NCONF_free_data":[[213,9,213,21,0,0,4,1]],"OPENSSL_sk_delete_ptr":[[321,9,321,19,0,0,4,1],[324,17,324,28,0,0,4,0],[325,13,325,29,0,0,4,0],[325,13,325,29,0,0,4,1]],"_CONF_new_data":[[122,9,122,21,0,0,4,1],[124,9,124,27,0,0,4,0],[126,13,126,31,0,0,4,1]],"conf_value_cmp":[[107,9,107,33,0,0,4,0],[107,9,107,33,0,0,4,1],[109,13,109,19,0,0,4,1],[113,9,113,24,0,0,4,0],[113,9,113,24,0,0,4,1],[113,28,113,43,0,0,4,0],[113,28,113,43,0,0,4,1],[115,9,115,27,0,0,4,0],[115,9,115,27,0,0,4,1],[117,12,117,29,0,0,4,0],[117,12,117,29,0,0,4,1]],"def_load_bio":[[232,9,232,39,0,0,4,1],[238,9,238,24,0,0,4,1],[241,9,241,34,0,0,4,1],[247,9,247,19,0,0,4,1],[255,13,255,54,0,0,4,1],[262,13,262,23,0,0,4,0],[262,27,262,63,0,0,4,1],[266,13,266,23,0,0,4,0],[266,13,266,23,0,0,4,1],[271,17,271,23,0,0,4,0],[271,17,271,23,0,0,4,1],[271,27,271,54,0,0,4,0],[271,27,271,54,0,0,4,1],[279,13,279,19,0,0,4,0],[279,13,279,19,0,0,4,1],[279,23,279,29,0,0,4,0],[279,23,279,29,0,0,4,1],[285,17,285,31,0,0,4,1],[299,17,299,53,0,0,4,0],[309,16,309,21,0,0,4,0],[309,16,309,21,0,0,4,1],[310,17,310,35,0,0,4,0],[310,17,310,35,0,0,4,1],[310,39,310,57,0,0,4,0],[310,39,310,57,0,0,4,1],[318,13,318,15,0,0,4,0],[318,13,318,15,0,0,4,1],[318,19,318,26,0,0,4,0],[318,19,318,26,0,0,4,1],[332,13,332,19,0,0,4,0],[332,13,332,19,0,0,4,1],[332,23,332,34,0,0,4,0],[332,23,332,34,0,0,4,1],[338,40,338,53,0,0,4,0],[338,40,338,53,0,0,4,1],[338,57,338,64,0,0,4,0],[338,57,338,64,0,0,4,1],[343,13,343,18,0,0,4,0],[343,13,343,18,0,0,4,1],[352,13,352,22,0,0,4,0],[352,13,352,22,0,0,4,1],[361,17,361,26,0,0,4,0],[361,17,361,26,0,0,4,1],[362,21,362,31,0,0,4,0],[362,21,362,31,0,0,4,1],[362,35,362,42,0,0,4,0],[362,35,362,42,0,0,4,1],[370,17,370,55,0,0,4,1],[372,17,372,64,0,0,4,0],[372,17,372,64,0,0,4,1],[374,17,374,27,0,0,4,1],[382,17,382,32,0,0,4,0],[382,17,382,32,0,0,4,1],[382,36,382,51,0,0,4,0],[382,36,382,51,0,0,4,1],[393,21,393,31,0,0,4,0],[393,21,393,31,0,0,4,1],[393,35,393,44,0,0,4,0],[393,35,393,44,0,0,4,1],[396,21,396,30,0,0,4,0],[396,21,396,30,0,0,4,1],[404,21,404,33,0,0,4,0],[404,21,404,33,0,0,4,1],[404,37,404,46,0,0,4,0],[404,37,404,46,0,0,4,1],[404,50,404,65,0,0,4,0],[404,50,404,65,0,0,4,1],[420,21,420,47,0,0,4,1],[423,28,423,53,0,0,4,1],[426,28,426,56,0,0,4,1],[437,21,437,31,0,0,4,0],[437,21,437,31,0,0,4,1],[437,35,437,44,0,0,4,0],[437,35,437,44,0,0,4,1],[453,21,453,39,0,0,4,1],[457,21,457,40,0,0,4,0],[460,21,460,30,0,0,4,0],[460,21,460,30,0,0,4,1],[465,21,465,59,0,0,4,0],[465,21,465,59,0,0,4,1],[468,21,468,40,0,0,4,1],[486,21,486,39,0,0,4,1],[496,21,496,44,0,0,4,0],[496,21,496,44,0,0,4,1],[505,21,505,33,0,0,4,0],[505,21,505,33,0,0,4,1],[507,25,507,38,0,0,4,0],[508,29,508,64,0,0,4,1],[514,25,514,37,0,0,4,1],[523,24,523,33,0,0,4,0],[523,24,523,33,0,0,4,1],[533,17,533,57,0,0,4,1],[537,17,537,32,0,0,4,1],[539,17,539,62,0,0,4,0],[539,17,539,62,0,0,4,1],[542,17,542,47,0,0,4,0],[542,17,542,47,0,0,4,1],[543,21,544,28,0,0,4,0],[543,21,544,28,0,0,4,1],[546,21,546,31,0,0,4,1],[553,17,553,51,0,0,4,1],[577,12,577,33,0,0,4,0],[577,12,577,33,0,0,4,1],[585,9,585,23,0,0,4,0],[585,9,585,23,0,0,4,1],[588,9,588,21,0,0,4,1],[592,9,592,24,0,0,4,0],[596,9,596,18,0,0,4,0],[596,9,596,18,0,0,4,1],[33,32,33,61,12,0,4,0],[33,32,33,61,12,0,4,1],[32,32,32,61,14,0,4,0],[32,32,32,61,14,0,4,1],[61,5,61,63,17,0,4,0],[61,5,61,63,17,0,4,1],[61,5,61,63,21,0,4,0],[61,5,61,63,21,0,4,1],[58,30,58,74,62,0,4,0],[58,30,58,74,62,0,4,1],[58,30,58,74,68,0,4,0],[58,30,58,74,68,0,4,1]],"asn1_item_clear":[[178,5,178,27,0,0,4,0],[180,13,180,15,0,0,4,0],[180,19,180,36,0,0,4,1]],"ERR_add_error_vdata":[[837,13,837,24,0,0,4,0],[842,17,842,26,0,0,4,1]],"clear_comments":[[611,13,611,19,0,0,4,0],[611,13,611,19,0,0,4,1],[31,32,31,66,1,0,4,1],[30,32,30,65,3,0,4,0],[30,32,30,65,3,0,4,1],[39,32,39,64,4,0,4,1],[38,32,38,63,5,0,4,0],[38,32,38,63,5,0,4,1],[33,32,33,61,6,0,4,0],[33,32,33,61,6,0,4,1],[53,29,53,53,7,0,4,0],[53,29,53,53,7,0,4,1],[32,32,32,61,8,0,4,0],[32,32,32,61,8,0,4,1]],"BIO_new_ex":[[98,9,98,31,0,0,4,1],[103,9,103,31,0,0,4,0]],"OPENSSL_sk_pop":[[443,9,443,19,0,0,4,0]],"OPENSSL_strlcpy":[[81,12,81,20,0,0,4,1]],"get_next_file":[[837,12,837,63,0,0,4,0],[842,14,842,25,0,0,4,0],[842,14,842,25,0,0,4,1],[843,20,843,76,0,0,4,1],[844,17,844,28,0,0,4,0],[844,17,844,28,0,0,4,1],[845,20,845,75,0,0,4,0],[845,20,845,75,0,0,4,1],[852,17,852,32,0,0,4,1],[866,17,866,35,0,0,4,0],[875,17,875,28,0,0,4,0]],"internal_delete":[[308,9,308,27,0,0,4,0]],"ERR_vset_error":[[90,13,90,28,0,0,4,0]],"EVP_PKEY_CTX_get_signature_md":[[664,9,664,20,0,0,4,1],[664,24,664,53,0,0,4,0]],"cms_signed_data_init":[[38,9,38,29,0,0,4,0],[40,13,40,31,0,0,4,1]],"OPENSSL_DIR_end":[[155,9,155,20,0,0,4,0],[155,24,155,36,0,0,4,0],[160,9,160,15,0,0,4,0],[162,9,162,16,0,0,4,1],[164,9,164,16,0,0,4,1]],"CMS_sign_ex":[[520,9,520,20,0,0,4,1],[520,24,520,49,0,0,4,1],[524,9,524,30,0,0,4,1],[531,9,531,21,0,0,4,1],[536,17,536,32,0,0,4,1],[545,9,545,32,0,0,4,1],[548,9,548,45,0,0,4,1],[549,12,549,45,0,0,4,1]],"OPENSSL_LH_strhash":[[366,9,366,20,0,0,4,0],[366,24,366,36,0,0,4,0]],"ossl_cms_SignedData_init_bio":[[1370,9,1370,19,0,0,4,1],[1372,9,1372,53,0,0,4,0],[1374,17,1374,38,0,0,4,1]],"def_load":[[177,9,177,19,0,0,4,1]],"cms_get0_signed":[[29,9,29,58,0,0,4,1]],"value_free_stack_doall":[[170,9,170,24,0,0,4,1],[174,41,174,47,0,0,4,0],[174,41,174,47,0,0,4,1]],"OPENSSL_LH_delete":[[156,9,156,20,0,0,4,1],[166,9,166,36,0,0,4,0],[166,9,166,36,0,0,4,1],[166,40,166,105,0,0,4,1]],"CMS_SignedData_init":[[57,9,57,34,0,0,4,0]],"CMS_get0_content":[[273,5,273,24,0,0,4,1],[276,5,276,26,0,0,4,0],[279,5,279,29,0,0,4,1],[282,5,282,26,0,0,4,1],[285,5,285,29,0,0,4,1],[288,5,288,43,0,0,4,1],[292,5,292,34,0,0,4,1],[295,5,295,40,0,0,4,1],[298,5,298,12,0,0,4,1]],"BIO_free":[[140,32,140,60,0,0,4,1]],"str_copy":[[647,9,647,38,0,0,4,1],[650,9,650,45,0,0,4,1],[657,20,657,27,0,0,4,0],[657,20,657,27,0,0,4,1],[657,44,657,56,0,0,4,0],[657,44,657,56,0,0,4,1],[665,17,665,27,0,0,4,0],[665,17,665,27,0,0,4,1],[687,22,687,30,0,0,4,0],[687,22,687,30,0,0,4,1],[689,22,689,30,0,0,4,0],[689,22,689,30,0,0,4,1],[691,22,691,30,0,0,4,0],[691,22,691,30,0,0,4,1],[693,22,693,30,0,0,4,0],[693,22,693,30,0,0,4,1],[698,18,698,30,0,0,4,0],[698,18,698,30,0,0,4,1],[699,17,699,37,0,0,4,0],[707,17,707,26,0,0,4,1],[709,22,709,31,0,0,4,0],[709,22,709,31,0,0,4,1],[714,17,714,18,0,0,4,0],[714,17,714,18,0,0,4,1],[719,21,719,40,0,0,4,1],[721,17,721,30,0,0,4,0],[721,17,721,30,0,0,4,1],[721,34,721,47,0,0,4,0],[721,34,721,47,0,0,4,1],[729,25,729,44,0,0,4,1],[735,17,735,18,0,0,4,0],[735,17,735,18,0,0,4,1],[736,21,736,27,0,0,4,0],[736,21,736,27,0,0,4,1],[753,17,753,28,0,0,4,0],[753,17,753,28,0,0,4,1],[756,17,756,26,0,0,4,0],[756,17,756,26,0,0,4,1],[761,17,761,48,0,0,4,1],[765,17,765,50,0,0,4,1],[769,20,769,22,0,0,4,0],[769,20,769,22,0,0,4,1],[38,32,38,63,1,0,4,0],[38,32,38,63,1,0,4,1],[33,32,33,61,2,0,4,0],[33,32,33,61,2,0,4,1],[32,32,32,61,3,0,4,0],[32,32,32,61,3,0,4,1],[39,32,39,64,5,0,4,1],[33,32,33,61,7,0,4,0],[33,32,33,61,7,0,4,1],[32,32,32,61,8,0,4,0],[32,32,32,61,8,0,4,1],[32,32,32,61,9,0,4,0],[32,32,32,61,9,0,4,1],[36,32,36,63,10,0,4,0],[36,32,36,63,10,0,4,1],[36,32,36,63,12,0,4,0],[36,32,36,63,12,0,4,1]],"NCONF_get_string":[[309,9,309,10,0,0,4,1],[312,9,312,21,0,0,4,1],[317,29,317,42,0,0,4,0]],"ossl_cms_content_bio":[[132,9,132,20,0,0,4,1],[135,9,135,21,0,0,4,0]],"scan_quote":[[948,12,948,31,0,0,4,0],[948,12,948,31,0,0,4,1],[948,35,948,44,0,0,4,0],[948,35,948,44,0,0,4,1],[956,9,956,16,0,0,4,0],[956,9,956,16,0,0,4,1],[33,32,33,61,1,0,4,0],[33,32,33,61,1,0,4,1],[32,32,32,61,2,0,4,0],[32,32,32,61,2,0,4,1]],"CMS_ContentInfo_new_ex":[[60,9,60,19,0,0,4,0],[63,13,63,26,0,0,4,1]],"CMS_dataInit":[[149,9,149,14,0,0,4,1],[153,9,153,14,0,0,4,1],[159,5,159,24,0,0,4,1],[162,5,162,26,0,0,4,0],[166,5,166,26,0,0,4,1],[175,5,175,29,0,0,4,1],[179,5,179,29,0,0,4,1],[183,5,183,43,0,0,4,1],[187,5,187,12,0,0,4,1],[192,9,192,15,0,0,4,1],[195,9,195,15,0,0,4,0]],"eat_alpha_numeric":[[937,21,937,40,0,0,4,1],[33,32,33,61,1,0,4,0],[33,32,33,61,1,0,4,1],[53,29,53,53,2,0,4,0],[53,29,53,53,2,0,4,1],[37,32,37,69,3,0,4,0],[37,32,37,69,3,0,4,1]],"BIO_vsnprintf":[[168,9,168,25,0,0,4,0]],"CMS_final":[[907,9,907,52,0,0,4,0]],"eat_ws":[[911,31,911,50,0,0,4,0],[35,32,35,60,1,0,4,0],[35,32,35,60,1,0,4,1]],"BIO_new_file":[[63,9,63,34,0,0,4,0],[66,9,66,21,0,0,4,1],[80,9,80,46,0,0,4,1]],"trim_ws":[[920,12,920,19,0,0,4,0],[920,12,920,19,0,0,4,1],[923,12,923,24,0,0,4,0],[923,12,923,24,0,0,4,1],[35,32,35,60,2,0,4,0],[35,32,35,60,2,0,4,1]],"_CONF_get_string":[[72,9,72,21,0,0,4,1],[74,9,74,21,0,0,4,1],[76,9,76,27,0,0,4,0],[76,9,76,27,0,0,4,1],[78,9,78,24,0,0,4,0],[82,13,82,22,0,0,4,0],[82,13,82,22,0,0,4,1],[84,13,84,40,0,0,4,1],[93,9,93,18,0,0,4,0],[93,9,93,18,0,0,4,1]]},"library_calls":["SSL_CTX_new","TLS_method","SSL_CTX_set_session_id_context","CMS_sign","CMS_add1_signer","CMS_SignerInfo_sign","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_get_signature_md","OSSL_CRMF_CERTTEMPLATE_new","OSSL_CRMF_CERTTEMPLATE_get0_issuer","ossl_check_OSSL_CMP_CERTRESPONSE_compfunc_type","NCONF_new","NCONF_load","NCONF_get_string","NCONF_free","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OPENSSL_sk_push","ossl_check_OSSL_CMP_CERTRESPONSE_sk_type","ossl_check_OSSL_CMP_CERTRESPONSE_type","OSSL_CRMF_CERTTEMPLATE_free","EVP_PKEY_CTX_free","CMS_ContentInfo_free","SSL_CTX_free","OPENSSL_sk_free","ossl_check_OSSL_CMP_CERTRESPONSE_sk_type"],"critical_calls":["SSL_CTX_new","TLS_method","SSL_CTX_set_session_id_context","CMS_sign","CMS_add1_signer","CMS_SignerInfo_sign","EVP_PKEY_CTX_new_id","EVP_PKEY_CTX_get_signature_md","OSSL_CRMF_CERTTEMPLATE_new","OSSL_CRMF_CERTTEMPLATE_get0_issuer","ossl_check_OSSL_CMP_CERTRESPONSE_compfunc_type","NCONF_new","NCONF_load","NCONF_get_string","NCONF_free","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","OPENSSL_sk_push","ossl_check_OSSL_CMP_CERTRESPONSE_type","ossl_check_OSSL_CMP_CERTRESPONSE_sk_type","OSSL_CRMF_CERTTEMPLATE_free","EVP_PKEY_CTX_free","CMS_ContentInfo_free","SSL_CTX_free","OPENSSL_sk_free","ossl_check_OSSL_CMP_CERTRESPONSE_sk_type"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create SSL_CTX and set session ID context using input data
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    if (ctx && size > 0) {
        // Use first portion of data for session ID context
        size_t sid_len = size > 32 ? 32 : size;
        SSL_CTX_set_session_id_context(ctx, data, sid_len);
    }
    
    // 2. Create CMS_SignerInfo and attempt to sign (using remaining data)
    CMS_ContentInfo *cms = CMS_sign(NULL, NULL, NULL, NULL, CMS_NOCERTS | CMS_DETACHED);
    if (cms) {
        // Create a signer info structure
        CMS_SignerInfo *si = CMS_add1_signer(cms, NULL, NULL, NULL, 0);
        if (si) {
            // Try to sign with whatever data we have
            CMS_SignerInfo_sign(si);
        }
    }
    
    // 3. Create EVP_PKEY_CTX and get signature MD
    EVP_PKEY_CTX *pkey_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (pkey_ctx) {
        const EVP_MD *md = NULL;
        EVP_PKEY_CTX_get_signature_md(pkey_ctx, &md);
    }
    
    // 4. Create OSSL_CRMF_CERTTEMPLATE and get issuer
    OSSL_CRMF_CERTTEMPLATE *tmpl = OSSL_CRMF_CERTTEMPLATE_new();
    if (tmpl) {
        const X509_NAME *issuer = OSSL_CRMF_CERTTEMPLATE_get0_issuer(tmpl);
        // issuer could be NULL for a new template
    }
    
    // 5. Use the comparison function type declaration
    OPENSSL_sk_compfunc cmp_func = ossl_check_OSSL_CMP_CERTRESPONSE_compfunc_type(
        (sk_OSSL_CMP_CERTRESPONSE_compfunc)NULL);
    
    // 6. Create CONF structure and get string from it using input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Write data to a temporary file to create configuration
        FILE *conf_file = fopen("temp_config.cnf", "wb");
        if (conf_file) {
            fwrite(data, 1, size, conf_file);
            assert_file_closed(&conf_file);;
        }
        
        // Load configuration from file
        CONF *conf = NCONF_new(NULL);
        if (conf) {
            NCONF_load(conf, "temp_config.cnf", NULL);
            
            // Try to get a string from the configuration
            // Using reasonable defaults for group and name
            char *value = NCONF_get_string(conf, "default", "key");
            
            // Clean up temp file
            remove("temp_config.cnf");
            NCONF_free(conf);
        }
        assert_file_closed(&in_file);;
    }
    
    // 7. Use input data with stack operations (comparison function)
    // Create a stack of CMP responses
    STACK_OF(OSSL_CMP_CERTRESPONSE) *stack = sk_OSSL_CMP_CERTRESPONSE_new_null();
    if (stack && size > 0) {
        // Add some dummy entries if we have data
        if (size >= sizeof(OSSL_CMP_CERTRESPONSE*)) {
            // Use first bytes as pointer (fuzzing - unsafe but acceptable for fuzzing)
            OSSL_CMP_CERTRESPONSE *resp = NULL;
            sk_OSSL_CMP_CERTRESPONSE_push(stack, resp);
        }
    }
    
    // 8. Additional step: Create output file and write processed data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (size > 0) {
            // Process data: simple transformation (XOR with 0xFF)
            for (size_t i = 0; i < size && i < 1024; i++) {
                uint8_t processed = data[i] ^ 0xFF;
                fwrite(&processed, 1, 1, out_file);
            }
        }
        assert_file_closed(&out_file);;
        remove("output_file"); // Clean up output file
    }
    
    // 9. Clean up all allocated resources
    if (tmpl) OSSL_CRMF_CERTTEMPLATE_free(tmpl);
    if (pkey_ctx) EVP_PKEY_CTX_free(pkey_ctx);
    if (cms) CMS_ContentInfo_free(cms);
    if (ctx) SSL_CTX_free(ctx);
    if (stack) sk_OSSL_CMP_CERTRESPONSE_free(stack);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}