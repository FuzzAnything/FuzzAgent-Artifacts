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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

static int invoke_codec_control(aom_codec_ctx_t *ctx, int control_id, void *arg) {
    if (!ctx) return 0;
    aom_codec_control(ctx, control_id, arg);
    return 1;
}

int LLVMFuzzerTestOneInput_313(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    write_dummy_file(Data, Size);

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1;
    cfg.rc_target_bitrate = (Data[3] * 256 + Data[4]) * 1000;
    cfg.rc_min_quantizer = Data[5] % 64;
    cfg.rc_max_quantizer = Data[6] % 64;
    cfg.g_error_resilient = (Data[7] % 3);
    cfg.g_lag_in_frames = (Data[8] % 20);
    cfg.g_threads = (Data[9] % 4) + 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // 1. AV1E_SET_EXTERNAL_RATE_CONTROL
    aom_rc_funcs_t rc_funcs;
    memset(&rc_funcs, 0, sizeof(rc_funcs));
    rc_funcs.rc_type = (aom_rc_type_t)(Data[10] % 7);
    // AV1E_SET_EXTERNAL_RATE_CONTROL is defined as an integer constant in aomcx.h
    invoke_codec_control(&ctx, AV1E_SET_EXTERNAL_RATE_CONTROL, &rc_funcs);

    // 2. AV1E_SET_QM_U
    int qm_u = Data[11];
    invoke_codec_control(&ctx, AV1E_SET_QM_U, &qm_u);

    // 3. AV1E_SET_ENABLE_TX_SIZE_SEARCH
    int enable_tx = Data[12] % 2;
    invoke_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, &enable_tx);

    // 4. AV1E_SET_ENABLE_INTRABC
    int enable_intrabc = Data[13] % 2;
    invoke_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, &enable_intrabc);

    // 5. AV1E_SET_ENABLE_CHROMA_DELTAQ
    int enable_chroma_dq = Data[14] % 2;
    invoke_codec_control(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, &enable_chroma_dq);

    // 6. AOME_SET_TUNING
    int tuning = Data[15] % 4;
    invoke_codec_control(&ctx, AOME_SET_TUNING, &tuning);

    aom_codec_destroy(&ctx);

    return 0;
}