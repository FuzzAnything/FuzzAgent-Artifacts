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
//<ID> 34
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 2
//<Quality> {"density":21,"unique_branches":{"aom_codec_enc_init_ver":[[61,7,61,37,0,0,4,0],[61,41,61,74,0,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error_detail","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_img_wrap","aom_codec_destroy","aom_codec_enc_config_set","aom_img_free","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_img_free","aom_codec_destroy","aom_codec_encode","aom_img_free","aom_codec_destroy","aom_uleb_encode","aom_img_add_metadata","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error_detail","aom_codec_set_frame_buffer_functions","aom_img_wrap","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_uleb_encode","aom_img_add_metadata","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Define frame buffer callbacks
static int  id_000024_get_frame_buffer(void *priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    // Simple implementation that allocates a buffer
    void *buf = malloc(min_size);
    if (!buf) return -1;
    fb->data = (uint8_t *)buf;
    fb->size = min_size;
    return 0;
}

static int  id_000024_release_frame_buffer(void *priv, aom_codec_frame_buffer_t *fb) {
    if (fb->data) {
        free(fb->data);
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if data is too small for basic operations
    if (size < 100) {
	return 0;
	}
	
    
    // Create encoder context
    aom_codec_ctx_t encoder;
    memset(&encoder, 0, sizeof(encoder));
    
    // Get encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
	return 0;
	}
	
    
    // Configure encoder with default settings
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&encoder, iface, &cfg, 0, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Get error detail (for demonstration, not used for control flow)
    const char *error_detail = aom_codec_error_detail(&encoder);
    
    // Set frame buffer functions
    if (aom_codec_set_frame_buffer_functions(&encoder,  id_000024_get_frame_buffer,  id_000024_release_frame_buffer, NULL) != AOM_CODEC_OK) {
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Create image from input data
    const unsigned int width = 16;
    const unsigned int height = 16;
    const unsigned int stride_align = 1;
    
    // Allocate buffer for image data
    size_t img_data_size = width * height * 3; // Approximate size
    std::vector<unsigned char> img_data(img_data_size);
    
    // Copy some input data to image buffer
    size_t copy_size = (size < img_data_size) ? size : img_data_size;
    memcpy(img_data.data(), data, copy_size);
    
    // Wrap the image
    aom_image_t img;
    aom_image_t *image = aom_img_wrap(&img, AOM_IMG_FMT_I420, width, height, 
                                      stride_align, img_data.data());
    if (!image) {
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Reconfigure encoder (just to call the API)
    if (aom_codec_enc_config_set(&encoder, &cfg) != AOM_CODEC_OK) {
        aom_img_free(image);
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Set up output buffer for encoded data
    aom_fixed_buf_t output_buf;
    output_buf.buf = malloc(1024 * 1024); // 1MB buffer
    output_buf.sz = 1024 * 1024;
    
    if (output_buf.buf) {
        // Set the output buffer with padding
        if (aom_codec_set_cx_data_buf(&encoder, &output_buf, 16, 16) != AOM_CODEC_OK) {
            free(output_buf.buf);
            aom_img_free(image);
            aom_codec_destroy(&encoder);
            return 0;
        }
    }
    
    // Encode the image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    if (aom_codec_encode(&encoder, image, pts, duration, flags) != AOM_CODEC_OK) {
        if (output_buf.buf) free(output_buf.buf);
        aom_img_free(image);
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Add metadata to image (using remaining input data)
    if (size > copy_size) {
        uint32_t metadata_type = 1;
        size_t metadata_size = size - copy_size;
        const uint8_t *metadata_data = data + copy_size;
        
        // Use ULEB encoding for metadata size
        uint64_t uleb_value = metadata_size;
        size_t available = 10; // Enough for 64-bit ULEB
        uint8_t coded_value[10];
        size_t coded_size = 0;
        
        aom_uleb_encode(uleb_value, available, coded_value, &coded_size);
        
        // Add metadata to image
        aom_img_add_metadata(image, metadata_type, metadata_data, 
                            metadata_size, AOM_MIF_ANY_FRAME);
    }
    
    // Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        // Process packet if needed
    }
    
    // Cleanup
    if (output_buf.buf) free(output_buf.buf);
    aom_img_free(image);
    aom_codec_destroy(&encoder);
    
    return 0;
}