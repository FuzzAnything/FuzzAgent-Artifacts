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

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int *value) {
    if (!ctx) return -1;
    return aom_codec_control(ctx, control_id, *value);
}

int LLVMFuzzerTestOneInput_334(const uint8_t *Data, size_t Size) {
    if (Size < 13) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    cfg.g_threads = (Data[5] % 8) + 1;
    cfg.g_profile = Data[6] % 4;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control values from input data
    int qm_max = (Data[7] % 256);
    int noise_sensitivity = (Data[8] % 256);
    int superblock_size = (Data[9] % 4); // Assuming valid range 0-3
    int enable_intrabc = (Data[10] % 2);
    int enable_paeth_intra = (Data[11] % 2);
    int chroma_subsampling_y = (Data[12] % 2);

    // Invoke target API functions with corrected identifiers
    fuzz_codec_control(&ctx, AV1E_SET_QM_MAX, &qm_max);
    fuzz_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, &noise_sensitivity);
    fuzz_codec_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, &superblock_size);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, &enable_intrabc);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, &enable_paeth_intra);
    fuzz_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, &chroma_subsampling_y);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}