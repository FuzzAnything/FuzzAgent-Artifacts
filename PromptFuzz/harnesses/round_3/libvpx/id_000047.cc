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
//<ID> 197
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 1
//<Quality> {"density":17,"unique_branches":{"vpx_codec_set_cx_data_buf":[[298,15,298,25,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_error_detail","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include necessary headers for libvpx
extern "C" {
}

// Callback function for put_frame
static void  id_000047_dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // Do nothing, just satisfy the API requirement
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<208) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_15, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_16, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_17, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_18, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_19, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_20, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_21, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_22, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_23, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_24, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_25, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_26, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_27, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_28, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_29, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_30, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_31, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_32, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_33, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_34, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_35, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors if needed (though not used directly in this example)
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as unused
    (void)out_fd; // Mark as unused
    
    // Variables for error handling
    vpx_codec_err_t err;
    const char *err_str;
    
    // 1. First, try to decode the input as VP9 video
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_iface_t *vp8_encoder_iface = vpx_codec_vp8_cx();
    
    // Get capabilities of the decoder
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps; // Mark as unused
    
    // Peek stream info to get dimensions
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    err = vpx_codec_peek_stream_info(decoder_iface, data, (unsigned int)size, &si);
    if (err != VPX_CODEC_OK) {
        err_str = vpx_codec_err_to_string(err);
        (void)err_str; // Mark as unused
        // Try with a minimal valid stream info instead
        si.w = 320;
        si.h = 240;
    }
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.w = si.w;
    dec_cfg.h = si.h;
    
    err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Try to get error details
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail; // Mark as unused
        
        // Cleanup and try encoding path instead
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        
        // Try encoding path with minimal setup
        in_file = fmemopen((void *)data, size, "rb");
        out_file = fopen("output_file", "wb");
        if (!in_file || !out_file) {
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
	}
	
        
        // Initialize VP8 encoder with minimal config
        vpx_codec_ctx_t vp8_encoder;
        vpx_codec_enc_cfg_t vp8_cfg;
        err = vpx_codec_enc_config_default(vp8_encoder_iface, &vp8_cfg, fuzz_uint32_t_2);
        if (err == VPX_CODEC_OK) {
            vp8_cfg.g_w = 320;
            vp8_cfg.g_h = 240;
            vp8_cfg.g_timebase.num = 1;
            vp8_cfg.g_timebase.den = 30;
            
            err = vpx_codec_enc_init_ver(&vp8_encoder, vp8_encoder_iface, &vp8_cfg, fuzz_int64_t_3, VPX_ENCODER_ABI_VERSION);
            if (err == VPX_CODEC_OK) {
                // Create a dummy image
                vpx_image_t img;
                vpx_img_alloc(&img, VPX_IMG_FMT_I420, 320, 240, fuzz_uint32_t_4);
                
                // Set a rectangle on the image
                vpx_img_set_rect(&img, fuzz_uint32_t_5, fuzz_uint32_t_6, fuzz_uint32_t_7, fuzz_uint32_t_8);
                
                // Encode the dummy image
                err = vpx_codec_encode(&vp8_encoder, &img, fuzz_int64_t_9, fuzz_uint64_t_10, fuzz_int64_t_11, fuzz_uint64_t_12);
                
                // Get encoded data
                vpx_codec_iter_t iter = NULL;
                const vpx_codec_cx_pkt_t *pkt;
                while ((pkt = vpx_codec_get_cx_data(&vp8_encoder, &iter)) != NULL) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                // Clean up
                vpx_img_free(&img);
                vpx_codec_destroy(&vp8_encoder);
            }
        }
        
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Register put frame callback
    err = vpx_codec_register_put_frame_cb(&decoder,  id_000047_dummy_put_frame_cb, NULL);
    
    // Decode the input data
    err = vpx_codec_decode(&decoder, data, (unsigned int)size, NULL, fuzz_int64_t_13);
    
    // Get stream info after decoding
    err = vpx_codec_get_stream_info(&decoder, &si);
    
    // Get decoded frames
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *decoded_img;
    std::vector<vpx_image_t> decoded_frames;
    
    while ((decoded_img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
        // Make a copy of the image info (not the actual data)
        decoded_frames.push_back(*decoded_img);
        
        // Break after a few frames to limit resource usage
        if (decoded_frames.size() >= 3) break;
    }
    
    // 2. Now try to encode the frames (or create new ones if decoding failed)
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    
    err = vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_14);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Configure encoder
    if (!decoded_frames.empty()) {
        cfg.g_w = decoded_frames[0].d_w;
        cfg.g_h = decoded_frames[0].d_h;
    } else {
        cfg.g_w = 320;
        cfg.g_h = 240;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    
    // Try multi-encoder init
    vpx_rational_t dsf = {1, 1};
    err = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &cfg, 1, fuzz_int64_t_15, &dsf, VPX_ENCODER_ABI_VERSION);
    
    // Also try regular init (for VP8)
    vpx_codec_ctx_t vp8_encoder;
    vpx_codec_enc_cfg_t vp8_cfg;
    vpx_codec_enc_config_default(vp8_encoder_iface, &vp8_cfg, fuzz_uint32_t_16);
    vpx_codec_enc_init_ver(&vp8_encoder, vp8_encoder_iface, &vp8_cfg, fuzz_int64_t_17, VPX_ENCODER_ABI_VERSION);
    
    // Set encoder config
    vpx_codec_enc_config_set(&encoder, &cfg);
    
    // Prepare output buffer
    vpx_fixed_buf_t output_buf;
    output_buf.buf = malloc(1024 * 1024); // 1MB buffer
    output_buf.sz = 1024 * 1024;
    
    if (output_buf.buf) {
        err = vpx_codec_set_cx_data_buf(&encoder, &output_buf, fuzz_uint32_t_18, fuzz_uint32_t_19);
        
        // Encode frames
        if (!decoded_frames.empty()) {
            for (size_t i = 0; i < decoded_frames.size(); i++) {
                // Create a new image to use vpx_img_alloc
                vpx_image_t img_to_encode;
                vpx_img_alloc(&img_to_encode, VPX_IMG_FMT_I420, 
                             decoded_frames[i].d_w, decoded_frames[i].d_h, fuzz_uint32_t_20);
                
                // Set rectangle on the image
                vpx_img_set_rect(&img_to_encode, fuzz_uint32_t_21, fuzz_uint32_t_22, 
                                decoded_frames[i].d_w, decoded_frames[i].d_h);
                
                // Encode the image
                err = vpx_codec_encode(&encoder, &img_to_encode, fuzz_int64_t_23, fuzz_uint64_t_24, fuzz_int64_t_25, fuzz_uint64_t_26);
                
                // Get encoded data
                vpx_codec_iter_t enc_iter = NULL;
                const vpx_codec_cx_pkt_t *pkt;
                while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                vpx_img_free(&img_to_encode);
            }
        } else {
            // Create and encode a dummy frame
            vpx_image_t dummy_img;
            vpx_img_alloc(&dummy_img, VPX_IMG_FMT_I420, 320, 240, fuzz_uint32_t_27);
            vpx_img_set_rect(&dummy_img, fuzz_uint32_t_28, fuzz_uint32_t_29, fuzz_uint32_t_30, fuzz_uint32_t_31);
            
            err = vpx_codec_encode(&encoder, &dummy_img, fuzz_int64_t_32, fuzz_uint64_t_33, fuzz_int64_t_34, fuzz_uint64_t_35);
            
            vpx_codec_iter_t enc_iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
            
            vpx_img_free(&dummy_img);
        }
        
        free(output_buf.buf);
    }
    
    // Get error detail for the encoder
    const char *encoder_error_detail = vpx_codec_error_detail(&encoder);
    (void)encoder_error_detail; // Mark as unused
    
    // Clean up
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&vp8_encoder);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}