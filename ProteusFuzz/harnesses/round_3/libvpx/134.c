// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_decoder.h>

/* 
 * Fuzz driver for libvpx vpx_codec_control_ function.
 * 
 * This driver initializes a decoder context, feeds it with fuzzed data to ensure
 * internal buffers are populated, and then invokes vpx_codec_control_ with various
 * control IDs derived from the input data.
 * 
 * Fix: Added checks to ensure the codec context is valid and initialized before
 * calling vpx_codec_control_. Also added a check to ensure the decoder has
 * processed at least one frame successfully before invoking control IDs that
 * require internal frame buffers.
 */

static vpx_codec_ctx_t g_codec_ctx;
static int g_initialized = 0;
static int g_frame_decoded = 0;
static vpx_codec_iface_t *g_iface = NULL;

static void cleanup_codec(void) {
    if (g_initialized) {
        vpx_codec_destroy(&g_codec_ctx);
        g_initialized = 0;
        g_frame_decoded = 0;
    }
}

static int init_decoder(void) {
    if (g_initialized) {
        cleanup_codec();
    }

    g_iface = vpx_codec_vp9_dx();
    if (!g_iface) {
        return 0;
    }

    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;

    if (vpx_codec_dec_init(&g_codec_ctx, g_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    g_initialized = 1;
    return 1;
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Ensure we have a valid decoder context */
    if (!g_initialized) {
        if (!init_decoder()) {
            return 0;
        }
    }

    /* 
     * Step 1: Feed some data to the decoder to ensure internal frame buffers 
     * are allocated and populated. This is required before calling certain 
     * control IDs like VP9D_SET_REFERENCE.
     */
    if (Size > 0) {
        /* vpx_codec_decode expects raw pointer, size, user_priv, and deadline */
        vpx_codec_err_t decode_res = vpx_codec_decode(&g_codec_ctx, Data, (unsigned int)Size, NULL, 0);
        if (decode_res == VPX_CODEC_OK) {
            g_frame_decoded = 1;
        }
    }

    /* 
     * Step 2: Prepare control ID and arguments from fuzz input.
     * We map the first byte to a control ID range.
     * We use subsequent bytes for arguments if needed.
     */
    int ctrl_id = Data[0];
    
    vpx_codec_err_t res;

    /* 
     * Only invoke control IDs that require frame data if we have successfully
     * decoded at least one frame. Otherwise, skip to avoid SEGV.
     */
    if (g_frame_decoded) {
        /* Case A: No extra arguments */
        res = vpx_codec_control_(&g_codec_ctx, ctrl_id);
        (void)res;

        /* Case B: One extra argument (int) */
        if (Size > 1) {
            int arg1 = (int)Data[1];
            res = vpx_codec_control_(&g_codec_ctx, ctrl_id, arg1);
            (void)res;
        }

        /* Case C: Two extra arguments (int, int) */
        if (Size > 2) {
            int arg1 = (int)Data[1];
            int arg2 = (int)Data[2];
            res = vpx_codec_control_(&g_codec_ctx, ctrl_id, arg1, arg2);
            (void)res;
        }

        /* Case D: Three extra arguments (int, int, int) */
        if (Size > 3) {
            int arg1 = (int)Data[1];
            int arg2 = (int)Data[2];
            int arg3 = (int)Data[3];
            res = vpx_codec_control_(&g_codec_ctx, ctrl_id, arg1, arg2, arg3);
            (void)res;
        }

        /* Case E: Four extra arguments (int, int, int, int) */
        if (Size > 4) {
            int arg1 = (int)Data[1];
            int arg2 = (int)Data[2];
            int arg3 = (int)Data[3];
            int arg4 = (int)Data[4];
            res = vpx_codec_control_(&g_codec_ctx, ctrl_id, arg1, arg2, arg3, arg4);
            (void)res;
        }
    } else {
        /* 
         * If no frame has been decoded, we can still try safe control IDs
         * that do not require frame data. For simplicity, we skip all
         * control IDs in this case to avoid potential crashes.
         */
    }

    return 0;
}