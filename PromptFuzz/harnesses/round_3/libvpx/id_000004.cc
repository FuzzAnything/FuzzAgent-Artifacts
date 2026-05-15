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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"pick_quickcompress_mode":[[833,14,833,38,0,0,4,0]]},"library_calls":["vpx_codec_version_str","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000004_put_frame_cb(void* user_priv, const vpx_image_t* img) {
    // This callback receives decoded frames
    // For fuzzing, we don't need to do anything with them
    (void)user_priv;
    (void)img;
}

// Callback function for put_slice
static void  id_000004_put_slice_cb(void* user_priv, const vpx_image_t* img,
                         const vpx_image_rect_t* rect,
                         const vpx_image_rect_t* rect2) {
    // This callback receives decoded slices
    // For fuzzing, we don't need to do anything with them
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<96) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Use vpx_codec_version_str() - required API
    const char* version = vpx_codec_version_str();
    (void)version; // Prevent unused variable warning

    // Create a FILE* to read input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // First, try to decode the input data (assuming it's VP8 encoded data)
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp8_dx();
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, NULL, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION)) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Register callbacks - required APIs
    vpx_codec_register_put_frame_cb(&decoder,  id_000004_put_frame_cb, NULL);
    vpx_codec_register_put_slice_cb(&decoder,  id_000004_put_slice_cb, NULL);

    // Read and decode the input data
    std::vector<uint8_t> buffer(size);
    size_t bytes_read = fread(buffer.data(), 1, size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read > 0) {
        vpx_codec_decode(&decoder, buffer.data(), bytes_read, NULL, fuzz_int64_t_2);
        
        // Get stream info - required API
        vpx_codec_stream_info_t si;
        si.sz = sizeof(si);
        vpx_codec_get_stream_info(&decoder, &si);
    }

    // Now set up an encoder to re-encode the decoded frames
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx(); // required API
    
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_3)) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Configure encoder with reasonable values
    cfg.g_w = 320;
    cfg.g_h = 240;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    // Initialize encoder - required API
    if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION)) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set encoder configuration - required API
    vpx_codec_enc_config_set(&encoder, &cfg);

    // Create a test image for encoding
    vpx_image_t* img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, fuzz_uint32_t_5);
    if (!img) {
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Fill image with some data (use fuzzed data if available)
    size_t y_size = cfg.g_w * cfg.g_h;
    size_t uv_size = ((cfg.g_w + 1) / 2) * ((cfg.g_h + 1) / 2);
    
    if (size >= y_size + uv_size * 2) {
        memcpy(img->planes[0], data, y_size);
        memcpy(img->planes[1], data + y_size, uv_size);
        memcpy(img->planes[2], data + y_size + uv_size, uv_size);
    } else {
        // Fill with default data if not enough fuzzed data
        memset(img->planes[0], 128, y_size);
        memset(img->planes[1], 128, uv_size);
        memset(img->planes[2], 128, uv_size);
    }

    // Encode the image
    vpx_codec_encode(&encoder, img, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);

    // Get encoded data - required API
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t* pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Got encoded frame packet
            // For fuzzing, we just want to exercise the API
            // No need to do anything with the packet
        }
    }

    // Encode another frame to exercise more code paths
    vpx_codec_encode(&encoder, NULL, fuzz_int64_t_10, fuzz_uint64_t_11, fuzz_int64_t_12, fuzz_uint64_t_13);
    
    // Get remaining data
    iter = NULL;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
        // Process any remaining packets
    }

    // Clean up
    vpx_img_free(img);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);

    assert_file_closed(&in_file);
	return 0;
}