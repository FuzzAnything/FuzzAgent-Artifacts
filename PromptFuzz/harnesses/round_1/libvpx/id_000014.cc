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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_control_","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_set_cx_data_buf","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_enc_config_set","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_control_","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_set_cx_data_buf","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_enc_config_set","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create input file from the fuzzer data
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
    
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_fixed_buf_t output_buf;
    vpx_codec_iter_t iter = NULL;
    std::vector<uint8_t> read_buffer;
    
    // Initialize the codec interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    
    // Get codec interface name
    const char *codec_name = vpx_codec_iface_name(iface);
    
    // Initialize encoder config with default values
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set some configuration parameters from fuzzer data
    if (size > 0) {
        cfg.g_w = (data[0] % 640) + 1;  // Width between 1-640
        cfg.g_h = (size > 1 ? (data[1] % 480) + 1 : 1);  // Height between 1-480
    }
    
    // Initialize encoder with multiple instances (num_enc = 1)
    if (vpx_codec_enc_init_multi_ver(&encoder, iface, &cfg, 1, 0, nullptr, 
                                      VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Apply codec control (example: set CPU used)
    vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
    
    // Read fuzzer data and try to encode
    read_buffer.resize(size);
    fread(read_buffer.data(), 1, size, in_file);
    
    // Create an image from the input data
    vpx_image_t img;
    unsigned int width = cfg.g_w;
    unsigned int height = cfg.g_h;
    
    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, width, height, 1) != nullptr) {
        // Fill image with some data from fuzzer input
        size_t plane_size = width * height;
        size_t uv_plane_size = (width / 2) * (height / 2);
        
        if (read_buffer.size() >= plane_size + 2 * uv_plane_size) {
            memcpy(img.planes[0], read_buffer.data(), plane_size);
            memcpy(img.planes[1], read_buffer.data() + plane_size, uv_plane_size);
            memcpy(img.planes[2], read_buffer.data() + plane_size + uv_plane_size, uv_plane_size);
        }
        
        // Encode the image
        vpx_codec_encode(&encoder, &img, 0, 1, 0, VPX_DL_REALTIME);
        
        // Get preview frame if available
        const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
        
        // Set up output buffer for encoded data
        output_buf.sz = 1024 * 1024;  // 1MB buffer
        output_buf.buf = malloc(output_buf.sz);
        
        if (output_buf.buf) {
            // Set the output buffer for encoded data
            vpx_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
            
            // Get encoded data packets
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Write encoded data to output file
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
            
            free(output_buf.buf);
        }
        
        vpx_img_free(&img);
    }
    
    // Try to update encoder configuration mid-stream
    if (size > 10) {
        cfg.rc_target_bitrate = (data[5] % 1000) + 100;
        vpx_codec_enc_config_set(&encoder, &cfg);
    }
    
    // Cleanup
    vpx_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}