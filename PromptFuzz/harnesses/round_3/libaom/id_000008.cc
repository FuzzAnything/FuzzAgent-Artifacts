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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"realloc_frame_buffer_aligned":[[111,11,111,27,0,0,4,0]]},"library_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_set_frame_buffer_functions","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_destroy","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_add_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_uleb_encode","aom_uleb_decode"],"critical_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_set_frame_buffer_functions","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_destroy","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_add_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_uleb_encode","aom_uleb_decode"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000008_get_frame_buffer(void *cb_priv, size_t min_size, 
                           aom_codec_frame_buffer_t *fb) {
    (void)min_size;
    (void)fb;
    // Just return 0 to indicate failure - we're fuzzing, not actually allocating buffers
    return 0;
}

static int  id_000008_release_frame_buffer(void *cb_priv, 
                               aom_codec_frame_buffer_t *fb) {
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing
    
    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor
    int fd = fuzz_fileno(in_file);
    (void)fd;  // Use fd if needed
    
    // 1. Get build configuration string
    const char *build_config = aom_codec_build_config();
    (void)build_config;
    
    // 2. Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    // 3. Get interface name
    const char *iface_name = aom_codec_iface_name(iface);
    (void)iface_name;
    
    // 4. Initialize decoder with default configuration
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 128;
    dec_cfg.h = 128;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t init_result = aom_codec_dec_init_ver(&decoder, iface, &dec_cfg, 
                                                         0, AOM_DECODER_ABI_VERSION);
    
    // 5. Get error string for init result
    const char *err_str = aom_codec_err_to_string(init_result);
    (void)err_str;
    
    if (init_result == AOM_CODEC_OK) {
        // 6. Set frame buffer functions
        aom_codec_set_frame_buffer_functions(&decoder,  id_000008_get_frame_buffer, 
                                             id_000008_release_frame_buffer, nullptr);
        
        // 7. Create a fixed buffer from input data for cx_data_buf
        aom_fixed_buf_t fixed_buf;
        fixed_buf.buf = (void *)data;
        fixed_buf.sz = size;
        
        // 8. Set cx data buffer (even though this is typically for encoding)
        aom_codec_set_cx_data_buf(&decoder, &fixed_buf, 0, 0);
        
        // 9. Decode the input data
        aom_codec_decode(&decoder, data, size, nullptr);
        
        // 10. Get error from decoder context
        const char *decoder_error = aom_codec_error(&decoder);
        (void)decoder_error;
        
        // 11. Try to get frames and metadata
        aom_codec_iter_t iter = nullptr;
        aom_image_t *img;
        while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
            // 12. Get metadata from image if available
            size_t metadata_count = aom_img_num_metadata(img);
            for (size_t i = 0; i < metadata_count; i++) {
                const aom_metadata_t *metadata = aom_img_get_metadata(img, i);
                (void)metadata;  // Use metadata if needed
            }
        }
        
        // 13. Clean up decoder
        aom_codec_destroy(&decoder);
    }
    
    // 14. Also test encoding path briefly
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    if (aom_codec_enc_config_default(enc_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
        enc_cfg.g_w = 64;
        enc_cfg.g_h = 64;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        
        if (aom_codec_enc_init_ver(&encoder, enc_iface, &enc_cfg, 0, 
                                   AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
            
            // Create a simple test image
            aom_image_t test_img;
            aom_img_alloc(&test_img, AOM_IMG_FMT_I420, 64, 64, 1);
            
            if (test_img.planes[0]) {
                // Add some metadata to the image
                uint8_t meta_data[] = {1, 2, 3, 4, 5};
                aom_img_add_metadata(&test_img, 1, meta_data, 5, 
                                    AOM_MIF_ANY_FRAME);
                
                // Encode the image
                aom_codec_encode(&encoder, &test_img, 0, 1, 0);
                
                // Get compressed data
                aom_codec_iter_t enc_iter = nullptr;
                const aom_codec_cx_pkt_t *pkt;
                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    (void)pkt;  // Use packet if needed
                }
            }
            
            aom_img_free(&test_img);
            aom_codec_destroy(&encoder);
        }
    }
    
    // 15. Test ULEB encoding/decoding
    uint64_t test_value = 12345;
    uint8_t coded_buffer[10];
    size_t coded_size = 0;
    
    if (size > 0) {
        // Use first byte as test value
        test_value = data[0];
    }
    
    aom_uleb_encode(test_value, sizeof(coded_buffer), coded_buffer, &coded_size);
    
    if (coded_size > 0 && coded_size < sizeof(coded_buffer)) {
        uint64_t decoded_value = 0;
        size_t decode_length = 0;
        aom_uleb_decode(coded_buffer, coded_size, &decoded_value, &decode_length);
    }
    
    // 16. Clean up
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}