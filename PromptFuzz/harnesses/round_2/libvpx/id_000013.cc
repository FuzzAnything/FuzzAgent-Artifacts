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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_codec_version_str","vpx_codec_get_caps","vpx_codec_vp9_cx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_img_alloc","vpx_img_set_rect","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_get_caps","vpx_codec_vp9_cx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_img_alloc","vpx_img_set_rect","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


// Include necessary libvpx headers

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get version string
    const char *version_str = vpx_codec_version_str();
    
    // 2. Get codec capabilities for VP9 encoder
    vpx_codec_caps_t caps = vpx_codec_get_caps(vpx_codec_vp9_cx());
    
    // 3. Create memory file for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 4. Get file descriptor
    int fd = fuzz_fileno(in_file);
    (void)fd; // Suppress unused warning
    
    // 5. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Initialize stream info structure
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // 7. Initialize decoder context for stream info
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, vpx_codec_vp9_dx(), 
                                                 &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res == VPX_CODEC_OK) {
        // 8. Get stream info from the input data
        vpx_codec_get_stream_info(&decoder, &stream_info);
        
        // 9. Clean up decoder
        vpx_codec_destroy(&decoder);
    }
    
    // 10. Initialize encoder context with multiple encoders
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    
    // Get default encoder configuration
    vpx_codec_enc_config_default(iface, &enc_cfg, 0);
    
    // Set width/height from stream info if available, otherwise use defaults
    if (stream_info.w > 0 && stream_info.h > 0) {
        enc_cfg.g_w = stream_info.w;
        enc_cfg.g_h = stream_info.h;
    } else {
        enc_cfg.g_w = 640;
        enc_cfg.g_h = 480;
    }
    
    // 11. Initialize multi-encoder (2 encoders)
    vpx_rational_t dsf[2] = {{1, 1}, {2, 1}}; // Downsampling factors
    res = vpx_codec_enc_init_multi_ver(&encoder, iface, &enc_cfg, 2, 0, dsf, 
                                       VPX_ENCODER_ABI_VERSION);
    
    if (res == VPX_CODEC_OK) {
        // 12. Set encoder configuration
        vpx_codec_enc_config_set(&encoder, &enc_cfg);
        
        // 13. Allocate image
        vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 
                                         enc_cfg.g_w, enc_cfg.g_h, 16);
        if (img) {
            // 14. Set rectangle on image (use first 4 bytes for coordinates)
            if (size >= 4) {
                unsigned int x = data[0] % enc_cfg.g_w;
                unsigned int y = data[1] % enc_cfg.g_h;
                unsigned int w = data[2] % (enc_cfg.g_w - x);
                unsigned int h = data[3] % (enc_cfg.g_h - y);
                vpx_img_set_rect(img, x, y, w, h);
            }
            
            // 15. Create output buffer
            vpx_fixed_buf_t output_buf;
            output_buf.buf = malloc(4096);
            output_buf.sz = 4096;
            
            if (output_buf.buf) {
                // 16. Set output buffer for encoder
                vpx_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
                
                // Encode one frame (dummy frame with zero data)
                vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
                
                // 17. Get encoded data
                vpx_codec_iter_t iter = NULL;
                const vpx_codec_cx_pkt_t *pkt;
                while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        // Write to output file
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                free(output_buf.buf);
            }
            
            // 18. Free image
            vpx_img_free(img);
        }
        
        // 19. Clean up encoder
        vpx_codec_destroy(&encoder);
    }
    
    // 20. Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}