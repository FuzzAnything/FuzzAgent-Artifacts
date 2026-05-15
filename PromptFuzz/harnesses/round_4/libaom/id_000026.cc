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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"aom_codec_get_global_headers":[[287,14,287,57,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_decode","aom_codec_get_global_headers","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_global_headers","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer callbacks for decoder
static int  id_000026_get_frame_buffer(void* cb_priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
    (void)cb_priv;
    fb->data = (uint8_t*)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    return 0;
}

static int  id_000026_release_frame_buffer(void* cb_priv, aom_codec_frame_buffer_t* fb) {
    (void)cb_priv;
    free(fb->data);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<44) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful test
    
    // Create a FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used
    (void)out_fd;  // Mark as used
    
    // Event: Decode an AV1 bitstream from input data, then re-encode it
    // This tests round-trip decoding/encoding with custom frame buffers
    
    aom_codec_ctx_t decoder;
    aom_codec_ctx_t encoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    aom_codec_enc_cfg_t enc_cfg;
    
    // Initialize decoder with AV1 interface
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    const char* decoder_name = aom_codec_iface_name(decoder_iface);
    (void)decoder_name;  // Mark as used
    
    dec_cfg.threads = 1;
    dec_cfg.w = 128;  // Default width
    dec_cfg.h = 128;  // Default height
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                                 fuzz_int64_t_1, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set custom frame buffer functions
    res = aom_codec_set_frame_buffer_functions(&decoder,  id_000026_get_frame_buffer, 
                                                id_000026_release_frame_buffer, NULL);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Decode the input data
    res = aom_codec_decode(&decoder, data, size, NULL);
    
    // Get global headers from decoder (if any)
    aom_fixed_buf_t* global_headers = aom_codec_get_global_headers(&decoder);
    (void)global_headers;  // We may not always have global headers
    
    // Try to get decoded frames
    aom_codec_iter_t iter = NULL;
    const aom_image_t* decoded_frame = aom_codec_get_frame(&decoder, &iter);
    
    // If we got a decoded frame, try to encode it
    if (decoded_frame) {
        // Initialize encoder with AV1 interface
        aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
        const char* encoder_name = aom_codec_iface_name(encoder_iface);
        (void)encoder_name;  // Mark as used
        
        // Get default encoder configuration
        res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_2);
        if (res != AOM_CODEC_OK) {
            aom_codec_destroy(&decoder);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
        
        // Set encoder configuration based on decoded frame
        enc_cfg.g_w = decoded_frame->d_w;
        enc_cfg.g_h = decoded_frame->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.g_threads = 1;
        enc_cfg.g_profile = 0;
        enc_cfg.rc_min_quantizer = 0;
        enc_cfg.rc_max_quantizer = 63;
        enc_cfg.rc_end_usage = AOM_Q;
        
        // Initialize encoder
        res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                     fuzz_int64_t_3, AOM_ENCODER_ABI_VERSION);
        if (res != AOM_CODEC_OK) {
            aom_codec_destroy(&decoder);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
        
        // Encode the decoded frame
        aom_codec_encode(&encoder, decoded_frame, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6);
        
        // Try to get preview frame from encoder
        const aom_image_t* preview_frame = aom_codec_get_preview_frame(&encoder);
        (void)preview_frame;  // Mark as used
        
        // Get encoded data
        aom_codec_iter_t enc_iter = NULL;
        const aom_codec_cx_pkt_t* pkt = aom_codec_get_cx_data(&encoder, &enc_iter);
        
        // Write encoded data to output file if available
        if (pkt && pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
        
        // Destroy encoder
        aom_codec_destroy(&encoder);
    }
    
    // Try to get more frames (loop through all decoded frames)
    while ((decoded_frame = aom_codec_get_frame(&decoder, &iter)) != NULL) {
        // Process additional frames if needed
        (void)decoded_frame;
    }
    
    // Cleanup
    aom_codec_destroy(&decoder);
    
    // Free global headers if they were allocated
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    // Close files
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