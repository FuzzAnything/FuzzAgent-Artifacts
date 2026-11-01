// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9_GET_REFERENCE__value at vp8.h:126:1 in vp8.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8_SET_REFERENCE__value at vp8.h:120:1 in vp8.h
// VP8_COPY_REFERENCE__value at vp8.h:122:1 in vp8.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_codec.h>

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_svc_layer_id_t svc_layer_id;
    vpx_svc_layer_id_t svc_params;
    vpx_image_t img;
    vpx_ref_frame_t ref_frame;
    vp9_ref_frame_t vp9_ref_frame;
    int ret;
    const vpx_codec_iface_t *codec = vpx_codec_vp9_dx();

    if (vpx_codec_enc_config_default(codec, &cfg, 0) != 0) {
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;
    cfg.g_lag_in_frames = 0;

    if (vpx_codec_enc_init(&ctx, codec, &cfg, 0) != 0) {
        return 0;
    }

    // Initialize image
    img.w = cfg.g_w;
    img.h = cfg.g_h;
    img.fmt = VPX_IMG_FMT_I420;
    img.bit_depth = 8;
    img.d_w = img.w;
    img.d_h = img.h;
    img.r_w = img.w;
    img.r_h = img.h;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    img.bps = 8;
    img.user_priv = NULL;
    img.fb_priv = NULL;
    img.img_data = NULL;
    img.img_data_owner = 0;
    img.self_allocd = 0;

    // Allocate planes
    for (int i = 0; i < 4; i++) {
        img.planes[i] = NULL;
        img.stride[i] = 0;
    }

    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, img.w, img.h, 1) != 0) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzz data
    size_t plane_size = img.w * img.h;
    if (size >= plane_size) {
        memcpy(img.planes[VPX_PLANE_Y], data, plane_size);
    } else if (size > 0) {
        memset(img.planes[VPX_PLANE_Y], 0, plane_size);
        memcpy(img.planes[VPX_PLANE_Y], data, size);
    }

    // Fuzz VP9E_SET_SVC_LAYER_ID
    if (size >= 4) {
        svc_layer_id.spatial_layer_id = *(int32_t *)(data + 0);
        svc_layer_id.temporal_layer_id = *(int32_t *)(data + 4);
        for (int i = 0; i < VPX_SS_MAX_LAYERS && (size_t)(8 + i * 4) < size; i++) {
            svc_layer_id.temporal_layer_id_per_spatial[i] = *(int32_t *)(data + 8 + i * 4);
        }
        ret = vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &svc_layer_id);
        if (ret != 0) {
            // Ignore error, continue fuzzing
        }
    }

    // Fuzz VP9E_SET_SVC (if available, though signature is unclear, we try generic control)
    // Note: VP9E_SET_SVC might not exist as a direct control, but we attempt to pass data
    if (size >= 8) {
        svc_params.spatial_layer_id = *(int32_t *)(data + 8);
        svc_params.temporal_layer_id = *(int32_t *)(data + 12);
        for (int i = 0; i < VPX_SS_MAX_LAYERS && (size_t)(16 + i * 4) < size; i++) {
            svc_params.temporal_layer_id_per_spatial[i] = *(int32_t *)(data + 16 + i * 4);
        }
        // Attempt to set SVC parameters (assuming it exists or is similar to SET_SVC_LAYER_ID)
        // Since signature is empty, we try a generic approach or skip if undefined
        // In real libvpx, VP9E_SET_SVC might be a specific control, but here we assume it's not directly callable without more info.
        // We skip this specific call if the control is not defined, but we can try to pass the struct if the control exists.
        // For safety, we only call if we know the control exists. Since the prompt says signature is empty, we skip direct call.
        // However, to explore state, we might try to call with a dummy control if we knew the enum, but we don't.
        // So we skip VP9E_SET_SVC__value call as per instruction to avoid undefined behavior.
    }

    // Fuzz VP9_GET_REFERENCE
    if (size >= 8) {
        vp9_ref_frame.idx = *(int32_t *)(data + 16);
        vp9_ref_frame.img = img; // Reuse the image
        ret = vpx_codec_control(&ctx, VP9_GET_REFERENCE, &vp9_ref_frame);
        if (ret != 0) {
            // Ignore error
        }
    }

    // Fuzz VP8_COPY_REFERENCE and VP8_SET_REFERENCE
    // These are for VP8, so we need a VP8 context.
    vpx_codec_ctx_t ctx_vp8;
    vpx_codec_enc_cfg_t cfg_vp8;
    const vpx_codec_iface_t *codec_vp8 = vpx_codec_vp8_cx();

    if (vpx_codec_enc_config_default(codec_vp8, &cfg_vp8, 0) != 0) {
        vpx_img_free(&img);
        vpx_codec_destroy(&ctx);
        return 0;
    }

    cfg_vp8.g_w = 128;
    cfg_vp8.g_h = 128;
    cfg_vp8.g_timebase.num = 1;
    cfg_vp8.g_timebase.den = 30;
    cfg_vp8.rc_target_bitrate = 100;
    cfg_vp8.g_error_resilient = 1;
    cfg_vp8.g_lag_in_frames = 0;

    if (vpx_codec_enc_init(&ctx_vp8, codec_vp8, &cfg_vp8, 0) != 0) {
        vpx_img_free(&img);
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Prepare VP8 reference frame
    ref_frame.frame_type = (vpx_ref_frame_type_t)(size % 4); // Cycle through types
    ref_frame.img = img;

    // Fuzz VP8_SET_REFERENCE
    ret = vpx_codec_control(&ctx_vp8, VP8_SET_REFERENCE, &ref_frame);
    if (ret != 0) {
        // Ignore error
    }

    // Fuzz VP8_COPY_REFERENCE
    // We need another reference frame to copy from, but we can reuse the same struct
    // or create a new one. Here we reuse.
    ret = vpx_codec_control(&ctx_vp8, VP8_COPY_REFERENCE, &ref_frame);
    if (ret != 0) {
        // Ignore error
    }

    // Cleanup
    vpx_codec_destroy(&ctx);
    vpx_codec_destroy(&ctx_vp8);
    vpx_img_free(&img);

    return 0;
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Call the main fuzzing logic
    fuzz_vp9_encoder(Data, Size);

    return 0;
}