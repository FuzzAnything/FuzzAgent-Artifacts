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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["aom_uleb_size_in_bytes","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_destroy","aom_codec_get_frame","aom_codec_get_preview_frame","aom_obu_type_to_string","aom_img_remove_metadata","aom_obu_type_to_string","aom_codec_get_preview_frame","aom_codec_destroy"],"critical_calls":["aom_uleb_size_in_bytes","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_get_preview_frame","aom_obu_type_to_string","aom_img_remove_metadata","aom_obu_type_to_string","aom_codec_get_preview_frame","aom_codec_destroy"],"visited":0}
/*We can create a fuzzer that processes an encoded AV1 bitstream. The event these APIs could achieve together is: **decoding an AV1 encoded bitstream and examining/extracting information about the decoded frames**.

Here's the implementation:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Use aom_uleb_size_in_bytes - calculate size for a sample value
    uint64_t sample_value = 100;
    size_t uleb_size = aom_uleb_size_in_bytes(sample_value);
    
    // 3. Get AV1 decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // 4. Get interface name using aom_codec_iface_name
    const char *iface_name = aom_codec_iface_name(decoder_iface);
    
    // 5. Peek at stream info using aom_codec_peek_stream_info
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    
    // 6. Initialize decoder using aom_codec_dec_init_ver
    aom_codec_ctx_t codec_ctx;
    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.w = stream_info.w;
    dec_cfg.h = stream_info.h;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &codec_ctx, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (init_result != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Decode the input data
    aom_codec_err_t decode_result = aom_codec_decode(
        &codec_ctx, data, size, nullptr);
    
    if (decode_result != AOM_CODEC_OK) {
        aom_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 8. Get frames using aom_codec_get_frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    
    while ((img = aom_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
        // 9. Try to get preview frame using aom_codec_get_preview_frame
        const aom_image_t *preview_frame = aom_codec_get_preview_frame(&codec_ctx);
        
        // 10. Process OBU type - sample usage (OBU type 1 is SEQUENCE_HEADER)
        const char *obu_type_str = aom_obu_type_to_string(static_cast<OBU_TYPE>(1));
        
        // 11. Remove metadata from the image using aom_img_remove_metadata
        aom_img_remove_metadata(img);
        
        // Try to process more OBU types based on available data
        if (size > 0) {
            // Use first byte as OBU type (masked to valid range 0-15)
            uint8_t obu_type_byte = data[0] & 0x0F;
            if (obu_type_byte <= 15) {
                const char *another_obu_str = aom_obu_type_to_string(
                    static_cast<OBU_TYPE>(obu_type_byte));
            }
        }
    }
    
    // 12. Try to get one more preview frame
    const aom_image_t *final_preview = aom_codec_get_preview_frame(&codec_ctx);
    
    // 13. Cleanup - destroy codec context
    aom_codec_destroy(&codec_ctx);
    
    // 14. Close input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}