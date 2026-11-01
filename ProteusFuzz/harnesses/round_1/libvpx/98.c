// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int parse_uint32(const uint8_t **data, size_t *size) {
    if (*size < 4) return 0;
    uint32_t val = data[0][0] | (data[0][1] << 8) | (data[0][2] << 16) | (data[0][3] << 24);
    *data += 4;
    *size -= 4;
    return val;
}

static int parse_uint16(const uint8_t **data, size_t *size) {
    if (*size < 2) return 0;
    uint16_t val = data[0][0] | (data[0][1] << 8);
    *data += 2;
    *size -= 2;
    return val;
}

static int parse_uint8(const uint8_t **data, size_t *size) {
    if (*size < 1) return 0;
    uint8_t val = data[0][0];
    *data += 1;
    *size -= 1;
    return val;
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    const uint8_t *ptr = Data;
    size_t remaining = Size;

    // Initialize codec context
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get VP8 encoder interface
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) return 0;

    // Initialize encoder
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) return 0;

    // Parse configuration from fuzz input
    cfg.g_w = parse_uint16(&ptr, &remaining);
    cfg.g_h = parse_uint16(&ptr, &remaining);
    cfg.g_threads = parse_uint8(&ptr, &remaining) & 0x0F; // Limit threads
    cfg.g_timebase.num = parse_uint8(&ptr, &remaining);
    cfg.g_timebase.den = parse_uint8(&ptr, &remaining) + 1; // Avoid division by zero
    cfg.rc_target_bitrate = parse_uint32(&ptr, &remaining);
    cfg.rc_min_quantizer = parse_uint8(&ptr, &remaining) & 0x3F;
    cfg.rc_max_quantizer = parse_uint8(&ptr, &remaining) & 0x3F;
    cfg.kf_mode = parse_uint8(&ptr, &remaining) % 3;
    cfg.kf_min_dist = parse_uint16(&ptr, &remaining);
    cfg.kf_max_dist = parse_uint16(&ptr, &remaining);

    // Ensure valid dimensions
    if (cfg.g_w == 0 || cfg.g_h == 0 || cfg.g_w > 4096 || cfg.g_h > 4096) {
        cfg.g_w = 128;
        cfg.g_h = 128;
    }

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) return 0;

    // Create a test image
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img.planes[0]) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzz data if available
    if (remaining > 0) {
        size_t plane_size = img.stride[0] * cfg.g_h;
        size_t copy_size = (remaining < plane_size) ? remaining : plane_size;
        memcpy(img.planes[0], ptr, copy_size);
    }

    // Fuzz vpx_codec_enc_config_set
    vpx_codec_enc_cfg_t new_cfg = cfg;
    if (remaining > 4) {
        new_cfg.g_w = parse_uint16(&ptr, &remaining);
        new_cfg.g_h = parse_uint16(&ptr, &remaining);
        if (new_cfg.g_w == 0 || new_cfg.g_h == 0) {
            new_cfg.g_w = cfg.g_w;
            new_cfg.g_h = cfg.g_h;
        }
        vpx_codec_enc_config_set(&ctx, &new_cfg);
    }

    // Fuzz vpx_codec_control_
    int ctrl_id = parse_uint32(&ptr, &remaining);
    // Try various control IDs with correct names
    int control_ids[] = {
        VP8E_SET_CPUUSED,
        VP8E_SET_TOKEN_PARTITIONS,
        VP8E_SET_NOISE_SENSITIVITY,
        VP8E_SET_STATIC_THRESHOLD,
        VP8E_SET_ENABLEAUTOALTREF,
        VP8E_SET_ARNR_MAXFRAMES,
        VP8E_SET_ARNR_STRENGTH,
        VP8E_SET_ARNR_TYPE,
        VP8E_SET_TUNING,
        0 // Invalid ID
    };

    // Use parsed ctrl_id or iterate through known ones
    if (ctrl_id > 0 && ctrl_id < sizeof(control_ids)/sizeof(int)) {
        vpx_codec_control_(&ctx, control_ids[ctrl_id], 0);
    } else {
        for (int i = 0; i < sizeof(control_ids)/sizeof(int); i++) {
            vpx_codec_control_(&ctx, control_ids[i], 0);
        }
    }

    // Fuzz vpx_codec_encode
    vpx_codec_pts_t pts = parse_uint32(&ptr, &remaining);
    unsigned long duration = parse_uint16(&ptr, &remaining) + 1;
    vpx_enc_frame_flags_t flags = parse_uint8(&ptr, &remaining);
    vpx_enc_deadline_t deadline = parse_uint32(&ptr, &remaining);

    // Encode a frame
    vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);

    // Flush encoder
    vpx_codec_encode(&ctx, NULL, pts + 1, duration, flags, deadline);

    // Clean up
    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);

    return 0;
}