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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 2
//<Quality> {"density":23,"unique_branches":{"validate_img":[[293,5,293,26,0,0,4,1],[295,5,295,12,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_error_detail","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_error_detail","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_enc_config_set","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_error_detail","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_error_detail","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_enc_config_set","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include libvpx headers
extern "C" {
}

// Frame buffer callback context
struct FrameBufferContext {
    const uint8_t* data;
    size_t size;
    size_t offset;
    bool used;
};

extern "C" {
    // Frame buffer callbacks
    int get_frame_buffer(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
        FrameBufferContext* ctx = static_cast<FrameBufferContext*>(cb_priv);
        if (ctx->offset + min_size > ctx->size) {
            return -1;
        }
        fb->data = const_cast<uint8_t*>(ctx->data + ctx->offset);
        fb->size = min_size;
        ctx->offset += min_size;
        ctx->used = true;
        return 0;
    }
    
    int release_frame_buffer(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
        // Nothing to do since we don't own the memory
        return 0;
    }
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create a FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize decoder for VP9
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps; // Use caps to avoid unused variable warning
    
    // Initialize decoder
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, NULL, 0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get stream info
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    if (vpx_codec_peek_stream_info(decoder_iface, data, 
                                   (size > 1000) ? 1000 : (unsigned int)size, 
                                   &stream_info) == VPX_CODEC_OK) {
        // Use vpx_codec_get_stream_info on decoder context
        if (vpx_codec_get_stream_info(&decoder, &stream_info) != VPX_CODEC_OK) {
            const char* error_detail = vpx_codec_error_detail(&decoder);
            (void)error_detail; // Use error_detail to avoid unused variable warning
        }
    }
    
    // Decode the input data
    if (vpx_codec_decode(&decoder, data, (unsigned int)size, NULL, 0) != VPX_CODEC_OK) {
        const char* error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail; // Use error_detail to avoid unused variable warning
    }
    
    // Get decoded frame
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t* decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    // Initialize encoder for VP8
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
    
    if (encoder_iface && decoded_img) {
        vpx_codec_enc_cfg_t cfg;
        
        // Get default encoder configuration
        if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) == VPX_CODEC_OK) {
            // Set image dimensions from decoded frame
            cfg.g_w = decoded_img->d_w;
            cfg.g_h = decoded_img->d_h;
            cfg.g_timebase.num = 1;
            cfg.g_timebase.den = 30;
            cfg.rc_target_bitrate = 1000;
            cfg.g_threads = 1;
            
            // Initialize encoder
            if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                // Set frame buffer functions
                FrameBufferContext fb_ctx = {data, size, 0, false};
                if (vpx_codec_set_frame_buffer_functions(&encoder, get_frame_buffer, 
                                                       release_frame_buffer, &fb_ctx) != VPX_CODEC_OK) {
                    const char* error_detail = vpx_codec_error_detail(&encoder);
                    (void)error_detail; // Use error_detail to avoid unused variable warning
                }
                
                // Allocate image for encoding
                vpx_image_t* img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 
                                                decoded_img->d_w, decoded_img->d_h, 16);
                if (img) {
                    // Try to encode the decoded frame
                    if (vpx_codec_encode(&encoder, decoded_img, 0, 1, 0, VPX_DL_REALTIME) == VPX_CODEC_OK) {
                        // Try to get preview frame
                        const vpx_image_t* preview = vpx_codec_get_preview_frame(&encoder);
                        (void)preview; // Use preview to avoid unused variable warning
                        
                        // Get encoded data
                        const vpx_codec_cx_pkt_t* pkt;
                        vpx_codec_iter_t enc_iter = NULL;
                        
                        while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                // Write to output file
                                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                            }
                        }
                    }
                    
                    // Try to change encoder configuration
                    cfg.rc_target_bitrate = 500;
                    if (vpx_codec_enc_config_set(&encoder, &cfg) != VPX_CODEC_OK) {
                        const char* error_detail = vpx_codec_error_detail(&encoder);
                        (void)error_detail; // Use error_detail to avoid unused variable warning
                    }
                    
                    vpx_img_free(img);
                }
                
                vpx_codec_destroy(&encoder);
            }
        }
    }
    
    // Clean up
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}