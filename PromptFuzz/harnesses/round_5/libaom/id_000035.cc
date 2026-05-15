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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 78, nr_unique_branch: 3
//<Quality> {"density":26,"unique_branches":{"at_ctrl_map_end":[[192,10,192,25,0,0,4,0],[192,29,192,42,0,0,4,0]],"aom_codec_control":[[105,8,105,31,0,0,4,1]]},"library_calls":["aom_codec_iface_name","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_error_detail","aom_codec_destroy","aom_codec_decode","aom_codec_error_detail","aom_codec_destroy","aom_codec_get_frame","aom_img_num_metadata","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_error_detail","aom_codec_destroy","aom_codec_set_option","aom_codec_enc_config_set","aom_codec_control","aom_codec_encode","aom_codec_error_detail","aom_codec_get_cx_data","aom_codec_control","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_iface_name","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_set_option","aom_codec_enc_config_set","aom_codec_control","aom_codec_encode","aom_codec_error_detail","aom_codec_control","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a FILE* from the input data
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
    
    aom_codec_ctx_t decoder;
    aom_codec_ctx_t encoder;
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt = NULL;
    aom_codec_err_t res;
    
    // Get decoder interface name
    const char *decoder_name = aom_codec_iface_name(aom_codec_av1_dx());
    const char *encoder_name = aom_codec_iface_name(aom_codec_av1_cx());
    
    // Initialize decoder
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    res = aom_codec_dec_init_ver(&decoder, aom_codec_av1_dx(), &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read and decode the input data
    uint8_t *buffer = (uint8_t *)malloc(size);
    if (!buffer) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    fread(buffer, 1, size, in_file);
    
    // Decode the frame
    res = aom_codec_decode(&decoder, buffer, size, NULL);
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder);
        free(buffer);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get decoded frame
    aom_image_t *img = aom_codec_get_frame(&decoder, &iter);
    
    if (img) {
        // Get number of metadata in the image
        size_t num_metadata = aom_img_num_metadata(img);
        
        // Add metadata to the image
        uint32_t metadata_type = 1;
        uint8_t metadata_data[] = {0x01, 0x02, 0x03};
        size_t metadata_size = sizeof(metadata_data);
        
        // Try to add metadata (may fail if image doesn't support it)
        int add_meta_res = aom_img_add_metadata(img, metadata_type, metadata_data, 
                                               metadata_size, AOM_MIF_ANY_FRAME);
        
        // Initialize encoder
        aom_codec_enc_cfg_t enc_cfg;
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        
        res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        if (res != AOM_CODEC_OK) {
            free(buffer);
            aom_codec_destroy(&decoder);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
        
        // Set encoder configuration
        enc_cfg.g_w = img->d_w;
        enc_cfg.g_h = img->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_pass = AOM_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        
        res = aom_codec_enc_init_ver(&encoder, aom_codec_av1_cx(), &enc_cfg, 0, 
                                     AOM_ENCODER_ABI_VERSION);
        if (res != AOM_CODEC_OK) {
            const char *error_detail = aom_codec_error_detail(&encoder);
            free(buffer);
            aom_codec_destroy(&decoder);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
        
        // Set encoder option
        res = aom_codec_set_option(&encoder, "cpu-used", "1");
        
        // Set encoder configuration (again to demonstrate usage)
        res = aom_codec_enc_config_set(&encoder, &enc_cfg);
        
        // Use codec control for encoder
        // AOME_SET_CPUUSED = 1
        res = aom_codec_control(&encoder, 1, 1);
        
        // Encode the frame
        res = aom_codec_encode(&encoder, img, 0, 1, 0);
        if (res != AOM_CODEC_OK) {
            const char *error_detail = aom_codec_error_detail(&encoder);
        } else {
            // Get encoded data
            iter = NULL;
            while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
        }
        
        // Use codec control for decoder
        // AOMD_GET_LAST_REF_UPDATES = 128
        int last_ref_updates = 0;
        res = aom_codec_control(&decoder, 128, &last_ref_updates);
        
        // Destroy encoder
        aom_codec_destroy(&encoder);
    }
    
    // Clean up
    free(buffer);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}