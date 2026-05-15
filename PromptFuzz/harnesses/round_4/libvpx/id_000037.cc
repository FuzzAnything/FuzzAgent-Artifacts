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
#include <vpx/vp8cx.h>
}
extern "C" {
#include <vpx/vp8dx.h>
}
extern "C" {
#include <vpx/vpx_decoder.h>
}
//<ID> 360
//<Prompt> []
/*<Combination>: [
*/
//<score> 153, nr_unique_branch: 16
//<Quality> {"density":17,"unique_branches":{"update_initial_width":[[2353,7,2353,41,0,0,4,1],[2354,7,2354,41,0,0,4,1],[2368,9,2368,39,0,0,4,0]],"vp9_copy_and_extend_frame":[[134,27,134,61,0,0,4,0]],"vpx_realloc_frame_buffer":[[162,7,162,33,0,0,4,0],[162,37,162,65,0,0,4,0]],"vp9_lookahead_init":[[71,11,76,63,0,0,4,0]],"vp9_rc_set_gf_interval_range":[[24,23,24,34,5,0,4,1]],"encoder_encode":[[1411,9,1411,29,0,0,4,1],[1411,33,1411,58,0,0,4,1],[1430,9,1430,60,0,0,4,1],[1459,7,1459,38,0,0,4,1],[1481,27,1481,70,0,0,4,0],[1488,9,1488,37,0,0,4,1]],"alloc_util_frame_buffers":[[1305,7,1311,49,0,0,4,0]],"vp9_rc_get_default_max_gf_interval":[[25,23,25,34,2,0,4,1]]},"library_calls":["vpx_codec_vp9_cx","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_img_alloc","vpx_img_free","vpx_img_wrap","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_img_alloc","vpx_img_wrap","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<96) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_14, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_15, fdp);
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE pointer to write output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Step 1: Use vpx_codec_peek_stream_info to get stream information
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_err_t err = vpx_codec_peek_stream_info(
        decoder_iface, data, size, &stream_info);
    
    // If we can't get valid stream info, use reasonable defaults
    unsigned int width = 320;
    unsigned int height = 240;
    if (err == VPX_CODEC_OK && stream_info.w > 0 && stream_info.h > 0) {
        width = stream_info.w;
        height = stream_info.h;
    }
    
    // Step 2: Use vpx_img_alloc to allocate an image
    vpx_image_t *allocated_img = vpx_img_alloc(
        NULL, VPX_IMG_FMT_I420, width, height, fuzz_uint32_t_1);
    if (!allocated_img) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Fill the allocated image with some data from the input
    size_t img_data_needed = width * height * 3 / 2;
    size_t to_read = img_data_needed < size ? img_data_needed : size;
    
    // Read image data from input
    size_t bytes_read = 0;
    for (int plane = 0; plane < 3; plane++) {
        int plane_width = (plane == 0) ? width : width / 2;
        int plane_height = (plane == 0) ? height : height / 2;
        
        for (int y = 0; y < plane_height; y++) {
            if (bytes_read >= to_read) break;
            
            size_t row_bytes = plane_width;
            if (row_bytes > (to_read - bytes_read)) {
                row_bytes = to_read - bytes_read;
            }
            
            if (row_bytes > 0) {
                fread(allocated_img->planes[plane] + y * allocated_img->stride[plane],
                      1, row_bytes, in_file);
                bytes_read += row_bytes;
            }
        }
        if (bytes_read >= to_read) break;
    }
    
    // Step 3: Use vpx_img_wrap to wrap existing data
    // Create another image by wrapping the data from allocated_img
    unsigned char *wrapped_data = (unsigned char *)malloc(width * height * 3);
    if (!wrapped_data) {
        vpx_img_free(allocated_img);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Copy some data to the wrapped buffer
    size_t copy_size = (size < width * height * 3) ? size : width * height * 3;
    memcpy(wrapped_data, data, copy_size);
    
    vpx_image_t wrapped_img;
    vpx_image_t *wrapped_result = vpx_img_wrap(
        &wrapped_img, VPX_IMG_FMT_I420, fuzz_uint32_t_2, fuzz_uint32_t_3, 1, wrapped_data);
    
    // Step 4: Use vpx_codec_enc_config_default to get default encoder config
    vpx_codec_enc_cfg_t encoder_config;
    err = vpx_codec_enc_config_default(encoder_iface, &encoder_config, fuzz_uint32_t_4);
    if (err != VPX_CODEC_OK) {
        free(wrapped_data);
        vpx_img_free(allocated_img);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Update config with our dimensions
    encoder_config.g_w = width;
    encoder_config.g_h = height;
    encoder_config.rc_target_bitrate = 256; // 256 kbps
    
    // Step 5: Create encoder context
    vpx_codec_ctx_t encoder;
    err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &encoder_config, fuzz_int64_t_5, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        free(wrapped_data);
        vpx_img_free(allocated_img);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 6: Use vpx_codec_set_cx_data_buf to set output buffer
    vpx_fixed_buf_t output_buffer;
    output_buffer.sz = 1024 * 1024; // 1MB buffer
    output_buffer.buf = malloc(output_buffer.sz);
    
    if (output_buffer.buf) {
        err = vpx_codec_set_cx_data_buf(&encoder, &output_buffer, fuzz_uint32_t_6, fuzz_uint32_t_7);
        // Note: vpx_codec_set_cx_data_buf may not be supported by all codecs
        // We call it as requested, but continue even if it fails
    }
    
    // Encode the allocated image
    err = vpx_codec_encode(&encoder, allocated_img, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10, fuzz_uint64_t_11);
    
    // Try to get encoded data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Write to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Also try to encode the wrapped image
    if (wrapped_result) {
        err = vpx_codec_encode(&encoder, wrapped_result, fuzz_int64_t_12, fuzz_uint64_t_13, fuzz_int64_t_14, fuzz_uint64_t_15);
        
        iter = NULL;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
    }
    
    // Cleanup
    if (output_buffer.buf) {
        free(output_buffer.buf);
    }
    
    vpx_codec_destroy(&encoder);
    free(wrapped_data);
    vpx_img_free(allocated_img);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}