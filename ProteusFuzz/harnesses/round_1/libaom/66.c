// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int initialize_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure encoder settings
    if (size > 0) cfg->g_w = data[0] ? data[0] : 64;
    if (size > 1) cfg->g_h = data[1] ? data[1] : 64;
    if (size > 2) cfg->g_timebase.num = data[2] ? data[2] : 1;
    if (size > 3) cfg->g_timebase.den = data[3] ? data[3] : 30;
    if (size > 4) cfg->g_threads = data[4] % 16;
    if (size > 5) cfg->g_lag_in_frames = data[5] % 25;
    if (size > 6) cfg->rc_target_bitrate = (data[6] * 1000) + 1000;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

static int try_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t ret;
    ret = aom_codec_control(ctx, control_id, value);
    // Ignore return value for fuzzing, just ensure no crash
    return 0;
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int initialized = 0;

    if (Size < 7) return 0; // Need enough data to set basic config

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (initialize_encoder(&ctx, &cfg, Data, Size)) {
        initialized = 1;
    }

    if (initialized) {
        // Extract values from fuzz data to set various controls
        int skip_postproc = (Size > 7) ? (Data[7] % 2) : 0;
        int enable_restoration = (Size > 8) ? (Data[8] % 2) : 0;
        int row_mt = (Size > 9) ? (Data[9] % 2) : 0;
        int tile_rows = (Size > 10) ? (Data[10] % 8) : 0;
        int enable_intrabc = (Size > 11) ? (Data[11] % 2) : 0;
        int chroma_pos = (Size > 12) ? (Data[12] % 3) : 0;

        // Map to actual control IDs based on libaom documentation
        try_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, skip_postproc);
        try_control(&ctx, AV1E_SET_ENABLE_RESTORATION, enable_restoration);
        try_control(&ctx, AV1E_SET_ROW_MT, row_mt);
        try_control(&ctx, AV1E_SET_TILE_ROWS, tile_rows);
        try_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
        try_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, chroma_pos);

        // Attempt to encode a dummy frame to exercise the state
        aom_image_t img;
        aom_codec_cx_pkt_t *pkt;
        aom_codec_err_t ret;

        memset(&img, 0, sizeof(img));
        img.w = cfg.g_w;
        img.h = cfg.g_h;
        img.bit_depth = 8;
        img.stride[0] = img.w;
        img.stride[1] = (img.w + 1) / 2;
        img.stride[2] = (img.w + 1) / 2;
        
        // Allocate planes manually
        size_t y_size = img.w * img.h;
        size_t uv_size = (img.w * img.h) / 4;
        
        img.planes[0] = (uint8_t *)malloc(y_size);
        img.planes[1] = (uint8_t *)malloc(uv_size);
        img.planes[2] = (uint8_t *)malloc(uv_size);

        if (img.planes[0] && img.planes[1] && img.planes[2]) {
            // Fill with fuzz data
            memset(img.planes[0], Data[0], y_size);
            memset(img.planes[1], Data[1], uv_size);
            memset(img.planes[2], Data[2], uv_size);

            // Set up frame info for encoding
            // aom_codec_encode signature: aom_codec_encode(ctx, img, pts, duration, flags)
            ret = aom_codec_encode(&ctx, &img, 0, 1, 0);
            if (ret == AOM_CODEC_OK) {
                while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                        // Do nothing with output, just consume
                    }
                    // No aom_codec_pkt_destroy function; the packet data is owned by the codec context
                    // or is a pointer to internal memory. We just loop.
                }
            }

            free(img.planes[0]);
            free(img.planes[1]);
            free(img.planes[2]);
        }

        cleanup_encoder(&ctx);
    }

    return 0;
}