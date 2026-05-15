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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_control_","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_control_","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Setup input stream using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get decoder capabilities
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
    
    // 3. Create decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (vpx_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Read compressed data from input stream
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    uint8_t *compressed_data = (uint8_t *)malloc(file_size);
    if (!compressed_data) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(compressed_data, 1, file_size, in_file);
    
    // 5. Peek stream info to get frame dimensions
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    
    if (vpx_codec_peek_stream_info(decoder_iface, compressed_data, bytes_read, &stream_info) == VPX_CODEC_OK) {
        // 6. Setup encoder configuration using default settings
        vpx_codec_enc_cfg_t enc_cfg;
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
        
        if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == VPX_CODEC_OK) {
            // Set encoder dimensions from stream info
            enc_cfg.g_w = stream_info.w;
            enc_cfg.g_h = stream_info.h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_pass = VPX_RC_ONE_PASS;
            
            // 7. Create encoder context
            vpx_codec_ctx_t encoder;
            if (vpx_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0) == VPX_CODEC_OK) {
                // 8. Use codec control API to set encoder parameters
                vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 6);
                vpx_codec_control_(&encoder, VP9E_SET_TILE_COLUMNS, 2);
                
                // 9. Decode the input frame
                if (vpx_codec_decode(&decoder, compressed_data, bytes_read, NULL, 0) == VPX_CODEC_OK) {
                    vpx_codec_iter_t iter = NULL;
                    vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder, &iter);
                    
                    if (decoded_frame) {
                        // 10. Create a wrapped image for encoding
                        vpx_image_t wrapped_img;
                        unsigned int stride_align = 1;
                        
                        // Use vpx_img_wrap to wrap the decoded image data
                        vpx_image_t *wrapped = vpx_img_wrap(&wrapped_img,
                                                           decoded_frame->fmt,
                                                           decoded_frame->d_w,
                                                           decoded_frame->d_h,
                                                           stride_align,
                                                           decoded_frame->img_data);
                        
                        if (wrapped) {
                            // 11. Encode the wrapped image
                            vpx_codec_encode(&encoder, wrapped, 0, 1, 0, VPX_DL_REALTIME);
                            
                            // 12. Get encoded data
                            vpx_codec_iter_t enc_iter = NULL;
                            const vpx_codec_cx_pkt_t *pkt;
                            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                    // Encoded frame data available
                                    // Could write to file if needed
                                }
                            }
                        }
                    }
                }
                
                // 13. Clean up encoder
                vpx_codec_destroy(&encoder);
            }
        }
    }
    
    // 14. Clean up resources
    free(compressed_data);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}