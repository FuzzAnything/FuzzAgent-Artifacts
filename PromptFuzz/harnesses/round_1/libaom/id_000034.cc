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
//<ID> 73
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_error","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_obu_type_to_string","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_get_global_headers","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_error","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_obu_type_to_string","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_get_global_headers","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_error","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Get version string
    const char *version_str = aom_codec_version_str();

    // 2. Create a FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Get AV1 decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    const char *iface_name = aom_codec_iface_name(decoder_iface);

    // 4. Initialize decoder
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
        &decoder_ctx, 
        decoder_iface, 
        &dec_cfg, 
        0, 
        AOM_DECODER_ABI_VERSION
    );
    
    if (dec_init_result != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Set frame buffer functions (using NULL callbacks for simplicity)
    aom_codec_set_frame_buffer_functions(
        &decoder_ctx, 
        nullptr, 
        nullptr, 
        nullptr
    );

    // 6. Read and decode the entire input
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    uint8_t *input_buffer = (uint8_t *)malloc(file_size);
    if (!input_buffer) {
        assert_file_closed(&in_file);;
        aom_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(input_buffer, 1, file_size, in_file);
    assert_file_closed(&in_file);;
    
    aom_codec_decode(
        &decoder_ctx, 
        input_buffer, 
        file_size, 
        nullptr
    );
    
    free(input_buffer);

    // 7. Get stream info
    aom_codec_stream_info_t stream_info;
    aom_codec_get_stream_info(&decoder_ctx, &stream_info);

    // 8. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_frame = nullptr;
    
    while ((decoded_frame = aom_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
        // 9. Print OBU type (for demonstration, using a hypothetical type)
        const char *obu_str = aom_obu_type_to_string((OBU_TYPE)1);
        
        // 10. Get plane dimensions
        int plane0_width = aom_img_plane_width(decoded_frame, 0);
        int plane0_height = aom_img_plane_height(decoded_frame, 0);
        
        // 11. Allocate a new image for encoding
        aom_image_t *image_to_encode = aom_img_alloc(
            nullptr, 
            AOM_IMG_FMT_I420, 
            decoded_frame->d_w, 
            decoded_frame->d_h, 
            1
        );
        
        if (!image_to_encode) {
            continue;
        }
        
        // 12. Remove metadata from the new image
        aom_img_remove_metadata(image_to_encode);
        
        // 13. Get AV1 encoder interface
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        
        // 14. Initialize encoder configuration
        aom_codec_enc_cfg_t enc_cfg;
        aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        
        // Set basic parameters
        enc_cfg.g_w = decoded_frame->d_w;
        enc_cfg.g_h = decoded_frame->d_h;
        enc_cfg.g_threads = 1;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        
        // 15. Initialize encoder
        aom_codec_ctx_t encoder_ctx;
        aom_codec_enc_init_ver(
            &encoder_ctx, 
            encoder_iface, 
            &enc_cfg, 
            0, 
            AOM_ENCODER_ABI_VERSION
        );
        
        // 16. Set encoder configuration (reapply)
        aom_codec_enc_config_set(&encoder_ctx, &enc_cfg);
        
        // 17. Get global headers
        aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder_ctx);
        
        // 18. Encode the image
        aom_codec_encode(
            &encoder_ctx, 
            image_to_encode, 
            0,  // pts
            1,  // duration
            0   // flags
        );
        
        // 19. Get encoded data (iteratively)
        aom_codec_iter_t enc_iter = nullptr;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
            // Process encoded packets if needed
        }
        
        // 20. Cleanup encoder resources
        if (global_headers) {
            // Note: aom_fixed_buf_t should be freed if allocated by library
            // but aom_codec_get_global_headers returns a pointer to internal buffer
        }
        
        aom_codec_destroy(&encoder_ctx);
        
        // 21. Free the allocated image
        aom_img_free(image_to_encode);
    }
    
    // 22. Get any decoder errors
    const char *decoder_error = aom_codec_error(&decoder_ctx);
    
    // 23. Cleanup decoder
    aom_codec_destroy(&decoder_ctx);
    
    assert_file_closed(&in_file);
	return 0;
}