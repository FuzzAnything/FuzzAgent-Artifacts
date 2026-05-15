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
//<ID> 64
//<Prompt> []
/*<Combination>: [
*/
//<score> 209, nr_unique_branch: 72
//<Quality> {"density":11,"unique_branches":{"cfl_luma_subsampling_422_hbd_ssse3":[[244,9,244,19,0,0,4,1],[250,11,250,21,0,0,4,0],[250,11,250,21,0,0,4,1],[257,13,257,24,0,0,4,1],[267,12,267,32,0,0,4,0],[267,12,267,32,0,0,4,1]],"setup_segmentation":[[1492,7,1492,22,0,0,4,1],[1500,7,1500,57,0,0,4,1],[1507,9,1507,24,0,0,4,1],[1516,7,1516,23,0,0,4,1],[1542,14,1542,28,0,0,4,1]],"highbd_dr_prediction_32bit_z1_16xN_internal_avx2":[[1425,19,1425,24,0,0,4,0],[1425,19,1425,24,0,0,4,1],[1429,9,1429,27,0,0,4,1],[1452,9,1452,17,0,0,4,0]],"av1_calculate_tile_cols":[[35,23,35,34,4,0,4,0]],"decrease_ref_count":[[469,7,469,18,0,0,4,0],[478,9,478,28,0,0,4,0],[478,9,478,28,0,0,4,1],[478,32,478,58,0,0,4,0]],"highbd_dr_prediction_z3_16x16_avx2":[[3166,7,3166,14,0,0,4,1]],"read_uncompressed_header":[[4865,13,4865,24,0,0,4,1],[4865,28,4865,57,0,0,4,0],[4865,28,4865,57,0,0,4,1],[4866,15,4866,26,0,0,4,0],[5037,9,5037,46,0,0,4,1]],"av1_dc_quant_QTX":[[203,5,203,21,0,0,4,0]],"cfl_subsampling_hbd":[[304,7,304,17,0,0,4,0],[305,9,305,19,0,0,4,0],[305,9,305,19,0,0,4,1]],"release_pending_output_frames":[[575,24,575,50,0,0,4,0]],"read_film_grain_params":[[4067,8,4067,40,0,0,4,0],[4067,44,4067,76,0,0,4,1]],"av1_get_tile_limits":[[35,23,35,34,8,0,4,0]],"cfl_luma_subsampling_420_hbd_ssse3":[[189,9,189,19,0,0,4,0],[189,9,189,19,0,0,4,1],[199,11,199,21,0,0,4,0],[199,11,199,21,0,0,4,1],[208,13,208,24,0,0,4,1],[224,12,224,47,0,0,4,0],[224,12,224,47,0,0,4,1]],"cfl_luma_subsampling_420_hbd_avx2":[[187,12,187,48,0,0,4,0],[187,12,187,48,0,0,4,1]],"highbd_dr_prediction_32bit_z2_HxW_avx2":[[2556,19,2556,24,0,0,4,0],[2556,19,2556,24,0,0,4,1],[2560,21,2560,26,0,0,4,0],[2560,21,2560,26,0,0,4,1],[2567,11,2567,38,0,0,4,0],[2567,11,2567,38,0,0,4,1],[2571,11,2571,29,0,0,4,0],[2571,11,2571,29,0,0,4,1],[2574,13,2574,30,0,0,4,0],[2574,13,2574,30,0,0,4,1],[2577,11,2577,25,0,0,4,0],[2577,11,2577,25,0,0,4,1],[2606,11,2606,42,0,0,4,0],[2606,11,2606,42,0,0,4,1],[2609,11,2609,26,0,0,4,0],[2609,11,2609,26,0,0,4,1],[2646,11,2646,32,0,0,4,0],[2646,11,2646,32,0,0,4,1]],"highbd_dr_prediction_z3_32x32_avx2":[[3186,7,3186,14,0,0,4,1]],"highbd_dr_prediction_32bit_z1_32xN_internal_avx2":[[1579,19,1579,24,0,0,4,0],[1579,19,1579,24,0,0,4,1],[1583,9,1583,27,0,0,4,1],[1594,21,1594,27,0,0,4,0],[1594,21,1594,27,0,0,4,1],[1596,11,1596,20,0,0,4,1],[1614,13,1614,21,0,0,4,0],[1644,11,1644,13,0,0,4,0],[1644,11,1644,13,0,0,4,1]],"av1_highbd_dr_prediction_z2_avx2":[[2900,11,2900,18,0,0,4,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_decode","aom_codec_error_detail","aom_codec_get_frame","aom_img_add_metadata","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_enc_config_set","aom_codec_control","aom_codec_control","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error_detail","aom_codec_get_frame","aom_img_add_metadata","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_enc_config_set","aom_codec_control","aom_codec_control","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    const char *iface_name = aom_codec_iface_name(decoder_iface);
    
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read and decode frames
    std::vector<uint8_t> buffer(4096);
    aom_codec_iter_t iter = nullptr;
    
    while (!feof(in_file)) {
        size_t bytes_read = fread(buffer.data(), 1, buffer.size(), in_file);
        if (bytes_read == 0) break;
        
        res = aom_codec_decode(&decoder, buffer.data(), bytes_read, nullptr);
        if (res != AOM_CODEC_OK) {
            const char *error_detail = aom_codec_error_detail(&decoder);
            // Continue anyway for fuzzing
        }
        
        // Get decoded frames
        aom_image_t *img;
        while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
            // Add metadata to the image
            uint32_t metadata_type = 1; // Example type
            uint8_t metadata_data[] = {0x01, 0x02, 0x03};
            size_t metadata_size = sizeof(metadata_data);
            
            int add_meta_res = aom_img_add_metadata(img, metadata_type, 
                                                   metadata_data, metadata_size,
                                                   AOM_MIF_ANY_FRAME);
            
            if (add_meta_res == 0) {
                // Get number of metadata
                size_t num_metadata = aom_img_num_metadata(img);
                (void)num_metadata; // Use variable to avoid unused warning
            }
            
            // Initialize encoder for re-encoding
            aom_codec_ctx_t encoder;
            aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
            
            aom_codec_enc_cfg_t enc_cfg;
            res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            if (res == AOM_CODEC_OK) {
                // Set encoder configuration
                enc_cfg.g_w = img->d_w;
                enc_cfg.g_h = img->d_h;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.rc_target_bitrate = 1000;
                enc_cfg.g_threads = 1;
                
                res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, 
                                            AOM_ENCODER_ABI_VERSION);
                
                if (res == AOM_CODEC_OK) {
                    // Set encoder options
                    aom_codec_set_option(&encoder, "cpu-used", "5");
                    aom_codec_set_option(&encoder, "end-usage", "vbr");
                    
                    // Set encoder configuration (again to ensure it's applied)
                    aom_codec_enc_config_set(&encoder, &enc_cfg);
                    
                    // Set encoder controls
                    aom_codec_control(&encoder, AOME_SET_CPUUSED, 5);
                    aom_codec_control(&encoder, AOME_SET_ENABLEAUTOALTREF, 0);
                    
                    // Encode the frame
                    res = aom_codec_encode(&encoder, img, 0, 1, 0);
                    
                    if (res == AOM_CODEC_OK) {
                        // Get encoded data
                        aom_codec_iter_t enc_iter = nullptr;
                        const aom_codec_cx_pkt_t *pkt;
                        while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                            // Process encoded packet if needed
                        }
                    }
                    
                    // Clean up encoder
                    aom_codec_destroy(&encoder);
                }
            }
        }
        iter = nullptr;
    }
    
    // Clean up
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}