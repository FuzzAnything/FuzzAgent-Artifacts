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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"av1_ml_prune_4_partition":[[1447,9,1447,27,0,0,4,0],[1454,9,1454,27,0,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_iface_name","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error","aom_img_alloc","aom_codec_destroy","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_iface_name","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// The main fuzzing entry point
extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if we don't have enough data for basic operations
    if (size < 100) {
        return 0;
    }

    // Step 1: Create a file handle for the input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Get a file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 3: Create an output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 4: Get a file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    if (out_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 5: Initialize encoder context
    aom_codec_ctx_t codec_ctx;
    memset(&codec_ctx, 0, sizeof(codec_ctx));
    
    // Get the AV1 encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 6: Get codec interface name (required API)
    const char *codec_name = aom_codec_iface_name(iface);
    if (!codec_name) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 7: Get codec capabilities (required API)
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    (void)caps; // Use caps to avoid unused variable warning

    // Step 8: Set up encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Configure basic settings
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;

    // Step 9: Initialize encoder (required API)
    aom_codec_err_t init_result = aom_codec_enc_init_ver(
        &codec_ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    
    if (init_result != AOM_CODEC_OK) {
        // Get error message (required API)
        const char *error_msg = aom_codec_error(&codec_ctx);
        (void)error_msg; // Use error_msg to avoid unused variable warning
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 10: Create an image
    aom_image_t img;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    unsigned int width = 64;
    unsigned int height = 64;
    unsigned int align = 16;
    
    if (!aom_img_alloc(&img, fmt, width, height, align)) {
        aom_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 11: Fill image planes with some data from input
    // Use first part of input data for Y plane
    size_t y_plane_size = img.stride[0] * img.h;
    size_t uv_plane_size = img.stride[1] * (img.h / 2);
    
    // Ensure we don't read beyond input data
    size_t copy_size = y_plane_size;
    if (copy_size > size) copy_size = size;
    if (copy_size > 0) {
        fread(img.planes[0], 1, copy_size, in_file);
    }
    
    // Fill remaining planes with pattern if we have data
    if (size > copy_size && uv_plane_size > 0) {
        size_t remaining = size - copy_size;
        size_t uv_copy = (remaining > uv_plane_size * 2) ? uv_plane_size : remaining / 2;
        
        if (uv_copy > 0) {
            fread(img.planes[1], 1, uv_copy, in_file);
            if (remaining > uv_copy) {
                fread(img.planes[2], 1, uv_copy, in_file);
            }
        }
    }

    // Step 12: Create metadata (required API)
    // Use a portion of input data for metadata
    size_t metadata_size = size > 50 ? 50 : size;
    uint32_t metadata_type = 1; // Arbitrary type
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
    
    aom_metadata_t *metadata = aom_img_metadata_alloc(
        metadata_type, data, metadata_size, insert_flag);
    
    if (metadata) {
        // Note: We would normally attach this to an image, but for fuzzing
        // we just create it and free it to exercise the API
        aom_img_metadata_free(metadata);
    }

    // Step 13: Encode the image (required API)
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    aom_codec_err_t encode_result = aom_codec_encode(
        &codec_ctx, &img, pts, duration, flags);
    
    // Get error message if encoding failed (required API)
    if (encode_result != AOM_CODEC_OK) {
        const char *encode_error = aom_codec_error(&codec_ctx);
        (void)encode_error; // Use encode_error to avoid unused variable warning
    }

    // Step 14: Try to get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&codec_ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Write to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }

    // Step 15: Clean up
    aom_img_free(&img);
    aom_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}