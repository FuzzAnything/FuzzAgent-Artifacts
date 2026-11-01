// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "aom.h"
#include "aom_decoder.h"
#include "aom_encoder.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_integer.h"
#include "aom_frame_buffer.h"
#include "aom_image.h"

/* Helper to safely write to a dummy file if needed */
static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

/* Helper to initialize a decoder context */
static int init_decoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;

    if (!ctx) return 0;

    memset(ctx, 0, sizeof(*ctx));
    memset(&cfg, 0, sizeof(cfg));

    /* Use the first few bytes to determine width/height if possible, else default */
    if (size >= 4) {
        cfg.w = data[0] * 16;
        cfg.h = data[1] * 16;
        if (cfg.w == 0) cfg.w = 64;
        if (cfg.h == 0) cfg.h = 64;
    } else {
        cfg.w = 64;
        cfg.h = 64;
    }
    cfg.threads = 1;

    iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

/* Helper to initialize an encoder context */
static int init_encoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;

    if (!ctx) return 0;

    memset(ctx, 0, sizeof(*ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (size >= 4) {
        cfg.g_w = data[0] * 16;
        cfg.g_h = data[1] * 16;
        if (cfg.g_w == 0) cfg.g_w = 64;
        if (cfg.g_h == 0) cfg.g_h = 64;
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = AOM_CBR;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

/* Helper to destroy context */
static void destroy_context(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Write to dummy file as per requirement if data is large enough */
    if (Size > 100) {
        write_dummy_file(Data, Size);
    }

    aom_codec_ctx_t ctx_dec;
    aom_codec_ctx_t ctx_enc;
    int initialized_dec = 0;
    int initialized_enc = 0;

    /* Initialize decoder context */
    if (init_decoder_context(&ctx_dec, Data, Size)) {
        initialized_dec = 1;
    }

    /* Initialize encoder context */
    if (init_encoder_context(&ctx_enc, Data, Size)) {
        initialized_enc = 1;
    }

    /* Fuzz aom_codec_control with various IDs on decoder */
    if (initialized_dec) {
        int ctrl_id_dec = Data[0] % 1000;
        if (ctrl_id_dec == 0) ctrl_id_dec = 1; /* Ensure non-zero */

        /* Try generic control without arguments first to check validity */
        aom_codec_control(&ctx_dec, ctrl_id_dec);

        /* Try with different argument types to explore states */
        int arg_int = (Size > 1) ? Data[1] : 0;
        int arg_int2 = (Size > 2) ? Data[2] : 0;
        int arg_int3 = (Size > 3) ? Data[3] : 0;
        
        /* Only pass arguments if the control ID is known to accept them or generic */
        /* We rely on the library to handle invalid args gracefully, but avoid passing 
           pointers to uninitialized memory or invalid structures */
        aom_codec_control(&ctx_dec, ctrl_id_dec, arg_int);
        aom_codec_control(&ctx_dec, ctrl_id_dec, arg_int, arg_int2);
        aom_codec_control(&ctx_dec, ctrl_id_dec, arg_int, arg_int2, arg_int3);

        /* Try AOMD_GET_TILE_INFO specifically if we can guess the ID or just use generic */
        aom_tile_info tile_info;
        memset(&tile_info, 0, sizeof(tile_info));
        /* Pass pointer to valid stack memory */
        aom_codec_control(&ctx_dec, ctrl_id_dec, &tile_info);
    }

    /* Fuzz aom_codec_control with various IDs on encoder */
    if (initialized_enc) {
        int ctrl_id_enc = Data[0] % 1000;
        if (ctrl_id_enc == 0) ctrl_id_enc = 1;

        int arg_int = (Size > 1) ? Data[1] : 0;
        int arg_int2 = (Size > 2) ? Data[2] : 0;
        int arg_int3 = (Size > 3) ? Data[3] : 0;

        /* Start with no arguments */
        aom_codec_control(&ctx_enc, ctrl_id_enc);

        /* Try with integer arguments */
        aom_codec_control(&ctx_enc, ctrl_id_enc, arg_int);
        aom_codec_control(&ctx_enc, ctrl_id_enc, arg_int, arg_int2);
        aom_codec_control(&ctx_enc, ctrl_id_enc, arg_int, arg_int2, arg_int3);
        
        /* Avoid passing random pointers to encoder controls as they often expect 
           specific structures like aom_enc_cfg_t or aom_rc_cfg_t which are not 
           initialized here. The crash in the prompt was likely due to an 
           uninitialized pointer or wrong type being passed to a control expecting 
           a struct. */
    }

    /* Cleanup */
    if (initialized_dec) {
        destroy_context(&ctx_dec);
    }
    if (initialized_enc) {
        destroy_context(&ctx_enc);
    }

    return 0;
}