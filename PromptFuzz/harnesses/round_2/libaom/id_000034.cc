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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 84, nr_unique_branch: 5
//<Quality> {"density":14,"unique_branches":{"aom_codec_get_preview_frame":[[268,9,268,20,0,0,4,0]],"aom_codec_set_frame_buffer_functions":[[127,14,127,25,0,0,4,0]],"aom_codec_enc_init_ver":[[67,12,67,50,0,0,4,0]],"aom_codec_enc_config_default":[[107,12,107,50,0,0,4,0]],"aom_codec_get_global_headers":[[285,9,285,20,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_get_global_headers","aom_img_wrap","aom_img_add_metadata","aom_codec_encode","aom_codec_get_preview_frame","aom_img_plane_width","aom_img_plane_height","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_get_global_headers","aom_img_wrap","aom_img_add_metadata","aom_codec_encode","aom_codec_get_preview_frame","aom_img_plane_width","aom_img_plane_height","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000034_get_frame_buffer(void *cb_priv, size_t min_size, 
                           aom_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t*)calloc(1, min_size);
    fb->size = min_size;
    return fb->data ? 0 : -1;
}

static int  id_000034_release_frame_buffer(void *cb_priv, 
                               aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data

    // 1. Peek stream info from input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_iface_t *codec_iface = aom_codec_av1_dx();
    aom_codec_peek_stream_info(codec_iface, data, size, &stream_info);

    // 2. Initialize encoder context
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = 0; // Good quality
    
    aom_codec_enc_config_default(codec_iface, &cfg, usage);
    cfg.g_w = stream_info.w ? stream_info.w : 128;
    cfg.g_h = stream_info.h ? stream_info.h : 128;
    cfg.g_bit_depth = AOM_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.rc_end_usage = AOM_Q;
    
    aom_codec_enc_init_ver(&encoder, codec_iface, &cfg, 0, 
                          AOM_ENCODER_ABI_VERSION);

    // 3. Set custom frame buffer functions
    aom_codec_set_frame_buffer_functions(&encoder,  id_000034_get_frame_buffer, 
                                         id_000034_release_frame_buffer, nullptr);

    // 4. Get global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder);
    if (global_headers) {
        // Could write to file if needed
    }

    // 5. Create image using data
    aom_image_t img;
    const size_t img_data_size = cfg.g_w * cfg.g_h * 3 / 2; // I420 format
    uint8_t *img_data = nullptr;
    
    if (size >= img_data_size) {
        img_data = (uint8_t*)data; // Use input data directly
    } else {
        img_data = (uint8_t*)calloc(1, img_data_size);
        if (img_data && size > 0) {
            memcpy(img_data, data, size < img_data_size ? size : img_data_size);
        }
    }
    
    if (img_data) {
        aom_img_wrap(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 
                    16, img_data);
        
        // 6. Add metadata to image
        size_t metadata_size = size > 100 ? 50 : size/2;
        if (metadata_size > 0) {
            uint32_t metadata_type = 1; // Custom metadata type
            const uint8_t *metadata_data = data + (size > 100 ? 50 : 0);
            aom_img_add_metadata(&img, metadata_type, metadata_data, 
                               metadata_size, AOM_MIF_ANY_FRAME);
        }
        
        // 7. Encode the image (required to get preview frame)
        aom_codec_encode(&encoder, &img, 0, 1, 0);
        
        // 8. Get preview frame
        const aom_image_t *preview = aom_codec_get_preview_frame(&encoder);
        
        // Write preview to file if available
        if (preview) {
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                for (int plane = 0; plane < 3; ++plane) {
                    const uint8_t *buf = preview->planes[plane];
                    int stride = preview->stride[plane];
                    int w = aom_img_plane_width(preview, plane);
                    int h = aom_img_plane_height(preview, plane);
                    
                    for (int y = 0; y < h; ++y) {
                        fwrite(buf + y * stride, 1, w, out_file);
                    }
                }
                assert_file_closed(&out_file);;
            }
        }
        
        // Cleanup image
        aom_img_free(&img);
        if (img_data != data) {
            free(img_data);
        }
    }

    // Cleanup encoder
    aom_codec_destroy(&encoder);

    return 0;
}