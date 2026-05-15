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
//<ID> 212
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_stream_info","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_control_","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_stream_info","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_control_","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer functions
static int  id_000052_get_frame_buffer(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    (void)min_size;
    fb->data = static_cast<uint8_t*>(cb_priv);
    fb->size = 4096; // Fixed buffer size for simplicity
    return 0;
}

static int  id_000052_release_frame_buffer(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read encoded data from input
    std::vector<uint8_t> encoded_data(size);
    size_t bytes_read = fread(encoded_data.data(), 1, size, in_file);
    if (bytes_read == 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    assert_file_closed(&in_file);;

    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    const char *iface_name = vpx_codec_iface_name(decoder_iface);
    (void)iface_name; // Use the interface name

    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Decode the input data
    vpx_codec_decode(&decoder, encoded_data.data(), encoded_data.size(), nullptr, 0);

    // Get error detail
    const char *error_detail = vpx_codec_error_detail(&decoder);
    (void)error_detail; // Use error detail

    // Get stream info
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    if (vpx_codec_get_stream_info(&decoder, &stream_info) == VPX_CODEC_OK) {
        // Initialize encoder with stream info dimensions
        vpx_codec_ctx_t encoder;
        vpx_codec_enc_cfg_t encoder_cfg;
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();

        if (vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, 0) == VPX_CODEC_OK) {
            encoder_cfg.g_w = stream_info.w ? stream_info.w : 640;
            encoder_cfg.g_h = stream_info.h ? stream_info.h : 480;

            if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &encoder_cfg, 0, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                // Set frame buffer functions
                uint8_t frame_buffer[4096];
                vpx_codec_set_frame_buffer_functions(&encoder,  id_000052_get_frame_buffer,  id_000052_release_frame_buffer, frame_buffer);

                // Use control API to set some parameter
                vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);

                // Create a dummy image for encoding
                vpx_image_t dummy_image;
                vpx_img_alloc(&dummy_image, VPX_IMG_FMT_I420, encoder_cfg.g_w, encoder_cfg.g_h, 16);

                if (dummy_image.planes[0]) {
                    // Encode the dummy image
                    vpx_codec_encode(&encoder, &dummy_image, 0, 1, 0, VPX_DL_REALTIME);

                    // Clean up image
                    vpx_img_free(&dummy_image);
                }

                // Destroy encoder
                vpx_codec_destroy(&encoder);
            }
        }
    }

    // Clean up decoder
    vpx_codec_destroy(&decoder);

    assert_file_closed(&in_file);
	return 0;
}