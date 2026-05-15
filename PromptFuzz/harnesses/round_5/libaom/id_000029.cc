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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 2
//<Quality> {"density":18,"unique_branches":{"estimate_screen_content":[[2097,35,2097,72,0,0,4,0],[2098,35,2098,68,0,0,4,0]]},"library_calls":["aom_uleb_decode","aom_uleb_encode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_err_to_string","aom_codec_enc_init_ver","aom_codec_err_to_string","aom_codec_control","aom_codec_control","aom_codec_control","aom_img_alloc","aom_codec_destroy","aom_img_add_metadata","aom_codec_encode","aom_codec_err_to_string","aom_codec_get_cx_data","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_err_to_string","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_uleb_encode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_control","aom_codec_control","aom_img_alloc","aom_img_add_metadata","aom_codec_encode","aom_codec_err_to_string","aom_codec_get_cx_data","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_err_to_string","aom_img_free","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to achieve the event of encoding an image with metadata and then decoding the encoded data:

*/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing
    
    // 1. Test aom_uleb_encode and aom_uleb_decode
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    uint8_t uleb_buffer[10];
    size_t encoded_size = 0;
    
    // Decode a ULEB value from input
    if (aom_uleb_decode(data, std::min(size, (size_t)10), &uleb_value, &uleb_length) == 0) {
        // Encode it back
        aom_uleb_encode(uleb_value, sizeof(uleb_buffer), uleb_buffer, &encoded_size);
    }
    
    // 2. Initialize encoder with default config
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_err_t err;
    
    // Get default encoder configuration
    err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err != AOM_CODEC_OK) {
        const char* error_str = aom_codec_err_to_string(err);
        // Use error string to avoid unused variable warning
        (void)error_str;
        return 0;
    }
    
    // Set basic encoding parameters
    enc_cfg.g_w = 64;
    enc_cfg.g_h = 64;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    
    // 3. Create encoder context
    aom_codec_ctx_t encoder;
    err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        const char* error_str = aom_codec_err_to_string(err);
        (void)error_str;
        return 0;
    }
    
    // 4. Use aom_codec_control to set encoder parameters
    // Set some encoder controls
    aom_codec_control(&encoder, AOME_SET_CPUUSED, 8);
    aom_codec_control(&encoder, AOME_SET_ENABLEAUTOALTREF, 1);
    aom_codec_control(&encoder, AOME_SET_ARNR_MAXFRAMES, 7);
    
    // 5. Create and prepare image
    aom_image_t* image = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 16);
    if (!image) {
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Fill image planes with fuzz data (simplified - just set some values)
    for (int plane = 0; plane < 3; plane++) {
        int stride = image->stride[plane];
        int height = (plane == 0) ? 64 : 32;
        int width = (plane == 0) ? 64 : 32;
        
        for (int y = 0; y < height; y++) {
            uint8_t* row = image->planes[plane] + y * stride;
            for (int x = 0; x < width; x++) {
                row[x] = (data[(y * width + x) % size] ^ (plane * 50)) & 0xFF;
            }
        }
    }
    
    // 6. Add metadata to image
    uint32_t metadata_type = 1;  // Example metadata type
    size_t metadata_size = std::min(size, (size_t)100);
    const uint8_t* metadata_data = data;
    
    int metadata_result = aom_img_add_metadata(
        image, 
        metadata_type, 
        metadata_data, 
        metadata_size, 
        AOM_MIF_ANY_FRAME
    );
    (void)metadata_result;  // Result not critical for fuzzing
    
    // 7. Encode the image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    err = aom_codec_encode(&encoder, image, pts, duration, flags);
    if (err != AOM_CODEC_OK) {
        const char* error_str = aom_codec_err_to_string(err);
        (void)error_str;
    }
    
    // 8. Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt = NULL;
    std::vector<uint8_t> encoded_buffer;
    
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            encoded_buffer.insert(encoded_buffer.end(),
                                (uint8_t*)pkt->data.frame.buf,
                                (uint8_t*)pkt->data.frame.buf + pkt->data.frame.sz);
        }
    }
    
    // 9. Use aom_codec_peek_stream_info to analyze the encoded data
    if (!encoded_buffer.empty()) {
        aom_codec_stream_info_t stream_info;
        memset(&stream_info, 0, sizeof(stream_info));
        
        aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
        err = aom_codec_peek_stream_info(
            decoder_iface,
            encoded_buffer.data(),
            encoded_buffer.size(),
            &stream_info
        );
        
        if (err != AOM_CODEC_OK) {
            const char* error_str = aom_codec_err_to_string(err);
            (void)error_str;
        }
    }
    
    // 10. Cleanup
    aom_img_free(image);
    aom_codec_destroy(&encoder);
    
    return 0;
}