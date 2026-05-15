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
//<ID> 226
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback functions for put_frame and put_slice
static void  id_000051_put_frame_cb(void* user_priv, const vpx_image_t* img) {
    // Minimal implementation - can be empty for fuzzing
    (void)user_priv;
    (void)img;
}

static void  id_000051_put_slice_cb(void* user_priv, const vpx_image_t* img,
                         const vpx_image_rect_t* rect, const vpx_image_rect_t* rect2) {
    // Minimal implementation - can be empty for fuzzing
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a file pointer from the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    (void)fd_in; // Use in file operations if needed

    // Initialize decoder
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 640;
    dec_cfg.h = 480;

    // Check decoder capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps;

    // Initialize decoder
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Register callbacks
    vpx_codec_register_put_frame_cb(&decoder,  id_000051_put_frame_cb, nullptr);
    vpx_codec_register_put_slice_cb(&decoder,  id_000051_put_slice_cb, nullptr);

    // Read data from file and decode
    uint8_t* buffer = new uint8_t[size];
    fread(buffer, 1, size, in_file);
    vpx_codec_decode(&decoder, buffer, static_cast<unsigned int>(size), nullptr, fuzz_int64_t_2);

    // Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t* decoded_img = vpx_codec_get_frame(&decoder, &iter);

    // Initialize encoder if we got a decoded frame
    if (decoded_img) {
        vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
        vpx_codec_ctx_t encoder;
        vpx_codec_enc_cfg_t enc_cfg;
        
        // Get default encoder config
        if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3) == VPX_CODEC_OK) {
            // Configure encoder
            enc_cfg.g_w = decoded_img->d_w;
            enc_cfg.g_h = decoded_img->d_h;
            enc_cfg.g_threads = 1;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_pass = VPX_RC_ONE_PASS;
            enc_cfg.g_lag_in_frames = 0;

            // Initialize encoder
            if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                // Allocate image for encoding
                vpx_image_t* enc_img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420,
                                                    static_cast<unsigned int>(decoded_img->d_w),
                                                    static_cast<unsigned int>(decoded_img->d_h),
                                                    fuzz_uint32_t_4);
                
                if (enc_img) {
                    // Set rectangle on image
                    vpx_img_set_rect(enc_img, fuzz_uint32_t_5, fuzz_uint32_t_6,
                                    static_cast<unsigned int>(decoded_img->d_w),
                                    static_cast<unsigned int>(decoded_img->h));
                    
                    // Copy decoded image data (simplified - assumes I420 format)
                    if (decoded_img->fmt == VPX_IMG_FMT_I420) {
                        for (int plane = 0; plane < 3; ++plane) {
                            int src_stride = decoded_img->stride[plane];
                            int dst_stride = enc_img->stride[plane];
                            int height = (plane == 0) ? decoded_img->d_h : decoded_img->d_h / 2;
                            int width = (plane == 0) ? decoded_img->d_w : decoded_img->d_w / 2;
                            
                            for (int y = 0; y < height; ++y) {
                                memcpy(enc_img->planes[plane] + y * dst_stride,
                                       decoded_img->planes[plane] + y * src_stride,
                                       width);
                            }
                        }
                    }

                    // Encode the frame
                    vpx_codec_encode(&encoder, enc_img, fuzz_int64_t_7, fuzz_uint64_t_8, fuzz_int64_t_9, fuzz_uint64_t_10);

                    // Get encoded data
                    vpx_codec_iter_t enc_iter = nullptr;
                    const vpx_codec_cx_pkt_t* pkt;
                    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                            // Write to output file (for demonstration)
                            FILE* out_file = fopen("output_file", "wb");
                            if (out_file) {
                                int fd_out = fuzz_fileno(out_file);
                                (void)fd_out;
                                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                                assert_file_closed(&out_file);;
                            }
                        }
                    }

                    // Free allocated image
                    vpx_img_free(enc_img);
                }

                // Destroy encoder
                vpx_codec_destroy(&encoder);
            }
        }
    }

    // Cleanup
    delete[] buffer;
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}