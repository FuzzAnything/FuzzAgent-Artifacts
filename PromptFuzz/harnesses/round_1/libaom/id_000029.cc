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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_set_rect","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_uleb_encode","aom_codec_encode","aom_codec_get_cx_data","aom_codec_encode","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_set_rect","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_uleb_encode","aom_codec_encode","aom_codec_get_cx_data","aom_codec_encode","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Dummy frame buffer callbacks for decoder
int  id_000029_dummy_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    fb->data = (uint8_t*)calloc(1, min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    fb->priv = fb->data;
    return 0;
}

int  id_000029_dummy_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    if (fb->priv) free(fb->priv);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get version info
    const char *extra_version = aom_codec_version_extra_str();
    (void)extra_version; // Suppress unused warning
    
    // 2. Initialize decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t ret = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                                                 AOM_DECODER_ABI_VERSION);
    if (ret != AOM_CODEC_OK) {
        return 0; // Early exit if decoder init fails
    }
    
    // 3. Set frame buffer functions
    ret = aom_codec_set_frame_buffer_functions(&decoder,  id_000029_dummy_get_frame_buffer, 
                                                id_000029_dummy_release_frame_buffer, NULL);
    
    // 4. Peek stream info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_peek_stream_info(decoder_iface, data, size, &si);
    
    // 5. Decode the input data
    ret = aom_codec_decode(&decoder, data, size, NULL);
    
    // 6. Get stream info after decode
    aom_codec_get_stream_info(&decoder, &si);
    
    // 7. Get decoded frames
    aom_codec_iter_t iter = NULL;
    std::vector<aom_image_t*> decoded_frames;
    aom_image_t *img;
    while ((img = aom_codec_get_frame(&decoder, &iter)) != NULL) {
        decoded_frames.push_back(img);
        
        // 8. Apply rectangle operation if image is valid
        if (img->w > 0 && img->h > 0) {
            unsigned int x = 0;
            unsigned int y = 0;
            unsigned int w = img->w > 16 ? 16 : img->w;
            unsigned int h = img->h > 16 ? 16 : img->h;
            
            // Ensure rectangle is within bounds
            if (x + w <= img->w && y + h <= img->h) {
                aom_img_set_rect(img, x, y, w, h, 0);
            }
        }
    }
    
    // 9. Initialize encoder if we have decoded frames
    if (!decoded_frames.empty()) {
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        aom_codec_ctx_t encoder;
        aom_codec_enc_cfg_t enc_cfg;
        
        // Get default encoder config
        aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        
        // Set resolution based on first frame
        if (decoded_frames[0]->w > 0 && decoded_frames[0]->h > 0) {
            enc_cfg.g_w = decoded_frames[0]->w;
            enc_cfg.g_h = decoded_frames[0]->h;
        } else {
            enc_cfg.g_w = 64;
            enc_cfg.g_h = 64;
        }
        
        // Initialize encoder
        ret = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, 
                                     AOM_ENCODER_ABI_VERSION);
        
        if (ret == AOM_CODEC_OK) {
            // 10. Set encoder option
            aom_codec_set_option(&encoder, "cpu-used", "1");
            
            // 11. Use uleb encode for frame counter
            uint64_t frame_count = decoded_frames.size();
            uint8_t uleb_buffer[16];
            size_t coded_size;
            int uleb_ret = aom_uleb_encode(frame_count, sizeof(uleb_buffer), 
                                          uleb_buffer, &coded_size);
            
            // Encode frames
            for (size_t i = 0; i < decoded_frames.size(); i++) {
                // 12. Encode each frame
                aom_codec_encode(&encoder, decoded_frames[i], i, 1, 0);
                
                // 13. Get encoded data
                aom_codec_iter_t enc_iter = NULL;
                const aom_codec_cx_pkt_t *pkt;
                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                    // Process packet if needed
                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                        // Optional: Write to file
                        FILE *out_file = fopen("output_file", "wb");
                        if (out_file) {
                            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                            assert_file_closed(&out_file);;
                        }
                    }
                }
            }
            
            // Flush encoder
            aom_codec_encode(&encoder, NULL, 0, 0, 0);
            
            // 14. Destroy encoder
            aom_codec_destroy(&encoder);
        }
    }
    
    // 15. Destroy decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}