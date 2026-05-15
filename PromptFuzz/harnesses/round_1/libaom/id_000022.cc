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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"aom_codec_set_frame_buffer_functions":[[129,14,129,71,0,0,4,0]]},"library_calls":["aom_codec_version","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_img_alloc_with_border","aom_codec_destroy","aom_img_plane_height","aom_img_set_rect","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_codec_set_cx_data_buf","aom_codec_get_global_headers","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_error_detail","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_version","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_img_alloc_with_border","aom_img_plane_height","aom_img_set_rect","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_codec_set_cx_data_buf","aom_codec_get_global_headers","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_error_detail","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom callbacks for frame buffer functions
static int  id_000022_dummy_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)min_size;
    (void)cb_priv;
    fb->data = static_cast<uint8_t*>(malloc(1024));
    fb->size = 1024;
    return fb->data ? 0 : -1;
}

static int  id_000022_dummy_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Check minimal size requirement
    if (size < 64) {
	return 0;
	}
	
    
    // 1. Check codec version
    int version = aom_codec_version();
    (void)version;
    
    // Create encoder context
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    // Initialize encoder with default config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set small resolution for fuzzing efficiency
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_lag_in_frames = 0;
    
    if (aom_codec_enc_init_ver(&encoder, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // 2. Set frame buffer functions
    if (aom_codec_set_frame_buffer_functions(&encoder, 
                                             id_000022_dummy_get_frame_buffer,
                                             id_000022_dummy_release_frame_buffer,
                                            nullptr) != AOM_CODEC_OK) {
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // 3. Create and setup image
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    // Allocate image with border
    unsigned int border = 16;
    if (!aom_img_alloc_with_border(&img, AOM_IMG_FMT_I420, 64, 64, 1, 1, border)) {
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Fill image planes with input data (capped to available size)
    size_t data_used = 0;
    for (int plane = 0; plane < 3; ++plane) {
        size_t plane_size = img.stride[plane] * aom_img_plane_height(&img, plane);
        size_t copy_size = std::min(plane_size, size - data_used);
        if (copy_size > 0) {
            memcpy(img.planes[plane], data + data_used, copy_size);
            data_used += copy_size;
        }
    }
    
    // 4. Set rectangle on image (using remaining data for coordinates)
    if (size - data_used >= 5) {
        unsigned int x = data[data_used] % 32;
        unsigned int y = data[data_used + 1] % 32;
        unsigned int w = 8 + (data[data_used + 2] % 24);
        unsigned int h = 8 + (data[data_used + 3] % 24);
        unsigned int rect_border = data[data_used + 4] % 8;
        aom_img_set_rect(&img, x, y, w, h, rect_border);
        data_used += 5;
    }
    
    // 5. Create metadata (using remaining data)
    if (size - data_used > 4) {
        uint32_t type = 0;
        size_t metadata_size = std::min<size_t>(size - data_used - 4, 256);
        
        // Extract metadata type and data
        if (size - data_used >= 4) {
            memcpy(&type, data + data_used, 4);
            data_used += 4;
        }
        
        // Create metadata
        aom_metadata_t *metadata = aom_img_metadata_alloc(
            type,
            data + data_used,
            metadata_size,
            AOM_MIF_ANY_FRAME
        );
        
        if (metadata) {
            // Add metadata to image
            aom_img_add_metadata(&img, metadata->type, metadata->payload, 
                                metadata->sz, metadata->insert_flag);
            aom_img_metadata_free(metadata);
        }
        data_used += metadata_size;
    }
    
    // 6. Setup output buffer for encoded data
    aom_fixed_buf_t output_buf;
    output_buf.buf = malloc(4096);
    output_buf.sz = 4096;
    
    if (output_buf.buf) {
        // Use remaining data for padding values
        unsigned int pad_before = 0;
        unsigned int pad_after = 0;
        if (size - data_used >= 2) {
            pad_before = data[data_used] % 64;
            pad_after = data[data_used + 1] % 64;
            data_used += 2;
        }
        
        aom_codec_set_cx_data_buf(&encoder, &output_buf, pad_before, pad_after);
    }
    
    // 7. Get global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder);
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    // 8. Encode the image
    aom_codec_encode(&encoder, &img, 0, 1, 0);
    
    // 9. Get preview frame if available
    const aom_image_t *preview = aom_codec_get_preview_frame(&encoder);
    (void)preview;
    
    // 10. Get error details if any
    const char *error_detail = aom_codec_error_detail(&encoder);
    (void)error_detail;
    
    // Cleanup
    aom_img_free(&img);
    if (output_buf.buf) {
        free(output_buf.buf);
    }
    aom_codec_destroy(&encoder);
    
    return 0;
}