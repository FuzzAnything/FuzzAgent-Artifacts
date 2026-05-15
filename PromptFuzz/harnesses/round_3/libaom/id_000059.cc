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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 110, nr_unique_branch: 11
//<Quality> {"density":22,"unique_branches":{"av1_receive_compressed_data":[[511,7,511,31,0,0,4,1]],"read_uncompressed_header":[[4652,11,4652,41,0,0,4,0],[4660,11,4660,35,0,0,4,1],[4664,11,4664,35,0,0,4,1],[4841,11,4841,53,0,0,4,0],[4865,13,4865,24,0,0,4,1],[4865,28,4865,57,0,0,4,0],[4866,15,4866,26,0,0,4,0],[5169,44,5169,75,0,0,4,1]],"alloc_read_metadata":[[601,7,601,21,0,0,4,1]],"av1_calculate_ref_frame_side":[[1008,9,1008,75,0,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_set_frame_buffer_functions","aom_img_alloc","aom_codec_destroy","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_encode","aom_codec_error_detail","aom_codec_get_cx_data","aom_codec_get_preview_frame","aom_img_remove_metadata","aom_codec_decode","aom_codec_error_detail","aom_codec_get_frame","aom_codec_get_stream_info","aom_img_wrap","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_encode","aom_codec_error_detail","aom_codec_get_cx_data","aom_codec_get_preview_frame","aom_img_remove_metadata","aom_codec_decode","aom_codec_error_detail","aom_codec_get_frame","aom_codec_get_stream_info","aom_img_wrap","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000059_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    uint8_t *buf = (uint8_t *)malloc(min_size);
    if (!buf) return -1;
    
    fb->data = buf;
    fb->size = min_size;
    fb->priv = buf;
    return 0;
}

static int  id_000059_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    if (fb->priv) {
        free(fb->priv);
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
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
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used
    
    // Initialize variables
    aom_codec_ctx_t encoder, decoder;
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_dec_cfg_t dec_cfg = {0};
    aom_image_t *img = nullptr;
    aom_codec_iter_t iter = nullptr;
    aom_codec_stream_info_t stream_info = {0};
    const aom_codec_cx_pkt_t *pkt = nullptr;
    const aom_image_t *preview_frame = nullptr;
    
    // Get codec interfaces
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Get codec capabilities
    aom_codec_caps_t enc_caps = aom_codec_get_caps(encoder_iface);
    aom_codec_caps_t dec_caps = aom_codec_get_caps(decoder_iface);
    (void)enc_caps;
    (void)dec_caps;
    
    // Initialize encoder with default config
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set encoder dimensions
    enc_cfg.g_w = 64;
    enc_cfg.g_h = 64;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set encoder option
    aom_codec_set_option(&encoder, "cpu-used", "1");
    
    // Initialize decoder
    dec_cfg.threads = 1;
    dec_cfg.w = 64;
    dec_cfg.h = 64;
    dec_cfg.allow_lowbitdepth = 1;
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set frame buffer functions for decoder
    aom_codec_set_frame_buffer_functions(&decoder,  id_000059_get_frame_buffer,  id_000059_release_frame_buffer, nullptr);
    
    // Allocate image for encoding
    img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 64, 64, 1);
    if (!img) {
        aom_codec_destroy(&decoder);
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Fill image with some data from input
    size_t data_offset = 0;
    for (int plane = 0; plane < 3; plane++) {
        int plane_width = aom_img_plane_width(img, plane);
        int plane_height = aom_img_plane_height(img, plane);
        for (int y = 0; y < plane_height; y++) {
            size_t bytes_to_copy = plane_width;
            if (data_offset + bytes_to_copy > size) bytes_to_copy = size - data_offset;
            if (bytes_to_copy > 0) {
                memcpy(img->planes[plane] + y * img->stride[plane], 
                       data + data_offset, bytes_to_copy);
                data_offset += bytes_to_copy;
            }
        }
    }
    
    // Create metadata
    uint8_t metadata_data[16] = {0};
    size_t metadata_size = sizeof(metadata_data);
    if (size - data_offset >= metadata_size) {
        memcpy(metadata_data, data + data_offset, metadata_size);
        data_offset += metadata_size;
    }
    
    // Add metadata to image
    aom_img_add_metadata(img, 1, metadata_data, metadata_size, AOM_MIF_KEY_FRAME);
    
    // Get metadata count
    size_t num_metadata = aom_img_num_metadata(img);
    
    // Get and free a metadata entry if exists
    if (num_metadata > 0) {
        const aom_metadata_t *metadata = aom_img_get_metadata(img, 0);
        // Allocate a new metadata and free it
        aom_metadata_t *new_metadata = aom_img_metadata_alloc(2, metadata_data, 8, AOM_MIF_NON_KEY_FRAME);
        if (new_metadata) {
            aom_img_metadata_free(new_metadata);
        }
    }
    
    // Encode the image
    if (aom_codec_encode(&encoder, img, 0, 1, 0) != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&encoder);
        (void)error_detail;
    }
    
    // Get encoded data
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Get preview frame
    preview_frame = aom_codec_get_preview_frame(&encoder);
    (void)preview_frame;
    
    // Remove metadata from image
    aom_img_remove_metadata(img);
    
    // Reset data offset for decoding
    data_offset = 0;
    
    // Decode in chunks
    const size_t chunk_size = 1024;
    while (data_offset < size && data_offset + chunk_size <= size) {
        if (aom_codec_decode(&decoder, data + data_offset, chunk_size, nullptr) != AOM_CODEC_OK) {
            const char *error_detail = aom_codec_error_detail(&decoder);
            (void)error_detail;
        }
        data_offset += chunk_size;
        
        // Get decoded frames
        aom_codec_iter_t dec_iter = nullptr;
        aom_image_t *decoded_img;
        while ((decoded_img = aom_codec_get_frame(&decoder, &dec_iter)) != nullptr) {
            // Get stream info
            aom_codec_get_stream_info(&decoder, &stream_info);
            (void)decoded_img;
        }
    }
    
    // Create a wrapped image
    uint8_t wrapped_data[64 * 64 * 3] = {0};
    aom_image_t wrapped_img;
    aom_img_wrap(&wrapped_img, AOM_IMG_FMT_I420, 64, 64, 1, wrapped_data);
    
    // Cleanup
    aom_img_free(&wrapped_img);
    aom_img_free(img);
    aom_codec_destroy(&decoder);
    aom_codec_destroy(&encoder);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}