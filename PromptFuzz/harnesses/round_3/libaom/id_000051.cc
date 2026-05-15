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
//<ID> 132
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error","aom_codec_decode","aom_codec_error","aom_codec_peek_stream_info","aom_codec_get_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_free","aom_codec_get_preview_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_control","aom_codec_error","aom_codec_encode","aom_codec_get_cx_data","aom_codec_get_preview_frame","aom_uleb_encode_fixed_size","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error","aom_codec_peek_stream_info","aom_codec_get_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_free","aom_codec_get_preview_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_control","aom_codec_error","aom_codec_encode","aom_codec_get_cx_data","aom_codec_get_preview_frame","aom_uleb_encode_fixed_size","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/*Looking at the APIs provided, I can create a fuzz driver that processes encoded AV1 data (libaom output), decodes it, extracts metadata from the decoded frames, and demonstrates ULEB encoding with fixed size. Here's a complete fuzz driver:

*/


extern "C" {
}

// Define control IDs if not available in headers
#ifndef AOM_CTRL_ID_START
#define AOME_GET_PREVIEW 1
#define AOMD_GET_FRAME_CORRUPTED 9
#endif

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;  // Need at least some data
    }

    // Create a FILE pointer for the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create an output file for any encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    aom_codec_ctx_t decoder;
    aom_codec_ctx_t encoder;
    aom_image_t *img = nullptr;
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    bool decoder_initialized = false;
    bool encoder_initialized = false;
    
    // Initialize decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    
    if (aom_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        const char *error_msg = aom_codec_error(&decoder);
        // Use error message even if we can't decode
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    decoder_initialized = true;

    // Try to decode the input data
    aom_codec_err_t decode_result = aom_codec_decode(&decoder, data, size, nullptr);
    
    // Get error message for decoder
    const char *decoder_error = aom_codec_error(&decoder);
    (void)decoder_error; // Use variable to avoid unused warning
    
    // Try to get stream info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    if (aom_codec_peek_stream_info(decoder_iface, data, size, &si) == AOM_CODEC_OK) {
        // Use stream info if available
    }

    // Try to get decoded frames
    const aom_image_t *frame = nullptr;
    while ((frame = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Store the first frame for further processing
        if (!img) {
            // Create a copy of the frame
            img = aom_img_alloc(nullptr, frame->fmt, frame->d_w, frame->d_h, 1);
            if (img) {
                // Copy frame data
                for (int plane = 0; plane < 3; plane++) {
                    int width = aom_img_plane_width(frame, plane);
                    int height = aom_img_plane_height(frame, plane);
                    for (int y = 0; y < height; y++) {
                        memcpy(img->planes[plane] + y * img->stride[plane],
                               frame->planes[plane] + y * frame->stride[plane],
                               width);
                    }
                }
                
                // Try to get metadata from the image
                size_t num_metadata = aom_img_num_metadata(img);
                for (size_t i = 0; i < num_metadata; i++) {
                    const aom_metadata_t *metadata = aom_img_get_metadata(img, i);
                    if (metadata) {
                        // Free the metadata after use
                        aom_img_metadata_free(const_cast<aom_metadata_t*>(metadata));
                    }
                }
                
                // Try to get preview frame from decoder context
                // Note: aom_codec_get_preview_frame is typically for encoder context,
                // but we'll call it on decoder for demonstration
                const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
                (void)preview_frame; // Use variable
            }
        }
    }

    // Try encoder initialization if we have an image
    if (img) {
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        aom_codec_enc_cfg_t enc_cfg;
        
        if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
            enc_cfg.g_w = img->d_w;
            enc_cfg.g_h = img->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            
            if (aom_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
                encoder_initialized = true;
                
                // Try to use aom_codec_control with some control IDs
                // Using placeholder control IDs since actual ones aren't provided
                aom_codec_control(&encoder, AOME_GET_PREVIEW, nullptr);
                aom_codec_control(&decoder, AOMD_GET_FRAME_CORRUPTED, nullptr);
                
                // Get error message for encoder
                const char *encoder_error = aom_codec_error(&encoder);
                (void)encoder_error; // Use variable
                
                // Try to encode the image
                if (aom_codec_encode(&encoder, img, 0, 1, 0) == AOM_CODEC_OK) {
                    // Get encoded data
                    iter = nullptr;
                    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                        }
                    }
                    
                    // Try to get preview frame from encoder context
                    const aom_image_t *encoder_preview = aom_codec_get_preview_frame(&encoder);
                    (void)encoder_preview; // Use variable
                }
            }
        }
    }

    // Demonstrate aom_uleb_encode_fixed_size
    // Use some values from the input for ULEB encoding
    if (size >= 8) {
        uint64_t value = 0;
        for (size_t i = 0; i < 8 && i < size; i++) {
            value = (value << 8) | data[i];
        }
        
        uint8_t coded_buffer[16];
        size_t coded_size = 0;
        
        // Try to encode with fixed size
        int encode_result = aom_uleb_encode_fixed_size(value, sizeof(coded_buffer), 
                                                      8, coded_buffer, &coded_size);
        if (encode_result == 0 && coded_size > 0) {
            // Write encoded ULEB to output file
            fwrite(coded_buffer, 1, coded_size, out_file);
        }
    }

    // Cleanup
    if (img) {
        aom_img_free(img);
    }
    
    if (encoder_initialized) {
        aom_codec_destroy(&encoder);
    }
    
    if (decoder_initialized) {
        aom_codec_destroy(&decoder);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}