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
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Use input data for decoding
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

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Suppress unused variable warning
    (void)out_fd; // Suppress unused variable warning

    // Variables for decoder and encoder
    aom_codec_ctx_t decoder;
    aom_codec_ctx_t encoder;
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt = NULL;
    aom_image_t *img = NULL;
    aom_metadata_t *metadata = NULL;
    
    // Get AV1 decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Peek stream info from input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    aom_codec_err_t peek_result = aom_codec_peek_stream_info(
        decoder_iface, data, size, &stream_info);
    
    // Initialize decoder
    aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
        &decoder, decoder_iface, NULL, 0, AOM_DECODER_ABI_VERSION);
    
    if (dec_init_result != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Decode input data
    aom_codec_err_t decode_result = aom_codec_decode(
        &decoder, data, size, NULL);
    
    // Get decoded frame
    img = aom_codec_get_frame(&decoder, &iter);
    
    if (img) {
        // Allocate image with border using dimensions from stream info
        // If stream info not available, use default dimensions
        unsigned int width = stream_info.w ? stream_info.w : 640;
        unsigned int height = stream_info.h ? stream_info.h : 480;
        
        aom_image_t *border_img = aom_img_alloc_with_border(
            NULL, AOM_IMG_FMT_I420, width, height, 
            32, 32, 16); // align=32, size_align=32, border=16
        
        if (border_img) {
            // Create metadata
            uint8_t meta_data[] = {0x01, 0x02, 0x03, 0x04};
            metadata = aom_img_metadata_alloc(
                1, meta_data, sizeof(meta_data), AOM_MIF_ANY_FRAME);
            
            if (metadata) {
                // Add metadata to image (note: this requires img to have metadata array)
                // For this example, we'll just allocate and free the metadata
                aom_img_metadata_free(metadata);
                metadata = NULL;
            }
            
            aom_img_free(border_img);
        }

        // Get AV1 encoder interface
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        if (encoder_iface) {
            // Initialize encoder configuration
            aom_codec_enc_cfg_t cfg;
            aom_codec_enc_config_default(encoder_iface, &cfg, 0);
            
            // Set dimensions
            cfg.g_w = img->d_w;
            cfg.g_h = img->d_h;
            cfg.g_timebase.num = 1;
            cfg.g_timebase.den = 30;
            
            // Initialize encoder
            aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(
                &encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
            
            if (enc_init_result == AOM_CODEC_OK) {
                // Encode the image
                aom_codec_err_t encode_result = aom_codec_encode(
                    &encoder, img, 0, 1, 0);
                
                // Get preview frame
                const aom_image_t *preview_frame = aom_codec_get_preview_frame(&encoder);
                (void)preview_frame; // Suppress unused variable warning
                
                // Get encoded data packets
                while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                        // Write encoded data to file
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                // Clean up encoder
                aom_codec_destroy(&encoder);
            }
        }
    }

    // Clean up decoder
    aom_codec_destroy(&decoder);

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}