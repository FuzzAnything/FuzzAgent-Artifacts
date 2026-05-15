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
//<ID> 242
//<Prompt> []
/*<Combination>: [
*/
//<score> 372, nr_unique_branch: 234
//<Quality> {"density":24,"unique_branches":{"bn_correct_top":[[22,24,22,50,1,0,4,1]],"bn_sqr_normal":[[122,9,122,16,0,0,4,1]],"bn_mul_words":[[166,13,166,23,0,0,4,1]],"ossl_bn_mask_bits_fixed_top":[[849,9,849,14,0,0,4,1],[854,9,854,20,0,0,4,1],[856,9,856,15,0,0,4,0],[856,9,856,15,0,0,4,1]],"BN_MONT_CTX_set_locked":[[430,9,430,39,0,0,4,1],[434,9,434,12,0,0,4,1],[446,9,446,20,0,0,4,1],[448,9,448,40,0,0,4,0],[448,9,448,40,0,0,4,1],[454,9,454,40,0,0,4,1],[459,9,459,15,0,0,4,1]],"dsa_mod_inverse_fermat":[[487,9,487,31,0,0,4,1],[491,9,491,38,0,0,4,0],[492,12,492,29,0,0,4,0],[493,12,493,27,0,0,4,0],[494,12,494,50,0,0,4,0],[494,12,494,50,0,0,4,1]],"SHA512_Update_thunk":[[240,9,240,17,0,0,4,1],[244,9,244,18,0,0,4,1],[250,9,250,20,0,0,4,0],[250,9,250,20,0,0,4,1],[253,13,253,20,0,0,4,0],[253,13,253,20,0,0,4,1],[263,9,263,28,0,0,4,1],[276,9,276,17,0,0,4,0]],"BN_mod_exp_mont":[[326,9,326,22,0,0,4,0],[332,13,332,51,0,0,4,0],[333,16,333,54,0,0,4,0]],"ossl_dsa_do_sign_int":[[88,9,88,30,0,0,4,1],[89,12,89,33,0,0,4,1],[90,12,90,33,0,0,4,1],[94,9,94,30,0,0,4,1],[100,9,100,20,0,0,4,1],[104,9,104,23,0,0,4,1],[104,27,104,41,0,0,4,1],[108,9,108,20,0,0,4,1],[114,9,114,20,0,0,4,1],[118,9,119,51,0,0,4,0],[118,9,119,51,0,0,4,1],[122,9,122,28,0,0,4,0],[122,9,122,28,0,0,4,1],[129,9,129,41,0,0,4,1],[147,13,148,61,0,0,4,1],[150,14,150,31,0,0,4,1],[156,9,156,67,0,0,4,1],[158,9,158,58,0,0,4,1],[162,9,162,58,0,0,4,1],[166,9,166,62,0,0,4,1],[170,9,170,62,0,0,4,1],[174,9,174,65,0,0,4,0],[174,9,174,65,0,0,4,1],[176,9,176,63,0,0,4,1],[185,9,185,27,0,0,4,1],[185,31,185,49,0,0,4,1],[194,9,194,16,0,0,4,1]],"dsa_sign_setup":[[229,9,229,23,0,0,4,1],[229,27,229,41,0,0,4,1],[229,45,229,59,0,0,4,1],[235,9,235,34,0,0,4,0],[235,9,235,34,0,0,4,1],[236,12,236,37,0,0,4,0],[236,12,236,37,0,0,4,1],[237,12,237,37,0,0,4,1],[238,12,238,41,0,0,4,1],[239,12,239,41,0,0,4,1],[240,12,240,41,0,0,4,1],[244,9,244,30,0,0,4,1],[250,9,250,18,0,0,4,1],[250,22,250,31,0,0,4,1],[253,9,253,23,0,0,4,1],[263,9,263,36,0,0,4,0],[263,9,263,36,0,0,4,1],[264,12,264,39,0,0,4,1],[265,12,265,39,0,0,4,1],[270,13,270,25,0,0,4,0],[271,17,271,32,0,0,4,1],[285,21,287,35,0,0,4,1],[292,14,292,45,0,0,4,1],[297,9,297,43,0,0,4,0],[298,13,299,47,0,0,4,0],[298,13,299,47,0,0,4,1],[318,9,318,37,0,0,4,1],[319,12,319,40,0,0,4,1],[324,9,324,40,0,0,4,1],[329,13,330,36,0,0,4,1],[334,9,334,16,0,0,4,1],[338,9,338,71,0,0,4,0],[338,9,338,71,0,0,4,1],[346,9,346,13,0,0,4,0],[346,9,346,13,0,0,4,1],[348,9,348,22,0,0,4,1]],"bn_mod_exp_mont_fixed_top":[[654,9,654,24,0,0,4,1],[657,13,657,47,0,0,4,1],[659,13,659,43,0,0,4,1],[663,19,663,37,0,0,4,0],[665,13,665,28,0,0,4,1],[666,16,666,45,0,0,4,1],[689,50,689,73,0,0,4,1],[709,13,709,24,0,0,4,0],[709,28,709,48,0,0,4,0],[751,9,751,58,0,0,4,1],[760,13,760,67,0,0,4,1],[908,13,908,24,0,0,4,0],[908,28,908,35,0,0,4,0],[908,28,908,35,0,0,4,1],[943,26,943,33,0,0,4,1],[945,27,945,34,0,0,4,1],[951,38,951,45,0,0,4,0],[951,38,951,45,0,0,4,1],[967,21,967,27,0,0,4,0],[967,21,967,27,0,0,4,1],[971,21,971,26,0,0,4,0],[971,21,971,26,0,0,4,1],[975,29,975,35,0,0,4,0],[975,29,975,35,0,0,4,1],[980,16,980,22,0,0,4,0],[980,16,980,22,0,0,4,1],[986,16,986,22,0,0,4,0],[986,16,986,22,0,0,4,1],[1007,13,1007,20,0,0,4,0],[1007,13,1007,20,0,0,4,1],[1008,20,1008,28,0,0,4,0],[1008,20,1008,28,0,0,4,1],[1018,20,1018,28,0,0,4,0],[1018,20,1018,28,0,0,4,1],[1136,9,1136,24,0,0,4,0],[333,22,333,31,6,0,4,0]],"BN_is_word":[[1047,12,1047,32,0,0,4,0],[1047,37,1047,39,0,0,4,1],[1047,43,1047,50,0,0,4,0]],"bn_mul_add_words":[[133,13,133,23,0,0,4,1],[136,13,136,23,0,0,4,0],[136,13,136,23,0,0,4,1]],"DSA_SIG_free":[[43,9,43,20,0,0,4,1]],"SHA512_Final":[[169,9,169,32,0,0,4,1],[188,9,188,20,0,0,4,1],[193,5,193,34,0,0,4,1],[196,5,196,30,0,0,4,1],[199,5,199,30,0,0,4,1],[202,5,202,30,0,0,4,0],[205,5,205,30,0,0,4,1],[222,5,222,12,0,0,4,1],[159,17,159,30,8,0,4,0],[159,17,159,30,8,0,4,1]],"BN_is_bit_set":[[23,26,23,52,4,0,4,0]],"bn_mul_normal":[[640,13,640,22,0,0,4,1]],"sha512_newctx":[[63,9,63,32,1,0,4,1]],"bn_mul_mont_fixed_top":[[44,9,44,16,0,0,4,1]],"BN_mod_mul":[[219,9,219,15,0,0,4,1],[223,13,223,34,0,0,4,1]],"bn_sqr_words":[[189,13,189,21,0,0,4,0]],"int_bn_mod_inverse":[[206,33,206,46,0,0,4,0],[260,9,260,21,0,0,4,1],[357,16,357,30,0,0,4,0],[357,16,357,30,0,0,4,1],[367,17,367,49,0,0,4,0],[367,17,367,49,0,0,4,1],[368,21,368,28,0,0,4,1],[370,21,370,37,0,0,4,1],[372,24,372,60,0,0,4,0],[372,24,372,60,0,0,4,1],[374,21,374,38,0,0,4,1],[376,21,376,38,0,0,4,0],[376,21,376,38,0,0,4,1],[378,25,378,32,0,0,4,1],[380,25,380,41,0,0,4,1],[384,25,384,41,0,0,4,1],[386,25,386,41,0,0,4,1],[388,25,388,42,0,0,4,0],[388,25,388,42,0,0,4,1],[390,29,390,47,0,0,4,1],[397,29,397,47,0,0,4,1],[402,29,402,45,0,0,4,1],[407,21,407,45,0,0,4,1],[448,17,448,29,0,0,4,0],[448,17,448,29,0,0,4,1],[449,21,449,39,0,0,4,1],[452,21,452,37,0,0,4,0],[452,21,452,37,0,0,4,1],[453,25,453,44,0,0,4,1],[455,28,455,44,0,0,4,0],[455,28,455,44,0,0,4,1],[456,25,456,46,0,0,4,1],[458,28,458,39,0,0,4,0],[459,25,459,48,0,0,4,1],[461,25,461,51,0,0,4,1],[467,21,467,41,0,0,4,1],[486,9,486,17,0,0,4,1],[492,9,492,21,0,0,4,1],[507,9,507,22,0,0,4,0],[507,26,507,38,0,0,4,1]],"sha512_internal_final":[[45,13,45,35,0,0,4,0],[45,39,45,56,0,0,4,0],[45,60,45,73,0,0,4,0]],"evp_md_init_internal":[[208,9,208,28,0,0,4,1],[22,24,22,50,9,0,4,0],[23,26,23,52,10,0,4,1]],"BN_MONT_CTX_set":[[344,20,344,31,0,0,4,1],[346,13,346,29,0,0,4,0],[348,18,348,61,0,0,4,0],[352,13,352,28,0,0,4,1],[357,17,357,43,0,0,4,1]],"BN_mul_word":[[186,9,186,15,0,0,4,0],[187,13,187,19,0,0,4,1],[191,17,191,19,0,0,4,1]],"sha512_internal_init":[[150,16,150,38,0,0,4,0],[150,42,150,51,0,0,4,0]],"BN_sub_word":[[174,9,174,23,0,0,4,0],[174,27,174,46,0,0,4,0]],"ossl_bn_is_word_fixed_top":[[1055,9,1055,15,0,0,4,1],[1055,19,1055,30,0,0,4,1],[1060,17,1060,27,0,0,4,0],[1060,17,1060,27,0,0,4,1]],"ossl_bn_gen_dsa_nonce_fixed_top":[[316,9,316,22,0,0,4,1],[320,9,320,24,0,0,4,1],[326,9,326,69,0,0,4,1],[337,9,337,19,0,0,4,1],[341,17,341,26,0,0,4,0],[344,24,344,42,0,0,4,0],[344,24,344,42,0,0,4,1],[345,17,347,21,0,0,4,1],[350,17,350,52,0,0,4,1],[351,20,351,59,0,0,4,1],[352,20,353,43,0,0,4,1],[354,20,354,66,0,0,4,1],[355,20,356,42,0,0,4,1],[357,20,357,60,0,0,4,1],[361,17,361,44,0,0,4,1],[368,13,368,50,0,0,4,1],[375,13,375,36,0,0,4,0],[375,13,375,36,0,0,4,1]]},"library_calls":["OPENSSL_init_ssl","ERR_clear_error","BIO_new_fp","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_X509_VERIFY_PARAM_sk_type","X509_VERIFY_PARAM_new","OPENSSL_sk_push","ossl_check_X509_VERIFY_PARAM_sk_type","ossl_check_X509_VERIFY_PARAM_type","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get0_trustedStore","ossl_check_GENERAL_SUBTREE_copyfunc_type","BIO_new","BIO_s_mem","BIO_write","BIO_get_shutdown","DSA_new","BN_new","BN_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_set_word","BN_set_word","BN_set_word","DSA_set0_pqg","DSA_set0_key","DSA_do_sign","BN_free","BN_free","BN_free","BN_free","BN_free","BIO_ctrl","PEM_read_bio_X509_REQ","BIO_ctrl","d2i_X509_REQ_bio","EVP_PKEY_new","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_assign","BN_free","X509_REQ_check_private_key","OPENSSL_sk_pop_free","ossl_check_X509_VERIFY_PARAM_sk_type","ossl_check_X509_VERIFY_PARAM_freefunc_type","OSSL_CMP_CTX_free","BIO_free","DSA_free","DSA_SIG_free","X509_REQ_free","EVP_PKEY_free","BIO_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_ssl","ERR_clear_error","BIO_new_fp","OPENSSL_sk_set_thunks","OPENSSL_sk_new_null","ossl_check_X509_VERIFY_PARAM_sk_type","X509_VERIFY_PARAM_new","OPENSSL_sk_push","ossl_check_X509_VERIFY_PARAM_type","ossl_check_X509_VERIFY_PARAM_sk_type","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get0_trustedStore","ossl_check_GENERAL_SUBTREE_copyfunc_type","BIO_new","BIO_s_mem","BIO_write","BIO_get_shutdown","DSA_new","BN_new","BN_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_set_word","BN_set_word","BN_set_word","DSA_set0_pqg","DSA_set0_key","DSA_do_sign","BIO_ctrl","PEM_read_bio_X509_REQ","BIO_ctrl","d2i_X509_REQ_bio","EVP_PKEY_new","RSA_new","BN_new","BN_set_word","RSA_generate_key_ex","EVP_PKEY_assign","BN_free","X509_REQ_check_private_key","OPENSSL_sk_pop_free","ossl_check_X509_VERIFY_PARAM_freefunc_type","ossl_check_X509_VERIFY_PARAM_sk_type","OSSL_CMP_CTX_free","BIO_free","DSA_free","DSA_SIG_free","X509_REQ_free","EVP_PKEY_free","BIO_free","ERR_clear_error"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 256) {
        return 0;  // Need sufficient data for meaningful operations
    }

    // Initialize OpenSSL
    OPENSSL_init_ssl(0, NULL);
    ERR_clear_error();

    // Create a temporary file with input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    BIO *bio_in = BIO_new_fp(in_file, BIO_NOCLOSE);
    if (!bio_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Create and use X509_VERIFY_PARAM stack
    struct stack_st_X509_VERIFY_PARAM *verify_params = sk_X509_VERIFY_PARAM_new_null();
    if (verify_params) {
        // Convert stack type (API usage)
        OPENSSL_STACK *check_stack = ossl_check_X509_VERIFY_PARAM_sk_type(verify_params);
        (void)check_stack;  // Prevent unused variable warning
        
        // Add a sample verify param if possible
        X509_VERIFY_PARAM *param = X509_VERIFY_PARAM_new();
        if (param) {
            sk_X509_VERIFY_PARAM_push(verify_params, param);
        }
    }

    // 2. Create OSSL_CMP context and get trusted store
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(NULL, NULL);
    X509_STORE *trusted_store = NULL;
    if (cmp_ctx) {
        trusted_store = OSSL_CMP_CTX_get0_trustedStore(cmp_ctx);
        (void)trusted_store;  // Prevent unused variable warning
    }

    // 3. Create and use GENERAL_SUBTREE copy function
    sk_GENERAL_SUBTREE_copyfunc subtree_cpy = NULL;
    OPENSSL_sk_copyfunc checked_cpy = ossl_check_GENERAL_SUBTREE_copyfunc_type(subtree_cpy);
    (void)checked_cpy;  // Prevent unused variable warning

    // 4. Create BIO and check shutdown status
    BIO *test_bio = BIO_new(BIO_s_mem());
    if (test_bio) {
        // Write some data to BIO
        BIO_write(test_bio, data, size > 128 ? 128 : size);
        
        // Get shutdown status (API usage)
        int shutdown_status = BIO_get_shutdown(test_bio);
        (void)shutdown_status;  // Prevent unused variable warning
    }

    // 5. DSA signing operation
    DSA *dsa = DSA_new();
    DSA_SIG *dsa_sig = NULL;
    if (dsa) {
        // Generate DSA parameters (simplified - in real usage would use proper generation)
        // Using a small portion of input data as "digest"
        const unsigned char *digest = data;
        int dlen = size > 40 ? 40 : size;
        
        // Create a simple DSA key for testing (in real usage, would properly generate/load)
        // Note: This is a minimal setup just to call the API
        BIGNUM *p = BN_new();
        BIGNUM *q = BN_new();
        BIGNUM *g = BN_new();
        BIGNUM *priv_key = BN_new();
        BIGNUM *pub_key = BN_new();
        
        if (p && q && g && priv_key && pub_key) {
            // Use input data to create some numbers (simplified)
            BN_bin2bn(data, size > 64 ? 64 : size, p);
            BN_bin2bn(data + (size > 64 ? 64 : 0), size > 128 ? 64 : (size > 64 ? size - 64 : 0), q);
            BN_set_word(g, 2);
            BN_set_word(priv_key, 12345);
            BN_set_word(pub_key, 54321);
            
            DSA_set0_pqg(dsa, p, q, g);
            DSA_set0_key(dsa, pub_key, priv_key);
            
            // Attempt DSA signing (API usage)
            dsa_sig = DSA_do_sign(digest, dlen, dsa);
        } else {
            if (p) BN_free(p);
            if (q) BN_free(q);
            if (g) BN_free(g);
            if (priv_key) BN_free(priv_key);
            if (pub_key) BN_free(pub_key);
        }
    }

    // 6. X509_REQ and private key checking
    X509_REQ *req = NULL;
    EVP_PKEY *pkey = NULL;
    
    // Try to read X509_REQ from input data
    BIO_reset(bio_in);
    req = PEM_read_bio_X509_REQ(bio_in, NULL, NULL, NULL);
    if (!req) {
        // Try DER format
        BIO_reset(bio_in);
        req = d2i_X509_REQ_bio(bio_in, NULL);
    }
    
    // Create a sample private key for testing
    pkey = EVP_PKEY_new();
    if (pkey && req) {
        // Create a minimal RSA key for testing
        RSA *rsa = RSA_new();
        if (rsa) {
            BIGNUM *e = BN_new();
            BN_set_word(e, 65537);
            // Generate RSA key (simplified)
            RSA_generate_key_ex(rsa, 512, e, NULL);
            EVP_PKEY_assign_RSA(pkey, rsa);
            BN_free(e);
            
            // Check private key against request (API usage)
            int check_result = X509_REQ_check_private_key(req, pkey);
            (void)check_result;  // Prevent unused variable warning
        }
    }

    // Clean up all allocated resources
    if (verify_params) {
        sk_X509_VERIFY_PARAM_pop_free(verify_params, X509_VERIFY_PARAM_free);
    }
    
    if (cmp_ctx) {
        OSSL_CMP_CTX_free(cmp_ctx);
    }
    
    if (test_bio) {
        BIO_free(test_bio);
    }
    
    if (dsa) {
        DSA_free(dsa);
    }
    
    if (dsa_sig) {
        DSA_SIG_free(dsa_sig);
    }
    
    if (req) {
        X509_REQ_free(req);
    }
    
    if (pkey) {
        EVP_PKEY_free(pkey);
    }
    
    if (bio_in) {
        BIO_free(bio_in);
    }
    
    assert_file_closed(&in_file);;
    
    // Clear any remaining errors
    ERR_clear_error();
    
    assert_file_closed(&in_file);
	return 0;
}