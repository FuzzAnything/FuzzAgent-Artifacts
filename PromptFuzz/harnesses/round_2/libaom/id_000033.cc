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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_get_global_headers","aom_codec_destroy","aom_codec_decode","aom_codec_get_global_headers","aom_codec_get_preview_frame","aom_codec_get_frame","aom_codec_get_cx_data","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_global_headers","aom_codec_get_preview_frame","aom_codec_get_frame","aom_codec_get_cx_data","aom_codec_destroy"],"visited":0}
/*Looking at the APIs provided and the instructions, I'll create a fuzz driver that processes libaom's output data (encoded AV1 stream) by:

1. Initializing a decoder
2. Parsing some metadata using LEB128 decoding
3. Decoding the video frames
4. Getting global headers and preview frames
5. Cleaning up all resources

Here's the implementation:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Parse LEB128 value from the beginning of the input
    uint64_t lebbuf_value = 0;
    size_t lebbuf_length = 0;
    aom_uleb_decode(data, size, &lebbuf_value, &lebbuf_length);
    
    // Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t cfg = {0};
    cfg.allow_lowbitdepth = 1;
    
    // Get decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Get the codec interface name
    const char *iface_name = aom_codec_iface_name(decoder_iface);
    
    // Initialize the decoder
    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &decoder, 
        decoder_iface, 
        &cfg, 
        0, 
        AOM_DECODER_ABI_VERSION
    );
    
    if (init_result != AOM_CODEC_OK) {
        // Try to get global headers anyway (should fail with error context)
        aom_codec_get_global_headers(&decoder);
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Decode the input data
    aom_codec_decode(&decoder, data, size, nullptr);
    
    // Get global headers (though this is more for encoder)
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&decoder);
    
    // Get preview frame (though this is more for encoder)
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
    
    // Get decoded frames
    aom_codec_iter_t iter = nullptr;
    const aom_image_t *frame = nullptr;
    
    while ((frame = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Just access the frame to ensure it's valid
        if (frame->w > 0 && frame->h > 0) {
            // Access some frame properties
            int width = frame->d_w;
            int height = frame->d_h;
            
            // Access plane information
            for (int plane = 0; plane < 3; plane++) {
                if (frame->stride[plane] > 0) {
                    // Just touch the data to ensure it's accessible
                    if (frame->planes[plane] != nullptr) {
                        volatile uint8_t dummy = frame->planes[plane][0];
                        (void)dummy;
                    }
                }
            }
        }
    }
    
    // Reset iterator for getting encoded data packets
    iter = nullptr;
    
    // Try to get encoded data (though this is more for encoder context)
    const aom_codec_cx_pkt_t *pkt = nullptr;
    while ((pkt = aom_codec_get_cx_data(&decoder, &iter)) != nullptr) {
        // Process different packet types
        switch (pkt->kind) {
            case AOM_CODEC_CX_FRAME_PKT:
                if (pkt->data.frame.buf != nullptr) {
                    volatile uint8_t dummy = ((uint8_t*)pkt->data.frame.buf)[0];
                    (void)dummy;
                }
                break;
            case AOM_CODEC_STATS_PKT:
                if (pkt->data.twopass_stats.buf != nullptr) {
                    volatile uint8_t dummy = ((uint8_t*)pkt->data.twopass_stats.buf)[0];
                    (void)dummy;
                }
                break;
            default:
                break;
        }
    }
    
    // Clean up
    aom_codec_destroy(&decoder);
    
    return 0;
}