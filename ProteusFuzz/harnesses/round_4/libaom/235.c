// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static unsigned int get_uint(const uint8_t **p, size_t *sz) {
    if (*sz < sizeof(unsigned int)) return 0;
    unsigned int val;
    memcpy(&val, *p, sizeof(unsigned int));
    *p += sizeof(unsigned int);
    *sz -= sizeof(unsigned int);
    return val;
}

static int get_int(const uint8_t **p, size_t *sz) {
    if (*sz < sizeof(int)) return 0;
    int val;
    memcpy(&val, *p, sizeof(int));
    *p += sizeof(int);
    *sz -= sizeof(int);
    return val;
}

static int try_fuzz_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    const uint8_t *ptr = data;
    size_t sz = size;

    // 1. AOME_SET_TUNING
    if (sz >= sizeof(unsigned int)) {
        unsigned int tuning = get_uint(&ptr, &sz);
        aom_codec_control(ctx, AOME_SET_TUNING, tuning);
    }

    // 2. AV1E_SET_DENOISE_BLOCK_SIZE
    if (sz >= sizeof(unsigned int)) {
        unsigned int block_size = get_uint(&ptr, &sz);
        aom_codec_control(ctx, AV1E_SET_DENOISE_BLOCK_SIZE, block_size);
    }

    // 3. AV1E_SET_ENABLE_RESTORATION
    if (sz >= sizeof(unsigned int)) {
        unsigned int enable = get_uint(&ptr, &sz);
        aom_codec_control(ctx, AV1E_SET_ENABLE_RESTORATION, enable);
    }

    // 4. AV1E_SET_QM_U
    if (sz >= sizeof(unsigned int)) {
        unsigned int qm_u = get_uint(&ptr, &sz);
        aom_codec_control(ctx, AV1E_SET_QM_U, qm_u);
    }

    // 5. AV1E_SET_ENABLE_CDEF
    if (sz >= sizeof(unsigned int)) {
        unsigned int enable_cdef = get_uint(&ptr, &sz);
        aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, enable_cdef);
    }

    // 6. AV1E_SET_SKIP_POSTPROC_FILTERING
    if (sz >= sizeof(unsigned int)) {
        unsigned int skip = get_uint(&ptr, &sz);
        aom_codec_control(ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, skip);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_235(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    try_fuzz_encoder(&ctx, Data, Size);

    aom_codec_destroy(&ctx);

    return 0;
}