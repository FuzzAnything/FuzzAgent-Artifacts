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
//<ID> 416
//<Prompt> []
/*<Combination>: [
*/
//<score> 139.5, nr_unique_branch: 119
//<Quality> {"density":9,"unique_branches":{"pushsig":[[593,17,593,30,0,0,4,0],[593,17,593,30,0,0,4,1],[595,13,595,25,0,0,4,0],[595,13,595,25,0,0,4,1],[599,13,599,25,0,0,4,0],[599,13,599,25,0,0,4,1],[603,13,603,25,0,0,4,0],[603,13,603,25,0,0,4,1]],"free_string":[[52,9,52,41,0,0,4,1]],"UI_new_method":[[27,9,27,20,0,0,4,1],[31,9,31,26,0,0,4,1],[37,9,37,23,0,0,4,0],[39,9,39,23,0,0,4,1],[43,9,43,68,0,0,4,1]],"echo_console":[[523,9,523,17,0,0,4,1]],"DH_generate_parameters_ex":[[123,9,123,35,0,0,4,1]],"d2i_PKCS8PrivateKey_bio":[[178,9,178,19,0,0,4,0],[178,9,178,19,0,0,4,1],[180,9,180,19,0,0,4,1],[184,9,184,17,0,0,4,0]],"noecho_console":[[491,9,491,17,0,0,4,1]],"UI_process":[[481,9,481,42,0,0,4,0],[482,12,482,46,0,0,4,1],[488,9,488,41,0,0,4,1],[493,17,493,37,0,0,4,0],[493,17,493,37,0,0,4,1],[494,13,494,46,0,0,4,0],[495,16,497,22,0,0,4,1],[504,9,504,35,0,0,4,1],[519,17,519,37,0,0,4,0],[520,13,520,45,0,0,4,0],[524,13,524,20,0,0,4,1],[528,13,528,19,0,0,4,0],[532,13,532,20,0,0,4,1],[545,9,545,43,0,0,4,0],[546,12,546,47,0,0,4,1],[552,9,552,17,0,0,4,0]],"i2d_DHxparams":[[153,9,153,22,0,0,4,1]],"bn_i2c":[[98,9,98,22,0,0,4,0]],"DH_free":[[117,9,117,18,0,0,4,1],[122,9,122,14,0,0,4,1],[126,9,126,24,0,0,4,0],[126,28,126,51,0,0,4,0],[293,12,293,18,2,0,4,1]],"dh_new_intern":[[74,9,74,20,0,0,4,1],[78,9,78,26,0,0,4,1],[84,9,84,45,0,0,4,1],[95,9,95,68,0,0,4,1],[101,9,101,34,0,0,4,0],[101,38,101,59,0,0,4,1]],"open_console":[[373,9,373,44,0,0,4,1],[397,9,397,47,0,0,4,0],[399,9,399,48,0,0,4,0],[404,9,404,49,0,0,4,0],[406,13,406,28,0,0,4,0]],"general_allocate_prompt":[[102,9,102,23,0,0,4,1],[104,17,104,35,0,0,4,0],[106,12,106,30,0,0,4,1],[108,16,108,60,0,0,4,0],[110,22,110,37,0,0,4,1]],"EVP_read_pw_string_min":[[59,9,59,25,0,0,4,1],[62,9,62,19,0,0,4,1],[64,9,66,16,0,0,4,1],[65,13,65,28,0,0,4,1],[67,13,67,19,0,0,4,1]],"UI_free":[[73,9,73,19,0,0,4,1],[75,9,75,45,0,0,4,1]],"asn1_ex_i2c":[[532,9,532,11,0,0,4,0],[532,15,532,27,0,0,4,0]],"ossl_ffc_params_get0_pqg":[[65,9,65,18,0,0,4,0],[67,9,67,18,0,0,4,0],[69,9,69,18,0,0,4,0]],"PEM_ASN1_write_bio_internal":[[353,9,353,19,0,0,4,0]],"ossl_ffc_params_get_validate_params":[[152,9,152,21,0,0,4,0],[154,9,154,24,0,0,4,0],[156,9,156,25,0,0,4,0]],"general_allocate_string":[[128,9,128,18,0,0,4,0],[129,13,129,43,0,0,4,0],[135,17,135,25,0,0,4,1]],"dh_builtin_genparams":[[163,9,163,48,0,0,4,1],[168,9,168,40,0,0,4,0]],"PEM_def_callback":[[42,9,42,17,0,0,4,1],[50,9,50,23,0,0,4,0],[60,15,60,21,0,0,4,1],[63,9,63,15,0,0,4,0]],"allocate_string_stack":[[86,9,86,28,0,0,4,0],[88,13,88,32,0,0,4,1]],"EVP_get_pw_prompt":[[36,9,36,33,0,0,4,0]],"popsig":[[630,17,630,30,0,0,4,0],[630,17,630,30,0,0,4,1],[632,13,632,25,0,0,4,0],[632,13,632,25,0,0,4,1],[636,13,636,25,0,0,4,0],[636,13,636,25,0,0,4,1]],"close_console":[[554,9,554,24,0,0,4,1],[556,9,556,26,0,0,4,1]],"write_string":[[205,13,205,36,0,0,4,1],[206,5,206,19,0,0,4,1],[207,5,207,18,0,0,4,1],[211,5,211,18,0,0,4,1],[212,5,212,20,0,0,4,0],[213,5,213,20,0,0,4,1],[214,5,214,21,0,0,4,1]],"read_string_inner":[[293,9,293,14,0,0,4,0],[293,18,293,37,0,0,4,1],[338,9,338,18,0,0,4,0],[353,9,353,30,0,0,4,1],[355,9,355,17,0,0,4,0],[357,9,357,16,0,0,4,0],[357,20,357,25,0,0,4,0],[357,29,357,46,0,0,4,1],[360,9,360,16,0,0,4,0]],"read_string":[[224,13,224,36,0,0,4,1],[225,5,225,21,0,0,4,1],[232,5,232,20,0,0,4,0],[238,5,238,20,0,0,4,1],[251,5,251,18,0,0,4,1],[252,5,252,18,0,0,4,1],[253,5,253,19,0,0,4,1]]},"library_calls":["BIO_new_fp","d2i_PKCS8PrivateKey_bio","BIO_new_fp","EVP_PKEY_print_private","OSSL_CMP_ITAV_get0_rootCaCert","ossl_check_ASN1_STRING_TABLE_sk_type","X509_CERT_AUX_free","sk_SRP_gN_cmpfunc_thunk","ossl_check_SCT_sk_type","DH_new","DH_generate_parameters_ex","PEM_write_DHxparams","DH_free","EVP_PKEY_free","BIO_free","BIO_free","X509_free","OSSL_CMP_ITAV_free"],"critical_calls":["BIO_new_fp","d2i_PKCS8PrivateKey_bio","BIO_new_fp","EVP_PKEY_print_private","OSSL_CMP_ITAV_get0_rootCaCert","ossl_check_ASN1_STRING_TABLE_sk_type","X509_CERT_AUX_free","sk_SRP_gN_cmpfunc_thunk","ossl_check_SCT_sk_type","DH_new","DH_generate_parameters_ex","PEM_write_DHxparams","DH_free","EVP_PKEY_free","BIO_free","BIO_free","X509_free","OSSL_CMP_ITAV_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create FILE* for writing output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create BIO from input FILE* for PKCS8 private key reading
    BIO *bio_in = BIO_new_fp(in_file, BIO_NOCLOSE);
    if (!bio_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Read PKCS8 private key from input BIO
    EVP_PKEY *pkey = NULL;
    pkey = d2i_PKCS8PrivateKey_bio(bio_in, &pkey, NULL, NULL);
    
    // 5. Create BIO for output
    BIO *bio_out = BIO_new_fp(out_file, BIO_NOCLOSE);
    if (bio_out && pkey) {
        // 6. Print private key to BIO
        EVP_PKEY_print_private(bio_out, pkey, 4, NULL);
    }
    
    // 7. Create dummy structures for other API calls
    
    // For OSSL_CMP_ITAV_get0_rootCaCert
    OSSL_CMP_ITAV *itav = NULL;
    X509 *x509_out = NULL;
    if (itav) {
        OSSL_CMP_ITAV_get0_rootCaCert(itav, &x509_out);
    }
    
    // For ossl_check_ASN1_STRING_TABLE_sk_type
    struct stack_st_ASN1_STRING_TABLE *ast_table_stack = NULL;
    if (ast_table_stack) {
        OPENSSL_STACK *checked_stack = ossl_check_ASN1_STRING_TABLE_sk_type(ast_table_stack);
    }
    
    // For X509_CERT_AUX_free
    X509_CERT_AUX *aux = NULL;
    X509_CERT_AUX_free(aux);
    
    // For sk_SRP_gN_cmpfunc_thunk
    struct stack_st_SRP_gN *srp_gn_stack = NULL;
    if (srp_gn_stack) {
        // Create dummy comparison function
        int (*cmp_func)(const void *, const void *) = [](const void *a, const void *b) -> int {
            return 0;
        };
        sk_SRP_gN_cmpfunc_thunk(cmp_func, NULL, NULL);
    }
    
    // For ossl_check_SCT_sk_type
    struct stack_st_SCT *sct_stack = NULL;
    if (sct_stack) {
        OPENSSL_STACK *checked_sct_stack = ossl_check_SCT_sk_type(sct_stack);
    }
    
    // 8. Generate DH parameters and write to file
    DH *dh = DH_new();
    if (dh) {
        // Generate some DH parameters (using small values for fuzzing)
        DH_generate_parameters_ex(dh, 64, 2, NULL);
        PEM_write_DHxparams(out_file, dh);
        DH_free(dh);
    }
    
    // 9. Clean up all resources
    if (pkey) EVP_PKEY_free(pkey);
    if (bio_in) BIO_free(bio_in);
    if (bio_out) BIO_free(bio_out);
    if (x509_out) X509_free(x509_out);
    if (itav) OSSL_CMP_ITAV_free(itav);
    
    // Note: in_file and out_file are closed by BIO_free of bio_in and bio_out
    // due to BIO_NOCLOSE flag, but we need to close them manually since
    // we didn't pass ownership to BIO
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}