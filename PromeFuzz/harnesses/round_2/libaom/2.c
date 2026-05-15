// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_decoder.h"

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Get the AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    // Initialize decoder configuration
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.w = 640;
    cfg.h = 480;
    
    // Initialize decoder context
    aom_codec_ctx_t ctx;
    aom_codec_err_t err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Try to decode the input data
    aom_codec_decode(&ctx, Data, Size, NULL);

    // Try to get stream information
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_get_stream_info(&ctx, &si);

    // Try some control operations with values derived from input
    if (Size > 0) {
        // Use first byte to select control ID
        int ctrl_id = Data[0] % 10;
        
        // Try different control operations based on input
        switch (ctrl_id) {
            case 0:
                aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, NULL);
                break;
            case 1:
                aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, NULL);
                break;
            case 2:
                aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, NULL);
                break;
            default:
                // Try with a simple integer parameter
                int param = (int)Data[0];
                aom_codec_control(&ctx, ctrl_id, &param);
                break;
        }
    }

    // Clean up
    aom_codec_destroy(&ctx);

    return 0;
}