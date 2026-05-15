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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_obu_type_to_string","aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_remove_metadata","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_img_alloc","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_remove_metadata","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_obu_type_to_string","aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_remove_metadata","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_img_alloc","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_remove_metadata","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


// Include necessary libaom headers

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Get version string
    const char *version_str = aom_codec_version_str();
    (void)version_str; // Suppress unused variable warning

    // 2. Try to get OBU type string (use first byte as OBU type)
    OBU_TYPE obu_type = static_cast<OBU_TYPE>(data[0] & 0x7); // Use lower 3 bits
    const char *obu_str = aom_obu_type_to_string(obu_type);
    (void)obu_str;

    // 3. Create decoder and encoder interfaces
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();

    // 4. Get interface names
    const char *dec_name = aom_codec_iface_name(decoder_iface);
    const char *enc_name = aom_codec_iface_name(encoder_iface);
    (void)dec_name;
    (void)enc_name;

    // 5. Create decoder context
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, 
                                                  &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // 6. Peek at stream info
    aom_codec_stream_info_t stream_info = {0};
    err = aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    (void)err; // Continue even if peek fails

    // 7. Decode the input data
    err = aom_codec_decode(&decoder_ctx, data, size, nullptr);
    
    // 8. Get stream info after decoding
    err = aom_codec_get_stream_info(&decoder_ctx, &stream_info);
    
    // 9. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder_ctx, &iter);
    
    // If we got a decoded frame, try to re-encode it
    if (decoded_img) {
        // 10. Get plane dimensions
        int plane_width = aom_img_plane_width(decoded_img, 0);
        int plane_height = aom_img_plane_height(decoded_img, 0);
        (void)plane_width;
        (void)plane_height;
        
        // 11. Remove metadata from decoded image
        aom_img_remove_metadata(decoded_img);
        
        // 12. Create encoder context
        aom_codec_ctx_t encoder_ctx;
        aom_codec_enc_cfg_t enc_cfg;
        
        // Get default encoder configuration
        err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        if (err == AOM_CODEC_OK) {
            // Set configuration based on decoded image
            enc_cfg.g_w = decoded_img->d_w;
            enc_cfg.g_h = decoded_img->d_h;
            enc_cfg.g_threads = 1;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.g_profile = 0;
            enc_cfg.g_bit_depth = AOM_BITS_8;
            enc_cfg.g_input_bit_depth = 8;
            enc_cfg.rc_end_usage = AOM_VBR;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_pass = AOM_RC_ONE_PASS;
            enc_cfg.g_lag_in_frames = 0;
            
            // Initialize encoder
            err = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 
                                         0, AOM_ENCODER_ABI_VERSION);
            
            if (err == AOM_CODEC_OK) {
                // 13. Set encoder configuration (again to ensure it's applied)
                err = aom_codec_enc_config_set(&encoder_ctx, &enc_cfg);
                
                // 14. Create a copy of the image for encoding
                aom_image_t *img_to_encode = aom_img_alloc(nullptr, decoded_img->fmt,
                                                          decoded_img->d_w, decoded_img->d_h, 16);
                
                if (img_to_encode) {
                    // Copy image data (simplified - just copy planes)
                    for (int plane = 0; plane < 3; plane++) {
                        int copy_size = decoded_img->stride[plane] * 
                                      aom_img_plane_height(decoded_img, plane);
                        if (copy_size > 0) {
                            memcpy(img_to_encode->planes[plane], 
                                   decoded_img->planes[plane], 
                                   copy_size);
                        }
                    }
                    
                    // 15. Encode the frame
                    err = aom_codec_encode(&encoder_ctx, img_to_encode, 
                                          0, 1, 0);
                    
                    // 16. Get encoded data
                    aom_codec_iter_t enc_iter = nullptr;
                    const aom_codec_cx_pkt_t *pkt = nullptr;
                    
                    while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
                        // Process packet if needed
                        (void)pkt;
                    }
                    
                    // 17. Free the allocated image
                    aom_img_free(img_to_encode);
                }
                
                // 18. Destroy encoder
                aom_codec_destroy(&encoder_ctx);
            }
        }
    }
    
    // 19. Create a dummy image to test allocation
    aom_image_t dummy_img;
    aom_image_t *allocated_img = aom_img_alloc(&dummy_img, AOM_IMG_FMT_I420, 
                                               64, 64, 16);
    if (allocated_img) {
        // Test plane dimensions
        int dummy_width = aom_img_plane_width(allocated_img, 0);
        int dummy_height = aom_img_plane_height(allocated_img, 0);
        (void)dummy_width;
        (void)dummy_height;
        
        // Remove metadata (even though there isn't any)
        aom_img_remove_metadata(allocated_img);
        
        // Free the dummy image
        aom_img_free(allocated_img);
    }
    
    // 20. Destroy decoder
    aom_codec_destroy(&decoder_ctx);
    
    return 0;
}