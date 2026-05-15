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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 31.666666, nr_unique_branch: 7
//<Quality> {"density":19,"unique_branches":{"set_bitstream_level_tier":[[490,14,491,33,0,0,4,1],[493,14,494,33,0,0,4,1],[496,14,497,33,0,0,4,1],[499,14,500,33,0,0,4,0]],"aom_img_free":[[315,7,315,10,0,0,4,1]],"set_tile_info":[[388,12,388,61,0,0,4,0]],"img_alloc_helper":[[75,7,75,34,0,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_enc_config_set","aom_img_alloc","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_img_flip","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_encode","aom_codec_get_global_headers","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_uleb_encode","aom_uleb_decode","aom_codec_peek_stream_info","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_codec_destroy","aom_codec_error_detail","aom_img_remove_metadata","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_enc_config_set","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_flip","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_encode","aom_codec_get_global_headers","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_uleb_encode","aom_uleb_decode","aom_codec_peek_stream_info","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_codec_destroy","aom_codec_error_detail","aom_img_remove_metadata","aom_img_free","aom_codec_destroy"],"visited":2}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to achieve an event of encoding raw image data to AV1 format and then decoding it back, with metadata handling and ULEB encoding/decoding:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful test

    // Split input data for different purposes
    size_t offset = 0;
    
    // 1. Extract image dimensions (first 8 bytes)
    if (offset + 8 > size) {
	return 0;
	}
	
    unsigned int width = (data[0] << 8) | data[1];
    unsigned int height = (data[2] << 8) | data[3];
    unsigned int align = data[4] & 0x1F;  // 5 bits for alignment
    align = align ? align : 1;  // Ensure at least 1
    width = width ? width : 16;  // Ensure non-zero
    height = height ? height : 16;
    offset += 8;

    // Limit dimensions to avoid excessive memory usage
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    // 2. Create and initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_threads = 1;
    cfg.rc_end_usage = AOM_Q;
    cfg.g_error_resilient = 0;
    
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set some encoder options
    aom_codec_set_option(&encoder, "cpu-used", "1");
    aom_codec_set_option(&encoder, "end-usage", "q");
    
    // Apply configuration
    aom_codec_enc_config_set(&encoder, &cfg);
    
    // 3. Create test image and fill with fuzzer data
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, align);
    if (!img) {
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Fill image planes with fuzzer data
    size_t plane_sizes[3] = {
        width * height,
        (width / 2) * (height / 2),
        (width / 2) * (height / 2)
    };
    
    for (int plane = 0; plane < 3; plane++) {
        if (offset >= size) break;
        size_t to_copy = plane_sizes[plane];
        if (to_copy > size - offset) to_copy = size - offset;
        memcpy(img->planes[plane], data + offset, to_copy);
        offset += to_copy;
        
        // Get plane dimensions
        int plane_w = aom_img_plane_width(img, plane);
        int plane_h = aom_img_plane_height(img, plane);
        (void)plane_w; (void)plane_h; // Suppress unused warnings
    }
    
    // Flip image for testing
    aom_img_flip(img);
    
    // 4. Add metadata to image
    uint32_t metadata_type = 0x12345678;
    size_t metadata_size = 32;
    uint8_t metadata_data[32];
    if (offset + metadata_size > size) {
        metadata_size = size - offset;
    }
    if (metadata_size > 0) {
        memcpy(metadata_data, data + offset, metadata_size);
        offset += metadata_size;
        
        aom_metadata_t *metadata = aom_img_metadata_alloc(
            metadata_type, metadata_data, metadata_size, AOM_MIF_ANY_FRAME);
        if (metadata) {
            // Metadata would need to be added via aom_img_add_metadata, but for now we just test alloc/free
            aom_img_metadata_free(metadata);
        }
    }
    
    // 5. Encode the image
    aom_codec_encode(&encoder, img, 0, 1, 0);
    
    // Get global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder);
    if (global_headers) {
        // Can use global_headers->buf and global_headers->sz
        free(global_headers->buf);
        free(global_headers);
    }
    
    // Try to get preview frame
    const aom_image_t *preview = aom_codec_get_preview_frame(&encoder);
    (void)preview; // Suppress unused warning
    
    // Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    std::vector<uint8_t> encoded_data;
    
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            encoded_data.insert(encoded_data.end(),
                (uint8_t*)pkt->data.frame.buf,
                (uint8_t*)pkt->data.frame.buf + pkt->data.frame.sz);
        }
    }
    
    // 6. Test ULEB encoding/decoding
    if (offset + 8 <= size) {
        uint64_t uleb_value = 0;
        for (int i = 0; i < 8 && offset + i < size; i++) {
            uleb_value |= ((uint64_t)data[offset + i] << (8 * i));
        }
        offset += 8;
        
        uint8_t uleb_buffer[10];
        size_t coded_size = 0;
        if (aom_uleb_encode(uleb_value, sizeof(uleb_buffer), uleb_buffer, &coded_size) == 0) {
            // Decode it back
            uint64_t decoded_value = 0;
            size_t decoded_length = 0;
            aom_uleb_decode(uleb_buffer, coded_size, &decoded_value, &decoded_length);
        }
    }
    
    // 7. Decode the encoded data
    if (!encoded_data.empty()) {
        // First peek stream info
        aom_codec_stream_info_t si;
        memset(&si, 0, sizeof(si));
        aom_codec_peek_stream_info(aom_codec_av1_dx(), encoded_data.data(), encoded_data.size(), &si);
        
        // Initialize decoder
        aom_codec_ctx_t decoder;
        aom_codec_dec_cfg_t dec_cfg;
        memset(&dec_cfg, 0, sizeof(dec_cfg));
        dec_cfg.threads = 1;
        dec_cfg.allow_lowbitdepth = 1;
        
        if (aom_codec_dec_init_ver(&decoder, aom_codec_av1_dx(), &dec_cfg, 0, AOM_DECODER_ABI_VERSION) == AOM_CODEC_OK) {
            // Decode the data
            aom_codec_decode(&decoder, encoded_data.data(), encoded_data.size(), NULL);
            
            // Get decoded frames
            aom_codec_iter_t dec_iter = NULL;
            const aom_image_t *decoded_img;
            while ((decoded_img = aom_codec_get_frame(&decoder, &dec_iter)) != NULL) {
                // Access some plane info
                int plane_w = aom_img_plane_width(decoded_img, 0);
                int plane_h = aom_img_plane_height(decoded_img, 0);
                (void)plane_w; (void)plane_h; // Suppress unused warnings
            }
            
            aom_codec_destroy(&decoder);
        }
    }
    
    // 8. Test error detail (should be NULL if no error)
    const char *error_detail = aom_codec_error_detail(&encoder);
    (void)error_detail; // Suppress unused warning
    
    // 9. Cleanup
    aom_img_remove_metadata(img);  // Remove any metadata
    aom_img_free(img);
    aom_codec_destroy(&encoder);
    
    return 0;
}