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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 37.5, nr_unique_branch: 6
//<Quality> {"density":15,"unique_branches":{"split_partition_search":[[4538,29,4538,62,0,0,4,1]],"decode_restoration_mode":[[1593,14,1593,26,0,0,4,0]],"av1_get_preview_raw_frame":[[5745,7,5745,22,0,0,4,1],[5749,9,5749,30,0,0,4,0],[5749,34,5749,77,0,0,4,0]],"encoder_get_preview":[[3912,7,3912,57,0,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_iface_name","aom_codec_get_caps","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_alloc_with_border","aom_img_free","aom_img_free","aom_codec_destroy","aom_img_set_rect","aom_img_metadata_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_codec_get_preview_frame","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_metadata_free","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_iface_name","aom_codec_get_caps","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_alloc_with_border","aom_img_set_rect","aom_img_metadata_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_codec_get_preview_frame","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_metadata_free","aom_img_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create input file from fuzzer data
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
    
    // Get file descriptors (not used directly but demonstrating the pattern)
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; (void)out_fd; // Prevent unused variable warnings
    
    // 1. Get codec interface name
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    const char *encoder_name = aom_codec_iface_name(encoder_iface);
    const char *decoder_name = aom_codec_iface_name(decoder_iface);
    (void)encoder_name; (void)decoder_name; // Prevent unused warnings
    
    // 2. Get codec capabilities
    aom_codec_caps_t encoder_caps = aom_codec_get_caps(encoder_iface);
    aom_codec_caps_t decoder_caps = aom_codec_get_caps(decoder_iface);
    (void)encoder_caps; (void)decoder_caps; // Prevent unused warnings
    
    // 3. Create encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err = aom_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set some basic configuration
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    // 4. Create encoder context
    aom_codec_ctx_t encoder;
    err = aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 5. Allocate images with and without border
    aom_image_t *img_without_border = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 1);
    aom_image_t *img_with_border = aom_img_alloc_with_border(NULL, AOM_IMG_FMT_I420, 64, 64, 1, 1, 16);
    
    if (!img_without_border || !img_with_border) {
        if (img_without_border) aom_img_free(img_without_border);
        if (img_with_border) aom_img_free(img_with_border);
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 6. Set rectangle on the image with border
    // Use fuzzer data to determine rectangle parameters (modulo to keep within bounds)
    if (size >= 4) {
        unsigned int x = data[0] % 32;
        unsigned int y = data[1] % 32;
        unsigned int w = 8 + (data[2] % 24);  // 8-32
        unsigned int h = 8 + (data[3] % 24);  // 8-32
        aom_img_set_rect(img_with_border, x, y, w, h, 8);
    }
    
    // 7. Create metadata
    uint32_t metadata_type = 1;
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
    size_t metadata_size = size > 100 ? 100 : size;
    
    aom_metadata_t *metadata = aom_img_metadata_alloc(
        metadata_type, 
        data, 
        metadata_size, 
        insert_flag
    );
    
    // 8. Encode the image
    // Fill image with some data from fuzzer input
    size_t data_offset = 0;
    for (int plane = 0; plane < 3; ++plane) {
        int plane_width = aom_img_plane_width(img_without_border, plane);
        int plane_height = aom_img_plane_height(img_without_border, plane);
        for (int y = 0; y < plane_height; ++y) {
            for (int x = 0; x < plane_width; ++x) {
                if (data_offset < size) {
                    img_without_border->planes[plane][y * img_without_border->stride[plane] + x] = 
                        data[data_offset++];
                }
            }
        }
    }
    
    err = aom_codec_encode(&encoder, img_without_border, 0, 1, 0);
    if (err != AOM_CODEC_OK) {
        // Cleanup and continue
    }
    
    // 9. Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // 10. Try to get preview frame (may return NULL)
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&encoder);
    (void)preview_frame; // Prevent unused warning
    
    // 11. Create decoder and decode the encoded data
    aom_codec_ctx_t decoder;
    err = aom_codec_dec_init_ver(&decoder, decoder_iface, NULL, 0, AOM_DECODER_ABI_VERSION);
    if (err == AOM_CODEC_OK) {
        // Rewind output file and read encoded data
        fflush(out_file);
        assert_file_closed(&out_file);;
        
        // Re-open output file for reading
        out_file = fopen("output_file", "rb");
        if (out_file) {
            fseek(out_file, 0, SEEK_END);
            long encoded_size = ftell(out_file);
            fseek(out_file, 0, SEEK_SET);
            
            if (encoded_size > 0) {
                std::vector<uint8_t> encoded_data(encoded_size);
                fread(encoded_data.data(), 1, encoded_size, out_file);
                
                // Decode the data
                err = aom_codec_decode(&decoder, encoded_data.data(), encoded_size, NULL);
                if (err == AOM_CODEC_OK) {
                    // 12. Get decoded frames
                    aom_codec_iter_t dec_iter = NULL;
                    aom_image_t *decoded_frame;
                    while ((decoded_frame = aom_codec_get_frame(&decoder, &dec_iter)) != NULL) {
                        // Do something with decoded frame
                        (void)decoded_frame;
                    }
                }
            }
        }
    }
    
    // 13. Cleanup
    if (metadata) aom_img_metadata_free(metadata);
    if (img_without_border) aom_img_free(img_without_border);
    if (img_with_border) aom_img_free(img_with_border);
    if (aom_codec_destroy(&encoder) != AOM_CODEC_OK) {
        // Handle error if needed
    }
    if (aom_codec_destroy(&decoder) != AOM_CODEC_OK) {
        // Handle error if needed
    }
    
    assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}