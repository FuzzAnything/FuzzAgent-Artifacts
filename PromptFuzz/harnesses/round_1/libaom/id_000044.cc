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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy","aom_codec_destroy","aom_img_wrap","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_err_to_string","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_decode","aom_codec_get_frame","aom_img_wrap","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_err_to_string","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom deleter for FILE*
struct FileDeleter {
    void operator()(FILE* fp) const {
        if (fp) assert_file_closed(&fp);;
    }
};

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create input file from fuzzed data
    FILE* in_file = fmemopen((void *)data, size, "rb");
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
    
    // Event: Decode AV1 bitstream and re-encode with added metadata
    
    // 1. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    
    // Use aom_codec_iface_name
    const char* iface_name = aom_codec_iface_name(decoder_iface);
    (void)iface_name; // Use variable to avoid unused warning
    
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // Use aom_codec_err_to_string
    const char* err_str = aom_codec_err_to_string(dec_init_result);
    (void)err_str;
    
    if (dec_init_result != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Decode input data
    aom_codec_decode(&decoder, data, size, NULL);
    
    // 3. Get decoded frame
    aom_codec_iter_t iter = NULL;
    aom_image_t* decoded_frame = aom_codec_get_frame(&decoder, &iter);
    
    if (!decoded_frame) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create wrapped image from decoded data
    // Allocate buffer for image data
    size_t img_data_size = decoded_frame->stride[0] * decoded_frame->d_h * 3 / 2;
    unsigned char* img_data = (unsigned char*)malloc(img_data_size);
    if (!img_data) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Copy decoded data
    for (int plane = 0; plane < 3; plane++) {
        int plane_height = (plane == 0) ? decoded_frame->d_h : decoded_frame->d_h / 2;
        for (int y = 0; y < plane_height; y++) {
            memcpy(img_data + (plane == 0 ? 0 : 
                   decoded_frame->stride[0] * decoded_frame->d_h + 
                   (plane - 1) * decoded_frame->stride[1] * decoded_frame->d_h / 2) + 
                   y * decoded_frame->stride[plane],
                   decoded_frame->planes[plane] + y * decoded_frame->stride[plane],
                   decoded_frame->stride[plane]);
        }
    }
    
    aom_image_t wrapped_img;
    aom_image_t* wrapped = aom_img_wrap(&wrapped_img, decoded_frame->fmt, 
                                       decoded_frame->d_w, decoded_frame->d_h, 
                                       32, img_data);
    
    // 5. Create and add metadata using aom_img_metadata_alloc
    uint32_t metadata_type = 1; // Example metadata type
    uint8_t metadata_data[] = {0x01, 0x02, 0x03, 0x04};
    size_t metadata_size = sizeof(metadata_data);
    
    aom_metadata_t* metadata = aom_img_metadata_alloc(
        metadata_type, metadata_data, metadata_size, AOM_MIF_ANY_FRAME);
    
    if (metadata) {
        // Add metadata to image (using alternative API since aom_img_add_metadata is available)
        aom_img_add_metadata(&wrapped_img, metadata_type, metadata_data, 
                           metadata_size, AOM_MIF_ANY_FRAME);
        aom_img_metadata_free(metadata);
    }
    
    // 6. Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    enc_cfg.g_w = wrapped_img.d_w;
    enc_cfg.g_h = wrapped_img.d_h;
    enc_cfg.rc_target_bitrate = 1000;
    
    // Use aom_codec_enc_init_ver
    aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(
        &encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    
    // Use aom_codec_err_to_string again
    const char* enc_err_str = aom_codec_err_to_string(enc_init_result);
    (void)enc_err_str;
    
    if (enc_init_result == AOM_CODEC_OK) {
        // 7. Encode the wrapped image
        aom_codec_encode(&encoder, &wrapped_img, 0, 1, 0);
        
        // Get encoded data
        aom_codec_iter_t enc_iter = NULL;
        const aom_codec_cx_pkt_t* pkt;
        
        while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Write to output file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
        
        aom_codec_destroy(&encoder);
    }
    
    // Cleanup
    free(img_data);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}