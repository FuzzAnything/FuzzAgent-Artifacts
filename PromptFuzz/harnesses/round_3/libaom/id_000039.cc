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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 2
//<Quality> {"density":16,"unique_branches":{"aom_codec_enc_init_ver":[[61,7,61,37,0,0,4,0],[61,41,61,74,0,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_codec_enc_config_set","aom_codec_encode","aom_codec_get_global_headers","aom_codec_get_stream_info","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_codec_enc_config_set","aom_codec_encode","aom_codec_get_global_headers","aom_codec_get_stream_info","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // 1. Initialize encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
	return 0;
	}
	

    // 2. Get encoder capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    (void)caps; // Use caps to avoid unused variable warning

    // 3. Create encoder configuration with default values
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // 4. Set encoder configuration based on input data
    cfg.g_w = 64 + (data[0] % 192);  // Width between 64-256
    cfg.g_h = 64 + (data[1] % 192);  // Height between 64-256
    cfg.rc_target_bitrate = 1000 + (data[2] % 4000); // Bitrate 1000-5000
    cfg.g_threads = 1 + (data[3] % 4); // Threads 1-4
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    cfg.kf_mode = AOM_KF_AUTO;
    cfg.kf_min_dist = 0;
    cfg.kf_max_dist = 9999;

    // 5. Create encoder context
    aom_codec_ctx_t codec;
    aom_codec_flags_t flags = 0;
    if (aom_codec_enc_init_ver(&codec, iface, &cfg, flags, 3) != AOM_CODEC_OK) {
        return 0;
    }

    // 6. Set encoder option from input data
    const char *option_name = "cpu-used";
    char option_value[10];
    snprintf(option_value, sizeof(option_value), "%d", data[4] % 9); // 0-8
    aom_codec_set_option(&codec, option_name, option_value);

    // 7. Create image for encoding
    aom_image_t img;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    unsigned int align = 32;
    
    if (!aom_img_alloc(&img, fmt, cfg.g_w, cfg.g_h, align)) {
        aom_codec_destroy(&codec);
        return 0;
    }

    // 8. Fill image planes with data
    size_t offset = 5;
    for (int plane = 0; plane < 3; plane++) {
        int width = aom_img_plane_width(&img, plane);
        int height = aom_img_plane_height(&img, plane);
        
        for (int y = 0; y < height; y++) {
            size_t bytes_needed = width;
            size_t bytes_available = size - offset;
            size_t bytes_to_copy = bytes_available < bytes_needed ? bytes_available : bytes_needed;
            
            if (bytes_to_copy > 0) {
                uint8_t *row_ptr = img.planes[plane] + y * img.stride[plane];
                memcpy(row_ptr, data + offset, bytes_to_copy);
                offset += bytes_to_copy;
            }
            
            // If we've consumed all input data, break
            if (offset >= size) break;
        }
        if (offset >= size) break;
    }

    // 9. Add metadata to image
    uint32_t metadata_type = 1; // Example metadata type
    size_t metadata_size = size > offset + 20 ? 20 : size - offset;
    if (metadata_size > 0) {
        aom_img_add_metadata(&img, metadata_type, 
                            data + offset, metadata_size,
                            AOM_MIF_ANY_FRAME);
        offset += metadata_size;
    }

    // 10. Update encoder configuration (if needed)
    if (aom_codec_enc_config_set(&codec, &cfg) != AOM_CODEC_OK) {
        // Continue anyway - configuration might already be valid
    }

    // 11. Encode the image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t enc_flags = 0;
    
    if (aom_codec_encode(&codec, &img, pts, duration, enc_flags) != AOM_CODEC_OK) {
        // Continue anyway - error might be recoverable
    }

    // 12. Get global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&codec);
    (void)global_headers; // Use to avoid unused variable warning

    // 13. Get stream info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    if (aom_codec_get_stream_info(&codec, &si) == AOM_CODEC_OK) {
        // Use stream info
        (void)si.w;
        (void)si.h;
        (void)si.is_kf;
    }

    // 14. Clean up
    aom_img_free(&img);
    aom_codec_destroy(&codec);

    return 0;
}