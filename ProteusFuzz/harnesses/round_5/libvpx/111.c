// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_TUNING__value at vp8cx.h:1006:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_integer.h>

static int fuzz_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();

    if (!iface) return 0;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz control parameters using input data safely
    if (size > 0) {
        int val = (int)data[0] & 0xFF; // Ensure value is within 0-255
        vpx_codec_control(&ctx, VP8E_SET_CPUUSED, val);
    }
    if (size > 1) {
        int val = (int)data[1] & 0xFF;
        vpx_codec_control(&ctx, VP8E_SET_TUNING, val);
    }
    if (size > 2) {
        int val = (int)data[2] & 0xFF;
        vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, val);
    }
    if (size > 3) {
        int val = (int)data[3] & 0xFF;
        vpx_codec_control(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, val);
    }
    if (size > 4) {
        int val = (int)data[4] & 0xFF;
        vpx_codec_control(&ctx, VP8E_SET_FRAME_FLAGS, val);
    }

    // Allocate a buffer large enough for a 16x16 I420 frame
    // Y: 16*16, U: 8*8, V: 8*8. Total = 256 + 64 + 64 = 384 bytes.
    // We allocate slightly more to be safe and avoid edge cases.
    uint8_t *frame_buf = (uint8_t *)malloc(400);
    if (!frame_buf) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Initialize frame buffer with zeros to ensure valid memory state
    memset(frame_buf, 0, 400);

    // Encode a few frames
    // We use a fixed small input to avoid buffer overflow issues with raw fuzzer data
    // The fuzzer data is used to seed the frame content if available, but we ensure bounds.
    for (int i = 0; i < 3; i++) {
        vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
        vpx_image_t img;
        vpx_codec_err_t res;
        vpx_codec_cx_pkt_t pkt;

        // Prepare image structure
        img.planes[0] = frame_buf; // Y plane
        img.planes[1] = frame_buf + 256; // U plane
        img.planes[2] = frame_buf + 320; // V plane
        img.stride[0] = 16;
        img.stride[1] = 8;
        img.stride[2] = 8;
        img.w = 16;
        img.h = 16;
        img.d_w = 16;
        img.d_h = 16;
        img.x_chroma_shift = 1;
        img.y_chroma_shift = 1;
        img.fmt = fmt;

        // Optionally copy some fuzzer data into the frame buffer if available
        size_t copy_size = (size > 384) ? 384 : size;
        if (copy_size > 0) {
            memcpy(frame_buf, data, copy_size);
        }

        res = vpx_codec_encode(&ctx, &img, i, 1, 0, 0);
        if (res != VPX_CODEC_OK) continue;

        while (vpx_codec_get_cx_data(&ctx, &pkt) == VPX_CODEC_OK) {
            if (pkt.kind == VPX_CODEC_CX_FRAME_PKT) {
                // Process encoded frame
                // No action needed for fuzzing, just consume the packet
            }
        }
    }

    free(frame_buf);
    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    return fuzz_encoder(Data, Size);
}