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
//<ID> 179
//<Prompt> []
/*<Combination>: [
*/
//<score> 95, nr_unique_branch: 5
//<Quality> {"density":19,"unique_branches":{"rd_pick_4partition":[[3943,18,3943,65,0,0,4,0]],"encode_sb":[[1697,22,1697,55,0,0,4,0]],"full_pixel_exhaustive":[[35,23,35,34,1,0,4,1],[35,44,35,45,1,0,4,0]],"write_modes_sb":[[1703,22,1703,55,0,0,4,0]]},"library_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_get_preview_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_img_metadata_free","aom_img_wrap","aom_img_free","aom_codec_encode","aom_codec_get_cx_data","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error_detail","aom_codec_destroy","aom_img_free","aom_codec_error_detail","aom_codec_destroy"],"critical_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_get_preview_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_img_metadata_free","aom_img_wrap","aom_img_free","aom_codec_encode","aom_codec_get_cx_data","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error_detail","aom_codec_destroy","aom_img_free","aom_codec_error_detail","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callbacks for decoder
static int  id_000064_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

static int  id_000064_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create input FILE* from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output FILE* for encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used
    
    // Get build config
    const char *build_config = aom_codec_build_config();
    (void)build_config;
    
    // DECODER SETUP
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_caps_t decoder_caps = aom_codec_get_caps(decoder_iface);
    (void)decoder_caps;
    
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set frame buffer functions
    res = aom_codec_set_frame_buffer_functions(&decoder,  id_000064_get_frame_buffer,  id_000064_release_frame_buffer, nullptr);
    (void)res;
    
    // DECODE INPUT
    // Read data in chunks to simulate streaming
    std::vector<uint8_t> buffer(4096);
    size_t bytes_read;
    while ((bytes_read = fread(buffer.data(), 1, buffer.size(), in_file)) > 0) {
        aom_codec_decode(&decoder, buffer.data(), bytes_read, nullptr);
    }
    
    // Get stream info
    aom_codec_stream_info_t si = {0};
    aom_codec_get_stream_info(&decoder, &si);
    
    // Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_frame = nullptr;
    std::vector<aom_image_t*> frames;
    
    while ((decoded_frame = aom_codec_get_frame(&decoder, &iter))) {
        // Try to get preview frame (may return null)
        const aom_image_t *preview = aom_codec_get_preview_frame(&decoder);
        (void)preview;
        
        // Make a copy of the frame for later use
        aom_image_t *frame_copy = aom_img_alloc(nullptr, decoded_frame->fmt, 
                                               decoded_frame->d_w, decoded_frame->d_h, 16);
        if (frame_copy) {
            // Copy plane data
            for (int plane = 0; plane < 3; plane++) {
                if (decoded_frame->planes[plane]) {
                    int plane_width = aom_img_plane_width(frame_copy, plane);
                    int plane_height = aom_img_plane_height(frame_copy, plane);
                    size_t copy_size = plane_width * plane_height;
                    if (plane_width > 0 && plane_height > 0) {
                        memcpy(frame_copy->planes[plane], decoded_frame->planes[plane], copy_size);
                    }
                }
            }
            frames.push_back(frame_copy);
        }
    }
    
    // ENCODER SETUP
    if (!frames.empty()) {
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        aom_codec_ctx_t encoder;
        aom_codec_enc_cfg_t enc_cfg;
        
        // Get default encoder config
        res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        if (res == AOM_CODEC_OK) {
            // Configure encoder
            enc_cfg.g_w = frames[0]->d_w;
            enc_cfg.g_h = frames[0]->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.g_threads = 1;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_pass = AOM_RC_ONE_PASS;
            enc_cfg.g_lag_in_frames = 0;
            
            res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
            if (res == AOM_CODEC_OK) {
                // Set encoder options
                aom_codec_set_option(&encoder, "cpu-used", "2");
                aom_codec_set_option(&encoder, "end-usage", "vbr");
                
                // Process each frame
                for (size_t i = 0; i < frames.size(); i++) {
                    aom_image_t *frame = frames[i];
                    
                    // Test metadata operations
                    uint32_t metadata_type = 1;
                    uint8_t metadata_data[] = {0x01, 0x02, 0x03};
                    size_t metadata_size = sizeof(metadata_data);
                    
                    // Allocate metadata
                    aom_metadata_t *metadata = aom_img_metadata_alloc(
                        metadata_type, metadata_data, metadata_size, AOM_MIF_ANY_FRAME);
                    
                    if (metadata) {
                        // Add metadata to image
                        aom_img_add_metadata(frame, metadata_type, metadata_data, 
                                           metadata_size, AOM_MIF_ANY_FRAME);
                        
                        // Get number of metadata
                        size_t num_metadata = aom_img_num_metadata(frame);
                        
                        // Get metadata
                        if (num_metadata > 0) {
                            const aom_metadata_t *retrieved_metadata = 
                                aom_img_get_metadata(frame, 0);
                            (void)retrieved_metadata;
                        }
                        
                        // Remove metadata
                        aom_img_remove_metadata(frame);
                        
                        // Free metadata
                        aom_img_metadata_free(metadata);
                    }
                    
                    // Test image wrapping
                    size_t img_size = frame->stride[0] * frame->d_h * 3 / 2;
                    std::vector<uint8_t> img_data(img_size);
                    aom_image_t *wrapped_img = aom_img_wrap(nullptr, frame->fmt, 
                                                          frame->d_w, frame->d_h, 
                                                          16, img_data.data());
                    if (wrapped_img) {
                        aom_img_free(wrapped_img);
                    }
                    
                    // Encode frame
                    aom_codec_encode(&encoder, frame, i, 1, 0);
                    
                    // Get encoded data
                    const aom_codec_cx_pkt_t *pkt;
                    aom_codec_iter_t enc_iter = nullptr;
                    
                    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter))) {
                        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                        }
                    }
                }
                
                // Flush encoder
                aom_codec_encode(&encoder, nullptr, 0, 1, 0);
                
                // Get any remaining packets
                const aom_codec_cx_pkt_t *pkt;
                aom_codec_iter_t enc_iter = nullptr;
                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter))) {
                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                // Get error detail
                const char *error_detail = aom_codec_error_detail(&encoder);
                (void)error_detail;
                
                // Destroy encoder
                aom_codec_destroy(&encoder);
            }
        }
        
        // Clean up frames
        for (auto frame : frames) {
            aom_img_free(frame);
        }
    }
    
    // Get decoder error detail
    const char *dec_error_detail = aom_codec_error_detail(&decoder);
    (void)dec_error_detail;
    
    // Clean up
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}