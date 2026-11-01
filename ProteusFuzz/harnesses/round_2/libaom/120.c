// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzz data to tweak configuration if possible
    if (size > 0) {
        cfg.g_w = (data[0] % 128) + 16;
        cfg.g_h = (data[1] % 128) + 16;
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
        cfg.rc_target_bitrate = (data[4] % 1000) + 100;
        cfg.g_threads = (data[5] % 8) + 1;
        cfg.g_lag_in_frames = (data[6] % 20) + 1;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) return 0;

    // Attempt to create a dummy frame to encode
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 128, 128, 1);
    
    // Fill image with fuzz data
    // Corrected access: img.planes[plane_idx] is a pointer to the start of the plane data
    // We access it as img.planes[plane_idx][row * stride + col]
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            img.planes[0][i * img.stride[0] + j] = Data[(i * img.w + j) % Size];
        }
    }
    // Fill chroma planes
    for (int i = 0; i < img.h / 2; i++) {
        for (int j = 0; j < img.w / 2; j++) {
            img.planes[1][i * img.stride[1] + j] = Data[((i * (img.w / 2) + j) + Size) % Size];
            img.planes[2][i * img.stride[2] + j] = Data[((i * (img.w / 2) + j) + (Size * 2)) % Size];
        }
    }

    // Encode the frame
    // Corrected arguments: ctx, img, pts, duration, flags
    aom_codec_encode(ctx, &img, 0, 1, 0);

    // Try to get packets
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(ctx, &iter)) != NULL) {
        // Process packet (no-op for fuzzing)
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Free the frame if needed, though usually handled by codec
        }
    }

    // Attempt to invoke the specific control functions if they are accessible.
    // Since the signatures are unknown and they are likely internal, we cannot
    // safely call them directly without risking crashes.
    // However, if the library exports them, we might try to cast them.
    // This part is highly experimental and depends on the build configuration.
    
    // We will try to call aom_codec_control with known AV1 specific enums
    // that might map to the functionality described in the target names.
    // This is a best-effort approach given the lack of signatures.
    
    // We use the raw data to generate random control arguments
    int32_t arg_val = 0;
    if (Size > 0) arg_val = Data[0];
    if (Size > 1) arg_val |= (Data[1] << 8);
    if (Size > 2) arg_val |= (Data[2] << 16);
    if (Size > 3) arg_val |= (Data[3] << 24);

    // Try a few known AV1 encoder controls to exercise the control path
    // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE is a known enum in some versions
    // but might not be exposed. We try it anyway.
    // If the specific typechecked functions are not available, this is the closest
    // we can get to exercising the internal logic.
    
    // We cannot safely call aom_codec_control_typechecked_* directly without
    // knowing their exact signatures (arguments, return types).
    // Instead, we rely on the standard aom_codec_control interface.
    
    // Attempt to call standard controls that might trigger similar paths
    // We removed the undefined AOM_CODEC_GET_FRAME_COUNT.
    // We can try to set some generic controls if we know their IDs, but without
    // the specific enum values for the target functions, we skip specific calls
    // to avoid undefined behavior.
    
    // Cleanup
    aom_img_free(&img);
    cleanup_encoder_context(ctx);

    return 0;
}