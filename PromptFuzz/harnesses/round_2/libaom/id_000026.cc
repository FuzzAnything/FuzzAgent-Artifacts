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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_err_to_string","aom_codec_get_caps","aom_uleb_decode","aom_img_alloc_with_border","aom_img_free"],"critical_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_err_to_string","aom_codec_get_caps","aom_uleb_decode","aom_img_alloc_with_border","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // API 1: aom_codec_peek_stream_info - Parse stream information from encoded data
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    aom_codec_err_t peek_err = aom_codec_peek_stream_info(iface, data, size, &stream_info);
    
    // API 5: aom_codec_err_to_string - Convert error code to string
    const char *err_str = aom_codec_err_to_string(peek_err);
    
    // API 2: aom_codec_get_caps - Get codec capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    
    // API 3: aom_uleb_decode - Decode ULEB value from input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    const size_t max_uleb_decode = size > 10 ? 10 : size; // Limit for safety
    int uleb_result = aom_uleb_decode(data, max_uleb_decode, &uleb_value, &uleb_length);
    
    // API 4: aom_img_alloc_with_border - Allocate image with border
    // Use stream info dimensions if available, otherwise use reasonable defaults
    unsigned int width = stream_info.w > 0 ? stream_info.w : 320;
    unsigned int height = stream_info.h > 0 ? stream_info.h : 240;
    
    aom_image_t img;
    aom_image_t *img_ptr = aom_img_alloc_with_border(&img, AOM_IMG_FMT_I420, 
                                                     width, height, 
                                                     32,  // align
                                                     1,   // size_align
                                                     16); // border
    
    if (img_ptr) {
        // Clean up allocated image
        aom_img_free(img_ptr);
    }
    
    // Use FILE* operations as specified in requirements
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Could perform additional file operations here
        assert_file_closed(&in_file);;
    }
    
    assert_file_closed(&in_file);
	return 0;
}