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
//<ID> 477
//<Prompt> []
/*<Combination>: [
*/
//<score> 130, nr_unique_branch: 244
//<Quality> {"density":5,"unique_branches":{"BN_mod_sub_quick":[[193,9,193,15,0,0,4,1],[198,9,198,25,0,0,4,1],[200,9,200,15,0,0,4,0],[200,9,200,15,0,0,4,1]],"evp_pkey_get_legacy":[[2093,9,2093,21,0,0,4,1],[2102,9,2102,30,0,0,4,0],[375,6,375,28,2,0,4,0]],"ossl_ec_wNAF_mul":[[431,9,431,34,0,0,4,0],[431,38,431,66,0,0,4,0],[437,13,437,37,0,0,4,0],[437,41,437,57,0,0,4,0],[437,61,437,71,0,0,4,0]],"BN_add_word":[[109,9,109,22,0,0,4,0]],"EC_GROUP_set_curve":[[588,9,588,42,0,0,4,1]],"ossl_ec_GFp_simple_ladder_step":[[1576,9,1576,19,0,0,4,1],[1577,12,1577,63,0,0,4,1],[1578,12,1578,63,0,0,4,1],[1579,12,1579,63,0,0,4,1],[1580,12,1580,63,0,0,4,1],[1581,12,1581,65,0,0,4,1],[1582,12,1582,55,0,0,4,1],[1583,12,1583,55,0,0,4,1],[1584,12,1584,59,0,0,4,1],[1585,12,1585,55,0,0,4,1],[1586,12,1586,63,0,0,4,1],[1587,12,1587,59,0,0,4,1],[1588,12,1588,55,0,0,4,1],[1589,12,1589,55,0,0,4,1],[1591,12,1591,57,0,0,4,1],[1592,12,1592,63,0,0,4,1],[1593,12,1593,55,0,0,4,1],[1595,12,1595,57,0,0,4,1],[1596,12,1596,57,0,0,4,1],[1597,12,1597,57,0,0,4,1],[1598,12,1598,65,0,0,4,1],[1599,12,1599,59,0,0,4,1],[1600,12,1600,55,0,0,4,1],[1601,12,1601,55,0,0,4,1],[1602,12,1602,55,0,0,4,1],[1603,12,1603,55,0,0,4,1],[1604,12,1604,55,0,0,4,1],[1605,12,1605,59,0,0,4,1],[1606,12,1606,59,0,0,4,1],[1608,12,1608,57,0,0,4,1],[1609,12,1609,55,0,0,4,1],[1610,12,1610,55,0,0,4,1],[1611,12,1611,59,0,0,4,1],[1612,12,1612,59,0,0,4,1],[1613,12,1613,55,0,0,4,1],[1615,12,1615,57,0,0,4,1]],"EC_GROUP_set_generator":[[373,9,373,26,0,0,4,1],[379,9,379,29,0,0,4,1],[379,33,379,57,0,0,4,1],[380,12,380,40,0,0,4,1],[390,9,390,22,0,0,4,1],[390,26,390,43,0,0,4,1],[390,47,390,68,0,0,4,1],[391,12,391,62,0,0,4,1],[401,9,401,25,0,0,4,0],[401,29,401,53,0,0,4,1],[406,9,406,33,0,0,4,0],[408,13,408,37,0,0,4,1],[411,9,411,52,0,0,4,1],[414,9,414,45,0,0,4,1],[418,9,418,25,0,0,4,0],[418,29,418,50,0,0,4,0],[419,13,419,55,0,0,4,1],[431,9,431,32,0,0,4,0]],"EC_POINT_clear_free":[[786,9,786,22,0,0,4,1],[789,9,789,45,0,0,4,0]],"ossl_ec_GFp_simple_ladder_post":[[1655,9,1655,25,0,0,4,1],[1658,9,1658,25,0,0,4,1],[1674,9,1674,19,0,0,4,1],[1675,12,1675,57,0,0,4,1],[1676,12,1676,61,0,0,4,1],[1677,12,1677,61,0,0,4,1],[1678,12,1678,61,0,0,4,1],[1679,12,1679,61,0,0,4,1],[1680,12,1680,61,0,0,4,1],[1681,12,1681,57,0,0,4,1],[1682,12,1682,59,0,0,4,1],[1683,12,1683,67,0,0,4,1],[1684,12,1684,63,0,0,4,1],[1685,12,1685,55,0,0,4,1],[1686,12,1686,61,0,0,4,1],[1687,12,1687,63,0,0,4,1],[1688,12,1688,57,0,0,4,1],[1689,12,1689,59,0,0,4,1],[1690,12,1690,55,0,0,4,1],[1691,12,1691,57,0,0,4,1],[1692,12,1692,55,0,0,4,1],[1693,12,1693,61,0,0,4,1],[1694,12,1694,55,0,0,4,1],[1695,12,1695,61,0,0,4,1],[1696,12,1696,59,0,0,4,1],[1697,13,1697,46,0,0,4,0],[1698,16,1698,62,0,0,4,1],[1699,12,1699,55,0,0,4,1],[1700,13,1700,46,0,0,4,0],[1701,16,1701,62,0,0,4,1],[1702,12,1702,61,0,0,4,1],[1703,12,1703,61,0,0,4,1],[1706,9,1706,46,0,0,4,0],[1707,13,1707,61,0,0,4,1]],"EC_POINT_mul":[[1191,9,1191,33,0,0,4,1]],"ossl_ec_GFp_simple_ladder_pre":[[1502,9,1502,21,0,0,4,1],[1503,12,1503,57,0,0,4,1],[1504,12,1504,61,0,0,4,1],[1505,12,1505,55,0,0,4,1],[1506,12,1506,67,0,0,4,1],[1507,12,1507,57,0,0,4,1],[1509,12,1509,57,0,0,4,1],[1510,12,1510,61,0,0,4,1],[1511,12,1511,61,0,0,4,1],[1512,12,1512,61,0,0,4,1],[1514,12,1514,59,0,0,4,1],[1519,13,1519,63,0,0,4,1],[1521,14,1521,30,0,0,4,1],[1525,13,1525,63,0,0,4,1],[1527,14,1527,30,0,0,4,1],[1530,9,1530,42,0,0,4,0],[1531,13,1531,63,0,0,4,1],[1532,16,1532,66,0,0,4,1],[1536,9,1536,62,0,0,4,1],[1537,12,1537,65,0,0,4,1],[1538,12,1538,65,0,0,4,1]],"BN_mod_exp_mont":[[334,16,334,54,0,0,4,1],[339,9,339,18,0,0,4,1],[354,9,354,23,0,0,4,1],[361,9,361,24,0,0,4,0],[370,9,370,15,0,0,4,1],[370,19,370,37,0,0,4,1],[376,9,376,53,0,0,4,1],[380,9,380,19,0,0,4,0],[381,13,381,65,0,0,4,1],[384,21,384,26,0,0,4,0],[384,21,384,26,0,0,4,1],[385,17,385,53,0,0,4,1],[385,57,385,113,0,0,4,1],[398,9,398,56,0,0,4,0],[399,13,399,37,0,0,4,1],[403,21,403,26,0,0,4,0],[403,21,403,26,0,0,4,1],[414,13,414,42,0,0,4,0],[414,13,414,42,0,0,4,1],[415,17,415,23,0,0,4,0],[416,21,416,63,0,0,4,1],[419,17,419,28,0,0,4,1],[431,21,431,31,0,0,4,0],[431,21,431,31,0,0,4,1],[432,17,432,31,0,0,4,0],[432,17,432,31,0,0,4,1],[434,17,434,45,0,0,4,0],[434,17,434,45,0,0,4,1],[444,13,444,19,0,0,4,0],[444,13,444,19,0,0,4,1],[445,25,445,30,0,0,4,0],[445,25,445,30,0,0,4,1],[446,21,446,63,0,0,4,1],[451,13,451,70,0,0,4,1],[457,13,457,23,0,0,4,0],[457,13,457,23,0,0,4,1],[476,13,476,50,0,0,4,1],[480,9,480,24,0,0,4,1],[309,6,309,15,8,0,4,1],[309,22,309,31,8,0,4,0]],"EC_GROUP_new_curve_GFp":[[58,9,58,20,0,0,4,1],[61,9,61,47,0,0,4,1]],"EVP_PKEY_get_field_type":[[2449,13,2449,23,0,0,4,1],[2452,13,2452,24,0,0,4,1]],"evp_pkey_get0_EC_KEY_int":[[78,9,78,50,0,0,4,1]],"ossl_ec_GFp_mont_group_set_curve":[[156,9,156,20,0,0,4,1],[163,9,163,21,0,0,4,1],[165,9,165,39,0,0,4,1],[170,9,170,20,0,0,4,1],[172,9,172,58,0,0,4,1],[182,9,182,13,0,0,4,1]],"BN_POOL_get":[[328,17,328,44,0,0,4,0]],"ossl_ec_GFp_mont_field_decode":[[281,9,281,35,0,0,4,1]],"ossl_ec_scalar_mul_ladder":[[153,9,153,22,0,0,4,1],[156,9,156,33,0,0,4,1],[160,9,160,36,0,0,4,1],[167,9,167,44,0,0,4,1],[168,12,168,47,0,0,4,1],[173,9,173,22,0,0,4,0],[174,13,174,48,0,0,4,1],[192,9,192,18,0,0,4,1],[197,9,197,65,0,0,4,1],[210,9,210,47,0,0,4,1],[211,12,211,55,0,0,4,1],[216,9,216,35,0,0,4,1],[223,9,223,44,0,0,4,1],[223,48,223,67,0,0,4,1],[234,9,234,40,0,0,4,1],[239,9,239,40,0,0,4,1],[251,9,251,46,0,0,4,1],[252,12,252,49,0,0,4,1],[253,12,253,49,0,0,4,1],[254,12,254,49,0,0,4,1],[255,12,255,49,0,0,4,1],[256,12,256,49,0,0,4,1],[257,12,257,49,0,0,4,1],[258,12,258,49,0,0,4,1],[259,12,259,49,0,0,4,1],[265,9,265,21,0,0,4,1],[271,9,271,50,0,0,4,1],[347,36,347,42,0,0,4,0],[347,36,347,42,0,0,4,1],[352,13,352,55,0,0,4,1],[367,9,367,51,0,0,4,1]],"BN_mod_lshift_quick":[[294,9,294,15,0,0,4,1]],"OBJ_length":[[755,9,755,20,0,0,4,1]],"BN_mod_lshift1_quick":[[257,9,257,26,0,0,4,1],[260,9,260,26,0,0,4,0],[260,9,260,26,0,0,4,1]],"ec_precompute_mont_data":[[1242,9,1242,20,0,0,4,1],[1246,9,1246,33,0,0,4,1],[1249,9,1249,62,0,0,4,1]],"ossl_ec_GFp_mont_field_inv":[[230,9,230,35,0,0,4,1],[233,9,233,20,0,0,4,1],[238,9,238,38,0,0,4,1],[242,9,242,27,0,0,4,1],[244,9,244,36,0,0,4,1],[250,9,250,73,0,0,4,1],[254,9,254,22,0,0,4,1]],"ec_group_new_from_data":[[3055,9,3055,27,0,0,4,1],[3074,9,3074,73,0,0,4,1],[3075,12,3075,76,0,0,4,1],[3076,12,3076,76,0,0,4,1],[3081,9,3081,22,0,0,4,1],[3086,16,3086,57,0,0,4,0],[3087,13,3087,67,0,0,4,1],[3105,9,3105,42,0,0,4,1],[3110,9,3110,73,0,0,4,1],[3111,12,3111,76,0,0,4,1],[3115,9,3115,62,0,0,4,1],[3119,9,3119,77,0,0,4,1],[3120,12,3120,53,0,0,4,1],[3124,9,3124,52,0,0,4,1],[3128,9,3128,17,0,0,4,1],[3136,9,3136,64,0,0,4,0],[3151,13,3151,28,0,0,4,1],[3155,13,3155,37,0,0,4,1],[3173,9,3173,12,0,0,4,1]]},"library_calls":["OPENSSL_init_crypto","d2i_OSSL_ATTRIBUTE_VALUE_MAPPING","ASN1_BIT_STRING_new","CRYPTO_malloc","ASN1_BIT_STRING_set","CRYPTO_free","BN_get_rfc3526_prime_1536","EC_KEY_new_by_curve_name","EC_KEY_generate_key","EVP_PKEY_new","EVP_PKEY_assign","EC_KEY_free","EVP_PKEY_get_field_type","OPENSSL_sk_new","ossl_check_OSSL_TARGETS_compfunc_type","ossl_check_OSSL_TARGETS_compfunc_type","OPENSSL_sk_free","CRYPTO_free","ASN1_BIT_STRING_free","BN_free","EVP_PKEY_free","EC_KEY_free","ERR_clear_error"],"critical_calls":["OPENSSL_init_crypto","d2i_OSSL_ATTRIBUTE_VALUE_MAPPING","ASN1_BIT_STRING_new","CRYPTO_malloc","ASN1_BIT_STRING_set","CRYPTO_free","BN_get_rfc3526_prime_1536","EC_KEY_new_by_curve_name","EC_KEY_generate_key","EVP_PKEY_new","EVP_PKEY_assign","EVP_PKEY_get_field_type","OPENSSL_sk_new","ossl_check_OSSL_TARGETS_compfunc_type","ossl_check_OSSL_TARGETS_compfunc_type","OPENSSL_sk_free","CRYPTO_free","ASN1_BIT_STRING_free","BN_free","EVP_PKEY_free","ERR_clear_error"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize OpenSSL error strings
    ERR_load_crypto_strings();

    // 1. Parse OSSL_ATTRIBUTE_VALUE_MAPPING from input data
    OSSL_ATTRIBUTE_VALUE_MAPPING *mapping = NULL;
    const unsigned char *p = data;
    long len = size;
    mapping = d2i_OSSL_ATTRIBUTE_VALUE_MAPPING(NULL, &p, len);
    
    // Update remaining data length after parsing
    size_t consumed = p - data;
    size_t remaining = (consumed < size) ? size - consumed : 0;

    // 2. Create and set ASN1_BIT_STRING with remaining data
    ASN1_BIT_STRING *bit_string = ASN1_BIT_STRING_new();
    if (bit_string && remaining > 0) {
        // Cast away const for the API (we own the data copy)
        unsigned char *bit_data = (unsigned char *)OPENSSL_malloc(remaining);
        if (bit_data) {
            memcpy(bit_data, data + consumed, remaining);
            ASN1_BIT_STRING_set(bit_string, bit_data, remaining);
            OPENSSL_free(bit_data);
        }
    }

    // 3. Get RFC3526 1536-bit prime
    BIGNUM *prime = BN_get_rfc3526_prime_1536(NULL);

    // 4. Create EC key for field type checking
    EVP_PKEY *pkey = NULL;
    // Generate an EC key using prime field (secp256k1 uses prime field)
    EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (ec_key) {
        EC_KEY_generate_key(ec_key);
        pkey = EVP_PKEY_new();
        if (pkey) {
            EVP_PKEY_assign_EC_KEY(pkey, ec_key);
        } else {
            EC_KEY_free(ec_key);
        }
    }

    // 5. Get field type if we have a valid key
    int field_type = -1;
    if (pkey) {
        field_type = EVP_PKEY_get_field_type(pkey);
    }

    // 6. Setup comparison function for OSSL_TARGETS stack
    // Create a dummy stack to use the comparison function
    OPENSSL_STACK *stack = OPENSSL_sk_new(ossl_check_OSSL_TARGETS_compfunc_type(NULL));
    if (stack) {
        // Create some dummy targets if we have data
        if (remaining > 0 && pkey) {
            OSSL_TARGETS *target1 = NULL; // Note: OSSL_TARGETS is not defined in provided types
            OSSL_TARGETS *target2 = NULL;
            // Since we don't have actual OSSL_TARGETS structure defined,
            // we just call the comparison function type converter
            OPENSSL_sk_compfunc comp_func = ossl_check_OSSL_TARGETS_compfunc_type(
                [](const OSSL_TARGETS *const *a, const OSSL_TARGETS *const *b) -> int {
                    return 0; // Dummy comparison
                }
            );
        }
        OPENSSL_sk_free(stack);
    }

    // 7. Cleanup all allocated resources
    if (mapping) {
        // Note: Assuming there's a corresponding free function
        // Since d2i_OSSL_ATTRIBUTE_VALUE_MAPPING was used, we should free it
        // OSSL_ATTRIBUTE_VALUE_MAPPING_free(mapping);
        // For now, use OPENSSL_free if no specific free function
        OPENSSL_free(mapping);
    }
    
    if (bit_string) {
        ASN1_BIT_STRING_free(bit_string);
    }
    
    if (prime) {
        BN_free(prime);
    }
    
    if (pkey) {
        EVP_PKEY_free(pkey);
    } else if (ec_key) {
        // If pkey wasn't created but ec_key was
        EC_KEY_free(ec_key);
    }

    // Clean up error queue
    ERR_clear_error();

    return 0;
}