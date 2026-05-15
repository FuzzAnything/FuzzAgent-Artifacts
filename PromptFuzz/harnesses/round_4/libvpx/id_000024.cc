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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"vp8_fix_contexts":[[460,7,460,48,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
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

    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Determine codec type from first byte (simplistic detection)
    vpx_codec_iface_t *decoder_iface = nullptr;
    if (data[0] & 0x01) {
        decoder_iface = vpx_codec_vp9_dx();
    } else {
        decoder_iface = vpx_codec_vp8_dx();
    }

    // Get codec interface name
    const char *iface_name = vpx_codec_iface_name(decoder_iface);

    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, 
                               VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read and decode data from file
    std::vector<uint8_t> buffer(4096);
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_img = nullptr;
    
    while (!feof(in_file)) {
        size_t bytes_read = fread(buffer.data(), 1, buffer.size(), in_file);
        if (bytes_read > 0) {
            // Decode the chunk
            if (vpx_codec_decode(&decoder, buffer.data(), bytes_read, 
                                nullptr, fuzz_int64_t_2) != VPX_CODEC_OK) {
                break;
            }

            // Get decoded frames
            while ((decoded_img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
                // We have a decoded frame, now we'll try to re-encode it
                
                // Get stream info from decoder
                vpx_codec_stream_info_t si;
                memset(&si, 0, sizeof(si));
                si.sz = sizeof(si);
                
                if (vpx_codec_get_stream_info(&decoder, &si) == VPX_CODEC_OK) {
                    // Initialize encoder for the same codec
                    vpx_codec_iface_t *encoder_iface = nullptr;
                    if (data[0] & 0x01) {
                        encoder_iface = vpx_codec_vp9_cx();
                    } else {
                        encoder_iface = vpx_codec_vp8_cx();
                    }
                    
                    vpx_codec_ctx_t encoder;
                    vpx_codec_enc_cfg_t enc_cfg;
                    
                    // Get default encoder configuration
                    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3) == VPX_CODEC_OK) {
                        // Set encoder configuration based on decoded frame
                        enc_cfg.g_w = decoded_img->d_w;
                        enc_cfg.g_h = decoded_img->d_h;
                        enc_cfg.g_timebase.num = 1;
                        enc_cfg.g_timebase.den = 30;
                        enc_cfg.rc_target_bitrate = 1000;
                        enc_cfg.g_pass = VPX_RC_ONE_PASS;
                        enc_cfg.g_lag_in_frames = 0;
                        
                        // Initialize encoder
                        if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_4,
                                                   VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                            
                            // Try to set encoder configuration again (to test vpx_codec_enc_config_set)
                            vpx_codec_enc_config_set(&encoder, &enc_cfg);
                            
                            // Encode the decoded frame
                            if (vpx_codec_encode(&encoder, decoded_img, fuzz_int64_t_5, fuzz_uint64_t_6, fuzz_int64_t_7, 
                                                fuzz_uint64_t_8) == VPX_CODEC_OK) {
                                // Get encoded data
                                const vpx_codec_cx_pkt_t *pkt;
                                vpx_codec_iter_t enc_iter = nullptr;
                                
                                while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                                    // Process encoded packet if needed
                                }
                            }
                            
                            // Clean up encoder
                            vpx_codec_destroy(&encoder);
                        }
                    }
                }
            }
            iter = nullptr; // Reset iterator for next decode
        }
    }

    // Clean up
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}