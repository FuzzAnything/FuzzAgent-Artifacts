// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_KEY_FRAME_FILTERING__value at vp8cx.h:1109:1 in vp8cx.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_codec.h>

static vpx_codec_ctx_t ctx;
static vpx_image_t img;
static vpx_codec_enc_cfg_t cfg;
static int initialized = 0;
static vpx_codec_pts_t pts = 0;

static void cleanup_encoder(void) {
    if (initialized) {
        vpx_codec_destroy(&ctx);
        initialized = 0;
    }
}

static int init_encoder(const uint8_t *Data, size_t Size) {
    vpx_codec_iface_t *iface;
    vpx_codec_err_t res;

    cleanup_encoder();

    if (Size < 4) {
        // Not enough data to determine width/height, use defaults
        cfg.g_w = 64;
        cfg.g_h = 64;
    } else {
        // Use fuzz data to determine dimensions, ensuring non-zero and reasonable limits
        uint32_t w = *(const uint32_t*)Data;
        uint32_t h = *(const uint32_t*)(Data + 4);
        
        if (w == 0 || w > 4096) w = 64;
        if (h == 0 || h > 4096) h = 64;
        
        cfg.g_w = w;
        cfg.g_h = h;
    }

    iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return 0;

    // Apply fuzzed configuration values
    if (Size > 4) {
        cfg.g_threads = *(const uint8_t*)(Data + 4) % 16;
        cfg.g_profile = *(const uint8_t*)(Data + 5) % 4;
        cfg.g_bit_depth = (vpx_bit_depth_t)(*(const uint8_t*)(Data + 6) % 3 + 8); // 8, 10, 12
        cfg.g_input_bit_depth = cfg.g_bit_depth;
        cfg.g_timebase.num = *(const uint8_t*)(Data + 7) + 1;
        cfg.g_timebase.den = *(const uint8_t*)(Data + 8) + 1;
        cfg.rc_end_usage = (enum vpx_rc_mode)(*(const uint8_t*)(Data + 9) % 4);
        cfg.rc_target_bitrate = *(const uint16_t*)(Data + 10) % 10000 + 100;
        cfg.kf_mode = (enum vpx_kf_mode)(*(const uint8_t*)(Data + 12) % 3);
        cfg.kf_min_dist = *(const uint16_t*)(Data + 13) % 100;
        cfg.kf_max_dist = *(const uint16_t*)(Data + 15) % 1000;
    }

    res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return 0;

    // Initialize image
    memset(&img, 0, sizeof(img));
    img.w = cfg.g_w;
    img.h = cfg.g_h;
    img.d_w = cfg.g_w;
    img.d_h = cfg.g_h;
    img.r_w = cfg.g_w;
    img.r_h = cfg.g_h;
    img.bit_depth = cfg.g_bit_depth;
    img.fmt = VPX_IMG_FMT_I420;
    img.cs = VPX_CS_BT_601;
    img.range = VPX_CR_STUDIO_RANGE;

    if (vpx_img_alloc(&img, img.fmt, img.w, img.h, 1) != 0) {
        cleanup_encoder();
        return 0;
    }

    initialized = 1;
    return 1;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize if not already done
    if (!initialized) {
        if (!init_encoder(Data, Size)) return 0;
    }

    // Fuzz vpx_codec_enc_config_set
    // Re-initialize config with fuzzed data to test reconfiguration
    vpx_codec_enc_cfg_t new_cfg = cfg;
    if (Size > 20) {
        new_cfg.g_threads = Data[20] % 16;
        new_cfg.rc_target_bitrate = (Data[21] << 8 | Data[22]) % 10000 + 100;
        new_cfg.kf_mode = (enum vpx_kf_mode)(Data[23] % 3);
    }
    vpx_codec_enc_config_set(&ctx, &new_cfg);

    // Fuzz vpx_codec_encode
    // Prepare image data with fuzzed content
    for (int i = 0; i < 4; i++) {
        if (img.planes[i] && img.stride[i]) {
            size_t plane_size = img.stride[i] * img.h;
            if (Size > 30) {
                size_t copy_len = (Size - 30) < plane_size ? (Size - 30) : plane_size;
                memcpy(img.planes[i], Data + 30, copy_len);
                // Fill rest with pattern to ensure valid memory access
                memset(img.planes[i] + copy_len, Data[30], plane_size - copy_len);
            } else {
                memset(img.planes[i], 0, plane_size);
            }
        }
    }

    // Generate fuzzed parameters for encode
    vpx_codec_pts_t fuzz_pts = pts;
    unsigned long fuzz_duration = 1;
    vpx_enc_frame_flags_t fuzz_flags = 0;
    vpx_enc_deadline_t fuzz_deadline = 0;

    if (Size > 40) {
        fuzz_duration = (Data[40] << 8 | Data[41]) % 1000 + 1;
        fuzz_flags = Data[42];
        fuzz_deadline = (Data[43] << 8 | Data[44]) % 1000000;
        if (fuzz_deadline == 0) fuzz_deadline = 1; // Avoid 0 if we want to test non-infinite, but 0 is valid per spec
    }

    // Ensure strictly increasing PTS
    if (Size > 50) {
        fuzz_pts = pts + (Data[50] % 10) + 1;
    } else {
        fuzz_pts = pts + 1;
    }
    pts = fuzz_pts;

    // Call encode
    vpx_codec_encode(&ctx, &img, fuzz_pts, fuzz_duration, fuzz_flags, fuzz_deadline);

    // Check for output data and consume it to keep encoder state clean
    vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Optionally process pkt->data.frame
        }
        // vpx_codec_cx_pkt_free is not a public API in standard libvpx headers for this version.
        // The packets returned by vpx_codec_get_cx_data are managed internally or do not need explicit freeing
        // in the same way as malloc'd memory in this context, or the free function is internal.
        // We simply consume them.
    }

    // Fuzz vpx_codec_control_ (using macro wrapper)
    // Test various control IDs
    if (Size > 55) {
        int ctrl_id = Data[55];
        // Common VP9 control IDs
        if (ctrl_id == VP9E_SET_TILE_COLUMNS) {
            int val = Data[56] % 16;
            vpx_codec_control(&ctx, VP9E_SET_TILE_COLUMNS, val);
        } else if (ctrl_id == VP9E_SET_SVC) {
            int val = Data[56] % 2;
            vpx_codec_control(&ctx, VP9E_SET_SVC, val);
        } else if (ctrl_id == VP9E_SET_KEY_FRAME_FILTERING) {
            int val = Data[56] % 2;
            vpx_codec_control(&ctx, VP9E_SET_KEY_FRAME_FILTERING, val);
        } else {
            // Try a generic control or invalid one using the internal function directly to avoid macro expansion issues with variable names
            vpx_codec_control_(&ctx, ctrl_id, 0);
        }
    }

    // Flush the stream (end of stream condition)
    // Call encode with NULL image
    vpx_codec_encode(&ctx, NULL, fuzz_pts + 1, 1, 0, 0);
    
    // Drain remaining packets
    while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
        // No explicit free needed for cx_pkt in this context
    }

    // Cleanup
    cleanup_encoder();

    return 0;
}