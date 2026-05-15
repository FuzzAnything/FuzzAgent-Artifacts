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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 81, nr_unique_branch: 8
//<Quality> {"density":27,"unique_branches":{"prune_4_way_partition_search":[[4097,31,4097,75,0,0,4,1],[4119,7,4119,54,0,0,4,1]],"av1_rd_pick_palette_intra_sby":[[34,23,34,34,3,0,4,1]],"clamp_pframe_target_size":[[35,23,35,34,1,0,4,0]],"set_stage2_params":[[35,23,35,34,3,0,4,0]],"av1_prune_ab_partitions":[[1916,34,1916,74,0,0,4,1],[1919,34,1919,74,0,0,4,1],[1997,7,1997,47,0,0,4,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_set_frame_buffer_functions","aom_img_alloc","aom_codec_destroy","aom_codec_destroy","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_codec_decode","aom_codec_get_frame","aom_codec_get_preview_frame","aom_img_wrap","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_img_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_codec_decode","aom_codec_get_frame","aom_codec_get_preview_frame","aom_img_wrap","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Custom frame buffer callbacks structure
struct FrameBufferData {
    uint8_t* data;
    size_t size;
};

// Callback to get frame buffer
int  id_000025_get_frame_buffer(void* cb_priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
    FrameBufferData* fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    if (fb_data->size < min_size) {
        free(fb_data->data);
        fb_data->data = static_cast<uint8_t*>(calloc(1, min_size));
        fb_data->size = min_size;
        if (!fb_data->data) return -1;
    }
    
    fb->data = fb_data->data;
    fb->size = fb_data->size;
    return 0;
}

// Callback to release frame buffer
int  id_000025_release_frame_buffer(void* cb_priv, aom_codec_frame_buffer_t* fb) {
    // We keep the buffer allocated for reuse
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1024) {
	return 0;
	}
	 // Need minimum data
    
    // Open input as FILE* for potential future use
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor
    int fd = fuzz_fileno(in_file);
    (void)fd; // Suppress unused warning
    
    // Initialize codec interfaces
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    
    // Check capabilities
    aom_codec_caps_t enc_caps = aom_codec_get_caps(encoder_iface);
    aom_codec_caps_t dec_caps = aom_codec_get_caps(decoder_iface);
    (void)enc_caps;
    (void)dec_caps;
    
    // Setup encoder configuration
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_err_t res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set some encoder parameters
    enc_cfg.g_w = 64;
    enc_cfg.g_h = 64;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 100;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    
    // Create encoder context
    aom_codec_ctx_t encoder;
    res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set encoder options
    aom_codec_set_option(&encoder, "cpu-used", "1");
    aom_codec_set_option(&encoder, "end-usage", "cbr");
    
    // Create decoder context
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Setup frame buffer callbacks
    FrameBufferData fb_data = {nullptr, 0};
    aom_codec_set_frame_buffer_functions(&decoder,  id_000025_get_frame_buffer, 
                                          id_000025_release_frame_buffer, &fb_data);
    
    // Allocate test image for encoding
    aom_image_t* img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 64, 64, 1);
    if (!img) {
        aom_codec_destroy(&decoder);
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Fill image with test data from input
    size_t bytes_to_copy = (size_t)(img->w * img->h * 1.5);
    if (bytes_to_copy > size) bytes_to_copy = size;
    
    // Fill Y plane
    size_t y_plane_size = img->w * img->h;
    size_t y_bytes = (bytes_to_copy > y_plane_size) ? y_plane_size : bytes_to_copy;
    memcpy(img->planes[0], data, y_bytes);
    
    // Fill U and V planes if we have enough data
    if (bytes_to_copy > y_plane_size) {
        size_t uv_size = img->w * img->h / 4;
        size_t remaining = bytes_to_copy - y_plane_size;
        size_t uv_bytes = (remaining > uv_size * 2) ? uv_size : remaining / 2;
        
        if (uv_bytes > 0) {
            memcpy(img->planes[1], data + y_plane_size, uv_bytes);
            memcpy(img->planes[2], data + y_plane_size + uv_bytes, uv_bytes);
        }
    }
    
    // Add metadata to image
    uint32_t metadata_type = 1; // Example type
    size_t metadata_size = (size > 100) ? 50 : size / 2;
    uint8_t* metadata = static_cast<uint8_t*>(malloc(metadata_size));
    if (metadata) {
        memcpy(metadata, data, metadata_size);
        aom_img_add_metadata(img, metadata_type, metadata, metadata_size, 
                           AOM_MIF_KEY_FRAME);
        free(metadata);
    }
    
    // Get metadata count
    size_t metadata_count = aom_img_num_metadata(img);
    
    // Get and check metadata if present
    if (metadata_count > 0) {
        const aom_metadata_t* retrieved_metadata = aom_img_get_metadata(img, 0);
        (void)retrieved_metadata; // Suppress unused warning
    }
    
    // Remove metadata
    aom_img_remove_metadata(img);
    
    // Encode the image
    res = aom_codec_encode(&encoder, img, 0, 1, 0);
    
    // Get encoded data
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t* pkt = nullptr;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Decode the encoded data
            aom_codec_decode(&decoder, static_cast<const uint8_t*>(pkt->data.frame.buf),
                           pkt->data.frame.sz, nullptr);
            
            // Get decoded frame
            aom_codec_iter_t dec_iter = nullptr;
            aom_image_t* decoded_img = aom_codec_get_frame(&decoder, &dec_iter);
            
            if (decoded_img) {
                // Test preview frame (may return null)
                const aom_image_t* preview = aom_codec_get_preview_frame(&encoder);
                (void)preview; // Suppress unused warning
                
                // Test aom_img_wrap with decoded data
                aom_image_t wrapped_img;
                unsigned char* img_data = static_cast<unsigned char*>(
                    malloc(decoded_img->w * decoded_img->h * 3));
                if (img_data) {
                    aom_image_t* wrapped = aom_img_wrap(&wrapped_img, decoded_img->fmt,
                                                       decoded_img->w, decoded_img->h,
                                                       1, img_data);
                    (void)wrapped; // Suppress unused warning
                    free(img_data);
                }
            }
        }
    }
    
    // Cleanup
    aom_img_free(img);
    aom_codec_destroy(&decoder);
    aom_codec_destroy(&encoder);
    
    // Free frame buffer data
    if (fb_data.data) {
        free(fb_data.data);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}