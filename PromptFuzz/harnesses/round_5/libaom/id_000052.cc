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
//<ID> 107
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 5
//<Quality> {"density":18,"unique_branches":{"get_bsize":[[357,7,357,20,0,0,4,0],[357,24,357,38,0,0,4,0],[357,24,357,38,0,0,4,1],[359,14,359,27,0,0,4,0],[361,14,361,28,0,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_err_to_string","aom_codec_enc_init_ver","aom_codec_err_to_string","aom_codec_set_option","aom_img_alloc","aom_codec_destroy","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_err_to_string","aom_codec_get_cx_data","aom_img_wrap","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_destroy","aom_img_remove_metadata","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_err_to_string","aom_codec_get_cx_data","aom_img_wrap","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_destroy","aom_img_remove_metadata","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callbacks for decoder
int  id_000052_get_frame_buffer_cb(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)min_size;
    static uint8_t buffer[1024 * 1024];  // Static buffer for simplicity
    fb->data = buffer;
    fb->size = sizeof(buffer);
    return 0;
}

int  id_000052_release_frame_buffer_cb(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // Open input as FILE* using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors (not used directly but demonstrating the API)
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;
    (void)out_fd;
    
    // 1. ENCODER SETUP
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    aom_codec_enc_cfg_t enc_cfg;
    unsigned int usage = 0;  // Good quality
    aom_codec_err_t err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, usage);
    if (err != AOM_CODEC_OK) {
        const char *error_str = aom_codec_err_to_string(err);
        (void)error_str;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Configure encoder settings from fuzz data
    enc_cfg.g_w = (data[0] % 64 + 64);  // 64-127 width
    enc_cfg.g_h = (data[1] % 64 + 64);  // 64-127 height
    enc_cfg.g_threads = 1;
    enc_cfg.rc_target_bitrate = 1000;
    
    aom_codec_ctx_t encoder_ctx;
    err = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        const char *error_str = aom_codec_err_to_string(err);
        (void)error_str;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set encoder option
    aom_codec_set_option(&encoder_ctx, "cpu-used", "1");
    
    // 2. CREATE INPUT IMAGE
    aom_image_t input_img;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    unsigned int align = 32;
    
    aom_image_t *img_ptr = aom_img_alloc(&input_img, fmt, enc_cfg.g_w, enc_cfg.g_h, align);
    if (!img_ptr) {
        aom_codec_destroy(&encoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Fill image planes with fuzz data
    size_t data_offset = 2;  // Skip width/height bytes
    for (int plane = 0; plane < 3; plane++) {
        int width = aom_img_plane_width(img_ptr, plane);
        int height = aom_img_plane_height(img_ptr, plane);
        uint8_t *plane_ptr = img_ptr->planes[plane];
        
        for (int y = 0; y < height; y++) {
            size_t bytes_to_copy = (size - data_offset) < (size_t)width ? 
                                   (size - data_offset) : (size_t)width;
            if (bytes_to_copy > 0) {
                fread(plane_ptr, 1, bytes_to_copy, in_file);
                data_offset += bytes_to_copy;
            }
            plane_ptr += img_ptr->stride[plane];
        }
    }
    
    // 3. ADD METADATA TO IMAGE
    uint32_t metadata_type = 1;  // Arbitrary type
    size_t metadata_size = size > data_offset ? size - data_offset : 0;
    if (metadata_size > 0) {
        uint8_t *metadata_data = (uint8_t *)data + data_offset;
        aom_img_add_metadata(img_ptr, metadata_type, metadata_data, 
                           metadata_size < 100 ? metadata_size : 100, 
                           AOM_MIF_ANY_FRAME);
    }
    
    // 4. SET OUTPUT BUFFER FOR ENCODED DATA
    aom_fixed_buf_t output_buf;
    std::vector<uint8_t> encoded_buffer(1024 * 1024);
    output_buf.buf = encoded_buffer.data();
    output_buf.sz = encoded_buffer.size();
    aom_codec_set_cx_data_buf(&encoder_ctx, &output_buf, 0, 0);
    
    // 5. ENCODE THE IMAGE
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    err = aom_codec_encode(&encoder_ctx, img_ptr, pts, duration, flags);
    if (err != AOM_CODEC_OK) {
        const char *error_str = aom_codec_err_to_string(err);
        (void)error_str;
    }
    
    // 6. GET ENCODED DATA
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    std::vector<uint8_t> compressed_data;
    
    while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            compressed_data.insert(compressed_data.end(),
                                  (uint8_t*)pkt->data.frame.buf,
                                  (uint8_t*)pkt->data.frame.buf + pkt->data.frame.sz);
        }
    }
    
    // 7. CREATE ANOTHER IMAGE USING WRAP (different API)
    aom_image_t wrapped_img;
    unsigned char dummy_data[100];
    aom_img_wrap(&wrapped_img, AOM_IMG_FMT_I420, 16, 16, 1, dummy_data);
    
    // 8. DECODER SETUP
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    err = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 0, 
                                 AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        const char *error_str = aom_codec_err_to_string(err);
        (void)error_str;
    } else {
        // 9. SET FRAME BUFFER FUNCTIONS
        void *cb_priv = NULL;
        aom_codec_set_frame_buffer_functions(&decoder_ctx, 
                                            id_000052_get_frame_buffer_cb,
                                            id_000052_release_frame_buffer_cb,
                                           cb_priv);
        
        // 10. DECODE IF WE HAVE COMPRESSED DATA
        if (!compressed_data.empty()) {
            err = aom_codec_decode(&decoder_ctx, compressed_data.data(), 
                                 compressed_data.size(), NULL);
            
            // 11. GET STREAM INFO
            aom_codec_stream_info_t si;
            memset(&si, 0, sizeof(si));
            aom_codec_get_stream_info(&decoder_ctx, &si);
            
            // Get decoded frames
            iter = NULL;
            aom_image_t *decoded_img;
            while ((decoded_img = aom_codec_get_frame(&decoder_ctx, &iter)) != NULL) {
                // Can process decoded image here
                (void)decoded_img;
            }
        }
        
        // 12. DESTROY DECODER
        aom_codec_destroy(&decoder_ctx);
    }
    
    // 13. REMOVE METADATA AND FREE IMAGES
    aom_img_remove_metadata(img_ptr);
    aom_img_free(img_ptr);
    // wrapped_img doesn't need freeing since it doesn't own its data
    
    // 14. DESTROY ENCODER
    aom_codec_destroy(&encoder_ctx);
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}