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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":6,"unique_branches":{"set_encoder_config":[[1347,7,1347,34,0,0,4,1]],"av1_change_config":[[931,34,931,57,0,0,4,0]]},"library_calls":["aom_codec_version","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_wrap","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_wrap","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


// Include libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Get codec version
    int version = aom_codec_version();
    (void)version; // Prevent unused variable warning

    // 2. Create decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 0;
    dec_cfg.h = 0;
    dec_cfg.allow_lowbitdepth = 1;

    // Initialize decoder
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // 3. Decode input data
    aom_codec_decode(&decoder, data, size, nullptr);

    // 4. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_frame = nullptr;
    
    // Try to get at least one frame
    decoded_frame = aom_codec_get_frame(&decoder, &iter);
    
    // If we got a decoded frame, process it
    if (decoded_frame != nullptr) {
        // 5. Create encoder context
        aom_codec_ctx_t encoder;
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        aom_codec_enc_cfg_t enc_cfg;
        
        // Get default encoder configuration
        if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_2) == AOM_CODEC_OK) {
            // Set basic encoder parameters
            enc_cfg.g_w = decoded_frame->d_w;
            enc_cfg.g_h = decoded_frame->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.g_profile = 0;
            enc_cfg.g_bit_depth = AOM_BITS_8;
            enc_cfg.g_input_bit_depth = 8;
            enc_cfg.rc_end_usage = AOM_Q;
            enc_cfg.rc_min_quantizer = 2;
            enc_cfg.rc_max_quantizer = 58;
            enc_cfg.g_threads = 1;
            enc_cfg.g_lag_in_frames = 0;
            enc_cfg.kf_mode = AOM_KF_DISABLED;
            
            // Initialize encoder
            if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_3, AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
                
                // 6. Wrap decoded frame data for encoder input
                // Calculate required buffer size for I420 format
                size_t y_size = decoded_frame->d_w * decoded_frame->d_h;
                size_t uv_size = ((decoded_frame->d_w + 1) / 2) * ((decoded_frame->d_h + 1) / 2);
                size_t total_size = y_size + 2 * uv_size;
                
                // Allocate buffer for image data
                std::vector<unsigned char> img_buffer(total_size);
                
                // Create image wrapper
                aom_image_t wrapped_img;
                aom_img_wrap(&wrapped_img, AOM_IMG_FMT_I420, 
                            decoded_frame->d_w, decoded_frame->d_h, 
                            fuzz_uint32_t_4, img_buffer.data());
                
                // Copy plane data from decoded frame to our buffer
                // Simplified: just fill with pattern for demonstration
                for (size_t i = 0; i < total_size; i++) {
                    img_buffer[i] = static_cast<unsigned char>(i % 256);
                }
                
                // 7. Set encoder configuration
                aom_codec_enc_config_set(&encoder, &enc_cfg);
                
                // 8. Create output buffer for encoder
                aom_fixed_buf_t output_buf;
                output_buf.sz = 1024 * 1024; // 1MB buffer
                output_buf.buf = malloc(output_buf.sz);
                
                if (output_buf.buf != nullptr) {
                    // 9. Set compressed data buffer
                    aom_codec_set_cx_data_buf(&encoder, &output_buf, fuzz_uint32_t_5, fuzz_uint32_t_6);
                    
                    // Clean up
                    free(output_buf.buf);
                }
                
                // Clean up encoder
                aom_codec_destroy(&encoder);
            }
        }
    }

    // 10. Clean up decoder
    aom_codec_destroy(&decoder);

    return 0;
}