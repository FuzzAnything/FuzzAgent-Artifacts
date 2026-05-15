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
//<ID> 277
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_error","vpx_codec_enc_init_multi_ver","vpx_codec_error","vpx_codec_set_cx_data_buf","vpx_codec_error","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_set_cx_data_buf","vpx_codec_error","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
	//fuzzer shim end}




    // Early exit if not enough data for basic operations
    if (size < 100) {
	return 0;
	}
	

    // Create a FILE* from input data
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

    // Initialize variables
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_fixed_buf_t output_buf;
    vpx_rational_t dsf = {1, 1};
    vpx_codec_err_t res = VPX_CODEC_OK;
    vpx_codec_caps_t caps;
    const char *iface_name = nullptr;
    const char *error_str = nullptr;

    // 1. Get codec capabilities
    vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();
    caps = vpx_codec_get_caps(codec_iface);

    // 2. Get interface name
    iface_name = vpx_codec_iface_name(codec_iface);

    // 3. Initialize encoder configuration
    res = vpx_codec_enc_config_default(codec_iface, &cfg, fuzz_uint32_t_1);
    if (res != VPX_CODEC_OK) {
        error_str = vpx_codec_error(&codec);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Configure encoding parameters
    cfg.g_w = 640;
    cfg.g_h = 480;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    // 4. Initialize multi-encoder
    res = vpx_codec_enc_init_multi_ver(&codec, codec_iface, &cfg, 1, fuzz_int64_t_2, &dsf, fuzz_int32_t_3);
    if (res != VPX_CODEC_OK) {
        error_str = vpx_codec_error(&codec);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 5. Prepare output buffer
    std::vector<uint8_t> buffer(size * 2);  // Double size for safety
    output_buf.buf = buffer.data();
    output_buf.sz = buffer.size();

    // 6. Set output buffer with padding
    res = vpx_codec_set_cx_data_buf(&codec, &output_buf, fuzz_uint32_t_4, fuzz_uint32_t_5);
    if (res != VPX_CODEC_OK) {
        error_str = vpx_codec_error(&codec);
    }

    // 7. Read data from input file and encode
    std::vector<uint8_t> frame_data(cfg.g_w * cfg.g_h * 3 / 2);  // YUV420
    size_t bytes_read = fread(frame_data.data(), 1, frame_data.size(), in_file);
    
    if (bytes_read > 0) {
        // Create a dummy image for encoding
        vpx_image_t img;
        vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, fuzz_uint32_t_6);
        
        // Fill image with data from input
        size_t copy_size = std::min(bytes_read, static_cast<size_t>(cfg.g_w * cfg.g_h * 3 / 2));
        memcpy(img.planes[0], frame_data.data(), copy_size);
        
        // Encode the frame
        res = vpx_codec_encode(&codec, &img, fuzz_int64_t_7, fuzz_uint64_t_8, fuzz_int64_t_9, fuzz_uint64_t_10);
        
        if (res == VPX_CODEC_OK) {
            // Get encoded data
            vpx_codec_iter_t iter = nullptr;
            const vpx_codec_cx_pkt_t *pkt = nullptr;
            
            while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
        } else {
            error_str = vpx_codec_error(&codec);
        }
        
        vpx_img_free(&img);
    }

    // Cleanup
    vpx_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}