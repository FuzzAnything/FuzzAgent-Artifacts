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
//<ID> 274
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"vpx_codec_get_cx_data":[[271,9,273,41,0,0,4,1]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Dummy callback functions for frame buffer management
static int  id_000063_get_frame_buffer_cb(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    static uint8_t dummy_buffer[4096];
    fb->data = dummy_buffer;
    fb->size = sizeof(dummy_buffer);
    return 0;
}

static int  id_000063_release_frame_buffer_cb(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    return 0;
}

// Dummy callback for put slice
static void  id_000063_put_slice_cb(void* user_priv, const vpx_image_t* img,
                         const vpx_image_rect_t* slice, const vpx_image_rect_t* updated) {
    // Do nothing in fuzzing context
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)updated;
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
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
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // Create output buffer for encoded data
    std::vector<uint8_t> output_buffer(size * 2);
    vpx_fixed_buf_t cx_buf;
    cx_buf.buf = output_buffer.data();
    cx_buf.sz = output_buffer.size();
    
    // Initialize codec context for encoding
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
    
    // Get codec interface name
    const char* iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; // Used to satisfy API call requirement
    
    // Initialize encoder configuration with default settings
    if (vpx_codec_enc_config_default(iface, &cfg, fuzz_uint32_t_1) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Adjust configuration based on input data
    cfg.g_w = (data[0] % 64 + 1) * 16;
    cfg.g_h = (data[1] % 64 + 1) * 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    // Initialize encoder
    if (vpx_codec_enc_init_ver(&codec_ctx, iface, &cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Set compressed data buffer
    unsigned int pad_before = data[2] % 16;
    unsigned int pad_after = data[3] % 16;
    vpx_codec_set_cx_data_buf(&codec_ctx, &cx_buf, pad_before, pad_after);
    
    // Set frame buffer functions
    void* cb_priv = nullptr;
    vpx_codec_set_frame_buffer_functions(&codec_ctx,  id_000063_get_frame_buffer_cb,
                                          id_000063_release_frame_buffer_cb, cb_priv);
    
    // Register put slice callback
    void* user_priv = nullptr;
    vpx_codec_register_put_slice_cb(&codec_ctx,  id_000063_put_slice_cb, user_priv);
    
    // Create input image
    vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420,
                                     cfg.g_w, cfg.g_h, fuzz_uint32_t_3);
    if (!img) {
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // Fill image with some data (simplified for fuzzing)
    size_t fill_size = std::min(size, static_cast<size_t>(img->d_w * img->d_h * 3 / 2));
    if (fill_size > 0) {
        size_t offset = 0;
        for (int plane = 0; plane < 3; ++plane) {
            uint8_t* plane_ptr = img->planes[plane];
            int stride = img->stride[plane];
            int height = plane == 0 ? img->d_h : img->d_h / 2;
            int width = plane == 0 ? img->d_w : img->d_w / 2;
            
            for (int y = 0; y < height; ++y) {
                size_t copy_size = std::min(static_cast<size_t>(width), size - offset);
                if (copy_size == 0) break;
                
                memcpy(plane_ptr + y * stride, data + offset, copy_size);
                offset += copy_size;
                if (offset >= size) break;
            }
            if (offset >= size) break;
        }
    }
    
    // Encode the image
    vpx_codec_encode(&codec_ctx, img, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7);
    
    // Get compressed data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t* pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process frame packet
            (void)pkt->data.frame.buf;
            (void)pkt->data.frame.sz;
        }
    }
    
    // Decode part - peek stream info from input data
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    
    // Use the input data to peek stream info
    size_t peek_size = std::min(size, static_cast<size_t>(1024));
    if (peek_size > 0) {
        vpx_codec_iface_t* dec_iface = vpx_codec_vp8_dx();
        vpx_codec_peek_stream_info(dec_iface, data, static_cast<unsigned int>(peek_size), &stream_info);
        
        // Try to decode if we have enough data
        vpx_codec_ctx_t dec_ctx;
        if (vpx_codec_dec_init_ver(&dec_ctx, dec_iface, nullptr, fuzz_int64_t_8, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
            // Create a FILE* for reading input data (as per instruction 4)
            FILE* in_file = fmemopen((void*)data, size, "rb");
            if (in_file) {
                // Read and decode in chunks
                std::vector<uint8_t> chunk(4096);
                size_t bytes_read;
                size_t total_read = 0;
                
                while (total_read < size) {
                    size_t to_read = std::min(chunk.size(), size - total_read);
                    bytes_read = fread(chunk.data(), 1, to_read, in_file);
                    if (bytes_read == 0) break;
                    
                    vpx_codec_decode(&dec_ctx, chunk.data(), static_cast<unsigned int>(bytes_read),
                                     nullptr, fuzz_int64_t_9);
                    
                    // Try to get decoded frame
                    vpx_codec_iter_t dec_iter = nullptr;
                    vpx_image_t* dec_img;
                    while ((dec_img = vpx_codec_get_frame(&dec_ctx, &dec_iter)) != nullptr) {
                        // Process decoded frame if needed
                        (void)dec_img;
                    }
                    
                    total_read += bytes_read;
                }
                
                assert_file_closed(&in_file);;
            }
            vpx_codec_destroy(&dec_ctx);
        }
    }
    
    // Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&codec_ctx);
    
    return 0;
}