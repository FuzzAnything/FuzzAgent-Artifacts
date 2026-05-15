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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 2
//<Quality> {"density":18,"unique_branches":{"aom_codec_enc_init_ver":[[61,7,61,37,0,0,4,0],[61,41,61,74,0,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error","aom_codec_set_option","aom_codec_set_option","aom_codec_destroy","aom_img_wrap","aom_img_add_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_codec_encode","aom_codec_error","aom_img_free","aom_codec_destroy","aom_codec_get_global_headers","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_img_wrap","aom_img_add_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_codec_encode","aom_codec_get_global_headers","aom_img_free","aom_codec_destroy"],"visited":0}
/*Looking at the APIs provided, I can create a fuzz driver that simulates encoding an image using the libaom AV1 encoder. Here's a step-by-step implementation:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Check minimum size requirement for image data
    if (size < 64) {
        return 0;
    }

    // Create an encoder context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get the AV1 encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Configure encoder settings
    aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    // Get default configuration
    unsigned int usage = 0; // Good quality mode
    if (aom_codec_enc_config_default(iface, &cfg, usage) != AOM_CODEC_OK) {
        return 0;
    }

    // Set basic configuration parameters
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_threads = 1;
    cfg.g_limit = 1; // Encode only one frame
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;

    // Initialize encoder
    unsigned int flags = 0;
    int version = 0; // Use default version
    if (aom_codec_enc_init_ver(&ctx, iface, &cfg, flags, version) != AOM_CODEC_OK) {
        const char *error = aom_codec_error(&ctx);
        (void)error; // Prevent unused variable warning
        return 0;
    }

    // Set encoder options
    aom_codec_set_option(&ctx, "cpu-used", "1");
    aom_codec_set_option(&ctx, "end-usage", "vbr");

    // Prepare image data from fuzzer input
    // Use a portion of the input as image data
    const unsigned int image_width = 64;
    const unsigned int image_height = 64;
    
    // Calculate required size for YV12 format (Y plane + U plane + V plane)
    const size_t y_plane_size = image_width * image_height;
    const size_t uv_plane_size = (image_width / 2) * (image_height / 2);
    const size_t total_image_size = y_plane_size + (uv_plane_size * 2);
    
    // Ensure we have enough data
    if (size < total_image_size) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Create image wrapper
    aom_image_t img;
    memset(&img, 0, sizeof(img));

    // Allocate image buffer
    std::vector<unsigned char> image_buffer(total_image_size);
    
    // Copy fuzzer data to image buffer
    size_t copy_size = std::min(size, total_image_size);
    memcpy(image_buffer.data(), data, copy_size);
    
    // Pad remaining with zeros if needed
    if (copy_size < total_image_size) {
        memset(image_buffer.data() + copy_size, 0, total_image_size - copy_size);
    }

    // Wrap the image data
    aom_img_wrap(&img, AOM_IMG_FMT_YV12, image_width, image_height, 
                1, image_buffer.data());

    // Add metadata to image (if we have extra data after image)
    if (size > total_image_size) {
        size_t metadata_size = size - total_image_size;
        if (metadata_size > 0) {
            // Limit metadata size to reasonable amount
            metadata_size = std::min(metadata_size, (size_t)1024);
            uint32_t metadata_type = 1; // Arbitrary type
            aom_img_add_metadata(&img, metadata_type, 
                                data + total_image_size, metadata_size, 
                                AOM_MIF_ANY_FRAME);
            
            // Get and use the metadata
            size_t index = 0;
            const aom_metadata_t *metadata = aom_img_get_metadata(&img, index);
            (void)metadata; // Prevent unused variable warning
            
            // Remove metadata after use
            aom_img_remove_metadata(&img);
        }
    }

    // Encode the image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t enc_flags = 0;
    
    if (aom_codec_encode(&ctx, &img, pts, duration, enc_flags) != AOM_CODEC_OK) {
        const char *error = aom_codec_error(&ctx);
        (void)error; // Prevent unused variable warning
        aom_img_free(&img);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Get global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        // Write global headers to file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(global_headers->buf, 1, global_headers->sz, out_file);
            assert_file_closed(&out_file);;
        }
    }

    // Clean up
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}