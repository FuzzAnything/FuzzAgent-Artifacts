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
//<ID> 260
//<Prompt> []
/*<Combination>: [
*/
//<score> 81, nr_unique_branch: 2
//<Quality> {"density":27,"unique_branches":{"vpx_codec_error_detail":[[60,7,60,10,0,0,4,1]],"vpx_codec_error":[[55,10,55,15,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_error","vpx_codec_error_detail","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_error_detail","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_alloc","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_error","vpx_codec_error_detail","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_error_detail","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"visited":0}
/*I'll create a fuzz driver that tests libvpx's VP9 decoder and encoder functionality. The event I'll implement is: **Decoding VP9 encoded video, then re-encoding it, and finally decoding the re-encoded output**.

*/


// Include libvpx headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for stream info
    
    vpx_codec_ctx_t decoder_ctx = {0};
    vpx_codec_ctx_t encoder_ctx = {0};
    vpx_codec_ctx_t re_decoder_ctx = {0};
    vpx_image_t *img = nullptr;
    vpx_image_t *tmp_img = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    vpx_codec_stream_info_t stream_info = {0};
    vpx_codec_enc_cfg_t enc_cfg = {0};
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    uint8_t *encoded_buffer = nullptr;
    size_t encoded_size = 0;
    
    // Create in-memory file for input data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for encoded data
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get VP9 decoder interface
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get VP9 encoder interface
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Peek stream info from input data
    stream_info.sz = sizeof(stream_info);
    vpx_codec_err_t err = vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    
    // Get error info (API usage)
    const char *error_str = vpx_codec_error(nullptr);
    const char *error_detail = vpx_codec_error_detail(nullptr);
    
    // 2. Initialize decoder
    err = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION);
    
    // Get error info from decoder context
    error_str = vpx_codec_error(&decoder_ctx);
    error_detail = vpx_codec_error_detail(&decoder_ctx);
    
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Decode the input
    err = vpx_codec_decode(&decoder_ctx, data, size, nullptr, 0);
    
    // 4. Get decoded frame
    iter = nullptr;
    img = vpx_codec_get_frame(&decoder_ctx, &iter);
    
    if (!img) {
        // Try to get preview frame (API usage - though typically for encoder)
        const vpx_image_t *preview = vpx_codec_get_preview_frame(&decoder_ctx);
        
        // Clean up and exit if no frames
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Allocate a temporary image (API usage)
    tmp_img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                           img->d_w > 0 ? img->d_w : 64,
                           img->d_h > 0 ? img->d_h : 64, 16);
    
    // 6. Get default encoder configuration
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    if (err != VPX_CODEC_OK) {
        vpx_img_free(tmp_img);
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration based on decoded frame
    if (img->d_w > 0 && img->d_h > 0) {
        enc_cfg.g_w = img->d_w;
        enc_cfg.g_h = img->d_h;
    } else {
        enc_cfg.g_w = 64;
        enc_cfg.g_h = 64;
    }
    enc_cfg.rc_target_bitrate = 256;  // Low bitrate for fuzzing
    
    // 7. Initialize encoder using vpx_codec_enc_init_ver
    err = vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    // Also initialize another encoder using vpx_codec_enc_init_multi_ver (API usage)
    vpx_codec_ctx_t multi_encoder_ctx = {0};
    vpx_rational_t dsf = {1, 1};
    err = vpx_codec_enc_init_multi_ver(&multi_encoder_ctx, encoder_iface, &enc_cfg, 
                                       1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    
    // 8. Encode the decoded frame
    err = vpx_codec_encode(&encoder_ctx, img, 0, 1, 0, VPX_DL_REALTIME);
    
    // 9. Get encoded data packets
    iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Write to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            
            // Also try to decode the re-encoded data
            // 10. Initialize re-decoder
            err = vpx_codec_dec_init_ver(&re_decoder_ctx, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION);
            
            if (err == VPX_CODEC_OK) {
                // 11. Decode the re-encoded data
                err = vpx_codec_decode(&re_decoder_ctx, 
                                      (const uint8_t*)pkt->data.frame.buf,
                                      pkt->data.frame.sz, nullptr, 0);
                
                // 12. Get frame from re-decoder
                vpx_codec_iter_t re_iter = nullptr;
                vpx_image_t *re_img = vpx_codec_get_frame(&re_decoder_ctx, &re_iter);
                
                // Clean up re-decoder
                vpx_codec_destroy(&re_decoder_ctx);
            }
        }
    }
    
    // Try to get preview frame from encoder (API usage)
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&encoder_ctx);
    
    // 13. Clean up
    fflush(out_file);
    
    // Get file descriptor for output file (API usage requirement)
    int out_fd = fuzz_fileno(out_file);
    (void)out_fd;  // Use to avoid unused variable warning
    
    // Clean up multi-encoder
    vpx_codec_destroy(&multi_encoder_ctx);
    
    // Clean up encoder
    vpx_codec_destroy(&encoder_ctx);
    
    // Clean up decoder
    vpx_codec_destroy(&decoder_ctx);
    
    // Free images
    vpx_img_free(tmp_img);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	return 0;
}