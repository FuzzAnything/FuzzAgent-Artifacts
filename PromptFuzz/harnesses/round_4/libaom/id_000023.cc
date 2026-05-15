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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"aom_codec_enc_config_default":[[107,12,107,50,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_img_alloc_with_border","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_img_free","aom_codec_destroy","aom_img_wrap","aom_img_add_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_img_wrap","aom_img_add_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_12, fdp);
	//fuzzer shim end}




    if (!data || size < 10) {
        return 0;
    }

    // Step 1: Parse stream information from input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    aom_codec_iface_t *codec_iface = aom_codec_av1_dx();
    aom_codec_peek_stream_info(codec_iface, data, size, &stream_info);

    // Step 2: Initialize encoder configuration
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t cfg;
    
    if (aom_codec_enc_config_default(codec_iface, &cfg, fuzz_uint32_t_1) != AOM_CODEC_OK) {
        return 0;
    }

    // Use stream info to set configuration
    cfg.g_w = stream_info.w > 0 ? stream_info.w : 64;
    cfg.g_h = stream_info.h > 0 ? stream_info.h : 64;
    cfg.g_limit = 1; // Only encode one frame
    cfg.rc_target_bitrate = 256;

    // Step 3: Initialize encoder
    if (aom_codec_enc_init_ver(&encoder, codec_iface, &cfg, fuzz_int64_t_2, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // Step 4: Set encoder configuration (call this API)
    aom_codec_enc_config_set(&encoder, &cfg);

    // Step 5: Create image with border using input data
    unsigned int width = cfg.g_w;
    unsigned int height = cfg.g_h;
    unsigned int border = 16;
    
    aom_image_t img_with_border;
    if (!aom_img_alloc_with_border(&img_with_border, AOM_IMG_FMT_I420, width, height, 
                                    fuzz_uint32_t_3, fuzz_uint32_t_4, fuzz_uint32_t_5)) {
        aom_codec_destroy(&encoder);
        return 0;
    }

    // Fill image planes with data from input
    size_t data_offset = 0;
    for (int plane = 0; plane < 3; ++plane) {
        int plane_width = aom_img_plane_width(&img_with_border, fuzz_int32_t_6);
        int plane_height = aom_img_plane_height(&img_with_border, fuzz_int32_t_7);
        size_t plane_size = plane_width * plane_height;
        
        if (plane_size > 0) {
            size_t copy_size = std::min(plane_size, size - data_offset);
            if (copy_size > 0) {
                memcpy(img_with_border.planes[plane], data + data_offset, copy_size);
                data_offset = (data_offset + copy_size) % size;
            }
        }
    }

    // Step 6: Create wrapped image (using same image data from img_with_border)
    aom_image_t wrapped_img;
    
    // Calculate strides for wrapping
    unsigned int stride_align = 32;
    unsigned int stride_y = ((width + stride_align - 1) / stride_align) * stride_align;
    unsigned int stride_uv = ((width / 2 + stride_align - 1) / stride_align) * stride_align;
    
    // Allocate buffer for wrapped image
    size_t y_plane_size = stride_y * height;
    size_t uv_plane_size = stride_uv * height / 2;
    size_t total_size = y_plane_size + 2 * uv_plane_size;
    
    unsigned char *img_buffer = (unsigned char *)malloc(total_size);
    if (!img_buffer) {
        aom_img_free(&img_with_border);
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Fill wrapped image buffer with input data
    data_offset = 0;
    memcpy(img_buffer, data, std::min(total_size, size));
    if (size < total_size && size > 0) {
        // Repeat input data if needed
        for (size_t i = size; i < total_size; i++) {
            img_buffer[i] = data[i % size];
        }
    }
    
    aom_img_wrap(&wrapped_img, AOM_IMG_FMT_I420, width, height, fuzz_uint32_t_8, img_buffer);

    // Step 7: Add metadata to wrapped image
    uint32_t metadata_type = 1;
    size_t metadata_size = std::min((size_t)100, size);
    
    if (metadata_size > 0) {
        aom_img_add_metadata(&wrapped_img, fuzz_uint32_t_9, data, metadata_size, 
                            AOM_MIF_ANY_FRAME);
    }

    // Step 8: Encode the image
    aom_codec_encode(&encoder, &img_with_border, fuzz_int64_t_10, fuzz_uint64_t_11, fuzz_int64_t_12);
    
    // Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Encoded frame available
            break;
        }
    }

    // Cleanup
    free(img_buffer);
    aom_img_free(&wrapped_img);
    aom_img_free(&img_with_border);
    aom_codec_destroy(&encoder);

    return 0;
}