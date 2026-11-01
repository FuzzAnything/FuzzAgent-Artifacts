// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_image.h>

static int try_fuzz_encoding(const uint8_t *Data, size_t Size) {
    // 1. Prepare environment: Allocate image
    vpx_image_t *img = NULL;
    unsigned int width = 64;
    unsigned int height = 64;
    unsigned int align = 16;

    // Fuzz dimensions from input data, ensuring they are within valid bounds
    // Fix: Check Size strictly before casting or accessing Data + 4
    if (Size >= 8) {
        // Read 4 bytes for width and 4 bytes for height safely
        uint32_t w_val = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
        uint32_t h_val = Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24);
        width = w_val;
        height = h_val;
    } else if (Size >= 2) {
        width = Data[0];
        height = Data[1];
    }

    // Clamp dimensions to valid range (max 2^27, min 1)
    if (width == 0) width = 16;
    if (height == 0) height = 16;
    if (width > 0x08000000) width = 0x08000000;
    if (height > 0x08000000) height = 0x08000000;
    if (align > 65536) align = 16;

    // vpx_img_alloc
    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, align);
    if (!img) {
        return 0; // Allocation failed, skip this iteration
    }

    // Get encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        vpx_img_free(img);
        return 0;
    }

    // 2. vpx_codec_iface_name (for logging/debugging, deterministic)
    const char *name = vpx_codec_iface_name(iface);
    (void)name; // Use to avoid unused variable warning

    // 3. vpx_codec_enc_config_default
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }

    // Fuzz some configuration parameters safely with bounds checks
    if (Size > 0) {
        cfg.g_threads = Data[0] % 16; // Limit threads
    }
    if (Size > 1) {
        cfg.g_profile = Data[1] % 4;
    }
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = VPX_BITS_8;
    
    if (Size > 2) {
        cfg.rc_end_usage = (enum vpx_rc_mode)(Data[2] % 4);
    }
    if (Size > 3) {
        cfg.rc_target_bitrate = 100 + (Data[3] % 1000);
    }
    if (Size > 4) {
        cfg.kf_mode = (enum vpx_kf_mode)(Data[4] % 3);
    }
    if (Size > 5) {
        cfg.kf_min_dist = 10 + (Data[5] % 100);
    }
    if (Size > 6) {
        cfg.kf_max_dist = 100 + (Data[6] % 500);
    }

    // 4. vpx_codec_enc_init_ver
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Use standard flags, maybe fuzz a bit if safe
    vpx_codec_flags_t flags = 0;
    if (Size > 7) {
        flags = Data[7] & 0x0F; // Limit to lower bits
    }

    res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }

    // 5. vpx_codec_control_
    // Try a few control commands. VP8E_SET_CPUUSED is a common one.
    int ctrl_id = VP8E_SET_CPUUSED;
    if (Size > 8) {
        ctrl_id = (int)Data[8];
    }
    
    int arg_val = 0;
    if (Size > 9) {
        arg_val = (int)Data[9];
    }
    
    // For VP8E_SET_CPUUSED, we need an int argument.
    if (ctrl_id == VP8E_SET_CPUUSED) {
        vpx_codec_control_(&ctx, ctrl_id, arg_val);
    } else {
        // Try calling without extra args for other IDs to explore error paths
        vpx_codec_control_(&ctx, ctrl_id);
    }

    // 6. Cleanup: vpx_img_free
    vpx_img_free(img);

    // 7. Cleanup: vpx_codec_destroy
    vpx_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Execute the fuzzing logic
    try_fuzz_encoding(Data, Size);

    return 0;
}