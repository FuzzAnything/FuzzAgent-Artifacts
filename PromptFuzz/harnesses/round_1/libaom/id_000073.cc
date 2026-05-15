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
//<ID> 157
//<Prompt> []
/*<Combination>: [
*/
//<score> 322, nr_unique_branch: 33
//<Quality> {"density":23,"unique_branches":{"get_mult_shift_ndiag":[[58,4,58,17,1,0,4,0],[58,4,58,17,1,0,4,1]],"build_compound_diffwtd_mask_d16_inv_avx2":[[418,14,418,21,0,0,4,0],[444,14,444,19,0,0,4,0],[444,14,444,19,0,0,4,1]],"av1_find_projection":[[911,7,912,30,0,0,4,1],[916,7,916,39,0,0,4,1]],"allow_warp":[[47,7,47,37,0,0,4,0],[47,41,47,65,0,0,4,0],[48,9,48,34,0,0,4,0]],"build_compound_diffwtd_mask_d16_avx2":[[239,14,239,21,0,0,4,0],[255,14,255,19,0,0,4,0],[255,14,255,19,0,0,4,1]],"find_affine_int":[[835,19,835,25,0,0,4,0],[835,19,835,25,0,0,4,1],[843,9,843,33,0,0,4,0],[843,37,843,61,0,0,4,0],[865,7,865,15,0,0,4,1],[868,60,868,67,0,0,4,1],[870,7,870,16,0,0,4,1]],"get_mult_shift_diag":[[58,4,58,17,1,0,4,1]],"foreach_overlappable_nb_left":[[34,23,34,34,1,0,4,1]],"read_inter_block_mode_info":[[1484,7,1484,41,0,0,4,0],[1490,9,1490,31,0,0,4,1],[1495,9,1497,62,0,0,4,1]],"set_encoder_config":[[34,23,34,34,9,0,4,0]],"resolve_divisor_64":[[174,22,174,31,0,0,4,1],[179,7,179,28,0,0,4,0]],"av1_get_reference_mode_context":[[164,18,164,45,0,0,4,1],[168,18,168,44,0,0,4,1],[142,4,142,33,8,0,4,1],[142,4,142,33,9,0,4,1]],"read_motion_mode":[[238,7,238,46,0,0,4,1]]},"library_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_uleb_encode","aom_codec_set_option","aom_img_alloc","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_uleb_encode","aom_codec_set_option","aom_img_alloc","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy"],"visited":0}
/**/


// libaom headers
extern "C" {
}

// The required fuzz driver function
extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // 1. Get version string (API 1)
    const char *version = aom_codec_version_str();
    (void)version; // Avoid unused variable warning

    // 2. ULEB decode (API 2)
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    int decode_result = aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    (void)decode_result;

    // 3. Create input file from fuzzer data (Instruction 4)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 4. Create output file (Instruction 4)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 5. Get file descriptors (Instruction 5)
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    (void)fd_in;
    (void)fd_out;

    // 6. DECODER SETUP
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx(); // API 4
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    // Initialize decoder (API 5)
    aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (dec_init_result == AOM_CODEC_OK) {
        // 7. Decode the input (API 6)
        aom_codec_decode(&decoder, data, size, NULL);
        
        // 8. Get stream info (API 7)
        aom_codec_stream_info_t stream_info;
        memset(&stream_info, 0, sizeof(stream_info));
        aom_codec_get_stream_info(&decoder, &stream_info);
        
        // 9. Get decoded frames
        aom_codec_iter_t iter = NULL;
        aom_image_t *img = aom_codec_get_frame(&decoder, &iter); // API 8
        
        while (img) {
            // 10. Get image plane dimensions (API 9 & 10)
            for (int plane = 0; plane < 3; plane++) {
                int plane_w = aom_img_plane_width(img, plane);
                int plane_h = aom_img_plane_height(img, plane);
                (void)plane_w;
                (void)plane_h;
            }
            
            // 11. Get metadata count (API 11)
            size_t metadata_count = aom_img_num_metadata(img);
            (void)metadata_count;
            
            // 12. ENCODER SETUP
            aom_codec_ctx_t encoder;
            aom_codec_iface_t *encoder_iface = aom_codec_av1_cx(); // API 12
            
            aom_codec_enc_cfg_t enc_cfg;
            // Get default config
            aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            
            // Set config (API 13)
            enc_cfg.g_w = img->d_w;
            enc_cfg.g_h = img->d_h;
            enc_cfg.g_threads = 1;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_error_resilient = 0;
            
            // Initialize encoder (API 14)
            aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(
                &encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
            
            if (enc_init_result == AOM_CODEC_OK) {
                // 13. ULEB encode (API 15)
                uint8_t coded_buffer[10];
                size_t coded_size = 0;
                int encode_result = aom_uleb_encode(uleb_value, sizeof(coded_buffer), 
                                                   coded_buffer, &coded_size);
                (void)encode_result;
                
                // 14. Set encoder option (API 16)
                aom_codec_set_option(&encoder, "cpu-used", "1");
                
                // 15. Allocate image (API 17)
                aom_image_t *enc_img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 
                                                     img->d_w, img->d_h, 16);
                
                if (enc_img) {
                    // Copy image data (simplified - in real code would need proper conversion)
                    // For fuzzing, we'll just use the decoded image
                    
                    // 16. Set output buffer
                    aom_fixed_buf_t output_buf;
                    uint8_t output_buffer[1024 * 1024]; // 1MB buffer
                    output_buf.buf = output_buffer;
                    output_buf.sz = sizeof(output_buffer);
                    
                    // 17. Set CX data buffer (API 3)
                    aom_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
                    
                    // 18. Encode frame (API 18)
                    aom_codec_encode(&encoder, img, 0, 1, 0);
                    
                    // 19. Get encoded data (API 19)
                    aom_codec_iter_t enc_iter = NULL;
                    const aom_codec_cx_pkt_t *pkt = aom_codec_get_cx_data(&encoder, &enc_iter);
                    
                    while (pkt) {
                        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                            // Write to output file
                            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                        }
                        pkt = aom_codec_get_cx_data(&encoder, &enc_iter);
                    }
                    
                    aom_img_free(enc_img);
                }
                
                // 20. Destroy encoder (API 20)
                aom_codec_destroy(&encoder);
            }
            
            // Get next frame
            img = aom_codec_get_frame(&decoder, &iter);
        }
        
        // Destroy decoder (API 20)
        aom_codec_destroy(&decoder);
    }

    // 21. Clean up files and resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}