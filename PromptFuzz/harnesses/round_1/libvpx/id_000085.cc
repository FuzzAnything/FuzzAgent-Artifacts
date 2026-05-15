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
//<ID> 342
//<Prompt> []
/*<Combination>: [
*/
//<score> 160, nr_unique_branch: 85
//<Quality> {"density":16,"unique_branches":{"vpx_internal_error":[[123,7,123,10,0,0,4,1]],"vp8_decode_frame":[[1209,7,1209,49,0,0,4,0],[1211,9,1211,38,0,0,4,0],[1217,22,1217,57,0,0,4,1]],"vp8_decode":[[493,11,493,31,0,0,4,0]],"setup_decoding_thread_data":[[48,15,48,24,0,0,4,0],[48,15,48,24,0,0,4,1],[82,9,82,23,0,0,4,1],[85,15,85,30,0,0,4,0],[85,15,85,30,0,0,4,1]],"mt_decode_macroblock":[[99,7,99,48,0,0,4,1],[101,14,101,47,0,0,4,1],[111,7,111,31,0,0,4,0],[142,7,142,59,0,0,4,0],[148,9,148,23,0,0,4,1],[157,11,157,52,0,0,4,1],[161,19,161,25,0,0,4,0],[161,19,161,25,0,0,4,1],[172,13,172,18,0,0,4,0],[172,13,172,18,0,0,4,1],[172,22,172,46,0,0,4,0],[178,13,178,23,0,0,4,0],[178,13,178,23,0,0,4,1],[178,27,178,51,0,0,4,0],[186,14,186,20,0,0,4,0],[186,14,186,20,0,0,4,1],[186,24,186,30,0,0,4,0],[186,24,186,30,0,0,4,1],[186,34,186,41,0,0,4,0],[186,34,186,41,0,0,4,1],[186,46,186,70,0,0,4,0],[195,13,195,24,0,0,4,1],[216,7,216,49,0,0,4,0],[218,9,218,23,0,0,4,1]],"vp8mt_alloc_temp_buffers":[[752,16,752,29,0,0,4,0]],"mt_decode_mb_rows":[[277,15,277,33,0,0,4,0],[277,15,277,33,0,0,4,1],[296,31,296,51,0,0,4,0],[296,31,296,51,0,0,4,1],[308,9,308,19,0,0,4,0],[308,9,308,19,0,0,4,1],[328,9,328,33,0,0,4,0],[362,22,362,42,0,0,4,0],[362,22,362,42,0,0,4,1],[363,11,363,38,0,0,4,0],[363,11,363,38,0,0,4,1],[367,11,367,17,0,0,4,0],[367,11,367,17,0,0,4,1],[367,21,367,44,0,0,4,0],[367,21,367,44,0,0,4,1],[409,11,409,24,0,0,4,0],[409,11,409,24,0,0,4,1],[412,16,412,36,0,0,4,0],[412,16,412,36,0,0,4,1],[421,11,421,62,0,0,4,1],[443,11,443,36,0,0,4,1],[449,11,449,35,0,0,4,0],[449,11,449,35,0,0,4,1],[450,24,450,66,0,0,4,1],[461,13,461,38,0,0,4,0],[472,13,472,38,0,0,4,0],[475,15,475,50,0,0,4,0],[476,25,476,31,0,0,4,0],[476,25,476,31,0,0,4,1],[480,25,480,30,0,0,4,0],[480,25,480,30,0,0,4,1],[490,13,490,25,0,0,4,0],[491,15,491,51,0,0,4,0],[500,17,500,27,0,0,4,1],[505,17,505,25,0,0,4,0],[511,17,511,27,0,0,4,0],[511,17,511,27,0,0,4,1],[516,17,516,25,0,0,4,0],[550,9,550,33,0,0,4,1],[580,7,580,71,0,0,4,1]],"vp8mt_decode_mb_rows":[[853,7,853,19,0,0,4,0],[862,17,862,32,0,0,4,0],[862,17,862,32,0,0,4,1],[872,17,872,32,0,0,4,0],[872,17,872,32,0,0,4,1],[887,15,887,45,0,0,4,0],[887,15,887,45,0,0,4,1],[897,17,897,47,0,0,4,0],[897,17,897,47,0,0,4,1]],"thread_decoding_proc":[[594,11,594,65,0,0,4,1]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_get_global_headers","vpx_img_wrap","vpx_img_set_rect","vpx_img_flip","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_error_detail","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_get_global_headers","vpx_img_wrap","vpx_img_set_rect","vpx_img_flip","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_error_detail","vpx_codec_destroy","vpx_img_free"],"visited":0}
/*To create a comprehensive fuzzer for libvpx that exercises both decoding and encoding APIs, I'll implement a fuzz driver that processes input VP8 data, decodes it, modifies the frames, and re-encodes them. Here's the complete solution:

*/


extern "C" {
}

// Callback function declarations
static void  id_000085_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // Simple callback - just track that it was called
    int *callback_called = static_cast<int*>(user_priv);
    *callback_called = 1;
}

static void  id_000085_put_slice_cb(void *user_priv, const vpx_image_t *img, 
                         const vpx_image_rect_t *rec, const vpx_image_rect_t *rec2) {
    // Simple callback - just track that it was called
    int *callback_called = static_cast<int*>(user_priv);
    *callback_called = 1;
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 10) {
	return 0;
	}
	
    
    // Create FILE pointer for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for encoded data
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
    (void)out_fd;  // Mark as used
    
    // Track callback usage
    int frame_callback_called = 0;
    int slice_callback_called = 0;
    
    // DECODING PHASE
    
    // Get VP8 decoder interface
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    
    // Peek at stream info
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    vpx_codec_err_t err = vpx_codec_peek_stream_info(decoder_iface, data, 
                                                     static_cast<unsigned int>(size), 
                                                     &stream_info);
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 2;
    dec_cfg.w = stream_info.w;
    dec_cfg.h = stream_info.h;
    
    err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                 0, VPX_DECODER_ABI_VERSION);
    
    if (err != VPX_CODEC_OK) {
        // Try with default config
        err = vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 
                                     0, VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
    }
    
    // Register put frame callback
    err = vpx_codec_register_put_frame_cb(&decoder,  id_000085_put_frame_cb, &frame_callback_called);
    
    // Decode the input data
    err = vpx_codec_decode(&decoder, data, static_cast<unsigned int>(size), 
                          nullptr, 0);
    
    // Get stream info from decoder context
    err = vpx_codec_get_stream_info(&decoder, &stream_info);
    
    // Collect decoded frames
    std::vector<vpx_image_t*> decoded_frames;
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *frame;
    
    while ((frame = vpx_codec_get_frame(&decoder, &iter))) {
        // Create a copy of the frame for modification
        vpx_image_t *frame_copy = vpx_img_alloc(nullptr, frame->fmt, 
                                               frame->d_w, frame->d_h, 1);
        if (frame_copy) {
            // Copy image data
            for (int plane = 0; plane < 3; ++plane) {
                if (frame->planes[plane]) {
                    size_t plane_size = static_cast<size_t>(frame->stride[plane]) * 
                                       (plane == 0 ? frame->d_h : frame->d_h / 2);
                    memcpy(frame_copy->planes[plane], frame->planes[plane], plane_size);
                }
            }
            decoded_frames.push_back(frame_copy);
        }
    }
    
    // ENCODING PHASE (only if we have decoded frames)
    if (!decoded_frames.empty()) {
        // Get VP8 encoder interface
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
        
        // Get default encoder configuration
        vpx_codec_enc_cfg_t enc_cfg;
        err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        
        // Configure encoder based on first frame
        vpx_image_t *first_frame = decoded_frames[0];
        enc_cfg.g_w = first_frame->d_w;
        enc_cfg.g_h = first_frame->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 256;  // 256 kbps
        enc_cfg.g_pass = VPX_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        
        // Initialize encoder
        vpx_codec_ctx_t encoder;
        err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                     0, VPX_ENCODER_ABI_VERSION);
        
        if (err == VPX_CODEC_OK) {
            // Set encoder configuration (even though we just initialized)
            err = vpx_codec_enc_config_set(&encoder, &enc_cfg);
            
            // Register put slice callback
            err = vpx_codec_register_put_slice_cb(&encoder,  id_000085_put_slice_cb, &slice_callback_called);
            
            // Get global headers
            vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
            if (global_headers && global_headers->buf) {
                fwrite(global_headers->buf, 1, global_headers->sz, out_file);
            }
            
            // Process and encode each frame
            for (size_t i = 0; i < decoded_frames.size(); ++i) {
                vpx_image_t *frame_to_encode = decoded_frames[i];
                
                // Create another image using vpx_img_wrap
                size_t img_data_size = static_cast<size_t>(frame_to_encode->stride[0]) * frame_to_encode->d_h * 3 / 2;
                std::vector<unsigned char> img_data(img_data_size);
                vpx_image_t *wrapped_img = vpx_img_wrap(nullptr, frame_to_encode->fmt,
                                                       frame_to_encode->d_w, frame_to_encode->d_h,
                                                       1, img_data.data());
                
                if (wrapped_img) {
                    // Copy data to wrapped image
                    for (int plane = 0; plane < 3; ++plane) {
                        if (frame_to_encode->planes[plane] && wrapped_img->planes[plane]) {
                            size_t plane_size = static_cast<size_t>(frame_to_encode->stride[plane]) * 
                                               (plane == 0 ? frame_to_encode->d_h : frame_to_encode->d_h / 2);
                            memcpy(wrapped_img->planes[plane], frame_to_encode->planes[plane], plane_size);
                        }
                    }
                    
                    // Modify image using vpx_img_set_rect
                    unsigned int rect_w = frame_to_encode->d_w / 2;
                    unsigned int rect_h = frame_to_encode->d_h / 2;
                    if (rect_w > 0 && rect_h > 0) {
                        vpx_img_set_rect(wrapped_img, 0, 0, rect_w, rect_h);
                    }
                    
                    // Flip image
                    vpx_img_flip(wrapped_img);
                    
                    // Encode the frame
                    err = vpx_codec_encode(&encoder, wrapped_img, i, 1, 0, 0);
                    
                    // Get encoded data
                    vpx_codec_iter_t enc_iter = nullptr;
                    const vpx_codec_cx_pkt_t *pkt;
                    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter))) {
                        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                        }
                    }
                    
                    // Try to get preview frame
                    const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
                    (void)preview;  // Mark as used
                }
                
                // Free wrapped image (doesn't own data, so just free structure)
                free(wrapped_img);
            }
            
            // Flush encoder
            err = vpx_codec_encode(&encoder, nullptr, -1, 1, 0, 0);
            
            // Get any remaining encoded data
            vpx_codec_iter_t enc_iter = nullptr;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter))) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
            
            // Get error details (even if no error, to exercise the API)
            const char *error_detail = vpx_codec_error_detail(&encoder);
            (void)error_detail;  // Mark as used
            
            // Destroy encoder
            err = vpx_codec_destroy(&encoder);
        }
    }
    
    // Get error details from decoder
    const char *dec_error_detail = vpx_codec_error_detail(&decoder);
    (void)dec_error_detail;  // Mark as used
    
    // CLEANUP
    
    // Destroy decoder
    err = vpx_codec_destroy(&decoder);
    
    // Free decoded frames
    for (vpx_image_t *frame : decoded_frames) {
        vpx_img_free(frame);
    }
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}