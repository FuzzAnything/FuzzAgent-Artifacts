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
//<ID> 225
//<Prompt> []
/*<Combination>: [
*/
//<score> 120, nr_unique_branch: 17
//<Quality> {"density":12,"unique_branches":{"vp9_compute_qdelta":[[2559,30,2559,51,0,0,4,1],[2565,30,2565,51,0,0,4,1]],"rc_pick_q_and_bounds_one_pass_vbr":[[1294,18,1294,46,0,0,4,1]],"set_rt_speed_feature_framesize_independent":[[662,7,662,17,0,0,4,1]],"encode_nonrd_sb_row":[[5304,7,5304,31,0,0,4,1],[5328,7,5328,14,0,0,4,0],[5335,14,5335,42,0,0,4,0],[5335,46,5335,69,0,0,4,1],[5352,15,5352,38,0,0,4,0]],"get_skip_encode_frame":[[5384,15,5384,39,0,0,4,0],[5384,15,5384,39,0,0,4,1],[5389,10,5389,42,0,0,4,1]],"vp9_set_variance_partition_thresholds":[[643,7,643,55,0,0,4,1]],"update_coef_probs_common":[[591,11,591,25,0,0,4,0],[591,29,591,40,0,0,4,0]],"vp9_compute_qdelta_by_rate":[[2587,30,2587,51,0,0,4,1]],"encode_frame_internal":[[5751,7,5751,25,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_get_global_headers","vpx_codec_decode","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_get_global_headers","vpx_codec_decode","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback context
struct BufferCallbackContext {
    std::vector<uint8_t> buffer;
    bool callback_called = false;
};

// Frame buffer allocation callback
int  id_000055_get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    BufferCallbackContext *ctx = static_cast<BufferCallbackContext*>(cb_priv);
    ctx->callback_called = true;
    
    // Resize buffer if needed
    if (ctx->buffer.size() < min_size) {
        ctx->buffer.resize(min_size);
    }
    
    fb->data = ctx->buffer.data();
    fb->size = ctx->buffer.size();
    return 0;
}

// Frame buffer release callback
int  id_000055_release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    // We don't need to do anything special here since we're managing our own buffer
    return 0;
}

// Put frame callback
void  id_000055_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // This callback is called when a frame is decoded
    // In this fuzzer, we just mark that it was called
    bool *callback_called = static_cast<bool*>(user_priv);
    *callback_called = true;
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 1: Read encoded data from input
    std::vector<uint8_t> encoded_data(size);
    if (fread(encoded_data.data(), 1, size, in_file) != size) {
        // If we can't read all data, use what we have
        encoded_data.resize(ftell(in_file));
    }
    assert_file_closed(&in_file);;
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        // Try VP8 decoder if VP9 fails
        decoder_iface = vpx_codec_vp8_dx();
        res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
        if (res != VPX_CODEC_OK) {
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Step 2: Set up frame buffer callbacks
    BufferCallbackContext buffer_ctx;
    vpx_codec_set_frame_buffer_functions(&decoder,  id_000055_get_frame_buffer_cb, 
                                          id_000055_release_frame_buffer_cb, &buffer_ctx);
    
    // Step 3: Register put frame callback
    bool put_frame_called = false;
    vpx_codec_register_put_frame_cb(&decoder,  id_000055_put_frame_cb, &put_frame_called);
    
    // Step 4: Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t enc_cfg;
    
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (res != VPX_CODEC_OK) {
        // Try VP8 encoder if VP9 fails
        encoder_iface = vpx_codec_vp8_cx();
        res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        if (res != VPX_CODEC_OK) {
            vpx_codec_destroy(&decoder);
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Set some reasonable defaults for fuzzing
    enc_cfg.g_w = 64;
    enc_cfg.g_h = 64;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    
    res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Set encoder configuration
    // Modify some parameters based on fuzzer input
    enc_cfg.rc_min_quantizer = data[0] % 64;
    enc_cfg.rc_max_quantizer = 64 - (data[1] % 32);
    vpx_codec_enc_config_set(&encoder, &enc_cfg);
    
    // Step 6: Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }
    
    // Step 7: Decode input data
    res = vpx_codec_decode(&decoder, encoded_data.data(), encoded_data.size(), nullptr, 0);
    
    // Step 8: Create test image for encoding
    const int width = 64;
    const int height = 64;
    const int stride_align = 1;
    const size_t img_size = width * height * 3 / 2; // I420 format
    
    std::vector<uint8_t> img_data(img_size);
    if (img_size > 0) {
        // Initialize with some pattern based on fuzzer data
        for (size_t i = 0; i < img_size && i < size; i++) {
            img_data[i] = data[i % size];
        }
    }
    
    vpx_image_t img;
    // Step 9: Wrap image data
    vpx_image_t *wrapped_img = vpx_img_wrap(&img, VPX_IMG_FMT_I420, width, height, 
                                           stride_align, img_data.data());
    
    if (wrapped_img) {
        // Step 10: Encode the wrapped image
        res = vpx_codec_encode(&encoder, wrapped_img, 0, 1, 0, VPX_DL_REALTIME);
        
        // Get encoded data
        vpx_codec_iter_t iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
        
        // Step 11: Free the image
        vpx_img_free(&img);
    }
    
    // Cleanup
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&out_file);;
    
    // Verify callbacks were called
    if (buffer_ctx.callback_called) {
        // Buffer callback was invoked
    }
    
    if (put_frame_called) {
        // Put frame callback was invoked
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}