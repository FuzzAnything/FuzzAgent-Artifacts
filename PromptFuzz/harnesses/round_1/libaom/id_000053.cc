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
//<ID> 115
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"aom_codec_control":[[93,7,93,15,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_control","aom_codec_decode","aom_codec_get_preview_frame","aom_img_alloc_with_border","aom_img_metadata_alloc","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_error_detail","aom_codec_destroy","aom_img_metadata_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_control","aom_codec_decode","aom_codec_get_preview_frame","aom_img_alloc_with_border","aom_img_metadata_alloc","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_error_detail","aom_codec_destroy","aom_img_metadata_free","aom_img_free","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to achieve the event of decoding and re-encoding a video stream:

*/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1024) {
        return 0;  // Need minimum data
    }

    // Open input as file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    (void)fd_in; // We have it if needed

    // Initialize variables
    aom_codec_ctx_t decoder = {0};
    aom_codec_ctx_t encoder = {0};
    aom_image_t *image = nullptr;
    aom_metadata_t *metadata = nullptr;
    aom_codec_enc_cfg_t cfg = {0};
    aom_fixed_buf_t buf = {0};
    const aom_image_t *preview_frame = nullptr;
    aom_codec_iter_t iter = nullptr;
    
    // Allocate buffer for encoder output
    buf.buf = malloc(size * 2); // Double input size for safety
    buf.sz = size * 2;
    if (!buf.buf) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 1. Initialize decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail; // We have it if needed
        free(buf.buf);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 2. Use aom_codec_control - try to set decoder control
    // Use a control ID that's likely available (AOMD_SET_CPUUSED = 0)
    int control_id = 0;
    aom_codec_err_t control_result = aom_codec_control(&decoder, control_id);
    (void)control_result; // Result may be error but that's fine for fuzzing

    // 3. Decode the input data
    if (aom_codec_decode(&decoder, data, size, nullptr) == AOM_CODEC_OK) {
        // 4. Get preview frame using aom_codec_get_preview_frame
        preview_frame = aom_codec_get_preview_frame(&decoder);
        
        if (preview_frame) {
            // 5. Allocate image with border using preview frame dimensions
            image = aom_img_alloc_with_border(nullptr, 
                AOM_IMG_FMT_I420, 
                preview_frame->d_w, 
                preview_frame->d_h, 
                32,  // align
                0,   // size_align
                16); // border
            
            if (image) {
                // 6. Allocate metadata using input data
                size_t metadata_size = size > 1024 ? 1024 : size;
                uint32_t metadata_type = 1; // Arbitrary type
                aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
                
                metadata = aom_img_metadata_alloc(
                    metadata_type,
                    data,
                    metadata_size,
                    insert_flag
                );
                
                // 7. Initialize encoder
                aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
                if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) == AOM_CODEC_OK) {
                    // Configure encoder with preview frame dimensions
                    cfg.g_w = preview_frame->d_w;
                    cfg.g_h = preview_frame->d_h;
                    cfg.g_timebase.num = 1;
                    cfg.g_timebase.den = 30;
                    cfg.rc_target_bitrate = 1000;
                    cfg.g_pass = AOM_RC_ONE_PASS;
                    cfg.g_lag_in_frames = 0;
                    
                    // 8. Use aom_codec_enc_config_set
                    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
                        aom_codec_enc_config_set(&encoder, &cfg);
                        
                        // 9. Use aom_codec_set_cx_data_buf
                        aom_codec_set_cx_data_buf(&encoder, &buf, 0, 0);
                        
                        // 10. Try to encode the image (even if empty)
                        aom_codec_encode(&encoder, image, 0, 1, 0);
                        
                        // 11. Get encoder error detail
                        const char *encoder_error_detail = aom_codec_error_detail(&encoder);
                        (void)encoder_error_detail; // We have it if needed
                        
                        // Destroy encoder
                        aom_codec_destroy(&encoder);
                    }
                }
                
                // Free metadata if allocated
                if (metadata) {
                    aom_img_metadata_free(metadata);
                }
                
                // Free image
                aom_img_free(image);
            }
        }
    }
    
    // Destroy decoder
    aom_codec_destroy(&decoder);
    
    // Clean up
    free(buf.buf);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}