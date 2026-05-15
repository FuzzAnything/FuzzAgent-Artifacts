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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 5
//<Quality> {"density":11,"unique_branches":{"vpx_codec_get_cx_data":[[257,14,257,57,0,0,4,0]],"ctrl_get_last_ref_updates":[[530,7,530,18,0,0,4,0],[531,9,531,25,0,0,4,1]],"vp8_get_last_ref_updates":[[647,7,647,18,0,0,4,0],[649,9,649,20,0,0,4,0]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_control_","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_control_","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for frame output
static void  id_000024_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This callback receives decoded frames
    // For fuzzing, we just verify the image is valid
    if (img && user_priv) {
        // Can add frame validation here if needed
    }
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create a file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output file (will be discarded)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Try both VP8 and VP9 codecs
    vpx_codec_iface_t *decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    const int num_codecs = 2;
    vpx_codec_ctx_t codec_ctx = {0};
    vpx_codec_iter_t iter = nullptr;
    
    bool success = false;
    
    for (int codec_idx = 0; codec_idx < num_codecs && !success; ++codec_idx) {
        vpx_codec_iface_t *iface = decoder_ifaces[codec_idx];
        
        // Get codec capabilities
        vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
        (void)caps; // Use caps to avoid unused variable warning
        
        // Initialize decoder with default config
        vpx_codec_dec_cfg_t cfg = {0};
        cfg.threads = 1;
        
        // Try to initialize decoder
        if (vpx_codec_dec_init_ver(&codec_ctx, iface, &cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
            continue;
        }
        
        // Register frame callback
        void *user_priv = nullptr;
        if (vpx_codec_register_put_frame_cb(&codec_ctx,  id_000024_put_frame_callback, user_priv) != VPX_CODEC_OK) {
            // Continue even if callback registration fails
        }
        
        // Try to get stream info from the input data
        vpx_codec_stream_info_t si = {0};
        si.sz = sizeof(si);
        
        // Peek at stream info
        if (vpx_codec_peek_stream_info(iface, data, size, &si) == VPX_CODEC_OK) {
            // Use control API - try to get error resilience mode
            int ctrl_id = VP8D_GET_LAST_REF_UPDATES;
            int value = 0;
            if (vpx_codec_control_(&codec_ctx, ctrl_id, &value) != VPX_CODEC_OK) {
                // Control might fail, that's OK
            }
        }
        
        // Decode the input data
        if (vpx_codec_decode(&codec_ctx, data, size, nullptr, fuzz_int64_t_2) == VPX_CODEC_OK) {
            // Try to get stream info after decoding
            vpx_codec_stream_info_t si_post = {0};
            si_post.sz = sizeof(si_post);
            if (vpx_codec_get_stream_info(&codec_ctx, &si_post) == VPX_CODEC_OK) {
                // Successfully got stream info
            }
            
            // Get decoded frames
            const vpx_image_t *img;
            while ((img = vpx_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
                // Frame successfully decoded
                // Try to get preview frame (might return null)
                const vpx_image_t *preview = vpx_codec_get_preview_frame(&codec_ctx);
                (void)preview; // Use preview to avoid unused variable warning
                
                // Write frame info to output file
                if (img->d_w > 0 && img->d_h > 0) {
                    fwrite(&img->d_w, sizeof(img->d_w), 1, out_file);
                    fwrite(&img->d_h, sizeof(img->d_h), 1, out_file);
                }
            }
            
            // Try to get encoded data (though we're decoding)
            // This simulates getting data from encoder context
            vpx_codec_iter_t cx_iter = nullptr;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &cx_iter)) != nullptr) {
                // Process packet if available
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Write packet size to output
                    fwrite(&pkt->data.frame.sz, sizeof(pkt->data.frame.sz), 1, out_file);
                }
            }
            
            success = true;
        }
        
        // Clean up decoder for this iteration
        vpx_codec_destroy(&codec_ctx);
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}