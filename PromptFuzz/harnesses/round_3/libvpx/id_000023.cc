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
//<ID> 64
//<Prompt> []
/*<Combination>: [
*/
//<score> 16.666666, nr_unique_branch: 1
//<Quality> {"density":25,"unique_branches":{"vpx_codec_get_preview_frame":[[318,9,318,20,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_control_","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_err_to_string","vpx_codec_error_detail"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_control_","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_err_to_string","vpx_codec_error_detail"],"visited":2}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000023_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // This is just a placeholder callback for fuzzing
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<80) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    const char* iface_name = vpx_codec_iface_name(decoder_iface);
    (void)iface_name; // Use variable to avoid unused warning

    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    
    // Register callback
    vpx_codec_register_put_frame_cb(&decoder,  id_000023_put_frame_callback, nullptr);

    // Peek stream info
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        decoder_iface, data, static_cast<unsigned int>(size > 100 ? 100 : size), &si);

    // Read and decode data
    uint8_t* buffer = new uint8_t[size];
    fread(buffer, 1, size, in_file);
    assert_file_closed(&in_file);;

    vpx_codec_err_t decode_result = vpx_codec_decode(
        &decoder, buffer, static_cast<unsigned int>(size), nullptr, fuzz_int64_t_2);

    // Get stream info after decoding
    vpx_codec_err_t get_stream_info_result = vpx_codec_get_stream_info(&decoder, &si);

    // Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t* decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    
    // Initialize encoder if we have a valid frame
    if (decoded_frame) {
        vpx_codec_ctx_t encoder;
        vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
        
        vpx_codec_enc_cfg_t enc_cfg;
        vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
        
        // Set encoder config based on decoded frame
        enc_cfg.g_w = decoded_frame->d_w;
        enc_cfg.g_h = decoded_frame->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        
        // Initialize encoder with single encoder
        vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION);
        
        // Also try multi-encoder init (with num_enc = 1)
        vpx_codec_ctx_t multi_encoder;
        vpx_rational_t dsf = {1, 1};
        vpx_codec_enc_init_multi_ver(&multi_encoder, encoder_iface, &enc_cfg, 1, fuzz_int64_t_5, &dsf, VPX_ENCODER_ABI_VERSION);
        
        // Try to set config again
        vpx_codec_enc_config_set(&multi_encoder, &enc_cfg);
        
        // Try control API
        vpx_codec_control_(&multi_encoder, VP8E_SET_CPUUSED, 8);
        
        // Create image for encoding
        vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                        decoded_frame->d_w, decoded_frame->d_h, fuzz_uint32_t_6);
        
        if (img) {
            // Set rectangle on image
            vpx_img_set_rect(img, fuzz_uint32_t_7, fuzz_uint32_t_8, 
                            decoded_frame->d_w > 100 ? 100 : decoded_frame->d_w,
                            decoded_frame->d_h > 100 ? 100 : decoded_frame->d_h);
            
            // Encode the frame
            vpx_codec_encode(&multi_encoder, img, fuzz_int64_t_9, fuzz_uint64_t_10, fuzz_int64_t_11, fuzz_uint64_t_12);
            
            // Get encoded data
            vpx_codec_iter_t enc_iter = nullptr;
            const vpx_codec_cx_pkt_t* pkt = vpx_codec_get_cx_data(&multi_encoder, &enc_iter);
            
            // Try to get preview frame
            const vpx_image_t* preview_frame = vpx_codec_get_preview_frame(&multi_encoder);
            (void)preview_frame; // Use variable
            
            // Free allocated image
            vpx_img_free(img);
        }
        
        // Destroy encoders
        vpx_codec_destroy(&multi_encoder);
        vpx_codec_destroy(&encoder);
    }

    // Clean up decoder
    vpx_codec_destroy(&decoder);
    
    // Convert error codes to strings (for coverage)
    const char* err_str = vpx_codec_err_to_string(dec_init_result);
    (void)err_str;
    
    const char* err_detail = vpx_codec_error_detail(&decoder);
    (void)err_detail;

    delete[] buffer;
    assert_file_closed(&in_file);
	return 0;
}