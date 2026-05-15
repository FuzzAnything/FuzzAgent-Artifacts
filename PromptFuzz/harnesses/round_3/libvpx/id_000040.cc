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
//<ID> 133
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 0
//<Quality> {"density":33,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_img_wrap","vpx_img_set_rect","vpx_img_flip","vpx_codec_control_","vpx_codec_control_","vpx_codec_encode","vpx_codec_set_cx_data_buf","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_get_global_headers","vpx_codec_error_detail","vpx_codec_err_to_string","vpx_codec_destroy","vpx_img_free","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_img_wrap","vpx_img_set_rect","vpx_img_flip","vpx_codec_control_","vpx_codec_control_","vpx_codec_encode","vpx_codec_set_cx_data_buf","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_get_global_headers","vpx_codec_error_detail","vpx_codec_err_to_string","vpx_codec_destroy","vpx_img_free","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for decoded frames
static void  id_000040_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Do nothing in fuzzing context
    (void)user_priv;
    (void)img;
}

// Frame buffer callback functions (dummy implementations)
static int  id_000040_get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)min_size;
    (void)fb;
    return -1;
}

static int  id_000040_release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)fb;
    return -1;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<92) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_15, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Use FILE* for input as required
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file as required
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors as required
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;
    (void)out_fd;
    
    // Initialize variables
    vpx_codec_ctx_t decoder;
    vpx_codec_ctx_t encoder;
    vpx_codec_stream_info_t stream_info;
    vpx_codec_enc_cfg_t encoder_config;
    vpx_codec_dec_cfg_t decoder_config = {0};
    vpx_image_t *img = nullptr;
    vpx_image_t *wrapped_img = nullptr;
    vpx_fixed_buf_t cx_buf;
    vpx_codec_iter_t iter = nullptr;
    vpx_codec_caps_t caps;
    vpx_rational_t dsf = {1, 1};
    
    std::vector<uint8_t> buffer;
    vpx_codec_err_t res;
    
    // Get decoder interface
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    // Get interface names
    const char *decoder_name = vpx_codec_iface_name(decoder_iface);
    const char *encoder_name = vpx_codec_iface_name(encoder_iface);
    (void)decoder_name;
    (void)encoder_name;
    
    // Get capabilities
    caps = vpx_codec_get_caps(decoder_iface);
    (void)caps;
    
    // Peek stream info
    stream_info.sz = sizeof(stream_info);
    res = vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    if (res != VPX_CODEC_OK) {
        // If we can't peek stream info, try default dimensions
        stream_info.w = 320;
        stream_info.h = 240;
    }
    
    // Initialize decoder
    res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &decoder_config, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Register put frame callback
    res = vpx_codec_register_put_frame_cb(&decoder,  id_000040_put_frame_callback, nullptr);
    
    // Set frame buffer functions
    res = vpx_codec_set_frame_buffer_functions(&decoder,  id_000040_get_frame_buffer_cb, 
                                                id_000040_release_frame_buffer_cb, nullptr);
    
    // Decode the input data
    res = vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2);
    
    // Get stream info after decoding
    res = vpx_codec_get_stream_info(&decoder, &stream_info);
    
    // Get decoded frames
    const vpx_image_t *decoded_frame;
    while ((decoded_frame = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Create a copy of the decoded frame for re-encoding
        if (!img) {
            img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, decoded_frame->d_w, 
                               decoded_frame->d_h, fuzz_uint32_t_3);
            if (img) {
                // Copy the decoded frame data
                for (int plane = 0; plane < 3; plane++) {
                    uint8_t *dst = img->planes[plane];
                    const uint8_t *src = decoded_frame->planes[plane];
                    int dst_stride = img->stride[plane];
                    int src_stride = decoded_frame->stride[plane];
                    int height = (plane == 0) ? decoded_frame->d_h : decoded_frame->d_h / 2;
                    
                    for (int y = 0; y < height; y++) {
                        memcpy(dst, src, decoded_frame->d_w / (plane == 0 ? 1 : 2));
                        dst += dst_stride;
                        src += src_stride;
                    }
                }
            }
        }
    }
    
    // Re-encode if we have a decoded frame
    if (img) {
        // Get default encoder config
        res = vpx_codec_enc_config_default(encoder_iface, &encoder_config, fuzz_uint32_t_4);
        if (res == VPX_CODEC_OK) {
            encoder_config.g_w = img->d_w;
            encoder_config.g_h = img->d_h;
            encoder_config.g_timebase.num = 1;
            encoder_config.g_timebase.den = 30;
            encoder_config.rc_target_bitrate = 256;
            
            // Initialize encoder
            res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &encoder_config, fuzz_int64_t_5, 
                                         VPX_ENCODER_ABI_VERSION);
            
            // Try multi-encoder init
            vpx_codec_ctx_t multi_encoder;
            res = vpx_codec_enc_init_multi_ver(&multi_encoder, encoder_iface, &encoder_config,
                                              1, fuzz_int64_t_6, &dsf, VPX_ENCODER_ABI_VERSION);
            vpx_codec_destroy(&multi_encoder);
            
            // Set encoder config
            res = vpx_codec_enc_config_set(&encoder, &encoder_config);
            
            // Create a wrapped image
            size_t img_size = img->d_w * img->d_h * 3 / 2;
            buffer.resize(img_size);
            wrapped_img = vpx_img_wrap(nullptr, VPX_IMG_FMT_I420, img->d_w, img->d_h, 
                                       fuzz_uint32_t_7, buffer.data());
            
            // Set rectangle on wrapped image
            if (wrapped_img) {
                unsigned int rect_w = img->d_w > 16 ? 16 : img->d_w;
                unsigned int rect_h = img->d_h > 16 ? 16 : img->d_h;
                vpx_img_set_rect(wrapped_img, fuzz_uint32_t_8, fuzz_uint32_t_9, rect_w, rect_h);
                
                // Flip the wrapped image
                vpx_img_flip(wrapped_img);
            }
            
            // Set control parameters
            vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
            vpx_codec_control_(&encoder, VP8E_SET_NOISE_SENSITIVITY, 0);
            
            // Encode the image
            res = vpx_codec_encode(&encoder, img, fuzz_int64_t_10, fuzz_uint64_t_11, fuzz_int64_t_12, fuzz_uint64_t_13);
            
            // Set output buffer
            cx_buf.buf = malloc(4096);
            cx_buf.sz = 4096;
            if (cx_buf.buf) {
                res = vpx_codec_set_cx_data_buf(&encoder, &cx_buf, fuzz_uint32_t_14, fuzz_uint32_t_15);
            }
            
            // Get encoded data
            const vpx_codec_cx_pkt_t *pkt;
            iter = nullptr;
            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Write to output file
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
            
            // Try to get preview frame
            const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
            (void)preview;
            
            // Get global headers
            vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
            if (global_headers) {
                free(global_headers->buf);
            }
            
            // Get error details
            const char *error_detail = vpx_codec_error_detail(&encoder);
            (void)error_detail;
            
            // Convert error code to string
            const char *error_str = vpx_codec_err_to_string(res);
            (void)error_str;
            
            // Free output buffer
            if (cx_buf.buf) {
                free(cx_buf.buf);
            }
            
            // Destroy encoder
            vpx_codec_destroy(&encoder);
        }
        
        // Free images
        if (wrapped_img) {
            vpx_img_free(wrapped_img);
        }
        vpx_img_free(img);
    }
    
    // Destroy decoder
    vpx_codec_destroy(&decoder);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}