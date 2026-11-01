// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_image.h>

static int fuzz_image_format(const uint8_t *Data, size_t Size) {
    if (Size < 4) return VPX_IMG_FMT_I420;
    uint32_t fmt_val = *(uint32_t *)Data;
    // Clamp to valid range or specific known formats
    vpx_img_fmt_t fmt = (vpx_img_fmt_t)(fmt_val % 10); 
    if (fmt == VPX_IMG_FMT_NONE) fmt = VPX_IMG_FMT_I420;
    return fmt;
}

static unsigned int fuzz_dimension(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + 4) return 16;
    uint32_t val = *(uint32_t *)(Data + offset);
    // Clamp to reasonable limits to avoid OOM or invalid params
    if (val == 0) return 16;
    if (val > 4096) val = 4096;
    return val;
}

static unsigned int fuzz_alignment(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + 4) return 1;
    uint32_t val = *(uint32_t *)(Data + offset);
    if (val == 0) return 1;
    if (val > 65536) val = 65536;
    return val;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    // 1. vpx_img_alloc
    vpx_img_fmt_t fmt = fuzz_image_format(Data, Size);
    unsigned int w = fuzz_dimension(Data, Size, 4);
    unsigned int h = fuzz_dimension(Data, Size, 8);
    unsigned int align = fuzz_alignment(Data, Size, 12);

    vpx_image_t *img = vpx_img_alloc(NULL, fmt, w, h, align);
    if (!img) return 0;

    // 2. vpx_codec_iface_name
    // Use VP8 encoder interface directly from vp8cx.h
    vpx_codec_iface_t *iface = &vpx_codec_vp8_cx;
    
    const char *name = vpx_codec_iface_name(iface);
    (void)name; // Use to suppress unused warning

    // 3. vpx_codec_enc_config_default
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }

    // Fuzz some config parameters safely
    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_threads = (Size % 8) + 1;
    cfg.g_profile = Size % 4;
    cfg.rc_target_bitrate = (Size % 1000) + 100;
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = 8;

    // 4. vpx_codec_enc_init_ver
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Initialize encoder
    err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }

    // 5. vpx_codec_control_
    // Try a few control IDs that might be supported
    // VP8E_SET_CPUUSED
    int cpu_used = (int)(Size % 8);
    err = vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, cpu_used);
    (void)err;

    // VP8E_SET_ENABLEAUTOALTREF
    int auto_alt = (int)(Size % 2);
    err = vpx_codec_control_(&ctx, VP8E_SET_ENABLEAUTOALTREF, auto_alt);
    (void)err;

    // 6. vpx_img_free
    vpx_img_free(img);

    // 7. vpx_codec_destroy
    err = vpx_codec_destroy(&ctx);
    (void)err;

    return 0;
}