// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    if (data && size > 0) {
        aom_codec_decode(ctx, (const uint8_t *)data, size, NULL);
    }

    return ctx;
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_decoder(Data, Size);
    if (!ctx) return 0;

    int ret;
    int frame_flags;
    int last_ref_updates;
    int altref_present;
    int fwd_kf_present;
    aom_tile_info tile_info;
    int last_ref_used;

    // AOMD_GET_FRAME_FLAGS
    ret = aom_codec_control(ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);
    (void)ret;

    // AOMD_GET_LAST_REF_UPDATES
    ret = aom_codec_control(ctx, AOMD_GET_LAST_REF_UPDATES, &last_ref_updates);
    (void)ret;

    // AOMD_GET_ALTREF_PRESENT
    ret = aom_codec_control(ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);
    (void)ret;

    // AOMD_GET_FWD_KF_PRESENT
    ret = aom_codec_control(ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);
    (void)ret;

    // AOMD_GET_TILE_INFO
    ret = aom_codec_control(ctx, AOMD_GET_TILE_INFO, &tile_info);
    (void)ret;

    // AOMD_GET_LAST_REF_USED
    ret = aom_codec_control(ctx, AOMD_GET_LAST_REF_USED, &last_ref_used);
    (void)ret;

    cleanup_decoder(ctx);
    return 0;
}