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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_error","aom_codec_set_option","aom_codec_set_option","aom_codec_control","aom_codec_decode","aom_codec_get_preview_frame","aom_codec_error","aom_codec_destroy"],"critical_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_error","aom_codec_set_option","aom_codec_set_option","aom_codec_control","aom_codec_decode","aom_codec_get_preview_frame","aom_codec_error","aom_codec_destroy"],"visited":0}
/**/


// libaom headers
extern "C" {
}

// Custom callback functions for frame buffer management
static int  id_000005_get_frame_buffer(void *cb_priv, size_t min_size, 
                           aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) {
        return -1;
    }
    fb->size = min_size;
    return 0;
}

static int  id_000005_release_frame_buffer(void *cb_priv, 
                               aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    free(fb->data);
    fb->data = NULL;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Consume the input data using fmemopen as specified
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file as specified
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Get extra version string (for informational purposes)
    const char *version_extra = aom_codec_version_extra_str();
    (void)version_extra; // Prevent unused variable warning

    // 2. Initialize decoder context
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    const char *iface_name = aom_codec_iface_name(iface);
    (void)iface_name; // Prevent unused variable warning

    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1; // Single thread for fuzzing

    aom_codec_err_t res = aom_codec_dec_init(&codec_ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Set frame buffer functions
    res = aom_codec_set_frame_buffer_functions(&codec_ctx, 
                                                id_000005_get_frame_buffer, 
                                                id_000005_release_frame_buffer, 
                                               NULL);
    if (res != AOM_CODEC_OK) {
        const char *error_str = aom_codec_error(&codec_ctx);
        (void)error_str; // Prevent unused variable warning
    }

    // 4. Set decoder options based on input data
    // Use first few bytes to determine option values
    if (size > 0) {
        char option_name[32];
        char option_value[32];
        
        // Set thread count option
        snprintf(option_name, sizeof(option_name), "threads");
        snprintf(option_value, sizeof(option_value), "%d", 
                 (data[0] % 4) + 1); // 1-4 threads
        aom_codec_set_option(&codec_ctx, option_name, option_value);
        
        // Set error resilience mode
        if (size > 1) {
            snprintf(option_name, sizeof(option_name), "error-resilient");
            snprintf(option_value, sizeof(option_value), "%d", 
                     data[1] & 1); // 0 or 1
            aom_codec_set_option(&codec_ctx, option_name, option_value);
        }
    }

    // 5. Set control parameters
    // Try different control IDs based on input data
    if (size > 2) {
        // Use data[2] to select control ID
        int ctrl_id = (data[2] % 10) + 1; // Range 1-10
        aom_codec_control(&codec_ctx, ctrl_id);
    }

    // 6. Decode the input data
    // Read data from the file to simulate streaming
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (file_size > 0) {
        uint8_t *buffer = (uint8_t *)malloc(file_size);
        if (buffer) {
            fread(buffer, 1, file_size, in_file);
            
            // Decode the data
            res = aom_codec_decode(&codec_ctx, buffer, file_size, NULL);
            
            free(buffer);
        }
    }

    // 7. Get preview frame if available
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&codec_ctx);
    if (preview_frame) {
        // Write some metadata to output file
        fprintf(out_file, "Preview frame: %dx%d\n", 
                preview_frame->d_w, preview_frame->d_h);
    }

    // 8. Check for errors
    const char *error_msg = aom_codec_error(&codec_ctx);
    if (error_msg && strlen(error_msg) > 0) {
        fprintf(out_file, "Decoder error: %s\n", error_msg);
    }

    // 9. Clean up
    aom_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}