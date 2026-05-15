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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"validate_config":[[667,32,667,49,52,0,4,1]],"update_extra_cfg":[[1758,7,1758,26,0,0,4,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_control","aom_img_alloc","aom_codec_destroy","aom_img_plane_width","aom_img_remove_metadata","aom_codec_encode","aom_codec_get_global_headers","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_control","aom_img_alloc","aom_img_plane_width","aom_img_remove_metadata","aom_codec_encode","aom_codec_get_global_headers","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0;  // Need minimum data for meaningful fuzzing
    }

    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize encoder context
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *encoder = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    // Get default configuration
    if (aom_codec_enc_config_default(encoder, &cfg, AOM_USAGE_GOOD_QUALITY) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up basic configuration
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;

    // Initialize encoder with version check
    if (aom_codec_enc_init_ver(&codec_ctx, encoder, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set encoder options using fuzzer data
    char option_value[32];
    snprintf(option_value, sizeof(option_value), "%u", (unsigned int)data[0] % 10);
    aom_codec_set_option(&codec_ctx, "cpu-used", option_value);

    // Use control API
    aom_codec_control(&codec_ctx, AOME_SET_CPUUSED, (data[1] % 10) + 1);

    // Create test image
    aom_image_t img;
    const unsigned int align = 32;
    if (!aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, align)) {
        aom_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read some data into image planes
    size_t bytes_to_read = size < 1000 ? size : 1000;
    fread(img.planes[0], 1, bytes_to_read, in_file);
    
    // Get plane width
    int plane_width = aom_img_plane_width(&img, 0);
    (void)plane_width; // Use result to avoid unused variable warning

    // Remove metadata if present
    aom_img_remove_metadata(&img);

    // Encode the image
    aom_codec_encode(&codec_ctx, &img, 0, 1, 0);

    // Get global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&codec_ctx);
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }

    // Clean up
    aom_img_free(&img);
    aom_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}