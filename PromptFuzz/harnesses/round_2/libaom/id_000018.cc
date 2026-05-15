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
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_error_detail","aom_img_alloc","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_set_frame_buffer_functions","aom_codec_set_cx_data_buf","aom_codec_get_cx_data","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_error_detail","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_set_frame_buffer_functions","aom_codec_set_cx_data_buf","aom_codec_get_cx_data","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_img_free","aom_codec_destroy"],"visited":0}
/*Looking at the specified APIs, I can see they cover both encoding and ULEB operations. Here's a fuzz driver that tests a scenario where libaom processes input data for encoding with custom frame buffers and ULEB encoding:

*/


extern "C" {
}

// Custom frame buffer callbacks
int  id_000018_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    uint8_t *buf = (uint8_t *)malloc(min_size);
    if (!buf) return 1;
    
    fb->data = buf;
    fb->size = min_size;
    return 0;
}

int  id_000018_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    if (fb->data) free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful fuzzing
    
    // Create input/output files using the instructions
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    // Get default configuration
    if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Configure with small dimensions for fuzzing
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_bit_depth = AOM_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.rc_target_bitrate = 256;
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Set encoder configuration
    if (aom_codec_enc_config_set(&encoder, &cfg) != AOM_CODEC_OK) {
        // 2. Get error detail if config set fails
        const char *error_detail = aom_codec_error_detail(&encoder);
        // We'll just continue even if config set fails
    }
    
    // Create test image for encoding
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 32, 32, 1);
    if (!img) {
        aom_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Fill image with test data from input
    size_t bytes_to_read = std::min(size, (size_t)(img->w * img->h * 3 / 2));
    uint8_t *img_data = (uint8_t *)malloc(bytes_to_read);
    if (img_data) {
        fread(img_data, 1, bytes_to_read, in_file);
        
        // Copy to image planes (simplified - actual YUV data needs proper arrangement)
        for (int plane = 0; plane < 3; plane++) {
            int stride = img->stride[plane];
            int plane_width = aom_img_plane_width(img, plane);
            int plane_height = aom_img_plane_height(img, plane);
            
            for (int y = 0; y < plane_height && bytes_to_read > 0; y++) {
                size_t copy_size = std::min((size_t)plane_width, bytes_to_read);
                memcpy(img->planes[plane] + y * stride, img_data, copy_size);
                bytes_to_read -= copy_size;
            }
        }
        free(img_data);
    }
    
    // Encode the image
    aom_codec_encode(&encoder, img, 0, 1, 0);
    
    // 3. Set custom frame buffer functions
    aom_codec_set_frame_buffer_functions(&encoder,  id_000018_get_frame_buffer, 
                                         id_000018_release_frame_buffer, NULL);
    
    // Create output buffer for encoded data
    std::vector<uint8_t> output_buffer(4096);
    aom_fixed_buf_t buf;
    buf.buf = output_buffer.data();
    buf.sz = output_buffer.size();
    
    // 4. Set output buffer with padding
    aom_codec_set_cx_data_buf(&encoder, &buf, 16, 16);
    
    // 5. Get encoded data packets
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Write to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            
            // 6. & 7. Test ULEB encoding with frame size
            uint64_t frame_size = pkt->data.frame.sz;
            uint8_t uleb_buffer[10];
            size_t coded_size = 0;
            
            // Regular ULEB encoding
            if (aom_uleb_encode(frame_size, sizeof(uleb_buffer), 
                               uleb_buffer, &coded_size) == 0) {
                // Write ULEB encoded size
                fwrite(uleb_buffer, 1, coded_size, out_file);
            }
            
            // Fixed size ULEB encoding
            uint8_t fixed_uleb_buffer[10];
            size_t fixed_coded_size = 0;
            aom_uleb_encode_fixed_size(frame_size, sizeof(fixed_uleb_buffer),
                                       5, fixed_uleb_buffer, &fixed_coded_size);
        }
    }
    
    // Test ULEB encoding with random values from input
    if (size > 10) {
        uint64_t test_value = 0;
        for (size_t i = 0; i < 8 && i < size; i++) {
            test_value = (test_value << 8) | data[i];
        }
        
        uint8_t uleb_test_buffer[20];
        size_t test_coded_size = 0;
        aom_uleb_encode(test_value, sizeof(uleb_test_buffer), 
                       uleb_test_buffer, &test_coded_size);
        
        // Also test fixed size encoding
        uint8_t fixed_test_buffer[10];
        size_t fixed_test_coded_size = 0;
        aom_uleb_encode_fixed_size(test_value, sizeof(fixed_test_buffer),
                                   3, fixed_test_buffer, &fixed_test_coded_size);
    }
    
    // Clean up
    aom_img_free(img);
    aom_codec_destroy(&encoder);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}