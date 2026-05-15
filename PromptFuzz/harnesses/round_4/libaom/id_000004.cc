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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 9
//<Quality> {"density":12,"unique_branches":{"ctrl_get_last_ref_updates":[[1046,7,1046,18,0,0,4,0],[1047,9,1047,26,0,0,4,1]],"ctrl_get_frame_corrupted":[[1257,7,1257,16,0,0,4,0],[1258,9,1258,26,0,0,4,0],[1263,11,1263,33,0,0,4,0],[1263,11,1263,33,0,0,4,1],[1263,37,1263,64,0,0,4,0],[1263,37,1263,64,0,0,4,1],[1265,11,1265,39,0,0,4,1]]},"library_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_set_option","aom_codec_set_option","aom_codec_control","aom_codec_decode","aom_codec_error","aom_codec_get_preview_frame","aom_codec_control","aom_codec_destroy"],"critical_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_set_option","aom_codec_set_option","aom_codec_control","aom_codec_decode","aom_codec_error","aom_codec_get_preview_frame","aom_codec_control","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Dummy frame buffer callbacks
static int  id_000004_dummy_get_frame_buffer(void *cb_priv, size_t min_size, 
                                  aom_codec_frame_buffer_t *fb) {
    // Allocate minimum required buffer
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

static int  id_000004_dummy_release_frame_buffer(void *cb_priv, 
                                      aom_codec_frame_buffer_t *fb) {
    if (fb->data) {
        free(fb->data);
        fb->data = nullptr;
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<42) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // Call version extra string API
    const char *extra_version = aom_codec_version_extra_str();
    (void)extra_version; // Suppress unused warning
    
    // Create a decoder context
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    // Get interface name
    const char *iface_name = aom_codec_iface_name(iface);
    (void)iface_name; // Suppress unused warning
    
    // Initialize decoder with default configuration
    aom_codec_dec_cfg_t cfg = aom_codec_dec_cfg_t();
    cfg.threads = 1;
    
    aom_codec_err_t err = aom_codec_dec_init(&codec_ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        // Clean up and return if initialization fails
        return 0;
    }
    
    // Set frame buffer functions
    void *cb_priv = nullptr;
    err = aom_codec_set_frame_buffer_functions(&codec_ctx, 
                                                id_000004_dummy_get_frame_buffer,
                                                id_000004_dummy_release_frame_buffer,
                                               cb_priv);
    
    // Set some decoder options
    aom_codec_set_option(&codec_ctx, fuzz_str_1, fuzz_str_2);
    aom_codec_set_option(&codec_ctx, fuzz_str_3, fuzz_str_4);
    
    // Try to set some control parameters
    // AOMD_GET_LAST_REF_UPDATES = 1 (example control ID)
    int ctrl_value = 0;
    aom_codec_control(&codec_ctx, AOMD_GET_LAST_REF_UPDATES, &ctrl_value);
    
    // Decode the input data
    if (size > 0) {
        err = aom_codec_decode(&codec_ctx, data, size, nullptr);
        
        // Get error message if decoding failed
        if (err != AOM_CODEC_OK) {
            const char *error_msg = aom_codec_error(&codec_ctx);
            (void)error_msg; // Suppress unused warning
        }
        
        // Try to get preview frame
        const aom_image_t *preview_frame = aom_codec_get_preview_frame(&codec_ctx);
        (void)preview_frame; // Suppress unused warning
    }
    
    // Try another control - AOMD_GET_FRAME_CORRUPTED = 3
    int corrupted = 0;
    aom_codec_control(&codec_ctx, AOMD_GET_FRAME_CORRUPTED, &corrupted);
    
    // Clean up
    aom_codec_destroy(&codec_ctx);
    
    return 0;
}