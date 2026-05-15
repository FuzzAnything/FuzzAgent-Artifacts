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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_error","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_frame","aom_codec_get_preview_frame","aom_img_set_rect","aom_img_add_metadata","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_frame","aom_codec_get_preview_frame","aom_img_set_rect","aom_img_add_metadata","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to decode an AV1 bitstream, process the decoded frames, and add metadata:

*/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000053_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    fb->data = new uint8_t[min_size];
    fb->size = min_size;
    *(static_cast<uint8_t**>(cb_priv)) = static_cast<uint8_t*>(fb->data);
    return 0;
}

static int  id_000053_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    delete[] static_cast<uint8_t*>(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // Create input FILE* using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output FILE*
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; (void)out_fd; // Mark as used
    
    // 1. Get codec capabilities
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Mark as used
    
    // 2. Get codec interface name
    const char *iface_name = aom_codec_iface_name(decoder_iface);
    (void)iface_name; // Mark as used
    
    // Initialize decoder
    aom_codec_ctx_t codec_ctx;
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&codec_ctx, decoder_iface, &cfg, 0, 
                                                 AOM_DECODER_ABI_VERSION);
    
    if (err != AOM_CODEC_OK) {
        // 3. Get error message
        const char *error_msg = aom_codec_error(&codec_ctx);
        (void)error_msg; // Mark as used
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 4. Set frame buffer functions
    uint8_t *frame_buffer_ptr = nullptr;
    err = aom_codec_set_frame_buffer_functions(&codec_ctx,  id_000053_get_frame_buffer, 
                                                id_000053_release_frame_buffer, &frame_buffer_ptr);
    (void)err; // Error checking omitted for fuzzing
    
    // Read input data in chunks and decode
    std::vector<uint8_t> buffer(4096);
    size_t bytes_read;
    while ((bytes_read = fread(buffer.data(), 1, buffer.size(), in_file)) > 0) {
        err = aom_codec_decode(&codec_ctx, buffer.data(), bytes_read, nullptr);
        
        if (err != AOM_CODEC_OK) {
            break;
        }
        
        // Get decoded frames
        aom_codec_iter_t iter = nullptr;
        aom_image_t *img;
        
        while ((img = aom_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
            // 5. Get preview frame
            const aom_image_t *preview = aom_codec_get_preview_frame(&codec_ctx);
            (void)preview; // Mark as used
            
            // 6. Set rectangle on image (crop to 1/4 of the image)
            unsigned int crop_w = img->d_w / 4;
            unsigned int crop_h = img->d_h / 4;
            if (crop_w > 0 && crop_h > 0) {
                aom_img_set_rect(img, 0, 0, crop_w, crop_h, 0);
            }
            
            // 7. Add metadata to image
            uint32_t metadata_type = 1; // Example type
            std::vector<uint8_t> metadata_data = {0x01, 0x02, 0x03, 0x04};
            if (metadata_data.size() > 0) {
                aom_img_add_metadata(img, metadata_type, metadata_data.data(), 
                                    metadata_data.size(), AOM_MIF_ANY_FRAME);
            }
            
            // Write image data to output (simplified)
            for (int plane = 0; plane < 3; ++plane) {
                if (img->planes[plane]) {
                    int stride = img->stride[plane];
                    int height = aom_img_plane_height(img, plane);
                    fwrite(img->planes[plane], 1, stride * height, out_file);
                }
            }
        }
    }
    
    // 8. Set compressed data buffer (simulate encoding scenario)
    aom_fixed_buf_t cx_buf;
    std::vector<uint8_t> compressed_data(1024);
    cx_buf.buf = compressed_data.data();
    cx_buf.sz = compressed_data.size();
    
    // Re-initialize as encoder for set_cx_data_buf
    aom_codec_ctx_t enc_ctx;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
        enc_cfg.g_w = 640;
        enc_cfg.g_h = 480;
        
        if (aom_codec_enc_init_ver(&enc_ctx, encoder_iface, &enc_cfg, 0, 
                                   AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
            aom_codec_set_cx_data_buf(&enc_ctx, &cx_buf, 0, 0);
            
            // Clean up encoder
            aom_codec_destroy(&enc_ctx);
        }
    }
    
    // Clean up
    aom_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}