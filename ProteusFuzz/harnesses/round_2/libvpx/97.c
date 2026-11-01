// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

// Helper to safely extract integers from fuzz data
static int get_int_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) return 0;
    int val;
    memcpy(&val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

// Helper to safely extract unsigned ints
static unsigned int get_uint_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(unsigned int) > size) return 0;
    unsigned int val;
    memcpy(&val, data + *offset, sizeof(unsigned int));
    *offset += sizeof(unsigned int);
    return val;
}

// Helper to extract rational numbers
static vpx_rational_t get_rational_from_data(const uint8_t *data, size_t size, size_t *offset) {
    vpx_rational_t r = {0, 1};
    if (*offset + sizeof(int) <= size) {
        memcpy(&r.num, data + *offset, sizeof(int));
        *offset += sizeof(int);
    }
    if (*offset + sizeof(int) <= size) {
        memcpy(&r.den, data + *offset, sizeof(int));
        *offset += sizeof(int);
        if (r.den == 0) r.den = 1;
    }
    return r;
}

static vpx_codec_enc_cfg_t* create_config_from_data(const uint8_t *data, size_t size, size_t *offset) {
    vpx_codec_enc_cfg_t *cfg = (vpx_codec_enc_cfg_t*)malloc(sizeof(vpx_codec_enc_cfg_t));
    if (!cfg) return NULL;
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));

    // Use VP8 interface for default config
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        free(cfg);
        return NULL;
    }

    if (vpx_codec_enc_config_default(iface, cfg, 0) != VPX_CODEC_OK) {
        free(cfg);
        return NULL;
    }

    // Fuzz various fields
    cfg->g_w = get_uint_from_data(data, size, offset);
    cfg->g_h = get_uint_from_data(data, size, offset);
    if (cfg->g_w == 0) cfg->g_w = 16;
    if (cfg->g_h == 0) cfg->g_h = 16;
    
    cfg->g_threads = get_uint_from_data(data, size, offset) % 8 + 1;
    cfg->g_profile = get_uint_from_data(data, size, offset) % 4;
    
    int bd = get_int_from_data(data, size, offset);
    if (bd == 8) cfg->g_bit_depth = VPX_BITS_8;
    else if (bd == 10) cfg->g_bit_depth = VPX_BITS_10;
    else if (bd == 12) cfg->g_bit_depth = VPX_BITS_12;
    else cfg->g_bit_depth = VPX_BITS_8;

    cfg->g_input_bit_depth = cfg->g_bit_depth;

    cfg->g_timebase.num = get_int_from_data(data, size, offset);
    cfg->g_timebase.den = get_int_from_data(data, size, offset);
    if (cfg->g_timebase.den == 0) cfg->g_timebase.den = 1;

    cfg->rc_end_usage = (enum vpx_rc_mode)(get_int_from_data(data, size, offset) % 4);
    cfg->rc_target_bitrate = get_uint_from_data(data, size, offset);
    if (cfg->rc_target_bitrate == 0) cfg->rc_target_bitrate = 100;

    cfg->rc_min_quantizer = get_uint_from_data(data, size, offset) % 64;
    cfg->rc_max_quantizer = get_uint_from_data(data, size, offset) % 63;
    if (cfg->rc_max_quantizer <= cfg->rc_min_quantizer) cfg->rc_max_quantizer = cfg->rc_min_quantizer + 1;

    cfg->kf_mode = (enum vpx_kf_mode)(get_int_from_data(data, size, offset) % 3);
    cfg->kf_min_dist = get_uint_from_data(data, size, offset);
    cfg->kf_max_dist = get_uint_from_data(data, size, offset);
    if (cfg->kf_max_dist == 0) cfg->kf_max_dist = 120;

    // Fuzz some rational params
    cfg->active_wq_factor = get_rational_from_data(data, size, offset);
    cfg->err_per_mb_factor = get_rational_from_data(data, size, offset);

    return cfg;
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    size_t offset = 0;
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) return 0;

    vpx_codec_enc_cfg_t *cfg = create_config_from_data(Data, Size, &offset);
    if (!cfg) return 0;

    // Flags: 0 or VPX_CODEC_USE_PSNR
    // Note: VPX_CODEC_USE_STATS is not a valid flag in recent libvpx versions.
    // Only VPX_CODEC_USE_PSNR is commonly available for encoders.
    vpx_codec_flags_t flags = 0;
    int flag_val = get_int_from_data(Data, Size, &offset);
    if (flag_val & 1) flags |= VPX_CODEC_USE_PSNR;

    vpx_codec_err_t res = vpx_codec_enc_init_ver(&ctx, iface, cfg, flags, VPX_ENCODER_ABI_VERSION);
    
    if (res == VPX_CODEC_OK) {
        int ctrl_id = get_int_from_data(Data, Size, &offset);
        int arg1 = get_int_from_data(Data, Size, &offset);
        
        // Try calling with an int argument
        res = vpx_codec_control_(&ctx, ctrl_id, arg1);
        
        // Try calling with a pointer argument
        uint8_t dummy_buf[64];
        memset(dummy_buf, 0, 64);
        res = vpx_codec_control_(&ctx, ctrl_id, dummy_buf);
        
        // Attempt to set tuning (generic int)
        int tuning_val = get_int_from_data(Data, Size, &offset);
        res = vpx_codec_control_(&ctx, ctrl_id, tuning_val);

        // Attempt to set decryptor (struct pointer) - though this is usually for decoders
        vpx_decrypt_init decrypt_init;
        decrypt_init.decrypt_cb = NULL;
        decrypt_init.decrypt_state = NULL;
        res = vpx_codec_control_(&ctx, ctrl_id, &decrypt_init);
        
        // Cleanup
        vpx_codec_destroy(&ctx);
    }
    
    free(cfg);
    
    return 0;
}