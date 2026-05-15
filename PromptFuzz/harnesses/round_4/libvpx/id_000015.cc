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
//<ID> 264
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_get_global_headers","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callback structures
struct FrameBufferCallbackPriv {
    std::vector<uint8_t> buffer;
    bool in_use = false;
};

// Frame buffer get callback
static int  id_000015_get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    FrameBufferCallbackPriv *priv = static_cast<FrameBufferCallbackPriv*>(cb_priv);
    
    if (min_size > priv->buffer.size()) {
        priv->buffer.resize(min_size);
    }
    
    fb->data = priv->buffer.data();
    fb->size = priv->buffer.size();
    priv->in_use = true;
    return 0;
}

// Frame buffer release callback
static int  id_000015_release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    FrameBufferCallbackPriv *priv = static_cast<FrameBufferCallbackPriv*>(cb_priv);
    priv->in_use = false;
    return 0;
}

// Put slice callback
static void  id_000015_put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const vpx_image_rect_t *slice, const vpx_image_rect_t *updated) {
    // Callback implementation - can be empty for fuzzing
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)updated;
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<104) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_14, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_15, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Open input data as a file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors (though not strictly needed, following instructions)
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;
    (void)out_fd;
    
    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Get codec interface name
    const char *iface_name = vpx_codec_iface_name(decoder_iface);
    (void)iface_name;
    
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    // Initialize decoder
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION) 
        != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set up frame buffer callbacks
    FrameBufferCallbackPriv fb_priv;
    if (vpx_codec_set_frame_buffer_functions(&decoder, 
                                              id_000015_get_frame_buffer_cb,
                                              id_000015_release_frame_buffer_cb,
                                             &fb_priv) != VPX_CODEC_OK) {
        // Continue anyway, this is optional
    }
    
    // Register put slice callback
    if (vpx_codec_register_put_slice_cb(&decoder,  id_000015_put_slice_cb, nullptr) != VPX_CODEC_OK) {
        // Continue anyway, this is optional
    }
    
    // Decode the input data
    vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2);
    
    // Initialize encoder context for re-encoding
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
    
    // Set basic encoder configuration
    enc_cfg.g_w = 640;
    enc_cfg.g_h = 480;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_threads = 1;
    
    // Initialize multiple encoders (using multi-encoder init)
    vpx_rational_t dsf = {1, 1};
    if (vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &enc_cfg, 1, fuzz_int64_t_4, &dsf, 
                                     VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Get global headers from encoder
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }
    
    // Iterate through decoded frames
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_frame;
    
    while ((decoded_frame = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Wrap the decoded frame data for encoding
        vpx_image_t wrapped_img;
        unsigned int stride_align = 32;
        
        vpx_img_wrap(&wrapped_img, decoded_frame->fmt, decoded_frame->d_w, 
                    decoded_frame->d_h, fuzz_uint32_t_5, 
                    const_cast<unsigned char*>(decoded_frame->planes[0]));
        
        // Set a rectangle on the image (using half the image dimensions)
        unsigned int rect_x = 0;
        unsigned int rect_y = 0;
        unsigned int rect_w = decoded_frame->d_w / 2;
        unsigned int rect_h = decoded_frame->d_h / 2;
        
        if (rect_w > 0 && rect_h > 0) {
            vpx_img_set_rect(&wrapped_img, fuzz_uint32_t_6, fuzz_uint32_t_7, rect_w, rect_h);
        }
        
        // Encode the frame
        vpx_codec_encode(&encoder, &wrapped_img, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10, fuzz_uint64_t_11);
        
        // Get encoded data
        vpx_codec_iter_t enc_iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
    }
    
    // Flush encoder
    vpx_codec_encode(&encoder, nullptr, fuzz_int64_t_12, fuzz_uint64_t_13, fuzz_int64_t_14, fuzz_uint64_t_15);
    
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Clean up
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}