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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_error_detail","vpx_img_alloc","vpx_codec_destroy","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/*Looking at the libvpx APIs and the instructions, I'll create a fuzzer that tests libvpx's encoding and decoding functionality with a custom put_slice callback and frame buffer management. Here's the step-by-step implementation:

*/


extern "C" {
}

// Global file handle for output
static FILE*  id_000008_output_file = nullptr;

// Custom frame buffer context structure
struct FrameBufferContext {
    std::vector<uint8_t*> buffers;
};

// Frame buffer get callback
static int  id_000008_get_frame_buffer_cb(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    FrameBufferContext* ctx = static_cast<FrameBufferContext*>(cb_priv);
    uint8_t* buf = static_cast<uint8_t*>(malloc(min_size));
    if (!buf) {
        return -1;
    }
    
    fb->data = buf;
    fb->size = min_size;
    ctx->buffers.push_back(buf);
    return 0;
}

// Frame buffer release callback
static int  id_000008_release_frame_buffer_cb(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    if (fb && fb->data) {
        free(fb->data);
    }
    return 0;
}

// Put slice callback - writes decoded slices to file
static void  id_000008_put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* slice_rect,
                               const vpx_image_rect_t* clip_rect) {
    if (img &&  id_000008_output_file && slice_rect) {
        // Write basic slice info to output file
        uint32_t info[4] = {
            static_cast<uint32_t>(slice_rect->x),
            static_cast<uint32_t>(slice_rect->y),
            static_cast<uint32_t>(slice_rect->w),
            static_cast<uint32_t>(slice_rect->h)
        };
        fwrite(info, sizeof(uint32_t), 4,  id_000008_output_file);
    }
}

// Fuzzer entry point
extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }
    
    // Open input file from memory buffer
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open output file
     id_000008_output_file = fopen(" id_000008_output_file", "wb");
    if (! id_000008_output_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(& id_000008_output_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize encoder context
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
    
    // Get default encoder configuration
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(& id_000008_output_file);;
        assert_file_closed(& id_000008_output_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set basic encoder parameters
    cfg.g_w = 320;
    cfg.g_h = 240;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    
    // Initialize encoder
    if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        const char* error_detail = vpx_codec_error_detail(&encoder);
        if (error_detail) {
            fwrite(error_detail, 1, strlen(error_detail),  id_000008_output_file);
        }
        assert_file_closed(&in_file);;
        assert_file_closed(& id_000008_output_file);;
        assert_file_closed(& id_000008_output_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create an input image
    vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 320, 240, 1);
    if (!img) {
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(& id_000008_output_file);;
        assert_file_closed(& id_000008_output_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read some data from input file to populate image (simulate real data)
    uint8_t yuv_buffer[320 * 240 * 3 / 2] = {0};
    size_t bytes_read = fread(yuv_buffer, 1, std::min(sizeof(yuv_buffer), size), in_file);
    
    // Populate image planes with some test data
    for (unsigned int i = 0; i < img->d_h; ++i) {
        memset(img->planes[0] + i * img->stride[0], 0x80, img->d_w);
    }
    for (unsigned int i = 0; i < img->d_h / 2; ++i) {
        memset(img->planes[1] + i * img->stride[1], 0x80, img->d_w / 2);
        memset(img->planes[2] + i * img->stride[2], 0x80, img->d_w / 2);
    }
    
    // Encode the image
    if (vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME) != VPX_CODEC_OK) {
        const char* error_detail = vpx_codec_error_detail(&encoder);
        if (error_detail) {
            fwrite(error_detail, 1, strlen(error_detail),  id_000008_output_file);
        }
    }
    
    // Get encoded data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t* pkt;
    std::vector<uint8_t> encoded_data;
    
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            encoded_data.insert(encoded_data.end(),
                               static_cast<const uint8_t*>(pkt->data.frame.buf),
                               static_cast<const uint8_t*>(pkt->data.frame.buf) + pkt->data.frame.sz);
        }
    }
    
    // Set up decoder context with custom frame buffers
    vpx_codec_ctx_t decoder;
    FrameBufferContext fb_ctx;
    
    // Try to decode with VP8 decoder
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp8_dx();
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
        // Set frame buffer functions
        vpx_codec_set_frame_buffer_functions(&decoder,  id_000008_get_frame_buffer_cb,
                                             id_000008_release_frame_buffer_cb, &fb_ctx);
        
        // Register put slice callback
        vpx_codec_register_put_slice_cb(&decoder,  id_000008_put_slice_callback, nullptr);
        
        // Decode the encoded data
        if (!encoded_data.empty()) {
            vpx_codec_decode(&decoder, encoded_data.data(),
                           static_cast<unsigned int>(encoded_data.size()),
                           nullptr, 0);
            
            // Check for errors
            const char* error_detail = vpx_codec_error_detail(&decoder);
            if (error_detail) {
                fwrite(error_detail, 1, strlen(error_detail),  id_000008_output_file);
            }
            
            // Get decoded frames
            vpx_codec_iter_t dec_iter = nullptr;
            const vpx_image_t* dec_img;
            while ((dec_img = vpx_codec_get_frame(&decoder, &dec_iter)) != nullptr) {
                // Frame is available, callback would have been called during decoding
            }
        }
        
        // Clean up decoder
        vpx_codec_destroy(&decoder);
    }
    
    // Clean up frame buffers
    for (auto* buf : fb_ctx.buffers) {
        free(buf);
    }
    
    // Free image
    vpx_img_free(img);
    
    // Destroy encoder
    vpx_codec_destroy(&encoder);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(& id_000008_output_file);;
    
    assert_file_closed(& id_000008_output_file);
	assert_file_closed(&in_file);
	return 0;
}