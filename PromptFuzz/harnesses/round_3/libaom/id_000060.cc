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
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_enc_config_set","aom_img_alloc","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_img_wrap","aom_codec_set_frame_buffer_functions","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error_detail","aom_codec_err_to_string","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_uleb_encode_fixed_size","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_enc_config_set","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_wrap","aom_codec_set_frame_buffer_functions","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error_detail","aom_codec_err_to_string","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (!data || size < 100) {
	return 0;
	}
	
    
    // 1. ULEB decoding
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    int uleb_decode_result = aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    
    // 2. ULEB encoding with fixed size
    uint8_t encoded_buffer[16];
    size_t encoded_size = 0;
    int uleb_encode_result = aom_uleb_encode_fixed_size(
        uleb_value, 
        sizeof(encoded_buffer), 
        8,  // pad to 8 bytes
        encoded_buffer, 
        &encoded_size
    );
    
    // 3. Create encoder context
    aom_codec_iface_t *codec_iface = aom_codec_av1_cx();
    aom_codec_ctx_t codec_ctx;
    aom_codec_enc_cfg_t cfg;
    
    // 4. Get default encoder configuration
    unsigned int usage = 0;  // Good quality
    aom_codec_err_t config_result = aom_codec_enc_config_default(
        codec_iface, 
        &cfg, 
        usage
    );
    
    if (config_result != AOM_CODEC_OK) {
        return 0;
    }
    
    // 5. Set custom configuration
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_bit_depth = AOM_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    
    // 6. Initialize encoder
    int encoder_init_result = aom_codec_enc_init_ver(
        &codec_ctx, 
        codec_iface, 
        &cfg, 
        0,  // flags
        AOM_ENCODER_ABI_VERSION
    );
    
    if (encoder_init_result != AOM_CODEC_OK) {
        return 0;
    }
    
    // 7. Set encoder option
    aom_codec_set_option(&codec_ctx, "cpu-used", "1");
    
    // 8. Apply configuration
    aom_codec_enc_config_set(&codec_ctx, &cfg);
    
    // 9. Create image using aom_img_alloc
    aom_image_t img_allocated;
    aom_image_t *img_ptr = aom_img_alloc(
        &img_allocated, 
        AOM_IMG_FMT_I420, 
        cfg.g_w, 
        cfg.g_h, 
        16  // alignment
    );
    
    if (!img_ptr) {
        aom_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // 10. Fill image with fuzzed data
    size_t data_offset = uleb_length;
    for (int plane = 0; plane < 3; ++plane) {
        int plane_width = aom_img_plane_width(img_ptr, plane);
        int plane_height = aom_img_plane_height(img_ptr, plane);
        uint8_t *plane_ptr = img_ptr->planes[plane];
        
        for (int y = 0; y < plane_height; ++y) {
            size_t bytes_to_copy = plane_width;
            if (data_offset + bytes_to_copy > size) {
                bytes_to_copy = size - data_offset;
            }
            if (bytes_to_copy > 0) {
                memcpy(plane_ptr, data + data_offset, bytes_to_copy);
                data_offset += bytes_to_copy;
            }
            plane_ptr += img_ptr->stride[plane];
        }
    }
    
    // 11. Create another image using aom_img_wrap
    size_t img_data_size = cfg.g_w * cfg.g_h * 3 / 2;  // I420 size
    std::vector<uint8_t> img_data(img_data_size);
    if (data_offset < size && img_data_size > 0) {
        size_t copy_size = std::min(img_data_size, size - data_offset);
        memcpy(img_data.data(), data + data_offset, copy_size);
    }
    
    aom_image_t img_wrapped;
    aom_image_t *wrapped_ptr = aom_img_wrap(
        &img_wrapped,
        AOM_IMG_FMT_I420,
        cfg.g_w,
        cfg.g_h,
        16,  // stride alignment
        img_data.data()
    );
    
    // 12. Set frame buffer functions (dummy implementation)
    struct FrameBufferContext {
        std::vector<uint8_t> buffer;
    };
    
    FrameBufferContext fb_ctx;
    fb_ctx.buffer.resize(cfg.g_w * cfg.g_h * 3);
    
    auto get_frame_buffer = [](void *cb_priv, size_t min_size,
                                aom_codec_frame_buffer_t *fb) -> int {
        FrameBufferContext *ctx = static_cast<FrameBufferContext*>(cb_priv);
        ctx->buffer.resize(min_size);
        fb->data = ctx->buffer.data();
        fb->size = min_size;
        return 0;
    };
    
    auto release_frame_buffer = [](void *cb_priv,
                                    aom_codec_frame_buffer_t *fb) -> int {
        return 0;
    };
    
    aom_codec_set_frame_buffer_functions(
        &codec_ctx,
        get_frame_buffer,
        release_frame_buffer,
        &fb_ctx
    );
    
    // 13. Set output buffer
    aom_fixed_buf_t output_buf;
    std::vector<uint8_t> output_data(4096);
    output_buf.buf = output_data.data();
    output_buf.sz = output_data.size();
    aom_codec_set_cx_data_buf(&codec_ctx, &output_buf, 0, 0);
    
    // 14. Encode frame
    aom_codec_encode(
        &codec_ctx,
        img_ptr,
        0,  // pts
        1,  // duration
        0   // flags
    );
    
    // 15. Get encoded data
    const aom_codec_cx_pkt_t *pkt = nullptr;
    aom_codec_iter_t iter = nullptr;
    while ((pkt = aom_codec_get_cx_data(&codec_ctx, &iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Encoded frame available
            break;
        }
    }
    
    // 16. Get error details (if any)
    const char *error_detail = aom_codec_error_detail(&codec_ctx);
    const char *error_string = aom_codec_err_to_string(codec_ctx.err);
    
    // 17. Cleanup
    aom_img_free(img_ptr);
    aom_codec_destroy(&codec_ctx);
    
    return 0;
}