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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 7
//<Quality> {"density":9,"unique_branches":{"aom_read_obu_header_and_size":[[101,7,101,16,0,0,4,0],[106,9,106,31,0,0,4,0],[106,9,106,31,0,0,4,1],[115,7,115,34,0,0,4,0],[118,9,118,36,0,0,4,0],[118,9,118,36,0,0,4,1]],"read_obu_header":[[51,34,51,44,0,0,4,1]]},"library_calls":["aom_uleb_size_in_bytes","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_get_preview_frame","aom_img_remove_metadata","aom_obu_type_to_string","aom_codec_destroy","aom_img_alloc","aom_img_add_metadata","aom_img_remove_metadata","aom_img_free"],"critical_calls":["aom_uleb_size_in_bytes","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_get_preview_frame","aom_img_remove_metadata","aom_obu_type_to_string","aom_codec_destroy","aom_img_alloc","aom_img_add_metadata","aom_img_remove_metadata","aom_img_free"],"visited":0}
/**/


extern "C" {
}

// Helper function to parse OBU type from bitstream
static OBU_TYPE  id_000001_extract_obu_type(const uint8_t* data, size_t size) {
    if (size < 1) return OBU_SEQUENCE_HEADER;
    uint8_t obu_header = data[0];
    OBU_TYPE type = (OBU_TYPE)((obu_header >> 3) & 0x0F);
    return type;
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (!data || size < 1) {
	return 0;
	}
	

    // 1. Use aom_uleb_size_in_bytes
    uint64_t test_value = 12345;
    size_t uleb_size = aom_uleb_size_in_bytes(test_value);

    // 2. Create decoder interface and use aom_codec_iface_name
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    const char* iface_name = aom_codec_iface_name(decoder_iface);
    (void)iface_name; // Mark as used

    // 3. Use aom_codec_peek_stream_info to examine the input stream
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.is_annexb = 1; // Assume Annex-B format
    aom_codec_err_t peek_result = aom_codec_peek_stream_info(
        decoder_iface, data, size, &stream_info);

    // 4. Initialize decoder configuration
    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;

    // 5. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_flags_t flags = 0;
    int decoder_version = AOM_DECODER_ABI_VERSION;
    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, flags, decoder_version);

    // Only proceed if decoder initialization was successful
    if (init_result == AOM_CODEC_OK) {
        // 6. Decode the input data
        aom_codec_err_t decode_result = aom_codec_decode(
            &decoder, data, size, nullptr);

        // 7. Extract and process frames if decoding was successful
        if (decode_result == AOM_CODEC_OK) {
            aom_codec_iter_t iter = nullptr;
            aom_image_t* frame = nullptr;

            // 8. Get decoded frames
            while ((frame = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
                // 9. Use aom_codec_get_preview_frame (if available)
                const aom_image_t* preview_frame = aom_codec_get_preview_frame(&decoder);
                (void)preview_frame; // Mark as used

                // 10. Remove metadata from the frame
                aom_img_remove_metadata(frame);

                // 11. Parse OBU type from the beginning of the data
                OBU_TYPE obu_type =  id_000001_extract_obu_type(data, (size > 10) ? 10 : size);
                
                // 12. Use aom_obu_type_to_string
                const char* obu_type_str = aom_obu_type_to_string(obu_type);
                (void)obu_type_str; // Mark as used

                // Note: We don't actually need to write the frame anywhere
                // as this is a fuzzer test
            }
        }

        // 13. Clean up decoder
        aom_codec_err_t destroy_result = aom_codec_destroy(&decoder);
        (void)destroy_result; // Mark as used
    }

    // 14. Additional test: Create a mock image to test metadata removal
    // This ensures aom_img_remove_metadata is called even if decoder fails
    aom_image_t test_img;
    memset(&test_img, 0, sizeof(test_img));
    test_img.fmt = AOM_IMG_FMT_I420;
    test_img.d_w = 64;
    test_img.d_h = 64;
    test_img.x_chroma_shift = 1;
    test_img.y_chroma_shift = 1;
    
    // Allocate image
    if (aom_img_alloc(&test_img, AOM_IMG_FMT_I420, 64, 64, 16)) {
        // Add some metadata (simulated)
        uint8_t metadata_data[] = {1, 2, 3, 4};
        int metadata_result = aom_img_add_metadata(
            &test_img, 1, metadata_data, 4, AOM_MIF_ANY_FRAME);
        (void)metadata_result; // Mark as used

        // Remove metadata (this is the API we need to test)
        aom_img_remove_metadata(&test_img);
        
        // Clean up
        aom_img_free(&test_img);
    }

    return 0;
}