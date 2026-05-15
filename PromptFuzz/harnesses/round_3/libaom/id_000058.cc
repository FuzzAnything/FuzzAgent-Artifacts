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
//<ID> 168
//<Prompt> []
/*<Combination>: [
*/
//<score> 133, nr_unique_branch: 52
//<Quality> {"density":19,"unique_branches":{"av1_set_speed_features_qindex_dependent":[[2933,16,2933,25,0,0,4,1],[34,23,34,34,6,0,4,0]],"aom_sum_sse_2d_i16_avx2":[[54,19,54,41,4,0,4,1],[54,36,54,37,4,0,4,1]],"populate_unified_cost_update_freq":[[34,23,34,34,2,0,4,0],[34,23,34,34,3,0,4,0]],"aom_sum_sse_2d_i16_c":[[84,15,84,25,0,0,4,0],[84,15,84,25,0,0,4,1],[85,17,85,26,0,0,4,0],[85,17,85,26,0,0,4,1]],"sub_avg_block_avx2":[[533,9,533,60,0,0,4,0]],"av1_rd_pick_intra_sby_mode":[[1592,9,1593,29,0,0,4,0]],"search_tx_type":[[2119,58,2119,67,0,0,4,1]],"av1_set_cost_upd_freq":[[1638,5,1638,33,0,0,4,0],[1639,5,1639,30,0,0,4,1],[1653,5,1653,33,0,0,4,0],[1654,5,1654,30,0,0,4,1]],"encode_frame_internal":[[2458,8,2459,17,0,0,4,0],[2460,8,2460,74,0,0,4,0],[2463,17,2463,33,0,0,4,0],[2463,17,2463,33,0,0,4,1],[2468,19,2468,31,0,0,4,0],[2468,19,2468,31,0,0,4,1],[2471,30,2471,36,0,0,4,0],[2471,30,2471,36,0,0,4,1],[2474,13,2474,16,0,0,4,0],[2474,13,2474,16,0,0,4,1],[2476,20,2476,21,0,0,4,0],[2476,20,2476,21,0,0,4,1],[2502,13,2502,77,0,0,4,1],[2507,13,2507,37,0,0,4,0],[2511,15,2511,21,0,0,4,0],[2511,15,2511,21,0,0,4,1]],"av1_rd_pick_palette_intra_sbuv":[[903,11,903,65,0,0,4,0],[909,13,909,34,0,0,4,0],[909,13,909,34,0,0,4,1],[911,13,911,47,0,0,4,1]],"skip_cost_update":[[1560,7,1560,40,0,0,4,1],[1561,7,1561,41,0,0,4,1],[1564,7,1564,40,0,0,4,1],[1566,7,1566,47,0,0,4,1]],"compute_stats_win5_avx2":[[622,7,622,59,3,0,4,0],[622,7,622,59,7,0,4,0],[622,7,622,59,12,0,4,0],[622,7,622,59,17,0,4,0],[622,7,622,59,22,0,4,0],[622,7,622,59,27,0,4,0],[622,7,622,59,35,0,4,0],[622,7,622,59,41,0,4,0],[622,7,622,59,47,0,4,0],[622,7,622,59,53,0,4,0]],"get_cdef_filter_strengths":[[59,5,59,31,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_destroy","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_get_preview_frame","aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_remove_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_img_wrap","aom_img_free","aom_codec_error_detail","aom_codec_destroy","aom_codec_error_detail","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_get_preview_frame","aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_remove_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_img_wrap","aom_img_free","aom_codec_error_detail","aom_codec_destroy","aom_codec_error_detail","aom_codec_destroy"],"visited":1}
/**/


// Include necessary headers
extern "C" {
}

// Frame buffer callback structure
struct FrameBufferData {
    std::vector<uint8_t> data;
    size_t size;
};

// Frame buffer get callback
int  id_000058_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    FrameBufferData *fb_data = static_cast<FrameBufferData*>(cb_priv);
    fb_data->data.resize(min_size);
    fb_data->size = min_size;
    fb->data = fb_data->data.data();
    fb->size = min_size;
    return 0;
}

// Frame buffer release callback  
int  id_000058_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    // Nothing to do - memory is managed by FrameBufferData
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create FILE* from input data
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
    
    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Get decoder capabilities
    aom_codec_caps_t dec_caps = aom_codec_get_caps(decoder_iface);
    
    // Initialize decoder configuration
    aom_codec_dec_cfg_t dec_cfg;
    dec_cfg.threads = 1;
    dec_cfg.w = 0;
    dec_cfg.h = 0;
    dec_cfg.allow_lowbitdepth = 1;
    
    // Initialize decoder
    aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (dec_init_result != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        aom_codec_destroy(&decoder);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set frame buffer functions for decoder
    FrameBufferData fb_data;
    aom_codec_set_frame_buffer_functions(
        &decoder,  id_000058_get_frame_buffer,  id_000058_release_frame_buffer, &fb_data);
    
    // Read input data for decoding
    std::vector<uint8_t> input_buffer(size);
    fread(input_buffer.data(), 1, size, in_file);
    
    // Decode the input
    aom_codec_decode(&decoder, input_buffer.data(), size, nullptr);
    
    // Get stream info
    aom_codec_stream_info_t stream_info;
    aom_codec_get_stream_info(&decoder, &stream_info);
    
    // Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_image = nullptr;
    std::vector<aom_image_t*> decoded_images;
    
    while ((decoded_image = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        decoded_images.push_back(decoded_image);
    }
    
    // Try to get preview frame (may be nullptr)
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
    
    // Initialize encoder if we have decoded images
    if (!decoded_images.empty()) {
        aom_codec_ctx_t encoder;
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        
        // Get encoder capabilities
        aom_codec_caps_t enc_caps = aom_codec_get_caps(encoder_iface);
        
        // Initialize encoder configuration
        aom_codec_enc_cfg_t enc_cfg;
        aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        
        // Set encoder configuration based on decoded image
        aom_image_t *first_image = decoded_images[0];
        enc_cfg.g_w = first_image->d_w;
        enc_cfg.g_h = first_image->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_pass = AOM_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        
        // Initialize encoder
        aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(
            &encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
        
        if (enc_init_result == AOM_CODEC_OK) {
            // Set encoder option
            aom_codec_set_option(&encoder, "cpu-used", "8");
            
            // Allocate image for encoding
            aom_image_t *img_to_encode = aom_img_alloc(
                nullptr, first_image->fmt, first_image->d_w, first_image->d_h, 16);
            
            if (img_to_encode) {
                // Copy first decoded image to image for encoding
                for (int plane = 0; plane < 3; plane++) {
                    uint8_t *src = first_image->planes[plane];
                    uint8_t *dst = img_to_encode->planes[plane];
                    int src_stride = first_image->stride[plane];
                    int dst_stride = img_to_encode->stride[plane];
                    int height = aom_img_plane_height(first_image, plane);
                    int width = aom_img_plane_width(first_image, plane);
                    
                    for (int y = 0; y < height; y++) {
                        memcpy(dst, src, width);
                        src += src_stride;
                        dst += dst_stride;
                    }
                }
                
                // Add metadata to image
                uint32_t metadata_type = 1;
                const char *metadata_str = "test_metadata";
                aom_img_add_metadata(img_to_encode, metadata_type, 
                    (const uint8_t*)metadata_str, strlen(metadata_str), 
                    AOM_MIF_KEY_FRAME);
                
                // Get number of metadata
                size_t num_metadata = aom_img_num_metadata(img_to_encode);
                
                // Get metadata if exists
                if (num_metadata > 0) {
                    const aom_metadata_t *metadata = aom_img_get_metadata(img_to_encode, 0);
                    // Allocate and free metadata to use the API
                    aom_metadata_t *allocated_metadata = aom_img_metadata_alloc(
                        metadata_type, (const uint8_t*)metadata_str, 
                        strlen(metadata_str), AOM_MIF_KEY_FRAME);
                    if (allocated_metadata) {
                        aom_img_metadata_free(allocated_metadata);
                    }
                }
                
                // Remove metadata
                aom_img_remove_metadata(img_to_encode);
                
                // Encode the image
                aom_codec_encode(&encoder, img_to_encode, 0, 1, 0);
                
                // Get encoded data
                aom_codec_iter_t enc_iter = nullptr;
                const aom_codec_cx_pkt_t *pkt;
                
                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                        // Write to output file
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                // Free the allocated image
                aom_img_free(img_to_encode);
            }
            
            // Test aom_img_wrap with dummy data
            std::vector<unsigned char> dummy_data(1920 * 1080 * 3 / 2);
            aom_image_t *wrapped_img = aom_img_wrap(
                nullptr, AOM_IMG_FMT_I420, 640, 480, 1, dummy_data.data());
            if (wrapped_img) {
                aom_img_free(wrapped_img);
            }
            
            // Get error detail (may be nullptr)
            const char *enc_error_detail = aom_codec_error_detail(&encoder);
            
            // Destroy encoder
            aom_codec_destroy(&encoder);
        }
    }
    
    // Get decoder error detail
    const char *dec_error_detail = aom_codec_error_detail(&decoder);
    
    // Cleanup
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}