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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.5, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_destroy","vpx_img_wrap","vpx_codec_set_cx_data_buf","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_img_wrap","vpx_codec_set_cx_data_buf","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;  // Too small for meaningful processing
    }

    // Initialize variables
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t* img = nullptr;
    vpx_fixed_buf_t output_buffer;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t* pkt = nullptr;
    vpx_rational_t dsf[2] = {{1, 1}, {2, 2}};  // Downsampling factors for 2 encoders
    
    // Step 1: Get codec interface name
    vpx_codec_iface_t* iface = vpx_codec_vp9_cx();
    const char* iface_name = vpx_codec_iface_name(iface);
    
    // Step 2: Create FILE* for input using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 4: Initialize encoder configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Use input data to modify configuration parameters
    cfg.g_w = (data[0] * 4 + 64) & 0x1FF;  // 64-319 pixels
    cfg.g_h = (data[1] * 4 + 64) & 0x1FF;  // 64-319 pixels
    cfg.rc_target_bitrate = (data[2] * 1000 + 50000) % 1000000;
    cfg.g_threads = (data[3] % 4) + 1;
    cfg.g_lag_in_frames = (data[4] % 16);
    
    // Step 5: Initialize encoder with multiple instances
    if (vpx_codec_enc_init_multi_ver(&codec_ctx, iface, &cfg, 2, 0, dsf, 
                                     VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 6: Set encoder configuration
    if (vpx_codec_enc_config_set(&codec_ctx, &cfg) != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 7: Create image from input data
    // Use remaining input data for image
    size_t img_data_size = cfg.g_w * cfg.g_h * 3 / 2;  // I420 format
    if (img_data_size > size - 5) {
        img_data_size = size - 5;
    }
    
    uint8_t* img_data = (uint8_t*)malloc(img_data_size);
    if (!img_data) {
        vpx_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read image data from input
    fseek(in_file, 5, SEEK_SET);
    fread(img_data, 1, img_data_size, in_file);
    
    img = vpx_img_wrap(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1, img_data);
    
    // Step 8: Set up output buffer
    output_buffer.buf = malloc(size * 2);  // Allocate buffer for encoded data
    output_buffer.sz = size * 2;
    
    // Use input data to determine padding
    unsigned int pad_before = data[5] % 100;
    unsigned int pad_after = data[6] % 100;
    
    if (vpx_codec_set_cx_data_buf(&codec_ctx, &output_buffer, pad_before, pad_after) != VPX_CODEC_OK) {
        // Continue anyway - some encoders might not support this
    }
    
    // Step 9: Use codec control with parameters from input
    int control_ids[] = {VP8E_SET_CPUUSED, VP8E_SET_ENABLEAUTOALTREF, 
                         VP8E_SET_ARNR_MAXFRAMES, VP8E_SET_ARNR_STRENGTH};
    
    for (int i = 0; i < 4 && i + 7 < size; i++) {
        int ctrl_id = control_ids[data[7 + i] % 4];
        int ctrl_value = data[7 + i];
        vpx_codec_control_(&codec_ctx, ctrl_id);
        // Note: The actual value setting would require vpx_codec_control() with additional args
        // but vpx_codec_control_() only takes ctx and ctrl_id
    }
    
    // Step 10: Encode the image
    vpx_codec_encode(&codec_ctx, img, 0, 1, 0, VPX_DL_GOOD_QUALITY);
    
    // Step 11: Get preview frame
    const vpx_image_t* preview_frame = vpx_codec_get_preview_frame(&codec_ctx);
    
    // Step 12: Get encoded data
    while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Write to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Cleanup
    free(output_buffer.buf);
    free(img_data);
    vpx_img_free(img);
    vpx_codec_destroy(&codec_ctx);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}