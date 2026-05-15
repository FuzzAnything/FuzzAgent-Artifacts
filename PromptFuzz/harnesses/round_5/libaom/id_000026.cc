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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"av1_loop_restoration_corners_in_sb":[[1336,29,1336,44,0,0,4,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_destroy","aom_codec_get_frame","aom_img_alloc_with_border","aom_codec_destroy","aom_img_set_rect","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_free","aom_codec_destroy","aom_codec_enc_init_ver","aom_img_free","aom_codec_destroy","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_set_rect","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for processing

    // Create input file from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 128;
    dec_cfg.h = 128;
    dec_cfg.allow_lowbitdepth = 1;

    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                                 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Decode input data
    res = aom_codec_decode(&decoder, data, size, NULL);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Get decoded frame
    aom_codec_iter_t iter = NULL;
    aom_image_t* decoded_image = aom_codec_get_frame(&decoder, &iter);
    
    // Step 4: Allocate new image with border
    aom_image_t* new_image = aom_img_alloc_with_border(NULL, AOM_IMG_FMT_I420, 
                                                      128, 128, 1, 1, 16);
    if (!new_image) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Set rectangle on the new image
    // Use fuzzer data to determine rectangle parameters
    unsigned int x = data[0] % 64;
    unsigned int y = data[1] % 64;
    unsigned int w = 32 + (data[2] % 32);
    unsigned int h = 32 + (data[3] % 32);
    
    int rect_res = aom_img_set_rect(new_image, x, y, w, h, 8);
    // Note: aom_img_set_rect returns void in actual API, but we call it anyway

    // Step 6: Create metadata
    uint32_t metadata_type = 1;
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_KEY_FRAME;
    
    // Use part of fuzzer data for metadata payload
    size_t metadata_size = size > 200 ? 100 : size / 2;
    aom_metadata_t* metadata = aom_img_metadata_alloc(metadata_type, 
                                                      data + 4, 
                                                      metadata_size, 
                                                      insert_flag);
    
    // Add metadata to image if available
    if (metadata) {
        // Note: aom_img_add_metadata is needed but not in required APIs list
        // We'll just allocate and free it
        aom_img_metadata_free(metadata);
    }

    // Step 7: Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (res != AOM_CODEC_OK) {
        aom_img_free(new_image);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    enc_cfg.g_w = 128;
    enc_cfg.g_h = 128;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    
    res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                 0, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        aom_img_free(new_image);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 8: Encode the image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    res = aom_codec_encode(&encoder, new_image, pts, duration, flags);
    // We don't check result as fuzzing should continue even on encode failure

    // Step 9: Get encoded data
    iter = NULL;
    const aom_codec_cx_pkt_t* pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }

    // Cleanup
    aom_codec_destroy(&encoder);
    aom_img_free(new_image);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}