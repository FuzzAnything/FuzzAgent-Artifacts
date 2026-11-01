// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    // Helper to read an integer from fuzz data
    int32_t val;
    size_t offset = 0;

    // 1. AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    if (offset + sizeof(int32_t) <= size) {
        memcpy(&val, data + offset, sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, val);
    }

    // 2. AV1E_SET_ENABLE_DNL_DENOISING
    if (offset + sizeof(int32_t) <= size) {
        memcpy(&val, data + offset, sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, val);
    }

    // 3. AV1E_GET_SEQ_LEVEL_IDX (Output ignored for fuzzing, just trigger path)
    if (offset + sizeof(int32_t) <= size) {
        memcpy(&val, data + offset, sizeof(int32_t));
        offset += sizeof(int32_t);
        int32_t out_val = 0;
        aom_codec_control(ctx, AV1E_GET_SEQ_LEVEL_IDX, &out_val);
    }

    // 4. AV1E_SET_RENDER_SIZE
    if (offset + sizeof(int32_t) * 2 <= size) {
        int32_t width, height;
        memcpy(&width, data + offset, sizeof(int32_t));
        memcpy(&height, data + offset + sizeof(int32_t), sizeof(int32_t));
        offset += sizeof(int32_t) * 2;
        
        // Ensure non-negative and reasonable bounds to avoid immediate crashes in allocation
        if (width < 0) width = 0;
        if (height < 0) height = 0;
        if (width > 16384) width = 16384;
        if (height > 16384) height = 16384;

        aom_codec_control(ctx, AV1E_SET_RENDER_SIZE, &width, &height);
    }

    // 5. AV1E_SET_ENABLE_ORDER_HINT
    if (offset + sizeof(int32_t) <= size) {
        memcpy(&val, data + offset, sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AV1E_SET_ENABLE_ORDER_HINT, val);
    }

    // 6. AV1E_SET_REDUCED_TX_TYPE_SET
    if (offset + sizeof(int32_t) <= size) {
        memcpy(&val, data + offset, sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_222(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(NULL, &cfg, 0);

    // Use a small default width/height to avoid excessive memory usage during init
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    // Initialize the codec context
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Perform fuzzing on control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}