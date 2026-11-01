// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t *init_codec_ctx(void) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(vpx_codec_ctx_t));
    ctx->name = "dummy_codec";
    ctx->iface = NULL;
    ctx->err = VPX_CODEC_OK;
    ctx->err_detail = NULL;
    ctx->init_flags = 0;
    ctx->config.raw = NULL;
    ctx->priv = NULL;

    return ctx;
}

static void cleanup_codec_ctx(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    vpx_codec_ctx_t *ctx = init_codec_ctx();
    if (!ctx) {
        return 0;
    }

    int ctrl_id = Data[0];
    vpx_codec_err_t result = VPX_CODEC_ERROR;

    // Since ctx->iface is NULL, direct access to iface members is invalid.
    // We rely on the vpx_codec_control_ function to handle the NULL interface check internally
    // or we simulate the control call logic without dereferencing the opaque iface struct.
    // The actual libvpx implementation checks for NULL iface inside vpx_codec_control_.

    switch (ctrl_id) {
        case VP8E_SET_SCREEN_CONTENT_MODE:
            if (Size >= 2) {
                int value = Data[1];
                result = vpx_codec_control_(ctx, VP8E_SET_SCREEN_CONTENT_MODE, value);
            }
            break;
        case VP8E_SET_SCALEMODE:
            if (Size >= 3) {
                vpx_scaling_mode_t mode;
                mode.h_scaling_mode = (VPX_SCALING_MODE)(Data[1] % 4);
                mode.v_scaling_mode = (VPX_SCALING_MODE)(Data[2] % 4);
                result = vpx_codec_control_(ctx, VP8E_SET_SCALEMODE, &mode);
            }
            break;
        case VP9E_SET_DISABLE_LOOPFILTER:
            if (Size >= 2) {
                int value = Data[1];
                result = vpx_codec_control_(ctx, VP9E_SET_DISABLE_LOOPFILTER, value);
            }
            break;
        case VP9E_SET_NOISE_SENSITIVITY:
            if (Size >= 2) {
                int value = Data[1];
                result = vpx_codec_control_(ctx, VP9E_SET_NOISE_SENSITIVITY, value);
            }
            break;
        case VP9E_SET_FRAME_PARALLEL_DECODING:
            if (Size >= 2) {
                int value = Data[1];
                result = vpx_codec_control_(ctx, VP9E_SET_FRAME_PARALLEL_DECODING, value);
            }
            break;
        default:
            // For unknown control IDs, we just call with no extra args to test error handling
            result = vpx_codec_control_(ctx, ctrl_id);
            break;
    }

    (void)result;

    cleanup_codec_ctx(ctx);
    return 0;
}