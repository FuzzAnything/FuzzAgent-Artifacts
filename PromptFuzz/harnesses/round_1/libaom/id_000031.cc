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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_peek_stream_info","aom_codec_destroy","aom_codec_decode","aom_codec_destroy","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_set_rect","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_set_option","aom_codec_encode","aom_codec_get_cx_data","aom_uleb_encode","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_set_rect","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_encode","aom_codec_get_cx_data","aom_uleb_encode","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback functions
static int  id_000031_get_frame_buffer(void *cb_priv, size_t min_size, 
                           aom_codec_frame_buffer_t *fb) {
    static uint8_t buffer[4096 * 4096 * 3];  // Static buffer for simplicity
    fb->data = buffer;
    fb->size = sizeof(buffer);
    return 0;
}

static int  id_000031_release_frame_buffer(void *cb_priv, 
                               aom_codec_frame_buffer_t *fb) {
    // No-op for static buffer
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Get extra version string
    const char *extra_version = aom_codec_version_extra_str();
    (void)extra_version;  // Prevent unused warning

    // 2. Create decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
	return 0;
	}
	

    // 3. Create encoder interface
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
	return 0;
	}
	

    // 4. Initialize decoder
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    if (aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 
                               0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // 5. Set frame buffer functions
    if (aom_codec_set_frame_buffer_functions(&decoder_ctx,  id_000031_get_frame_buffer,
                                             id_000031_release_frame_buffer, nullptr) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder_ctx);
        return 0;
    }

    // 6. Peek at stream info
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    if (aom_codec_peek_stream_info(decoder_iface, data, 
                                   (size > 100) ? 100 : size, &stream_info) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder_ctx);
        return 0;
    }

    // 7. Decode the input
    if (aom_codec_decode(&decoder_ctx, data, size, nullptr) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder_ctx);
        return 0;
    }

    // 8. Get stream info from decoder
    aom_codec_stream_info_t dec_stream_info;
    memset(&dec_stream_info, 0, sizeof(dec_stream_info));
    
    if (aom_codec_get_stream_info(&decoder_ctx, &dec_stream_info) == AOM_CODEC_OK) {
        // Use the stream info if available
    }

    // 9. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    std::vector<aom_image_t*> decoded_frames;
    
    while ((img = aom_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
        decoded_frames.push_back(img);
        
        // 10. Set rectangle on image if valid
        if (img->d_w > 10 && img->d_h > 10) {
            unsigned int x = 0, y = 0;
            unsigned int w = (img->d_w > 8) ? 8 : img->d_w - 1;
            unsigned int h = (img->d_h > 8) ? 8 : img->d_h - 1;
            unsigned int border = 0;
            
            if (aom_img_set_rect(img, x, y, w, h, border) != 0) {
                // Handle error
            }
        }
    }

    // 11. Initialize encoder if we have decoded frames
    aom_codec_ctx_t encoder_ctx;
    aom_codec_enc_cfg_t enc_cfg;
    
    if (!decoded_frames.empty()) {
        // Get default encoder config
        if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
            aom_codec_destroy(&decoder_ctx);
            return 0;
        }
        
        // Configure encoder based on first frame
        aom_image_t *first_frame = decoded_frames[0];
        enc_cfg.g_w = first_frame->d_w;
        enc_cfg.g_h = first_frame->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.g_threads = 1;
        enc_cfg.rc_target_bitrate = 1000;
        
        if (aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg,
                                    0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
            aom_codec_destroy(&decoder_ctx);
            return 0;
        }
        
        // 12. Set encoder options
        aom_codec_set_option(&encoder_ctx, "cpu-used", "1");
        
        // 13. Encode frames
        for (size_t i = 0; i < decoded_frames.size(); ++i) {
            aom_enc_frame_flags_t flags = 0;
            if (i == 0) flags |= AOM_EFLAG_FORCE_KF;
            
            if (aom_codec_encode(&encoder_ctx, decoded_frames[i], 
                                 i, 1, flags) != AOM_CODEC_OK) {
                break;
            }
            
            // 14. Get encoded data
            const aom_codec_cx_pkt_t *pkt = nullptr;
            aom_codec_iter_t enc_iter = nullptr;
            
            while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // Process encoded frame
                    // For fuzzing, just validate it's not null
                    if (pkt->data.frame.buf && pkt->data.frame.sz > 0) {
                        // Valid frame
                    }
                }
            }
        }
        
        // 15. Encode a ULEB value
        uint8_t uleb_buffer[10];
        size_t coded_size = 0;
        uint64_t value_to_encode = size;  // Use input size as value
        
        if (aom_uleb_encode(value_to_encode, sizeof(uleb_buffer), 
                           uleb_buffer, &coded_size) == 0) {
            // ULEB encoding succeeded
        }
        
        // 16. Destroy encoder
        aom_codec_destroy(&encoder_ctx);
    }

    // 17. Destroy decoder
    aom_codec_destroy(&decoder_ctx);

    return 0;
}