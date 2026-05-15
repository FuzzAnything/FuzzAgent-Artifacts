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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["vpx_codec_version_str","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_peek_stream_info","vpx_codec_destroy","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include libvpx headers (as per instructions, these would be available)
extern "C" {
}

// Callback functions (do nothing but must be defined)
static void  id_000003_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    (void)user_priv;
    (void)img;
}

static void  id_000003_put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* rect, 
                               const vpx_image_rect_t* rect2) {
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Get version string (required API)
    const char* version = vpx_codec_version_str();
    (void)version; // Prevent unused warning

    // 2. Create input FILE* from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 3. Read encoded data from input
    std::vector<uint8_t> encoded_data(size);
    size_t bytes_read = fread(encoded_data.data(), 1, size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read == 0) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 4. Initialize decoder (VP8)
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp8_dx();
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, NULL, fuzz_int64_t_1, 
                               VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Register callbacks (required APIs)
    vpx_codec_register_put_frame_cb(&decoder,  id_000003_put_frame_callback, NULL);
    vpx_codec_register_put_slice_cb(&decoder,  id_000003_put_slice_callback, NULL);

    // 6. Get stream info (required API)
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Use peek first to avoid needing decoder context
    if (vpx_codec_peek_stream_info(decoder_iface, encoded_data.data(), 
                                   encoded_data.size(), &stream_info) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // 7. Decode the input data
    if (vpx_codec_decode(&decoder, encoded_data.data(), encoded_data.size(), 
                         NULL, fuzz_int64_t_2) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // 8. Get decoded frame
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t* decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    
    // 9. Initialize encoder if we have a valid frame
    if (decoded_frame) {
        vpx_codec_ctx_t encoder;
        vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx(); // Required API
        
        // Default encoder configuration
        vpx_codec_enc_cfg_t cfg;
        if (vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_3) != VPX_CODEC_OK) {
            vpx_codec_destroy(&decoder);
            assert_file_closed(&in_file);
	return 0;
        }
        
        // Set dimensions from decoded frame
        cfg.g_w = decoded_frame->d_w;
        cfg.g_h = decoded_frame->d_h;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 256; // 256 kbps
        
        // Initialize encoder (required API)
        if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, fuzz_int64_t_4, 
                                   VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
            vpx_codec_destroy(&decoder);
            assert_file_closed(&in_file);
	return 0;
        }
        
        // Set encoder configuration (required API)
        vpx_codec_enc_config_set(&encoder, &cfg);
        
        // 10. Encode the frame
        if (vpx_codec_encode(&encoder, decoded_frame, fuzz_int64_t_5, fuzz_uint64_t_6, fuzz_int64_t_7, fuzz_uint64_t_8) 
            != VPX_CODEC_OK) {
            vpx_codec_destroy(&encoder);
            vpx_codec_destroy(&decoder);
            assert_file_closed(&in_file);
	return 0;
        }
        
        // 11. Get encoded data (required API)
        vpx_codec_iter_t enc_iter = NULL;
        const vpx_codec_cx_pkt_t* pkt;
        FILE* out_file = fopen("output_file", "wb");
        if (out_file) {
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
            assert_file_closed(&out_file);;
        }
        
        // 12. Cleanup encoder
        vpx_codec_destroy(&encoder);
    }
    
    // 13. Cleanup decoder
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}