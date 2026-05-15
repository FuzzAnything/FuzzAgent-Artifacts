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
#include <vpx/vp8cx.h>
}
extern "C" {
#include <vpx/vp8dx.h>
}
extern "C" {
#include <vpx/vpx_decoder.h>
}
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 6
//<Quality> {"density":9,"unique_branches":{"encoder_encode":[[1413,9,1413,29,0,0,4,1],[1413,33,1413,58,0,0,4,1],[1432,9,1432,60,0,0,4,1],[1490,9,1490,37,0,0,4,1]],"update_initial_width":[[2353,7,2353,41,0,0,4,1],[2354,7,2354,41,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback structures
struct FrameBufferData {
    uint8_t* data;
    size_t size;
};

static int  id_000022_get_frame_buffer(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    FrameBufferData* fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    // Allocate or reallocate buffer if needed
    if (fb_data->size < min_size) {
        free(fb_data->data);
        fb_data->data = static_cast<uint8_t*>(calloc(min_size, 1));
        if (!fb_data->data) return -1;
        fb_data->size = min_size;
    }
    
    fb->data = fb_data->data;
    fb->size = fb_data->size;
    return 0;
}

static int  id_000022_release_frame_buffer(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    // For fuzzing, we don't actually release the buffer between frames
    // to avoid reallocation overhead
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small to be meaningful
    
    // Open input as a file stream (required by some libvpx patterns)
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx(); // Try VP9 first
    
    // Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    
    // Setup frame buffer callbacks
    FrameBufferData frame_buffer_data = {nullptr, 0};
    
    // Initialize decoder configuration
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 2;
    
    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Try VP8 if VP9 fails
        decoder_iface = vpx_codec_vp8_dx();
        err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Set frame buffer functions
    err = vpx_codec_set_frame_buffer_functions(&decoder,  id_000022_get_frame_buffer, 
                                                id_000022_release_frame_buffer, &frame_buffer_data);
    
    // Decode the input data
    err = vpx_codec_decode(&decoder, data, size, nullptr, 0);
    
    // Get stream information
    vpx_codec_stream_info_t stream_info = {0};
    stream_info.sz = sizeof(stream_info);
    vpx_codec_err_t si_err = vpx_codec_get_stream_info(&decoder, &stream_info);
    
    // Initialize encoder context
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx(); // Use VP9 for encoding
    
    // Setup encoder configuration
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    // Set dimensions from stream info or defaults
    if (si_err == VPX_CODEC_OK && stream_info.w > 0 && stream_info.h > 0) {
        enc_cfg.g_w = stream_info.w;
        enc_cfg.g_h = stream_info.h;
    } else {
        enc_cfg.g_w = 640;
        enc_cfg.g_h = 480;
    }
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_threads = 2;
    
    // Initialize encoder
    err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    // Create an image for encoding
    vpx_image_t* img = nullptr;
    if (err == VPX_CODEC_OK) {
        // Allocate image
        img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 16);
        
        if (img) {
            // Fill image with synthetic data for fuzzing
            // Use parts of input data to fill YUV planes
            size_t y_size = enc_cfg.g_w * enc_cfg.g_h;
            size_t uv_size = y_size / 4;
            
            // Fill Y plane
            size_t copy_size = std::min(y_size, size);
            if (copy_size > 0) {
                memcpy(img->planes[VPX_PLANE_Y], data, copy_size);
            }
            
            // Fill U and V planes with patterns
            if (uv_size > 0) {
                size_t offset = copy_size % size;
                size_t uv_copy = std::min(uv_size, size - offset);
                if (uv_copy > 0) {
                    memcpy(img->planes[VPX_PLANE_U], data + offset, uv_copy);
                    offset = (offset + uv_copy) % size;
                    uv_copy = std::min(uv_size, size - offset);
                    if (uv_copy > 0) {
                        memcpy(img->planes[VPX_PLANE_V], data + offset, uv_copy);
                    }
                }
            }
            
            // Encode the image
            err = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
            
            if (err == VPX_CODEC_OK) {
                // Get encoded data
                vpx_codec_iter_t iter = nullptr;
                const vpx_codec_cx_pkt_t* pkt;
                
                while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                    // Process the packet if needed
                    // For fuzzing, we just ensure the API is called
                }
            }
            
            // Encode a few more frames with different parameters
            for (int i = 0; i < 3 && err == VPX_CODEC_OK; i++) {
                // Modify image data slightly
                if (img->planes[VPX_PLANE_Y] && y_size > 0) {
                    size_t offset = (i * 100) % size;
                    size_t mod_size = std::min(y_size, size - offset);
                    if (mod_size > 0) {
                        uint8_t* y_plane = static_cast<uint8_t*>(img->planes[VPX_PLANE_Y]);
                        for (size_t j = 0; j < mod_size; j++) {
                            y_plane[j] ^= data[(offset + j) % size];
                        }
                    }
                }
                
                err = vpx_codec_encode(&encoder, img, i + 1, 1, 
                                      (i == 2) ? VPX_EFLAG_FORCE_KF : 0, 
                                      VPX_DL_REALTIME);
                
                // Get encoded data
                if (err == VPX_CODEC_OK) {
                    vpx_codec_iter_t iter2 = nullptr;
                    const vpx_codec_cx_pkt_t* pkt2;
                    while ((pkt2 = vpx_codec_get_cx_data(&encoder, &iter2)) != nullptr) {
                        // Process packet
                    }
                }
            }
        }
    }
    
    // Cleanup
    if (img) {
        vpx_img_free(img);
    }
    
    if (vpx_codec_destroy(&encoder) != VPX_CODEC_OK) {
        // Handle error if needed
    }
    
    if (vpx_codec_destroy(&decoder) != VPX_CODEC_OK) {
        // Handle error if needed
    }
    
    if (frame_buffer_data.data) {
        free(frame_buffer_data.data);
    }
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}