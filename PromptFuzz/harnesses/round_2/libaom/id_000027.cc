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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 7
//<Quality> {"density":5,"unique_branches":{"aom_read_obu_header_and_size":[[101,7,101,16,0,0,4,0],[106,9,106,31,0,0,4,0],[106,9,106,31,0,0,4,1],[115,7,115,34,0,0,4,0],[118,9,118,36,0,0,4,0],[118,9,118,36,0,0,4,1]],"read_obu_header":[[51,34,51,44,0,0,4,1]]},"library_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_err_to_string","aom_codec_peek_stream_info","aom_img_alloc_with_border","aom_img_free"],"critical_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_err_to_string","aom_codec_peek_stream_info","aom_img_alloc_with_border","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Decode ULEB128 value from input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (size > 0) {
        // Use aom_uleb_decode to parse a ULEB128 value from the input
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    }

    // Step 2: Get AV1 decoder capabilities
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    
    // Convert capabilities to error string for demonstration
    // (This is artificial since caps isn't an error, but shows API usage)
    const char *caps_str = aom_codec_err_to_string(static_cast<aom_codec_err_t>(caps & 0xFF));

    // Step 3: Peek at stream information from input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.is_annexb = 1; // Assume Annex-B format
    
    if (size > 0) {
        // Try to peek at stream info using the input data
        aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    }

    // Step 4: Allocate image with border using dimensions from stream info
    // Use decoded ULEB value for width if valid, otherwise default
    unsigned int width = (uleb_value > 0 && uleb_value < 4096) ? 
                         static_cast<unsigned int>(uleb_value) : 256;
    unsigned int height = (stream_info.h > 0) ? stream_info.h : 256;
    
    aom_image_t image;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    unsigned int align = 32;
    unsigned int size_align = 1;
    unsigned int border = 16;
    
    // Allocate image with border using dimensions from stream info
    aom_image_t *img_ptr = aom_img_alloc_with_border(&image, fmt, width, height, 
                                                     align, size_align, border);
    
    // Clean up allocated resources
    if (img_ptr != NULL) {
        aom_img_free(img_ptr);
    }

    // The caps_str variable is intentionally not used to avoid unused variable warning
    (void)caps_str;

    return 0;
}