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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 35, nr_unique_branch: 2
//<Quality> {"density":35,"unique_branches":{"get_intra_q_and_bounds":[[34,23,34,34,1,0,4,0],[34,23,34,34,2,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_destroy","aom_codec_decode","aom_codec_error_detail","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy","aom_img_flip","aom_img_plane_height","aom_img_plane_width","aom_uleb_encode","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_set_option","aom_codec_enc_config_set","aom_codec_destroy","aom_codec_destroy","aom_img_alloc","aom_codec_destroy","aom_codec_destroy","aom_codec_get_global_headers","aom_codec_encode","aom_img_free","aom_codec_destroy","aom_codec_destroy","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_flip","aom_img_plane_height","aom_img_plane_width","aom_uleb_encode","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_enc_config_set","aom_img_alloc","aom_codec_get_global_headers","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // 1. Initialize decoder to decode input bitstream
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // 2. Peek stream info from input data
    aom_codec_stream_info_t si = {0};
    if (aom_codec_peek_stream_info(decoder_iface, data, size, &si) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // 3. Decode the input frame
    if (aom_codec_decode(&decoder, data, size, nullptr) != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail; // Use error detail
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // 4. Get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
    if (!decoded_img) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // 5. Process image: flip it
    aom_img_flip(decoded_img);
    
    // 6. Get image dimensions
    int plane_height = aom_img_plane_height(decoded_img, 0);
    int plane_width = aom_img_plane_width(decoded_img, 0);
    (void)plane_height; (void)plane_width; // Use variables
    
    // 7. Create metadata
    uint8_t metadata_data[10] = {0};
    size_t coded_size = 0;
    if (aom_uleb_encode(12345, sizeof(metadata_data), metadata_data, &coded_size) == 0) {
        aom_metadata_t *metadata = aom_img_metadata_alloc(
            1, metadata_data, coded_size, AOM_MIF_ANY_FRAME);
        if (metadata) {
            aom_img_metadata_free(metadata);
        }
    }
    
    // 8. Remove any existing metadata from image
    aom_img_remove_metadata(decoded_img);
    
    // 9. Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Set encoder configuration
    enc_cfg.g_w = decoded_img->d_w;
    enc_cfg.g_h = decoded_img->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_threads = 1;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    enc_cfg.rc_end_usage = AOM_Q;
    enc_cfg.rc_min_quantizer = 20;
    enc_cfg.rc_max_quantizer = 40;
    
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // 10. Set encoder option
    aom_codec_set_option(&encoder, "cpu-used", "1");
    
    // 11. Reconfigure encoder
    if (aom_codec_enc_config_set(&encoder, &enc_cfg) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // 12. Allocate new image for encoding
    aom_image_t *enc_img = aom_img_alloc(
        nullptr, 
        decoded_img->fmt, 
        decoded_img->d_w, 
        decoded_img->d_h, 
        16
    );
    
    if (!enc_img) {
        aom_codec_destroy(&decoder);
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Copy decoded image to new image (simplified - actual copy would need plane-by-plane)
    // For fuzzing purposes, we'll just use the decoded image directly
    
    // 13. Get global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder);
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    // 14. Encode the frame
    if (aom_codec_encode(&encoder, decoded_img, 0, 1, 0) != AOM_CODEC_OK) {
        aom_img_free(enc_img);
        aom_codec_destroy(&decoder);
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // 15. Get preview frame
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&encoder);
    (void)preview_frame; // Use variable
    
    // 16. Get encoded data
    aom_codec_iter_t enc_iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // We have encoded frame data
            break;
        }
    }
    
    // 17. Cleanup
    aom_img_free(enc_img);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    
    return 0;
}