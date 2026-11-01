// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;
    aom_codec_stream_info_t si;
    aom_codec_iface_t *iface;

    // Initialize configuration with defaults
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    // Get the AV1 decoder interface
    iface = aom_codec_av1_dx();
    if (iface == NULL) {
        return 0;
    }

    // Initialize the codec context
    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare stream info structure
    memset(&si, 0, sizeof(si));

    // Attempt to decode the input data
    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt = NULL;

    // Decode the input data
    ret = aom_codec_decode(&ctx, Data, Size, &iter);
    
    // Even if decode fails, we can try to get stream info if the context is valid
    ret = aom_codec_get_stream_info(&ctx, &si);
    
    // Iterate through any decoded frames (if any)
    while ((pkt = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Frame successfully decoded
        }
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}