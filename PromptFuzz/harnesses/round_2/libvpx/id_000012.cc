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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["vpx_codec_version_extra_str","vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_get_global_headers","vpx_codec_decode","vpx_codec_error","vpx_codec_get_preview_frame","vpx_codec_get_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_extra_str","vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_get_global_headers","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_get_frame","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000012_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Do nothing in fuzzing context
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Get version extra string
    const char *version_extra = vpx_codec_version_extra_str();
    (void)version_extra; // Mark as used to avoid warnings

    // 2. Try both VP8 and VP9 decoders
    vpx_codec_iface_t *codec_iface_vp8 = vpx_codec_vp8_dx();
    vpx_codec_iface_t *codec_iface_vp9 = vpx_codec_vp9_dx();
    
    // 3. Peek stream info for VP8
    vpx_codec_stream_info_t stream_info_vp8;
    stream_info_vp8.sz = sizeof(stream_info_vp8);
    vpx_codec_err_t peek_result_vp8 = vpx_codec_peek_stream_info(
        codec_iface_vp8, data, (unsigned int)size, &stream_info_vp8);
    
    // 4. Peek stream info for VP9
    vpx_codec_stream_info_t stream_info_vp9;
    stream_info_vp9.sz = sizeof(stream_info_vp9);
    vpx_codec_err_t peek_result_vp9 = vpx_codec_peek_stream_info(
        codec_iface_vp9, data, (unsigned int)size, &stream_info_vp9);

    // 5. Initialize decoder based on which codec works
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *selected_iface = nullptr;
    
    if (peek_result_vp8 == VPX_CODEC_OK) {
        selected_iface = codec_iface_vp8;
    } else if (peek_result_vp9 == VPX_CODEC_OK) {
        selected_iface = codec_iface_vp9;
    } else {
        // Neither codec recognized the data, exit early
        return 0;
    }

    // Initialize decoder
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    vpx_codec_err_t init_result = vpx_codec_dec_init_ver(
        &decoder, selected_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (init_result != VPX_CODEC_OK) {
        // Failed to initialize decoder
        return 0;
    }

    // 6. Register put_frame callback
    vpx_codec_err_t register_result = vpx_codec_register_put_frame_cb(
        &decoder,  id_000012_put_frame_callback, nullptr);
    (void)register_result; // Result may fail, but we still continue

    // 7. Try to get global headers (even though this is typically for encoders)
    // This may fail for decoders, but we call it anyway
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&decoder);
    (void)global_headers;

    // 8. Decode the input data
    vpx_codec_err_t decode_result = vpx_codec_decode(
        &decoder, data, (unsigned int)size, nullptr, 0);
    
    // 9. Get error message if decode failed
    if (decode_result != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_error(&decoder);
        (void)error_msg;
    } else {
        // 10. Try to get preview frame (this may return NULL)
        const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder);
        (void)preview_frame;

        // 11. Get decoded frames
        vpx_codec_iter_t iter = nullptr;
        const vpx_image_t *frame;
        while ((frame = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
            // Mark frame as used
            (void)frame;
        }
    }

    // 12. Clean up
    vpx_codec_destroy(&decoder);

    return 0;
}