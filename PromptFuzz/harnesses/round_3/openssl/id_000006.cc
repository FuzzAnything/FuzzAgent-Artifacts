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
//<ID> 175
//<Prompt> []
/*<Combination>: [
*/
//<score> 164, nr_unique_branch: 298
//<Quality> {"density":8,"unique_branches":{"rsa_keygen":[[634,12,634,24,0,0,4,0],[635,13,635,28,0,0,4,1],[635,32,635,57,0,0,4,0]],"MOD_EXP_CTIME_COPY_FROM_PREBUF":[[550,9,550,20,0,0,4,0],[551,21,551,28,0,0,4,0],[551,21,551,28,0,0,4,1],[554,25,554,34,0,0,4,0],[554,25,554,34,0,0,4,1]],"bn_cmp_part_words":[[920,9,920,15,0,0,4,1],[926,9,926,15,0,0,4,1]],"bnrand_range":[[155,14,155,42,0,0,4,0],[155,46,155,74,0,0,4,0],[155,46,155,74,0,0,4,1],[161,17,162,35,0,0,4,1],[171,17,171,38,0,0,4,0],[171,17,171,38,0,0,4,1],[172,21,172,41,0,0,4,1],[174,21,174,42,0,0,4,0],[174,21,174,42,0,0,4,1],[175,25,175,45,0,0,4,1],[179,17,179,25,0,0,4,1],[184,18,184,39,0,0,4,0],[184,18,184,39,0,0,4,1]],"bn_mul_normal":[[637,13,637,22,0,0,4,0]],"ossl_rsa_get_lcm":[[270,12,270,41,0,0,4,0],[271,12,271,41,0,0,4,0],[272,12,272,37,0,0,4,0],[273,12,273,36,0,0,4,0],[274,12,274,45,0,0,4,0]],"ossl_ifc_ffc_compute_security_bits":[[310,13,310,14,0,0,4,1],[311,5,311,14,0,0,4,0],[313,5,313,14,0,0,4,1],[315,5,315,14,0,0,4,1],[317,5,317,14,0,0,4,1],[319,5,319,14,0,0,4,1],[321,5,321,14,0,0,4,1],[323,5,323,15,0,0,4,1]],"BN_mod_exp":[[142,9,142,21,0,0,4,0],[144,13,144,24,0,0,4,0],[144,13,144,24,0,0,4,1],[144,28,144,35,0,0,4,0],[145,16,145,56,0,0,4,0],[146,16,146,56,0,0,4,1]],"ossl_rsa_check_pminusq_diff":[[248,9,248,28,0,0,4,1],[252,9,252,25,0,0,4,1],[255,9,255,30,0,0,4,1]],"rsa_validate_rng_strength":[[207,9,207,20,0,0,4,1]],"bn_mul_recursive":[[187,9,187,16,0,0,4,1],[193,9,193,42,0,0,4,1],[204,13,204,24,0,0,4,1],[205,5,205,12,0,0,4,0],[205,5,205,12,0,0,4,1],[209,5,209,12,0,0,4,1],[212,5,212,12,0,0,4,0],[212,5,212,12,0,0,4,1],[217,5,217,12,0,0,4,1],[218,5,218,11,0,0,4,1],[219,5,219,11,0,0,4,1],[222,5,222,11,0,0,4,0],[222,5,222,11,0,0,4,1],[227,5,227,11,0,0,4,1],[230,5,230,11,0,0,4,0],[230,5,230,11,0,0,4,1],[236,9,236,15,0,0,4,1],[245,16,245,22,0,0,4,0],[245,26,245,34,0,0,4,0],[245,38,245,46,0,0,4,0],[248,13,248,18,0,0,4,0],[273,9,273,12,0,0,4,0],[273,9,273,12,0,0,4,1],[287,9,287,11,0,0,4,0],[287,9,287,11,0,0,4,1],[297,13,297,30,0,0,4,1]],"BN_mod_exp_mont":[[332,13,332,51,0,0,4,0],[333,16,333,54,0,0,4,0]],"bn_rsa_fips186_5_aux_prime_max_sum_size_for_prob_primes":[[113,23,113,31,0,0,4,0],[115,9,115,22,0,0,4,1],[117,9,117,22,0,0,4,1],[119,9,119,22,0,0,4,0]],"bn_cmp_words":[[896,17,896,26,0,0,4,0],[896,17,896,26,0,0,4,1],[23,26,23,52,1,0,4,1],[22,24,22,50,2,0,4,0]],"BN_add":[[26,13,26,24,0,0,4,0],[29,20,29,31,0,0,4,1]],"BN_rshift1":[[53,9,53,22,0,0,4,0]],"bn_rsa_fips186_5_aux_prime_min_size":[[90,9,90,22,0,0,4,1],[92,9,92,22,0,0,4,1],[94,9,94,22,0,0,4,0]],"ossl_bn_rsa_fips186_5_derive_prime":[[323,9,323,21,0,0,4,1],[326,9,326,20,0,0,4,1],[338,9,338,20,0,0,4,0],[339,13,339,52,0,0,4,1],[341,13,342,57,0,0,4,1],[343,16,343,55,0,0,4,1],[344,16,344,43,0,0,4,1],[357,11,357,31,0,0,4,0],[358,16,358,60,0,0,4,0],[360,16,360,58,0,0,4,0],[361,16,361,37,0,0,4,0],[362,16,362,43,0,0,4,0],[363,16,363,33,0,0,4,0],[365,16,365,45,0,0,4,0],[368,9,368,26,0,0,4,0],[368,9,368,26,0,0,4,1],[368,30,368,51,0,0,4,1],[371,9,371,15,0,0,4,0],[390,13,390,24,0,0,4,0],[395,17,395,57,0,0,4,1],[395,61,395,80,0,0,4,1],[399,13,399,46,0,0,4,1],[399,50,399,66,0,0,4,1],[406,13,406,19,0,0,4,1],[414,17,414,38,0,0,4,1],[423,17,423,39,0,0,4,1],[424,20,424,39,0,0,4,1],[427,17,427,43,0,0,4,0],[427,17,427,43,0,0,4,1],[430,21,430,27,0,0,4,0],[430,21,430,27,0,0,4,1],[432,21,432,27,0,0,4,1],[436,17,436,28,0,0,4,1],[440,17,440,43,0,0,4,1]],"bn_rsa_fips186_5_aux_prime_MR_rounds":[[57,9,57,22,0,0,4,1],[59,9,59,22,0,0,4,1],[61,9,61,22,0,0,4,0]],"bn_sqr_recursive":[[162,9,162,16,0,0,4,1],[165,16,165,23,0,0,4,0],[165,16,165,23,0,0,4,1],[169,9,169,42,0,0,4,1],[176,9,176,15,0,0,4,0],[176,9,176,15,0,0,4,1],[178,14,178,20,0,0,4,0],[186,9,186,14,0,0,4,0],[211,9,211,11,0,0,4,0],[211,9,211,11,0,0,4,1],[221,13,221,30,0,0,4,1]],"ossl_rsa_sp800_56b_pairwise_test":[[461,9,461,18,0,0,4,1],[465,12,465,29,0,0,4,0],[466,12,466,51,0,0,4,0],[467,12,467,53,0,0,4,0],[468,12,468,31,0,0,4,0],[469,9,469,17,0,0,4,1]],"BN_consttime_swap":[[989,17,989,27,0,0,4,0],[989,17,989,27,0,0,4,1]],"bn_mul_fixed_top":[[522,9,522,17,0,0,4,0],[523,13,523,43,0,0,4,1],[550,9,550,36,0,0,4,0],[550,40,550,67,0,0,4,0],[550,40,550,67,0,0,4,1],[551,13,551,20,0,0,4,0],[551,24,551,30,0,0,4,0],[556,17,556,23,0,0,4,0],[559,17,559,24,0,0,4,1],[566,17,566,26,0,0,4,1],[568,17,568,23,0,0,4,1],[568,27,568,33,0,0,4,1],[577,21,577,49,0,0,4,1],[579,21,579,50,0,0,4,1],[598,9,598,16,0,0,4,0],[598,20,598,42,0,0,4,1]],"BN_gcd":[[589,9,589,25,0,0,4,1],[594,9,594,25,0,0,4,1],[608,9,608,18,0,0,4,1],[609,12,609,32,0,0,4,1],[610,12,610,32,0,0,4,1],[617,17,617,28,0,0,4,0],[617,17,617,28,0,0,4,1],[617,32,617,43,0,0,4,0],[628,17,628,29,0,0,4,0],[628,17,628,29,0,0,4,1],[635,9,635,38,0,0,4,1],[636,12,636,41,0,0,4,1],[640,16,640,34,0,0,4,0],[641,9,641,35,0,0,4,1],[642,12,642,38,0,0,4,1],[643,12,643,41,0,0,4,1],[652,18,652,32,0,0,4,0],[654,17,654,22,0,0,4,0],[654,17,654,22,0,0,4,1],[667,13,667,32,0,0,4,1],[673,13,673,30,0,0,4,1],[680,9,680,38,0,0,4,1],[681,12,681,29,0,0,4,1]],"bn_sqr_fixed_top":[[72,13,72,46,0,0,4,1],[81,17,81,24,0,0,4,0],[82,21,82,51,0,0,4,1]],"bn_is_prime_int":[[302,9,302,26,0,0,4,0],[305,21,305,40,0,0,4,0],[305,21,305,40,0,0,4,1],[307,17,307,36,0,0,4,1],[309,17,309,25,0,0,4,0],[309,17,309,25,0,0,4,1],[312,13,312,38,0,0,4,1]],"calc_trial_divisions":[[76,9,76,20,0,0,4,1],[78,14,78,26,0,0,4,0]],"RAND_get0_private":[[855,12,855,24,0,0,4,1]],"BN_are_coprime":[[555,9,555,20,0,0,4,1]],"BN_mod_sub":[[117,9,117,25,0,0,4,1]],"ossl_rsa_fips186_5_gen_prob_primes":[[96,9,96,49,0,0,4,1],[102,9,102,43,0,0,4,0],[102,9,102,43,0,0,4,1],[117,9,117,20,0,0,4,1],[117,24,117,35,0,0,4,1],[117,39,117,50,0,0,4,1],[122,9,122,23,0,0,4,0],[124,9,124,23,0,0,4,0],[126,9,126,23,0,0,4,1],[126,27,126,41,0,0,4,1],[132,9,133,34,0,0,4,1],[137,13,138,43,0,0,4,1],[143,13,143,19,0,0,4,1],[145,13,145,20,0,0,4,1],[150,13,150,19,0,0,4,1],[152,13,152,20,0,0,4,1],[163,9,163,17,0,0,4,1]],"bn_mod_exp_mont_fixed_top":[[654,9,654,24,0,0,4,1],[657,13,657,47,0,0,4,1],[659,13,659,43,0,0,4,1],[678,9,678,23,0,0,4,0],[678,27,678,41,0,0,4,0],[678,45,678,69,0,0,4,0],[679,12,679,32,0,0,4,1],[723,58,723,79,0,0,4,0],[725,9,725,27,0,0,4,1],[729,13,730,20,0,0,4,1],[737,9,737,27,0,0,4,1],[1007,13,1007,20,0,0,4,1],[1018,20,1018,28,0,0,4,0],[1018,20,1018,28,0,0,4,1],[1136,9,1136,24,0,0,4,0],[333,6,333,15,6,0,4,0],[334,15,334,23,6,0,4,1],[335,15,335,23,6,0,4,0]],"ossl_bn_rsa_fips186_5_gen_prob_primes":[[203,9,203,18,0,0,4,1],[203,22,203,35,0,0,4,1],[208,11,208,23,0,0,4,1],[209,11,209,23,0,0,4,1],[210,12,210,25,0,0,4,1],[211,12,211,25,0,0,4,1],[212,9,212,20,0,0,4,1],[212,24,212,35,0,0,4,1],[212,39,212,51,0,0,4,1],[212,55,212,67,0,0,4,1],[216,9,216,20,0,0,4,1],[221,9,221,20,0,0,4,0],[223,13,224,24,0,0,4,1],[228,9,228,20,0,0,4,0],[230,13,231,24,0,0,4,1],[236,9,236,74,0,0,4,1],[237,12,237,77,0,0,4,1],[240,9,240,114,0,0,4,1],[243,9,244,24,0,0,4,1],[249,9,249,19,0,0,4,0],[251,9,251,19,0,0,4,0],[253,9,253,20,0,0,4,0],[255,9,255,20,0,0,4,0]],"bn_sqr_words":[[192,13,192,21,0,0,4,1]],"bn_rsa_fips186_5_find_aux_prob_prime":[[146,9,146,33,0,0,4,1],[156,13,156,20,0,0,4,0],[156,13,156,20,0,0,4,1],[158,13,158,20,0,0,4,1],[161,13,161,32,0,0,4,1]],"ossl_rsa_sp800_56b_generate_key":[[396,9,396,57,0,0,4,1],[400,9,400,74,0,0,4,1],[404,9,404,20,0,0,4,1],[408,9,408,23,0,0,4,1],[419,13,419,84,0,0,4,0],[419,13,419,84,0,0,4,1],[423,13,423,25,0,0,4,0],[423,29,423,55,0,0,4,0],[423,29,423,55,0,0,4,1],[431,13,431,19,0,0,4,1],[433,13,433,19,0,0,4,0],[441,9,441,23,0,0,4,1]],"ossl_rsa_sp800_56b_derive_params_from_pq":[[258,9,258,20,0,0,4,1],[268,9,268,75,0,0,4,1],[274,9,274,18,0,0,4,0],[278,13,278,27,0,0,4,1],[284,13,284,27,0,0,4,1],[287,13,287,56,0,0,4,1],[291,13,291,48,0,0,4,1],[297,13,297,27,0,0,4,0],[299,13,299,27,0,0,4,1],[299,31,299,67,0,0,4,1],[304,9,304,26,0,0,4,0],[306,9,306,26,0,0,4,1],[309,9,309,16,0,0,4,1],[313,9,313,26,0,0,4,0],[315,9,315,26,0,0,4,1],[318,9,318,16,0,0,4,1],[324,9,324,26,0,0,4,1],[327,9,327,63,0,0,4,1],[333,9,333,17,0,0,4,1]],"bn_sub_part_words":[[36,9,36,16,0,0,4,0]],"ossl_rsa_sp800_56b_validate_strength":[[194,9,194,23,0,0,4,1]],"bn_rsa_fips186_5_prime_MR_rounds":[[72,9,72,22,0,0,4,1],[74,9,74,22,0,0,4,0]]},"library_calls":["TLS_method","SSL_CTX_new","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","EVP_PKEY_new","BN_new","BN_new","BN_bin2bn","BN_rand","RSA_new","RSA_generate_key_ex","EVP_PKEY_assign","EVP_PKEY_get1_RSA","RSA_free","DH_meth_new","d2i_PKCS7_ENC_CONTENT","PKCS7_RECIP_INFO_new","RC2_set_key","RC2_cfb64_encrypt","RC2_cfb64_encrypt","BIO_dump_indent_fp","PKCS7_ENC_CONTENT_free","PKCS7_RECIP_INFO_free","DH_meth_free","EVP_PKEY_free","BN_free","BN_free","OSSL_CMP_CTX_free","SSL_CTX_free"],"critical_calls":["TLS_method","SSL_CTX_new","SSL_CTX_set_ct_validation_callback","OSSL_CMP_CTX_new","OSSL_CMP_CTX_get_http_cb_arg","EVP_PKEY_new","BN_new","BN_new","BN_bin2bn","BN_rand","RSA_new","RSA_generate_key_ex","EVP_PKEY_assign","EVP_PKEY_get1_RSA","RSA_free","DH_meth_new","d2i_PKCS7_ENC_CONTENT","PKCS7_RECIP_INFO_new","RC2_set_key","RC2_cfb64_encrypt","RC2_cfb64_encrypt","BIO_dump_indent_fp","PKCS7_ENC_CONTENT_free","PKCS7_RECIP_INFO_free","DH_meth_free","EVP_PKEY_free","BN_free","BN_free","OSSL_CMP_CTX_free","SSL_CTX_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 64) {
	return 0;
	}
	 // Minimum data for meaningful operations

    // 1. SSL Context and CT validation setup
    const SSL_METHOD *method = TLS_method();
    SSL_CTX *ssl_ctx = SSL_CTX_new(method);
    if (ssl_ctx) {
        // Dummy CT validation callback
        auto ct_callback = [](const CT_POLICY_EVAL_CTX *ctx, 
                              const struct stack_st_SCT *scts, void *arg) {
            return 1;
        };
        SSL_CTX_set_ct_validation_callback(ssl_ctx, ct_callback, nullptr);
    }

    // 2. CMP Context operations
    OSSL_CMP_CTX *cmp_ctx = OSSL_CMP_CTX_new(nullptr, nullptr);
    if (cmp_ctx) {
        void *http_arg = OSSL_CMP_CTX_get_http_cb_arg(cmp_ctx);
        (void)http_arg; // Use to avoid unused warning
    }

    // 3. RSA key generation using input data
    EVP_PKEY *pkey = EVP_PKEY_new();
    BIGNUM *bn = BN_new();
    BIGNUM *e = BN_new();
    
    if (bn && e && pkey) {
        // Use first 32 bytes for BN initialization
        size_t bn_len = size > 32 ? 32 : size;
        BN_bin2bn(data, bn_len, bn);
        
        // Generate random exponent
        BN_rand(e, 32, 0, 0);
        
        // Generate RSA key
        RSA *rsa = RSA_new();
        if (rsa) {
            RSA_generate_key_ex(rsa, 2048, e, nullptr);
            EVP_PKEY_assign(pkey, EVP_PKEY_RSA, rsa);
            
            // Extract and verify RSA key
            RSA *extracted_rsa = EVP_PKEY_get1_RSA(pkey);
            if (extracted_rsa) {
                RSA_free(extracted_rsa);
            }
        }
    }

    // 4. DH method creation
    DH_METHOD *dh_meth = DH_meth_new("fuzz_dh", 0);
    
    // 5. PKCS7 operations using input data
    const unsigned char *p = data;
    PKCS7_ENC_CONTENT *enc_content = d2i_PKCS7_ENC_CONTENT(nullptr, &p, size);
    
    PKCS7_RECIP_INFO *recip_info = PKCS7_RECIP_INFO_new();
    
    // 6. RC2 encryption/decryption using input data
    RC2_KEY rc2_key;
    unsigned char ivec[8] = {0};
    int num = 0;
    
    // Use portion of input for RC2 key (skip first 32 bytes used for BN)
    size_t rc2_data_offset = 32;
    if (size > rc2_data_offset + 16) {
        size_t key_len = 16;
        RC2_set_key(&rc2_key, key_len, data + rc2_data_offset, 128);
        
        // Encrypt/decrypt a small block
        unsigned char in[16], out[16];
        size_t block_len = size - rc2_data_offset - key_len;
        if (block_len > 16) block_len = 16;
        
        memcpy(in, data + rc2_data_offset + key_len, block_len);
        RC2_cfb64_encrypt(in, out, block_len, &rc2_key, ivec, &num, 1);
        RC2_cfb64_encrypt(out, in, block_len, &rc2_key, ivec, &num, 0);
    }

    // 7. BIO dump operations using FILE*
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            // Dump first 256 bytes or entire input if smaller
            int dump_len = size > 256 ? 256 : size;
            BIO_dump_indent_fp(out_file, data, dump_len, 2);
            
            // Demonstrate file descriptor usage
            int in_fd = fuzz_fileno(in_file);
            int out_fd = fuzz_fileno(out_file);
            (void)in_fd; // Use to avoid unused warning
            (void)out_fd; // Use to avoid unused warning
            
            assert_file_closed(&out_file);;
            remove("output_file");
        }
        assert_file_closed(&in_file);;
    }

    // 8. Cleanup all resources
    if (enc_content) PKCS7_ENC_CONTENT_free(enc_content);
    if (recip_info) PKCS7_RECIP_INFO_free(recip_info);
    if (dh_meth) DH_meth_free(dh_meth);
    if (pkey) EVP_PKEY_free(pkey);
    if (bn) BN_free(bn);
    if (e) BN_free(e);
    if (cmp_ctx) OSSL_CMP_CTX_free(cmp_ctx);
    if (ssl_ctx) SSL_CTX_free(ssl_ctx);

    assert_file_closed(&in_file);
	return 0;
}