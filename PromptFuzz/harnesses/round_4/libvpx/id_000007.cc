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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 0
//<Quality> {"density":28,"unique_branches":{},"library_calls":["vpx_codec_version","vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_set_frame_buffer_functions","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_version","vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_get_global_headers","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include necessary libvpx headers
extern "C" {
}

// Frame buffer callback structures
struct FrameBufferData {
    uint8_t* data;
    size_t size;
    int in_use;
};

static int  id_000007_get_frame_buffer(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    FrameBufferData* buffer_data = static_cast<FrameBufferData*>(cb_priv);
    
    if (buffer_data->in_use) {
        return -1;
    }
    
    if (buffer_data->size < min_size) {
        free(buffer_data->data);
        buffer_data->data = static_cast<uint8_t*>(malloc(min_size));
        if (!buffer_data->data) {
            return -1;
        }
        buffer_data->size = min_size;
    }
    
    fb->data = buffer_data->data;
    fb->size = buffer_data->size;
    buffer_data->in_use = 1;
    
    return 0;
}

static int  id_000007_release_frame_buffer(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    FrameBufferData* buffer_data = static_cast<FrameBufferData*>(cb_priv);
    buffer_data->in_use = 0;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
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

    // 1. Get version information
    int version = vpx_codec_version();
    const char* extra_str = vpx_codec_version_extra_str();
    (void)version; // Mark as used
    (void)extra_str; // Mark as used

    // Create input file from fuzzer data
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

    // Initialize decoder for VP9
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, NULL, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Read and decode the input data
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (file_size <= 0) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    uint8_t* encoded_data = static_cast<uint8_t*>(malloc(file_size));
    if (!encoded_data) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    fread(encoded_data, 1, file_size, in_file);
    
    // Decode the data
    if (vpx_codec_decode(&decoder, encoded_data, file_size, NULL, fuzz_int64_t_2) != VPX_CODEC_OK) {
        free(encoded_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get decoded frame
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t* decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    
    if (!decoded_frame) {
        free(encoded_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_3) != VPX_CODEC_OK) {
        free(encoded_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set encoder configuration
    cfg.g_w = decoded_frame->d_w;
    cfg.g_h = decoded_frame->d_h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        free(encoded_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. Set frame buffer functions
    FrameBufferData frame_buffer_data = {nullptr, 0, 0};
    if (vpx_codec_set_frame_buffer_functions(&encoder,  id_000007_get_frame_buffer, 
                                              id_000007_release_frame_buffer, &frame_buffer_data) != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        free(encoded_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Set encoder configuration (again to demonstrate the API)
    if (vpx_codec_enc_config_set(&encoder, &cfg) != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        free(encoded_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 4. Use control API
    if (vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8) != VPX_CODEC_OK) {
        // Continue anyway
    }

    // 5. Get global headers
    vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }

    // 6. Set output buffer for encoded data
    vpx_fixed_buf_t output_buf;
    size_t output_buffer_size = 1024 * 1024; // 1MB
    output_buf.buf = malloc(output_buffer_size);
    output_buf.sz = output_buffer_size;
    
    if (output_buf.buf) {
        if (vpx_codec_set_cx_data_buf(&encoder, &output_buf, fuzz_uint32_t_5, fuzz_uint32_t_6) != VPX_CODEC_OK) {
            // Continue anyway
        }
    }

    // Encode the frame
    if (vpx_codec_encode(&encoder, decoded_frame, fuzz_int64_t_7, fuzz_uint64_t_8, fuzz_int64_t_9, fuzz_uint64_t_10) != VPX_CODEC_OK) {
        if (output_buf.buf) free(output_buf.buf);
        vpx_codec_destroy(&encoder);
        free(encoded_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get encoded data
    vpx_codec_iter_t enc_iter = NULL;
    const vpx_codec_cx_pkt_t* pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }

    // Clean up
    if (output_buf.buf) free(output_buf.buf);
    if (frame_buffer_data.data) free(frame_buffer_data.data);
    vpx_codec_destroy(&encoder);
    free(encoded_data);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}