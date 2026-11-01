// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9_SET_BYTE_ALIGNMENT__value at vp8dx.h:206:1 in vp8dx.h
// VP8D_GET_LAST_REF_USED__value at vp8dx.h:194:1 in vp8dx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_TILE_ROWS__value at vp8cx.h:1026:1 in vp8cx.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_image.h"

// Forward declaration for VP9 encoder interface if not directly exposed in basic headers
// In standard libvpx builds, this is usually defined in vpx/vp9/vp9_encoder.h or similar.
// We assume the standard symbol name vpx_codec_vp9_cx is available or use the macro if defined.
// If the specific header is missing, we rely on the standard vpx_codec interface.

static int fuzzer_test_vp9_encoder(const uint8_t *data, size_t size) {
    if (size < 4) return 0;

    int tile_rows = data[0];
    int frame_parallel = data[1];
    int noise_sens = data[3];

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    const vpx_codec_iface_t *iface = NULL;

    // Try to get the VP9 encoder interface. 
    // Note: In some builds, vpx_codec_vp9_cx() is the function. 
    // If the header is missing, we might need to rely on string lookup or assume it's linked.
    // Standard libvpx provides vpx_codec_vp9_cx().
    iface = vpx_codec_vp9_cx();

    if (iface == NULL) {
        // Fallback or skip if VP9 encoder is not available in this build
        return 0;
    }

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Test VP9E_SET_TILE_ROWS
    // VP9E_SET_TILE_ROWS expects an int value
    if (vpx_codec_control(&ctx, VP9E_SET_TILE_ROWS, tile_rows) != VPX_CODEC_OK) {
        // Ignore error, continue to test other states
    }

    // Test VP9E_SET_FRAME_PARALLEL_DECODING
    // Expects an int (0 or 1)
    if (vpx_codec_control(&ctx, VP9E_SET_FRAME_PARALLEL_DECODING, frame_parallel) != VPX_CODEC_OK) {
        // Ignore error
    }

    // Test VP9E_SET_NOISE_SENSITIVITY
    // Expects an int
    if (vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, noise_sens) != VPX_CODEC_OK) {
        // Ignore error
    }

    // Note: VP9E_SET_SVC_PARAMETERS requires a complex struct (vpx_svc_layer_config_t) 
    // which might not be fully defined in the minimal headers provided or requires specific flags.
    // To avoid compilation errors due to missing struct definitions in the provided context,
    // we skip the SVC parameters test or use a generic pointer if the macro is known to accept void*.
    // However, since the struct definition was missing in the error log, we omit this specific call
    // to ensure the driver compiles, or we would need to include "vpx/vp9/vp9_encoder.h".
    // Assuming standard headers are present, we could try:
    // vpx_svc_layer_config_t svc_cfg; ... but since the type was unknown, we skip to be safe.

    vpx_codec_destroy(&ctx);
    return 0;
}

static int fuzzer_test_vp8_decoder(const uint8_t *data, size_t size) {
    if (size < 1) return 0;

    vpx_codec_ctx_t ctx;
    int byte_align = data[0];
    int last_ref_used;

    // Initialize VP8 Decoder
    if (vpx_codec_dec_init(&ctx, vpx_codec_vp8_dx(), NULL, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Test VP9_SET_BYTE_ALIGNMENT 
    // Note: This is a VP9 specific control. Calling it on a VP8 context might fail, 
    // which is a valid state to fuzz (error handling).
    if (vpx_codec_control(&ctx, VP9_SET_BYTE_ALIGNMENT, byte_align) != VPX_CODEC_OK) {
        // Expected to potentially fail on VP8 context, continue
    }

    // Test VP8D_GET_LAST_REF_USED
    // Expects a pointer to an int to store the result
    if (vpx_codec_control(&ctx, VP8D_GET_LAST_REF_USED, &last_ref_used) == VPX_CODEC_OK) {
        // Successfully retrieved value
        (void)last_ref_used;
    }

    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Randomly choose between encoder and decoder tests based on first byte
    if (Data[0] & 1) {
        fuzzer_test_vp9_encoder(Data, Size);
    } else {
        fuzzer_test_vp8_decoder(Data, Size);
    }

    return 0;
}