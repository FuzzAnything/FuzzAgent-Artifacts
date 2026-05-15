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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_uleb_encode_fixed_size","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_set_option","aom_img_wrap","aom_codec_destroy","aom_codec_destroy","aom_codec_encode","aom_codec_get_cx_data","aom_codec_decode","aom_codec_get_preview_frame","aom_obu_type_to_string","aom_obu_type_to_string","aom_img_remove_metadata","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_uleb_encode_fixed_size","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_wrap","aom_codec_encode","aom_codec_get_cx_data","aom_codec_decode","aom_codec_get_preview_frame","aom_obu_type_to_string","aom_obu_type_to_string","aom_img_remove_metadata","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    // 1. Create input FILE* from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output FILE* for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Read some data for ULEB encoding
    uint64_t uleb_value = 0;
    size_t uleb_bytes_read = 0;
    if (size >= 8) {
        memcpy(&uleb_value, data, 8);
    }
    
    // 5. Use aom_uleb_encode_fixed_size
    uint8_t uleb_buffer[10];
    size_t coded_size = 0;
    aom_uleb_encode_fixed_size(uleb_value, sizeof(uleb_buffer), 5, 
                              uleb_buffer, &coded_size);
    
    // 6. Initialize encoder context
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    enc_cfg.g_w = 64;
    enc_cfg.g_h = 64;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_threads = 1;
    enc_cfg.rc_end_usage = AOM_Q;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.rc_min_quantizer = 2;
    enc_cfg.rc_max_quantizer = 56;
    
    if (aom_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Use aom_codec_set_option
    aom_codec_set_option(&encoder, "cpu-used", "1");
    
    // 8. Create image using aom_img_wrap
    const int img_width = 64;
    const int img_height = 64;
    const size_t img_data_size = img_width * img_height * 3 / 2; // I420 format
    std::vector<unsigned char> img_data(img_data_size);
    
    // Fill with some pattern from input data
    for (size_t i = 0; i < img_data_size && i < size; i++) {
        img_data[i] = data[i] ^ 0x80;
    }
    
    aom_image_t img;
    aom_image_t *wrapped_img = aom_img_wrap(&img, AOM_IMG_FMT_I420, 
                                           img_width, img_height, 
                                           1, img_data.data());
    
    if (!wrapped_img) {
        aom_codec_destroy(&encoder);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 9. Encode the image
    aom_codec_encode(&encoder, wrapped_img, 0, 1, 0);
    
    // 10. Get encoded data using aom_codec_get_cx_data
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Write to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // 11. Try to decode some data
    size_t decode_size = size > 4096 ? 4096 : size;
    aom_codec_decode(&decoder, data, decode_size, nullptr);
    
    // 12. Get preview frame using aom_codec_get_preview_frame
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
    
    // 13. Use aom_obu_type_to_string for some OBU types
    const char *obu_str1 = aom_obu_type_to_string(OBU_SEQUENCE_HEADER);
    const char *obu_str2 = aom_obu_type_to_string(OBU_FRAME);
    
    // 14. Use aom_img_remove_metadata
    aom_img_remove_metadata(wrapped_img);
    
    // 15. Cleanup
    // Note: aom_img_wrap doesn't allocate the image structure, so we don't call aom_img_free
    // But if we had used aom_img_alloc, we would need to free it
    
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}