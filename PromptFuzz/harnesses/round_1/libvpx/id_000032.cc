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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


// Include libvpx headers
extern "C" {
}

// Custom structure to hold callback context
struct CallbackContext {
    vpx_codec_ctx_t* encoder;
    FILE* output_file;
    bool encoder_initialized;
    vpx_codec_enc_cfg_t cfg;
    vpx_fixed_buf_t global_headers;
};

// Callback function for when frames are decoded
void  id_000032_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    CallbackContext* ctx = static_cast<CallbackContext*>(user_priv);
    
    if (!ctx || !ctx->encoder) return;
    
    // Initialize encoder on first frame if not done
    if (!ctx->encoder_initialized) {
        // Get default encoder config
        vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
        vpx_codec_enc_config_default(encoder_iface, &ctx->cfg, 0); // 0 for good quality
        
        // Set dimensions from decoded frame
        ctx->cfg.g_w = img->d_w;
        ctx->cfg.g_h = img->d_h;
        ctx->cfg.g_timebase.num = 1;
        ctx->cfg.g_timebase.den = 30; // 30 fps
        
        // Initialize encoder
        if (vpx_codec_enc_init_ver(ctx->encoder, encoder_iface, &ctx->cfg, 0, 
                                   VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
            
            // Use control API to set some parameter
            vpx_codec_control_(ctx->encoder, VP8E_SET_CPUUSED, 6);
            
            // Get global headers
            ctx->global_headers = *vpx_codec_get_global_headers(ctx->encoder);
            if (ctx->global_headers.buf && ctx->global_headers.sz > 0) {
                fwrite(ctx->global_headers.buf, 1, ctx->global_headers.sz, ctx->output_file);
            }
            
            ctx->encoder_initialized = true;
        }
    }
    
    // If encoder initialized, encode the frame
    if (ctx->encoder_initialized) {
        // Create a buffer for encoded data
        std::vector<uint8_t> buffer(4096);
        vpx_fixed_buf_t output_buf;
        output_buf.buf = buffer.data();
        output_buf.sz = buffer.size();
        
        // Set buffer for encoded data
        vpx_codec_set_cx_data_buf(ctx->encoder, &output_buf, 0, 0);
        
        // Encode the frame
        vpx_codec_encode(ctx->encoder, img, 0, 1, 0, VPX_DL_REALTIME);
        
        // Get encoded data
        vpx_codec_iter_t iter = nullptr;
        const vpx_codec_cx_pkt_t* pkt;
        while ((pkt = vpx_codec_get_cx_data(ctx->encoder, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, ctx->output_file);
            }
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    FILE* output_file = fopen("output_file", "wb");
    if (!output_file) {
	assert_file_closed(&output_file);
	return 0;
	}
	
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0,
                              VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&output_file);;
        assert_file_closed(&output_file);
	return 0;
    }
    
    // Initialize encoder context
    vpx_codec_ctx_t encoder;
    CallbackContext callback_ctx;
    callback_ctx.encoder = &encoder;
    callback_ctx.output_file = output_file;
    callback_ctx.encoder_initialized = false;
    callback_ctx.global_headers = {nullptr, 0};
    
    // Register callback
    vpx_codec_register_put_frame_cb(&decoder,  id_000032_put_frame_callback, &callback_ctx);
    
    // Decode the input data - this will trigger callbacks
    vpx_codec_decode(&decoder, data, static_cast<unsigned int>(size), nullptr, 0);
    
    // Iterate through decoded frames to ensure all callbacks are triggered
    vpx_codec_iter_t iter = nullptr;
    while (vpx_codec_get_frame(&decoder, &iter) != nullptr) {
        // Callback already handles everything
    }
    
    // Cleanup
    if (callback_ctx.encoder_initialized) {
        vpx_codec_destroy(&encoder);
        if (callback_ctx.global_headers.buf) {
            free(callback_ctx.global_headers.buf);
        }
    }
    
    vpx_codec_destroy(&decoder);
    assert_file_closed(&output_file);;
    
    assert_file_closed(&output_file);
	return 0;
}