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
//<score> 25, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_decode","aom_codec_error","aom_codec_destroy","aom_codec_av1_cx","aom_codec_destroy","aom_codec_destroy","aom_codec_destroy","aom_codec_get_frame","aom_img_remove_metadata","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_av1_cx","aom_codec_get_frame","aom_img_remove_metadata","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to decode an AV1 bitstream, remove metadata from decoded frames, then re-encode them:

*/


extern "C" {
}

// Frame buffer callbacks
static int  id_000031_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    fb->data = (uint8_t*)calloc(1, min_size);
    fb->size = min_size;
    fb->priv = nullptr;
    return fb->data ? 0 : -1;
}

static int  id_000031_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0, 0, 0};
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read and decode data from input file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    uint8_t *decoder_input = (uint8_t*)malloc(file_size);
    if (!decoder_input) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(decoder_input, 1, file_size, in_file);
    assert_file_closed(&in_file);;
    
    // Decode the input data
    res = aom_codec_decode(&decoder, decoder_input, file_size, nullptr);
    if (res != AOM_CODEC_OK) {
        const char *error_msg = aom_codec_error(&decoder);
        (void)error_msg; // Can be used for debugging
        free(decoder_input);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    free(decoder_input);
    
    // Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    int frame_count = 0;
    const int max_frames = 10;
    
    // Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    // Create output file for encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Setup buffer for compressed data
    aom_fixed_buf_t *cx_data_buf = (aom_fixed_buf_t*)calloc(1, sizeof(aom_fixed_buf_t));
    if (!cx_data_buf) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    cx_data_buf->sz = size * 2;
    cx_data_buf->buf = malloc(cx_data_buf->sz);
    if (!cx_data_buf->buf) {
        free(cx_data_buf);
        aom_codec_destroy(&decoder);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Process decoded frames
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr && frame_count < max_frames) {
        frame_count++;
        
        // Remove metadata from the decoded image
        aom_img_remove_metadata(img);
        
        // For the first frame, initialize encoder with image dimensions
        if (frame_count == 1) {
            res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            if (res != AOM_CODEC_OK) {
                break;
            }
            
            enc_cfg.g_w = img->d_w;
            enc_cfg.g_h = img->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            
            res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
            if (res != AOM_CODEC_OK) {
                const char *error_msg = aom_codec_error(&encoder);
                (void)error_msg;
                break;
            }
            
            // Set frame buffer functions
            res = aom_codec_set_frame_buffer_functions(&encoder,  id_000031_get_frame_buffer, 
                                                       id_000031_release_frame_buffer, nullptr);
            if (res != AOM_CODEC_OK) {
                aom_codec_destroy(&encoder);
                break;
            }
            
            // Set compressed data buffer
            res = aom_codec_set_cx_data_buf(&encoder, cx_data_buf, 0, 0);
            if (res != AOM_CODEC_OK) {
                aom_codec_destroy(&encoder);
                break;
            }
        }
        
        // Encode the frame
        res = aom_codec_encode(&encoder, img, frame_count-1, 1, 0);
        if (res != AOM_CODEC_OK) {
            const char *error_msg = aom_codec_error(&encoder);
            (void)error_msg;
            continue;
        }
        
        // Get encoded data
        aom_codec_iter_t enc_iter = nullptr;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Write to output file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
    }
    
    // Flush encoder
    if (frame_count > 0) {
        res = aom_codec_encode(&encoder, nullptr, -1, 1, 0);
        if (res == AOM_CODEC_OK) {
            aom_codec_iter_t enc_iter = nullptr;
            const aom_codec_cx_pkt_t *pkt;
            while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
        }
    }
    
    // Cleanup
    if (frame_count > 0) {
        aom_codec_destroy(&encoder);
    }
    
    assert_file_closed(&out_file);;
    aom_codec_destroy(&decoder);
    
    // Free compressed data buffer
    if (cx_data_buf) {
        free(cx_data_buf->buf);
        free(cx_data_buf);
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}