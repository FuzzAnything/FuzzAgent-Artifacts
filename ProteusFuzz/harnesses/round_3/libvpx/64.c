// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_ENABLEAUTOALTREF__value at vp8cx.h:986:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>

static int fuzz_encoder_controls(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 1) {
        return 0;
    }

    int ret = 0;
    int val;

    // Extract value from input data
    val = data[0];

    // 1. VP8E_SET_STATIC_THRESHOLD
    // Expected range: 0-65535 (uint16_t), but we clamp to safe int range for testing
    if (size >= 1) {
        ret = vpx_codec_control(ctx, VP8E_SET_STATIC_THRESHOLD, val);
        if (ret != VPX_CODEC_OK) {
            // Continue to test other controls even if one fails
        }
    }

    // 2. VP8E_SET_CPUUSED
    // Expected range: 0-8
    if (size >= 1) {
        val = data[0] % 9;
        ret = vpx_codec_control(ctx, VP8E_SET_CPUUSED, val);
    }

    // 3. VP8E_SET_NOISE_SENSITIVITY
    // Expected range: 0-127 (uint8_t)
    if (size >= 1) {
        val = data[0] % 128;
        ret = vpx_codec_control(ctx, VP8E_SET_NOISE_SENSITIVITY, val);
    }

    // 4. VP8E_SET_ENABLEAUTOALTREF
    // Expected range: 0-1 (boolean)
    if (size >= 1) {
        val = data[0] % 2;
        ret = vpx_codec_control(ctx, VP8E_SET_ENABLEAUTOALTREF, val);
    }

    // 5. VP8E_SET_MAX_INTRA_BITRATE_PCT
    // Expected range: 0-100
    if (size >= 1) {
        val = data[0] % 101;
        ret = vpx_codec_control(ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, val);
    }

    // 6. VP8E_SET_FRAME_FLAGS
    // Expected: vp8_frame_flags_t flags
    if (size >= 1) {
        // Use lower bits as flags
        uint32_t flags = data[0];
        ret = vpx_codec_control(ctx, VP8E_SET_FRAME_FLAGS, flags);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    vpx_codec_enc_cfg_t cfg;
    int ret;

    // Initialize encoder interface
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder context
    ret = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Run fuzz logic
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}