// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "vpx_codec.h"
#include "vp8cx.h"
#include "vp8dx.h"
#include "vpx_decoder.h"

/* 
 * Fuzz driver for libvpx codec control functions.
 * Targets: vpx_codec_control_ via vpx_codec_control macro.
 */

static vpx_codec_err_t init_encoder(vpx_codec_ctx_t *ctx, const vpx_codec_iface_t *iface) {
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;

    if (iface == NULL) {
        return VPX_CODEC_ERROR;
    }

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return VPX_CODEC_ERROR;
    }

    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_threads = 1;

    err = vpx_codec_enc_init(ctx, iface, &cfg, 0);
    return err;
}

static vpx_codec_err_t init_decoder(vpx_codec_ctx_t *ctx, const vpx_codec_iface_t *iface) {
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_err_t err;

    if (iface == NULL) {
        return VPX_CODEC_ERROR;
    }

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    err = vpx_codec_dec_init(ctx, iface, &cfg, 0);
    return err;
}

static void cleanup_codec(vpx_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        vpx_codec_destroy(ctx);
    }
}

static int try_control(vpx_codec_ctx_t *ctx, int ctrl_id, int value) {
    vpx_codec_err_t res;
    res = vpx_codec_control_(ctx, ctrl_id, value);
    (void)res;
    return 0;
}

static int try_control_no_arg(vpx_codec_ctx_t *ctx, int ctrl_id) {
    vpx_codec_err_t res;
    res = vpx_codec_control_(ctx, ctrl_id);
    (void)res;
    return 0;
}

static int32_t get_int_from_fuzz(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int32_t) > size) {
        return 0;
    }
    int32_t val;
    memcpy(&val, data + *offset, sizeof(int32_t));
    *offset += sizeof(int32_t);
    return val;
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx_enc = {0};
    vpx_codec_ctx_t ctx_dec = {0};
    const vpx_codec_iface_t *enc_iface = NULL;
    const vpx_codec_iface_t *dec_iface = NULL;
    int32_t val;
    size_t offset = 0;
    int ctrl_id;
    int use_encoder = 1;
    int use_decoder = 1;

    if (Size > 0) {
        use_encoder = (Data[0] % 2) == 0;
        use_decoder = (Data[0] % 2) != 0;
        offset = 1;
    }

    if (use_encoder) {
        enc_iface = vpx_codec_vp8_cx();
        if (enc_iface == NULL) {
            enc_iface = vpx_codec_vp9_cx();
        }
    }

    if (use_decoder) {
        dec_iface = vpx_codec_vp8_dx();
        if (dec_iface == NULL) {
            dec_iface = vpx_codec_vp9_dx();
        }
    }

    if (use_encoder && enc_iface != NULL) {
        if (init_encoder(&ctx_enc, enc_iface) != VPX_CODEC_OK) {
            use_encoder = 0;
        }
    }

    if (use_decoder && dec_iface != NULL) {
        if (init_decoder(&ctx_dec, dec_iface) != VPX_CODEC_OK) {
            use_decoder = 0;
        }
    }

    if (!use_encoder && !use_decoder) {
        return 0;
    }

    if (offset < Size) {
        ctrl_id = get_int_from_fuzz(Data, Size, &offset);
    } else {
        ctrl_id = 0;
    }

    if (offset < Size) {
        val = get_int_from_fuzz(Data, Size, &offset);
    } else {
        val = 0;
    }

    if (use_encoder) {
        try_control(&ctx_enc, ctrl_id, val);
        try_control_no_arg(&ctx_enc, ctrl_id);
    }
    if (use_decoder) {
        try_control(&ctx_dec, ctrl_id, val);
        try_control_no_arg(&ctx_dec, ctrl_id);
    }

    #ifdef VP8E_SET_CPUUSED
    if (use_encoder) {
        int cpu_val = val % 9;
        try_control(&ctx_enc, VP8E_SET_CPUUSED, cpu_val);
    }
    #endif

    #ifdef VP9E_SET_TILE_COLUMNS
    if (use_encoder) {
        int tile_cols = (val % 64) + 1;
        try_control(&ctx_enc, VP9E_SET_TILE_COLUMNS, tile_cols);
    }
    #endif

    #ifdef VP9E_SET_TILE_ROWS
    if (use_encoder) {
        int tile_rows = (val % 64) + 1;
        try_control(&ctx_enc, VP9E_SET_TILE_ROWS, tile_rows);
    }
    #endif

    #ifdef VP9E_SET_DISABLE_LOOPFILTER
    if (use_encoder) {
        int disable = val % 2;
        try_control(&ctx_enc, VP9E_SET_DISABLE_LOOPFILTER, disable);
    }
    #endif

    #ifdef VP8D_GET_LAST_REF_UPDATES
    if (use_decoder) {
        try_control(&ctx_dec, VP8D_GET_LAST_REF_UPDATES, val);
        try_control_no_arg(&ctx_dec, VP8D_GET_LAST_REF_UPDATES);
    }
    #endif

    if (use_encoder) {
        cleanup_codec(&ctx_enc);
    }
    if (use_decoder) {
        cleanup_codec(&ctx_dec);
    }

    return 0;
}