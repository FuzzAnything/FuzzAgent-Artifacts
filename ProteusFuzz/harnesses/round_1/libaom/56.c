// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely access indices used below
    if (Size < 7) return 0;

    // Setup dummy file if needed
    setup_dummy_file(Data, Size);

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzed data to configure some fields safely
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 100) + 1;
    cfg.rc_target_bitrate = (Data[3] % 10000) + 1000;

    // Initialize the codec context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzzed control values
    int enable_val = (Data[4] % 2);
    int width = (Data[5] % 1024) + 16;
    int height = (Data[6] % 1024) + 16;

    // Invoke target API functions with fuzzed inputs
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, enable_val);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, enable_val);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, enable_val);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_MASKED_COMP, enable_val);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, enable_val);

    // AV1E_SET_RENDER_SIZE requires an array of 2 integers, not two separate arguments
    int render_size[2];
    render_size[0] = width;
    render_size[1] = height;
    aom_codec_control(&ctx, AV1E_SET_RENDER_SIZE, render_size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}