// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, const aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;

    // Initialize default configuration
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzer data to configure some parameters safely
    if (Size > 0) {
        cfg.g_w = (Data[0] % 1024) + 16;
        cfg.g_h = (Data[1 % Size] % 1024) + 16;
        cfg.g_timebase.num = (Data[2 % Size] % 100) + 1;
        cfg.g_timebase.den = (Data[3 % Size] % 100) + 1;
        cfg.rc_target_bitrate = (Data[4 % Size] * 256) + 100;
        cfg.g_threads = (Data[5 % Size] % 8) + 1;
        cfg.g_error_resilient = (Data[6 % Size] % 2);
    }

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // AV1E_SET_QM_U
    if (Size > 10) {
        int qm_u_val = Data[7 % Size];
        aom_codec_control(&ctx, AV1E_SET_QM_U, &qm_u_val);
    }

    // AV1E_SET_MAX_PARTITION_SIZE
    if (Size > 11) {
        int max_part_size = Data[8 % Size];
        aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, &max_part_size);
    }

    // AV1E_SET_ENABLE_CDEF
    if (Size > 12) {
        int enable_cdef = Data[9 % Size] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, &enable_cdef);
    }

    // AV1E_SET_ENABLE_KEYFRAME_FILTERING
    if (Size > 13) {
        int enable_keyframe_filtering = Data[10 % Size] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, &enable_keyframe_filtering);
    }

    // AV1E_SET_LOOPFILTER_CONTROL
    if (Size > 14) {
        int loopfilter_val = Data[11 % Size];
        aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, &loopfilter_val);
    }

    // AV1E_SET_SKIP_POSTPROC_FILTERING
    if (Size > 15) {
        int skip_postproc = Data[12 % Size] % 2;
        aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, &skip_postproc);
    }

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}