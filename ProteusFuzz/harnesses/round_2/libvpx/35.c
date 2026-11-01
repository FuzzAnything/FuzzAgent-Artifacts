// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_decoder.h"

// Forward declaration of the internal control function
// We use vpx_codec_control_ directly to avoid macro expansion issues with dynamic IDs
vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t *ctx, int ctrl_id, ...);

static int fuzz_encoder_init(vpx_codec_ctx_t *ctx, const vpx_codec_iface_t *iface,
                             const vpx_codec_enc_cfg_t *cfg, unsigned int flags) {
    vpx_codec_err_t err = vpx_codec_enc_init(ctx, iface, cfg, flags);
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    return 1;
}

static int fuzz_decoder_init(vpx_codec_ctx_t *ctx, const vpx_codec_iface_t *iface,
                             const vpx_codec_dec_cfg_t *cfg, unsigned int flags) {
    vpx_codec_err_t err = vpx_codec_dec_init(ctx, iface, cfg, flags);
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    return 1;
}

// Helper to read an integer from the fuzz buffer
static int read_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) return 0;
    int val;
    memcpy(&val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

// Helper to read a size_t from the fuzz buffer
static size_t read_size_t(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(size_t) > size) return 0;
    size_t val;
    memcpy(&val, data + *offset, sizeof(size_t));
    *offset += sizeof(size_t);
    return val;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    size_t offset = 0;
    int mode = read_int(Data, Size, &offset);
    
    // Determine if we are fuzzing encoder or decoder based on mode
    // 0: VP8 Encoder, 1: VP9 Encoder, 2: VP8 Decoder, 3: VP9 Decoder
    int use_encoder = (mode % 2 == 0);
    int use_vp9 = (mode % 2 == 1); // 1 or 3
    
    const vpx_codec_iface_t *iface = NULL;
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (use_encoder) {
        if (use_vp9) {
            iface = vpx_codec_vp9_cx();
        } else {
            iface = vpx_codec_vp8_cx();
        }

        if (!iface) return 0;

        vpx_codec_enc_cfg_t cfg;
        if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) return 0;

        // Fuzz configuration values
        if (offset + sizeof(int) <= Size) {
            int g_w = read_int(Data, Size, &offset);
            if (g_w > 0 && g_w < 4096) cfg.g_w = g_w;
        }
        if (offset + sizeof(int) <= Size) {
            int g_h = read_int(Data, Size, &offset);
            if (g_h > 0 && g_h < 4096) cfg.g_h = g_h;
        }
        if (offset + sizeof(int) <= Size) {
            int g_timebase_den = read_int(Data, Size, &offset);
            if (g_timebase_den > 0) cfg.g_timebase.den = g_timebase_den;
        }
        if (offset + sizeof(int) <= Size) {
            int g_timebase_num = read_int(Data, Size, &offset);
            if (g_timebase_num > 0) cfg.g_timebase.num = g_timebase_num;
        }
        if (offset + sizeof(int) <= Size) {
            int g_error_resilient = read_int(Data, Size, &offset);
            cfg.g_error_resilient = g_error_resilient & 0xFF;
        }

        if (!fuzz_encoder_init(&ctx, iface, &cfg, 0)) return 0;
    } else {
        if (use_vp9) {
            iface = vpx_codec_vp9_dx();
        } else {
            iface = vpx_codec_vp8_dx();
        }

        if (!iface) return 0;

        vpx_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;
        // Removed cfg.allow_highbitdepth as it may not exist in all versions

        if (!fuzz_decoder_init(&ctx, iface, &cfg, 0)) return 0;
    }

    // Fuzz control commands using vpx_codec_control_ directly
    while (offset < Size) {
        int ctrl_id = read_int(Data, Size, &offset);
        
        vpx_codec_err_t res = VPX_CODEC_OK;
        
        // Try to determine argument type from next byte if available
        if (offset < Size) {
            uint8_t arg_type = Data[offset++];
            
            if (arg_type == 0) {
                // No arguments
                res = vpx_codec_control_(&ctx, ctrl_id);
            } else if (arg_type == 1) {
                // Integer argument
                if (offset + sizeof(int) <= Size) {
                    int arg_val = read_int(Data, Size, &offset);
                    res = vpx_codec_control_(&ctx, ctrl_id, arg_val);
                }
            } else if (arg_type == 2) {
                // Pointer argument (using a dummy buffer)
                static uint8_t dummy_buf[64];
                res = vpx_codec_control_(&ctx, ctrl_id, dummy_buf);
            } else if (arg_type == 3) {
                // Two integer arguments
                if (offset + 2 * sizeof(int) <= Size) {
                    int arg1 = read_int(Data, Size, &offset);
                    int arg2 = read_int(Data, Size, &offset);
                    res = vpx_codec_control_(&ctx, ctrl_id, arg1, arg2);
                }
            } else {
                // Default: try with one int
                if (offset + sizeof(int) <= Size) {
                    int arg_val = read_int(Data, Size, &offset);
                    res = vpx_codec_control_(&ctx, ctrl_id, arg_val);
                }
            }
        } else {
            // No arg type specified, try no args
            res = vpx_codec_control_(&ctx, ctrl_id);
        }
        
        // Check for errors, but continue fuzzing
        // Expected errors: VPX_CODEC_ERROR, VPX_CODEC_INVALID_PARAM, VPX_CODEC_INCAPABLE
        // We ignore them to allow exploration
    }

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}