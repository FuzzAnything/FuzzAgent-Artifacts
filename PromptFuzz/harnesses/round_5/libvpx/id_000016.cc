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
//<score> 66, nr_unique_branch: 17
//<Quality> {"density":12,"unique_branches":{"vp8e_encode":[[925,7,925,10,0,0,4,1],[989,9,989,20,0,0,4,1]],"vpx_codec_control_":[[101,41,101,50,0,0,4,1]],"vp9_lookahead_pop":[[168,7,168,10,0,0,4,1]],"vp8_change_config":[[1709,7,1709,35,0,0,4,1],[1713,12,1713,53,0,0,4,1]],"vp8e_init":[[749,11,749,21,0,0,4,0]],"vp8_create_compressor":[[1863,7,1863,38,0,0,4,1],[1866,14,1866,45,0,0,4,1]],"vpx_codec_encode":[[199,16,199,19,0,0,4,1]],"encoder_encode":[[1401,7,1401,18,0,0,4,1]],"vp8_alloc_compressor_data":[[1223,14,1223,27,0,0,4,1],[1225,14,1225,27,0,0,4,0],[1225,14,1225,27,0,0,4,1],[440,9,440,16,3,0,4,0]],"vp8_get_compressed_data":[[4916,9,4916,14,0,0,4,0],[4916,18,4916,32,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_cx","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_control_","vpx_codec_control_","vpx_codec_get_global_headers","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_codec_encode","vpx_img_wrap","vpx_codec_get_cx_data","vpx_codec_encode","vpx_img_wrap","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


// Include libvpx headers

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
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
    
    // Variables for codec interfaces
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;
    
    // Try to determine codec type from input data
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Try VP9 first
    decoder_iface = vpx_codec_vp9_dx();
    if (vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info) == VPX_CODEC_OK) {
        encoder_iface = vpx_codec_vp9_cx();
    } else {
        // Try VP8
        decoder_iface = vpx_codec_vp8_dx();
        if (vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info) == VPX_CODEC_OK) {
            encoder_iface = vpx_codec_vp8_cx();
        } else {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Get codec interface name
    const char *iface_name = vpx_codec_iface_name(decoder_iface);
    (void)iface_name; // Use variable to avoid warning
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration based on stream info
    enc_cfg.g_w = stream_info.w;
    enc_cfg.g_h = stream_info.h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_threads = 1;
    enc_cfg.rc_target_bitrate = 1000;
    
    if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder control (quality)
    vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 6);
    vpx_codec_control_(&encoder, VP8E_SET_NOISE_SENSITIVITY, 0);
    
    // Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }
    
    // Callback structure for put_frame
    struct CallbackData {
        vpx_codec_ctx_t *encoder;
        FILE *out_file;
        int frame_count;
    };
    
    CallbackData cb_data = {&encoder, out_file, 0};
    
    // Define put_frame callback
    auto put_frame_callback = [](void *user_priv, const vpx_image_t *img) {
        CallbackData *data = static_cast<CallbackData*>(user_priv);
        
        // Create a wrapped image for encoding
        vpx_image_t wrapped_img;
        if (img->img_data) {
            vpx_img_wrap(&wrapped_img, img->fmt, img->d_w, img->d_h, 
                         img->stride[0], const_cast<unsigned char*>(img->img_data));
            
            // Encode the frame
            vpx_codec_pts_t pts = data->frame_count * 1000 / 30;
            if (vpx_codec_encode(data->encoder, &wrapped_img, pts, 1, 0, VPX_DL_REALTIME) == VPX_CODEC_OK) {
                vpx_codec_iter_t iter = nullptr;
                const vpx_codec_cx_pkt_t *pkt;
                
                while ((pkt = vpx_codec_get_cx_data(data->encoder, &iter)) != nullptr) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, data->out_file);
                    }
                }
                
                data->frame_count++;
            }
            
            // Note: We don't call vpx_img_free on wrapped_img since we didn't allocate new memory
        }
    };
    
    // Register put_frame callback
    vpx_codec_register_put_frame_cb(&decoder, put_frame_callback, &cb_data);
    
    // Decode the input data
    if (vpx_codec_decode(&decoder, data, size, nullptr, 0) == VPX_CODEC_OK) {
        // Get stream info from decoder
        vpx_codec_stream_info_t dec_stream_info;
        memset(&dec_stream_info, 0, sizeof(dec_stream_info));
        dec_stream_info.sz = sizeof(dec_stream_info);
        
        if (vpx_codec_get_stream_info(&decoder, &dec_stream_info) == VPX_CODEC_OK) {
            // The callback should have been called during decoding
        }
    }
    
    // Flush encoder
    vpx_codec_encode(&encoder, nullptr, -1, 1, 0, VPX_DL_REALTIME);
    
    // Cleanup
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}