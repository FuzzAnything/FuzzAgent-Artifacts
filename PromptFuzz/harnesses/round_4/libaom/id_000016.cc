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
//<ID> 39
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_decode","aom_codec_get_stream_info","aom_codec_error_detail","aom_uleb_decode","aom_codec_get_frame","aom_img_num_metadata","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_error_detail","aom_uleb_decode","aom_codec_get_frame","aom_img_num_metadata","aom_codec_destroy"],"visited":0}
/**/


// Include libaom headers
extern "C" {
}

// Function prototypes from libaom that we need
extern "C" {
size_t aom_uleb_size_in_bytes(uint64_t value);
int aom_uleb_decode(const uint8_t *buffer, size_t available, uint64_t *value, size_t *length);
int aom_uleb_encode(uint64_t value, size_t available, uint8_t *coded_value, size_t *coded_size);
int aom_uleb_encode_fixed_size(uint64_t value, size_t available, size_t pad_to_size, uint8_t *coded_value, size_t *coded_size);
aom_image_t *aom_img_alloc(aom_image_t *img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align);
void aom_img_free(aom_image_t *img);
size_t aom_img_num_metadata(const aom_image_t *img);
const char *aom_codec_version_str();
const char *aom_codec_error_detail(const aom_codec_ctx_t *ctx);
aom_codec_err_t aom_codec_get_stream_info(aom_codec_ctx_t *ctx, aom_codec_stream_info_t *si);
aom_codec_err_t aom_codec_destroy(aom_codec_ctx_t *ctx);
aom_codec_err_t aom_codec_dec_init_ver(aom_codec_ctx_t *ctx, aom_codec_iface_t *iface, const aom_codec_dec_cfg_t *cfg, aom_codec_flags_t flags, int ver);
aom_codec_err_t aom_codec_decode(aom_codec_ctx_t *ctx, const uint8_t *data, size_t data_sz, void *user_priv);
aom_image_t *aom_codec_get_frame(aom_codec_ctx_t *ctx, aom_codec_iter_t *iter);
aom_codec_iface_t *aom_codec_av1_dx();
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
	//fuzzer shim end}




    // 1. First, call aom_codec_version_str() - this doesn't need any input
    const char *version_str = aom_codec_version_str();
    (void)version_str; // Use the variable to avoid unused warning
    
    // 2. Create a decoder context to process the AV1 bitstream
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1; // Use single thread for fuzzing
    
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }
    
    // Initialize decoder
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, iface, &cfg, fuzz_int64_t_1, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        // Call aom_codec_error_detail on error
        const char *error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail; // Use the variable
        return 0;
    }
    
    // 3. Try to get stream information from the input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Note: aom_codec_get_stream_info typically requires some data to have been decoded first
    // So we'll decode first, then try to get stream info
    if (size > 0) {
        // Try to decode the input data
        err = aom_codec_decode(&decoder, data, size, nullptr);
        
        // Now try to get stream info (may or may not succeed depending on valid data)
        err = aom_codec_get_stream_info(&decoder, &stream_info);
        
        // Call aom_codec_error_detail to check for any errors
        const char *error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail; // Use the variable
    }
    
    // 4. Try to decode a ULEB value from the input data
    if (size > 0) {
        uint64_t uleb_value = 0;
        size_t uleb_length = 0;
        
        // Try to decode a ULEB value from the beginning of the data
        int uleb_result = aom_uleb_decode(data, size, &uleb_value, &uleb_length);
        (void)uleb_result; // Use the variable
    }
    
    // 5. Try to get frames from the decoder and check metadata
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    int frame_count = 0;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        frame_count++;
        
        // Call aom_img_num_metadata to get the number of metadata entries
        size_t num_metadata = aom_img_num_metadata(img);
        (void)num_metadata; // Use the variable
    }
    
    // 6. Clean up
    aom_codec_destroy(&decoder);
    
    return 0;
}