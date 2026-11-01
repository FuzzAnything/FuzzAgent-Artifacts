// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    return 0;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 256) + 1;
    cfg.g_timebase.den = (Data[3] % 256) + 1;
    cfg.g_threads = (Data[4] % 8) + 1;
    cfg.g_lag_in_frames = (Data[5] % 256);
    cfg.rc_target_bitrate = (Data[6] * 256 + Data[7]) + 100;
    cfg.g_pass = AOM_RC_ONE_PASS;

    aom_codec_err_t init_ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (init_ret != AOM_CODEC_OK) return 0;

    // Extract control flags from input data
    int enable_palette = (Data[8] % 2);
    int enable_interintra_wedge = (Data[9] % 2);
    int enable_smooth_interintra = (Data[10] % 2);
    int enable_rect_tx = (Data[11] % 2);
    int enable_ab_partitions = (Data[12] % 2);
    int enable_rect_partitions = (Data[13] % 2);

    // Invoke control functions with extracted values using correct enum names
    fuzz_encoder_controls(&ctx, AV1E_SET_ENABLE_PALETTE, enable_palette);
    fuzz_encoder_controls(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, enable_interintra_wedge);
    fuzz_encoder_controls(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, enable_smooth_interintra);
    fuzz_encoder_controls(&ctx, AV1E_SET_ENABLE_RECT_TX, enable_rect_tx);
    fuzz_encoder_controls(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, enable_ab_partitions);
    fuzz_encoder_controls(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, enable_rect_partitions);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}