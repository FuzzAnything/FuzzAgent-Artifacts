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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_error_detail","vpx_codec_destroy","vpx_img_wrap","vpx_codec_destroy","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if no data
    if (size == 0) {
	return 0;
	}
	

    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Initialize decoder for VP9 (using VP9 codec interface)
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 2;
    
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read and decode the input data
    std::vector<uint8_t> compressed_data(size);
    fread(compressed_data.data(), 1, size, in_file);
    assert_file_closed(&in_file);;

    vpx_codec_decode(&decoder, compressed_data.data(), compressed_data.size(), 
                     nullptr, 0);

    // Try to get decoded frames
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    
    if (!decoded_frame) {
        // Try to get error detail if no frame was decoded
        const char *error_detail = vpx_codec_error_detail(&decoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize encoder for re-encoding the frame
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    // Get default encoder configuration
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    // Set basic encoder parameters based on decoded frame
    enc_cfg.g_w = decoded_frame->d_w;
    enc_cfg.g_h = decoded_frame->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 256;  // 256 kbps
    
    vpx_codec_err_t enc_init_result = vpx_codec_enc_init_ver(
        &encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    if (enc_init_result != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&encoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a wrapped image for encoding using the decoded frame data
    // Note: We're using the decoded frame's data directly here for demonstration
    vpx_image_t wrapped_image;
    memset(&wrapped_image, 0, sizeof(wrapped_image));
    
    vpx_image_t *wrapped_img_result = vpx_img_wrap(
        &wrapped_image, 
        VPX_IMG_FMT_I420,  // Common format for encoding
        decoded_frame->d_w, 
        decoded_frame->d_h,
        1,  // stride alignment
        decoded_frame->planes[0]  // Using Y plane from decoded frame
    );
    
    if (!wrapped_img_result) {
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set a rectangle on the wrapped image (using entire image area)
    int set_rect_result = vpx_img_set_rect(
        &wrapped_image, 
        0, 0,  // x, y
        decoded_frame->d_w,  // width
        decoded_frame->d_h   // height
    );

    // Encode the wrapped image
    vpx_codec_encode(&encoder, &wrapped_image, 0, 1, 0, VPX_DL_REALTIME);

    // Get encoded data packets
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Encoded frame data available in pkt->data.frame.buf
            // Could write to file or process further
            break;
        }
    }

    // Clean up resources
    // Note: vpx_img_wrap doesn't allocate new memory for image data,
    // so we don't need to free the image data separately
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);

    assert_file_closed(&in_file);
	return 0;
}