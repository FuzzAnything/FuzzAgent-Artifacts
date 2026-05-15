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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 3
//<Quality> {"density":18,"unique_branches":{"ctrl_get_frame_corrupted":[[1258,9,1258,26,0,0,4,0],[1263,11,1263,33,0,0,4,1],[1265,11,1265,39,0,0,4,1]]},"library_calls":["aom_codec_version_extra_str","aom_codec_peek_stream_info","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_get_stream_info","aom_codec_decode","aom_codec_control","aom_img_alloc","aom_img_set_rect","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_version_extra_str","aom_codec_peek_stream_info","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_get_stream_info","aom_codec_decode","aom_codec_control","aom_img_alloc","aom_img_set_rect","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


// Include necessary headers for libaom

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Get version extra string
    const char *version_extra = aom_codec_version_extra_str();
    (void)version_extra; // Prevent unused variable warning

    // Create a file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor
    int in_fd = fuzz_fileno(in_file);
    (void)in_fd; // Prevent unused variable warning

    // 1. Peek stream info from input data
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_peek_stream_info(aom_codec_av1_dx(), data, size, &si);

    // 2. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, aom_codec_av1_dx(), &dec_cfg, 
                                                  0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 3. Get stream info using decoder context
    res = aom_codec_get_stream_info(&decoder, &si);
    (void)res; // Prevent unused variable warning

    // 4. Decode the input data
    res = aom_codec_decode(&decoder, data, size, NULL);
    
    // 5. Try to use codec control (example: get frame corruption status)
    int corrupted = 0;
    aom_codec_control(&decoder, AOMD_GET_FRAME_CORRUPTED, &corrupted);

    // 6. Allocate an image for potential encoding
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 
                                      si.w > 0 ? si.w : 64, 
                                      si.h > 0 ? si.h : 64, 
                                      16);
    if (img) {
        // 7. Set a rectangle on the image
        unsigned int rect_w = img->d_w > 8 ? img->d_w / 2 : 4;
        unsigned int rect_h = img->d_h > 8 ? img->d_h / 2 : 4;
        aom_img_set_rect(img, 0, 0, rect_w, rect_h, 0);

        // 8. Get plane height
        int plane_height = aom_img_plane_height(img, 0);
        (void)plane_height; // Prevent unused variable warning

        // 9. Initialize encoder with default config
        aom_codec_ctx_t encoder;
        aom_codec_enc_cfg_t enc_cfg;
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        
        // Get default encoder configuration
        aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        
        // Set some basic parameters
        enc_cfg.g_w = img->d_w;
        enc_cfg.g_h = img->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_error_resilient = 0;
        enc_cfg.g_pass = AOM_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        enc_cfg.kf_mode = AOM_KF_AUTO;
        
        // Initialize encoder
        res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                      0, AOM_ENCODER_ABI_VERSION);
        
        if (res == AOM_CODEC_OK) {
            // Try to encode the image
            res = aom_codec_encode(&encoder, img, 0, 1, 0);
            
            // Get encoded data
            aom_codec_iter_t iter = NULL;
            const aom_codec_cx_pkt_t *pkt = NULL;
            while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // We got an encoded frame
                    break;
                }
            }
            
            // Clean up encoder
            aom_codec_destroy(&encoder);
        }
        
        // Free the image
        aom_img_free(img);
    }

    // Clean up decoder
    aom_codec_destroy(&decoder);
    
    // Close the file
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}