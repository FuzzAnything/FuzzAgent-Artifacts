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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_get_frame","aom_img_remove_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error","aom_codec_error","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_get_frame","aom_img_remove_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error","aom_codec_error","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callback functions
static int  id_000028_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    fb->data = static_cast<uint8_t*>(malloc(min_size));
    fb->size = min_size;
    fb->priv = nullptr;
    return fb->data ? 0 : -1;
}

static int  id_000028_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output file for encoded data
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
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION
    );
    
    // Set custom frame buffer functions
    if (dec_init_result == AOM_CODEC_OK) {
        aom_codec_set_frame_buffer_functions(
            &decoder,  id_000028_get_frame_buffer,  id_000028_release_frame_buffer, nullptr
        );
    }

    // Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    enc_cfg.g_w = 640;
    enc_cfg.g_h = 480;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    
    aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(
        &encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION
    );

    // Allocate output buffer for encoded data
    aom_fixed_buf_t output_buf;
    output_buf.buf = malloc(size * 2);
    output_buf.sz = size * 2;
    
    if (enc_init_result == AOM_CODEC_OK && output_buf.buf) {
        aom_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
    }

    // Read and decode input
    uint8_t *decode_buffer = static_cast<uint8_t*>(malloc(size));
    if (decode_buffer) {
        size_t bytes_read = fread(decode_buffer, 1, size, in_file);
        
        if (dec_init_result == AOM_CODEC_OK && bytes_read > 0) {
            aom_codec_decode(&decoder, decode_buffer, bytes_read, nullptr);
        }
        
        free(decode_buffer);
    }

    // Get decoded frame and encode it
    if (dec_init_result == AOM_CODEC_OK && enc_init_result == AOM_CODEC_OK) {
        aom_codec_iter_t iter = nullptr;
        const aom_image_t *img;
        
        while ((img = aom_codec_get_frame(&decoder, &iter))) {
            // Create a copy to remove metadata
            aom_image_t img_copy;
            memcpy(&img_copy, img, sizeof(aom_image_t));
            aom_img_remove_metadata(&img_copy);
            
            // Encode the frame
            aom_codec_encode(&encoder, img, 0, 1, 0);
            
            // Get encoded data and write to file
            const aom_codec_cx_pkt_t *pkt;
            aom_codec_iter_t enc_iter = nullptr;
            while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter))) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
        }
    }

    // Check for errors
    if (dec_init_result != AOM_CODEC_OK) {
        const char *dec_error = aom_codec_error(&decoder);
        (void)dec_error; // Prevent unused variable warning
    }
    
    if (enc_init_result != AOM_CODEC_OK) {
        const char *enc_error = aom_codec_error(&encoder);
        (void)enc_error; // Prevent unused variable warning
    }

    // Cleanup
    if (dec_init_result == AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
    }
    
    if (enc_init_result == AOM_CODEC_OK) {
        aom_codec_destroy(&encoder);
    }
    
    if (output_buf.buf) {
        free(output_buf.buf);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}