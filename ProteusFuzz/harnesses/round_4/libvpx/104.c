// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

// Helper to safely parse integers from fuzz data
static int parse_int(const uint8_t *data, size_t size, size_t *offset, int min, int max) {
    if (*offset + sizeof(int) > size) return 0;
    int val = *(const int *)(data + *offset);
    *offset += sizeof(int);
    if (val < min || val > max) val = min;
    return val;
}

// Helper to parse unsigned int
static unsigned int parse_uint(const uint8_t *data, size_t size, size_t *offset, unsigned int max) {
    if (*offset + sizeof(unsigned int) > size) return 0;
    unsigned int val = *(const unsigned int *)(data + *offset);
    *offset += sizeof(unsigned int);
    if (val > max) val = max;
    return val;
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    size_t offset = 0;

    // Initialize config with defaults
    if (vpx_codec_enc_config_default(NULL, &cfg, 0) != VPX_CODEC_OK) {
        // Fallback to manual initialization if default fails or interface is unknown
        memset(&cfg, 0, sizeof(cfg));
        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.rc_target_bitrate = 100;
        cfg.rc_min_quantizer = 0;
        cfg.rc_max_quantizer = 63;
        cfg.g_error_resilient = 0;
        cfg.g_pass = VPX_RC_ONE_PASS;
        cfg.kf_mode = VPX_KF_DISABLED;
        cfg.kf_min_dist = 0;
        cfg.kf_max_dist = 0;
    }

    // Fuzz configuration parameters
    if (offset + 4 < Size) {
        cfg.g_w = parse_uint(Data, Size, &offset, 65535);
        cfg.g_h = parse_uint(Data, Size, &offset, 65535);
        if (cfg.g_w == 0 || cfg.g_h == 0) { cfg.g_w = 16; cfg.g_h = 16; }
    }

    if (offset + 4 < Size) {
        int val = parse_int(Data, Size, &offset, 0, 1000);
        cfg.g_threads = (unsigned int)val;
    }

    if (offset + 4 < Size) {
        int val = parse_int(Data, Size, &offset, 0, 1000000);
        cfg.rc_target_bitrate = (unsigned int)val;
    }

    if (offset + 4 < Size) {
        int val = parse_int(Data, Size, &offset, 0, 63);
        cfg.rc_min_quantizer = (unsigned int)val;
        cfg.rc_max_quantizer = (unsigned int)val + 10;
        if (cfg.rc_max_quantizer > 63) cfg.rc_max_quantizer = 63;
    }

    // Try VP8 Encoder
    vpx_codec_iface_t *iface_vp8 = vpx_codec_vp8_cx();
    if (iface_vp8) {
        res = vpx_codec_enc_init_ver(&ctx, iface_vp8, &cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            // Fuzz vpx_codec_control_
            if (offset < Size) {
                int ctrl_id = parse_int(Data, Size, &offset, -1000, 1000);
                // Try VP8E_SET_TUNING if available (mapped via control)
                // We cast to void* to avoid strict type checking in va_list if needed, 
                // but vpx_codec_control_ expects specific args based on ctrl_id.
                // For fuzzing, we pass a dummy value for tuning if ctrl_id matches.
                int tuning_val = 0;
                if (offset < Size) tuning_val = parse_int(Data, Size, &offset, 0, 100);
                
                // Attempt to call control. Note: VP8E_SET_TUNING is usually VP8E_SET_TUNING
                // We use a generic call. If ctrl_id is invalid, it returns error safely.
                vpx_codec_control_(&ctx, ctrl_id, tuning_val);
            }
            
            // Cleanup VP8
            vpx_codec_destroy(&ctx);
        }
    }

    // Try VP9 Encoder
    // Note: vpx_codec_vp9_cx() might not be available if VP9 is not compiled in,
    // but we try to call it. If it returns NULL, we skip.
    vpx_codec_iface_t *iface_vp9 = vpx_codec_vp9_cx();
    if (iface_vp9) {
        // Re-init config if needed (some fields might be overwritten by macro, but we reset manually)
        if (vpx_codec_enc_config_default(iface_vp9, &cfg, 0) != VPX_CODEC_OK) {
             // Fallback
             cfg.g_w = 16; cfg.g_h = 16; cfg.g_threads = 1; cfg.rc_target_bitrate = 100;
             cfg.rc_min_quantizer = 0; cfg.rc_max_quantizer = 63;
             cfg.g_pass = VPX_RC_ONE_PASS; cfg.kf_mode = VPX_KF_DISABLED;
        }

        res = vpx_codec_enc_init_ver(&ctx, iface_vp9, &cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            // Fuzz vpx_codec_control_ for VP9 specific controls
            if (offset < Size) {
                int ctrl_id = parse_int(Data, Size, &offset, -1000, 1000);
                int val = 0;
                if (offset < Size) val = parse_int(Data, Size, &offset, 0, 100);
                
                // Attempt VP9E_SET_TUNE_CONTENT or other controls
                // The actual enum values are internal, so we fuzz the int directly
                vpx_codec_control_(&ctx, ctrl_id, val);
            }
            
            // Cleanup VP9
            vpx_codec_destroy(&ctx);
        }
    }

    // Attempt to fuzz VP8D_SET_DECRYPTOR__value if it exists as a macro or function
    // Since the signature is empty in the prompt, we cannot call it directly.
    // However, if it's a macro expanding to a control call, it might be accessible via control.
    // We assume the prompt implies these might be control IDs.
    // If they are actual functions not exposed, we skip them as per instructions to not invent.
    
    // Attempt to fuzz VPXD_SET_DECRYPTOR__value similarly.
    // Without signature, we cannot construct a valid call.

    return 0;
}