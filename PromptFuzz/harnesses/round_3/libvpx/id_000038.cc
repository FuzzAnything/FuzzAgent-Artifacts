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
//<ID> 116
//<Prompt> []
/*<Combination>: [
*/
//<score> 4.6666665, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_error","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_error","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":2}
/**/


// Include libvpx headers
extern "C" {
}

// Callback function for put_frame
static void  id_000038_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This callback is triggered when a frame is available
    // We can process the frame here, but for fuzzing we'll just count frames
    static int frame_count = 0;
    frame_count++;
}

// Callback function for put_slice
static void  id_000038_put_slice_callback(void *user_priv, const vpx_image_t *img,
                               const vpx_image_rect_t *slice,
                               const vpx_image_rect_t *total) {
    // This callback is triggered when a slice is available
    static int slice_count = 0;
    slice_count++;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
	//fuzzer shim end}




    // Early return if data is too small for meaningful processing
    if (size < 4) {
	return 0;
	}
	
    
    // Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a FILE pointer for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize variables
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t *img = nullptr;
    const vpx_image_t *preview_frame = nullptr;
    vpx_codec_iter_t iter = nullptr;
    vpx_codec_err_t res;
    
    // Determine if we should use VP8 or VP9 based on input size
    // For simplicity, use VP8
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    
    // Initialize encoder configuration with default values
    res = vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_1);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set some configuration values based on input data
    // Use values from the input to vary the configuration
    cfg.g_w = (data[0] % 640) + 64;  // Width between 64 and 703
    cfg.g_h = (data[1] % 480) + 64;  // Height between 64 and 543
    cfg.rc_target_bitrate = (data[2] % 1000) + 100;  // Bitrate between 100 and 1099
    
    // Initialize encoder
    res = vpx_codec_enc_init(&encoder, encoder_iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Apply encoder configuration (call vpx_codec_enc_config_set)
    // This re-applies the configuration to ensure it's properly set
    res = vpx_codec_enc_config_set(&encoder, &cfg);
    
    // Register put_frame callback (call vpx_codec_register_put_frame_cb)
    vpx_codec_register_put_frame_cb(&encoder,  id_000038_put_frame_callback, nullptr);
    
    // Register put_slice callback (call vpx_codec_register_put_slice_cb)
    vpx_codec_register_put_slice_cb(&encoder,  id_000038_put_slice_callback, nullptr);
    
    // Allocate image buffer for encoding
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, fuzz_uint32_t_2);
    if (!img) {
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Fill image with data from input
    // We'll use a simple pattern based on input data
    size_t y_plane_size = cfg.g_w * cfg.g_h;
    size_t uv_plane_size = (cfg.g_w * cfg.g_h) / 4;
    
    // Fill Y plane
    for (size_t i = 0; i < y_plane_size && (i + 3) < size; i++) {
        img->planes[0][i] = data[(i + 3) % size];
    }
    
    // Fill U and V planes
    for (size_t i = 0; i < uv_plane_size && (i + y_plane_size + 3) < size; i++) {
        img->planes[1][i] = data[(i + y_plane_size + 3) % size];
        img->planes[2][i] = data[(i + y_plane_size * 2 + 3) % size];
    }
    
    // Encode the image
    res = vpx_codec_encode(&encoder, img, fuzz_int64_t_3, fuzz_uint64_t_4, fuzz_int64_t_5, fuzz_uint64_t_6);
    
    // Get preview frame (call vpx_codec_get_preview_frame)
    preview_frame = vpx_codec_get_preview_frame(&encoder);
    
    // Write encoded data to output file
    while (const vpx_codec_cx_pkt_t *pkt = vpx_codec_get_cx_data(&encoder, &iter)) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Check for errors (call vpx_codec_error)
    const char *error_str = vpx_codec_error(&encoder);
    if (error_str && strlen(error_str) > 0) {
        // Error occurred during processing
    }
    
    // Now decode the encoded data to test the full pipeline
    // Read back the encoded data
    fflush(out_file);
    assert_file_closed(&out_file);;
    
    // Open the output file for reading
    out_file = fopen("output_file", "rb");
    if (out_file) {
        fseek(out_file, 0, SEEK_END);
        size_t encoded_size = ftell(out_file);
        fseek(out_file, 0, SEEK_SET);
        
        if (encoded_size > 0) {
            std::vector<uint8_t> encoded_data(encoded_size);
            fread(encoded_data.data(), 1, encoded_size, out_file);
            
            // Initialize decoder
            vpx_codec_ctx_t decoder;
            vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
            
            if (vpx_codec_dec_init(&decoder, decoder_iface, nullptr, 0) == VPX_CODEC_OK) {
                // Decode the data
                vpx_codec_decode(&decoder, encoded_data.data(), encoded_size, nullptr, fuzz_int64_t_7);
                
                // Get decoded frames
                vpx_codec_iter_t dec_iter = nullptr;
                while (const vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &dec_iter)) {
                    // Process decoded frame if needed
                }
                
                vpx_codec_destroy(&decoder);
            }
        }
        assert_file_closed(&out_file);;
        remove("output_file");
    }
    
    // Clean up
    vpx_img_free(img);
    vpx_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}