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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"vp9_set_row_mt":[[6857,35,6857,57,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    vpx_codec_ctx_t decoder_ctx = {0};
    vpx_codec_ctx_t encoder_ctx = {0};
    vpx_codec_stream_info_t stream_info = {0};
    vpx_codec_enc_cfg_t enc_cfg = {0};
    vpx_image_t *img = nullptr;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    int ret_val = 0;
    stream_info.sz = sizeof(stream_info);

    // 1. Get codec capabilities first
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps; // Prevent unused warning

    // 2. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Initialize decoder
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 320;
    dec_cfg.h = 240;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, 
                                                 &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Set custom frame buffer functions
    err = vpx_codec_set_frame_buffer_functions(&decoder_ctx, nullptr, nullptr, nullptr);
    (void)err; // Continue even if this fails

    // 5. Read data for stream info
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (file_size > 0) {
        uint8_t *stream_data = (uint8_t *)malloc(file_size);
        if (stream_data) {
            fread(stream_data, 1, file_size, in_file);
            
            // 6. Get stream information
            err = vpx_codec_get_stream_info(&decoder_ctx, &stream_info);
            (void)err;
            
            free(stream_data);
        }
    }

    // 7. Initialize encoder
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err == VPX_CODEC_OK) {
        enc_cfg.g_w = 320;
        enc_cfg.g_h = 240;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        
        err = vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 
                                     0, VPX_ENCODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
            // 8. Create test image
            img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 320, 240, 1);
            if (img) {
                // Fill with some test data
                for (int i = 0; i < 320 * 240 * 3 / 2; i++) {
                    if (i < 320 * 240) {
                        img->planes[0][i] = i % 256;
                    }
                }
                
                // 9. Encode the image
                err = vpx_codec_encode(&encoder_ctx, img, 0, 1, 0, 0);
                if (err == VPX_CODEC_OK) {
                    // 10. Get encoded data
                    while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, &iter)) != nullptr) {
                        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                            // Successfully got encoded packet
                            ret_val = 1;
                        }
                    }
                }
                
                vpx_img_free(img);
            }
        }
    }

    // 11. Cleanup
    if (encoder_ctx.iface) {
        vpx_codec_destroy(&encoder_ctx);
    }
    if (decoder_ctx.iface) {
        vpx_codec_destroy(&decoder_ctx);
    }
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return ret_val;
}