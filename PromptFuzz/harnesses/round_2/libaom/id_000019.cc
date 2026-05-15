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
//<ID> 34
//<Prompt> []
/*<Combination>: [
*/
//<score> 80, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"av1_lookahead_peek":[[183,7,183,18,0,0,4,0]],"aom_codec_enc_config_set":[[302,15,302,26,0,0,4,0]],"realloc_frame_buffer_aligned":[[108,11,108,59,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_config_set","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_codec_get_frame","aom_codec_encode","aom_codec_get_cx_data","aom_codec_encode","aom_codec_get_cx_data","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_config_set","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_get_frame","aom_codec_encode","aom_codec_get_cx_data","aom_codec_encode","aom_codec_get_cx_data","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to achieve decoding and re-encoding of AV1 video data:

*/


extern "C" {
}

// Custom frame buffer structure
struct FrameBufferData {
    std::vector<uint8_t> buffer;
    size_t size;
    bool in_use;
};

// Callback for getting frame buffer
int  id_000019_get_frame_buffer(void* cb_priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
    FrameBufferData* frame_buffers = static_cast<FrameBufferData*>(cb_priv);
    
    // Find an available buffer
    for (size_t i = 0; i < 4; i++) {
        if (!frame_buffers[i].in_use) {
            if (frame_buffers[i].buffer.size() < min_size) {
                frame_buffers[i].buffer.resize(min_size);
            }
            frame_buffers[i].in_use = true;
            fb->data = frame_buffers[i].buffer.data();
            fb->size = frame_buffers[i].buffer.size();
            fb->priv = &frame_buffers[i];
            return 0;
        }
    }
    
    // All buffers in use - allocate new one
    return -1;
}

// Callback for releasing frame buffer
int  id_000019_release_frame_buffer(void* cb_priv, aom_codec_frame_buffer_t* fb) {
    if (fb->priv) {
        FrameBufferData* buffer = static_cast<FrameBufferData*>(fb->priv);
        buffer->in_use = false;
    }
    return 0;
}

// Error checking helper
void  id_000019_check_error(const aom_codec_ctx_t* ctx, const char* operation) {
    const char* detail = aom_codec_error_detail(ctx);
    if (detail) {
        // Log error detail if available
    }
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create file-like stream from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Output file for encoded data
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
    (void)in_fd; (void)out_fd; // Use if needed
    
    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set up custom frame buffers
    FrameBufferData frame_buffers[4];
    for (auto& fb : frame_buffers) {
        fb.buffer.resize(4096);
        fb.size = 4096;
        fb.in_use = false;
    }
    
    // Set frame buffer functions
    aom_codec_set_frame_buffer_functions(&decoder,  id_000019_get_frame_buffer, 
                                          id_000019_release_frame_buffer, frame_buffers);
    
    // Decode input data
    aom_codec_decode(&decoder, data, size, nullptr);
    
    // Check for errors
     id_000019_check_error(&decoder, "decoding");
    
    // Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    
    aom_codec_enc_cfg_t enc_cfg;
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Configure encoder
    enc_cfg.g_w = 640;
    enc_cfg.g_h = 480;
    enc_cfg.g_threads = 1;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    
    // Set encoder configuration
    aom_codec_enc_config_set(&encoder, &enc_cfg);
    
    if (aom_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set up output buffer for encoded data
    std::vector<uint8_t> output_buffer(size * 2); // Allocate larger buffer
    aom_fixed_buf_t cx_data_buf;
    cx_data_buf.buf = output_buffer.data();
    cx_data_buf.sz = output_buffer.size();
    
    // Set output buffer with padding
    unsigned int pad_before = 16;
    unsigned int pad_after = 16;
    aom_codec_set_cx_data_buf(&encoder, &cx_data_buf, pad_before, pad_after);
    
    // Get decoded frames and re-encode them
    aom_codec_iter_t iter = nullptr;
    aom_image_t* img;
    int frame_count = 0;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr && frame_count < 10) {
        // Encode the frame
        aom_codec_encode(&encoder, img, frame_count, 1, 0);
        
        // Get encoded data
        const aom_codec_cx_pkt_t* pkt;
        aom_codec_iter_t enc_iter = nullptr;
        
        while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Write to output file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
        
        frame_count++;
    }
    
    // Flush encoder
    aom_codec_encode(&encoder, nullptr, frame_count, 1, 0);
    
    // Get remaining packets
    const aom_codec_cx_pkt_t* pkt;
    aom_codec_iter_t enc_iter = nullptr;
    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Test ULEB encoding APIs
    uint8_t uleb_buffer[16];
    size_t coded_size = 0;
    
    // Encode a ULEB value from input data
    uint64_t value_to_encode = 0;
    if (size >= 8) {
        memcpy(&value_to_encode, data, 8);
    } else {
        value_to_encode = size;
    }
    
    // Regular ULEB encode
    aom_uleb_encode(value_to_encode, sizeof(uleb_buffer), uleb_buffer, &coded_size);
    
    // Fixed size ULEB encode
    size_t fixed_coded_size = 0;
    aom_uleb_encode_fixed_size(value_to_encode, sizeof(uleb_buffer), 
                               std::min<size_t>(8, sizeof(uleb_buffer)), 
                               uleb_buffer, &fixed_coded_size);
    
    // Check for errors in encoder
     id_000019_check_error(&encoder, "encoding");
    
    // Cleanup
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}