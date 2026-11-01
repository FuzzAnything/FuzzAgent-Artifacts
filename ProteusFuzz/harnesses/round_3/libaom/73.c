// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    if (size < 2) return NULL;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to set some configuration parameters safely
    cfg.g_w = (data[0] % 1024) + 16;
    cfg.g_h = (data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // Define control values based on input data, ensuring bounds checking
    int enable_qm = (Size > 6) ? (Data[6] % 2) : 0;
    int intra_dct_only = (Size > 7) ? (Data[7] % 2) : 0;
    int denoise_block_size = (Size > 8) ? ((Data[8] % 8) + 1) : 1;
    int force_video_mode = (Size > 9) ? (Data[9] % 2) : 0;
    int enable_cdef = (Size > 10) ? (Data[10] % 2) : 0;
    int enable_keyframe_filtering = (Size > 11) ? (Data[11] % 2) : 0;

    // Invoke target API functions with typechecked variants
    aom_codec_control(ctx, AV1E_SET_ENABLE_QM, enable_qm);
    aom_codec_control(ctx, AV1E_SET_INTRA_DCT_ONLY, intra_dct_only);
    aom_codec_control(ctx, AV1E_SET_DENOISE_BLOCK_SIZE, denoise_block_size);
    aom_codec_control(ctx, AV1E_SET_FORCE_VIDEO_MODE, force_video_mode);
    aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, enable_cdef);
    aom_codec_control(ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, enable_keyframe_filtering);

    // Cleanup
    cleanup_encoder(ctx);

    return 0;
}