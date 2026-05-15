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
//<ID> 301
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_enc_init_ver","vpx_codec_enc_config_default","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_enc_init_ver","vpx_codec_enc_config_default","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"visited":0}
/**/


extern "C" {
}

// Callback functions
static void  id_000077_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // Empty callback for fuzzing
    (void)user_priv;
    (void)img;
}

static void  id_000077_put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const vpx_image_rect_t *slice,
                         const vpx_image_rect_t *recon) {
    // Empty callback for fuzzing
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)recon;
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
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
    
    vpx_codec_ctx_t decoder;
    vpx_codec_ctx_t encoder;
    vpx_codec_stream_info_t stream_info;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_image_t *image = nullptr;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    
    // Try both VP8 and VP9 interfaces
    vpx_codec_iface_t* decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    vpx_codec_iface_t* encoder_ifaces[] = {
        vpx_codec_vp8_cx(),
        vpx_codec_vp9_cx()
    };
    
    for (int codec_idx = 0; codec_idx < 2; codec_idx++) {
        // 1. Use vpx_codec_peek_stream_info
        memset(&stream_info, 0, sizeof(stream_info));
        stream_info.sz = sizeof(stream_info);
        vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(
            decoder_ifaces[codec_idx], data, (unsigned int)size, &stream_info);
        (void)peek_err;
        
        // 2. Initialize decoder
        vpx_codec_err_t dec_init_err = vpx_codec_dec_init_ver(
            &decoder, decoder_ifaces[codec_idx], nullptr, 0, VPX_DECODER_ABI_VERSION);
        if (dec_init_err != VPX_CODEC_OK) continue;
        
        // 3. Register callbacks
        vpx_codec_register_put_frame_cb(&decoder,  id_000077_put_frame_cb, nullptr);
        vpx_codec_register_put_slice_cb(&decoder,  id_000077_put_slice_cb, nullptr);
        
        // 4. Decode the input
        vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, 0);
        
        // 5. Get stream info after decoding
        vpx_codec_get_stream_info(&decoder, &stream_info);
        
        // 6. Try to get decoded frames
        while ((image = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
            // 7. Use vpx_img_set_rect on the image
            if (image->w > 10 && image->h > 10) {
                vpx_img_set_rect(image, 5, 5, image->w - 10, image->h - 10);
            }
            
            // 8. Initialize encoder with default config
            vpx_codec_err_t enc_init_err = vpx_codec_enc_init_ver(
                &encoder, encoder_ifaces[codec_idx], nullptr, 0, VPX_ENCODER_ABI_VERSION);
            if (enc_init_err == VPX_CODEC_OK) {
                // 9. Get default encoder configuration
                vpx_codec_enc_config_default(encoder_ifaces[codec_idx], &enc_cfg, 0);
                
                // Set some reasonable values
                enc_cfg.g_w = image->w;
                enc_cfg.g_h = image->h;
                enc_cfg.rc_target_bitrate = 256;
                
                // 10. Use vpx_codec_enc_config_set
                vpx_codec_enc_config_set(&encoder, &enc_cfg);
                
                // 11. Encode the image
                vpx_codec_encode(&encoder, image, 0, 1, 0, VPX_DL_REALTIME);
                
                // 12. Use vpx_codec_get_cx_data
                while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        // Write to output file
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                // Clean up encoder
                vpx_codec_destroy(&encoder);
            }
        }
        
        // Clean up decoder
        vpx_codec_destroy(&decoder);
        
        // Break after first successful codec
        if (image != nullptr) break;
    }
    
    // Clean up
    if (image) {
        vpx_img_free(image);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}