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
//<ID> 34
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"av1_ml_prune_4_partition":[[1514,13,1514,19,0,0,4,1],[1515,13,1515,19,0,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_set_option","aom_uleb_encode_fixed_size","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_destroy","aom_codec_encode","aom_codec_error_detail","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_uleb_encode_fixed_size","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_encode","aom_codec_error_detail","aom_img_free","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to achieve encoding with custom options and buffer management:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 64) {
	return 0;
	}
	  // Need minimum data for operations

    // 1. Initialize encoder
    aom_codec_iface_t *encoder = aom_codec_av1_cx();
    aom_codec_ctx_t codec_ctx;
    aom_codec_enc_cfg_t cfg;
    
    // Get default configuration
    if (aom_codec_enc_config_default(encoder, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set basic configuration
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&codec_ctx, encoder, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // 2. Use aom_codec_set_option to set encoder options
    // Extract option name and value from input data
    size_t name_len = data[0] % 32 + 1;
    size_t value_len = data[1] % 32 + 1;
    
    if (name_len + value_len + 2 > size) {
        aom_codec_destroy(&codec_ctx);
        return 0;
    }
    
    std::vector<char> option_name(name_len + 1);
    std::vector<char> option_value(value_len + 1);
    
    memcpy(option_name.data(), data + 2, name_len);
    option_name[name_len] = '\0';
    memcpy(option_value.data(), data + 2 + name_len, value_len);
    option_value[value_len] = '\0';
    
    aom_codec_set_option(&codec_ctx, option_name.data(), option_value.data());

    // 3. Use aom_uleb_encode_fixed_size to encode a value
    uint64_t uleb_value = 0;
    size_t data_offset = 2 + name_len + value_len;
    
    if (data_offset + 8 <= size) {
        memcpy(&uleb_value, data + data_offset, 8);
        data_offset += 8;
    }
    
    uint8_t coded_buffer[16];
    size_t coded_size = 0;
    size_t pad_to_size = 4;  // Pad to 4 bytes
    
    aom_uleb_encode_fixed_size(uleb_value, sizeof(coded_buffer), pad_to_size, 
                               coded_buffer, &coded_size);

    // 4. Create and set output buffer with aom_codec_set_cx_data_buf
    aom_fixed_buf_t output_buf;
    size_t buffer_size = 1024 * 1024;  // 1MB buffer
    output_buf.buf = malloc(buffer_size);
    output_buf.sz = buffer_size;
    
    if (!output_buf.buf) {
        aom_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // Set padding (use remaining input data to determine padding)
    unsigned int pad_before = 0;
    unsigned int pad_after = 0;
    
    if (data_offset + 2 <= size) {
        pad_before = data[data_offset] % 16;
        pad_after = data[data_offset + 1] % 16;
    }
    
    aom_codec_set_cx_data_buf(&codec_ctx, &output_buf, pad_before, pad_after);

    // 5. Create test image and encode
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 1);
    if (!img) {
        free(output_buf.buf);
        aom_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // Fill image with test data (use remaining input)
    size_t remaining = size - data_offset;
    size_t fill_size = std::min(remaining, (size_t)(img->w * img->h * 3 / 2));
    
    if (fill_size > 0) {
        size_t y_plane_size = img->w * img->h;
        size_t uv_plane_size = y_plane_size / 4;
        
        // Fill Y plane
        size_t y_fill = std::min(fill_size, y_plane_size);
        memcpy(img->planes[0], data + data_offset, y_fill);
        
        // Fill U and V planes if we have more data
        if (fill_size > y_fill) {
            size_t uv_fill = std::min(fill_size - y_fill, uv_plane_size * 2);
            memcpy(img->planes[1], data + data_offset + y_fill, uv_fill / 2);
            if (uv_fill > uv_plane_size) {
                memcpy(img->planes[2], data + data_offset + y_fill + uv_plane_size, 
                       uv_fill - uv_plane_size);
            }
        }
    }

    // 6. Encode the image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    aom_codec_encode(&codec_ctx, img, pts, duration, flags);

    // 7. Check for errors using aom_codec_error_detail
    const char *error_detail = aom_codec_error_detail(&codec_ctx);
    if (error_detail) {
        // Error details available, but we continue anyway for fuzzing
    }

    // 8. Clean up
    aom_img_free(img);
    free(output_buf.buf);
    aom_codec_destroy(&codec_ctx);

    return 0;
}