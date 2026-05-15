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
//<ID> 49
//<Prompt> []
/*<Combination>: [
*/
//<score> 560, nr_unique_branch: 91
//<Quality> {"density":16,"unique_branches":{"copy_unit_info":[[1856,7,1856,46,0,0,4,0]],"wait_for_top_right_sb":[[788,31,788,74,0,0,4,0],[791,7,791,29,0,0,4,1]],"encode_restoration_mode":[[1792,7,1792,26,0,0,4,0]],"av1_prune_ab_partitions":[[1916,34,1916,74,0,0,4,1],[1919,34,1919,74,0,0,4,1],[1997,7,1997,47,0,0,4,1]],"av1_cdef_fb_row":[[450,36,450,62,0,0,4,1],[451,9,451,24,0,0,4,0],[453,11,453,63,0,0,4,1]],"write_sgrproj_filter":[[1898,7,1898,24,0,0,4,0]],"av1_lookahead_push":[[159,7,159,20,0,0,4,0],[160,7,160,66,0,0,4,1]],"loop_restoration_write_sb_coeffs":[[1966,14,1966,43,0,0,4,0],[1972,9,1972,35,0,0,4,0]],"sgrproj_filter_stripe":[[34,23,34,34,1,0,4,0]],"av1_foreach_transformed_block_in_plane":[[34,23,34,34,1,0,4,0],[34,23,34,34,2,0,4,0],[34,23,34,34,3,0,4,0],[34,23,34,34,4,0,4,0]],"av1_alloc_pmc":[[100,7,100,36,0,0,4,1]],"search_filter_level":[[175,13,175,40,0,0,4,1]],"sad32x32":[[23,15,23,21,0,0,4,0],[23,15,23,21,0,0,4,1]],"av1_filter_block_plane_vert":[[34,23,34,34,3,0,4,1],[34,23,34,34,4,0,4,1]],"av1_cdef_search":[[965,11,965,26,0,0,4,1]],"av1_filter_block_plane_horz":[[34,23,34,34,3,0,4,1],[34,23,34,34,4,0,4,1]],"finer_search_wiener":[[1581,13,1581,55,0,0,4,1]],"aom_codec_get_cx_data":[[220,9,220,16,0,0,4,0],[220,20,220,48,0,0,4,0],[221,9,223,41,0,0,4,0],[221,9,223,41,0,0,4,1],[235,9,235,37,0,0,4,0]],"search_wiener":[[1726,7,1726,32,0,0,4,0],[1745,7,1745,32,0,0,4,0],[1746,7,1746,30,0,0,4,0]],"wiener_filter_stripe":[[34,23,34,34,1,0,4,0]],"aom_remove_metadata_from_frame_buffer":[[282,14,282,27,0,0,4,0]],"cdef_prepare_fb":[[175,7,175,22,0,0,4,1],[200,25,200,32,0,0,4,0],[208,25,208,39,0,0,4,0],[225,18,225,25,0,0,4,0],[231,18,231,32,0,0,4,0],[250,7,250,36,0,0,4,1],[254,7,254,37,0,0,4,1],[34,23,34,34,7,0,4,0]],"none_partition_search":[[4376,7,4376,14,0,0,4,1]],"apply_sgr":[[34,23,34,34,1,0,4,0],[34,23,34,34,2,0,4,0]],"av1_cdef_mse_calc_block":[[34,23,34,34,5,0,4,0]],"av1_rd_pick_palette_intra_sbuv":[[818,31,818,53,0,0,4,1]],"fill_borders_for_fbs_on_frame_boundary":[[321,7,321,34,0,0,4,0],[321,38,321,66,0,0,4,1],[331,39,331,65,0,0,4,1],[337,39,337,66,0,0,4,1],[349,36,349,62,0,0,4,1],[353,36,353,63,0,0,4,1],[359,7,359,33,0,0,4,1],[365,7,365,34,0,0,4,1]],"av1_write_metadata_obu":[[4199,7,4200,8,0,0,4,1],[4203,7,4203,56,0,0,4,1]],"aom_img_metadata_array_alloc":[[366,7,366,13,0,0,4,0],[369,9,369,29,0,0,4,1]],"recon_intra":[[902,9,902,19,0,0,4,1]],"av1_write_metadata_array":[[4217,7,4217,11,0,0,4,1],[4222,22,4222,33,0,0,4,0],[4222,22,4222,33,0,0,4,1],[4224,9,4224,25,0,0,4,0],[4224,29,4224,54,0,0,4,0],[4227,12,4227,53,0,0,4,0],[4228,12,4228,57,0,0,4,0],[4233,13,4233,25,0,0,4,1],[4247,13,4247,34,0,0,4,1],[4253,13,4253,35,0,0,4,1],[4257,13,4258,71,0,0,4,0]],"aom_copy_metadata_to_frame_buffer":[[290,7,290,11,0,0,4,1],[290,15,290,19,0,0,4,1],[290,23,290,43,0,0,4,1],[291,7,291,27,0,0,4,1],[294,7,294,21,0,0,4,1],[295,22,295,43,0,0,4,0],[295,22,295,43,0,0,4,1],[299,9,299,49,0,0,4,1]],"write_wiener_filter":[[1851,7,1851,31,0,0,4,0],[1851,7,1851,31,0,0,4,1],[1870,7,1870,31,0,0,4,0],[1870,7,1870,31,0,0,4,1]],"decoder_destroy":[[124,7,124,32,0,0,4,1],[141,7,141,23,0,0,4,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_img_alloc_with_border","aom_codec_destroy","aom_codec_destroy","aom_img_add_metadata","aom_codec_encode","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_get_cx_data","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_decode","aom_codec_get_frame","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc_with_border","aom_img_add_metadata","aom_codec_encode","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_get_cx_data","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_decode","aom_codec_get_frame","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Helper function to read data from input
static size_t  id_000021_read_from_data(const uint8_t*& data, size_t& remaining, void* buf, size_t bytes) {
    size_t to_read = std::min(remaining, bytes);
    if (to_read > 0) {
        memcpy(buf, data, to_read);
        data += to_read;
        remaining -= to_read;
    }
    return to_read;
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    const uint8_t* input_ptr = data;
    size_t remaining = size;
    
    // 1. Create decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    
    // Create a simple decoder config
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    // Initialize decoder
    if (aom_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // 2. Create encoder context
    aom_codec_ctx_t encoder;
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    
    // Create encoder config
    aom_codec_enc_cfg_t enc_cfg;
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_1) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Set basic encoder parameters
    enc_cfg.g_w = 128;
    enc_cfg.g_h = 128;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    
    // Initialize encoder
    if (aom_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // 3. Allocate image with border using aom_img_alloc_with_border
    aom_image_t* img = nullptr;
    img = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, 
                                    fuzz_uint32_t_2, fuzz_uint32_t_3,  // Dimensions
                                    fuzz_uint32_t_4,        // Alignment
                                    fuzz_uint32_t_5,         // Size alignment  
                                    fuzz_uint32_t_6);       // Border
    
    if (!img) {
        aom_codec_destroy(&encoder);
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Fill image planes with fuzzer data
    size_t plane_bytes = (128 * 128 * 3) / 2;  // I420: Y + U + V planes
    std::vector<uint8_t> image_data(plane_bytes);
     id_000021_read_from_data(input_ptr, remaining, image_data.data(), 
                  std::min(plane_bytes, remaining));
    
    // Fill Y plane (luma)
    for (int y = 0; y < 128; y++) {
        for (int x = 0; x < 128; x++) {
            size_t idx = y * img->stride[0] + x;
            if (idx < plane_bytes) {
                img->planes[0][idx] = image_data[(y * 128 + x) % image_data.size()];
            }
        }
    }
    
    // Fill U and V planes (chroma)
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            size_t u_idx = y * img->stride[1] + x;
            size_t v_idx = y * img->stride[2] + x;
            if (u_idx < plane_bytes/4 && v_idx < plane_bytes/4) {
                img->planes[1][u_idx] = image_data[(y * 64 + x) % image_data.size()];
                img->planes[2][v_idx] = image_data[(y * 64 + x + 4096) % image_data.size()];
            }
        }
    }
    
    // 4. Add metadata to image using aom_img_add_metadata
    uint32_t metadata_type = 1;  // Arbitrary type
    std::vector<uint8_t> metadata_data(50);
     id_000021_read_from_data(input_ptr, remaining, metadata_data.data(), 
                  std::min((size_t)50, remaining));
    
    aom_img_add_metadata(img, fuzz_uint32_t_7, metadata_data.data(), 
                        metadata_data.size(), AOM_MIF_KEY_FRAME);
    
    // 5. Encode the image
    if (aom_codec_encode(&encoder, img, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10) != AOM_CODEC_OK) {
        // Cleanup and continue - encoding might fail with fuzzed data
    }
    
    // 6. Get global headers using aom_codec_get_global_headers
    aom_fixed_buf_t* global_headers = aom_codec_get_global_headers(&encoder);
    if (global_headers) {
        // Can use global_headers->buf and global_headers->sz if needed
        free(global_headers->buf);
        free(global_headers);
    }
    
    // 7. Set up compressed data buffer using aom_codec_set_cx_data_buf
    size_t cx_buffer_size = 10000;
    std::vector<uint8_t> cx_buffer(cx_buffer_size);
    aom_fixed_buf_t cx_buf;
    cx_buf.buf = cx_buffer.data();
    cx_buf.sz = cx_buffer_size;
    
    // Set the buffer (pad values from fuzzer data)
    uint32_t pad_before = 0, pad_after = 0;
    if (remaining >= 8) {
        memcpy(&pad_before, input_ptr, 4);
        memcpy(&pad_after, input_ptr + 4, 4);
        input_ptr += 8;
        remaining -= 8;
    }
    
    aom_codec_set_cx_data_buf(&encoder, &cx_buf, pad_before % 100, pad_after % 100);
    
    // 8. Get compressed data using aom_codec_get_cx_data
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t* pkt = nullptr;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        // Process packet if needed
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Can access pkt->data.frame.buf and pkt->data.frame.sz
        }
    }
    
    // 9. Get metadata from image and free it using aom_img_metadata_free
    size_t num_metadata = aom_img_num_metadata(img);
    for (size_t i = 0; i < num_metadata; i++) {
        const aom_metadata_t* meta = aom_img_get_metadata(img, fuzz_uint64_t_11);
        if (meta) {
            // Create a copy to free
            aom_metadata_t* meta_copy = aom_img_metadata_alloc(
                meta->type, meta->payload, meta->sz, meta->insert_flag);
            if (meta_copy) {
                aom_img_metadata_free(meta_copy);
            }
        }
    }
    
    // 10. Try to decode some fuzzer data (if any remaining)
    if (remaining > 0) {
        aom_codec_decode(&decoder, input_ptr, remaining, nullptr);
        
        // Try to get a decoded frame
        aom_codec_iter_t dec_iter = nullptr;
        aom_image_t* decoded_img = nullptr;
        while ((decoded_img = aom_codec_get_frame(&decoder, &dec_iter)) != nullptr) {
            // Frame available
        }
    }
    
    // 11. Cleanup
    aom_img_free(img);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    
    return 0;
}