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
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_preview_frame","aom_img_alloc","aom_img_add_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_preview_frame","aom_img_alloc","aom_img_add_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


// Include necessary libaom headers (these would normally be provided by the library)
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for processing
    
    // 1. Get version information
    const char* version_str = aom_codec_version_str();
    const char* extra_str = aom_codec_version_extra_str();
    
    // Use the version strings to avoid unused variable warnings
    if (version_str && extra_str) {
        // Just reference them to prevent optimization
        volatile const char* v = version_str;
        volatile const char* e = extra_str;
        (void)v;
        (void)e;
    }
    
    // 2. Create a decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    
    // Initialize decoder with default configuration
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    aom_codec_err_t dec_init_res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (dec_init_res != AOM_CODEC_OK) {
        // Cleanup and return if decoder initialization fails
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // 3. Peek at stream information
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_peek_stream_info(decoder_iface, data, size, &si);
    
    // 4. Decode the input data
    aom_codec_decode(&decoder, data, size, nullptr);
    
    // 5. Get stream info from decoder context
    aom_codec_stream_info_t si2;
    memset(&si2, 0, sizeof(si2));
    aom_codec_get_stream_info(&decoder, &si2);
    
    // 6. Try to get a preview frame
    aom_codec_iter_t iter = nullptr;
    const aom_image_t* preview_frame = aom_codec_get_preview_frame(&decoder);
    
    // 7. Create an image for metadata operations
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
    
    // 8. Add metadata to the image
    uint8_t metadata_data[] = {0x01, 0x02, 0x03, 0x04};
    aom_img_add_metadata(&img, 1, metadata_data, sizeof(metadata_data), AOM_MIF_ANY_FRAME);
    
    // 9. Retrieve metadata from the image
    const aom_metadata_t* retrieved_metadata = aom_img_get_metadata(&img, 0);
    
    // 10. Create encoder context
    aom_codec_ctx_t encoder;
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    // Adjust configuration for our test
    enc_cfg.g_w = 64;
    enc_cfg.g_h = 64;
    enc_cfg.rc_target_bitrate = 1000;
    
    // Initialize encoder
    aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    
    // 11. Set encoder configuration (reapply)
    aom_codec_enc_config_set(&encoder, &enc_cfg);
    
    // 12. Create a fixed buffer for compressed data
    aom_fixed_buf_t buf;
    buf.buf = malloc(10000); // Allocate buffer for compressed data
    buf.sz = 10000;
    
    // 13. Set the compressed data buffer
    aom_codec_set_cx_data_buf(&encoder, &buf, 0, 0);
    
    // 14. Encode the image we created
    aom_codec_encode(&encoder, &img, 0, 1, 0);
    
    // 15. Iterate through encoded data packets
    iter = nullptr;
    const aom_codec_cx_pkt_t* pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        // Process packet if needed
    }
    
    // Cleanup resources
    if (buf.buf) {
        free(buf.buf);
    }
    
    aom_img_free(&img);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    
    return 0;
}