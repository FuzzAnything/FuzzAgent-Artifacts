// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>

static int create_test_image(const uint8_t *data, size_t size, vpx_image_t *img) {
    if (size < 10) return 0;

    unsigned int w = data[0] % 256;
    unsigned int h = data[1] % 256;
    if (w == 0) w = 16;
    if (h == 0) h = 16;
    if (w > 4096) w = 4096;
    if (h > 4096) h = 4096;

    if (vpx_img_alloc(img, VPX_IMG_FMT_I420, w, h, 1) != VPX_CODEC_OK) {
        return 0;
    }

    size_t plane_size = (size_t)img->stride[0] * img->h;
    size_t uv_size = (size_t)img->stride[1] * (img->h / 2);
    size_t total_needed = plane_size + 2 * uv_size;
    size_t copy_size = (size < total_needed) ? size : total_needed;

    memcpy(img->planes[0], data + 2, (size < plane_size + 2) ? size - 2 : plane_size);
    if (size > plane_size + 2) {
        memcpy(img->planes[1], data + 2 + plane_size, (size < plane_size + 2 + uv_size) ? size - 2 - plane_size : uv_size);
    }
    if (size > plane_size + 2 + uv_size) {
        memcpy(img->planes[2], data + 2 + plane_size + uv_size, (size < plane_size + 2 + 2 * uv_size) ? size - 2 - plane_size - uv_size : uv_size);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    vpx_image_t img;
    int init_success = 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    memset(&img, 0, sizeof(img));

    // Use vpx_codec_vp9_cx as the interface provider
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return 0;

    // Fuzz configuration parameters
    cfg.g_w = Data[0] % 256;
    if (cfg.g_w == 0) cfg.g_w = 16;
    cfg.g_h = Data[1] % 256;
    if (cfg.g_h == 0) cfg.g_h = 16;
    cfg.g_threads = (Size > 2) ? Data[2] % 8 : 1;
    cfg.g_profile = (Size > 3) ? Data[3] % 4 : 0;
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = VPX_BITS_8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Size > 4) ? (Data[4] % 60) + 1 : 30;
    cfg.rc_end_usage = (Size > 5) ? (Data[5] % 4) : VPX_VBR;
    cfg.rc_target_bitrate = (Size > 6) ? (Data[6] % 1000) + 100 : 1000;
    cfg.kf_mode = VPX_KF_AUTO;
    cfg.kf_min_dist = 12;
    cfg.kf_max_dist = 120;

    res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res == VPX_CODEC_OK) {
        init_success = 1;

        // Test vpx_codec_control_ with various control IDs
        // VP9E_SET_SVC and VP9E_SET_ROI_MAP are typically handled via vpx_codec_control
        // We attempt to trigger these paths if possible, though specific values might be ignored if not supported
        if (Size > 10) {
            vpx_roi_map_t roi_map;
            memset(&roi_map, 0, sizeof(roi_map));
            roi_map.enabled = Data[10] % 2;
            // Allocate a small dummy map if enabled
            if (roi_map.enabled && Size > 20) {
                roi_map.rows = Data[11] % 16;
                roi_map.cols = Data[12] % 16;
                if (roi_map.rows > 0 && roi_map.cols > 0) {
                    roi_map.roi_map = malloc(roi_map.rows * roi_map.cols);
                    if (roi_map.roi_map) {
                        memcpy(roi_map.roi_map, Data + 13, roi_map.rows * roi_map.cols);
                        // Attempt to set ROI map
                        vpx_codec_control_(&ctx, VP9E_SET_ROI_MAP, &roi_map);
                        free(roi_map.roi_map);
                    }
                }
            }
            // Attempt to set SVC (Scalable Video Coding)
            // The exact structure for VP9E_SET_SVC varies, often just a flag or layer config
            // We pass a dummy integer or pointer based on fuzz data
            int svc_val = (Size > 14) ? Data[14] : 0;
            vpx_codec_control_(&ctx, VP9E_SET_SVC, &svc_val);
        }

        // Create image and encode
        if (create_test_image(Data, Size, &img)) {
            vpx_codec_pts_t pts = 0;
            unsigned long duration = (Size > 15) ? Data[15] % 100 + 1 : 1;
            vpx_enc_frame_flags_t flags = 0;
            vpx_enc_deadline_t deadline = 0; // Infinite deadline for fuzzing stability

            // Encode a few frames to exercise state
            for (int i = 0; i < 3; i++) {
                res = vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
                if (res != VPX_CODEC_OK) break;

                // Iterate over output packets
                while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
                    // Consume packet data to prevent leaks in some implementations
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        // Just access to ensure validity, no need to free here as it's internal
                        (void)pkt->data.frame.buf;
                        (void)pkt->data.frame.sz;
                    }
                }
                pts += duration;
            }

            // Flush the encoder
            res = vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline);
            if (res == VPX_CODEC_OK) {
                while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
                    (void)pkt;
                }
            }
        }

        // Cleanup
        vpx_codec_destroy(&ctx);
    }

    // Cleanup image if allocated
    if (img.planes[0]) {
        vpx_img_free(&img);
    }

    return 0;
}