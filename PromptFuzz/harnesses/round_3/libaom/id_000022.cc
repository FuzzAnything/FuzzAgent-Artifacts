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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 71.5, nr_unique_branch: 40
//<Quality> {"density":11,"unique_branches":{"av1_decode_tg_tiles_and_wrapup":[[5510,25,5510,75,0,0,4,1]],"aom_lpf_horizontal_14_quad_avx2":[[663,7,663,62,0,0,4,0],[742,9,742,64,0,0,4,1],[848,11,848,67,0,0,4,1]],"aom_codec_set_option":[[124,36,124,59,0,0,4,0]],"aom_lpf_horizontal_6_quad_avx2":[[88,7,88,62,0,0,4,0],[157,9,157,64,0,0,4,1]],"av1_rd_pick_intra_sby_mode":[[1700,13,1700,56,0,0,4,0],[1720,9,1720,56,0,0,4,0]],"av1_pick_filter_restoration":[[2158,8,2158,42,0,0,4,1]],"av1_set_speed_features_qindex_dependent":[[2933,16,2933,25,0,0,4,0],[2934,11,2934,29,0,0,4,0],[3010,7,3010,17,0,0,4,0],[3011,11,3011,44,0,0,4,0]],"lpf_internal_6_dual_sse2":[[1062,7,1062,62,0,0,4,1]],"search_filter_level":[[175,13,175,40,0,0,4,1]],"search_tx_type":[[2119,45,2119,54,0,0,4,1],[2175,7,2175,19,0,0,4,1]],"intra_mode_cnn_partition":[[262,14,262,34,0,0,4,1],[274,14,274,34,0,0,4,0],[274,14,274,34,0,0,4,1],[279,26,279,54,0,0,4,0],[279,26,279,54,0,0,4,1],[285,26,285,54,0,0,4,0],[285,26,285,54,0,0,4,1],[289,14,289,32,0,0,4,0],[294,26,294,54,0,0,4,0],[294,26,294,54,0,0,4,1],[300,26,300,54,0,0,4,0],[300,26,300,54,0,0,4,1],[331,7,331,36,0,0,4,0],[334,9,334,46,0,0,4,0]],"aom_var_2d_u8_avx2":[[196,15,196,29,0,0,4,0],[199,17,199,31,0,0,4,0],[199,17,199,31,0,0,4,1],[201,23,201,28,0,0,4,0],[201,23,201,28,0,0,4,1],[205,23,205,28,0,0,4,0],[205,23,205,28,0,0,4,1],[225,12,225,22,0,0,4,1]]},"library_calls":["aom_img_wrap","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_encode","aom_codec_get_cx_data","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_destroy","aom_codec_destroy","aom_img_remove_metadata"],"critical_calls":["aom_img_wrap","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_encode","aom_codec_get_cx_data","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_destroy","aom_codec_destroy","aom_img_remove_metadata"],"visited":1}
/**/


// Include libaom headers
extern "C" {
}

// Custom frame buffer callbacks
static int  id_000022_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t*)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

static int  id_000022_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Create an image using aom_img_wrap
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int stride_align = 1;
    
    // Allocate image buffer
    size_t img_size = width * height * 3 / 2; // I420 format
    std::vector<unsigned char> img_buffer(img_size);
    
    // Fill with some data from input
    size_t copy_size = std::min(size, img_size);
    memcpy(img_buffer.data(), data, copy_size);
    
    aom_image_t img;
    aom_image_t *wrapped_img = aom_img_wrap(&img, AOM_IMG_FMT_I420, 
                                            width, height, 
                                            stride_align, 
                                            img_buffer.data());
    if (!wrapped_img) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 2: Add metadata to the image
    uint32_t metadata_type = 1;
    size_t metadata_size = std::min(size_t(50), size - copy_size);
    const uint8_t *metadata_data = data + copy_size;
    
    aom_img_add_metadata(wrapped_img, metadata_type, metadata_data, 
                        metadata_size, AOM_MIF_ANY_FRAME);
    
    // Step 3: Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    // Get default config
    if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = AOM_Q;
    cfg.rc_min_quantizer = 20;
    cfg.rc_max_quantizer = 40;
    
    // Initialize encoder with version
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, 
                               AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 4: Set encoder option
    aom_codec_set_option(&encoder, "cpu-used", "5");
    
    // Step 5: Encode the image with metadata
    aom_codec_encode(&encoder, wrapped_img, 0, 1, 0);
    
    // Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    std::vector<uint8_t> encoded_data;
    
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            encoded_data.insert(encoded_data.end(),
                              (uint8_t*)pkt->data.frame.buf,
                              (uint8_t*)pkt->data.frame.buf + pkt->data.frame.sz);
        }
    }
    
    // Step 6: Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0,
                               AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 7: Set decoder option
    aom_codec_set_option(&decoder, "tile_columns", "1");
    
    // Step 8: Set frame buffer functions
    aom_codec_set_frame_buffer_functions(&decoder,  id_000022_get_frame_buffer, 
                                         id_000022_release_frame_buffer, NULL);
    
    // Step 9: Decode the encoded data
    if (encoded_data.size() > 0) {
        aom_codec_decode(&decoder, encoded_data.data(), encoded_data.size(), NULL);
        
        // Step 10: Get preview frame
        const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
        if (preview_frame) {
            // Step 11: Get metadata from the decoded image
            size_t num_metadata = aom_img_num_metadata(preview_frame);
            for (size_t i = 0; i < num_metadata; ++i) {
                const aom_metadata_t *metadata = aom_img_get_metadata(preview_frame, i);
                // Metadata is available for inspection
            }
        }
    }
    
    // Write some data to output file
    if (encoded_data.size() > 0) {
        fwrite(encoded_data.data(), 1, encoded_data.size(), out_file);
    }
    
    // Cleanup
    aom_codec_destroy(&decoder);
    aom_codec_destroy(&encoder);
    
    // Remove metadata from image
    aom_img_remove_metadata(wrapped_img);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}