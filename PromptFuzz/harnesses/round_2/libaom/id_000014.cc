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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 56, nr_unique_branch: 28
//<Quality> {"density":14,"unique_branches":{"update_cdef_row_next_job_info":[[1050,7,1050,29,0,0,4,1]],"get_next_job_info":[[3181,11,3181,41,0,0,4,1]],"av1_cdef_init_fb_row_mt":[[1182,34,1182,60,0,0,4,1],[1183,7,1183,22,0,0,4,0],[1185,9,1185,61,0,0,4,1],[1204,11,1204,26,0,0,4,0],[1209,11,1209,26,0,0,4,0]],"aom_reader_init":[[15,7,15,11,0,0,4,1]],"loop_restoration_read_sb_coeffs":[[1736,14,1736,60,0,0,4,1]],"aom_internal_error":[[167,7,167,19,0,0,4,1]],"read_tile_group_header":[[312,9,312,28,0,0,4,1]],"cdef_row_mt_sync_read":[[179,7,179,11,0,0,4,1],[183,10,183,47,0,0,4,0],[183,10,183,47,0,0,4,1]],"restore_processing_stripe_boundary":[[368,9,368,19,0,0,4,0],[376,9,376,19,0,0,4,0],[381,11,381,65,0,0,4,0],[205,27,205,32,10,0,4,0],[205,27,205,32,12,0,4,0]],"av1_cdef_fb_row":[[441,20,441,59,0,0,4,1]],"setup_processing_stripe_boundary":[[306,9,306,19,0,0,4,0],[319,9,319,19,0,0,4,0],[205,27,205,32,13,0,4,0],[205,27,205,32,14,0,4,0],[205,27,205,32,15,0,4,0],[205,27,205,32,16,0,4,0],[205,27,205,32,17,0,4,0],[205,27,205,32,18,0,4,0]]},"library_calls":["aom_uleb_decode","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_preview_frame","aom_img_alloc","aom_img_plane_height","aom_img_remove_metadata","aom_img_set_rect","aom_codec_control","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_set_option","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_preview_frame","aom_img_alloc","aom_img_plane_height","aom_img_remove_metadata","aom_img_set_rect","aom_codec_control","aom_img_free","aom_codec_destroy"],"visited":2}
/**/


extern "C" {
}

// Frame buffer callbacks for set_frame_buffer_functions
static int  id_000014_get_frame_buffer(void *cb_priv, size_t min_size, 
                           aom_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    fb->priv = nullptr;
    return 0;
}

static int  id_000014_release_frame_buffer(void *cb_priv, 
                               aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful fuzzing

    // Step 1: Decode ULEB value from beginning of input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    size_t uleb_available = size > 50 ? 50 : size; // Use first 50 bytes for ULEB
    int uleb_result = aom_uleb_decode(data, uleb_available, &uleb_value, &uleb_length);
    
    // Step 2: Create decoder context
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 2;
    
    // Initialize decoder with AV1
    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &decoder, aom_codec_av1_dx(), &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (init_result != AOM_CODEC_OK) {
        // Cleanup and exit if initialization fails
        return 0;
    }
    
    // Step 3: Set decoder options using uleb_value as parameter
    char option_value[64];
    snprintf(option_value, sizeof(option_value), "%lu", uleb_value % 10);
    aom_codec_set_option(&decoder, "tune", option_value);
    
    // Step 4: Set frame buffer functions
    aom_codec_set_frame_buffer_functions(
        &decoder,  id_000014_get_frame_buffer,  id_000014_release_frame_buffer, nullptr);
    
    // Step 5: Decode the input data (minus what we used for ULEB)
    size_t decode_size = size > uleb_length ? size - uleb_length : 0;
    const uint8_t *decode_data = data + (uleb_length > 0 ? uleb_length : 0);
    
    aom_codec_decode(&decoder, decode_data, decode_size, nullptr);
    
    // Step 6: Get stream information
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_get_stream_info(&decoder, &stream_info);
    
    // Step 7: Get preview frame
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
    
    // Step 8: Process frame if available
    if (preview_frame) {
        // Create a copy we can modify
        aom_image_t *modifiable_frame = aom_img_alloc(
            nullptr, preview_frame->fmt, 
            preview_frame->d_w, preview_frame->d_h, 1);
        
        if (modifiable_frame) {
            // Copy the data
            for (int plane = 0; plane < 3; plane++) {
                if (modifiable_frame->planes[plane] && preview_frame->planes[plane]) {
                    size_t plane_size = aom_img_plane_height(modifiable_frame, plane) *
                                       modifiable_frame->stride[plane];
                    memcpy(modifiable_frame->planes[plane], 
                          preview_frame->planes[plane], 
                          plane_size);
                }
            }
            
            // Step 9: Remove metadata from the image
            aom_img_remove_metadata(modifiable_frame);
            
            // Step 10: Set a rectangle on the image
            unsigned int rect_w = modifiable_frame->d_w > 100 ? 100 : modifiable_frame->d_w;
            unsigned int rect_h = modifiable_frame->d_h > 100 ? 100 : modifiable_frame->d_h;
            unsigned int border = (uleb_value % 4);
            
            aom_img_set_rect(modifiable_frame, 0, 0, rect_w, rect_h, border);
            
            // Step 11: Use control API
            aom_codec_control(&decoder, AOME_GET_LAST_QUANTIZER, &uleb_value);
            
            // Clean up modifiable frame
            aom_img_free(modifiable_frame);
        }
    }
    
    // Step 12: Cleanup
    aom_codec_destroy(&decoder);
    
    return 0;
}