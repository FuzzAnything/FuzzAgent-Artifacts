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
//<ID> 141
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["vpx_img_alloc","vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_img_free","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_img_free","vpx_codec_set_cx_data_buf","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_control_","vpx_codec_control_","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_img_alloc","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_control_","vpx_codec_control_","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_slice
static void  id_000040_put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const vpx_image_rect_t *rect, 
                         const vpx_image_rect_t *rect2) {
    // Empty callback - just to demonstrate usage
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* for input using fmemopen as instructed
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE* for output as instructed
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used
    
    // Allocate a dummy image to demonstrate vpx_img_free
    vpx_image_t *dummy_img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 16, 16, 1);
    
    // Try to decode with both VP8 and VP9 decoders
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_stream_info_t si;
    const vpx_codec_iface_t *decoder_iface = nullptr;
    const vpx_codec_iface_t *encoder_iface = nullptr;
    
    // Determine which decoder to use based on first byte
    if (data[0] % 2 == 0) {
        decoder_iface = vpx_codec_vp8_dx();
    } else {
        decoder_iface = vpx_codec_vp9_dx();
    }
    
    // Initialize decoder
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 640;
    dec_cfg.h = 480;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, 
                                                &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(dummy_img);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Register put slice callback
    res = vpx_codec_register_put_slice_cb(&decoder_ctx,  id_000040_put_slice_cb, nullptr);
    
    // Try to decode the input data
    res = vpx_codec_decode(&decoder_ctx, data, size, nullptr, 0);
    
    // Initialize encoder (VP9)
    encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t enc_cfg;
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    // Set encoder configuration
    enc_cfg.g_w = 640;
    enc_cfg.g_h = 480;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_threads = 1;
    
    // Initialize encoder
    res = vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 
                                0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder_ctx);
        vpx_img_free(dummy_img);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set up buffer for encoded data
    const size_t buf_size = 4096;
    std::vector<uint8_t> enc_buf(buf_size);
    vpx_fixed_buf_t fixed_buf;
    fixed_buf.buf = enc_buf.data();
    fixed_buf.sz = buf_size;
    
    res = vpx_codec_set_cx_data_buf(&encoder_ctx, &fixed_buf, 0, 0);
    
    // Try to get decoded frames
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *img;
    int frame_count = 0;
    
    while ((img = vpx_codec_get_frame(&decoder_ctx, &iter)) != nullptr && frame_count < 1) {
        // Try to encode the decoded frame
        res = vpx_codec_encode(&encoder_ctx, img, frame_count, 1, 0, 0);
        
        // Get encoded data
        vpx_codec_iter_t enc_iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt;
        
        while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Write to output file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
        
        frame_count++;
    }
    
    // Flush encoder
    res = vpx_codec_encode(&encoder_ctx, nullptr, frame_count, 1, 0, 0);
    
    // Get any remaining encoded data
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Try to use control API
    int ctrl_id = 0;
    vpx_codec_control_(&decoder_ctx, ctrl_id);
    vpx_codec_control_(&encoder_ctx, ctrl_id);
    
    // Cleanup
    vpx_codec_destroy(&decoder_ctx);
    vpx_codec_destroy(&encoder_ctx);
    vpx_img_free(dummy_img);
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}