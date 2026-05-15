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
//<ID> 251
//<Prompt> []
/*<Combination>: [
*/
//<score> 153, nr_unique_branch: 31
//<Quality> {"density":17,"unique_branches":{"blend_a64_mask_sx_sy_avx2":[[523,5,523,11,0,0,4,1],[546,5,546,11,0,0,4,1],[569,5,569,12,0,0,4,0],[573,5,573,12,0,0,4,1]],"combine_interintra":[[1067,7,1067,27,0,0,4,0],[1068,9,1068,33,0,0,4,0]],"warp_vertical_filter_gamma0_delta0_avx2":[[34,23,34,34,1,0,4,0]],"aom_blend_a64_mask_avx2":[[886,9,886,20,0,0,4,0]],"av1_dist_wtd_convolve_2d_no_avg_copy_avx2":[[827,7,827,14,0,0,4,0],[829,9,829,17,0,0,4,1],[837,16,837,23,0,0,4,1],[844,16,844,23,0,0,4,1],[851,16,851,23,0,0,4,0],[857,16,857,17,0,0,4,0],[857,16,857,17,0,0,4,1]],"blend_a64_mask_sx_sy_w16_avx2":[[470,12,470,15,0,0,4,0],[470,12,470,15,0,0,4,1]],"av1_dist_wtd_convolve_2d_copy_avx2":[[958,7,958,14,4,0,4,0],[963,9,963,17,4,0,4,1],[972,16,972,23,4,0,4,1],[981,16,981,23,4,0,4,1],[999,16,999,17,4,0,4,0],[999,16,999,17,4,0,4,1]],"av1_warp_affine_avx2":[[1129,18,1129,34,0,0,4,0],[1131,22,1131,56,0,0,4,0],[1131,22,1131,56,0,0,4,1],[1167,15,1167,41,0,0,4,1],[1188,13,1188,39,0,0,4,1],[34,23,34,34,20,0,4,0],[34,23,34,34,20,0,4,1],[34,23,34,34,21,0,4,0]]},"library_calls":["aom_img_alloc","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error","aom_codec_decode","aom_codec_error","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error","aom_codec_set_option","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_codec_encode","aom_codec_error","aom_uleb_encode_fixed_size","aom_codec_get_cx_data","aom_codec_get_global_headers","aom_img_remove_metadata","aom_codec_destroy","aom_codec_destroy","aom_img_free"],"critical_calls":["aom_img_alloc","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error","aom_codec_decode","aom_codec_error","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error","aom_codec_set_option","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_codec_encode","aom_codec_error","aom_uleb_encode_fixed_size","aom_codec_get_cx_data","aom_codec_get_global_headers","aom_img_remove_metadata","aom_codec_destroy","aom_codec_destroy","aom_img_free"],"visited":0}
/**/


// libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data

    // Initialize variables
    aom_codec_ctx_t decoder = {0}, encoder = {0};
    aom_codec_enc_cfg_t cfg = {0};
    aom_image_t *img = nullptr;
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    int ret = 0;
    
    // 1. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Allocate image
    img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 64, 64, 1);
    if (!img) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Initialize decoder (AV1 decoder)
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, AOM_DECODER_ABI_VERSION)) {
        const char *error = aom_codec_error(&decoder);
        // Continue despite error
    }
    
    // 4. Decode input data
    // Read some data from file to simulate decoding
    uint8_t decode_buffer[4096];
    size_t read_size = fread(decode_buffer, 1, std::min(size, sizeof(decode_buffer)), in_file);
    if (read_size > 0) {
        aom_codec_decode(&decoder, decode_buffer, read_size, nullptr);
    }
    
    // 5. Get decoder error
    const char *decoder_error = aom_codec_error(&decoder);
    
    // 6. Initialize encoder
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) == AOM_CODEC_OK) {
        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
        
        if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION)) {
            const char *error = aom_codec_error(&encoder);
            // Continue despite error
        }
    }
    
    // 7. Set encoder option
    aom_codec_set_option(&encoder, "cpu-used", "5");
    
    // 8. Get plane dimensions
    int plane_width = aom_img_plane_width(img, 0);
    int plane_height = aom_img_plane_height(img, 0);
    
    // 9. Add metadata to image
    uint32_t metadata_type = 1;
    uint8_t metadata_data[10] = {0,1,2,3,4,5,6,7,8,9};
    aom_img_add_metadata(img, metadata_type, metadata_data, 10, AOM_MIF_KEY_FRAME);
    
    // 10. Get number of metadata entries
    size_t metadata_count = aom_img_num_metadata(img);
    
    // 11. Encode image
    aom_codec_encode(&encoder, img, 0, 1, 0);
    
    // 12. Get encoder error
    const char *encoder_error = aom_codec_error(&encoder);
    
    // 13. ULEB encode a value
    uint8_t uleb_buffer[10];
    size_t coded_size = 0;
    uint64_t test_value = 12345;
    aom_uleb_encode_fixed_size(test_value, sizeof(uleb_buffer), 5, uleb_buffer, &coded_size);
    
    // 14. Get encoded data from encoder
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process the packet if needed
        }
    }
    
    // 15. Get global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder);
    
    // 16. Remove metadata from image
    aom_img_remove_metadata(img);
    
    // 17. Cleanup
    
    // Destroy codecs
    if (encoder.iface) aom_codec_destroy(&encoder);
    if (decoder.iface) aom_codec_destroy(&decoder);
    
    // Free image
    if (img) aom_img_free(img);
    
    // Close file
    if (in_file) assert_file_closed(&in_file);;
    
    // Free global headers if allocated
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    assert_file_closed(&in_file);
	return 0;
}