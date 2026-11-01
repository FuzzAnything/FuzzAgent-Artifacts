// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_ARNR_MAXFRAMES__value at vp8cx.h:1000:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int fuzz_vp8_encoder_controls(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg_enc;
    int ret = 0;

    if (size < 4) return 0;

    // Extract control values from input data
    int screen_content_mode = (data[0] % 2) == 0 ? 0 : 1;
    int arnr_strength = data[1] % 10;
    int noise_sensitivity = data[2] % 10;
    int arnr_maxframes = data[3] % 16;

    // Initialize encoder configuration
    vpx_codec_iface_t *encoder_if = vpx_codec_vp8_cx();
    if (!encoder_if) return 0;

    if (vpx_codec_enc_config_default(encoder_if, &cfg_enc, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg_enc.g_w = 128;
    cfg_enc.g_h = 128;
    cfg_enc.g_timebase.num = 1;
    cfg_enc.g_timebase.den = 30;
    cfg_enc.g_threads = 1;
    cfg_enc.g_lag_in_frames = 0;
    cfg_enc.rc_end_usage = VPX_CBR;
    cfg_enc.rc_target_bitrate = 1000;

    if (vpx_codec_enc_init(&ctx, encoder_if, &cfg_enc, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Apply VP8 encoder controls
    vpx_codec_control(&ctx, VP8E_SET_SCREEN_CONTENT_MODE, screen_content_mode);
    vpx_codec_control(&ctx, VP8E_SET_ARNR_STRENGTH, arnr_strength);
    vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    vpx_codec_control(&ctx, VP8E_SET_ARNR_MAXFRAMES, arnr_maxframes);

    // Cleanup encoder
    vpx_codec_destroy(&ctx);

    return 0;
}

static int fuzz_vp8_decoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_iface_t *decoder_if = NULL;

    if (size < 1) return 0;

    decoder_if = vpx_codec_vp8_dx();
    if (!decoder_if) return 0;

    // Initialize decoder config manually to avoid missing function
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    if (vpx_codec_dec_init(&ctx, decoder_if, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Attempt to decode dummy data
    vpx_codec_decode(&ctx, data, size, NULL, 0);

    vpx_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Fuzz VP8 encoder controls
    fuzz_vp8_encoder_controls(Data, Size);

    // Fuzz VP8 decoder
    fuzz_vp8_decoder(Data, Size);

    return 0;
}