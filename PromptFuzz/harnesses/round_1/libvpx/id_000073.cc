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
//<ID> 295
//<Prompt> []
/*<Combination>: [
*/
//<score> 182, nr_unique_branch: 86
//<Quality> {"density":28,"unique_branches":{"update_mode":[[65,7,65,31,0,0,4,0],[73,38,73,39,0,0,4,0],[74,14,74,21,0,0,4,0],[74,14,74,21,0,0,4,1]],"vp8_update_rate_correction_factors":[[1081,7,1081,34,0,0,4,0],[1086,12,1086,17,0,0,4,0],[1086,12,1086,17,0,0,4,1],[1091,11,1091,26,0,0,4,1]],"vp8_regulate_q":[[1245,11,1245,46,0,0,4,1],[1247,18,1247,49,0,0,4,0],[1248,18,1248,46,0,0,4,0],[1249,19,1249,52,0,0,4,1],[1250,20,1250,52,0,0,4,1],[1279,14,1279,50,0,0,4,0],[1282,13,1282,49,0,0,4,1],[1290,13,1290,28,0,0,4,1],[1293,13,1293,56,0,0,4,0],[1293,13,1293,56,0,0,4,1]],"vp8_initialize_rd_consts":[[177,7,177,34,0,0,4,0]],"vp8_auto_select_speed":[[281,7,281,58,0,0,4,1],[287,11,287,70,0,0,4,0],[292,13,292,28,0,0,4,1],[304,13,304,27,0,0,4,1],[312,9,312,24,0,0,4,1]],"vpx_codec_register_put_frame_cb":[[138,15,138,18,0,0,4,0]],"vp8cx_pick_filter_level_fast":[[167,7,167,42,0,0,4,0],[187,10,187,38,0,0,4,1]],"calc_pframe_target_size":[[604,13,604,71,0,0,4,0],[627,11,627,54,0,0,4,1],[632,13,632,69,0,0,4,0]],"vp8_pick_inter_mode":[[1010,15,1010,43,0,0,4,0]],"vp8_set_speed_features":[[846,7,846,24,0,0,4,1],[957,11,957,20,0,0,4,0],[965,13,965,45,0,0,4,1],[969,21,969,28,0,0,4,0],[969,21,969,28,0,0,4,1],[977,16,977,24,0,0,4,0],[977,16,977,24,0,0,4,1],[980,15,981,72,0,0,4,1],[989,13,989,26,0,0,4,1],[991,13,991,27,0,0,4,0],[997,13,997,27,0,0,4,1],[1003,13,1003,27,0,0,4,1]],"vp8_hex_search":[[910,7,910,13,0,0,4,0],[911,17,911,22,0,0,4,0],[911,17,911,22,0,0,4,1],[931,7,931,22,0,0,4,0],[980,15,980,28,0,0,4,0],[984,9,984,15,0,0,4,0],[985,19,985,24,0,0,4,0],[985,19,985,24,0,0,4,1],[1005,9,1005,24,0,0,4,0],[825,9,825,26,2,0,4,0],[828,11,828,28,2,0,4,1],[825,9,825,26,10,0,4,0],[828,11,828,28,10,0,4,1]],"vp8_find_best_sub_pixel_step":[[420,7,420,21,0,0,4,1],[432,7,432,22,0,0,4,1],[446,7,446,19,0,0,4,1],[458,7,458,21,0,0,4,1],[466,15,466,27,0,0,4,0],[466,40,466,49,0,0,4,0],[470,5,470,11,0,0,4,0],[477,5,477,11,0,0,4,1],[483,5,483,11,0,0,4,1],[489,5,489,11,0,0,4,1],[490,5,490,12,0,0,4,1],[500,7,500,21,0,0,4,1],[508,7,508,44,0,0,4,1],[510,7,510,44,0,0,4,1],[517,7,517,28,0,0,4,1],[529,7,529,21,0,0,4,1],[541,7,541,22,0,0,4,1],[551,7,551,28,0,0,4,1],[563,7,563,19,0,0,4,1],[575,7,575,21,0,0,4,1],[583,15,583,27,0,0,4,0],[583,40,583,49,0,0,4,0],[587,11,587,19,0,0,4,1],[588,5,588,11,0,0,4,0],[590,11,590,32,0,0,4,1],[605,13,605,34,0,0,4,1],[617,5,617,11,0,0,4,1],[631,5,631,11,0,0,4,1],[645,5,645,11,0,0,4,1],[655,7,655,21,0,0,4,1]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_error_detail","vpx_codec_enc_config_set","vpx_codec_error_detail","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_img_wrap","vpx_img_free","vpx_codec_destroy","vpx_img_set_rect","vpx_img_flip","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error_detail","vpx_img_free","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_img_alloc","vpx_img_wrap","vpx_img_set_rect","vpx_img_flip","vpx_codec_control_","vpx_codec_control_","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error_detail","vpx_img_free","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


// libvpx headers

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create FILE pointer for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create FILE pointer for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    vpx_codec_ctx_t encoder;
    vpx_image_t *img = nullptr;
    vpx_image_t *wrapped_img = nullptr;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iter_t iter = nullptr;
    vpx_fixed_buf_t *global_headers = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    
    // Get VP8 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get interface name
    const char *iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; // Use variable to avoid unused warning

    // Allocate default encoder configuration
    vpx_codec_err_t res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Configure encoder with reasonable defaults
    cfg.g_w = 640;
    cfg.g_h = 480;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    // Initialize encoder
    res = vpx_codec_enc_init_ver(&encoder, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&encoder);
        (void)error_detail;
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set encoder configuration
    res = vpx_codec_enc_config_set(&encoder, &cfg);
    if (res != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&encoder);
        (void)error_detail;
        vpx_codec_destroy(&encoder);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Allocate image using vpx_img_alloc
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&encoder);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Read some data to use as image data
    std::vector<unsigned char> img_data(cfg.g_w * cfg.g_h * 3 / 2);
    size_t bytes_read = fread(img_data.data(), 1, std::min(img_data.size(), size), in_file);
    if (bytes_read > 0) {
        // Pad with zeros if needed
        if (bytes_read < img_data.size()) {
            memset(img_data.data() + bytes_read, 0, img_data.size() - bytes_read);
        }
    }

    // Create wrapped image using vpx_img_wrap
    wrapped_img = vpx_img_wrap(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1, img_data.data());
    if (!wrapped_img) {
        vpx_img_free(img);
        vpx_codec_destroy(&encoder);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Try to set rectangle (may fail if params are invalid)
    vpx_img_set_rect(wrapped_img, 0, 0, cfg.g_w, cfg.g_h);

    // Flip image
    vpx_img_flip(wrapped_img);

    // Call vpx_codec_control_ with some control IDs
    // Note: We'll try a few common control IDs
    vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 6);
    vpx_codec_control_(&encoder, VP8E_SET_ENABLEAUTOALTREF, 1);
    vpx_codec_control_(&encoder, VP8E_SET_NOISE_SENSITIVITY, 0);

    // Get global headers
    global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }

    // Register put frame callback (for decoding, but we'll call it anyway)
    vpx_codec_register_put_frame_cb(&encoder, nullptr, nullptr);

    // Encode the wrapped image
    res = vpx_codec_encode(&encoder, wrapped_img, 0, 1, 0, VPX_DL_REALTIME);
    if (res != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&encoder);
        (void)error_detail;
    } else {
        // Get encoded data
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
    }

    // Encode the allocated image as well (with different parameters)
    if (img) {
        // Fill img with some data from input
        if (size > 0) {
            size_t offset = std::min(size, (size_t)img->bps);
            memcpy(img->img_data, data, offset);
        }
        
        res = vpx_codec_encode(&encoder, img, 1, 1, 0, VPX_DL_REALTIME);
        if (res == VPX_CODEC_OK) {
            iter = nullptr;
            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
        }
    }

    // Get error detail (even if no error, just to call the API)
    const char *error_detail = vpx_codec_error_detail(&encoder);
    (void)error_detail;

    // Cleanup
    if (wrapped_img) {
        // Note: wrapped_img doesn't own the data, so we don't free img_data here
        // Just set to nullptr to avoid double free
        wrapped_img->img_data = nullptr;
        vpx_img_free(wrapped_img);
    }
    
    if (img) {
        vpx_img_free(img);
    }
    
    vpx_codec_destroy(&encoder);
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}