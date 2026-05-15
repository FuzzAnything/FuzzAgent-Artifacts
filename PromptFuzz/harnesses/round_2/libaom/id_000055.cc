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
//<ID> 188
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"sync_write":[[244,7,244,10,0,0,4,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_destroy","aom_codec_destroy","aom_codec_decode","aom_codec_get_frame","aom_img_set_rect","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_get_metadata","aom_img_wrap","aom_img_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_error","aom_img_metadata_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_set_rect","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_get_metadata","aom_img_wrap","aom_img_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_error","aom_img_metadata_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


// Include libaom headers

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing

    // Initialize variables
    aom_codec_ctx_t decoder;
    aom_codec_ctx_t encoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_stream_info_t stream_info = {0};
    aom_fixed_buf_t output_buf = {0};
    aom_image_t *img = nullptr;
    aom_metadata_t *metadata = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    
    // 1. Initialize decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    dec_cfg.threads = 2;
    dec_cfg.w = 256;
    dec_cfg.h = 256;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t dec_init_res = aom_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // Get decoder error if initialization failed
    if (dec_init_res != AOM_CODEC_OK) {
        const char *error_str = aom_codec_error(&decoder);
        (void)error_str; // Use error string to avoid unused variable warning
    }
    
    // 2. Get stream information from input data
    // We'll use a portion of the input data for this
    size_t stream_info_size = size > 1000 ? 1000 : size;
    aom_codec_peek_stream_info(decoder_iface, data, stream_info_size, &stream_info);
    aom_codec_get_stream_info(&decoder, &stream_info);
    
    // 3. Create input file from data buffer
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        aom_codec_destroy(&decoder);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Decode input data
    // Use a portion of data for decoding
    size_t decode_size = size > 5000 ? 5000 : size;
    aom_codec_decode(&decoder, data, decode_size, nullptr);
    
    // 6. Get decoded frame
    aom_codec_iter_t iter = nullptr;
    img = aom_codec_get_frame(&decoder, &iter);
    
    if (img) {
        // 7. Set rectangle on image (modify a portion)
        aom_img_set_rect(img, 10, 10, 50, 50, 1);
        
        // 8. Allocate and add metadata to image
        uint32_t metadata_type = 1;
        uint8_t metadata_data[10] = {0,1,2,3,4,5,6,7,8,9};
        size_t metadata_sz = 10;
        
        metadata = aom_img_metadata_alloc(
            metadata_type, metadata_data, metadata_sz, AOM_MIF_KEY_FRAME);
        
        if (metadata) {
            aom_img_add_metadata(img, metadata_type, metadata_data, 
                                metadata_sz, AOM_MIF_KEY_FRAME);
            
            // 9. Get metadata back from image
            const aom_metadata_t *retrieved_metadata = 
                aom_img_get_metadata(img, 0);
            (void)retrieved_metadata; // Use to avoid unused warning
        }
        
        // 10. Create another image using img_wrap
        unsigned char *img_data = (unsigned char *)malloc(256 * 256 * 3);
        if (img_data) {
            aom_image_t wrapped_img;
            aom_img_wrap(&wrapped_img, AOM_IMG_FMT_I420, 256, 256, 
                        1, img_data);
            aom_img_free(&wrapped_img);  // This doesn't free img_data
            free(img_data);
        }
    }
    
    // 11. Initialize encoder
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    enc_cfg.g_w = 256;
    enc_cfg.g_h = 256;
    
    aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                          0, AOM_ENCODER_ABI_VERSION);
    
    // 12. Set CX data buffer for encoder
    output_buf.sz = 10000;
    output_buf.buf = malloc(output_buf.sz);
    if (output_buf.buf) {
        aom_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
        
        // 13. Encode the image if we have one
        if (img) {
            aom_codec_encode(&encoder, img, 0, 1, 0);
        }
    }
    
    // 14. Get encoder error status
    const char *encoder_error = aom_codec_error(&encoder);
    (void)encoder_error; // Use to avoid unused warning
    
    // Cleanup
    if (metadata) {
        aom_img_metadata_free(metadata);
    }
    
    if (output_buf.buf) {
        free(output_buf.buf);
    }
    
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}