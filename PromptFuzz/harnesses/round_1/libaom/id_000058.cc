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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error","aom_codec_error_detail","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_destroy","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_error","aom_codec_error_detail","aom_codec_destroy","aom_img_alloc","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_encode","aom_codec_error","aom_codec_error_detail","aom_codec_get_cx_data","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_encode","aom_codec_error","aom_codec_error_detail","aom_codec_get_cx_data","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/*Here's a fuzz driver that decodes AV1 video from input data, then re-encodes it using libaom:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Get version string
    const char *version_str = aom_codec_version_str();
    (void)version_str; // Use to avoid unused variable warning

    // 2. Initialize decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_ctx_t dec_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;

    aom_codec_err_t res = aom_codec_dec_init_ver(&dec_ctx, decoder_iface, &dec_cfg, 0, 
                                                  AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        const char *error = aom_codec_error(&dec_ctx);
        const char *error_detail = aom_codec_error_detail(&dec_ctx);
        (void)error;
        (void)error_detail;
        return 0;
    }

    // 3. Peek stream info
    aom_codec_stream_info_t stream_info = {0};
    if (aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info) == AOM_CODEC_OK) {
        (void)stream_info.w; // Use variables
        (void)stream_info.h;
    }

    // 4. Decode the input
    aom_codec_decode(&dec_ctx, data, size, nullptr);

    // 5. Try to get decoded frames
    aom_codec_iter_t dec_iter = nullptr;
    aom_image_t *decoded_img = nullptr;
    
    while ((decoded_img = aom_codec_get_frame(&dec_ctx, &dec_iter)) != nullptr) {
        // Get image properties
        int plane_width = aom_img_plane_width(decoded_img, 0);
        int plane_height = aom_img_plane_height(decoded_img, 0);
        size_t metadata_count = aom_img_num_metadata(decoded_img);
        (void)plane_width;
        (void)plane_height;
        (void)metadata_count;

        // Remove metadata if any exists
        aom_img_remove_metadata(decoded_img);

        break; // Just process first frame for fuzzing
    }

    if (!decoded_img) {
        aom_codec_destroy(&dec_ctx);
        return 0;
    }

    // 6. Prepare for encoding
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_ctx_t enc_ctx;
    aom_codec_enc_cfg_t enc_cfg;

    // Get default encoder configuration
    res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&dec_ctx);
        return 0;
    }

    // Set encoder configuration based on decoded image
    enc_cfg.g_w = decoded_img->d_w;
    enc_cfg.g_h = decoded_img->d_h;
    enc_cfg.g_threads = 1;
    enc_cfg.rc_target_bitrate = 100; // Low bitrate for fuzzing

    // 7. Initialize encoder
    res = aom_codec_enc_init_ver(&enc_ctx, encoder_iface, &enc_cfg, 0, 
                                  AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        const char *error = aom_codec_error(&enc_ctx);
        const char *error_detail = aom_codec_error_detail(&enc_ctx);
        (void)error;
        (void)error_detail;
        aom_codec_destroy(&dec_ctx);
        return 0;
    }

    // 8. Create a test image with alloc and alloc_with_border
    aom_image_t *test_img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 
                                           decoded_img->d_w, decoded_img->d_h, 16);
    aom_image_t *test_img_with_border = aom_img_alloc_with_border(
        nullptr, AOM_IMG_FMT_I420, decoded_img->d_w, decoded_img->d_h, 16, 16, 32);

    if (test_img) {
        // Get plane dimensions
        int test_plane_width = aom_img_plane_width(test_img, 0);
        int test_plane_height = aom_img_plane_height(test_img, 0);
        size_t test_metadata_count = aom_img_num_metadata(test_img);
        (void)test_plane_width;
        (void)test_plane_height;
        (void)test_metadata_count;
        
        // Remove metadata
        aom_img_remove_metadata(test_img);
        
        // Copy some data from decoded image for testing
        if (decoded_img->planes[0] && test_img->planes[0]) {
            size_t copy_size = (size_t)(test_img->stride[0] * test_img->d_h);
            if (copy_size > 0) {
                size_t src_size = (size_t)(decoded_img->stride[0] * decoded_img->d_h);
                copy_size = copy_size < src_size ? copy_size : src_size;
                if (copy_size > 0 && copy_size <= size) {
                    memcpy(test_img->planes[0], data, copy_size % size);
                }
            }
        }
    }

    // 9. Encode the decoded image
    res = aom_codec_encode(&enc_ctx, decoded_img, 0, 1, 0);
    if (res != AOM_CODEC_OK) {
        const char *error = aom_codec_error(&enc_ctx);
        const char *error_detail = aom_codec_error_detail(&enc_ctx);
        (void)error;
        (void)error_detail;
    }

    // 10. Get encoded data
    aom_codec_iter_t enc_iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    
    while ((pkt = aom_codec_get_cx_data(&enc_ctx, &enc_iter)) != nullptr) {
        // Process different packet types
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Access frame data
            (void)pkt->data.frame.buf;
            (void)pkt->data.frame.sz;
        }
        break; // Just get first packet for fuzzing
    }

    // 11. Clean up
    if (test_img) {
        aom_img_free(test_img);
    }
    if (test_img_with_border) {
        aom_img_free(test_img_with_border);
    }
    
    aom_codec_destroy(&enc_ctx);
    aom_codec_destroy(&dec_ctx);

    return 0;
}