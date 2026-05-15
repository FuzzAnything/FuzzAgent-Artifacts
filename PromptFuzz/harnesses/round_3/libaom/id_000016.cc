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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 47.666668, nr_unique_branch: 31
//<Quality> {"density":13,"unique_branches":{"aom_lpf_horizontal_8_quad_avx2":[[378,9,378,64,0,0,4,1]],"get_cdef_filter_strengths":[[49,5,49,31,0,0,4,0],[53,5,53,31,0,0,4,0]],"av1_set_speed_features_qindex_dependent":[[2922,12,2922,17,0,0,4,1],[2982,7,2982,17,0,0,4,0],[2984,9,2984,46,0,0,4,1]],"av1_filter_block_plane_horz_opt":[[1995,37,1995,57,0,0,4,1]],"encode_restoration_mode":[[1810,25,1810,65,0,0,4,1],[1817,9,1817,22,0,0,4,0]],"av1_prune_ab_partitions":[[2010,7,2010,36,0,0,4,0],[2015,7,2015,36,0,0,4,0],[2020,7,2020,36,0,0,4,0],[2025,7,2025,36,0,0,4,0]],"av1_rd_pick_intra_sby_mode":[[1713,13,1715,33,0,0,4,0]],"evaluate_ab_partition_based_on_split":[[1879,7,1879,35,0,0,4,1],[1880,9,1880,38,0,0,4,0],[1880,9,1880,38,0,0,4,1],[1882,14,1882,28,0,0,4,1],[1883,7,1883,33,0,0,4,0],[1885,9,1885,69,0,0,4,0],[1885,9,1885,69,0,0,4,1],[1889,7,1889,33,0,0,4,0],[1891,9,1891,69,0,0,4,0],[1891,9,1891,69,0,0,4,1],[1895,7,1895,31,0,0,4,0],[34,23,34,34,1,0,4,1]],"av1_ml_prune_4_partition":[[1486,7,1486,23,0,0,4,0],[1487,7,1487,23,0,0,4,1],[1494,13,1494,25,0,0,4,0]],"prune_4_partition_using_split_info":[[4038,11,4038,73,0,0,4,1],[4040,9,4040,47,0,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_uleb_encode_fixed_size","aom_codec_set_option","aom_img_alloc","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_error_detail","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_uleb_encode_fixed_size","aom_codec_set_option","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_error_detail","aom_img_free","aom_codec_destroy"],"visited":2}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
	return 0;
	}
	  // Need minimum data for operations

    // 1. Create encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    // Initialize encoder with default config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set small dimensions for fuzzing
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_lag_in_frames = 0;
    
    if (aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // 2. Use aom_uleb_encode_fixed_size
    uint64_t value = 0;
    size_t value_size = (size > 8) ? 8 : size;
    memcpy(&value, data, value_size);
    
    uint8_t coded_buffer[16];
    size_t coded_size = 0;
    aom_uleb_encode_fixed_size(value, sizeof(coded_buffer), 8, coded_buffer, &coded_size);

    // 3. Use aom_codec_set_option
    char option_name[32];
    char option_value[32];
    snprintf(option_name, sizeof(option_name), "cpu-used");
    snprintf(option_value, sizeof(option_value), "%d", data[0] % 5);
    aom_codec_set_option(&ctx, option_name, option_value);

    // 4. Create and setup image
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 16, 16, 1);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image planes with fuzzer data
    size_t data_offset = 0;
    for (int plane = 0; plane < 3; ++plane) {
        int plane_width = aom_img_plane_width(img, plane);
        int plane_height = aom_img_plane_height(img, plane);
        int stride = img->stride[plane];
        
        for (int y = 0; y < plane_height; ++y) {
            uint8_t *row = img->planes[plane] + y * stride;
            size_t bytes_to_copy = (size - data_offset) < (size_t)plane_width ? 
                                  (size - data_offset) : plane_width;
            if (bytes_to_copy > 0) {
                memcpy(row, data + data_offset, bytes_to_copy);
                data_offset += bytes_to_copy;
            }
            if (data_offset >= size) data_offset = 0;
        }
    }

    // 5. Use aom_codec_set_cx_data_buf
    aom_fixed_buf_t output_buf;
    std::vector<uint8_t> buffer(4096);
    output_buf.buf = buffer.data();
    output_buf.sz = buffer.size();
    aom_codec_set_cx_data_buf(&ctx, &output_buf, 0, 0);

    // 6. Use aom_codec_encode
    aom_codec_encode(&ctx, img, 0, 1, 0);

    // 7. Use aom_codec_error_detail
    const char *error_detail = aom_codec_error_detail(&ctx);
    (void)error_detail;  // Use variable to avoid unused warning

    // Cleanup
    aom_img_free(img);
    aom_codec_destroy(&ctx);

    return 0;
}