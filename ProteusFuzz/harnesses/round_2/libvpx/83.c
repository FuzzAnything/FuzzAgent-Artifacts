// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

// Helper to parse a rational number from fuzz input
static vpx_rational_t parse_rational(const uint8_t *data, size_t *offset, size_t size) {
    vpx_rational_t r = {0, 1};
    if (*offset + 8 <= size) {
        r.num = *(int32_t *)(data + *offset);
        *offset += 4;
        r.den = *(int32_t *)(data + *offset);
        *offset += 4;
        if (r.den == 0) r.den = 1;
    }
    return r;
}

// Helper to parse an integer from fuzz input
static unsigned int parse_uint(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 4 <= size) {
        unsigned int val = *(uint32_t *)(data + *offset);
        *offset += 4;
        return val;
    }
    return 0;
}

// Helper to parse an enum from fuzz input
static int parse_int(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 4 <= size) {
        int val = *(int32_t *)(data + *offset);
        *offset += 4;
        return val;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t res;
    int ver = VPX_ENCODER_ABI_VERSION;
    size_t offset = 0;

    // Try to get VP8 encoder interface
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    // Initialize configuration structure
    memset(&cfg, 0, sizeof(cfg));
    cfg.g_w = parse_uint(Data, &offset, Size);
    cfg.g_h = parse_uint(Data, &offset, Size);
    cfg.g_threads = parse_uint(Data, &offset, Size);
    cfg.g_profile = parse_uint(Data, &offset, Size);
    cfg.g_bit_depth = parse_int(Data, &offset, Size);
    cfg.g_input_bit_depth = parse_uint(Data, &offset, Size);
    
    // Parse timebase
    cfg.g_timebase = parse_rational(Data, &offset, Size);
    
    cfg.g_error_resilient = parse_uint(Data, &offset, Size);
    cfg.g_pass = parse_int(Data, &offset, Size);
    cfg.g_lag_in_frames = parse_uint(Data, &offset, Size);

    // Rate control settings
    cfg.rc_dropframe_thresh = parse_uint(Data, &offset, Size);
    cfg.rc_resize_allowed = parse_uint(Data, &offset, Size);
    cfg.rc_scaled_width = parse_uint(Data, &offset, Size);
    cfg.rc_scaled_height = parse_uint(Data, &offset, Size);
    cfg.rc_resize_up_thresh = parse_uint(Data, &offset, Size);
    cfg.rc_resize_down_thresh = parse_uint(Data, &offset, Size);
    cfg.rc_end_usage = parse_int(Data, &offset, Size);
    cfg.rc_target_bitrate = parse_uint(Data, &offset, Size);

    // Quantizer settings
    cfg.rc_min_quantizer = parse_uint(Data, &offset, Size);
    cfg.rc_max_quantizer = parse_uint(Data, &offset, Size);

    // Bitrate tolerance
    cfg.rc_undershoot_pct = parse_uint(Data, &offset, Size);
    cfg.rc_overshoot_pct = parse_uint(Data, &offset, Size);

    // Decoder buffer model
    cfg.rc_buf_sz = parse_uint(Data, &offset, Size);
    cfg.rc_buf_initial_sz = parse_uint(Data, &offset, Size);
    cfg.rc_buf_optimal_sz = parse_uint(Data, &offset, Size);

    // Keyframing
    cfg.kf_mode = parse_int(Data, &offset, Size);
    cfg.kf_min_dist = parse_uint(Data, &offset, Size);
    cfg.kf_max_dist = parse_uint(Data, &offset, Size);

    // Validate basic dimensions
    if (cfg.g_w == 0 || cfg.g_h == 0) {
        cfg.g_w = 128;
        cfg.g_h = 128;
    }
    if (cfg.g_threads == 0) cfg.g_threads = 1;
    if (cfg.g_bit_depth < 8) cfg.g_bit_depth = 8;
    if (cfg.g_input_bit_depth < 8) cfg.g_input_bit_depth = 8;

    // Initialize encoder
    memset(&ctx, 0, sizeof(ctx));
    res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, ver);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz control commands
    // 1. VP8E_SET_CPUUSED
    int cpu_used = parse_int(Data, &offset, Size) % 9; // 0-8
    vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, cpu_used);

    // 2. VP8E_SET_TUNING
    int tuning = parse_int(Data, &offset, Size) % 10;
    vpx_codec_control_(&ctx, VP8E_SET_TUNING, tuning);

    // 3. VP9E_SET_TUNE_CONTENT (Not available for VP8, but we can try to call it to test error handling)
    // Since we have an encoder context, this should likely fail or be ignored, which is good for fuzzing.
    // We need a dummy content type
    int content = parse_int(Data, &offset, Size) % 3; // 0: VP9E_CONTENT_DEFAULT, 1: VP9E_CONTENT_SCREEN, 2: VP9E_CONTENT_FILM
    vpx_codec_control_(&ctx, VP9E_SET_TUNE_CONTENT, content);

    // 4. VP8D_SET_DECRYPTOR (This is for decoder, but we can try to call it to test error handling)
    // Since we have an encoder context, this should likely fail or be ignored, which is good for fuzzing.
    // We need a dummy decrypt_init struct
    vpx_decrypt_init decrypt_init;
    decrypt_init.decrypt_cb = NULL;
    decrypt_init.decrypt_state = NULL;
    vpx_codec_control_(&ctx, VP8D_SET_DECRYPTOR, &decrypt_init);

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}