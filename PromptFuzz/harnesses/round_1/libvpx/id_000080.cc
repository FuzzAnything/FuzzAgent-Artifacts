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
//<ID> 310
//<Prompt> []
/*<Combination>: [
*/
//<score> 108, nr_unique_branch: 63
//<Quality> {"density":18,"unique_branches":{"encode_frame_to_data_rate":[[3774,7,3774,39,0,0,4,0],[3777,10,3777,44,0,0,4,1],[3778,9,3778,32,0,0,4,0],[3778,36,3778,57,0,0,4,0],[3778,61,3778,75,0,0,4,0],[4370,7,4370,38,0,0,4,1],[4438,7,4438,37,0,0,4,0],[4724,7,4724,38,0,0,4,1]],"write_mb_features":[[360,7,360,30,0,0,4,0],[360,34,360,63,0,0,4,0],[362,7,362,13,0,0,4,0],[366,7,366,13,0,0,4,1],[370,7,370,13,0,0,4,1],[374,7,374,13,0,0,4,1],[380,7,380,14,0,0,4,1]],"cyclic_background_refresh":[[509,7,509,36,0,0,4,1],[534,7,534,42,0,0,4,1]],"vp8_encode_frame":[[815,11,815,35,0,0,4,0],[818,13,818,37,0,0,4,0],[819,23,819,53,0,0,4,0],[819,23,819,53,0,0,4,1],[820,25,820,30,0,0,4,0],[820,25,820,30,0,0,4,1],[906,7,906,31,0,0,4,0],[906,35,906,65,0,0,4,0],[916,9,916,18,0,0,4,0],[922,11,922,24,0,0,4,0],[928,11,928,24,0,0,4,1],[933,19,933,44,0,0,4,0],[933,19,933,44,0,0,4,1],[934,13,934,46,0,0,4,1]],"write_kfmodes":[[591,11,591,51,0,0,4,0]],"vp8_get_frame":[[530,7,530,15,0,0,4,1]],"vp8_get_compressed_data":[[4862,7,4862,42,0,0,4,1],[5130,7,5130,37,0,0,4,0],[5138,7,5138,31,0,0,4,0]],"vp8cx_mb_init_quantizer":[[298,7,298,31,0,0,4,0],[300,9,300,52,0,0,4,1],[309,16,309,29,0,0,4,0],[309,33,309,49,0,0,4,0],[321,7,321,18,0,0,4,1],[321,22,321,42,0,0,4,1],[387,14,387,59,0,0,4,1],[388,14,388,59,0,0,4,1],[389,14,389,53,0,0,4,1]],"thread_encoding_proc":[[53,9,53,61,0,0,4,0]],"encode_mb_row":[[426,9,426,33,0,0,4,0],[430,11,430,57,0,0,4,0]],"vp8_pack_bitstream":[[1075,7,1075,31,0,0,4,0],[1080,9,1080,40,0,0,4,0],[1086,19,1086,33,0,0,4,0],[1086,19,1086,33,0,0,4,1],[1088,21,1088,40,0,0,4,0],[1088,21,1088,40,0,0,4,1],[1092,15,1092,19,0,0,4,0],[1092,15,1092,19,0,0,4,1],[1095,17,1095,25,0,0,4,0],[1109,9,1109,39,0,0,4,0],[1111,19,1111,44,0,0,4,0],[1111,19,1111,44,0,0,4,1],[1114,13,1114,24,0,0,4,1],[1147,13,1147,43,0,0,4,0],[1244,7,1244,37,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_img_set_rect","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free"],"visited":1}
/*Here's a C++ fuzzer that demonstrates encoding and decoding with libvpx, using the specified APIs to process encoded video data:

*/


extern "C" {
}

// Frame buffer callback context
struct FrameBufferContext {
    uint8_t* data = nullptr;
    size_t size = 0;
};

// Frame buffer get callback
int  id_000080_get_frame_buffer(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    FrameBufferContext* ctx = static_cast<FrameBufferContext*>(cb_priv);
    
    if (min_size > ctx->size) {
        delete[] ctx->data;
        ctx->data = new uint8_t[min_size];
        ctx->size = min_size;
    }
    
    fb->data = ctx->data;
    fb->size = ctx->size;
    return 0;
}

// Frame buffer release callback
int  id_000080_release_frame_buffer(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    // Do nothing - we manage our own memory
    return 0;
}

// Put frame callback
void  id_000080_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // Empty callback for testing
}

// Put slice callback  
void  id_000080_put_slice_callback(void* user_priv, const vpx_image_t* img,
                        const vpx_image_rect_t* rect, const vpx_image_rect_t* rect2) {
    // Empty callback for testing
}

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create FILE* for input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    vpx_codec_ctx_t encoder = {};
    vpx_codec_enc_cfg_t cfg = {};
    vpx_codec_iface_t* iface = nullptr;
    vpx_image_t* img = nullptr;
    vpx_fixed_buf_t cx_buf = {};
    
    // Initialize encoder interface
    iface = vpx_codec_vp8_cx();
    
    // Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
    (void)caps; // Use variable
    
    // Set default encoder configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Configure encoder with random parameters from input data
    cfg.g_w = (data[0] % 128 + 1) * 16;
    cfg.g_h = (data[1] % 128 + 1) * 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256 + (data[2] % 2048);
    cfg.g_error_resilient = data[3] & 1;
    cfg.g_threads = 1 + (data[4] % 4);
    
    // Create rational for downsampling factor (using single encoder)
    vpx_rational_t dsf = {1, 1};
    
    // Initialize encoder (single encoder instance)
    if (vpx_codec_enc_init_multi_ver(&encoder, iface, &cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration
    if (vpx_codec_enc_config_set(&encoder, &cfg) != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up frame buffer functions
    FrameBufferContext fb_ctx;
    if (vpx_codec_set_frame_buffer_functions(&encoder,  id_000080_get_frame_buffer, 
                                             id_000080_release_frame_buffer, &fb_ctx) != VPX_CODEC_OK) {
        // Continue anyway - not all codecs support this
    }
    
    // Register callbacks
    if (vpx_codec_register_put_frame_cb(&encoder,  id_000080_put_frame_callback, nullptr) != VPX_CODEC_OK) {
        // Continue anyway
    }
    if (vpx_codec_register_put_slice_cb(&encoder,  id_000080_put_slice_callback, nullptr) != VPX_CODEC_OK) {
        // Continue anyway
    }
    
    // Allocate image buffer
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Generate random YUV data for image using input data
    size_t y_plane_size = img->stride[0] * cfg.g_h;
    size_t uv_plane_size = img->stride[1] * cfg.g_h / 2;
    
    // Use input data to populate image planes
    for (size_t i = 0; i < y_plane_size && i < size; i++) {
        img->planes[0][i] = data[i % size];
    }
    for (size_t i = 0; i < uv_plane_size && i < size; i++) {
        img->planes[1][i] = data[(i + 100) % size];
        img->planes[2][i] = data[(i + 200) % size];
    }
    
    // Set rectangle on image (using random coordinates from input)
    unsigned int x = data[10] % (cfg.g_w / 2);
    unsigned int y = data[11] % (cfg.g_h / 2);
    unsigned int w = 16 + (data[12] % 64);
    unsigned int h = 16 + (data[13] % 64);
    vpx_img_set_rect(img, x, y, w, h);
    
    // Prepare cx data buffer
    cx_buf.buf = malloc(4096);
    cx_buf.sz = 4096;
    if (cx_buf.buf) {
        vpx_codec_set_cx_data_buf(&encoder, &cx_buf, data[14] % 64, data[15] % 64);
    }
    
    // Encode frame
    vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
    
    // Get encoded data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t* pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter))) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Write to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Try to get preview frame
    const vpx_image_t* preview = vpx_codec_get_preview_frame(&encoder);
    (void)preview; // Use variable
    
    // Cleanup encoder
    vpx_codec_destroy(&encoder);
    
    // Now decode the output we just wrote
    if (out_file) {
        assert_file_closed(&out_file);;
        
        // Read output file back
        out_file = fopen("output_file", "rb");
        if (out_file) {
            fseek(out_file, 0, SEEK_END);
            long out_size = ftell(out_file);
            fseek(out_file, 0, SEEK_SET);
            
            if (out_size > 0) {
                uint8_t* out_data = new uint8_t[out_size];
                fread(out_data, 1, out_size, out_file);
                
                // Initialize decoder
                vpx_codec_ctx_t decoder = {};
                vpx_codec_iface_t* dec_iface = vpx_codec_vp8_dx();
                
                if (vpx_codec_dec_init_ver(&decoder, dec_iface, nullptr, 0, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
                    // Decode the data
                    vpx_codec_decode(&decoder, out_data, out_size, nullptr, 0);
                    
                    // Get decoded frames
                    vpx_codec_iter_t dec_iter = nullptr;
                    while (vpx_codec_get_frame(&decoder, &dec_iter)) {
                        // Frames processed
                    }
                    
                    vpx_codec_destroy(&decoder);
                }
                
                delete[] out_data;
            }
            assert_file_closed(&out_file);;
        }
    }
    
    // Free allocated resources
    if (img) vpx_img_free(img);
    if (cx_buf.buf) free(cx_buf.buf);
    if (fb_ctx.data) delete[] fb_ctx.data;
    
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}