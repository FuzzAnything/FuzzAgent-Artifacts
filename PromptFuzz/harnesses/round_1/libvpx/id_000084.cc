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
//<ID> 323
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_img_wrap","vpx_img_set_rect","vpx_img_flip","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_img_free","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_img_wrap","vpx_img_set_rect","vpx_img_flip","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_img_free","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Forward declarations for callback functions
static int  id_000084_get_frame_buffer(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb);
static int  id_000084_release_frame_buffer(void *cb_priv, vpx_codec_frame_buffer_t *fb);
static void  id_000084_put_slice_callback(void *user_priv, const vpx_image_t *img,
                              const vpx_image_rect_t *slice, const vpx_image_rect_t *clip);

// Custom frame buffer structure
struct FrameBufferData {
    uint8_t *data;
    size_t size;
    int in_use;
};

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small to be meaningful
    
    // Initialize variables
    vpx_codec_ctx_t encoder = {0}, decoder = {0};
    vpx_codec_enc_cfg_t enc_cfg = {0};
    vpx_codec_stream_info_t stream_info = {0};
    vpx_image_t *img = nullptr, *preview_img = nullptr;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    vpx_fixed_buf_t *global_headers = nullptr;
    FrameBufferData frame_buffers[4] = {0};
    int frame_count = 0;
    
    // 1. Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output file for encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. First, try to decode the input as VP8 stream
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    
    // Get stream information
    stream_info.sz = sizeof(stream_info);
    vpx_codec_err_t err = vpx_codec_peek_stream_info(decoder_iface, data, (unsigned int)size, &stream_info);
    
    if (err == VPX_CODEC_OK) {
        // Initialize decoder
        vpx_codec_dec_cfg_t dec_cfg = {0};
        dec_cfg.threads = 2;
        dec_cfg.w = stream_info.w;
        dec_cfg.h = stream_info.h;
        
        err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
        
        if (err == VPX_CODEC_OK) {
            // Set up frame buffer callbacks
            err = vpx_codec_set_frame_buffer_functions(&decoder,  id_000084_get_frame_buffer,  id_000084_release_frame_buffer, frame_buffers);
            
            // Decode the data
            err = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, 0);
            
            // Get stream info from decoder context
            err = vpx_codec_get_stream_info(&decoder, &stream_info);
            
            // Get decoded frames
            while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
                frame_count++;
                
                // 4. Now encode the decoded frame
                vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
                
                // Get default encoder configuration
                err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
                
                if (err == VPX_CODEC_OK) {
                    // Set encoder configuration
                    enc_cfg.g_w = img->d_w;
                    enc_cfg.g_h = img->d_h;
                    enc_cfg.g_timebase.num = 1;
                    enc_cfg.g_timebase.den = 30;
                    enc_cfg.rc_target_bitrate = 1000;
                    enc_cfg.g_error_resilient = 1;
                    
                    // Initialize encoder
                    err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
                    
                    if (err == VPX_CODEC_OK) {
                        // Set encoder configuration (again to ensure it's applied)
                        err = vpx_codec_enc_config_set(&encoder, &enc_cfg);
                        
                        // Register put slice callback
                        err = vpx_codec_register_put_slice_cb(&encoder,  id_000084_put_slice_callback, nullptr);
                        
                        // Create a copy of the image for encoding
                        vpx_image_t *enc_img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                                           img->d_w, img->d_h, 16);
                        
                        if (enc_img) {
                            // Copy image data (simplified - in real code would need proper conversion)
                            if (img->fmt == VPX_IMG_FMT_I420) {
                                for (int plane = 0; plane < 3; plane++) {
                                    int src_stride = img->stride[plane];
                                    int dst_stride = enc_img->stride[plane];
                                    int height = (plane == 0) ? img->d_h : img->d_h / 2;
                                    int width = (plane == 0) ? img->d_w : img->d_w / 2;
                                    
                                    for (int y = 0; y < height; y++) {
                                        memcpy(enc_img->planes[plane] + y * dst_stride,
                                               img->planes[plane] + y * src_stride,
                                               width);
                                    }
                                }
                            }
                            
                            // Also test vpx_img_wrap
                            unsigned char *wrapped_data = (unsigned char *)malloc(img->d_w * img->d_h * 3 / 2);
                            vpx_image_t *wrapped_img = vpx_img_wrap(nullptr, VPX_IMG_FMT_I420, 
                                                                   img->d_w, img->d_h, 16, wrapped_data);
                            
                            // Test vpx_img_set_rect
                            if (wrapped_img) {
                                vpx_img_set_rect(wrapped_img, 0, 0, img->d_w / 2, img->d_h / 2);
                                vpx_img_flip(wrapped_img);
                            }
                            
                            // Encode the image
                            err = vpx_codec_encode(&encoder, enc_img, frame_count, 1, 0, VPX_DL_REALTIME);
                            
                            // Get global headers
                            global_headers = vpx_codec_get_global_headers(&encoder);
                            if (global_headers && global_headers->buf) {
                                fwrite(global_headers->buf, 1, global_headers->sz, out_file);
                            }
                            
                            // Get encoded data
                            iter = nullptr;
                            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                                }
                            }
                            
                            // Get preview frame
                            preview_img = (vpx_image_t *)vpx_codec_get_preview_frame(&encoder);
                            
                            // Clean up images
                            vpx_img_free(enc_img);
                            if (wrapped_img) {
                                vpx_img_free(wrapped_img);
                                free(wrapped_data);
                            }
                        }
                        
                        // Get error detail
                        const char *error_detail = vpx_codec_error_detail(&encoder);
                        (void)error_detail; // Suppress unused warning
                        
                        // Destroy encoder
                        vpx_codec_destroy(&encoder);
                    }
                }
                
                // Break after processing one frame to avoid infinite loops
                if (frame_count >= 1) break;
            }
            
            // Destroy decoder
            vpx_codec_destroy(&decoder);
        }
    }
    
    // Clean up resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Free any allocated frame buffers
    for (int i = 0; i < 4; i++) {
        if (frame_buffers[i].data) {
            free(frame_buffers[i].data);
        }
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}

// Frame buffer callback implementations
static int  id_000084_get_frame_buffer(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    FrameBufferData *bufs = (FrameBufferData *)cb_priv;
    
    for (int i = 0; i < 4; i++) {
        if (!bufs[i].in_use) {
            if (!bufs[i].data || bufs[i].size < min_size) {
                free(bufs[i].data);
                bufs[i].data = (uint8_t *)malloc(min_size);
                bufs[i].size = min_size;
                if (!bufs[i].data) return -1;
            }
            bufs[i].in_use = 1;
            fb->data = bufs[i].data;
            fb->size = bufs[i].size;
            return 0;
        }
    }
    return -1;
}

static int  id_000084_release_frame_buffer(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    FrameBufferData *bufs = (FrameBufferData *)cb_priv;
    
    for (int i = 0; i < 4; i++) {
        if (bufs[i].data == fb->data) {
            bufs[i].in_use = 0;
            return 0;
        }
    }
    return -1;
}

static void  id_000084_put_slice_callback(void *user_priv, const vpx_image_t *img,
                              const vpx_image_rect_t *slice, const vpx_image_rect_t *clip) {
    // Simple callback implementation
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)clip;
}