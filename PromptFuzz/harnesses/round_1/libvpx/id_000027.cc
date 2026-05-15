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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 0
//<Quality> {"density":32,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_control_","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_img_wrap","vpx_img_free","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_encode","vpx_img_free","vpx_img_free","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_img_free","vpx_img_free","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_control_","vpx_img_alloc","vpx_img_wrap","vpx_img_set_rect","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_img_free","vpx_img_free","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to implement a complete video encoding/decoding pipeline:

*/


extern "C" {
}

// Callback functions
static void  id_000027_put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* rect,
                               const vpx_image_rect_t* rect2) {
    // Callback for slice processing - minimal implementation
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

static int  id_000027_get_frame_buffer_callback(void* cb_priv, size_t min_size,
                                     vpx_codec_frame_buffer_t* fb) {
    // Minimal implementation - allocate buffer
    fb->data = (uint8_t*)malloc(min_size);
    fb->size = min_size;
    fb->priv = NULL;
    return 0;
}

static int  id_000027_release_frame_buffer_callback(void* cb_priv,
                                         vpx_codec_frame_buffer_t* fb) {
    // Release allocated buffer
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 128) {
        return 0;  // Need minimum data
    }

    vpx_codec_ctx_t encoder;
    vpx_codec_ctx_t decoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_dec_cfg_t dec_cfg;
    vpx_image_t* img = nullptr;
    vpx_image_t* wrapped_img = nullptr;
    FILE* in_file = nullptr;
    const vpx_codec_cx_pkt_t* pkt = nullptr;
    vpx_codec_iter_t iter = nullptr;
    
    // Open input file from memory buffer
    in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Get VP8 encoder interface
    vpx_codec_iface_t* codec_iface = vpx_codec_vp8_cx();
    
    // 2. Get codec interface name
    const char* iface_name = vpx_codec_iface_name(codec_iface);
    (void)iface_name; // Prevent unused variable warning
    
    // 3. Get default encoder configuration
    unsigned int usage = 0; // Good quality
    if (vpx_codec_enc_config_default(codec_iface, &cfg, usage) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Configure encoder
    cfg.g_w = 64;  // Small width for fuzzing
    cfg.g_h = 64;  // Small height for fuzzing
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;  // Low bitrate for fuzzing
    
    // 4. Initialize encoder
    if (vpx_codec_enc_init_ver(&encoder, codec_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Set encoder configuration
    if (vpx_codec_enc_config_set(&encoder, &cfg) != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Set encoder control (set CPU usage to maximum speed)
    int ctrl_id = VP8E_SET_CPUUSED;
    int value = 8;  // Maximum speed
    vpx_codec_control_(&encoder, ctrl_id, value);
    
    // Create raw image buffer from input data
    unsigned int width = 64;
    unsigned int height = 64;
    unsigned int align = 1;
    
    // Calculate required size for I420 format
    size_t y_size = width * height;
    size_t uv_size = (width / 2) * (height / 2);
    size_t total_size = y_size + 2 * uv_size;
    
    uint8_t* img_data = nullptr;
    if (total_size <= size) {
        img_data = (uint8_t*)data;
    } else {
        // Allocate and fill with pattern if input is too small
        img_data = (uint8_t*)malloc(total_size);
        if (img_data) {
            size_t copy_size = size < total_size ? size : total_size;
            memcpy(img_data, data, copy_size);
            // Fill remainder with pattern
            if (copy_size < total_size) {
                for (size_t i = copy_size; i < total_size; i++) {
                    img_data[i] = i % 256;
                }
            }
        } else {
            vpx_codec_destroy(&encoder);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // 7. Allocate image
    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, align);
    if (!img) {
        if (img_data && img_data != data) {
            free(img_data);
        }
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 8. Wrap image data
    wrapped_img = vpx_img_wrap(NULL, VPX_IMG_FMT_I420, width, height, align, img_data);
    if (!wrapped_img) {
        vpx_img_free(img);
        if (img_data && img_data != data) {
            free(img_data);
        }
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 9. Set image rectangle (use entire image)
    vpx_img_set_rect(img, 0, 0, width, height);
    
    // 10. Register put slice callback
    vpx_codec_register_put_slice_cb(&encoder,  id_000027_put_slice_callback, NULL);
    
    // 11. Set frame buffer functions for encoder (optional)
    vpx_codec_set_frame_buffer_functions(&encoder,  id_000027_get_frame_buffer_callback,
                                         id_000027_release_frame_buffer_callback, NULL);
    
    // 12. Encode the image
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
    
    if (vpx_codec_encode(&encoder, img, pts, duration, flags, deadline) != VPX_CODEC_OK) {
        vpx_img_free(img);
        vpx_img_free(wrapped_img);
        if (img_data && img_data != data) {
            free(img_data);
        }
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 13. Get global headers
    vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&encoder);
    (void)global_headers; // Prevent unused variable warning
    
    // 14. Get encoded data
    iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        // Process packet if needed
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Found a frame packet
            break;
        }
    }
    
    // 15. Free images
    vpx_img_free(img);
    vpx_img_free(wrapped_img);
    
    // 16. Initialize decoder
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp8_dx();
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.w = width;
    dec_cfg.h = height;
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        if (img_data && img_data != data) {
            free(img_data);
        }
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set frame buffer functions for decoder
    vpx_codec_set_frame_buffer_functions(&decoder,  id_000027_get_frame_buffer_callback,
                                         id_000027_release_frame_buffer_callback, NULL);
    
    // Try to decode if we have packet data
    if (pkt && pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
        vpx_codec_decode(&decoder, (const uint8_t*)pkt->data.frame.buf,
                        pkt->data.frame.sz, NULL, 0);
        
        // Get decoded frame
        vpx_codec_iter_t dec_iter = nullptr;
        const vpx_image_t* dec_img = vpx_codec_get_frame(&decoder, &dec_iter);
        (void)dec_img; // Prevent unused variable warning
    }
    
    // 17. Clean up
    if (img_data && img_data != data) {
        free(img_data);
    }
    
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}