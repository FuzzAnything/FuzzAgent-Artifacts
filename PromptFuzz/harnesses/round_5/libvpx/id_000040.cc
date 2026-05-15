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
#include <vpx/vp8cx.h>
}
extern "C" {
#include <vpx/vp8dx.h>
}
extern "C" {
#include <vpx/vpx_decoder.h>
}
//<ID> 131
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"rescale":[[272,7,272,24,0,0,4,1]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_enc_config_set","vpx_codec_error_detail","vpx_codec_get_global_headers","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_error_detail","vpx_codec_get_global_headers","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Need minimum data for configuration
    }

    // Create a file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize encoder configuration
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    
    // Get default configuration
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read some configuration parameters from input data
    fseek(in_file, 0, SEEK_SET);
    fread(&cfg.g_w, sizeof(unsigned int), 1, in_file);
    fread(&cfg.g_h, sizeof(unsigned int), 1, in_file);
    fread(&cfg.rc_target_bitrate, sizeof(unsigned int), 1, in_file);
    
    // Clamp values to reasonable ranges
    cfg.g_w = cfg.g_w % 1000 + 100;
    cfg.g_h = cfg.g_h % 1000 + 100;
    cfg.rc_target_bitrate = cfg.rc_target_bitrate % 1000000 + 1000;

    // Initialize encoder with version
    err = vpx_codec_enc_init_ver(&codec_ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    // Use error APIs
    if (err != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(err);
        const char *error_detail = vpx_codec_error_detail(&codec_ctx);
        (void)error_str;
        (void)error_detail;
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Try to set configuration again (this might fail but we want to test the API)
    err = vpx_codec_enc_config_set(&codec_ctx, &cfg);
    if (err != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&codec_ctx);
        (void)error_detail;
    }

    // Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec_ctx);
    if (global_headers) {
        // Write headers to output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(global_headers->buf, 1, global_headers->sz, out_file);
            assert_file_closed(&out_file);;
        }
    }

    // Clean up
    vpx_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}