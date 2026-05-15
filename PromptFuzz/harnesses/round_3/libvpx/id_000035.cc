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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to achieve a video decoding/encoding pipeline with callbacks and custom buffer management:

*/


extern "C" {
}

// Custom callback structures
typedef struct {
    FILE* out_file;
    int frame_count;
} UserPrivData;

typedef struct {
    uint8_t* buffer;
    size_t size;
} FrameBufferData;

// Callback functions
static void  id_000035_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    UserPrivData* priv = static_cast<UserPrivData*>(user_priv);
    if (priv && priv->out_file && img) {
        priv->frame_count++;
        // Write some metadata about the frame
        fwrite(&img->d_w, sizeof(img->d_w), 1, priv->out_file);
        fwrite(&img->d_h, sizeof(img->d_h), 1, priv->out_file);
        fwrite(&img->fmt, sizeof(img->fmt), 1, priv->out_file);
    }
}

static int  id_000035_get_frame_buffer_callback(void* cb_priv, size_t min_size,
                                     vpx_codec_frame_buffer_t* fb) {
    FrameBufferData* fb_data = static_cast<FrameBufferData*>(cb_priv);
    if (!fb_data || !fb) return -1;
    
    if (!fb_data->buffer || fb_data->size < min_size) {
        free(fb_data->buffer);
        fb_data->buffer = static_cast<uint8_t*>(malloc(min_size));
        fb_data->size = min_size;
        if (!fb_data->buffer) return -1;
    }
    
    fb->data = fb_data->buffer;
    fb->size = fb_data->size;
    return 0;
}

static int  id_000035_release_frame_buffer_callback(void* cb_priv,
                                         vpx_codec_frame_buffer_t* fb) {
    // We'll reuse the buffer, so just return success
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<68) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Initialize file pointers
    FILE* in_file = fmemopen((void*)data, size, "rb");
    FILE* out_file = fopen("output_file", "wb");
    if (!in_file || !out_file) {
        if (in_file) assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize variables
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_dec_cfg_t dec_cfg = {0};
    vpx_codec_enc_cfg_t enc_cfg = {0};
    UserPrivData user_priv = {out_file, 0};
    FrameBufferData fb_data = {nullptr, 0};
    vpx_fixed_buf_t output_buf = {nullptr, 0};
    
    // Try both VP8 and VP9 decoders
    vpx_codec_iface_t* codec_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    int success = 0;
    
    for (int i = 0; i < 2 && !success; i++) {
        vpx_codec_iface_t* iface = codec_ifaces[i];
        
        // 1. Get interface name
        const char* iface_name = vpx_codec_iface_name(iface);
        if (iface_name) {
            fwrite(iface_name, strlen(iface_name), 1, out_file);
        }
        
        // 2. Get codec capabilities
        vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
        fwrite(&caps, sizeof(caps), 1, out_file);
        
        // Configure decoder
        dec_cfg.threads = 1;
        dec_cfg.w = 320;
        dec_cfg.h = 240;
        
        // 3. Initialize decoder
        vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, iface, &dec_cfg, fuzz_int64_t_1,
                                                    VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) continue;
        
        // 4. Register put frame callback
        err = vpx_codec_register_put_frame_cb(&decoder,  id_000035_put_frame_callback, &user_priv);
        
        // 5. Set frame buffer functions
        err = vpx_codec_set_frame_buffer_functions(&decoder,
                                                   id_000035_get_frame_buffer_callback,
                                                   id_000035_release_frame_buffer_callback,
                                                  &fb_data);
        
        // Try to decode the input data
        fseek(in_file, 0, SEEK_SET);
        
        // Read chunks and decode
        uint8_t buffer[4096];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
            err = vpx_codec_decode(&decoder, buffer, bytes_read, nullptr, fuzz_int64_t_2);
            
            // Get decoded frames
            vpx_codec_iter_t iter = nullptr;
            vpx_image_t* img;
            while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
                // Try to initialize encoder for this codec type
                vpx_codec_iface_t* enc_iface = (i == 0) ? 
                    vpx_codec_vp8_cx() : vpx_codec_vp9_cx();
                
                // Get default encoder config
                if (vpx_codec_enc_config_default(enc_iface, &enc_cfg, fuzz_uint32_t_3) == VPX_CODEC_OK) {
                    enc_cfg.g_w = img->d_w;
                    enc_cfg.g_h = img->d_h;
                    enc_cfg.rc_target_bitrate = 1000;
                    
                    // Initialize encoder
                    if (vpx_codec_enc_init_ver(&encoder, enc_iface, &enc_cfg, fuzz_int64_t_4,
                                              VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                        
                        // 6. Set encoder configuration
                        err = vpx_codec_enc_config_set(&encoder, &enc_cfg);
                        
                        // Allocate output buffer
                        output_buf.sz = 1024 * 1024; // 1MB
                        output_buf.buf = malloc(output_buf.sz);
                        
                        if (output_buf.buf) {
                            // 7. Set output buffer for encoder
                            err = vpx_codec_set_cx_data_buf(&encoder, &output_buf, fuzz_uint32_t_5, fuzz_uint32_t_6);
                            
                            // Try to encode the frame
                            err = vpx_codec_encode(&encoder, img, fuzz_int64_t_7, fuzz_uint64_t_8, fuzz_int64_t_9, fuzz_uint64_t_10);
                            
                            // Get encoded data
                            vpx_codec_iter_t enc_iter = nullptr;
                            const vpx_codec_cx_pkt_t* pkt;
                            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                                }
                            }
                            
                            free(output_buf.buf);
                        }
                        
                        // 8. Get error message (even if no error)
                        const char* error_msg = vpx_codec_error(&encoder);
                        if (error_msg) {
                            fwrite(error_msg, strlen(error_msg), 1, out_file);
                        }
                        
                        vpx_codec_destroy(&encoder);
                    }
                }
            }
        }
        
        success = 1; // Mark as successful for this codec
        
        // Clean up decoder for this iteration
        vpx_codec_destroy(&decoder);
    }
    
    // Clean up resources
    free(fb_data.buffer);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}