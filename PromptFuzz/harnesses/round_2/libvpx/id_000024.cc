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
#include <vpx/vp8cx.h>
}
extern "C" {
#include <vpx/vp8dx.h>
}
extern "C" {
#include <vpx/vpx_decoder.h>
}
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_destroy"],"visited":0}
/**/


// Include libvpx headers
extern "C" {
}

// Callback function for put_slice
static void  id_000024_put_slice_callback(void *user_priv, const vpx_image_t *img,
                              const vpx_image_rect_t *valid,
                              const vpx_image_rect_t *update) {
    // This callback can be used to process slices during encoding
    // For fuzzing, we just need to register it
    (void)user_priv;
    (void)img;
    (void)valid;
    (void)update;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 4) {
        return 0;
    }

    // Initialize variables
    vpx_codec_ctx_t codec_ctx = {0};
    vpx_codec_enc_cfg_t cfg = {0};
    vpx_codec_iter_t iter = NULL;
    vpx_codec_stream_info_t stream_info = {0};
    stream_info.sz = sizeof(stream_info);
    
    // Use fmemopen to create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Try both VP8 and VP9 codecs
    vpx_codec_iface_t *codec_ifaces[] = {
        vpx_codec_vp8_cx(),
        vpx_codec_vp9_cx()
    };
    
    // Try both encoder and decoder interfaces
    vpx_codec_iface_t *decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    // Process with each codec interface
    for (int codec_idx = 0; codec_idx < 2; codec_idx++) {
        // 1. Use vpx_codec_iface_name
        const char *iface_name = vpx_codec_iface_name(codec_ifaces[codec_idx]);
        (void)iface_name;  // Use the name to avoid compiler warnings
        
        // 2. Use vpx_codec_get_caps
        vpx_codec_caps_t caps = vpx_codec_get_caps(codec_ifaces[codec_idx]);
        (void)caps;
        
        // Reset file pointer
        fseek(in_file, 0, SEEK_SET);
        
        // 3. Initialize encoder configuration
        vpx_codec_err_t err = vpx_codec_enc_config_default(codec_ifaces[codec_idx], &cfg, 0);
        if (err != VPX_CODEC_OK) {
            continue;
        }
        
        // Set some basic configuration based on input data
        cfg.g_w = 256;
        cfg.g_h = 256;
        cfg.rc_target_bitrate = 256;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        
        // 4. Use vpx_codec_enc_init_ver - Initialize encoder
        err = vpx_codec_enc_init_ver(&codec_ctx, codec_ifaces[codec_idx], &cfg, 0, 
                                     VPX_ENCODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            continue;
        }
        
        // 5. Use vpx_codec_register_put_slice_cb - Register callback
        err = vpx_codec_register_put_slice_cb(&codec_ctx,  id_000024_put_slice_callback, NULL);
        (void)err;  // Don't fail if callback registration fails
        
        // Create a dummy image for encoding
        vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
        if (!img) {
            vpx_codec_destroy(&codec_ctx);
            continue;
        }
        
        // Fill image with some data from input
        size_t bytes_to_fill = img->w * img->h;
        if (bytes_to_fill > 0) {
            size_t data_offset = 0;
            for (int plane = 0; plane < 3; plane++) {
                unsigned char *plane_ptr = img->planes[plane];
                int stride = img->stride[plane];
                int plane_height = (plane == 0) ? img->h : img->h / 2;
                int plane_width = (plane == 0) ? img->w : img->w / 2;
                
                for (int y = 0; y < plane_height; y++) {
                    size_t bytes_to_copy = plane_width;
                    if (data_offset + bytes_to_copy > size) {
                        bytes_to_copy = size - data_offset;
                        if (bytes_to_copy == 0) {
                            data_offset = 0;  // Wrap around
                        }
                    }
                    
                    if (bytes_to_copy > 0) {
                        memcpy(plane_ptr + y * stride, data + data_offset, bytes_to_copy);
                        data_offset = (data_offset + bytes_to_copy) % size;
                    }
                }
            }
        }
        
        // Encode the image
        err = vpx_codec_encode(&codec_ctx, img, 0, 1, 0, VPX_DL_REALTIME);
        
        // Get compressed data
        const vpx_codec_cx_pkt_t *pkt = NULL;
        while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Now try to decode this compressed data
                vpx_codec_ctx_t decoder_ctx = {0};
                vpx_codec_iface_t *decoder_iface = decoder_ifaces[codec_idx];
                
                // Initialize decoder
                if (vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, NULL, 0, 
                                          VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
                    
                    // 6. Use vpx_codec_get_stream_info on the original input data
                    vpx_codec_peek_stream_info(decoder_iface, data, 
                                              (size < 1000) ? size : 1000, &stream_info);
                    
                    // Decode the compressed frame
                    vpx_codec_decode(&decoder_ctx, (const uint8_t*)pkt->data.frame.buf,
                                    pkt->data.frame.sz, NULL, 0);
                    
                    // 7. Use vpx_codec_get_frame
                    vpx_codec_iter_t frame_iter = NULL;
                    const vpx_image_t *decoded_img;
                    while ((decoded_img = vpx_codec_get_frame(&decoder_ctx, &frame_iter)) != NULL) {
                        // Process the decoded frame if needed
                        (void)decoded_img;
                    }
                    
                    vpx_codec_destroy(&decoder_ctx);
                }
            }
        }
        
        // Clean up
        vpx_img_free(img);
        vpx_codec_destroy(&codec_ctx);
    }
    
    // Try to get stream info from the original input data using decoder interfaces
    for (int codec_idx = 0; codec_idx < 2; codec_idx++) {
        // Reset file pointer
        fseek(in_file, 0, SEEK_SET);
        
        // Read a chunk of data to analyze
        uint8_t header[1000];
        size_t read_size = (size < sizeof(header)) ? size : sizeof(header);
        fread(header, 1, read_size, in_file);
        
        // Use vpx_codec_peek_stream_info
        vpx_codec_peek_stream_info(decoder_ifaces[codec_idx], header, read_size, &stream_info);
        
        // Initialize decoder and try to decode
        vpx_codec_ctx_t decoder_ctx = {0};
        if (vpx_codec_dec_init_ver(&decoder_ctx, decoder_ifaces[codec_idx], NULL, 0,
                                  VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
            
            // Reset file pointer
            fseek(in_file, 0, SEEK_SET);
            
            // Read all data
            uint8_t *file_data = (uint8_t*)malloc(size);
            if (file_data) {
                fread(file_data, 1, size, in_file);
                
                // Try to decode
                vpx_codec_decode(&decoder_ctx, file_data, size, NULL, 0);
                
                // Get stream info from decoder context
                vpx_codec_get_stream_info(&decoder_ctx, &stream_info);
                
                // Get decoded frames
                vpx_codec_iter_t frame_iter = NULL;
                const vpx_image_t *decoded_img;
                while ((decoded_img = vpx_codec_get_frame(&decoder_ctx, &frame_iter)) != NULL) {
                    // Process decoded frame
                    (void)decoded_img;
                }
                
                free(file_data);
            }
            
            vpx_codec_destroy(&decoder_ctx);
        }
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}