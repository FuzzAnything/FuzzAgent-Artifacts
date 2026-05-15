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
//<ID> 119
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 2
//<Quality> {"density":15,"unique_branches":{"validate_config":[[125,33,125,49,1,0,4,1]],"vp8e_init":[[740,9,740,13,0,0,4,1]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create FILE* for input data
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

    // Initialize variables
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_stream_info_t stream_info = {0};
    vpx_codec_enc_cfg_t enc_cfg = {0};
    vpx_image_t *img = nullptr;
    vpx_fixed_buf_t *global_headers = nullptr;
    
    // Try both VP8 and VP9 decoders
    vpx_codec_iface_t* decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    vpx_codec_iface_t* encoder_ifaces[] = {
        vpx_codec_vp8_cx(),
        vpx_codec_vp9_cx()
    };
    
    // First, try to peek stream info from input data
    bool peek_success = false;
    for (int i = 0; i < 2; i++) {
        stream_info.sz = sizeof(stream_info);
        if (vpx_codec_peek_stream_info(decoder_ifaces[i], data, size, &stream_info) == VPX_CODEC_OK) {
            peek_success = true;
            
            // Initialize decoder
            if (vpx_codec_dec_init_ver(&decoder, decoder_ifaces[i], nullptr, 0, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
                
                // Get stream info from decoder context
                vpx_codec_get_stream_info(&decoder, &stream_info);
                
                // Initialize encoder with default config
                if (vpx_codec_enc_config_default(encoder_ifaces[i], &enc_cfg, 0) == VPX_CODEC_OK) {
                    enc_cfg.g_w = stream_info.w;
                    enc_cfg.g_h = stream_info.h;
                    
                    if (vpx_codec_enc_init_ver(&encoder, encoder_ifaces[i], &enc_cfg, 0, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                        
                        // Set encoder control parameters
                        int ctrl_id = 0;
                        vpx_codec_control_(&encoder, ctrl_id);
                        
                        // Get global headers
                        global_headers = vpx_codec_get_global_headers(&encoder);
                        
                        // Decode input data
                        if (vpx_codec_decode(&decoder, data, size, nullptr, 0) == VPX_CODEC_OK) {
                            
                            // Get decoded frames
                            vpx_codec_iter_t iter = nullptr;
                            vpx_image_t *frame = nullptr;
                            while ((frame = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
                                
                                // Create output image
                                if (!img) {
                                    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, frame->d_w, frame->d_h, 16);
                                    if (!img) break;
                                }
                                
                                // Copy frame data to image
                                for (int plane = 0; plane < 3; plane++) {
                                    uint8_t* src = frame->planes[plane];
                                    uint8_t* dst = img->planes[plane];
                                    int stride = frame->stride[plane];
                                    int height = plane == 0 ? frame->d_h : frame->d_h / 2;
                                    
                                    for (int y = 0; y < height; y++) {
                                        memcpy(dst, src, stride);
                                        src += stride;
                                        dst += stride;
                                    }
                                }
                                
                                // Encode the frame
                                vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
                                
                                // Get encoded data
                                vpx_codec_iter_t enc_iter = nullptr;
                                const vpx_codec_cx_pkt_t *pkt = nullptr;
                                while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                                    }
                                }
                            }
                        }
                        
                        // Cleanup encoder
                        vpx_codec_destroy(&encoder);
                    }
                }
                
                // Cleanup decoder
                vpx_codec_destroy(&decoder);
            }
            break;
        }
    }
    
    // Cleanup resources
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    if (img) {
        vpx_img_free(img);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}