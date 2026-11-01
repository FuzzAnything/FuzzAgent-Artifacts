// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static vpx_codec_iface_t *get_encoder_iface(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return &vpx_codec_vp8_cx;
    }
    switch (Data[0] % 2) {
        case 0:
            return &vpx_codec_vp8_cx;
        case 1:
            return &vpx_codec_vp9_cx;
        default:
            return &vpx_codec_vp8_cx;
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    vpx_image_t *img = NULL;
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface;
    vpx_codec_err_t res;
    const char *iface_name;

    // Select interface based on input
    iface = get_encoder_iface(Data, Size);

    // Initialize config with defaults
    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz config parameters safely
    if (Size > 0) cfg.g_w = (Data[0] % 2048) + 16;
    if (Size > 1) cfg.g_h = (Data[1] % 2048) + 16;
    if (Size > 2) cfg.g_threads = (Data[2] % 16) + 1;
    if (Size > 3) cfg.g_profile = Data[3] % 4;
    if (Size > 4) cfg.g_bit_depth = (Data[4] % 3 == 0) ? VPX_BITS_8 : ((Data[4] % 3 == 1) ? VPX_BITS_10 : VPX_BITS_12);
    if (Size > 5) cfg.g_input_bit_depth = cfg.g_bit_depth;
    if (Size > 6) cfg.rc_target_bitrate = (Data[6] % 1000) + 100;
    if (Size > 7) cfg.kf_min_dist = (Data[7] % 100) + 1;
    if (Size > 8) cfg.kf_max_dist = (Data[8] % 100) + 1;
    if (Size > 9) cfg.kf_mode = (Data[9] % 3); // VPX_KF_DISABLED, VPX_KF_AUTO, VPX_KF_FIXED
    if (Size > 10) cfg.rc_end_usage = (Data[10] % 4); // VPX_VBR, VPX_CBR, VPX_CQ, VPX_Q
    if (Size > 11) cfg.g_pass = (Data[11] % 3); // VPX_RC_ONE_PASS, etc.

    // Initialize timebase
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Size > 12) ? (Data[12] % 60 + 1) : 30;

    // 2. Invoke target functions in order

    // vpx_img_alloc
    // Use safe dimensions and alignment
    unsigned int w = cfg.g_w;
    unsigned int h = cfg.g_h;
    unsigned int align = 16;
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    
    if (Size > 13) {
        fmt = (vpx_img_fmt_t)(Data[13] % 11); // Limit to known formats
        if (fmt == VPX_IMG_FMT_NONE) fmt = VPX_IMG_FMT_I420;
    }

    img = vpx_img_alloc(NULL, fmt, w, h, align);
    if (img == NULL) {
        return 0;
    }

    // vpx_codec_iface_name
    iface_name = vpx_codec_iface_name(iface);
    if (iface_name == NULL) {
        vpx_img_free(img);
        return 0;
    }
    (void)iface_name; // Use to avoid unused warning

    // vpx_codec_enc_init_ver
    memset(&ctx, 0, sizeof(ctx));
    vpx_codec_flags_t flags = 0;
    if (Size > 14) flags = (vpx_codec_flags_t)(Data[14] & 0x0F);
    
    res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }

    // vpx_codec_control_
    // Try a few control commands based on input
    int ctrl_id = 0;
    if (Size > 15) {
        ctrl_id = (int)(Data[15]);
    }
    
    // Attempt to call control. Note: vpx_codec_control_ is variadic.
    // We pass a dummy argument to satisfy the signature, though specific controls need specific args.
    // For fuzzing, we try a generic call. If the control ID requires args, it might fail gracefully.
    // We use a safe dummy value.
    res = vpx_codec_control_(&ctx, ctrl_id, 0);
    // Ignore return value for fuzzing exploration, but check for crash

    // 3. Cleanup
    // vpx_img_free
    vpx_img_free(img);

    // vpx_codec_destroy
    vpx_codec_destroy(&ctx);

    return 0;
}