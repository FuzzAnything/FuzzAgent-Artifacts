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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"aom_img_add_metadata":[[393,7,393,21,0,0,4,1]]},"library_calls":["aom_codec_version_str","aom_codec_version_extra_str","aom_codec_peek_stream_info","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_destroy","aom_img_free","aom_codec_get_frame","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_version_extra_str","aom_codec_peek_stream_info","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_destroy","aom_img_free","aom_codec_get_frame","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if we don't have enough data
    if (size < 10) {
        return 0;
    }

    // Get version strings (these don't consume input data)
    const char *version_str = aom_codec_version_str();
    const char *version_extra_str = aom_codec_version_extra_str();
    
    // Use these to avoid unused variable warnings
    (void)version_str;
    (void)version_extra_str;

    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // First, try to peek at stream info from the input data
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    
    // Consume some data for peek_stream_info
    std::vector<uint8_t> peek_data(std::min(size, (size_t)1024));
    size_t peek_read = fread(peek_data.data(), 1, peek_data.size(), in_file);
    if (peek_read > 0) {
        aom_codec_peek_stream_info(aom_codec_av1_dx(), peek_data.data(), peek_read, &si);
    }

    // Reset file position for decoder
    fseek(in_file, 0, SEEK_SET);

    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init(&decoder, aom_codec_av1_dx(), &dec_cfg, 0);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get stream info from decoder
    aom_codec_get_stream_info(&decoder, &si);

    // Read and decode the entire input
    std::vector<uint8_t> buffer(size);
    size_t bytes_read = fread(buffer.data(), 1, size, in_file);
    assert_file_closed(&in_file);;
    in_file = nullptr;
    
    if (bytes_read > 0) {
        aom_codec_decode(&decoder, buffer.data(), bytes_read, nullptr);
    }

    // Try to get preview frame
    aom_codec_iter_t iter = nullptr;
    const aom_image_t *img = aom_codec_get_frame(&decoder, &iter);
    
    // If we got an image, work with metadata
    if (img) {
        // Create a copy of the image to work with
        aom_image_t *img_copy = aom_img_alloc(nullptr, img->fmt, img->d_w, img->d_h, 16);
        if (img_copy) {
            // Copy image data
            for (int plane = 0; plane < 3; ++plane) {
                if (img->planes[plane]) {
                    int plane_width = aom_img_plane_width(img, plane);
                    int plane_height = aom_img_plane_height(img, plane);
                    size_t plane_size = plane_width * plane_height;
                    
                    if (plane_size > 0 && img_copy->planes[plane]) {
                        size_t copy_size = std::min(plane_size, (size_t)256);
                        if (data && copy_size <= size) {
                            // Add metadata using some of the input data
                            uint32_t metadata_type = 1; // Example type
                            size_t metadata_size = std::min((size_t)128, size);
                            aom_metadata_insert_flags_t flags = AOM_MIF_ANY_FRAME;
                            
                            aom_img_add_metadata(img_copy, metadata_type, data, metadata_size, flags);
                            
                            // Try to get the metadata back
                            size_t num_metadata = aom_img_num_metadata(img_copy);
                            if (num_metadata > 0) {
                                const aom_metadata_t *metadata = aom_img_get_metadata(img_copy, 0);
                                // Use metadata to avoid unused variable warning
                                (void)metadata;
                            }
                        }
                    }
                }
            }
            
            // Now initialize an encoder with the image
            aom_codec_ctx_t encoder;
            aom_codec_enc_cfg_t enc_cfg;
            
            res = aom_codec_enc_config_default(aom_codec_av1_cx(), &enc_cfg, 0);
            if (res == AOM_CODEC_OK) {
                // Set some basic encoder configuration
                enc_cfg.g_w = img_copy->d_w;
                enc_cfg.g_h = img_copy->d_h;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.g_threads = 1;
                enc_cfg.rc_target_bitrate = 1000;
                
                res = aom_codec_enc_init(&encoder, aom_codec_av1_cx(), &enc_cfg, 0);
                if (res == AOM_CODEC_OK) {
                    // Set encoder configuration (this is a no-op in this context but demonstrates API usage)
                    aom_codec_enc_config_set(&encoder, &enc_cfg);
                    
                    // Create output buffer for encoded data
                    aom_fixed_buf_t output_buf;
                    output_buf.buf = malloc(4096);
                    output_buf.sz = 4096;
                    
                    if (output_buf.buf) {
                        // Set the output buffer
                        aom_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
                        
                        // Try to encode the image
                        aom_codec_encode(&encoder, img_copy, 0, 1, 0);
                        
                        free(output_buf.buf);
                    }
                    
                    aom_codec_destroy(&encoder);
                }
            }
            
            aom_img_free(img_copy);
        }
        
        // Continue iterating through any remaining frames
        while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
            // Just consume remaining frames
        }
    }

    // Clean up
    aom_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}