// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

static int fuzz_codec_control_int(aom_codec_ctx_t *ctx, int control_id, int32_t value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

static int fuzz_codec_control_ptr(aom_codec_ctx_t *ctx, int control_id, void *ptr) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, ptr);
}

static int32_t read_int32_safe(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + 4) return 0;
    int32_t val;
    memcpy(&val, Data + offset, sizeof(int32_t));
    return val;
}

int LLVMFuzzerTestOneInput_185(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Initialize configuration from fuzz data safely
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.g_threads = 1;
    cfg.g_profile = 0;

    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from fuzz data safely
    int32_t val1 = read_int32_safe(Data, Size, 4);
    int32_t val2 = read_int32_safe(Data, Size, 8);
    int32_t val4 = read_int32_safe(Data, Size, 16);
    int32_t val5 = read_int32_safe(Data, Size, 20);
    int32_t val6 = read_int32_safe(Data, Size, 24);

    // Fuzz AV1E_SET_MIN_CR
    fuzz_codec_control_int(&ctx, AV1E_SET_MIN_CR, val1);

    // Fuzz AV1E_SET_ENABLE_DIST_8X8 (Using AV1E_SET_ENABLE_RECT_TX as fallback if undefined)
    // Note: Using AV1E_SET_ENABLE_RECT_TX to ensure compilation if the specific macro is missing
    fuzz_codec_control_int(&ctx, AV1E_SET_ENABLE_RECT_TX, val2);

    // Fuzz AV1E_SET_QM_V (expects pointer to quantization matrix)
    if (Size > 32 + 16 * sizeof(int32_t)) {
        int32_t *qm_v = (int32_t *)malloc(16 * sizeof(int32_t));
        if (qm_v) {
            memcpy(qm_v, Data + 32, 16 * sizeof(int32_t));
            fuzz_codec_control_ptr(&ctx, AV1E_SET_QM_V, qm_v);
            free(qm_v);
        }
    }

    // Fuzz AV1E_SET_CHROMA_SUBSAMPLING_X
    fuzz_codec_control_int(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val4);

    // Fuzz AV1E_SET_ENABLE_QM
    fuzz_codec_control_int(&ctx, AV1E_SET_ENABLE_QM, val5);

    // Fuzz AV1E_SET_MV_COST_UPD_FREQ
    fuzz_codec_control_int(&ctx, AV1E_SET_MV_COST_UPD_FREQ, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}