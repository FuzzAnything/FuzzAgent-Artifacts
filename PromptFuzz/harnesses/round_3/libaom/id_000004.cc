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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_control","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_control","aom_img_alloc","aom_img_plane_height","aom_img_set_rect","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_control","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_img_alloc","aom_img_plane_height","aom_img_set_rect","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Return early if there's not enough data
    if (size < 100) {
	return 0;
	}
	
    
    // Get version info (API requirement)
    const char *version_extra = aom_codec_version_extra_str();
    
    // Part 1: Decode stream info from input data
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Peek at stream info from input data
    aom_codec_err_t err = aom_codec_peek_stream_info(decoder_iface, data, 
                                                     size < 1000 ? size : 1000, 
                                                     &stream_info);
    
    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                                AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        return 0;
    }
    
    // Decode the input data
    err = aom_codec_decode(&decoder, data, size, nullptr);
    
    // Get stream info after decoding
    aom_codec_stream_info_t post_decode_info;
    memset(&post_decode_info, 0, sizeof(post_decode_info));
    aom_codec_get_stream_info(&decoder, &post_decode_info);
    
    // Use codec control API on decoder
    aom_codec_control(&decoder, AOMD_GET_LAST_REF_UPDATES, nullptr);
    
    // Get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
    
    // Part 2: Encoding setup
    if (decoded_img && stream_info.w > 0 && stream_info.h > 0) {
        // Get encoder interface
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        
        // Setup encoder configuration (API requirement)
        aom_codec_enc_cfg_t enc_cfg;
        err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        if (err != AOM_CODEC_OK) {
            aom_codec_destroy(&decoder);
            return 0;
        }
        
        // Configure encoder based on decoded stream
        enc_cfg.g_w = stream_info.w;
        enc_cfg.g_h = stream_info.h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.g_threads = 1;
        enc_cfg.rc_target_bitrate = 1000;
        
        // Initialize encoder
        aom_codec_ctx_t encoder;
        err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0,
                                    AOM_ENCODER_ABI_VERSION);
        if (err != AOM_CODEC_OK) {
            aom_codec_destroy(&decoder);
            return 0;
        }
        
        // Use codec control on encoder
        aom_codec_control(&encoder, AOME_SET_CPUUSED, 8);
        
        // Part 3: Image manipulation
        // Allocate new image (API requirement)
        aom_image_t *new_img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420,
                                           enc_cfg.g_w, enc_cfg.g_h, 1);
        
        if (new_img) {
            // Get plane height (API requirement)
            int plane_height = aom_img_plane_height(new_img, 0);
            
            // Set rectangle on image (API requirement)
            // Use data-derived coordinates (modulo to stay within bounds)
            unsigned int x = data[0] % (new_img->d_w / 2);
            unsigned int y = data[1] % (new_img->d_h / 2);
            unsigned int w = 16 + (data[2] % 32);
            unsigned int h = 16 + (data[3] % 32);
            
            // Ensure rectangle stays within bounds
            if (x + w > new_img->d_w) w = new_img->d_w - x;
            if (y + h > new_img->d_h) h = new_img->d_h - y;
            
            if (w > 0 && h > 0) {
                aom_img_set_rect(new_img, x, y, w, h, data[4] % 256);
            }
            
            // Encode the new image
            err = aom_codec_encode(&encoder, new_img, 0, 1, 0);
            
            // Get encoded data
            aom_codec_iter_t enc_iter = nullptr;
            const aom_codec_cx_pkt_t *pkt;
            while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // Write encoded data to output file
                    FILE *out_file = fopen("output_file", "wb");
                    if (out_file) {
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                        assert_file_closed(&out_file);;
                    }
                    break;
                }
            }
            
            // Clean up allocated image
            aom_img_free(new_img);
        }
        
        // Clean up encoder
        aom_codec_destroy(&encoder);
    }
    
    // Clean up decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}