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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 9
//<Quality> {"density":18,"unique_branches":{"aom_sse_avx2":[[173,5,173,13,0,0,4,0],[182,16,182,26,0,0,4,0],[182,16,182,26,0,0,4,1]],"av1_cdef_mse_calc_block":[[549,7,549,34,0,0,4,0],[551,32,551,50,0,0,4,1],[555,32,555,50,0,0,4,0],[34,23,34,34,10,0,4,0]],"av1_cdef_compute_sb_list":[[52,30,52,48,0,0,4,0],[34,23,34,34,3,0,4,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_iface_name","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_error","aom_img_alloc","aom_codec_destroy","aom_img_wrap","aom_img_set_rect","aom_img_num_metadata","aom_img_free","aom_codec_decode","aom_codec_error","aom_codec_enc_config_default","aom_img_free","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_err_to_string","aom_codec_error","aom_img_free","aom_codec_destroy","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_iface_name","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_img_alloc","aom_img_wrap","aom_img_set_rect","aom_img_num_metadata","aom_img_free","aom_codec_decode","aom_codec_error","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing

    // Create input FILE* using fmemopen as specified
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output FILE* for encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Get codec interface name
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    const char *iface_name = aom_codec_iface_name(encoder_iface);
    (void)iface_name; // Use result to prevent unused variable warning

    // 2. Create decoder context for AV1
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                                  0, AOM_DECODER_ABI_VERSION);
    
    // Use error APIs
    if (err != AOM_CODEC_OK) {
        const char *error_str = aom_codec_err_to_string(err);
        const char *error_detail = aom_codec_error(&decoder);
        (void)error_str;
        (void)error_detail;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Allocate image using aom_img_alloc
    const unsigned int width = 320;
    const unsigned int height = 240;
    const unsigned int align = 16;
    aom_image_t *img_allocated = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, align);
    
    if (!img_allocated) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 4. Also wrap some image data using aom_img_wrap
    // Allocate buffer for image data
    size_t img_buf_size = width * height * 3 / 2; // I420 format
    unsigned char *img_data = (unsigned char *)malloc(img_buf_size);
    if (img_data) {
        // Fill with some pattern from input data
        for (size_t i = 0; i < img_buf_size && i < size; i++) {
            img_data[i] = data[i];
        }
        
        aom_image_t *img_wrapped = aom_img_wrap(NULL, AOM_IMG_FMT_I420, width, height, align, img_data);
        if (img_wrapped) {
            // 5. Use aom_img_set_rect to set a sub-rectangle
            unsigned int x = 10, y = 10, w = 100, h = 100;
            int rect_result = aom_img_set_rect(img_wrapped, x, y, w, h, 0);
            (void)rect_result;
            
            // 6. Use aom_img_num_metadata
            size_t metadata_count = aom_img_num_metadata(img_wrapped);
            (void)metadata_count;
            
            // Clean up wrapped image
            aom_img_free(img_wrapped);
        }
        free(img_data);
    }

    // 7. Try to decode input data
    err = aom_codec_decode(&decoder, data, size, NULL);
    if (err != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&decoder);
        (void)error_detail;
    }

    // 8. Create encoder context
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t enc_cfg;
    
    err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err != AOM_CODEC_OK) {
        aom_img_free(img_allocated);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    enc_cfg.g_w = width;
    enc_cfg.g_h = height;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    enc_cfg.rc_end_usage = AOM_Q;
    enc_cfg.rc_min_quantizer = 20;
    enc_cfg.rc_max_quantizer = 50;
    
    err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    
    if (err != AOM_CODEC_OK) {
        const char *error_str = aom_codec_err_to_string(err);
        const char *error_detail = aom_codec_error(&encoder);
        (void)error_str;
        (void)error_detail;
        aom_img_free(img_allocated);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 9. Encode the allocated image
    err = aom_codec_encode(&encoder, img_allocated, 0, 1, 0);
    if (err != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&encoder);
        (void)error_detail;
    }

    // 10. Use aom_codec_get_cx_data to get encoded packets
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt = NULL;
    
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Write to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }

    // 11. Clean up all resources
    aom_img_free(img_allocated);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}