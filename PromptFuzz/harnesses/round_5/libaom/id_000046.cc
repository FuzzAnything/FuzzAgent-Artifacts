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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"aom_codec_err_to_string":[[42,5,42,29,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_error","aom_codec_decode","aom_codec_get_preview_frame","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_get_cx_data","aom_codec_error","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_preview_frame","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_get_cx_data","aom_codec_error","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    const char *iface_name = aom_codec_iface_name(decoder_iface);
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                                  0, AOM_DECODER_ABI_VERSION);
    
    if (res != AOM_CODEC_OK) {
        const char *error_str = aom_codec_error(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read and decode frames
    std::vector<uint8_t> buffer(size);
    size_t bytes_read = fread(buffer.data(), 1, size, in_file);
    
    if (bytes_read > 0) {
        // Decode the input data
        res = aom_codec_decode(&decoder, buffer.data(), bytes_read, nullptr);
        
        if (res == AOM_CODEC_OK) {
            // Try to get preview frame
            const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
            
            // Get decoded frames
            aom_codec_iter_t iter = nullptr;
            const aom_image_t *frame;
            while ((frame = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
                // Get metadata if available
                size_t metadata_count = aom_img_num_metadata(frame);
                for (size_t i = 0; i < metadata_count; i++) {
                    const aom_metadata_t *metadata = aom_img_get_metadata(frame, i);
                    // Use metadata if needed
                }
            }
            
            // Try to get global headers (though typically for encoder)
            aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&decoder);
            
            // Try encoder operations (may fail since we have decoder context)
            aom_fixed_buf_t cx_buf = {nullptr, 0};
            aom_codec_set_cx_data_buf(&decoder, &cx_buf, 0, 0);
            
            // Try to get compressed data (may fail with decoder)
            aom_codec_iter_t cx_iter = nullptr;
            const aom_codec_cx_pkt_t *pkt;
            while ((pkt = aom_codec_get_cx_data(&decoder, &cx_iter)) != nullptr) {
                // Process compressed data packets if any
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
        }
        
        // Get error status after operations
        const char *error_detail = aom_codec_error(&decoder);
    }
    
    // Cleanup
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}