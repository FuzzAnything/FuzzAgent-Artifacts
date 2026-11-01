// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_frame_buffer.h"

/* Helper function to initialize an encoder context */
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    if (size < 4) return NULL;

    /* Extract width and height from fuzz input to vary test states */
    unsigned int w = (data[0] % 256) + 16;
    unsigned int h = (data[1] % 256) + 16;
    unsigned int bit_depth = (data[2] % 8) + 8; /* 8, 10, or 12 */
    int profile = data[3] % 4;

    /* Align dimensions to multiples of 16 for AV1 */
    w = (w + 15) & ~15;
    h = (h + 15) & ~15;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_bit_depth = bit_depth;
    cfg.g_profile = profile;
    cfg.rc_target_bitrate = 1000;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper function to initialize a decoder context */
static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t res;

    if (size < 2) return NULL;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = (data[0] % 8) + 1;
    cfg.allow_lowbitdepth = (data[1] % 2) ? 1 : 0;

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_188(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Use first byte to decide between encoder and decoder context */
    int use_encoder = (Data[0] % 2) != 0;
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_err_t res;

    if (use_encoder) {
        ctx = init_encoder(Data, Size);
    } else {
        ctx = init_decoder(Data, Size);
    }

    if (!ctx) return 0;

    /* Prepare data for reference frame operations */
    av1_ref_frame_t ref_frame;
    memset(&ref_frame, 0, sizeof(ref_frame));

    /* Use fuzz data to populate reference frame struct */
    if (Size > 1) {
        ref_frame.idx = Data[1] % 8; /* AV1 supports up to 8 reference frames */
    }
    if (Size > 2) {
        ref_frame.use_external_ref = Data[2] % 2;
    }

    /* Initialize image for reference frame output */
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    img.w = 16;
    img.h = 16;
    img.bit_depth = 8;
    img.planes[0] = malloc(16 * 16);
    img.planes[1] = malloc(16 * 16);
    img.planes[2] = malloc(16 * 16);
    if (!img.planes[0] || !img.planes[1] || !img.planes[2]) {
        /* Cleanup on allocation failure */
        free(img.planes[0]);
        free(img.planes[1]);
        free(img.planes[2]);
        if (use_encoder) aom_codec_destroy(ctx);
        else aom_codec_destroy(ctx);
        free(ctx);
        return 0;
    }
    img.stride[0] = 16;
    img.stride[1] = 16;
    img.stride[2] = 16;
    img.fmt = AOM_IMG_FMT_I420;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    ref_frame.img = img;

    /* Fuzz AV1E_SET_S_FRAME_MODE (Encoder only) */
    if (use_encoder && Size > 3) {
        int s_frame_mode = Data[3] % 2;
        res = aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, &s_frame_mode);
        (void)res; /* Ignore return for fuzzing, just trigger execution */
    }

    /* Fuzz AV1_SET_REFERENCE (Decoder only) - Requires decoded frames to exist */
    /* We skip this if no frames have been decoded, as it will crash on empty state */
    if (!use_encoder) {
        /* Only attempt if we have some data to potentially decode first, 
           but for pure control fuzzing without decode loop, these often fail safely 
           or require specific state. To avoid the crash on worker->data1 being null,
           we must ensure the decoder has processed at least one frame or is in a valid state.
           Since we are not feeding a full bitstream here, we skip reference frame controls
           that require active frame buffers unless we decode something first.
           For safety in this specific fuzz driver context without a decode loop:
           We skip AV1_SET_REFERENCE, AV1_COPY_REFERENCE, AV1_GET_REFERENCE 
           as they require a valid reference frame list which is empty on init.
        */
    }

    /* Fuzz AV1E_SET_MAX_REFERENCE_FRAMES (Encoder only) */
    if (use_encoder && Size > 6) {
        int max_refs = Data[6] % 8;
        res = aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, &max_refs);
        (void)res;
    }

    /* Fuzz AV1E_SET_ENABLE_REF_FRAME_MVS (Encoder only) */
    if (use_encoder && Size > 7) {
        int enable_mvs = Data[7] % 2;
        res = aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, &enable_mvs);
        (void)res;
    }

    /* Cleanup */
    free(img.planes[0]);
    free(img.planes[1]);
    free(img.planes[2]);

    if (use_encoder) {
        aom_codec_destroy(ctx);
    } else {
        aom_codec_destroy(ctx);
    }
    free(ctx);

    return 0;
}