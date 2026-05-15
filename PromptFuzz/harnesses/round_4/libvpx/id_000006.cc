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
//<score> 56, nr_unique_branch: 14
//<Quality> {"density":7,"unique_branches":{"vp8_get_frame":[[530,7,530,15,0,0,4,1]],"validate_config":[[125,54,125,71,1,0,4,1],[125,54,125,71,2,0,4,1]],"vp8e_init":[[740,9,740,13,0,0,4,1],[749,11,749,21,0,0,4,0]],"vp8_remove_compressor":[[2105,7,2105,11,0,0,4,0]],"vp8_create_compressor":[[1858,8,1858,38,0,0,4,0],[1863,7,1863,38,0,0,4,1],[1866,14,1866,45,0,0,4,1],[1876,7,1876,38,0,0,4,1]],"vp8_change_config":[[1709,7,1709,35,0,0,4,1],[1713,12,1713,53,0,0,4,1]],"vp8_alloc_compressor_data":[[1225,14,1225,27,0,0,4,1],[440,9,440,16,3,0,4,0]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


// Include necessary libvpx headers

// Simple slice callback function for demonstration
static void  id_000006_put_slice_callback(void *user_priv, 
                               const vpx_image_t *img,
                               const vpx_image_rect_t *valid,
                               const vpx_image_rect_t *update) {
    // This callback can be used to process slices during decoding
    (void)user_priv;
    (void)img;
    (void)valid;
    (void)update;
    // Do nothing in fuzzing context
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);

    // Determine which codec interface to use based on input data
    vpx_codec_iface_t *codec_iface = nullptr;
    
    // Try VP8 first
    codec_iface = vpx_codec_vp8_dx();
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        codec_iface, data, (unsigned int)(size > 1000 ? 1000 : size), &stream_info);
    
    // If VP8 fails, try VP9
    if (peek_result != VPX_CODEC_OK) {
        codec_iface = vpx_codec_vp9_dx();
        peek_result = vpx_codec_peek_stream_info(
            codec_iface, data, (unsigned int)(size > 1000 ? 1000 : size), &stream_info);
    }
    
    if (peek_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(codec_iface);
    (void)caps; // Use caps to avoid unused variable warning

    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (vpx_codec_dec_init(&decoder, codec_iface, &dec_cfg, 0) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Register put_slice callback (if supported)
    vpx_codec_register_put_slice_cb(&decoder,  id_000006_put_slice_callback, nullptr);

    // Allocate image for potential use
    vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                     stream_info.w, stream_info.h, fuzz_uint32_t_1);
    
    // Decode the input data
    vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, fuzz_int64_t_2);
    
    // Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *decoded_img;
    while ((decoded_img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Process decoded frame if needed
        (void)decoded_img;
    }

    // Now test encoding path with the allocated image
    if (img && img->w > 0 && img->h > 0) {
        // Initialize encoder
        vpx_codec_ctx_t encoder;
        vpx_codec_enc_cfg_t enc_cfg;
        
        // Get default encoder configuration
        vpx_codec_iface_t *enc_iface = vpx_codec_vp8_cx();
        if (vpx_codec_enc_config_default(enc_iface, &enc_cfg, fuzz_uint32_t_3) == VPX_CODEC_OK) {
            enc_cfg.g_w = img->w;
            enc_cfg.g_h = img->h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            
            if (vpx_codec_enc_init(&encoder, enc_iface, &enc_cfg, 0) == VPX_CODEC_OK) {
                // Encode the allocated image
                vpx_codec_encode(&encoder, img, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7);
                
                // Get encoded data
                vpx_codec_iter_t enc_iter = nullptr;
                const vpx_codec_cx_pkt_t *pkt;
                while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        // Write encoded data to output file
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                // Destroy encoder
                vpx_codec_destroy(&encoder);
            }
        }
    }

    // Cleanup
    if (img) {
        vpx_img_free(img);
    }
    
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}