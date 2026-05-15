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
//<ID> 92
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"vpx_realloc_frame_buffer":[[222,31,222,61,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_error","vpx_codec_get_stream_info","vpx_img_alloc","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_error","vpx_codec_get_stream_info","vpx_img_alloc","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback structures
static int  id_000030_get_frame_buffer(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    (void)min_size;
    fb->data = static_cast<uint8_t*>(malloc(4096)); // Simple allocation
    fb->size = 4096;
    return 0;
}

static int  id_000030_release_frame_buffer(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

// Put frame callback
static void  id_000030_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
    // Frame received callback - can be used for processing
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Too small for meaningful processing

    // Create a FILE* for input data
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

    vpx_codec_ctx_t decoder, encoder;
    vpx_codec_stream_info_t stream_info;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_rational_t timebase = {1, 30}; // 30 fps
    vpx_image_t *image = nullptr;
    std::vector<uint8_t> buffer(size);
    size_t bytes_read;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;

    // 1. Read input data into buffer
    bytes_read = fread(buffer.data(), 1, size, in_file);
    if (bytes_read == 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. Initialize decoder (try VP9 first, then VP8)
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION);
    
    if (res != VPX_CODEC_OK) {
        // Try VP8
        decoder_iface = vpx_codec_vp8_dx();
        res = vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION);
    }

    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Set frame buffer functions for decoder
    res = vpx_codec_set_frame_buffer_functions(&decoder,  id_000030_get_frame_buffer,  id_000030_release_frame_buffer, nullptr);
    if (res != VPX_CODEC_OK) {
        // Continue anyway - this is fuzzing
    }

    // 4. Register put frame callback
    res = vpx_codec_register_put_frame_cb(&decoder,  id_000030_put_frame_callback, nullptr);
    if (res != VPX_CODEC_OK) {
        // Continue anyway
    }

    // 5. Decode the input data
    res = vpx_codec_decode(&decoder, buffer.data(), buffer.size(), nullptr, 0);
    if (res != VPX_CODEC_OK) {
        // Get error message
        const char *error = vpx_codec_error(&decoder);
        (void)error; // Use error in debug scenarios
    }

    // 6. Get stream information
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    res = vpx_codec_get_stream_info(&decoder, &stream_info);
    if (res != VPX_CODEC_OK) {
        // Use default values
        stream_info.w = 320;
        stream_info.h = 240;
    }

    // 7. Allocate image using vpx_img_alloc
    image = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, stream_info.w, stream_info.h, 16);
    if (!image) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Fill image with some data for encoding
    for (unsigned int i = 0; i < image->d_h; ++i) {
        memset(image->planes[0] + i * image->stride[0], i % 256, image->d_w);
    }

    // 8. Initialize encoder (multi-encoder initialization)
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(image);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    enc_cfg.g_w = image->d_w;
    enc_cfg.g_h = image->d_h;
    enc_cfg.g_timebase = timebase;
    enc_cfg.rc_target_bitrate = 1000; // 1 Mbps

    // Initialize multiple encoders (1 in this case)
    res = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &enc_cfg, 1, 0, &timebase, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(image);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 9. Use vpx_codec_control_ to set encoder options
    // Set CPU used to 1 for faster encoding
    vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 1);

    // 10. Get global headers from encoder
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers && global_headers->buf) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }

    // 11. Encode the image
    res = vpx_codec_encode(&encoder, image, 0, 1, 0, VPX_DL_REALTIME);
    if (res == VPX_CODEC_OK) {
        // Get encoded data
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
    }

    // 12. Cleanup
    vpx_codec_destroy(&encoder);
    vpx_img_free(image);
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}