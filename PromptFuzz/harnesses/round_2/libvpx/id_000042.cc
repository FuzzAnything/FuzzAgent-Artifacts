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
//<ID> 152
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode a VP9 video stream and re-encode it:

*/


extern "C" {
}

// Custom frame buffer callback structures
struct FrameBufferData {
    std::vector<uint8_t> buffer;
    size_t size;
};

static int  id_000042_get_frame_buffer_cb(void *cb_priv, size_t min_size, 
                               vpx_codec_frame_buffer_t *fb) {
    FrameBufferData *fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    // Ensure buffer is large enough
    if (fb_data->buffer.size() < min_size) {
        fb_data->buffer.resize(min_size);
        fb_data->size = min_size;
    }
    
    fb->data = fb_data->buffer.data();
    fb->size = fb_data->size;
    fb->priv = nullptr;
    
    return 0;
}

static int  id_000042_release_frame_buffer_cb(void *cb_priv, 
                                   vpx_codec_frame_buffer_t *fb) {
    // No special cleanup needed since we manage memory ourselves
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Get decoder interface and its properties
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) {
	return 0;
	}
	
    
    const char *iface_name = vpx_codec_iface_name(decoder_iface);
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    
    // 2. Initialize decoder context
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t ret = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, 
                                                  &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }
    
    // 3. Set up frame buffer callbacks
    FrameBufferData frame_buffer_data;
    ret = vpx_codec_set_frame_buffer_functions(&decoder_ctx, 
                                                id_000042_get_frame_buffer_cb,
                                                id_000042_release_frame_buffer_cb,
                                               &frame_buffer_data);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // 4. Peek stream info from input data
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_err_t peek_ret = vpx_codec_peek_stream_info(decoder_iface, 
                                                          data, size, &stream_info);
    
    // 5. Decode the input data
    ret = vpx_codec_decode(&decoder_ctx, data, size, nullptr, 0);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // 6. Get stream info after decoding
    ret = vpx_codec_get_stream_info(&decoder_ctx, &stream_info);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // 7. Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *img = vpx_codec_get_frame(&decoder_ctx, &iter);
    
    // 8. If we got a decoded frame, try to re-encode it
    if (img && stream_info.w > 0 && stream_info.h > 0) {
        // Get encoder interface
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
        if (encoder_iface) {
            const char *enc_name = vpx_codec_iface_name(encoder_iface);
            vpx_codec_caps_t enc_caps = vpx_codec_get_caps(encoder_iface);
            
            // Initialize encoder context
            vpx_codec_ctx_t encoder_ctx;
            vpx_codec_enc_cfg_t enc_cfg;
            
            ret = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            if (ret == VPX_CODEC_OK) {
                // Configure encoder with decoded frame dimensions
                enc_cfg.g_w = stream_info.w;
                enc_cfg.g_h = stream_info.h;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.rc_target_bitrate = 1000;
                enc_cfg.g_pass = VPX_RC_ONE_PASS;
                enc_cfg.g_lag_in_frames = 0;
                enc_cfg.g_error_resilient = 0;
                
                // Call vpx_codec_enc_init_ver as required
                ret = vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, 
                                             &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
                
                if (ret == VPX_CODEC_OK) {
                    // Create output file for encoded data
                    FILE *out_file = fopen("output_file", "wb");
                    if (out_file) {
                        // Encode the decoded frame
                        ret = vpx_codec_encode(&encoder_ctx, img, 0, 1, 0, VPX_DL_REALTIME);
                        
                        // Get encoded data
                        vpx_codec_iter_t enc_iter = nullptr;
                        const vpx_codec_cx_pkt_t *pkt;
                        
                        while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
                            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                            }
                        }
                        
                        assert_file_closed(&out_file);;
                    }
                    
                    // Get global headers (optional)
                    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder_ctx);
                    
                    vpx_codec_destroy(&encoder_ctx);
                }
            }
        }
    }
    
    // 9. Clean up
    vpx_codec_destroy(&decoder_ctx);
    
    return 0;
}