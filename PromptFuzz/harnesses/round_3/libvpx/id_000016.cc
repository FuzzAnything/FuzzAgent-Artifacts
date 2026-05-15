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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include necessary libvpx headers

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* from the input data
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

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used

    vpx_codec_ctx_t decoder;
    vpx_codec_ctx_t encoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_err_t res = VPX_CODEC_OK;
    
    // Initialize decoder with VP9
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        // Try VP8 if VP9 fails
        decoder_iface = vpx_codec_vp8_dx();
        res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_2, VPX_DECODER_ABI_VERSION);
        if (res != VPX_CODEC_OK) {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
    }

    // Decode the input data
    res = vpx_codec_decode(&decoder, data, size, NULL, fuzz_int64_t_3);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Get decoded frame
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *img = vpx_codec_get_frame(&decoder, &iter);
    
    if (img) {
        // Initialize encoder with VP9
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
        
        // Get default encoder configuration
        res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_4);
        if (res != VPX_CODEC_OK) {
            // Try VP8 if VP9 fails
            encoder_iface = vpx_codec_vp8_cx();
            res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_5);
            if (res != VPX_CODEC_OK) {
                vpx_codec_destroy(&decoder);
                assert_file_closed(&in_file);;
                assert_file_closed(&out_file);;
                assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
            }
        }

        // Set encoder configuration based on decoded image
        enc_cfg.g_w = img->d_w;
        enc_cfg.g_h = img->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_pass = VPX_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;

        // Initialize encoder
        res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_6, VPX_ENCODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            // Set the encoder configuration (required API call)
            res = vpx_codec_enc_config_set(&encoder, &enc_cfg);
            
            // Get global headers (required API call)
            vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
            if (global_headers && global_headers->buf && global_headers->sz > 0) {
                // Write global headers to output file
                fwrite(global_headers->buf, 1, global_headers->sz, out_file);
            }

            // Encode the frame
            res = vpx_codec_encode(&encoder, img, fuzz_int64_t_7, fuzz_uint64_t_8, fuzz_int64_t_9, fuzz_uint64_t_10);
            if (res == VPX_CODEC_OK) {
                // Get encoded data
                vpx_codec_iter_t enc_iter = NULL;
                const vpx_codec_cx_pkt_t *pkt = NULL;
                
                while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        // Write encoded data to output file
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
            }

            // Destroy encoder
            vpx_codec_destroy(&encoder);
        }
    }

    // Destroy decoder
    vpx_codec_destroy(&decoder);

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}