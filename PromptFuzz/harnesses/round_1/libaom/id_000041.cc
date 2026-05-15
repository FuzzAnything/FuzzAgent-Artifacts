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
//<ID> 87
//<Prompt> []
/*<Combination>: [
*/
//<score> 189, nr_unique_branch: 79
//<Quality> {"density":21,"unique_branches":{"pixel_dist_visible_only":[[976,35,976,59,0,0,4,1],[983,7,983,25,0,0,4,1]],"compute_stats_win5_avx2":[[798,11,798,26,0,0,4,0],[854,11,854,26,0,0,4,0],[898,11,898,26,0,0,4,0],[942,11,942,26,0,0,4,0],[984,11,984,26,0,0,4,0],[1035,13,1035,28,0,0,4,0],[599,19,599,33,5,0,4,0],[599,19,599,33,5,0,4,1],[591,19,591,35,9,0,4,0],[591,19,591,35,9,0,4,1],[655,11,655,26,10,0,4,0],[591,19,591,35,14,0,4,0],[591,19,591,35,14,0,4,1],[655,11,655,26,15,0,4,0],[591,19,591,35,19,0,4,0],[591,19,591,35,19,0,4,1],[655,11,655,26,20,0,4,0],[591,19,591,35,24,0,4,0],[591,19,591,35,24,0,4,1],[655,11,655,26,25,0,4,0],[591,19,591,35,29,0,4,0],[591,19,591,35,29,0,4,1],[591,19,591,35,37,0,4,0],[591,19,591,35,37,0,4,1],[591,19,591,35,43,0,4,0],[591,19,591,35,43,0,4,1],[591,19,591,35,49,0,4,0],[591,19,591,35,49,0,4,1],[591,19,591,35,55,0,4,0],[591,19,591,35,55,0,4,1]],"av1_calc_proj_params_c":[[639,7,639,25,0,0,4,0],[639,7,639,25,0,0,4,1],[639,29,639,47,0,0,4,0],[639,29,639,47,0,0,4,1],[642,14,642,30,0,0,4,0],[642,14,642,30,0,0,4,1],[645,14,645,30,0,0,4,0]],"av1_lowbd_pixel_proj_error_avx2":[[1590,19,1590,28,0,0,4,0],[1635,19,1635,28,0,0,4,0]],"variance":[[45,15,45,20,0,0,4,0],[45,15,45,20,0,0,4,1],[46,17,46,22,0,0,4,0],[46,17,46,22,0,0,4,1]],"av1_update_state":[[34,23,34,34,4,0,4,0]],"rc_pick_q_and_bounds_no_stats":[[34,23,34,34,1,0,4,0],[34,23,34,34,2,0,4,0]],"calc_proj_params_r0_c":[[545,19,545,29,0,0,4,0],[545,19,545,29,0,0,4,1],[546,21,546,30,0,0,4,0],[546,21,546,30,0,0,4,1]],"get_sse":[[82,7,82,13,0,0,4,0]],"get_proj_subspace":[[687,9,687,27,0,0,4,1]],"aom_sse_avx2":[[110,5,110,11,0,0,4,0],[116,16,116,26,0,0,4,1],[119,5,119,11,0,0,4,0],[125,16,125,26,0,0,4,0],[125,16,125,26,0,0,4,1]],"calc_proj_params_r1_c":[[590,19,590,29,0,0,4,0],[590,19,590,29,0,0,4,1],[591,21,591,30,0,0,4,0],[591,21,591,30,0,0,4,1]],"rectangular_partition_search":[[3630,11,3630,37,0,0,4,1]],"calc_dgd_buf_avg_avx2":[[504,9,504,24,0,0,4,0]],"calc_proj_params_r0_r1_c":[[484,19,484,29,0,0,4,0],[484,19,484,29,0,0,4,1],[485,21,485,30,0,0,4,0],[485,21,485,30,0,0,4,1]],"av1_set_speed_features_qindex_dependent":[[34,23,34,34,1,0,4,0],[34,23,34,34,2,0,4,0],[34,23,34,34,3,0,4,0],[34,23,34,34,4,0,4,0],[34,23,34,34,5,0,4,0]],"get_error_calc_width_in_filt_units":[[383,7,383,26,0,0,4,1],[383,30,383,62,0,0,4,1],[390,7,390,26,0,0,4,0],[390,7,390,26,0,0,4,1],[390,30,390,62,0,0,4,1]]},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_free","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_free","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Get codec version string
    const char *version_str = aom_codec_version_str();
    (void)version_str; // Suppress unused warning

    // 2. Initialize decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 128;
    dec_cfg.h = 128;
    dec_cfg.allow_lowbitdepth = 1;

    aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
        &decoder_ctx, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (dec_init_result != AOM_CODEC_OK) {
        return 0;
    }

    // 3. Peek stream info
    aom_codec_stream_info_t stream_info = {0};
    aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);

    // 4. Get stream info from decoder context
    aom_codec_get_stream_info(&decoder_ctx, &stream_info);

    // 5. Decode the input data
    aom_codec_decode(&decoder_ctx, data, size, nullptr);

    // 6. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    const aom_image_t *decoded_img = nullptr;
    
    while ((decoded_img = aom_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
        // 7. Allocate a new image for encoding
        aom_image_t *img_to_encode = aom_img_alloc(
            nullptr, 
            AOM_IMG_FMT_I420, 
            decoded_img->d_w, 
            decoded_img->d_h, 
            16);
        
        if (!img_to_encode) {
            continue;
        }

        // 8. Copy plane data from decoded image
        for (int plane = 0; plane < 3; plane++) {
            int width = aom_img_plane_width(img_to_encode, plane);
            int height = aom_img_plane_height(img_to_encode, plane);
            
            for (int y = 0; y < height; y++) {
                memcpy(img_to_encode->planes[plane] + y * img_to_encode->stride[plane],
                       decoded_img->planes[plane] + y * decoded_img->stride[plane],
                       width);
            }
        }

        // 9. Initialize encoder
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        aom_codec_ctx_t encoder_ctx;
        aom_codec_enc_cfg_t enc_cfg;
        
        aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        
        enc_cfg.g_w = decoded_img->d_w;
        enc_cfg.g_h = decoded_img->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_lag_in_frames = 0;
        enc_cfg.g_threads = 1;

        aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(
            &encoder_ctx, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
        
        if (enc_init_result != AOM_CODEC_OK) {
            aom_img_free(img_to_encode);
            continue;
        }

        // 10. Add metadata to image
        uint32_t metadata_type = 1;
        uint8_t metadata_data[] = {0x01, 0x02, 0x03, 0x04};
        size_t metadata_size = sizeof(metadata_data);
        
        if (size > metadata_size) {
            aom_img_add_metadata(img_to_encode, metadata_type, 
                               data, metadata_size, AOM_MIF_ANY_FRAME);
        }

        // 11. Get number of metadata entries
        size_t num_metadata = aom_img_num_metadata(img_to_encode);
        (void)num_metadata; // Suppress unused warning

        // 12. Remove metadata
        aom_img_remove_metadata(img_to_encode);

        // 13. Encode the image
        aom_codec_encode(&encoder_ctx, img_to_encode, 0, 1, 0);

        // 14. Get encoded data
        aom_codec_iter_t enc_iter = nullptr;
        const aom_codec_cx_pkt_t *pkt;
        
        while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
            // Process packets if needed
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Frame packet received
            }
        }

        // 15. Clean up encoder
        aom_codec_destroy(&encoder_ctx);
        
        // 16. Clean up image
        aom_img_free(img_to_encode);
    }

    // 17. Clean up decoder
    aom_codec_destroy(&decoder_ctx);

    return 0;
}