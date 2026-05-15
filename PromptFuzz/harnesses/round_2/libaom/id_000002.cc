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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["aom_codec_version","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_uleb_size_in_bytes","aom_obu_type_to_string","aom_codec_get_global_headers","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy"],"critical_calls":["aom_codec_version","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_uleb_size_in_bytes","aom_obu_type_to_string","aom_codec_get_global_headers","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy"],"visited":0}
/**/


// Include necessary headers (based on typical libaom headers)
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Use aom_codec_version()
    int version = aom_codec_version();
    (void)version; // Prevent unused warning

    // 2. Use aom_codec_av1_dx() to get decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        return 0;
    }

    // 3. Use aom_codec_get_caps() with decoder interface
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Prevent unused warning

    // 4. Create a decoder context to use with aom_codec_get_global_headers
    aom_codec_ctx_t codec_ctx;
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    // Initialize decoder
    aom_codec_err_t res = aom_codec_dec_init_ver(&codec_ctx, decoder_iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // 5. Use aom_uleb_size_in_bytes() with a value from input data
    uint64_t uleb_value = 0;
    if (size >= sizeof(uint64_t)) {
        memcpy(&uleb_value, data, sizeof(uint64_t));
    }
    size_t uleb_size = aom_uleb_size_in_bytes(uleb_value);
    (void)uleb_size; // Prevent unused warning

    // 6. Use aom_obu_type_to_string() - need to parse OBU type from input
    OBU_TYPE obu_type = OBU_SEQUENCE_HEADER; // Default type
    if (size > 0) {
        // Extract OBU type from first byte (bits 5-1 in AV1 spec)
        uint8_t first_byte = data[0];
        uint8_t obu_type_raw = (first_byte >> 3) & 0x1F;
        obu_type = static_cast<OBU_TYPE>(obu_type_raw);
    }
    const char *obu_type_str = aom_obu_type_to_string(obu_type);
    (void)obu_type_str; // Prevent unused warning

    // 7. Use aom_codec_get_global_headers() with decoder context
    // Note: This function is typically for encoders, but we'll call it anyway
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&codec_ctx);
    if (global_headers) {
        // Free the buffer if it was allocated
        free(global_headers->buf);
        free(global_headers);
    }

    // 8. Try to decode the input data to test the decoder
    // Use fmemopen as specified for FILE* operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read a small chunk to decode
        const size_t chunk_size = 4096;
        uint8_t *buffer = (uint8_t *)malloc(chunk_size);
        if (buffer) {
            size_t bytes_read = fread(buffer, 1, chunk_size, in_file);
            if (bytes_read > 0) {
                // Try to decode the chunk (ignore errors as input may be invalid)
                aom_codec_decode(&codec_ctx, buffer, bytes_read, NULL);
                
                // Try to get a frame from decoder
                aom_codec_iter_t iter = NULL;
                aom_image_t *img = aom_codec_get_frame(&codec_ctx, &iter);
                (void)img; // Prevent unused warning
            }
            free(buffer);
        }
        assert_file_closed(&in_file);;
    }

    // 9. Create output file as specified
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write version info to output
        fprintf(out_file, "Codec version: %d\n", version);
        assert_file_closed(&out_file);;
    }

    // 10. Clean up
    aom_codec_destroy(&codec_ctx);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}