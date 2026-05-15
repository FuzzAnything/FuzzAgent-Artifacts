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
//<ID> 33
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["aom_uleb_encode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error_detail","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_destroy","aom_img_wrap","aom_codec_destroy","aom_codec_enc_config_set","aom_img_free","aom_codec_destroy","aom_img_free","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_img_free","aom_codec_destroy","aom_codec_encode","aom_img_free","aom_codec_destroy","aom_img_add_metadata","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_encode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error_detail","aom_codec_set_frame_buffer_functions","aom_img_wrap","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_img_add_metadata","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback functions
static int  id_000023_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)min_size;
    fb->data = static_cast<uint8_t*>(cb_priv);
    fb->size = 4096; // Fixed size for simplicity
    return 0;
}

static int  id_000023_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing

    // 1. Use aom_uleb_encode
    uint64_t uleb_value = size % 1000;
    uint8_t coded_value[10];
    size_t coded_size = 0;
    aom_uleb_encode(uleb_value, sizeof(coded_value), coded_value, &coded_size);

    // 2. Create encoder interface
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
	return 0;
	}
	

    // 3. Get default encoder configuration
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = 0; // Good quality
    if (aom_codec_enc_config_default(encoder_iface, &cfg, usage) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some configuration parameters based on input data
    cfg.g_w = 64 + (data[0] % 64);
    cfg.g_h = 64 + (data[1] % 64);
    cfg.g_bit_depth = AOM_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.rc_end_usage = AOM_VBR;
    cfg.rc_target_bitrate = 1000 + (data[2] % 4000);
    cfg.g_threads = 1 + (data[3] % 4);

    // 4. Initialize encoder context
    aom_codec_ctx_t codec;
    aom_codec_flags_t flags = 0;
    int ver = AOM_ENCODER_ABI_VERSION;
    
    if (aom_codec_enc_init_ver(&codec, encoder_iface, &cfg, flags, ver) != AOM_CODEC_OK) {
        return 0;
    }

    // 5. Get error detail (even if no error)
    const char *error_detail = aom_codec_error_detail(&codec);
    (void)error_detail; // Use variable to avoid unused warning

    // 6. Set frame buffer functions
    uint8_t frame_buffer[4096];
    if (aom_codec_set_frame_buffer_functions(&codec,  id_000023_get_frame_buffer, 
                                              id_000023_release_frame_buffer, frame_buffer) != AOM_CODEC_OK) {
        aom_codec_destroy(&codec);
        return 0;
    }

    // 7. Create image using input data
    const unsigned int width = cfg.g_w;
    const unsigned int height = cfg.g_h;
    const aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    const unsigned int stride_align = 1;
    
    // Calculate required image buffer size for I420
    size_t y_plane_size = width * height;
    size_t uv_plane_size = (width / 2) * (height / 2);
    size_t total_image_size = y_plane_size + 2 * uv_plane_size;
    
    if (total_image_size > size) {
        aom_codec_destroy(&codec);
        return 0;
    }

    // Use input data as image buffer
    unsigned char *img_data = const_cast<unsigned char*>(data);
    aom_image_t img;
    aom_image_t *img_ptr = aom_img_wrap(&img, fmt, width, height, stride_align, img_data);
    if (!img_ptr) {
        aom_codec_destroy(&codec);
        return 0;
    }

    // 8. Set encoder configuration (again to ensure it's applied)
    if (aom_codec_enc_config_set(&codec, &cfg) != AOM_CODEC_OK) {
        aom_img_free(&img);
        aom_codec_destroy(&codec);
        return 0;
    }

    // 9. Set cx data buffer
    aom_fixed_buf_t cx_buf;
    cx_buf.buf = malloc(4096);
    cx_buf.sz = 4096;
    
    if (!cx_buf.buf) {
        aom_img_free(&img);
        aom_codec_destroy(&codec);
        return 0;
    }
    
    unsigned int pad_before = data[4] % 16;
    unsigned int pad_after = data[5] % 16;
    
    if (aom_codec_set_cx_data_buf(&codec, &cx_buf, pad_before, pad_after) != AOM_CODEC_OK) {
        free(cx_buf.buf);
        aom_img_free(&img);
        aom_codec_destroy(&codec);
        return 0;
    }

    // 10. Encode the image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t enc_flags = 0;
    
    if (data[6] & 0x01) enc_flags |= AOM_EFLAG_FORCE_KF;
    
    if (aom_codec_encode(&codec, &img, pts, duration, enc_flags) != AOM_CODEC_OK) {
        free(cx_buf.buf);
        aom_img_free(&img);
        aom_codec_destroy(&codec);
        return 0;
    }

    // 11. Add metadata to image
    uint32_t metadata_type = 1; // Arbitrary type
    size_t metadata_size = size > 200 ? 100 : size / 2;
    const uint8_t *metadata_data = data + 100;
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
    
    aom_img_add_metadata(&img, metadata_type, metadata_data, metadata_size, insert_flag);

    // 12. Get encoded data
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt;
    
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
        // Process packets if needed
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Could write to file or process here
            break;
        }
    }

    // 13. Free image
    aom_img_free(&img);

    // 14. Destroy codec context
    aom_codec_destroy(&codec);
    
    // Free allocated buffer
    free(cx_buf.buf);

    return 0;
}