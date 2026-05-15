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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 68, nr_unique_branch: 3
//<Quality> {"density":17,"unique_branches":{"aom_codec_err_to_string":[[40,5,40,29,0,0,4,0]],"setup_buffer_pool":[[2000,7,2005,20,0,0,4,0]],"realloc_frame_buffer_aligned":[[111,31,111,61,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_img_alloc","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_img_alloc","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback functions
static int  id_000029_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)min_size;
    // For simplicity, allocate a fixed buffer
    static uint8_t buffer[4096];
    fb->data = buffer;
    fb->size = sizeof(buffer);
    return 0;
}

static int  id_000029_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    // Nothing to do for static buffer
    (void)cb_priv;
    (void)fb;
    return 0;
}

// Main fuzzer function
extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Use fmemopen to create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open output file for encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Read input data into a buffer
    fseek(in_file, 0, SEEK_END);
    long input_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (input_size <= 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    std::vector<uint8_t> input_buffer(input_size);
    size_t bytes_read = fread(input_buffer.data(), 1, input_size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read != static_cast<size_t>(input_size)) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                                                 AOM_DECODER_ABI_VERSION);
    
    // Set frame buffer functions for decoder
    if (res == AOM_CODEC_OK) {
        res = aom_codec_set_frame_buffer_functions(&decoder,  id_000029_get_frame_buffer, 
                                                    id_000029_release_frame_buffer, nullptr);
    }

    // Decode input data
    if (res == AOM_CODEC_OK) {
        res = aom_codec_decode(&decoder, input_buffer.data(), input_buffer.size(), nullptr);
    }

    // Get error message
    const char *error_msg = aom_codec_error(&decoder);
    (void)error_msg; // Prevent unused variable warning

    // Try to get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_frame = nullptr;
    if (res == AOM_CODEC_OK) {
        decoded_frame = aom_codec_get_frame(&decoder, &iter);
    }

    // Initialize encoder if we got a frame
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t enc_cfg;
    aom_image_t *image_to_encode = nullptr;
    
    if (decoded_frame) {
        // Create image for encoding
        image_to_encode = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 
                                       decoded_frame->d_w, decoded_frame->d_h, 16);
        
        if (image_to_encode) {
            // Remove metadata from image
            aom_img_remove_metadata(image_to_encode);
            
            // Initialize encoder
            aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
            res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            
            if (res == AOM_CODEC_OK) {
                enc_cfg.g_w = decoded_frame->d_w;
                enc_cfg.g_h = decoded_frame->d_h;
                enc_cfg.rc_end_usage = AOM_Q;
                
                res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0,
                                            AOM_ENCODER_ABI_VERSION);
            }

            // Set frame buffer functions for encoder
            if (res == AOM_CODEC_OK) {
                res = aom_codec_set_frame_buffer_functions(&encoder,  id_000029_get_frame_buffer,
                                                           id_000029_release_frame_buffer, nullptr);
            }

            // Set output buffer for encoder
            if (res == AOM_CODEC_OK) {
                aom_fixed_buf_t output_buf;
                static uint8_t enc_buffer[8192];
                output_buf.buf = enc_buffer;
                output_buf.sz = sizeof(enc_buffer);
                
                res = aom_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
            }

            // Encode the frame
            if (res == AOM_CODEC_OK) {
                res = aom_codec_encode(&encoder, image_to_encode, 0, 1, 0);
            }

            // Get encoded data and write to file
            if (res == AOM_CODEC_OK) {
                aom_codec_iter_t enc_iter = nullptr;
                const aom_codec_cx_pkt_t *pkt = nullptr;
                
                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                        break;
                    }
                }
            }

            // Clean up encoder
            aom_codec_destroy(&encoder);
        }
    }

    // Clean up resources
    if (image_to_encode) {
        aom_img_free(image_to_encode);
    }
    
    aom_codec_destroy(&decoder);
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}