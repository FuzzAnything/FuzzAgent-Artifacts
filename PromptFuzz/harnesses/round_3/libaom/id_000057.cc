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
//<ID> 165
//<Prompt> []
/*<Combination>: [
*/
//<score> 96, nr_unique_branch: 5
//<Quality> {"density":24,"unique_branches":{"round_shift_16bit_ssse3":[[61,7,61,14,0,0,4,1],[66,14,66,21,0,0,4,1]],"decoder_get_frame":[[820,7,820,32,0,0,4,0]],"decoder_decode":[[690,7,690,19,0,0,4,0],[690,23,690,35,0,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_img_add_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_codec_dec_init_ver","aom_img_free","aom_codec_destroy","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_get_preview_frame","aom_img_wrap","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_num_metadata","aom_img_get_metadata","aom_codec_error_detail","aom_codec_error_detail","aom_img_remove_metadata","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_img_add_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_get_preview_frame","aom_img_wrap","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_num_metadata","aom_img_get_metadata","aom_codec_error_detail","aom_codec_error_detail","aom_img_remove_metadata","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000057_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)min_size;
    fb->data = (uint8_t*)malloc(4096); // Simple allocation
    fb->size = 4096;
    return fb->data ? 0 : -1;
}

static int  id_000057_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Get codec interfaces
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // 2. Check capabilities
    aom_codec_caps_t enc_caps = aom_codec_get_caps(encoder_iface);
    aom_codec_caps_t dec_caps = aom_codec_get_caps(decoder_iface);
    (void)enc_caps; (void)dec_caps; // Prevent unused warnings
    
    // 3. Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t enc_cfg;
    unsigned int usage = 0; // Good quality
    aom_codec_enc_config_default(encoder_iface, &enc_cfg, usage);
    
    // Set some config from input data
    enc_cfg.g_w = 128 + (data[0] % 128);
    enc_cfg.g_h = 128 + (data[1] % 128);
    enc_cfg.g_threads = 1 + (data[2] % 4);
    enc_cfg.rc_target_bitrate = 1000 + (data[3] % 4000);
    
    aom_codec_err_t res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, 
                                                 AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
	return 0;
	}
	
    
    // 4. Set encoder option
    aom_codec_set_option(&encoder, "cpu-used", "1");
    
    // 5. Create and prepare source image
    aom_image_t src_img;
    aom_img_alloc(&src_img, AOM_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 16);
    
    // Fill image with pattern from input data
    size_t data_offset = 4;
    for (int plane = 0; plane < 3; ++plane) {
        int stride = src_img.stride[plane];
        int height = aom_img_plane_height(&src_img, plane);
        int width = aom_img_plane_width(&src_img, plane);
        
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (data_offset < size) {
                    src_img.planes[plane][y * stride + x] = data[data_offset++];
                } else {
                    src_img.planes[plane][y * stride + x] = (plane == 0) ? 128 : 0;
                }
            }
        }
    }
    
    // 6. Add metadata to image
    uint32_t metadata_type = 1; // Example type
    size_t metadata_size = (size > data_offset + 20) ? 20 : size - data_offset;
    aom_img_add_metadata(&src_img, metadata_type, 
                        data + data_offset, metadata_size, 
                        AOM_MIF_ANY_FRAME);
    
    // 7. Encode the image
    aom_codec_encode(&encoder, &src_img, 0, 1, 0);
    
    // 8. Get encoded data
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt;
    std::vector<uint8_t> encoded_data;
    
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            encoded_data.insert(encoded_data.end(),
                              (uint8_t*)pkt->data.frame.buf,
                              (uint8_t*)pkt->data.frame.buf + pkt->data.frame.sz);
        }
    }
    
    // 9. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1 + (data[2] % 4);
    dec_cfg.allow_lowbitdepth = 1;
    
    res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0,
                                 AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        aom_img_free(&src_img);
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // 10. Set frame buffer functions
    aom_codec_set_frame_buffer_functions(&decoder,  id_000057_get_frame_buffer,
                                         id_000057_release_frame_buffer, nullptr);
    
    // 11. Decode the encoded data
    aom_codec_decode(&decoder, encoded_data.data(), encoded_data.size(), nullptr);
    
    // 12. Get stream info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_get_stream_info(&decoder, &si);
    
    // 13. Get decoded frame
    aom_codec_iter_t dec_iter = nullptr;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &dec_iter);
    
    // 14. Try to get preview frame (may return null)
    const aom_image_t *preview = aom_codec_get_preview_frame(&decoder);
    (void)preview; // May be null
    
    // 15. Test image wrapping with remaining input data
    if (size - data_offset > 100) {
        aom_image_t wrapped_img;
        unsigned char *img_data = (unsigned char*)malloc(enc_cfg.g_w * enc_cfg.g_h * 3 / 2);
        if (img_data) {
            size_t copy_size = (enc_cfg.g_w * enc_cfg.g_h * 3 / 2 < size - data_offset) ?
                              enc_cfg.g_w * enc_cfg.g_h * 3 / 2 : size - data_offset;
            memcpy(img_data, data + data_offset, copy_size);
            
            aom_img_wrap(&wrapped_img, AOM_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h,
                        16, img_data);
            
            // Test metadata functions on wrapped image
            if (data_offset + 50 < size) {
                aom_img_add_metadata(&wrapped_img, 2, data + data_offset + 30,
                                    20, AOM_MIF_KEY_FRAME);
                
                size_t meta_count = aom_img_num_metadata(&wrapped_img);
                if (meta_count > 0) {
                    const aom_metadata_t *meta = aom_img_get_metadata(&wrapped_img, 0);
                    (void)meta; // Check metadata
                    
                    // Test metadata removal
                    aom_img_remove_metadata(&wrapped_img);
                }
            }
            
            // Test metadata allocation and free
            if (data_offset + 100 < size) {
                aom_metadata_t *custom_meta = aom_img_metadata_alloc(
                    3, data + data_offset + 80, 20, AOM_MIF_ANY_FRAME);
                if (custom_meta) {
                    aom_img_metadata_free(custom_meta);
                }
            }
            
            free(img_data);
        }
    }
    
    // 16. Test metadata functions on source image
    size_t meta_count = aom_img_num_metadata(&src_img);
    for (size_t i = 0; i < meta_count; ++i) {
        const aom_metadata_t *meta = aom_img_get_metadata(&src_img, i);
        (void)meta; // Access metadata
    }
    
    // 17. Get error details if any
    const char *error_detail = aom_codec_error_detail(&encoder);
    (void)error_detail;
    error_detail = aom_codec_error_detail(&decoder);
    (void)error_detail;
    
    // 18. Cleanup
    aom_img_remove_metadata(&src_img);
    aom_img_free(&src_img);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    
    return 0;
}