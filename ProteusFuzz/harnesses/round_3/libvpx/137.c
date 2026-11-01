// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_ARNR_TYPE__value at vp8cx.h:1004:1 in vp8cx.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// VP8E_SET_ARNR_MAXFRAMES__value at vp8cx.h:1000:1 in vp8cx.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

// Helper to safely read integers from fuzz input
static int read_int(const uint8_t *data, size_t *offset, size_t size, int min_val, int max_val) {
    if (*offset + sizeof(int) > size) return 0;
    int val = *(const int *)(data + *offset);
    *offset += sizeof(int);
    if (val < min_val || val > max_val) val = min_val;
    return val;
}

// Helper to safely read a byte from fuzz input
static uint8_t read_byte(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset >= size) return 0;
    return data[(*offset)++];
}

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *codec_iface;
    vpx_codec_err_t res;
    size_t offset = 0;

    // Initialize configuration
    if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure encoder settings slightly
    cfg.g_w = read_int(Data, &offset, Size, 16, 1920);
    cfg.g_h = read_int(Data, &offset, Size, 16, 1080);
    cfg.g_timebase.num = read_byte(Data, &offset, Size) + 1;
    cfg.g_timebase.den = read_byte(Data, &offset, Size) + 1;
    cfg.g_error_resilient = read_byte(Data, &offset, Size) % 3;

    // Initialize encoder with VP8 interface
    // vpx_codec_vp8_cx() returns the VP8 encoder interface
    codec_iface = vpx_codec_vp8_cx();
    if (codec_iface == NULL) {
        return 0;
    }

    res = vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz VP8E_SET_CPUUSED (0-8)
    if (offset < Size) {
        int cpu_used = read_int(Data, &offset, Size, 0, 8);
        vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used);
    }

    // Fuzz VP8E_SET_NOISE_SENSITIVITY (0-255)
    if (offset < Size) {
        int noise_sens = read_int(Data, &offset, Size, 0, 255);
        vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, noise_sens);
    }

    // Fuzz VP8E_SET_ARNR_TYPE (0-2)
    if (offset < Size) {
        int arnr_type = read_int(Data, &offset, Size, 0, 2);
        vpx_codec_control(&ctx, VP8E_SET_ARNR_TYPE, arnr_type);
    }

    // Fuzz VP8E_SET_ARNR_STRENGTH (0-127)
    if (offset < Size) {
        int arnr_strength = read_int(Data, &offset, Size, 0, 127);
        vpx_codec_control(&ctx, VP8E_SET_ARNR_STRENGTH, arnr_strength);
    }

    // Fuzz VP8E_SET_ARNR_MAXFRAMES (1-64)
    if (offset < Size) {
        int arnr_maxframes = read_int(Data, &offset, Size, 1, 64);
        vpx_codec_control(&ctx, VP8E_SET_ARNR_MAXFRAMES, arnr_maxframes);
    }

    // Call vpx_codec_control_ directly with various control IDs to test robustness
    // We use the internal function as requested, though typically the macro is used
    if (offset < Size) {
        int ctrl_id = read_int(Data, &offset, Size, 0, 1000);
        int dummy_val = read_int(Data, &offset, Size, 0, 1000);
        // Note: vpx_codec_control_ is internal, we call it directly.
        // The signature is vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t *ctx, int ctrl_id, ...);
        res = vpx_codec_control_(&ctx, ctrl_id, dummy_val);
        (void)res; // Suppress unused warning
    }

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}