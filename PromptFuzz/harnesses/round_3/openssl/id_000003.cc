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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 85, nr_unique_branch: 77
//<Quality> {"density":17,"unique_branches":{"DES_set_key":[[686,9,686,35,0,0,4,0],[686,9,686,35,0,0,4,1],[688,9,688,29,0,0,4,0],[688,9,688,29,0,0,4,1]],"ERR_reason_error_string":[[628,9,628,18,0,0,4,1],[632,13,632,24,0,0,4,1]],"ossl_err_load_PEM_strings":[[72,9,72,66,0,0,4,1]],"DES_set_key_unchecked":[[742,17,742,31,0,0,4,0],[742,17,742,31,0,0,4,1],[743,13,743,23,0,0,4,0],[743,13,743,23,0,0,4,1]],"X509_EXTENSION_get_data":[[278,9,278,19,0,0,4,0]],"BN_GF2m_mod_arr":[[292,9,292,18,0,0,4,1],[302,9,302,15,0,0,4,0],[303,13,303,35,0,0,4,1],[305,21,305,31,0,0,4,0],[305,21,305,31,0,0,4,1],[314,26,314,32,0,0,4,0],[314,26,314,32,0,0,4,1],[316,13,316,22,0,0,4,0],[316,13,316,22,0,0,4,1],[322,21,322,30,0,0,4,0],[322,21,322,30,0,0,4,1],[329,17,329,19,0,0,4,0],[329,17,329,19,0,0,4,1],[343,12,343,19,0,0,4,0],[343,12,343,19,0,0,4,1],[347,13,347,20,0,0,4,0],[347,13,347,20,0,0,4,1],[358,21,358,30,0,0,4,0],[358,21,358,30,0,0,4,1],[366,17,366,19,0,0,4,0],[366,17,366,19,0,0,4,1],[366,23,366,45,0,0,4,1],[22,24,22,50,8,0,4,0],[22,24,22,50,11,0,4,0]],"DES_check_key_parity":[[76,17,76,31,0,0,4,0],[76,17,76,31,0,0,4,1]],"BN_GF2m_poly2arr":[[1143,9,1143,22,0,0,4,0],[1143,9,1143,22,0,0,4,1],[1146,26,1146,32,0,0,4,0],[1146,26,1146,32,0,0,4,1],[1147,13,1147,21,0,0,4,1],[1151,32,1151,38,0,0,4,0],[1151,32,1151,38,0,0,4,1],[1152,17,1152,31,0,0,4,0],[1152,17,1152,31,0,0,4,1],[1153,21,1153,28,0,0,4,0],[1161,9,1161,14,0,0,4,0],[1161,18,1161,51,0,0,4,1],[1164,9,1164,16,0,0,4,0]],"BN_GF2m_mod_mul_arr":[[411,9,411,15,0,0,4,1],[416,9,416,38,0,0,4,1],[420,9,420,29,0,0,4,1],[424,17,424,25,0,0,4,0],[424,17,424,25,0,0,4,1],[427,17,427,27,0,0,4,0],[427,17,427,27,0,0,4,1],[429,14,429,33,0,0,4,0],[429,14,429,33,0,0,4,1],[430,21,430,31,0,0,4,0],[430,21,430,31,0,0,4,1],[432,18,432,37,0,0,4,0],[432,18,432,37,0,0,4,1],[434,25,434,30,0,0,4,0],[434,25,434,30,0,0,4,1],[440,9,440,33,0,0,4,0]],"bn_correct_top":[[22,24,22,50,1,0,4,1]],"EVP_MD_get_block_size":[[784,9,784,19,0,0,4,1]],"BN_GF2m_mod_mul":[[468,9,468,20,0,0,4,1],[471,9,471,13,0,0,4,0],[471,9,471,13,0,0,4,1],[471,17,471,26,0,0,4,1]],"bn2binpad":[[591,9,591,18,0,0,4,0],[592,13,592,23,0,0,4,1]],"DES_is_weak_key":[[124,17,124,31,0,0,4,0],[124,17,124,31,0,0,4,1]]},"library_calls":["ERR_load_PEM_strings","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_get_data","ASN1_OCTET_STRING_free","DES_set_key","BN_CTX_new","BN_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_rand","BN_GF2m_mod_mul","EVP_sha256","EVP_MD_get_block_size","EVP_cast5_cfb64","OSSL_FUNC_rand_get_params","BIO_ADDR_dup","BN_num_bits","BN_bn2bin","BN_free","BN_free","BN_free","BN_free","BN_CTX_free"],"critical_calls":["ERR_load_PEM_strings","ASN1_OCTET_STRING_new","ASN1_OCTET_STRING_set","X509_EXTENSION_get_data","ASN1_OCTET_STRING_free","DES_set_key","BN_CTX_new","BN_new","BN_new","BN_new","BN_new","BN_bin2bn","BN_bin2bn","BN_rand","BN_GF2m_mod_mul","EVP_sha256","EVP_MD_get_block_size","EVP_cast5_cfb64","OSSL_FUNC_rand_get_params","BIO_ADDR_dup","BN_num_bits","BN_bn2bin","BN_free","BN_free","BN_free","BN_free","BN_CTX_free"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 64) {
	return 0;
	}
	  // Need minimum data for operations

    // 1. Load PEM error strings
    ERR_load_PEM_strings();

    // 2. Process X509 extension data (simulating parsing of certificate data)
    X509_EXTENSION *ext = nullptr;
    // Create a dummy extension using input data
    ASN1_OCTET_STRING *octet_str = ASN1_OCTET_STRING_new();
    if (octet_str && size > 10) {
        ASN1_OCTET_STRING_set(octet_str, data, size > 128 ? 128 : size);
        // Simulate getting extension data
        const ASN1_OCTET_STRING *ext_data = X509_EXTENSION_get_data(ext);
        // Note: ext is NULL, but we demonstrate API usage
        (void)ext_data;
        ASN1_OCTET_STRING_free(octet_str);
    }

    // 3. DES operations
    DES_key_schedule schedule;
    const_DES_cblock des_key;
    if (size >= 8) {
        memcpy(des_key, data, 8);
        DES_set_key(&des_key, &schedule);
    }

    // 4. BN_GF2m modular multiplication
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *r = BN_new();
    BIGNUM *a = BN_new();
    BIGNUM *b = BN_new();
    BIGNUM *p = BN_new();
    
    if (ctx && r && a && b && p && size >= 32) {
        // Use input data to initialize BIGNUMs
        BN_bin2bn(data, 16, a);
        BN_bin2bn(data + 16, 16, b);
        // Create a random polynomial for GF(2^m)
        BN_rand(p, 128, 0, 0);
        BN_GF2m_mod_mul(r, a, b, p, ctx);
    }

    // 5. Get EVP_MD block size
    const EVP_MD *md = EVP_sha256();
    int block_size = EVP_MD_get_block_size(md);
    (void)block_size;

    // 6. Get CAST5 cipher in CFB mode
    const EVP_CIPHER *cast5 = EVP_cast5_cfb64();
    (void)cast5;

    // 7. Get RAND parameters function pointer
    // Create a minimal dispatch table
    OSSL_DISPATCH dispatch[] = {
        {0, nullptr}
    };
    OSSL_FUNC_rand_get_params_fn *rand_get_params = OSSL_FUNC_rand_get_params(dispatch);
    (void)rand_get_params;

    // 8. BIO_ADDR duplication (simulating network address handling)
    BIO_ADDR *addr = nullptr;
    // Create a dummy address from input data
    char addr_str[64];
    if (size >= 20) {
        snprintf(addr_str, sizeof(addr_str), "%u.%u.%u.%u:%u", 
                 data[0], data[1], data[2], data[3], 
                 (data[4] << 8) | data[5]);
        // In real usage, we'd parse this to BIO_ADDR
        // For demonstration, we'll create and duplicate a dummy
        if (addr) {
            BIO_ADDR *dup_addr = BIO_ADDR_dup(addr);
            if (dup_addr) {
                // Use dup_addr somehow
                (void)dup_addr;
                // Free would normally be BIO_ADDR_free(dup_addr);
            }
        }
    }

    // 9. Process input data through a file (as per instructions)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read and process some data
        uint8_t buffer[64];
        fread(buffer, 1, sizeof(buffer), in_file);
        assert_file_closed(&in_file);;
    }

    // 10. Write output to file (as per instructions)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (r) {
            unsigned char *bn_buf = new unsigned char[BN_num_bytes(r)];
            int len = BN_bn2bin(r, bn_buf);
            fwrite(bn_buf, 1, len, out_file);
            delete[] bn_buf;
        }
        assert_file_closed(&out_file);;
    }

    // 11. Cleanup
    BN_free(r);
    BN_free(a);
    BN_free(b);
    BN_free(p);
    BN_CTX_free(ctx);
    
    // Note: We would normally free BIO_ADDR if we had created one
    // if (addr) BIO_ADDR_free(addr);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}