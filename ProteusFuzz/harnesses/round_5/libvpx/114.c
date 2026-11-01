// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_TUNING__value at vp8cx.h:1006:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>

static void cleanup_encoder(vpx_codec_ctx_t *encoder) {
    if (encoder) {
        vpx_codec_destroy(encoder);
    }
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    vpx_codec_iface_t *codec_iface = vpx_codec_vp8_cx();
    if (!codec_iface) {
        return 0;
    }

    vpx_codec_ctx_t encoder;
    if (vpx_codec_enc_init(&encoder, codec_iface, NULL, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Initialize config to set basic parameters
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(codec_iface, &cfg, 0) == VPX_CODEC_OK) {
        cfg.g_w = (Data[0] % 256) + 16;
        cfg.g_h = (Data[1] % 256) + 16;
        if (vpx_codec_enc_config_set(&encoder, &cfg) != VPX_CODEC_OK) {
            cleanup_encoder(&encoder);
            return 0;
        }
    }

    // Fuzz VP8E_SET_MAX_INTRA_BITRATE_PCT
    if (Size > 2) {
        int pct = (int)(Data[2] % 101); // 0 to 100
        vpx_codec_control(&encoder, VP8E_SET_MAX_INTRA_BITRATE_PCT, pct);
    }

    // Fuzz VP8E_SET_TEMPORAL_LAYER_ID
    if (Size > 3) {
        int tid = (int)(Data[3] % 8); // 0 to 7
        vpx_codec_control(&encoder, VP8E_SET_TEMPORAL_LAYER_ID, tid);
    }

    // Fuzz VP8E_SET_FRAME_FLAGS
    if (Size > 4) {
        int flags = (int)(Data[4] & 0xFF);
        vpx_codec_control(&encoder, VP8E_SET_FRAME_FLAGS, flags);
    }

    // Fuzz VP8E_SET_NOISE_SENSITIVITY
    if (Size > 5) {
        int sensitivity = (int)(Data[5] % 10); // 0 to 9
        vpx_codec_control(&encoder, VP8E_SET_NOISE_SENSITIVITY, sensitivity);
    }

    // Fuzz VP8E_SET_CPUUSED
    if (Size > 6) {
        int cpu_used = (int)(Data[6] % 9); // 0 to 8
        vpx_codec_control(&encoder, VP8E_SET_CPUUSED, cpu_used);
    }

    // Fuzz VP8E_SET_TUNING
    if (Size > 7) {
        int tuning = (int)(Data[7] % 2); // 0 or 1
        vpx_codec_control(&encoder, VP8E_SET_TUNING, tuning);
    }

    cleanup_encoder(&encoder);

    return 0;
}