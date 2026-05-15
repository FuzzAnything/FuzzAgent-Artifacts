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
//<ID> 157
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"read_uncompressed_header":[[4893,15,4899,21,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_error_detail","aom_codec_get_stream_info","aom_img_alloc","aom_img_metadata_alloc","aom_img_num_metadata","aom_img_metadata_free","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_error_detail","aom_codec_get_stream_info","aom_img_alloc","aom_img_metadata_alloc","aom_img_num_metadata","aom_img_metadata_free","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callbacks
static int  id_000054_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)min_size;
    static uint8_t dummy_buffer[4096];
    fb->data = dummy_buffer;
    fb->size = sizeof(dummy_buffer);
    return 0;
}

static int  id_000054_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get encoder interface
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get codec capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps;
    
    // 5. Peek stream info from input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    
    // 6. Initialize decoder
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = stream_info.w;
    dec_cfg.h = stream_info.h;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg,
                                                0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Set frame buffer functions
    err = aom_codec_set_frame_buffer_functions(&decoder_ctx,  id_000054_get_frame_buffer,
                                               id_000054_release_frame_buffer, nullptr);
    
    // 8. Decode the input data
    err = aom_codec_decode(&decoder_ctx, data, size, nullptr);
    
    // 9. Get error detail if any
    const char *error_detail = aom_codec_error_detail(&decoder_ctx);
    (void)error_detail;
    
    // 10. Get stream info from decoder
    aom_codec_get_stream_info(&decoder_ctx, &stream_info);
    
    // 11. Create an image for encoding
    aom_image_t *img = nullptr;
    if (stream_info.w > 0 && stream_info.h > 0 && stream_info.w < 4096 && stream_info.h < 4096) {
        img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, stream_info.w, stream_info.h, 1);
    }
    
    if (img) {
        // 12. Create metadata
        uint32_t metadata_type = 1;
        aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
        
        // Use part of input data for metadata (safe bounds check)
        size_t metadata_size = size > 100 ? 100 : size;
        aom_metadata_t *metadata = aom_img_metadata_alloc(
            metadata_type, data, metadata_size, insert_flag);
        
        if (metadata) {
            // 13. Get number of metadata (will be 0 since we haven't added metadata to image)
            size_t num_metadata = aom_img_num_metadata(img);
            (void)num_metadata;
            
            // Note: aom_img_add_metadata is not in our required list, so we skip adding
            
            // 14. Free metadata
            aom_img_metadata_free(metadata);
        }
        
        // 15. Initialize encoder
        aom_codec_ctx_t encoder_ctx;
        aom_codec_enc_cfg_t enc_cfg;
        
        err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        if (err == AOM_CODEC_OK) {
            // Set reasonable dimensions
            if (stream_info.w > 0 && stream_info.h > 0) {
                enc_cfg.g_w = stream_info.w;
                enc_cfg.g_h = stream_info.h;
            } else {
                enc_cfg.g_w = 640;
                enc_cfg.g_h = 480;
            }
            enc_cfg.g_threads = 1;
            enc_cfg.rc_target_bitrate = 1000;
            
            err = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg,
                                        0, AOM_ENCODER_ABI_VERSION);
            
            if (err == AOM_CODEC_OK) {
                // 16. Get global headers
                aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder_ctx);
                if (global_headers) {
                    free(global_headers->buf);
                    free(global_headers);
                }
                
                // 17. Set cx data buffer
                std::vector<uint8_t> output_buffer(4096);
                aom_fixed_buf_t buf;
                buf.buf = output_buffer.data();
                buf.sz = output_buffer.size();
                aom_codec_set_cx_data_buf(&encoder_ctx, &buf, 0, 0);
                
                // 18. Encode the image
                aom_codec_encode(&encoder_ctx, img, 0, 1, 0);
                
                // 19. Get encoded data
                aom_codec_iter_t iter = nullptr;
                const aom_codec_cx_pkt_t *pkt;
                while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &iter)) != nullptr) {
                    // Process packet if needed
                    (void)pkt;
                }
                
                // 20. Destroy encoder
                aom_codec_destroy(&encoder_ctx);
            }
        }
        
        // 21. Free image
        aom_img_free(img);
    }
    
    // 22. Destroy decoder
    aom_codec_destroy(&decoder_ctx);
    
    // 23. Clean up
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}