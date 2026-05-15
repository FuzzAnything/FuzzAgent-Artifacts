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
//<ID> 257
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"vpx_codec_get_preview_frame":[[318,9,318,20,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_get_stream_info","vpx_codec_set_cx_data_buf","vpx_codec_get_preview_frame","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_get_stream_info","vpx_codec_set_cx_data_buf","vpx_codec_get_preview_frame","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000066_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // This is a no-op callback for demonstration
    (void)user_priv;
    (void)img;
}

// Buffer structure for encoded data
struct encoded_buffer {
    uint8_t* data;
    size_t size;
};

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE pointer from the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for encoded data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    vpx_codec_ctx_t decoder_ctx = {0};
    vpx_codec_ctx_t encoder_ctx = {0};
    vpx_image_t* img = nullptr;
    vpx_fixed_buf_t enc_buf = {0};
    encoded_buffer enc_output = {0};
    
    // 1. Get decoder and encoder interfaces
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
    
    // 2. Get capabilities
    vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
    vpx_codec_caps_t encoder_caps = vpx_codec_get_caps(encoder_iface);
    (void)decoder_caps;
    (void)encoder_caps;
    
    // 3. Peek stream info to get dimensions
    vpx_codec_stream_info_t stream_info = {0};
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_err_t res = vpx_codec_peek_stream_info(
        decoder_iface, data, (unsigned int)size, &stream_info);
    
    // 4. Initialize decoder
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = stream_info.w;
    dec_cfg.h = stream_info.h;
    
    res = vpx_codec_dec_init_ver(
        &decoder_ctx, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Allocate image for encoding
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                       stream_info.w > 0 ? stream_info.w : 128,
                       stream_info.h > 0 ? stream_info.h : 128,
                       1);
    
    // 6. Setup output buffer for encoder
    enc_output.size = 1024 * 1024; // 1MB buffer
    enc_output.data = (uint8_t*)malloc(enc_output.size);
    enc_buf.buf = enc_output.data;
    enc_buf.sz = enc_output.size;
    
    // 7. Register put frame callback
    res = vpx_codec_register_put_frame_cb(&decoder_ctx,  id_000066_put_frame_callback, nullptr);
    
    // 8. Decode the input data
    res = vpx_codec_decode(&decoder_ctx, data, (unsigned int)size, nullptr, 0);
    
    // 9. Initialize encoder with default config
    vpx_codec_enc_cfg_t enc_cfg = {0};
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    enc_cfg.g_w = stream_info.w > 0 ? stream_info.w : 128;
    enc_cfg.g_h = stream_info.h > 0 ? stream_info.h : 128;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    
    res = vpx_codec_enc_init_ver(
        &encoder_ctx, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    // 10. Get codec interface name
    const char* decoder_name = vpx_codec_iface_name(decoder_iface);
    const char* encoder_name = vpx_codec_iface_name(encoder_iface);
    (void)decoder_name;
    (void)encoder_name;
    
    // 11. Get stream info from decoder context
    vpx_codec_stream_info_t dec_stream_info = {0};
    dec_stream_info.sz = sizeof(dec_stream_info);
    res = vpx_codec_get_stream_info(&decoder_ctx, &dec_stream_info);
    
    // 12. Set output buffer for encoder
    res = vpx_codec_set_cx_data_buf(&encoder_ctx, &enc_buf, 0, 0);
    
    // Try to get frames from decoder and encode them
    vpx_codec_iter_t iter = nullptr;
    int frame_count = 0;
    
    while (frame_count < 5) { // Limit to 5 frames for fuzzing
        // 13. Get preview frame from encoder (may be null)
        const vpx_image_t* preview = vpx_codec_get_preview_frame(&encoder_ctx);
        (void)preview;
        
        // 14. Get decoded frame
        const vpx_image_t* decoded_frame = vpx_codec_get_frame(&decoder_ctx, &iter);
        if (!decoded_frame) {
            break;
        }
        
        // Encode the frame
        res = vpx_codec_encode(&encoder_ctx, decoded_frame, 
                              frame_count, 1, 0, VPX_DL_REALTIME);
        
        frame_count++;
    }
    
    // Flush encoder
    res = vpx_codec_encode(&encoder_ctx, nullptr, -1, 1, 0, VPX_DL_REALTIME);
    
    // 15. Free allocated image
    if (img) {
        vpx_img_free(img);
    }
    
    // 16. Destroy codec contexts
    if (decoder_ctx.priv) {
        vpx_codec_destroy(&decoder_ctx);
    }
    
    if (encoder_ctx.priv) {
        vpx_codec_destroy(&encoder_ctx);
    }
    
    // Write encoded output to file
    if (enc_buf.sz > 0 && enc_buf.sz <= enc_output.size) {
        fwrite(enc_buf.buf, 1, enc_buf.sz, out_file);
    }
    
    // Cleanup
    free(enc_output.data);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}