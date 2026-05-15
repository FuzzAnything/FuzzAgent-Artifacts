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
//<ID> 135
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 0
//<Quality> {"density":30,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_error","aom_codec_destroy","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_cx_data","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_cx_data","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data
    
    // 1. Get version string (API used)
    const char* version_str = aom_codec_version_str();
    (void)version_str; // Prevent unused warning
    
    // 2. Decode ULEB value from input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (size > 0) {
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    }
    
    // 3. Initialize AV1 decoder
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    const char* decoder_name = aom_codec_iface_name(decoder_iface);
    (void)decoder_name;
    
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // 4. Peek stream info from input data
    aom_codec_stream_info_t stream_info = {0};
    if (size > 0) {
        aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    }
    
    // 5. Decode the input data
    res = aom_codec_decode(&decoder_ctx, data, size, NULL);
    if (res != AOM_CODEC_OK) {
        const char* error_detail = aom_codec_error(&decoder_ctx);
        (void)error_detail;
        aom_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // 6. Get stream info from decoder context
    aom_codec_get_stream_info(&decoder_ctx, &stream_info);
    
    // 7. Get decoded frame
    aom_codec_iter_t iter = NULL;
    aom_image_t* decoded_img = aom_codec_get_frame(&decoder_ctx, &iter);
    
    // 8. If we got a decoded frame, process it
    if (decoded_img) {
        // Get plane dimensions (APIs used)
        int plane_width = aom_img_plane_width(decoded_img, 0);
        int plane_height = aom_img_plane_height(decoded_img, 0);
        (void)plane_width;
        (void)plane_height;
        
        // Add metadata to the image
        uint8_t metadata_data[] = {0x01, 0x02, 0x03};
        aom_img_add_metadata(decoded_img, 1, metadata_data, 3, AOM_MIF_ANY_FRAME);
        
        // Get number of metadata (API used)
        size_t num_metadata = aom_img_num_metadata(decoded_img);
        (void)num_metadata;
        
        // Remove metadata (API used)
        aom_img_remove_metadata(decoded_img);
        
        // 9. Initialize AV1 encoder
        aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
        const char* encoder_name = aom_codec_iface_name(encoder_iface);
        (void)encoder_name;
        
        aom_codec_ctx_t encoder_ctx;
        aom_codec_enc_cfg_t enc_cfg;
        
        // Get default encoder configuration
        aom_codec_err_t enc_res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        if (enc_res != AOM_CODEC_OK) {
            aom_codec_destroy(&decoder_ctx);
            return 0;
        }
        
        // Set some basic encoder parameters
        enc_cfg.g_w = decoded_img->d_w;
        enc_cfg.g_h = decoded_img->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.g_threads = 1;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_error_resilient = 0;
        
        // Initialize encoder
        enc_res = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
        if (enc_res != AOM_CODEC_OK) {
            aom_codec_destroy(&decoder_ctx);
            return 0;
        }
        
        // 10. Allocate a new image using aom_img_alloc (API used)
        aom_image_t* allocated_img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 
                                                  decoded_img->d_w, 
                                                  decoded_img->d_h, 
                                                  16);
        
        if (allocated_img) {
            // Copy data from decoded image to allocated image (simplified)
            if (decoded_img->fmt == AOM_IMG_FMT_I420 && allocated_img->fmt == AOM_IMG_FMT_I420) {
                for (int plane = 0; plane < 3; plane++) {
                    int src_stride = decoded_img->stride[plane];
                    int dst_stride = allocated_img->stride[plane];
                    int plane_height = aom_img_plane_height(decoded_img, plane);
                    int copy_width = aom_img_plane_width(decoded_img, plane);
                    
                    if (copy_width > 0 && plane_height > 0) {
                        for (int y = 0; y < plane_height; y++) {
                            memcpy(allocated_img->planes[plane] + y * dst_stride,
                                  decoded_img->planes[plane] + y * src_stride,
                                  copy_width);
                        }
                    }
                }
            }
            
            // 11. Encode the allocated image (API used)
            aom_codec_pts_t pts = 0;
            unsigned long duration = 1;
            aom_enc_frame_flags_t flags = 0;
            
            enc_res = aom_codec_encode(&encoder_ctx, allocated_img, pts, duration, flags);
            
            if (enc_res == AOM_CODEC_OK) {
                // 12. Get encoded data (API used)
                aom_codec_iter_t enc_iter = NULL;
                const aom_codec_cx_pkt_t* pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter);
                
                // Process packets if any
                while (pkt != NULL) {
                    // Do something with the packet data
                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                        // Could write to file if needed
                    }
                    pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter);
                }
            }
            
            // 13. Free the allocated image (API used)
            aom_img_free(allocated_img);
        }
        
        // 14. Clean up encoder (API used indirectly through aom_codec_destroy)
        aom_codec_destroy(&encoder_ctx);
    }
    
    // 15. Clean up decoder (API used)
    aom_codec_destroy(&decoder_ctx);
    
    return 0;
}