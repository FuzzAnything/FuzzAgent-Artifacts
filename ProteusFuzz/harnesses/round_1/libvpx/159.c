// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;
    int ctrl_id;
    int value;

    // Initialize encoder configuration
    if (vpx_codec_enc_config_default(&vpx_codec_vp9_cx, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Initialize encoder context
    if (vpx_codec_enc_init(&ctx, &vpx_codec_vp9_cx, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Parse control ID and value from input data
    ctrl_id = Data[0];
    if (Size > 1) {
        value = Data[1];
        if (Size > 2) value |= (Data[2] << 8);
        if (Size > 3) value |= (Data[3] << 16);
        if (Size > 4) value |= (Data[4] << 24);
    }

    // Use vpx_codec_control_ to bypass the macro expansion issue with variable IDs
    // This allows passing the ID and value directly as arguments
    err = vpx_codec_control_(&ctx, ctrl_id, value);
    (void)err; // Suppress unused variable warning

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}