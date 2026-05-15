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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_global_headers","aom_codec_get_frame","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_global_headers","aom_codec_get_frame","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    (void)fd_in; // Use to avoid unused variable warning
    
    // Step 1: Use aom_uleb_decode on the input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    // Use first min(8, size) bytes for ULEB decoding
    size_t uleb_available = size > 8 ? 8 : size;
    int uleb_result = aom_uleb_decode(data, uleb_available, &uleb_value, &uleb_length);
    (void)uleb_result; // Result may be error, but we continue anyway
    
    // Step 2: Initialize decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Step 3: Get codec interface name
    const char *iface_name = aom_codec_iface_name(decoder_iface);
    (void)iface_name; // Use to avoid unused variable warning
    
    // Step 4: Initialize decoder context
    aom_codec_ctx_t codec_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 128;  // Default width
    dec_cfg.h = 128;  // Default height
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &codec_ctx, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (init_result != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Step 5: Decode the input data
    aom_codec_err_t decode_result = aom_codec_decode(
        &codec_ctx, data, size, nullptr);
    
    // Step 6: Get global headers (may return NULL for decoder)
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&codec_ctx);
    if (global_headers) {
        // Global headers available, but we don't need to use them further
        // The buffer is owned by the codec context
    }
    
    // Step 7: Iterate through decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *frame = nullptr;
    
    while ((frame = aom_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
        // Frame available, but we don't need to process it for fuzzing
        // Just ensure the API is called
        
        // Step 8: Get preview frame (if available)
        const aom_image_t *preview_frame = aom_codec_get_preview_frame(&codec_ctx);
        (void)preview_frame; // Use to avoid unused variable warning
        
        // Break after first frame to avoid infinite loop
        break;
    }
    
    // Step 9: Iterate through encoded data packets (for encoder side, but we call anyway)
    // Reset iterator
    iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    
    // Create output file for writing if needed
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Get file descriptor for writing
        int fd_out = fuzz_fileno(out_file);
        (void)fd_out; // Use to avoid unused variable warning
        
        // Try to get encoded data packets
        while ((pkt = aom_codec_get_cx_data(&codec_ctx, &iter)) != nullptr) {
            // Write packet data to output file if it's a frame packet
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
            // Break after first packet to avoid infinite loop
            break;
        }
        assert_file_closed(&out_file);;
    }
    
    // Step 10: Clean up resources
    aom_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}