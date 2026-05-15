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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 2
//<Quality> {"density":11,"unique_branches":{"aom_memalign":[[57,7,57,52,0,0,4,0]],"img_alloc_helper":[[172,7,172,21,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_img_alloc_with_border","aom_img_free","aom_img_wrap","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_img_free"],"critical_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_img_alloc_with_border","aom_img_wrap","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
	//fuzzer shim end}




    // We need at least some minimum data for stream info and metadata
    if (size < 100) {
	return 0;
	}
	
    
    // Create a FILE pointer for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 1. First use aom_codec_peek_stream_info to get stream information
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Get decoder interface for AV1
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Peek at stream info from the input data
    aom_codec_err_t peek_err = aom_codec_peek_stream_info(
        decoder_iface, data, size, &stream_info);
    
    // 2. Create an image using aom_img_alloc_with_border
    aom_image_t image;
    memset(&image, 0, sizeof(image));
    
    // Use dimensions from stream info if available, otherwise use defaults
    unsigned int width = stream_info.w > 0 ? stream_info.w : 640;
    unsigned int height = stream_info.h > 0 ? stream_info.h : 480;
    unsigned int border = 16;
    
    aom_image_t *img_ptr = aom_img_alloc_with_border(
        &image, 
        AOM_IMG_FMT_I420, 
        width, 
        height, 
        fuzz_uint32_t_1,      // align
        fuzz_uint32_t_2,      // size_align
        fuzz_uint32_t_3
    );
    
    if (!img_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Also create an image using aom_img_wrap
    // Allocate some memory for the wrapped image
    unsigned char *wrapped_img_data = (unsigned char *)malloc(
        width * height * 3 / 2);  // I420 format
    if (!wrapped_img_data) {
        aom_img_free(img_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    aom_image_t wrapped_image;
    memset(&wrapped_image, 0, sizeof(wrapped_image));
    
    aom_image_t *wrapped_img_ptr = aom_img_wrap(
        &wrapped_image,
        AOM_IMG_FMT_I420,
        width,
        height,
        fuzz_uint32_t_4,      // stride_align
        wrapped_img_data
    );
    
    // 4. Add metadata to the first image
    // Use part of the input data as metadata
    size_t metadata_size = size > 1024 ? 1024 : size;
    uint32_t metadata_type = 1;  // Example metadata type
    
    // Use a portion of the input data for metadata
    const uint8_t *metadata_data = data + (size - metadata_size) % size;
    
    int add_meta_result = aom_img_add_metadata(
        img_ptr,
        fuzz_uint32_t_5,
        metadata_data,
        metadata_size,
        AOM_MIF_ANY_FRAME
    );
    
    // 5. Initialize encoder
    aom_codec_ctx_t encoder;
    memset(&encoder, 0, sizeof(encoder));
    
    // Get encoder interface for AV1
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    // Create encoder configuration
    aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    // Set basic configuration
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_profile = 0;
    cfg.g_bit_depth = AOM_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.rc_end_usage = AOM_VBR;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    // Initialize encoder with version 10 (AOM_ENCODER_ABI_VERSION)
    aom_codec_err_t enc_init_err = aom_codec_enc_init_ver(
        &encoder,
        encoder_iface,
        &cfg,
        fuzz_int64_t_6,      // flags
        fuzz_int32_t_7      // AOM_ENCODER_ABI_VERSION
    );
    
    // 6. Set encoder configuration (re-apply with possible modifications)
    if (enc_init_err == AOM_CODEC_OK) {
        // Modify some configuration values
        cfg.rc_target_bitrate = 2000;
        
        aom_codec_err_t config_set_err = aom_codec_enc_config_set(
            &encoder,
            &cfg
        );
        
        // Try to encode a frame if we have valid images
        if (img_ptr && config_set_err == AOM_CODEC_OK) {
            // Fill image with some data from input
            size_t data_to_copy = size;
            if (data_to_copy > (size_t)(width * height * 3 / 2)) {
                data_to_copy = width * height * 3 / 2;
            }
            
            // Copy data to image planes (simplified - real usage would need proper plane handling)
            for (int plane = 0; plane < 3; plane++) {
                if (img_ptr->planes[plane]) {
                    size_t plane_size = img_ptr->stride[plane] * 
                                       aom_img_plane_height(img_ptr, fuzz_int32_t_8);
                    size_t copy_size = data_to_copy > plane_size ? plane_size : data_to_copy;
                    if (copy_size > 0) {
                        memcpy(img_ptr->planes[plane], data, copy_size);
                        data_to_copy -= copy_size;
                    }
                }
            }
            
            // Encode the image
            aom_codec_encode(&encoder, img_ptr, fuzz_int64_t_9, fuzz_uint64_t_10, fuzz_int64_t_11);
            
            // Get compressed data
            aom_codec_iter_t iter = NULL;
            const aom_codec_cx_pkt_t *pkt;
            while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
                // Process packet if needed
            }
        }
        
        // Destroy encoder
        aom_codec_destroy(&encoder);
    }
    
    // Cleanup
    if (wrapped_img_ptr) {
        aom_img_free(wrapped_img_ptr);
    }
    free(wrapped_img_data);
    
    if (img_ptr) {
        aom_img_free(img_ptr);
    }
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}