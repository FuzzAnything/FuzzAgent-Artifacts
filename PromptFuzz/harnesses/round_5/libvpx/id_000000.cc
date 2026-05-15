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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["vpx_codec_version_str","vpx_codec_vp9_cx","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_img_alloc","vpx_codec_destroy","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_vp9_cx","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_img_alloc","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Use vpx_codec_version_str to get version string
    const char *version_str = vpx_codec_version_str();
    (void)version_str; // Use to avoid unused variable warning
    
    // 2. Use vpx_codec_get_caps for both encoder and decoder
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    vpx_codec_caps_t encoder_caps = vpx_codec_get_caps(encoder_iface);
    vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
    (void)encoder_caps; // Use to avoid unused variable warning
    (void)decoder_caps; // Use to avoid unused variable warning
    
    // 3. Create a FILE* to read the input data (libvpx output data)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create output file for re-encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Peek at stream info to get image dimensions
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    // Read input data for stream info
    fseek(in_file, 0, SEEK_SET);
    uint8_t *buffer = (uint8_t *)malloc(size);
    if (!buffer) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    fread(buffer, 1, size, in_file);
    vpx_codec_peek_stream_info(decoder_iface, buffer, (unsigned int)size, &si);
    
    // 7. Use vpx_img_alloc to allocate image for decoded frame
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, si.w, si.h, 1);
    if (!img) {
        free(buffer);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 8. Decode the input data
    res = vpx_codec_decode(&decoder, buffer, (unsigned int)size, NULL, 0);
    
    // 9. Use vpx_codec_error_detail if there's an error
    if (res != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail; // Use to avoid unused variable warning
    }
    
    // 10. Get decoded frame
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
        // Copy decoded image to our allocated image
        for (int plane = 0; plane < 3; plane++) {
            uint8_t *src = decoded_img->planes[plane];
            uint8_t *dst = img->planes[plane];
            int src_stride = decoded_img->stride[plane];
            int dst_stride = img->stride[plane];
            int height = (plane == 0) ? si.h : si.h / 2;
            int width = (plane == 0) ? si.w : si.w / 2;
            
            for (int y = 0; y < height; y++) {
                memcpy(dst, src, width);
                src += src_stride;
                dst += dst_stride;
            }
        }
        
        // 11. Initialize encoder configuration using vpx_codec_enc_config_default
        vpx_codec_enc_cfg_t cfg;
        vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
        
        // Set encoder configuration based on decoded image
        cfg.g_w = si.w;
        cfg.g_h = si.h;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
        cfg.g_pass = VPX_RC_ONE_PASS;
        cfg.g_lag_in_frames = 0;
        
        // 12. Initialize encoder
        vpx_codec_ctx_t encoder;
        res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
        
        if (res == VPX_CODEC_OK) {
            // 13. Encode the image
            res = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
            
            if (res == VPX_CODEC_OK) {
                // 14. Get encoded data
                vpx_codec_iter_t enc_iter = NULL;
                const vpx_codec_cx_pkt_t *pkt;
                
                while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        // Write encoded data to output file
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
            }
            
            // 15. Clean up encoder
            vpx_codec_destroy(&encoder);
        }
    }
    
    // 16. Clean up all resources
    vpx_img_free(img);
    free(buffer);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}