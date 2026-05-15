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
//<ID> 321
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"update_extra_cfg":[[936,7,936,26,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_get_caps","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_img_wrap","vpx_codec_control_","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_get_caps","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_img_wrap","vpx_codec_control_","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer context structure
struct FrameBufferContext {
    std::vector<uint8_t> buffer;
    size_t allocated_size;
};

// Custom get frame buffer callback
static int  id_000030_get_frame_buffer_cb(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    FrameBufferContext* ctx = static_cast<FrameBufferContext*>(cb_priv);
    
    if (min_size > ctx->allocated_size) {
        ctx->buffer.resize(min_size);
        ctx->allocated_size = min_size;
    }
    
    fb->data = ctx->buffer.data();
    fb->size = ctx->allocated_size;
    return 0;
}

// Custom release frame buffer callback
static int  id_000030_release_frame_buffer_cb(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    // In this simple implementation, we don't need to do anything special
    // The buffer is managed by the FrameBufferContext
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE pointer for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE pointer for output data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used

    vpx_codec_ctx_t codec_ctx;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_stream_info_t stream_info;
    vpx_image_t img;
    FrameBufferContext fb_ctx;
    
    // Initialize variables
    memset(&codec_ctx, 0, sizeof(codec_ctx));
    memset(&enc_cfg, 0, sizeof(enc_cfg));
    memset(&stream_info, 0, sizeof(stream_info));
    memset(&img, 0, sizeof(img));
    
    // 1. First, peek at stream info to understand the input data
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_err_t err = vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    
    // 2. Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps; // Mark as used
    
    // 3. Initialize encoder with default configuration
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_1);
    
    // Set some basic encoding parameters based on stream info
    if (stream_info.w > 0 && stream_info.h > 0) {
        enc_cfg.g_w = stream_info.w;
        enc_cfg.g_h = stream_info.h;
    } else {
        // Use small default values if stream info not available
        enc_cfg.g_w = 64;
        enc_cfg.g_h = 64;
    }
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_threads = 1;
    
    // 4. Initialize encoder context
    err = vpx_codec_enc_init_ver(&codec_ctx, encoder_iface, &enc_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 5. Set custom frame buffer functions
    fb_ctx.allocated_size = 0;
    err = vpx_codec_set_frame_buffer_functions(&codec_ctx,  id_000030_get_frame_buffer_cb, 
                                                id_000030_release_frame_buffer_cb, &fb_ctx);
    
    // 6. Create a test image using vpx_img_wrap
    // We'll create a small test image for encoding
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int stride_align = 1;
    
    // Allocate image buffer
    size_t img_size = width * height * 3 / 2; // For I420 format
    std::vector<unsigned char> img_buffer(img_size);
    
    // Fill with some test pattern based on input data
    for (size_t i = 0; i < img_size && i < size; i++) {
        img_buffer[i] = data[i % size];
    }
    
    // Wrap the buffer in a vpx_image_t
    vpx_image_t* wrapped_img = vpx_img_wrap(&img, VPX_IMG_FMT_I420, fuzz_uint32_t_3, fuzz_uint32_t_4, 
                                           stride_align, img_buffer.data());
    
    // 7. Use vpx_codec_control_ to set some encoder parameters
    // Set error resilience mode
    int ctrl_value = 1; // Enable error resilience
    err = vpx_codec_control_(&codec_ctx, VP8E_SET_ENABLEAUTOALTREF, &ctrl_value);
    
    // Set CPU used (speed)
    ctrl_value = 4; // Medium speed
    err = vpx_codec_control_(&codec_ctx, VP8E_SET_CPUUSED, &ctrl_value);
    
    // 8. Encode the image
    if (wrapped_img) {
        err = vpx_codec_encode(&codec_ctx, wrapped_img, fuzz_int64_t_5, fuzz_uint64_t_6, fuzz_int64_t_7, fuzz_uint64_t_8);
        
        // Get encoded data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t* pkt;
        
        while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Write to output file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
    }
    
    // 9. Clean up
    vpx_codec_destroy(&codec_ctx);
    if (wrapped_img) {
        // Note: vpx_img_wrap doesn't allocate the buffer, so we don't need vpx_img_free
        // But we should clear the pointer
        wrapped_img = NULL;
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}