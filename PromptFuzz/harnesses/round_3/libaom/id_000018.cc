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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 1
//<Quality> {"density":17,"unique_branches":{"aom_img_metadata_alloc":[[340,7,340,12,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_alloc_with_border","aom_img_set_rect","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_alloc_with_border","aom_img_set_rect","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


// Include necessary headers from libaom

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if there's not enough data
    if (size < 1) {
	return 0;
	}
	

    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Read the entire input into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    if (file_size <= 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    uint8_t *input_buffer = (uint8_t *)malloc(file_size);
    if (!input_buffer) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    size_t bytes_read = fread(input_buffer, 1, file_size, in_file);
    assert_file_closed(&in_file);;

    if (bytes_read != (size_t)file_size) {
        free(input_buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // First, peek stream info to get dimensions
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    aom_codec_err_t peek_result = aom_codec_peek_stream_info(
        decoder_iface, input_buffer, bytes_read, &stream_info);
    
    // Initialize decoder with default config
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (dec_init_result != AOM_CODEC_OK) {
        free(input_buffer);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get stream info again using decoder context
    aom_codec_err_t get_info_result = aom_codec_get_stream_info(&decoder, &stream_info);

    // Decode the input frame
    aom_codec_err_t decode_result = aom_codec_decode(
        &decoder, input_buffer, bytes_read, nullptr);

    // Try to get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);

    // If we have a decoded image, process it
    if (decoded_img) {
        // Allocate image using aom_img_alloc
        aom_image_t *allocated_img = aom_img_alloc(
            nullptr, 
            decoded_img->fmt, 
            decoded_img->d_w, 
            decoded_img->d_h, 
            32); // 32-byte alignment
        
        if (allocated_img) {
            // Allocate another image with border using aom_img_alloc_with_border
            aom_image_t *bordered_img = aom_img_alloc_with_border(
                nullptr,
                decoded_img->fmt,
                decoded_img->d_w,
                decoded_img->d_h,
                32, // alignment
                32, // size alignment
                8); // 8-pixel border
            
            if (bordered_img && allocated_img) {
                // Set a rectangle in the allocated image
                // Use safe dimensions to avoid out-of-bounds
                unsigned int rect_w = decoded_img->d_w > 16 ? 16 : decoded_img->d_w;
                unsigned int rect_h = decoded_img->h > 16 ? 16 : decoded_img->h;
                
                if (rect_w > 0 && rect_h > 0) {
                    aom_img_set_rect(
                        allocated_img,
                        0, // x
                        0, // y
                        rect_w,
                        rect_h,
                        0); // border (for fill value)
                }
                
                // Create and free metadata
                aom_metadata_t *metadata = aom_img_metadata_alloc(
                    1, // arbitrary type
                    nullptr,
                    0,
                    AOM_MIF_ANY_FRAME);
                
                if (metadata) {
                    aom_img_metadata_free(metadata);
                }
                
                // Initialize encoder
                aom_codec_ctx_t encoder;
                aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
                
                aom_codec_enc_cfg_t enc_cfg;
                aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
                
                // Set encoder configuration
                enc_cfg.g_w = decoded_img->d_w;
                enc_cfg.g_h = decoded_img->d_h;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.rc_target_bitrate = 1000;
                enc_cfg.g_pass = AOM_RC_ONE_PASS;
                enc_cfg.g_lag_in_frames = 0;
                enc_cfg.rc_end_usage = AOM_VBR;
                
                aom_codec_enc_init_ver(
                    &encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
                
                // Encode the image
                aom_codec_encode(
                    &encoder,
                    allocated_img,
                    0, // pts
                    1, // duration
                    0); // flags
                
                // Get encoded data
                aom_codec_iter_t enc_iter = nullptr;
                const aom_codec_cx_pkt_t *pkt;
                
                // Open output file
                FILE *out_file = fopen("output_file", "wb");
                if (out_file) {
                    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                        }
                    }
                    assert_file_closed(&out_file);;
                }
                
                // Clean up encoder
                aom_codec_destroy(&encoder);
                
                // Clean up allocated images
                aom_img_free(bordered_img);
            }
            
            // Clean up allocated image
            if (allocated_img) {
                aom_img_free(allocated_img);
            }
        }
    }

    // Clean up
    aom_codec_destroy(&decoder);
    free(input_buffer);

    assert_file_closed(&in_file);
	return 0;
}