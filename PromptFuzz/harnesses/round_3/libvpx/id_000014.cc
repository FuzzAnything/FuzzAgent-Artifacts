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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"vpx_codec_error_detail":[[60,7,60,10,0,0,4,1]]},"library_calls":["vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_img_free","vpx_codec_destroy","vpx_codec_error_detail"],"critical_calls":["vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_img_free","vpx_codec_destroy","vpx_codec_error_detail"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callback structure
struct FrameBufferCallback {
    std::vector<uint8_t> buffer;
    size_t size_used;
};

// Custom get frame buffer callback
int  id_000014_get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    FrameBufferCallback *callback = static_cast<FrameBufferCallback*>(cb_priv);
    
    // Ensure buffer is large enough
    if (callback->buffer.size() < min_size) {
        callback->buffer.resize(min_size);
    }
    
    fb->data = callback->buffer.data();
    fb->size = callback->buffer.size();
    callback->size_used = min_size;
    
    return 0;
}

// Custom release frame buffer callback
int  id_000014_release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    FrameBufferCallback *callback = static_cast<FrameBufferCallback*>(cb_priv);
    callback->size_used = 0;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Create a FILE* for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read a portion of data for stream info
    std::vector<uint8_t> stream_data(size);
    fread(stream_data.data(), 1, size, in_file);
    assert_file_closed(&in_file);;

    // Try to peek stream info for both VP8 and VP9
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);

    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        vpx_codec_vp8_dx(), 
        stream_data.data(), 
        std::min(size, (size_t)1000), // Use first 1000 bytes for peek
        &stream_info
    );

    // If VP8 fails, try VP9
    if (peek_result != VPX_CODEC_OK) {
        peek_result = vpx_codec_peek_stream_info(
            vpx_codec_vp9_dx(),
            stream_data.data(),
            std::min(size, (size_t)1000),
            &stream_info
        );
    }

    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    
    // Try VP8 first, then VP9
    vpx_codec_err_t init_result = vpx_codec_dec_init_ver(
        &decoder, 
        decoder_iface, 
        nullptr, 
        fuzz_int64_t_1, 
        VPX_DECODER_ABI_VERSION
    );
    
    if (init_result != VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp9_dx();
        init_result = vpx_codec_dec_init_ver(
            &decoder, 
            decoder_iface, 
            nullptr, 
            fuzz_int64_t_2, 
            VPX_DECODER_ABI_VERSION
        );
    }

    if (init_result == VPX_CODEC_OK) {
        // Set up frame buffer callbacks
        FrameBufferCallback frame_buffer_cb;
        vpx_codec_err_t set_buffer_result = vpx_codec_set_frame_buffer_functions(
            &decoder,
             id_000014_get_frame_buffer_cb,
             id_000014_release_frame_buffer_cb,
            &frame_buffer_cb
        );

        // Create a fixed buffer for encoded data
        vpx_fixed_buf_t encoded_buf;
        std::vector<uint8_t> encoded_data(size);
        encoded_buf.buf = encoded_data.data();
        encoded_buf.sz = size;
        
        // Set the buffer for encoded data (simulating encoder context)
        // Note: This API is typically used with encoder, but we'll call it anyway
        vpx_codec_err_t set_cx_result = vpx_codec_set_cx_data_buf(
            &decoder,
            &encoded_buf,
            fuzz_uint32_t_3,  // pad_before
            fuzz_uint32_t_4   // pad_after
        );

        // Try to decode the input data
        vpx_codec_err_t decode_result = vpx_codec_decode(
            &decoder,
            stream_data.data(),
            stream_data.size(),
            nullptr,
            fuzz_int64_t_5
        );

        // Get error detail if available
        const char *error_detail = vpx_codec_error_detail(&decoder);

        // Try to get decoded frames
        vpx_codec_iter_t iter = nullptr;
        vpx_image_t *img;
        while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
            // Create a copy of the image to work with
            vpx_image_t *img_copy = vpx_img_alloc(nullptr, img->fmt, img->d_w, img->d_h, fuzz_uint32_t_6);
            if (img_copy) {
                // Copy image data
                for (int plane = 0; plane < 3; ++plane) {
                    if (img->planes[plane]) {
                        size_t plane_size = img->stride[plane] * 
                                          (plane == 0 ? img->d_h : (img->d_h + 1) >> 1);
                        memcpy(img_copy->planes[plane], img->planes[plane], plane_size);
                    }
                }

                // Set a rectangle on the image using fuzzed data for coordinates
                // Use modulo to ensure valid coordinates
                unsigned int x = data[0] % (img_copy->d_w > 0 ? img_copy->d_w : 1);
                unsigned int y = data[1] % (img_copy->d_h > 0 ? img_copy->d_h : 1);
                unsigned int w = data[2] % (img_copy->d_w - x > 0 ? img_copy->d_w - x : 1);
                unsigned int h = data[3] % (img_copy->d_h - y > 0 ? img_copy->d_h - y : 1);
                
                if (w > 0 && h > 0) {
                    vpx_img_set_rect(img_copy, x, y, w, h);
                }

                vpx_img_free(img_copy);
            }
        }

        // Clean up decoder
        vpx_codec_destroy(&decoder);
    }

    // Also try to use the error detail API with a null context
    const char *null_error_detail = vpx_codec_error_detail(nullptr);

    assert_file_closed(&in_file);
	return 0;
}