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
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_img_alloc_with_border","aom_codec_destroy","aom_img_set_rect","aom_img_metadata_alloc","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_metadata_free","aom_img_free","aom_codec_destroy","aom_codec_enc_init_ver","aom_img_metadata_free","aom_img_free","aom_codec_destroy","aom_codec_decode","aom_codec_get_frame","aom_codec_encode","aom_codec_get_cx_data","aom_img_metadata_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_img_alloc_with_border","aom_img_set_rect","aom_img_metadata_alloc","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_encode","aom_codec_get_cx_data","aom_img_metadata_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize variables
    aom_codec_ctx_t decoder, encoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    aom_codec_enc_cfg_t enc_cfg;
    aom_image_t *img = nullptr;
    aom_metadata_t *metadata = nullptr;
    aom_codec_err_t res;
    
    // 1. Initialize decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    dec_cfg.threads = 1;
    dec_cfg.w = 128;
    dec_cfg.h = 128;
    dec_cfg.allow_lowbitdepth = 1;
    
    res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Allocate image with border for encoding
    const unsigned int width = 128;
    const unsigned int height = 128;
    const unsigned int border = 16;
    const aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    
    img = aom_img_alloc_with_border(nullptr, fmt, width, height, 1, 1, border);
    if (!img) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Set rectangle on the image (using fuzzed data for coordinates)
    // Use first 16 bytes for rectangle parameters, modulo for safety
    unsigned int x = data[0] % (width - 32);
    unsigned int y = data[1] % (height - 32);
    unsigned int w = 32 + (data[2] % 64);
    unsigned int h = 32 + (data[3] % 64);
    
    // Ensure rectangle fits within image
    w = (x + w > width) ? (width - x) : w;
    h = (y + h > height) ? (height - y) : h;
    
    if (w > 0 && h > 0) {
        aom_img_set_rect(img, x, y, w, h, border);
    }
    
    // 4. Create metadata using fuzzed data
    // Use a portion of input for metadata
    size_t metadata_size = size > 200 ? 100 : size / 2;
    if (metadata_size > 0) {
        uint32_t metadata_type = 1; // Example type
        aom_metadata_insert_flags_t insert_flag = AOM_MIF_KEY_FRAME;
        
        metadata = aom_img_metadata_alloc(
            metadata_type, 
            data + 50, // Offset into fuzzed data
            metadata_size,
            insert_flag
        );
        
        if (metadata) {
            // Add metadata to image if supported by the API
            // Note: aom_img_add_metadata is in the API list but not in our required APIs
            // We'll just create the metadata object as required
        }
    }
    
    // 5. Initialize encoder
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (res != AOM_CODEC_OK) {
        if (metadata) aom_img_metadata_free(metadata);
        aom_img_free(img);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    enc_cfg.g_w = width;
    enc_cfg.g_h = height;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_error_resilient = 0;
    
    res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        if (metadata) aom_img_metadata_free(metadata);
        aom_img_free(img);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Try to decode some data (simulate using input data)
    // We'll use a portion of the fuzzed data as if it were a compressed frame
    size_t decode_size = size > 500 ? 200 : size / 3;
    if (decode_size > 0) {
        aom_codec_decode(&decoder, data, decode_size, nullptr);
        
        // Try to get a decoded frame
        aom_codec_iter_t iter = nullptr;
        aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
        
        // If we got a decoded image, we could use it, but for this fuzzer
        // we're focusing on the required APIs
    }
    
    // 7. Encode the image using required API
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    res = aom_codec_encode(&encoder, img, pts, duration, flags);
    
    // 8. Get encoded data
    aom_codec_iter_t enc_iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    
    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        // Process encoded packets if needed
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Could write to file or process further
        }
    }
    
    // 9. Create output file and write something (optional)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write a small header or the first few bytes of processed data
        fwrite(data, 1, size > 50 ? 50 : size, out_file);
        assert_file_closed(&out_file);;
    }
    
    // 10. Cleanup
    if (metadata) {
        aom_img_metadata_free(metadata);
    }
    
    aom_img_free(img);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}