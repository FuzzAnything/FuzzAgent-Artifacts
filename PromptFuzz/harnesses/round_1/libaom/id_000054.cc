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
//<ID> 119
//<Prompt> []
/*<Combination>: [
*/
//<score> 31, nr_unique_branch: 0
//<Quality> {"density":31,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_decode","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy","aom_img_alloc_with_border","aom_codec_destroy","aom_img_plane_height","aom_img_plane_width","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_free","aom_codec_destroy","aom_codec_enc_init_ver","aom_img_free","aom_codec_destroy","aom_codec_enc_config_set","aom_codec_error_detail","aom_codec_destroy","aom_img_free","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_codec_control","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_error_detail","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_plane_height","aom_img_plane_width","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_error_detail","aom_codec_set_cx_data_buf","aom_codec_control","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_error_detail","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified libaom APIs to decode and process encoded video data, then re-encode it while extracting a preview frame:

*/


// Assuming the libaom headers are included
// In practice, these would be: #include "aom/aom.h", #include "aom/aom_codec.h", etc.

extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Need minimum data for processing
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION)) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read encoded data from file
    uint8_t *enc_data = (uint8_t *)malloc(size);
    if (!enc_data) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(enc_data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Decode the frame
    if (aom_codec_decode(&decoder, enc_data, size, nullptr)) {
        free(enc_data);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
    
    if (!decoded_img) {
        free(enc_data);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create image with border for encoding
    aom_image_t input_img;
    unsigned int border = 16;
    unsigned int align = 32;
    unsigned int size_align = 4096;
    
    aom_image_t *img_with_border = aom_img_alloc_with_border(
        &input_img, 
        AOM_IMG_FMT_I420, 
        decoded_img->d_w, 
        decoded_img->d_h, 
        align, 
        size_align, 
        border
    );
    
    if (!img_with_border) {
        free(enc_data);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Copy decoded image to bordered image (simplified - in practice would need proper plane copying)
    if (decoded_img->fmt == AOM_IMG_FMT_I420 && img_with_border->fmt == AOM_IMG_FMT_I420) {
        for (int plane = 0; plane < 3; ++plane) {
            int src_stride = decoded_img->stride[plane];
            int dst_stride = img_with_border->stride[plane];
            int height = aom_img_plane_height(decoded_img, plane);
            int width = aom_img_plane_width(decoded_img, plane);
            
            for (int y = 0; y < height; ++y) {
                memcpy(
                    img_with_border->planes[plane] + y * dst_stride,
                    decoded_img->planes[plane] + y * src_stride,
                    width
                );
            }
        }
    }

    // Create metadata for the image
    uint32_t metadata_type = 1; // Example metadata type
    uint8_t metadata_data[] = {0x01, 0x02, 0x03, 0x04};
    size_t metadata_size = sizeof(metadata_data);
    
    aom_metadata_t *metadata = aom_img_metadata_alloc(
        metadata_type,
        metadata_data,
        metadata_size,
        AOM_MIF_ANY_FRAME
    );
    
    if (metadata) {
        // Add metadata to image (assuming aom_img_add_metadata exists - though not in our required list)
        // For demonstration, we just allocate and free
        aom_img_metadata_free(metadata);
    }

    // Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    aom_codec_enc_cfg_t enc_cfg;
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0)) {
        aom_img_free(img_with_border);
        free(enc_data);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set encoder configuration
    enc_cfg.g_w = img_with_border->d_w;
    enc_cfg.g_h = img_with_border->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;

    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION)) {
        aom_img_free(img_with_border);
        free(enc_data);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set encoder configuration (required API call)
    if (aom_codec_enc_config_set(&encoder, &enc_cfg)) {
        const char *error_detail = aom_codec_error_detail(&encoder);
        // error_detail could be logged or used
    }

    // Set up output buffer for encoded data
    size_t output_buf_size = 1024 * 1024; // 1MB
    uint8_t *output_buffer = (uint8_t *)malloc(output_buf_size);
    if (!output_buffer) {
        aom_codec_destroy(&encoder);
        aom_img_free(img_with_border);
        free(enc_data);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    aom_fixed_buf_t output_buf;
    output_buf.buf = output_buffer;
    output_buf.sz = output_buf_size;

    // Set output buffer for encoder (with padding)
    unsigned int pad_before = 64;
    unsigned int pad_after = 64;
    aom_codec_set_cx_data_buf(&encoder, &output_buf, pad_before, pad_after);

    // Set control to enable preview (example control ID)
    int enable_preview = 1;
    aom_codec_control(&encoder, AOME_SET_ENABLEAUTOALTREF, enable_preview);

    // Encode the frame
    aom_codec_encode(&encoder, img_with_border, 0, 1, 0);

    // Get preview frame from encoder
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&encoder);
    if (preview_frame) {
        // Preview frame available - could be processed further
        // For fuzzing, we just validate it exists
    }

    // Check for any errors and get detailed error message
    const char *error_detail = aom_codec_error_detail(&encoder);
    // error_detail could be logged or used for debugging

    // Clean up
    free(output_buffer);
    aom_codec_destroy(&encoder);
    aom_img_free(img_with_border);
    free(enc_data);
    aom_codec_destroy(&decoder);

    assert_file_closed(&in_file);
	return 0;
}