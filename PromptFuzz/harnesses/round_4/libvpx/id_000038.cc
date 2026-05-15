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
//<ID> 361
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 2
//<Quality> {"density":22,"unique_branches":{"vpx_codec_get_cx_data":[[271,9,273,41,0,0,4,1]],"vp8_update_rate_correction_factors":[[1122,14,1122,36,0,0,4,1]]},"library_calls":["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_img_alloc","vpx_codec_destroy","vpx_img_wrap","vpx_img_set_rect","vpx_img_flip","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_img_alloc","vpx_img_wrap","vpx_img_set_rect","vpx_img_flip","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_img_free","vpx_codec_destroy"],"visited":0}
/*Looking at these APIs, I can see they're related to encoding with libvpx. The event they could achieve together is: **initialize an encoder configuration and set up image buffers for video encoding**. Let me create a fuzz driver that demonstrates this:

*/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<104) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_13, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_14, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_15, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_16, fdp);
	//fuzzer shim end}




    // Early return if we don't have enough data
    if (size < 100) {
	return 0;
	}
	
    
    // Create a FILE* for reading input data if needed
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create an output file for encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t *img = nullptr;
    vpx_fixed_buf_t output_buffer;
    
    // Initialize variables
    int fd = fuzz_fileno(in_file); // File descriptor example
    (void)fd; // Mark as used
    
    // 1. Get default encoder configuration
    vpx_codec_err_t err = vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, fuzz_uint32_t_1);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set some configuration values from input data
    // Use the first few bytes to determine dimensions (modulo to keep reasonable)
    cfg.g_w = (data[0] % 64 + 64);  // Width between 64-127
    cfg.g_h = (data[1] % 64 + 64);  // Height between 64-127
    cfg.rc_target_bitrate = (data[2] % 1000 + 100); // Bitrate between 100-1099
    
    // 2. Initialize encoder
    err = vpx_codec_enc_init_ver(&codec, vpx_codec_vp8_cx(), &cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Try to peek stream info (even though this is typically for decoding)
    // We'll use a portion of the input data as if it were encoded data
    vpx_codec_stream_info_t si;
    si.sz = sizeof(si);
    
    // Use a small portion of the input data for peek_stream_info
    size_t peek_size = size > 1000 ? 1000 : size;
    vpx_codec_peek_stream_info(vpx_codec_vp8_dx(), data, peek_size, &si);
    
    // 4. Create image using vpx_img_alloc
    // We need to ensure we have enough data for image allocation
    size_t img_data_needed = cfg.g_w * cfg.g_h * 3 / 2; // YUV420 format
    if (size < img_data_needed + 100) {
        // Not enough data, use a smaller image
        cfg.g_w = 64;
        cfg.g_h = 64;
        img_data_needed = cfg.g_w * cfg.g_h * 3 / 2;
    }
    
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, fuzz_uint32_t_3);
    if (!img) {
        vpx_codec_destroy(&codec);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Fill image planes with input data
    size_t offset = 0;
    for (int plane = 0; plane < 3; plane++) {
        int plane_width = (plane == 0) ? cfg.g_w : cfg.g_w / 2;
        int plane_height = (plane == 0) ? cfg.g_h : cfg.g_h / 2;
        
        for (int y = 0; y < plane_height; y++) {
            size_t bytes_to_copy = plane_width;
            if (offset + bytes_to_copy > size) {
                bytes_to_copy = size - offset;
                if (bytes_to_copy <= 0) break;
            }
            
            memcpy(&img->planes[plane][y * img->stride[plane]], 
                   &data[offset], bytes_to_copy);
            offset += bytes_to_copy;
            
            // Pad with zeros if we ran out of data
            if (bytes_to_copy < (size_t)plane_width) {
                memset(&img->planes[plane][y * img->stride[plane] + bytes_to_copy], 
                       0, plane_width - bytes_to_copy);
            }
            
            if (offset >= size) break;
        }
        if (offset >= size) break;
    }
    
    // 5. Try vpx_img_wrap with a different portion of the data
    // Allocate a buffer for wrapped image
    size_t wrap_buffer_size = cfg.g_w * cfg.g_h * 3 / 2;
    unsigned char *wrap_buffer = (unsigned char *)malloc(wrap_buffer_size);
    if (wrap_buffer) {
        // Copy some data into the buffer
        size_t copy_size = size > wrap_buffer_size ? wrap_buffer_size : size;
        memcpy(wrap_buffer, data, copy_size);
        
        // Create another image using vpx_img_wrap
        vpx_image_t wrapped_img;
        vpx_img_wrap(&wrapped_img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, fuzz_uint32_t_4, wrap_buffer);
        
        // Try to set a rectangle on the wrapped image
        vpx_img_set_rect(&wrapped_img, 
                         data[3] % (cfg.g_w/2),  // x
                         data[4] % (cfg.g_h/2),  // y
                         cfg.g_w/2,              // width
                         cfg.g_h/2);             // height
        
        // Flip the wrapped image
        vpx_img_flip(&wrapped_img);
        
        // Free the wrapped image buffer
        free(wrap_buffer);
    }
    
    // 6. Set up output buffer for encoded data
    output_buffer.buf = malloc(size * 2); // Allocate buffer twice input size
    output_buffer.sz = size * 2;
    
    if (output_buffer.buf) {
        // 7. Use vpx_codec_set_cx_data_buf
        vpx_codec_set_cx_data_buf(&codec, &output_buffer, fuzz_uint32_t_5, fuzz_uint32_t_6);
        
        // 8. Encode the image
        err = vpx_codec_encode(&codec, img, fuzz_int64_t_7, fuzz_uint64_t_8, fuzz_int64_t_9, fuzz_uint64_t_10);
        
        if (err == VPX_CODEC_OK) {
            // Try to get encoded data
            vpx_codec_iter_t iter = nullptr;
            const vpx_codec_cx_pkt_t *pkt;
            
            while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Write to output file
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
        }
        
        // Clean up output buffer
        free(output_buffer.buf);
    }
    
    // 9. Try to encode multiple frames with different configurations
    if (size > img_data_needed * 2) {
        // Reconfigure encoder with different settings
        cfg.g_w = (data[5] % 128 + 128);  // Width between 128-255
        cfg.g_h = (data[6] % 128 + 128);  // Height between 128-255
        
        // Re-initialize encoder
        vpx_codec_destroy(&codec);
        vpx_codec_enc_init_ver(&codec, vpx_codec_vp8_cx(), &cfg, fuzz_int64_t_11, VPX_ENCODER_ABI_VERSION);
        
        // Create a new image
        vpx_image_t *img2 = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, fuzz_uint32_t_12);
        if (img2) {
            // Fill with more data from input
            offset = img_data_needed;
            for (int plane = 0; plane < 3; plane++) {
                int plane_width = (plane == 0) ? cfg.g_w : cfg.g_w / 2;
                int plane_height = (plane == 0) ? cfg.g_h : cfg.g_h / 2;
                
                for (int y = 0; y < plane_height; y++) {
                    size_t bytes_to_copy = plane_width;
                    if (offset + bytes_to_copy > size) {
                        bytes_to_copy = size - offset;
                        if (bytes_to_copy <= 0) break;
                    }
                    
                    memcpy(&img2->planes[plane][y * img2->stride[plane]], 
                           &data[offset], bytes_to_copy);
                    offset += bytes_to_copy;
                    
                    if (offset >= size) break;
                }
                if (offset >= size) break;
            }
            
            // Encode the second image
            vpx_codec_encode(&codec, img2, fuzz_int64_t_13, fuzz_uint64_t_14, fuzz_int64_t_15, fuzz_uint64_t_16);
            vpx_img_free(img2);
        }
    }
    
    // Clean up
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}