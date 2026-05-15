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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 51, nr_unique_branch: 2
//<Quality> {"density":17,"unique_branches":{"read_uncompressed_header":[[4893,15,4899,21,0,0,4,0]],"realloc_frame_buffer_aligned":[[111,31,111,61,0,0,4,0]]},"library_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_error","aom_codec_set_frame_buffer_functions","aom_codec_error","aom_codec_set_option","aom_codec_set_option","aom_codec_control","aom_codec_control","aom_codec_destroy","aom_codec_decode","aom_codec_error","aom_codec_error_detail","aom_codec_get_preview_frame","aom_codec_error","aom_codec_destroy"],"critical_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_error","aom_codec_set_option","aom_codec_set_option","aom_codec_control","aom_codec_control","aom_codec_decode","aom_codec_error","aom_codec_error_detail","aom_codec_get_preview_frame","aom_codec_error","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to decode AV1 video data from the input stream:

*/


// Include necessary headers for libaom

// Custom frame buffer callbacks
static int  id_000006_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)min_size; // Unused parameter
    // Allocate frame buffer with minimal size
    fb->data = (uint8_t *)malloc(4096); // Simple allocation for fuzzing
    if (!fb->data) return -1;
    fb->size = 4096;
    fb->priv = nullptr;
    return 0;
}

static int  id_000006_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    if (fb->data) {
        free(fb->data);
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<47) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Get version extra string
    const char *version_extra = aom_codec_version_extra_str();
    (void)version_extra; // Use to avoid unused variable warning

    // 2. Initialize decoder context for AV1
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    const char *iface_name = aom_codec_iface_name(decoder_iface);
    (void)iface_name; // Use to avoid unused variable warning

    aom_codec_ctx_t codec_ctx;
    aom_codec_dec_cfg_t cfg = aom_codec_dec_cfg_t();
    cfg.threads = 1; // Single thread for safety

    // Initialize decoder
    aom_codec_err_t res = aom_codec_dec_init(&codec_ctx, decoder_iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        const char *error = aom_codec_error(&codec_ctx);
        (void)error; // Use to avoid unused variable warning
        return 0;
    }

    // 3. Set frame buffer functions
    res = aom_codec_set_frame_buffer_functions(&codec_ctx,  id_000006_get_frame_buffer,  id_000006_release_frame_buffer, nullptr);
    if (res != AOM_CODEC_OK) {
        const char *error = aom_codec_error(&codec_ctx);
        (void)error; // Use to avoid unused variable warning
    }

    // 4. Set decoder options
    // Set various options that might affect decoding
    aom_codec_set_option(&codec_ctx, fuzz_str_1, fuzz_str_2);
    aom_codec_set_option(&codec_ctx, fuzz_str_3, fuzz_str_4);

    // 5. Use codec control to set decoder parameters
    // Control ID examples from aom/aomdx.h
    // AOMD_SET_DECODE_TILE_ROW = 1, AOMD_SET_DECODE_TILE_COL = 2
    aom_codec_control(&codec_ctx, fuzz_int32_t_5, 0); // Set tile row
    aom_codec_control(&codec_ctx, fuzz_int32_t_6, 0); // Set tile column

    // 6. Decode the input data
    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        aom_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);
	return 0;
    }

    // Read data in chunks
    uint8_t buffer[4096];
    size_t bytes_read;
    bool decode_success = false;
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        res = aom_codec_decode(&codec_ctx, buffer, bytes_read, nullptr);
        
        if (res != AOM_CODEC_OK) {
            const char *error = aom_codec_error(&codec_ctx);
            const char *error_detail = aom_codec_error_detail(&codec_ctx);
            (void)error;
            (void)error_detail;
            // Continue trying to decode even if some frames fail
        } else {
            decode_success = true;
        }
    }

    assert_file_closed(&in_file);;

    // 7. Get preview frame if available
    if (decode_success) {
        const aom_image_t *preview_frame = aom_codec_get_preview_frame(&codec_ctx);
        (void)preview_frame; // Use to avoid unused variable warning
    }

    // 8. Output file for testing (not used in fuzzing but following instructions)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some metadata about the decode operation
        const char *error_str = aom_codec_error(&codec_ctx);
        if (error_str) {
            fwrite(error_str, 1, strlen(error_str), out_file);
        }
        assert_file_closed(&out_file);;
    }

    // 9. Cleanup
    aom_codec_destroy(&codec_ctx);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}