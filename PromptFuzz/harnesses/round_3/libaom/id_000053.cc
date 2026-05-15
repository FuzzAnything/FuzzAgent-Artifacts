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
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_error_detail","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_av1_cx","aom_img_num_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_error_detail","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_av1_cx","aom_img_num_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000053_get_frame_buffer(void *cb_priv, size_t min_size, 
                           aom_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

static int  id_000053_release_frame_buffer(void *cb_priv, 
                               aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Initialize decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_caps_t decoder_caps = aom_codec_get_caps(decoder_iface);
    
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, 
                                                 &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        const char *detail = aom_codec_error_detail(&decoder_ctx);
        (void)detail; // Use detail to satisfy API requirement
        return 0;
    }
    
    // Set frame buffer functions
    res = aom_codec_set_frame_buffer_functions(&decoder_ctx,  id_000053_get_frame_buffer, 
                                               id_000053_release_frame_buffer, nullptr);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // Peek stream info
    aom_codec_stream_info_t stream_info = {0};
    res = aom_codec_peek_stream_info(decoder_iface, data, 
                                     size > 100 ? 100 : size, &stream_info);
    
    // Decode the input
    res = aom_codec_decode(&decoder_ctx, data, size, nullptr);
    if (res != AOM_CODEC_OK) {
        const char *detail = aom_codec_error_detail(&decoder_ctx);
        (void)detail; // Use detail
    }
    
    // Get stream info after decode
    aom_codec_get_stream_info(&decoder_ctx, &stream_info);
    
    // Try to get a decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = aom_codec_get_frame(&decoder_ctx, &iter);
    
    // Initialize encoder if we have a valid image
    aom_codec_ctx_t encoder_ctx;
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    if (img) {
        // Check metadata
        size_t metadata_count = aom_img_num_metadata(img);
        
        // Create and free a metadata object (even if not attaching to image)
        if (metadata_count > 0) {
            aom_metadata_t *metadata = aom_img_metadata_alloc(
                1, data, size > 100 ? 100 : size, AOM_MIF_ANY_FRAME);
            if (metadata) {
                aom_img_metadata_free(metadata);
            }
        }
        
        // Get encoder caps
        aom_codec_caps_t encoder_caps = aom_codec_get_caps(encoder_iface);
        
        // Setup encoder config
        res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        if (res == AOM_CODEC_OK) {
            enc_cfg.g_w = img->d_w;
            enc_cfg.g_h = img->d_h;
            enc_cfg.g_threads = 1;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_error_resilient = 0;
            
            res = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 
                                         0, AOM_ENCODER_ABI_VERSION);
            if (res == AOM_CODEC_OK) {
                // Get global headers
                aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder_ctx);
                if (global_headers) {
                    free(global_headers->buf);
                    free(global_headers);
                }
                
                // Set output buffer
                uint8_t *output_buf = (uint8_t *)malloc(4096);
                if (output_buf) {
                    aom_fixed_buf_t buf;
                    buf.buf = output_buf;
                    buf.sz = 4096;
                    aom_codec_set_cx_data_buf(&encoder_ctx, &buf, 0, 0);
                    
                    // Encode the frame
                    res = aom_codec_encode(&encoder_ctx, img, 0, 1, 0);
                    
                    // Get encoded data
                    iter = nullptr;
                    const aom_codec_cx_pkt_t *pkt;
                    while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &iter)) != nullptr) {
                        // Process packet if needed
                    }
                    
                    free(output_buf);
                }
                
                aom_codec_destroy(&encoder_ctx);
            }
        }
    }
    
    aom_codec_destroy(&decoder_ctx);
    return 0;
}