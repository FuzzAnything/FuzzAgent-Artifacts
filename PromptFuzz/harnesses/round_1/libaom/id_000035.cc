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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 76, nr_unique_branch: 3
//<Quality> {"density":19,"unique_branches":{"does_level_match":[[460,10,460,43,0,0,4,1]],"set_bitstream_level_tier":[[475,14,476,33,0,0,4,0]],"init_seq_coding_tools":[[625,7,625,34,0,0,4,1]]},"library_calls":["aom_codec_version_str","aom_obu_type_to_string","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_set_frame_buffer_functions","aom_codec_error","aom_codec_destroy","aom_codec_decode","aom_codec_error","aom_codec_destroy","aom_codec_get_stream_info","aom_codec_error","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_img_remove_metadata","aom_img_free","aom_codec_av1_cx","aom_codec_destroy","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_error","aom_codec_destroy","aom_codec_enc_config_set","aom_codec_error","aom_codec_get_global_headers","aom_img_alloc","aom_img_plane_height","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_obu_type_to_string","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_error","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_img_remove_metadata","aom_img_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_error","aom_codec_get_global_headers","aom_img_alloc","aom_img_plane_height","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


// Include necessary libaom headers
extern "C" {
}

// Frame buffer callbacks for decoder
int  id_000035_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    fb->priv = fb->data;
    return 0;
}

int  id_000035_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    free(fb->priv);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Get and print version string
    const char *version_str = aom_codec_version_str();
    (void)version_str; // Suppress unused warning

    // 2. Convert an OBU type to string (using a known OBU type)
    const char *obu_str = aom_obu_type_to_string(OBU_FRAME);
    (void)obu_str;

    // 3. Create decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
	return 0;
	}
	

    // 4. Get decoder interface name
    const char *iface_name = aom_codec_iface_name(decoder_iface);
    (void)iface_name;

    // 5. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail;
        return 0;
    }

    // 6. Set frame buffer functions
    res = aom_codec_set_frame_buffer_functions(&decoder,  id_000035_get_frame_buffer,  id_000035_release_frame_buffer, nullptr);
    if (res != AOM_CODEC_OK) {
        const char *error = aom_codec_error(&decoder);
        (void)error;
        aom_codec_destroy(&decoder);
        return 0;
    }

    // 7. Decode the input data
    res = aom_codec_decode(&decoder, data, size, nullptr);
    if (res != AOM_CODEC_OK) {
        const char *error = aom_codec_error(&decoder);
        (void)error;
        aom_codec_destroy(&decoder);
        return 0;
    }

    // 8. Get stream info
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    res = aom_codec_get_stream_info(&decoder, &stream_info);
    if (res != AOM_CODEC_OK) {
        const char *error = aom_codec_error(&decoder);
        (void)error;
    }

    // 9. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = nullptr;
    int frame_count = 0;
    
    while ((decoded_img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        if (frame_count > 10) break; // Limit frames processed
        
        // 10. Get plane dimensions
        for (int plane = 0; plane < 3; ++plane) {
            int plane_w = aom_img_plane_width(decoded_img, plane);
            int plane_h = aom_img_plane_height(decoded_img, plane);
            (void)plane_w;
            (void)plane_h;
        }
        
        // 11. Allocate a dummy image
        aom_image_t dummy_img;
        aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
        unsigned int width = 64;
        unsigned int height = 64;
        unsigned int align = 1;
        
        aom_image_t *allocated_img = aom_img_alloc(&dummy_img, fmt, width, height, align);
        if (allocated_img) {
            // 12. Remove metadata (none exists, but call anyway)
            aom_img_remove_metadata(allocated_img);
            
            // 13. Free the allocated image
            aom_img_free(allocated_img);
        }
        
        frame_count++;
    }

    // 14. Create encoder interface
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
        aom_codec_destroy(&decoder);
        return 0;
    }

    // 15. Get default encoder configuration
    aom_codec_enc_cfg_t enc_cfg;
    unsigned int usage = 0; // Good quality
    res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, usage);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }

    // Set basic encoder parameters from stream info if available
    if (stream_info.w > 0 && stream_info.h > 0) {
        enc_cfg.g_w = stream_info.w;
        enc_cfg.g_h = stream_info.h;
    } else {
        enc_cfg.g_w = 64;
        enc_cfg.g_h = 64;
    }
    enc_cfg.g_threads = 1;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;

    // 16. Initialize encoder
    aom_codec_ctx_t encoder;
    res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        const char *error = aom_codec_error(&encoder);
        (void)error;
        aom_codec_destroy(&decoder);
        return 0;
    }

    // 17. Set encoder configuration (reapply)
    res = aom_codec_enc_config_set(&encoder, &enc_cfg);
    if (res != AOM_CODEC_OK) {
        const char *error = aom_codec_error(&encoder);
        (void)error;
    }

    // 18. Get global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder);
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }

    // 19. Encode a test frame if we have decoded images
    if (frame_count > 0) {
        // Create a simple test image
        aom_image_t test_img;
        aom_img_fmt_t test_fmt = AOM_IMG_FMT_I420;
        unsigned int test_width = 64;
        unsigned int test_height = 64;
        unsigned int test_align = 1;
        
        aom_image_t *test_allocated = aom_img_alloc(&test_img, test_fmt, test_width, test_height, test_align);
        if (test_allocated) {
            // Fill with some data
            for (int plane = 0; plane < 3; ++plane) {
                unsigned char *plane_ptr = test_allocated->planes[plane];
                int stride = test_allocated->stride[plane];
                int plane_h = aom_img_plane_height(test_allocated, plane);
                
                for (int y = 0; y < plane_h; ++y) {
                    memset(plane_ptr + y * stride, 128, test_width >> (plane > 0 ? 1 : 0));
                }
            }
            
            // Encode the frame
            aom_codec_pts_t pts = 0;
            unsigned long duration = 1;
            aom_enc_frame_flags_t flags = 0;
            
            res = aom_codec_encode(&encoder, test_allocated, pts, duration, flags);
            if (res != AOM_CODEC_OK) {
                const char *error = aom_codec_error(&encoder);
                (void)error;
            }
            
            // Get encoded data
            aom_codec_iter_t enc_iter = nullptr;
            const aom_codec_cx_pkt_t *pkt;
            while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                // Process packet if needed
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // Frame data available in pkt->data.frame.buf
                }
            }
            
            aom_img_free(test_allocated);
        }
    }

    // 20. Destroy both codec contexts
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);

    return 0;
}