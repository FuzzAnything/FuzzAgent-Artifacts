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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_img_free","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_img_free","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_slice
static void  id_000028_put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const vpx_image_rect_t *slice_rect, 
                         const vpx_image_rect_t *frame_rect) {
    // No-op callback for fuzzing
    (void)user_priv;
    (void)img;
    (void)slice_rect;
    (void)frame_rect;
}

// Callback function for put_frame
static void  id_000028_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // No-op callback for fuzzing
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create FILE pointer for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    if (fd_in < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Try both VP8 and VP9 codecs
    vpx_codec_iface_t *decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    
    for (int i = 0; i < 2; i++) {
        vpx_codec_iface_t *iface = decoder_ifaces[i];
        
        // Get interface name
        const char *iface_name = vpx_codec_iface_name(iface);
        if (!iface_name) continue;
        
        // Peek stream info
        vpx_codec_stream_info_t si;
        memset(&si, 0, sizeof(si));
        si.sz = sizeof(si);
        
        vpx_codec_err_t err = vpx_codec_peek_stream_info(iface, data, 
                                                         (unsigned int)size, &si);
        if (err != VPX_CODEC_OK) continue;
        
        // Initialize decoder
        vpx_codec_ctx_t decoder;
        vpx_codec_dec_cfg_t dec_cfg;
        memset(&dec_cfg, 0, sizeof(dec_cfg));
        dec_cfg.threads = 1;
        
        err = vpx_codec_dec_init_ver(&decoder, iface, &dec_cfg, 0, 
                                    VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) continue;
        
        // Register callbacks
        vpx_codec_register_put_slice_cb(&decoder,  id_000028_put_slice_cb, nullptr);
        vpx_codec_register_put_frame_cb(&decoder,  id_000028_put_frame_cb, nullptr);
        
        // Decode the input
        err = vpx_codec_decode(&decoder, data, (unsigned int)size, 
                              nullptr, 0);
        
        // Get decoded frames
        vpx_codec_iter_t iter = nullptr;
        vpx_image_t *img;
        while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
            // Create a new vpx_image_t using vpx_img_wrap
            // We'll create a dummy image with the same dimensions
            if (img->w > 0 && img->h > 0) {
                unsigned int stride_align = 1;
                unsigned char *img_data = (unsigned char*)malloc(
                    img->w * img->h * 3 / 2); // For I420
                
                if (img_data) {
                    vpx_image_t wrapped_img;
                    vpx_img_wrap(&wrapped_img, VPX_IMG_FMT_I420, 
                                img->w, img->h, stride_align, img_data);
                    
                    // Clean up wrapped image
                    vpx_img_free(&wrapped_img);
                    free(img_data);
                }
            }
        }
        
        // Also create an encoder context to use encoding APIs
        vpx_codec_ctx_t encoder;
        vpx_codec_enc_cfg_t enc_cfg;
        
        if (i == 0) {  // VP8
            vpx_codec_iface_t *enc_iface = vpx_codec_vp8_cx();
            if (enc_iface) {
                err = vpx_codec_enc_config_default(enc_iface, &enc_cfg, 0);
                if (err == VPX_CODEC_OK) {
                    // Modify config based on stream info
                    enc_cfg.g_w = si.w > 0 ? si.w : 640;
                    enc_cfg.g_h = si.h > 0 ? si.h : 480;
                    
                    err = vpx_codec_enc_init_ver(&encoder, enc_iface, &enc_cfg, 
                                                0, VPX_ENCODER_ABI_VERSION);
                    if (err == VPX_CODEC_OK) {
                        // Set encoder configuration
                        vpx_codec_enc_config_set(&encoder, &enc_cfg);
                        
                        // Get global headers
                        vpx_fixed_buf_t *headers = vpx_codec_get_global_headers(&encoder);
                        if (headers && out_file) {
                            fwrite(headers->buf, 1, headers->sz, out_file);
                        }
                        
                        vpx_codec_destroy(&encoder);
                    }
                }
            }
        }
        
        vpx_codec_destroy(&decoder);
    }
    
    // Clean up
    if (out_file) assert_file_closed(&out_file);;
    if (in_file) assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}