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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_vp8_cx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_peek_stream_info","vpx_codec_vp8_cx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback structures
struct FrameBufferData {
    std::vector<uint8_t> buffer;
    size_t size;
};

static int  id_000060_get_frame_buffer(void *cb_priv, size_t min_size, 
                            vpx_codec_frame_buffer_t *fb) {
    FrameBufferData *fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    // Ensure buffer is large enough
    if (fb_data->buffer.size() < min_size) {
        fb_data->buffer.resize(min_size);
        fb_data->size = min_size;
    }
    
    fb->data = fb_data->buffer.data();
    fb->size = fb_data->buffer.size();
    return 0;
}

static int  id_000060_release_frame_buffer(void *cb_priv, 
                                vpx_codec_frame_buffer_t *fb) {
    // Nothing to do - we manage the buffer in FrameBufferData
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1024) {
        return 0; // Too small for meaningful processing
    }
    
    // Try to open input as a file stream using fmemopen
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Try to determine codec type by peeking stream info
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Get both decoder interfaces
    vpx_codec_iface_t *vp8_decoder = vpx_codec_vp8_dx();
    vpx_codec_iface_t *vp9_decoder = vpx_codec_vp9_dx();
    
    // Get interface names
    const char *vp8_name = vpx_codec_iface_name(vp8_decoder);
    const char *vp9_name = vpx_codec_iface_name(vp9_decoder);
    
    // Try VP9 first
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        vp9_decoder, data, (unsigned int)(size > 4096 ? 4096 : size), &stream_info);
    
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;
    
    if (peek_result == VPX_CODEC_OK && stream_info.w > 0 && stream_info.h > 0) {
        decoder_iface = vp9_decoder;
        encoder_iface = vpx_codec_vp9_cx();
    } else {
        // Try VP8
        memset(&stream_info, 0, sizeof(stream_info));
        stream_info.sz = sizeof(stream_info);
        peek_result = vpx_codec_peek_stream_info(
            vp8_decoder, data, (unsigned int)(size > 4096 ? 4096 : size), &stream_info);
        
        if (peek_result == VPX_CODEC_OK && stream_info.w > 0 && stream_info.h > 0) {
            decoder_iface = vp8_decoder;
            encoder_iface = vpx_codec_vp8_cx();
        }
    }
    
    if (!decoder_iface || !encoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 2: Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    vpx_codec_err_t init_result = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (init_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 3: Set frame buffer functions
    FrameBufferData fb_data;
    vpx_codec_set_frame_buffer_functions(
        &decoder,  id_000060_get_frame_buffer,  id_000060_release_frame_buffer, &fb_data);
    
    // Step 4: Decode the input data
    vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, 0);
    
    // Step 5: Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    
    if (!decoded_frame) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 6: Initialize encoder with default configuration
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    // Use dimensions from decoded frame
    enc_cfg.g_w = decoded_frame->d_w;
    enc_cfg.g_h = decoded_frame->d_h;
    enc_cfg.g_threads = 1;
    
    // Initialize encoder
    vpx_rational_t dsf = {1, 1}; // No downsampling
    init_result = vpx_codec_enc_init_multi_ver(
        &encoder, encoder_iface, &enc_cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    
    if (init_result != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 7: Allocate image for encoder input
    vpx_image_t *input_image = vpx_img_alloc(
        nullptr, VPX_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 16);
    
    if (!input_image) {
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 8: Copy decoded frame data to input image
    // For simplicity, we'll just use black frames if format conversion is needed
    memset(input_image->img_data, 128, input_image->bps * input_image->d_h / 3 * 2);
    memset(input_image->img_data + input_image->bps * input_image->d_h / 3, 
           64, input_image->bps * input_image->d_h / 3);
    
    // Step 9: Encode the frame
    vpx_codec_encode(&encoder, input_image, 0, 1, 0, VPX_DL_REALTIME);
    
    // Step 10: Get encoded data
    const vpx_codec_cx_pkt_t *pkt;
    vpx_codec_iter_t enc_iter = nullptr;
    
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Write to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Step 11: Cleanup
    vpx_img_free(input_image);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}