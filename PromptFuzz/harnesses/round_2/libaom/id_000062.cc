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
//<ID> 249
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 4
//<Quality> {"density":24,"unique_branches":{"blend_a64_mask_bn_sx_sy_w8n_sse4_1":[[772,17,772,22,0,0,4,0],[772,17,772,22,0,0,4,1],[792,12,792,15,0,0,4,0],[792,12,792,15,0,0,4,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy","aom_img_wrap","aom_codec_destroy","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_remove_metadata","aom_codec_destroy","aom_codec_enc_init_ver","aom_img_remove_metadata","aom_codec_destroy","aom_codec_control","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_remove_metadata","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_wrap","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_remove_metadata","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzzer that achieves the specified event using the provided libaom APIs:

*/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // Create a decoder context to process the input byte stream
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;

    if (aom_codec_dec_init_ver(&decoder, iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // Decode the input data (assuming it's valid encoded data)
    aom_codec_decode(&decoder, data, size, nullptr);
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
    
    if (!decoded_img) {
        aom_codec_destroy(&decoder);
        return 0;
    }

    // Use aom_img_wrap to create a new image wrapper
    const unsigned int align = 32;
    const unsigned int d_w = decoded_img->d_w;
    const unsigned int d_h = decoded_img->d_h;
    aom_img_fmt_t fmt = decoded_img->fmt;
    
    // Allocate image data for wrapping
    size_t img_data_size = d_w * d_h * 3; // Conservative estimate
    unsigned char *img_data = new unsigned char[img_data_size];
    
    aom_image_t wrapped_img;
    aom_image_t *img_result = aom_img_wrap(&wrapped_img, fmt, d_w, d_h, align, img_data);
    
    if (!img_result) {
        delete[] img_data;
        aom_codec_destroy(&decoder);
        return 0;
    }

    // Use aom_img_add_metadata to add metadata to the wrapped image
    // Use first 50 bytes of input data as metadata (or less if size is smaller)
    size_t metadata_size = size > 50 ? 50 : size;
    uint32_t metadata_type = 1; // Arbitrary type
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
    
    int add_metadata_result = aom_img_add_metadata(&wrapped_img, metadata_type, 
                                                  data, metadata_size, insert_flag);
    
    // Check if metadata was added successfully
    if (add_metadata_result == 0) {
        // Use aom_img_num_metadata to get number of metadata entries
        size_t num_metadata = aom_img_num_metadata(&wrapped_img);
        
        // Use aom_img_get_metadata to retrieve metadata if available
        if (num_metadata > 0) {
            const aom_metadata_t *metadata = aom_img_get_metadata(&wrapped_img, 0);
            // Metadata retrieved, could be used here
        }
    }

    // Create an encoder context
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    // Use aom_codec_enc_config_default to get default encoder configuration
    unsigned int usage = 0; // Good quality
    if (aom_codec_enc_config_default(enc_iface, &enc_cfg, usage) != AOM_CODEC_OK) {
        aom_img_remove_metadata(&wrapped_img);
        delete[] img_data;
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Set basic encoder parameters
    enc_cfg.g_w = d_w;
    enc_cfg.g_h = d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000; // 1 Mbps
    enc_cfg.g_profile = 0;
    enc_cfg.g_bit_depth = AOM_BITS_8;
    enc_cfg.g_input_bit_depth = 8;
    enc_cfg.g_threads = 1;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;

    // Initialize encoder
    if (aom_codec_enc_init_ver(&encoder, enc_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_img_remove_metadata(&wrapped_img);
        delete[] img_data;
        aom_codec_destroy(&decoder);
        return 0;
    }

    // Use aom_codec_control to set encoder parameters
    int ctrl_id = AOME_SET_CPUUSED;
    int cpu_used = 8; // Balanced quality/speed
    aom_codec_control(&encoder, ctrl_id, cpu_used);

    // Prepare output buffer for encoded data
    aom_fixed_buf_t output_buf;
    size_t output_buffer_size = size * 2; // Conservative estimate
    output_buf.buf = malloc(output_buffer_size);
    output_buf.sz = output_buffer_size;

    // Use aom_codec_set_cx_data_buf to set output buffer
    unsigned int pad_before = 0;
    unsigned int pad_after = 0;
    aom_codec_set_cx_data_buf(&encoder, &output_buf, pad_before, pad_after);

    // Encode the image (using dummy image data for simplicity)
    // In reality, we would copy data from decoded_img to wrapped_img
    aom_codec_encode(&encoder, &wrapped_img, 0, 1, 0);
    
    // Get encoded data (optional, for completeness)
    const aom_codec_cx_pkt_t *pkt = nullptr;
    aom_codec_iter_t enc_iter = nullptr;
    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        // Process encoded packet if needed
    }

    // Cleanup
    aom_codec_destroy(&encoder);
    aom_img_remove_metadata(&wrapped_img);
    delete[] img_data;
    free(output_buf.buf);
    aom_codec_destroy(&decoder);

    return 0;
}