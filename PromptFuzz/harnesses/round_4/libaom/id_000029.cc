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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 76, nr_unique_branch: 3
//<Quality> {"density":19,"unique_branches":{"init_seq_coding_tools":[[625,7,625,34,0,0,4,1]],"decode_partition":[[1421,22,1421,58,0,0,4,0]],"av1_copy_and_extend_frame":[[149,7,149,23,0,0,4,1]]},"library_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_control","aom_codec_control","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_img_free","aom_codec_error","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_control","aom_codec_control","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_img_free","aom_codec_error","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<79) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_13, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. First try to decode ULEB value from the beginning of input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (size > 0) {
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    }

    // 2. Initialize decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
        &decoder_ctx, decoder_iface, &dec_cfg, fuzz_int64_t_1, AOM_DECODER_ABI_VERSION);
    
    if (dec_init_result != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Peek stream info from input data
    aom_codec_stream_info_t stream_info = {0};
    aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);

    // 4. Try to decode the input data
    aom_codec_decode(&decoder_ctx, data, size, NULL);

    // 5. Get decoded frames if available
    aom_codec_iter_t iter = NULL;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder_ctx, &iter);
    
    // 6. Initialize encoder if we got a decoded frame
    if (decoded_img) {
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        aom_codec_ctx_t encoder_ctx;
        aom_codec_enc_cfg_t enc_cfg;
        
        // Get default encoder configuration
        aom_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_2);
        
        // Set basic encoder parameters
        enc_cfg.g_w = decoded_img->d_w;
        enc_cfg.g_h = decoded_img->d_h;
        enc_cfg.g_threads = 1;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_error_resilient = 0;
        
        // Initialize encoder
        aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(
            &encoder_ctx, encoder_iface, &enc_cfg, fuzz_int64_t_3, AOM_ENCODER_ABI_VERSION);
        
        if (enc_init_result == AOM_CODEC_OK) {
            // Set some encoder options
            aom_codec_set_option(&encoder_ctx, fuzz_str_4, fuzz_str_5);
            
            // Set encoder controls
            aom_codec_control(&encoder_ctx, AOME_SET_CPUUSED, 1);
            aom_codec_control(&encoder_ctx, AOME_SET_ENABLEAUTOALTREF, 0);
            
            // Create output file for encoded data
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                // Encode the decoded frame
                aom_codec_encode(&encoder_ctx, decoded_img, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8);
                
                // Get encoded data
                aom_codec_iter_t enc_iter = NULL;
                const aom_codec_cx_pkt_t *pkt;
                while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != NULL) {
                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                assert_file_closed(&out_file);;
            }
            
            // Get error information if any
            const char *enc_error = aom_codec_error(&encoder_ctx);
            
            // Destroy encoder
            aom_codec_destroy(&encoder_ctx);
        }
        
        // Get image plane dimensions
        for (int plane = 0; plane < 3; plane++) {
            int plane_width = aom_img_plane_width(decoded_img, fuzz_int32_t_9);
            int plane_height = aom_img_plane_height(decoded_img, fuzz_int32_t_10);
        }
    }

    // 7. Allocate a test image
    aom_image_t test_img;
    aom_img_alloc(&test_img, AOM_IMG_FMT_I420, fuzz_uint32_t_11, fuzz_uint32_t_12, fuzz_uint32_t_13);
    
    // 8. Free allocated image
    aom_img_free(&test_img);

    // 9. Get decoder error information
    const char *dec_error = aom_codec_error(&decoder_ctx);
    
    // 10. Destroy decoder
    aom_codec_destroy(&decoder_ctx);

    // Clean up
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}