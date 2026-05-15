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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["aom_codec_peek_stream_info","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_alloc","aom_img_alloc_with_border","aom_img_set_rect","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_peek_stream_info","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_alloc","aom_img_alloc_with_border","aom_img_set_rect","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_img_free","aom_codec_destroy"],"visited":0}
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

    // Create a FILE* for input data
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

    // Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);
    (void)in_fd; // Use to avoid unused variable warning

    // Variables for the fuzzing process
    aom_codec_ctx_t decoder, encoder;
    aom_codec_stream_info_t stream_info;
    aom_image_t *img = nullptr, *img_with_border = nullptr;
    aom_metadata_t *metadata = nullptr;
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    aom_codec_err_t res;
    size_t bytes_read;
    
    // 1. First, peek at stream info using the input data
    memset(&stream_info, 0, sizeof(stream_info));
    res = aom_codec_peek_stream_info(aom_codec_av1_dx(), data, size, &stream_info);
    (void)res; // Result may be error if data is not valid AV1 stream

    // 2. Initialize decoder
    res = aom_codec_dec_init_ver(&decoder, aom_codec_av1_dx(), &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 3. Decode the input data
    res = aom_codec_decode(&decoder, data, size, nullptr);
    
    // 4. Get stream info from decoder context
    memset(&stream_info, 0, sizeof(stream_info));
    res = aom_codec_get_stream_info(&decoder, &stream_info);

    // 5. Try to get a decoded frame
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
        // 6. Allocate an image without border
        img = aom_img_alloc(nullptr, decoded_img->fmt, 
                           decoded_img->d_w, decoded_img->d_h, 1);
        
        // 7. Allocate an image with border
        img_with_border = aom_img_alloc_with_border(nullptr, decoded_img->fmt,
                                                   decoded_img->d_w, decoded_img->d_h,
                                                   1, 1, 16); // 16 pixel border
        
        if (img && img_with_border) {
            // 8. Set a rectangle in the image with border
            // Use small rectangle to avoid out-of-bounds
            unsigned int rect_w = decoded_img->d_w > 32 ? 32 : decoded_img->d_w;
            unsigned int rect_h = decoded_img->d_h > 32 ? 32 : decoded_img->d_h;
            aom_img_set_rect(img_with_border, 0, 0, rect_w, rect_h, 16);
            
            // 9. Create and free metadata
            // Use first few bytes of input data for metadata
            size_t metadata_size = size > 100 ? 100 : size;
            metadata = aom_img_metadata_alloc(1, data, metadata_size, 
                                             AOM_MIF_ANY_FRAME);
            if (metadata) {
                aom_img_metadata_free(metadata);
            }
            
            // 10. Initialize encoder with default configuration
            res = aom_codec_enc_config_default(aom_codec_av1_cx(), &enc_cfg, 0);
            if (res == AOM_CODEC_OK) {
                // Set encoder configuration
                enc_cfg.g_w = decoded_img->d_w;
                enc_cfg.g_h = decoded_img->d_h;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.rc_target_bitrate = 1000;
                enc_cfg.g_pass = AOM_RC_ONE_PASS;
                enc_cfg.g_lag_in_frames = 0;
                
                // 11. Initialize encoder
                res = aom_codec_enc_init_ver(&encoder, aom_codec_av1_cx(), 
                                            &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
                
                if (res == AOM_CODEC_OK) {
                    // 12. Encode the image
                    // Use the image with border for encoding
                    res = aom_codec_encode(&encoder, img_with_border, 
                                          0, 1, 0);
                    
                    // Get encoded data
                    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                            // Write to output file
                            fwrite(pkt->data.frame.buf, 1, 
                                  pkt->data.frame.sz, out_file);
                        }
                    }
                    
                    // Destroy encoder
                    aom_codec_destroy(&encoder);
                }
            }
        }
        
        // Free allocated images
        if (img) {
            aom_img_free(img);
        }
        if (img_with_border) {
            aom_img_free(img_with_border);
        }
    }
    
    // 13. Destroy decoder
    aom_codec_destroy(&decoder);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(in_fd);
	return 0;
}