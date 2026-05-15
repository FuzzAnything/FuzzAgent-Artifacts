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
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 8
//<Quality> {"density":11,"unique_branches":{"aom_obu_type_to_string":[[189,5,189,32,0,0,4,0],[190,5,190,26,0,0,4,0],[191,5,191,36,0,0,4,0],[193,5,193,24,0,0,4,0],[194,5,194,22,0,0,4,0],[195,5,195,23,0,0,4,0],[196,5,196,21,0,0,4,0],[197,5,197,12,0,0,4,0]]},"library_calls":["aom_obu_type_to_string","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_wrap","aom_codec_destroy","aom_uleb_encode_fixed_size","aom_codec_encode","aom_codec_get_cx_data","aom_codec_get_preview_frame","aom_img_remove_metadata","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_obu_type_to_string","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_wrap","aom_uleb_encode_fixed_size","aom_codec_encode","aom_codec_get_cx_data","aom_codec_get_preview_frame","aom_img_remove_metadata","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Too small for meaningful processing
    }

    // 1. Create a file-like object from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Parse OBU type from first byte and use aom_obu_type_to_string
    uint8_t first_byte = data[0];
    OBU_TYPE obu_type = static_cast<OBU_TYPE>((first_byte >> 3) & 0x1F);
    const char *obu_type_str = aom_obu_type_to_string(obu_type);
    (void)obu_type_str; // Use the string to satisfy fuzzer usage

    // 3. Create encoder context
    aom_codec_ctx_t encoder_ctx;
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    enc_cfg.g_w = 64;
    enc_cfg.g_h = 64;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    
    if (aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 0, 
                               AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Use aom_codec_set_option
    aom_codec_set_option(&encoder_ctx, "cpu-used", "1");

    // 5. Create and wrap an image
    const int width = 64;
    const int height = 64;
    const int stride_align = 32;
    
    // Allocate image data buffer
    size_t img_buffer_size = width * height * 3 / 2; // I420 format
    std::vector<unsigned char> img_buffer(img_buffer_size);
    
    // Fill with some data from input
    size_t copy_size = std::min(img_buffer_size, size - 1);
    memcpy(img_buffer.data(), data + 1, copy_size);
    
    aom_image_t img;
    aom_image_t *wrapped_img = aom_img_wrap(&img, AOM_IMG_FMT_I420, width, height, 
                                            stride_align, img_buffer.data());
    
    if (!wrapped_img) {
        aom_codec_destroy(&encoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Use aom_uleb_encode_fixed_size
    uint64_t uleb_value = static_cast<uint64_t>(size);
    uint8_t coded_value[16];
    size_t coded_size = 0;
    aom_uleb_encode_fixed_size(uleb_value, sizeof(coded_value), 8, 
                               coded_value, &coded_size);

    // 7. Encode the image
    if (aom_codec_encode(&encoder_ctx, wrapped_img, 0, 1, 0) == AOM_CODEC_OK) {
        // 8. Get encoded data using aom_codec_get_cx_data
        aom_codec_iter_t iter = nullptr;
        const aom_codec_cx_pkt_t *pkt;
        
        while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &iter)) != nullptr) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Process encoded frame
                break;
            }
        }
        
        // 9. Try to get preview frame
        const aom_image_t *preview_frame = aom_codec_get_preview_frame(&encoder_ctx);
        (void)preview_frame; // Use to satisfy fuzzer usage
    }

    // 10. Remove metadata from image
    aom_img_remove_metadata(&img);

    // 11. Create decoder context
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0, 0, 0};
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    if (aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 0,
                               AOM_DECODER_ABI_VERSION) == AOM_CODEC_OK) {
        // Try to decode some data
        size_t decode_size = std::min(size, static_cast<size_t>(1000));
        aom_codec_decode(&decoder_ctx, data, decode_size, nullptr);
        
        // Get decoded frames
        aom_codec_iter_t iter = nullptr;
        aom_image_t *decoded_img;
        while ((decoded_img = aom_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
            // Process decoded image
            break;
        }
        
        aom_codec_destroy(&decoder_ctx);
    }

    // 12. Cleanup
    assert_file_closed(&in_file);;
    aom_codec_destroy(&encoder_ctx);
    
    // Note: aom_img_wrap doesn't allocate the buffer, so we don't need to free it
    // The img_buffer vector will be automatically destroyed

    assert_file_closed(&in_file);
	return 0;
}