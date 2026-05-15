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
//<score> 30, nr_unique_branch: 0
//<Quality> {"density":30,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_iface_name","aom_codec_get_caps","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_error","aom_codec_set_frame_buffer_functions","aom_codec_error","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_codec_enc_config_default","aom_codec_error","aom_codec_enc_init_ver","aom_codec_error","aom_codec_enc_config_set","aom_codec_error","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_codec_set_cx_data_buf","aom_codec_error","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_iface_name","aom_codec_get_caps","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_error","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_error","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_codec_set_cx_data_buf","aom_codec_error","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback structures
struct FrameBufferData {
    std::vector<uint8_t> buffer;
    size_t size;
};

static int  id_000046_get_frame_buffer(void *cb_priv, size_t min_size, 
                           aom_codec_frame_buffer_t *fb) {
    FrameBufferData *fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    // Allocate or resize buffer if needed
    if (fb_data->buffer.size() < min_size) {
        fb_data->buffer.resize(min_size);
        fb_data->size = min_size;
    }
    
    fb->data = fb_data->buffer.data();
    fb->size = fb_data->size;
    fb->priv = fb_data;
    
    return 0;
}

static int  id_000046_release_frame_buffer(void *cb_priv, 
                               aom_codec_frame_buffer_t *fb) {
    // Nothing to do here since we're managing the buffer ourselves
    (void)cb_priv;
    (void)fb;
    return 0;
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
    
    // Use fmemopen to create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get encoder interface
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get interface names
    const char *decoder_name = aom_codec_iface_name(decoder_iface);
    const char *encoder_name = aom_codec_iface_name(encoder_iface);
    (void)decoder_name;
    (void)encoder_name;
    
    // Get codec capabilities
    aom_codec_caps_t decoder_caps = aom_codec_get_caps(decoder_iface);
    aom_codec_caps_t encoder_caps = aom_codec_get_caps(encoder_iface);
    (void)decoder_caps;
    (void)encoder_caps;
    
    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, 
                                                &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&decoder);
        (void)error_detail;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set frame buffer functions
    FrameBufferData frame_buffer_data;
    res = aom_codec_set_frame_buffer_functions(&decoder,  id_000046_get_frame_buffer, 
                                               id_000046_release_frame_buffer, &frame_buffer_data);
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&decoder);
        (void)error_detail;
    }
    
    // Peek at stream info
    aom_codec_stream_info_t stream_info = {0};
    res = aom_codec_peek_stream_info(decoder_iface, data, 
                                     std::min(size, (size_t)4096), &stream_info);
    if (res != AOM_CODEC_OK) {
        // Continue anyway
    }
    
    // Decode the input
    res = aom_codec_decode(&decoder, data, size, nullptr);
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&decoder);
        (void)error_detail;
    }
    
    // Get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = aom_codec_get_frame(&decoder, &iter);
    
    // Initialize encoder if we got a frame
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t enc_cfg;
    
    if (img) {
        // Get default encoder config
        res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        if (res != AOM_CODEC_OK) {
            const char *error_detail = aom_codec_error(&encoder);
            (void)error_detail;
        } else {
            // Adjust config based on decoded frame
            enc_cfg.g_w = img->d_w;
            enc_cfg.g_h = img->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            
            // Initialize encoder
            res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                        0, AOM_ENCODER_ABI_VERSION);
            if (res != AOM_CODEC_OK) {
                const char *error_detail = aom_codec_error(&encoder);
                (void)error_detail;
            } else {
                // Set encoder config
                res = aom_codec_enc_config_set(&encoder, &enc_cfg);
                if (res != AOM_CODEC_OK) {
                    const char *error_detail = aom_codec_error(&encoder);
                    (void)error_detail;
                }
                
                // Create and add metadata to image
                if (size > 10) {
                    aom_metadata_t *metadata = aom_img_metadata_alloc(
                        1, data, std::min(size, (size_t)100), 
                        AOM_MIF_ANY_FRAME);
                    if (metadata) {
                        // Add metadata to image
                        int meta_res = aom_img_add_metadata(img, metadata->type,
                                                           metadata->payload,
                                                           metadata->sz,
                                                           metadata->insert_flag);
                        (void)meta_res;
                        
                        // Free metadata
                        aom_img_metadata_free(metadata);
                    }
                }
                
                // Prepare buffer for encoded data
                std::vector<uint8_t> cx_buffer(4096);
                aom_fixed_buf_t cx_buf;
                cx_buf.buf = cx_buffer.data();
                cx_buf.sz = cx_buffer.size();
                
                // Set buffer for encoded data
                res = aom_codec_set_cx_data_buf(&encoder, &cx_buf, 0, 0);
                if (res != AOM_CODEC_OK) {
                    const char *error_detail = aom_codec_error(&encoder);
                    (void)error_detail;
                }
                
                // Encode the frame
                res = aom_codec_encode(&encoder, img, 0, 1, 0);
                if (res != AOM_CODEC_OK) {
                    const char *error_detail = aom_codec_error(&encoder);
                    (void)error_detail;
                }
                
                // Get encoded data
                aom_codec_iter_t enc_iter = nullptr;
                const aom_codec_cx_pkt_t *pkt;
                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                        // Write to output file
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                // Clean up encoder
                aom_codec_destroy(&encoder);
            }
        }
        
        // Free the image
        aom_img_free(img);
    }
    
    // Clean up decoder
    aom_codec_destroy(&decoder);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}