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
//<ID> 115
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 14
//<Quality> {"density":11,"unique_branches":{"ctrl_get_last_ref_updates":[[1046,7,1046,18,0,0,4,0],[1047,9,1047,26,0,0,4,0]],"ctrl_get_frame_corrupted":[[1257,7,1257,16,0,0,4,0],[1258,9,1258,26,0,0,4,0],[1263,11,1263,33,0,0,4,0],[1263,11,1263,33,0,0,4,1],[1263,37,1263,64,0,0,4,0],[1263,37,1263,64,0,0,4,1],[1265,11,1265,39,0,0,4,1]],"aom_decode_frame_from_obus":[[978,15,978,38,0,0,4,1],[990,13,990,36,0,0,4,1],[991,14,991,35,0,0,4,1],[1001,15,1001,47,0,0,4,1],[1002,15,1002,65,0,0,4,0]]},"library_calls":["aom_uleb_encode_fixed_size","aom_img_wrap","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_error","aom_codec_error_detail","aom_codec_decode","aom_codec_error","aom_codec_error_detail","aom_codec_control","aom_codec_get_frame","aom_codec_destroy","aom_img_free"],"critical_calls":["aom_uleb_encode_fixed_size","aom_img_wrap","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error","aom_codec_error_detail","aom_codec_control","aom_codec_get_frame","aom_codec_destroy","aom_img_free"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to decode and process AV1 video data:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // 1. First, use aom_uleb_encode_fixed_size to encode a value
    uint64_t test_value = 123456;
    uint8_t encoded_buffer[16];
    size_t encoded_size = 0;
    aom_uleb_encode_fixed_size(test_value, sizeof(encoded_buffer), 8, 
                               encoded_buffer, &encoded_size);
    
    // 2. Create image wrapper for potential use
    aom_image_t img;
    unsigned int width = 640;
    unsigned int height = 480;
    unsigned char *image_data = (unsigned char *)malloc(width * height * 3 / 2);
    
    if (!image_data) {
	return 0;
	}
	
    
    // Wrap image with dummy data
    aom_img_wrap(&img, AOM_IMG_FMT_I420, width, height, 1, image_data);
    
    // 3. Initialize decoder context
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Try to peek stream info first
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    aom_codec_err_t peek_result = aom_codec_peek_stream_info(
        decoder_iface, data, size, &stream_info);
    
    // Initialize decoder with default config
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &codec_ctx, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (init_result != AOM_CODEC_OK) {
        // Get error information
        const char *error_str = aom_codec_error(&codec_ctx);
        const char *error_detail = aom_codec_error_detail(&codec_ctx);
        
        // Clean up and return
        free(image_data);
        return 0;
    }
    
    // 4. Try to decode the input data
    aom_codec_err_t decode_result = aom_codec_decode(
        &codec_ctx, data, size, NULL);
    
    // Get error information after decode attempt
    const char *error_str = aom_codec_error(&codec_ctx);
    const char *error_detail = aom_codec_error_detail(&codec_ctx);
    
    // 5. Use aom_codec_control with various control IDs
    // Try a few common decoder controls
    int ctrl_ids[] = {AOMD_GET_LAST_REF_UPDATES, AOMD_GET_FRAME_CORRUPTED};
    
    for (int i = 0; i < sizeof(ctrl_ids)/sizeof(ctrl_ids[0]); i++) {
        int value = 0;
        aom_codec_control(&codec_ctx, ctrl_ids[i], &value);
    }
    
    // Try to get frames from decoder
    aom_codec_iter_t iter = NULL;
    aom_image_t *decoded_frame = NULL;
    
    while ((decoded_frame = aom_codec_get_frame(&codec_ctx, &iter)) != NULL) {
        // Frame available, but we don't need to process it for fuzzing
    }
    
    // 6. Create output file and write encoded ULEB data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(encoded_buffer, 1, encoded_size, out_file);
        assert_file_closed(&out_file);;
    }
    
    // 7. Clean up all allocated resources
    aom_codec_destroy(&codec_ctx);
    aom_img_free(&img);
    free(image_data);
    
    assert_file_closed(&out_file);
	return 0;
}