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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 20
//<Quality> {"density":9,"unique_branches":{"aom_obu_type_to_string":[[188,5,188,29,0,0,4,0],[188,5,188,29,0,0,4,1],[189,5,189,32,0,0,4,0],[189,5,189,32,0,0,4,1],[190,5,190,26,0,0,4,0],[190,5,190,26,0,0,4,1],[191,5,191,36,0,0,4,0],[191,5,191,36,0,0,4,1],[192,5,192,19,0,0,4,0],[192,5,192,19,0,0,4,1],[193,5,193,24,0,0,4,0],[193,5,193,24,0,0,4,1],[194,5,194,22,0,0,4,0],[194,5,194,22,0,0,4,1],[195,5,195,23,0,0,4,0],[195,5,195,23,0,0,4,1],[196,5,196,21,0,0,4,0],[196,5,196,21,0,0,4,1],[197,5,197,12,0,0,4,0],[197,5,197,12,0,0,4,1]]},"library_calls":["aom_codec_version","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_destroy","aom_codec_get_stream_info","aom_obu_type_to_string","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_global_headers","aom_uleb_size_in_bytes","aom_uleb_encode","aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy"],"critical_calls":["aom_codec_version","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_obu_type_to_string","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_global_headers","aom_uleb_size_in_bytes","aom_uleb_encode","aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy"],"visited":0}
/**/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Get codec version
    int version = aom_codec_version();
    (void)version; // Avoid unused variable warning

    // 2. Get AV1 decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        return 0;
    }

    // 3. Get decoder capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Avoid unused variable warning

    // 4. Create decoder context with input data
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    // Initialize decoder
    if (aom_codec_dec_init(&decoder_ctx, decoder_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // 5. Decode the input data
    if (aom_codec_decode(&decoder_ctx, data, size, nullptr) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder_ctx);
        return 0;
    }

    // 6. Try to get stream info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    if (aom_codec_get_stream_info(&decoder_ctx, &si) == AOM_CODEC_OK) {
        // 7. Convert OBU type to string for each possible type
        for (int i = 0; i < 16; i++) {
            const char *obu_str = aom_obu_type_to_string(static_cast<OBU_TYPE>(i));
            (void)obu_str; // Avoid unused variable warning
        }
    }

    // 8. Create encoder context to get global headers
    aom_codec_ctx_t encoder_ctx;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
        enc_cfg.g_w = 128;
        enc_cfg.g_h = 128;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 256;
        enc_cfg.g_threads = 1;
        enc_cfg.g_pass = AOM_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        
        if (aom_codec_enc_init(&encoder_ctx, encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
            // 9. Get global headers
            aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder_ctx);
            if (global_headers) {
                // 10. Use ULEB functions with the global headers size
                uint64_t uleb_value = global_headers->sz;
                size_t uleb_size = aom_uleb_size_in_bytes(uleb_value);
                
                // Try to encode the ULEB value
                size_t coded_size = 0;
                uint8_t coded_buffer[10];
                if (aom_uleb_encode(uleb_value, sizeof(coded_buffer), 
                                    coded_buffer, &coded_size) == 0) {
                    // Try to decode it back
                    uint64_t decoded_value = 0;
                    size_t decoded_length = 0;
                    if (aom_uleb_decode(coded_buffer, coded_size, 
                                        &decoded_value, &decoded_length) == 0) {
                        // Also try fixed size encoding
                        size_t fixed_coded_size = 0;
                        uint8_t fixed_coded_buffer[10];
                        aom_uleb_encode_fixed_size(uleb_value, sizeof(fixed_coded_buffer),
                                                  4, fixed_coded_buffer, &fixed_coded_size);
                    }
                }
                
                free(global_headers->buf);
                free(global_headers);
            }
            aom_codec_destroy(&encoder_ctx);
        }
    }

    // 11. Get frames from decoder
    aom_codec_iter_t iter = nullptr;
    while (aom_codec_get_frame(&decoder_ctx, &iter)) {
        // Frame processing would go here
    }

    // 12. Cleanup
    aom_codec_destroy(&decoder_ctx);

    return 0;
}