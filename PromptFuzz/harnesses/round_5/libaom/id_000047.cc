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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.5, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_destroy","aom_img_remove_metadata","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_destroy","aom_img_remove_metadata","aom_img_free","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	 // Need minimal data for metadata
    
    // 1. Initialize encoder
    aom_codec_iface_t *encoder = aom_codec_av1_cx();
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    
    // 2. Get default encoder config
    if (aom_codec_enc_config_default(encoder, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set small dimensions for fuzzing
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = AOM_Q;
    
    // 3. Initialize encoder
    if (aom_codec_enc_init_ver(&codec, encoder, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // 4. Set an encoder option
    aom_codec_set_option(&codec, "cpu-used", "8");
    
    // 5. Allocate image
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        aom_codec_destroy(&codec);
        return 0;
    }
    
    // Fill image with test pattern
    for (int plane = 0; plane < 3; ++plane) {
        int width = aom_img_plane_width(img, plane);
        int height = aom_img_plane_height(img, plane);
        int stride = img->stride[plane];
        uint8_t *plane_data = img->planes[plane];
        
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                plane_data[y * stride + x] = (uint8_t)((x + y) % 256);
            }
        }
    }
    
    // 6. Add metadata using input data
    // Use first 4 bytes as metadata type, rest as metadata payload
    uint32_t metadata_type = 0;
    if (size >= 4) {
        memcpy(&metadata_type, data, 4);
    }
    
    size_t metadata_size = (size > 4) ? size - 4 : 0;
    const uint8_t *metadata_data = (size > 4) ? data + 4 : data;
    
    if (metadata_size > 0) {
        aom_img_add_metadata(img, metadata_type, metadata_data, 
                           metadata_size > 1024 ? 1024 : metadata_size, // Limit size
                           AOM_MIF_ANY_FRAME);
    }
    
    // 7. Set up output buffer
    aom_fixed_buf_t output_buf;
    std::vector<uint8_t> buffer(4096); // Initial buffer
    output_buf.buf = buffer.data();
    output_buf.sz = buffer.size();
    
    aom_codec_set_cx_data_buf(&codec, &output_buf, 0, 0);
    
    // 8. Encode image
    aom_codec_encode(&codec, img, 0, 1, 0);
    
    // 9. Get stream info (for encoded data)
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    
    // Try to get stream info from the encoded data if any
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    std::vector<uint8_t> encoded_data;
    
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            encoded_data.assign((uint8_t*)pkt->data.frame.buf,
                              (uint8_t*)pkt->data.frame.buf + pkt->data.frame.sz);
            break;
        }
    }
    
    if (!encoded_data.empty()) {
        // Create decoder context for stream info
        aom_codec_ctx_t decoder;
        aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
        
        if (aom_codec_dec_init_ver(&decoder, decoder_iface, NULL, 0, 
                                  AOM_DECODER_ABI_VERSION) == AOM_CODEC_OK) {
            aom_codec_get_stream_info(&decoder, &si);
            aom_codec_destroy(&decoder);
        }
    }
    
    // 10. Remove metadata (cleanup)
    aom_img_remove_metadata(img);
    
    // 11. Free resources
    aom_img_free(img);
    aom_codec_destroy(&codec);
    
    return 0;
}