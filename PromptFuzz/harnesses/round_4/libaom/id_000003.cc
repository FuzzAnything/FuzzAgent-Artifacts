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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_codec_destroy","aom_img_set_rect","aom_img_plane_width","aom_uleb_encode","aom_codec_encode","aom_codec_err_to_string","aom_img_wrap","aom_codec_encode","aom_img_set_rect","aom_img_plane_width","aom_uleb_encode","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_set_rect","aom_img_plane_width","aom_uleb_encode","aom_codec_encode","aom_codec_err_to_string","aom_img_wrap","aom_codec_encode","aom_img_set_rect","aom_img_plane_width","aom_uleb_encode","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<80) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_13, fdp);
	//fuzzer shim end}




    // Early exit if we don't have enough data for basic operations
    if (size < 100) {
	return 0;
	}
	
    
    // Create a FILE* from input data
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
    
    // Initialize encoder context
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *encoder = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    // Get default encoder configuration
    if (aom_codec_enc_config_default(encoder, &cfg, fuzz_uint32_t_1) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Configure encoder with small dimensions for fuzzing
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_lag_in_frames = 0;
    
    // Initialize encoder
    if (aom_codec_enc_init(&codec_ctx, encoder, &cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create an image
    aom_image_t img;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    unsigned int align = 32;
    
    // Allocate image
    if (!aom_img_alloc(&img, fmt, fuzz_uint32_t_2, 64, fuzz_uint32_t_3)) {
        aom_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read some data from input to populate image planes
    size_t bytes_read = fread(img.planes[0], 1, 
                             std::min(size, (size_t)img.stride[0] * 64), in_file);
    
    // Use aom_img_set_rect to set a portion of the image
    // Using random values from input data for coordinates
    unsigned int x = data[0] % 32;
    unsigned int y = data[1] % 32;
    unsigned int w = 16 + (data[2] % 16);
    unsigned int h = 16 + (data[3] % 16);
    unsigned int border = data[4] % 8;
    
    // Call aom_img_set_rect
    aom_img_set_rect(&img, x, y, w, h, border);
    
    // Get plane width for plane 0
    int plane_width = aom_img_plane_width(&img, fuzz_int32_t_4);
    
    // Use ULEB encode with the plane width
    uint64_t value = plane_width;
    size_t available = 10;
    uint8_t coded_value[10];
    size_t coded_size = 0;
    
    // Call aom_uleb_encode
    int uleb_result = aom_uleb_encode(value, available, coded_value, &coded_size);
    
    // Write encoded ULEB to output file if successful
    if (uleb_result == 0 && coded_size > 0) {
        fwrite(coded_value, 1, coded_size, out_file);
    }
    
    // Prepare for encoding
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    // Call aom_codec_encode
    aom_codec_err_t encode_err = aom_codec_encode(&codec_ctx, &img, fuzz_int64_t_5, 
                                                  fuzz_uint64_t_6, fuzz_int64_t_7);
    
    // Convert error code to string (for completeness, though we may not use it)
    const char *err_str = aom_codec_err_to_string(encode_err);
    
    // Read remaining data from input and encode as frames
    // Reset file pointer
    fseek(in_file, 0, SEEK_SET);
    
    // Create a simple test pattern for additional frames
    std::vector<uint8_t> test_frame(64 * 64 * 3 / 2, 128); // I420 format
    if (size > 1000) {
        // Use some input data for the test frame
        size_t copy_size = std::min(test_frame.size(), size - 100);
        memcpy(test_frame.data(), data + 100, copy_size);
        
        // Create another image with the test pattern
        aom_image_t img2;
        if (aom_img_wrap(&img2, fmt, 64, fuzz_uint32_t_8, align, test_frame.data())) {
            // Encode the second frame
            aom_codec_encode(&codec_ctx, &img2, pts + 1, fuzz_uint64_t_9, fuzz_int64_t_10);
            
            // Set rectangle on second image
            aom_img_set_rect(&img2, 
                            data[5] % 32, 
                            data[6] % 32, 
                            8 + (data[7] % 24), 
                            8 + (data[8] % 24), 
                            data[9] % 4);
            
            // Get plane width for plane 1
            int plane1_width = aom_img_plane_width(&img2, fuzz_int32_t_11);
            
            // Encode another ULEB value
            uint64_t value2 = plane1_width;
            size_t coded_size2 = 0;
            aom_uleb_encode(value2, available, coded_value, &coded_size2);
        }
    }
    
    // Flush encoder
    aom_codec_encode(&codec_ctx, NULL, pts + 2, fuzz_uint64_t_12, fuzz_int64_t_13);
    
    // Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&codec_ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Clean up
    aom_img_free(&img);
    
    // Call aom_codec_destroy
    aom_codec_destroy(&codec_ctx);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}