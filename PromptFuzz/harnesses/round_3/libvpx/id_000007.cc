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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_destroy","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_encode","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<100) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_13, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_14, fdp);
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // 1. Get version extra string
    const char *version_extra = vpx_codec_version_extra_str();
    (void)version_extra; // Use variable to avoid unused warning

    // 2. Create a decoder context for VP9
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Initialize decoder with default configuration
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }

    // 3. Get stream information from the input data
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    
    if (vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // 4. Decode the input data
    if (vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // 5. Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *img = nullptr;
    std::vector<vpx_image_t*> decoded_frames;
    
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        decoded_frames.push_back(img);
    }

    // 6. Create encoder context for re-encoding
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    // Configure encoder
    vpx_codec_enc_cfg_t enc_cfg;
    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // Set encoder configuration based on decoded frame dimensions
    if (!decoded_frames.empty()) {
        vpx_image_t *first_frame = decoded_frames[0];
        enc_cfg.g_w = first_frame->d_w;
        enc_cfg.g_h = first_frame->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_error_resilient = 0;
    }

    // 7. Initialize encoder with multiple contexts (num_enc = 1)
    vpx_rational_t dsf = {1, 1}; // Downsampling factor
    if (vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &enc_cfg, 1, fuzz_int64_t_4, &dsf, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // 8. Process decoded frames
    for (vpx_image_t *frame : decoded_frames) {
        // Set a rectangle on the image (using a portion of the frame)
        unsigned int rect_x = 0;
        unsigned int rect_y = 0;
        unsigned int rect_w = frame->d_w / 2;
        unsigned int rect_h = frame->d_h / 2;
        
        if (rect_w > 0 && rect_h > 0) {
            vpx_img_set_rect(frame, fuzz_uint32_t_5, fuzz_uint32_t_6, rect_w, rect_h);
        }

        // Encode the frame
        vpx_codec_encode(&encoder, frame, fuzz_int64_t_7, fuzz_uint64_t_8, fuzz_int64_t_9, fuzz_uint64_t_10);
    }

    // 9. Flush encoder
    vpx_codec_encode(&encoder, nullptr, fuzz_int64_t_11, fuzz_uint64_t_12, fuzz_int64_t_13, fuzz_uint64_t_14);

    // 10. Get encoded data
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Encoded frame data available in pkt->data.frame.buf
            // Could write to file or process further
        }
    }

    // 11. Cleanup
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);

    return 0;
}