// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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

static int read_int(const uint8_t *d, size_t *off, size_t max) {
    if (*off + sizeof(int32_t) > max) return 0;
    int32_t val = *(const int32_t *)(d + *off);
    *off += sizeof(int32_t);
    return val;
}

static int read_uint(const uint8_t *d, size_t *off, size_t max) {
    if (*off + sizeof(uint32_t) > max) return 0;
    uint32_t val = *(const uint32_t *)(d + *off);
    *off += sizeof(uint32_t);
    return val;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL) return 0;

    size_t off = 0;
    int ret = 0;

    // 1. AV1E_SET_QM_V (Quantization Matrices)
    // Expected: int32_t *qm_v (array of 32 values)
    if (off + 32 * sizeof(int32_t) <= size) {
        int32_t qm_v[32];
        memcpy(qm_v, data + off, 32 * sizeof(int32_t));
        off += 32 * sizeof(int32_t);
        ret = aom_codec_control(ctx, AV1E_SET_QM_V, qm_v);
        if (ret != AOM_CODEC_OK) ret = 0; // Ignore errors for fuzzing
    }

    // 2. AV1E_SET_INTER_DCT_ONLY
    // Expected: int32_t (0 or 1)
    if (off + sizeof(int32_t) <= size) {
        int32_t val = read_int(data, &off, size);
        ret = aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val);
        if (ret != AOM_CODEC_OK) ret = 0;
    }

    // 3. AV1E_SET_SUPERBLOCK_SIZE
    // Expected: int32_t (superblock size)
    if (off + sizeof(int32_t) <= size) {
        int32_t val = read_int(data, &off, size);
        ret = aom_codec_control(ctx, AV1E_SET_SUPERBLOCK_SIZE, val);
        if (ret != AOM_CODEC_OK) ret = 0;
    }

    // 4. AV1E_SET_ENABLE_INTRABC
    // Expected: int32_t (0 or 1)
    if (off + sizeof(int32_t) <= size) {
        int32_t val = read_int(data, &off, size);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);
        if (ret != AOM_CODEC_OK) ret = 0;
    }

    // 5. AV1E_SET_CHROMA_SUBSAMPLING_Y
    // Expected: int32_t (chroma subsampling)
    if (off + sizeof(int32_t) <= size) {
        int32_t val = read_int(data, &off, size);
        ret = aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, val);
        if (ret != AOM_CODEC_OK) ret = 0;
    }

    // 6. AV1E_SET_ENABLE_CDEF
    // Expected: int32_t (0 or 1)
    if (off + sizeof(int32_t) <= size) {
        int32_t val = read_int(data, &off, size);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, val);
        if (ret != AOM_CODEC_OK) ret = 0;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_348(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize encoder with minimal valid config
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Run fuzz logic
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}