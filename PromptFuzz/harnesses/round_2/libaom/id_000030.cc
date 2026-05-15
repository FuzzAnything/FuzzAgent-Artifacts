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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_decode","aom_codec_error","aom_codec_destroy","aom_codec_get_frame","aom_img_remove_metadata","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_codec_destroy","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_frame","aom_img_remove_metadata","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


// Include libaom headers
extern "C" {
}

// Custom frame buffer callbacks
static int  id_000030_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t *)calloc(1, min_size);
    fb->size = min_size;
    return fb->data ? 0 : -1;
}

static int  id_000030_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // 1. Initialize decoder for AV1
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init(&decoder, aom_codec_av1_dx(), &dec_cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // 2. Set frame buffer functions for the decoder
    void *cb_priv = nullptr;
    if (aom_codec_set_frame_buffer_functions(&decoder,  id_000030_get_frame_buffer,  id_000030_release_frame_buffer, cb_priv) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // 3. Decode the input data
    size_t decode_size = size > 10000 ? 10000 : size; // Limit decode size
    if (aom_codec_decode(&decoder, data, decode_size, nullptr) != AOM_CODEC_OK) {
        // Get error message even on failure
        const char *error_str = aom_codec_error(&decoder);
        (void)error_str; // Use variable to prevent unused warning
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // 4. Get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = aom_codec_get_frame(&decoder, &iter);
    
    if (img) {
        // 5. Remove metadata from the image
        aom_img_remove_metadata(img);
        
        // 6. Initialize encoder
        aom_codec_ctx_t encoder;
        aom_codec_enc_cfg_t enc_cfg;
        
        if (aom_codec_enc_config_default(aom_codec_av1_cx(), &enc_cfg, 0) != AOM_CODEC_OK) {
            aom_codec_destroy(&decoder);
            return 0;
        }
        
        // Configure encoder with fuzzed parameters
        enc_cfg.g_w = img->d_w > 0 ? img->d_w : 64;
        enc_cfg.g_h = img->d_h > 0 ? img->d_h : 64;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.g_threads = 1;
        enc_cfg.rc_target_bitrate = 1000;
        
        if (aom_codec_enc_init(&encoder, aom_codec_av1_cx(), &enc_cfg, 0) != AOM_CODEC_OK) {
            aom_codec_destroy(&decoder);
            return 0;
        }
        
        // 7. Prepare output buffer
        aom_fixed_buf_t output_buf;
        output_buf.sz = size > 100000 ? 100000 : size;
        output_buf.buf = malloc(output_buf.sz);
        
        if (output_buf.buf) {
            // 8. Set output buffer for encoded data
            unsigned int pad_before = size % 16;
            unsigned int pad_after = (size + 1) % 16;
            if (aom_codec_set_cx_data_buf(&encoder, &output_buf, pad_before, pad_after) != AOM_CODEC_OK) {
                free(output_buf.buf);
                aom_codec_destroy(&encoder);
                aom_codec_destroy(&decoder);
                return 0;
            }
            
            // 9. Encode the frame with fuzzed parameters
            int pts = 0;
            unsigned long duration = 1;
            aom_enc_frame_flags_t flags = (data[0] & 1) ? AOM_EFLAG_FORCE_KF : 0;
            
            if (aom_codec_encode(&encoder, img, pts, duration, flags) != AOM_CODEC_OK) {
                // Get encoder error
                const char *enc_error = aom_codec_error(&encoder);
                (void)enc_error; // Use variable to prevent unused warning
            }
            
            // 10. Try to get encoded data
            aom_codec_iter_t enc_iter = nullptr;
            const aom_codec_cx_pkt_t *pkt;
            while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                // Process packet if needed
                (void)pkt;
            }
            
            free(output_buf.buf);
        }
        
        // 11. Destroy encoder
        aom_codec_destroy(&encoder);
    }
    
    // 12. Destroy decoder
    aom_codec_destroy(&decoder);
    
    // 13. Use FILE* operations as requested
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read some data from the file
        uint8_t buffer[100];
        size_t read_bytes = fread(buffer, 1, sizeof(buffer), in_file);
        (void)read_bytes; // Use variable to prevent unused warning
        
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use variable to prevent unused warning
        
        assert_file_closed(&in_file);;
    }
    
    // 14. Write to output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data
        fwrite(data, 1, size > 100 ? 100 : size, out_file);
        
        // Get file descriptor
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Use variable to prevent unused warning
        
        assert_file_closed(&out_file);;
        
        // Clean up output file
        remove("output_file");
    }
    
    // 15. Use file names directly
    const char *input_filename = "input_file";
    const char *output_filename = "output_file";
    (void)input_filename;
    (void)output_filename;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}