// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static aom_codec_ctx_t codec_ctx;
static aom_codec_enc_cfg_t cfg;
static int codec_initialized = 0;

static int init_codec(const uint8_t *data, size_t size) {
    if (codec_initialized) {
        return 0;
    }

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    // Use fuzzing data to tweak config slightly if available
    if (size >= 4) {
        cfg.g_w = (data[0] % 1024) + 16;
        cfg.g_h = (data[1] % 1024) + 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (data[2] % 60) + 1;
        cfg.rc_target_bitrate = (data[3] % 1000) + 100;
    } else {
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 500;
    }

    if (aom_codec_enc_init(&codec_ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    codec_initialized = 1;
    return 0;
}

static void cleanup_codec(void) {
    if (codec_initialized) {
        aom_codec_destroy(&codec_ctx);
        codec_initialized = 0;
    }
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize codec if not already done
    if (init_codec(Data, Size) != 0) {
        return 0;
    }

    // Fuzz AV1E_SET_INTER_DCT_ONLY
    int inter_dct_only = (Size > 0) ? (Data[0] % 2) : 0;
    aom_codec_control(&codec_ctx, AV1E_SET_INTER_DCT_ONLY, inter_dct_only);

    // Fuzz AV1E_SET_REDUCED_TX_TYPE_SET
    int reduced_tx_type = (Size > 1) ? (Data[1] % 2) : 0;
    aom_codec_control(&codec_ctx, AV1E_SET_REDUCED_TX_TYPE_SET, reduced_tx_type);

    // Fuzz AV1E_SET_ENABLE_INTERINTER_WEDGE
    int enable_wedge = (Size > 2) ? (Data[2] % 2) : 0;
    aom_codec_control(&codec_ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, enable_wedge);

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE
    int min_partition_size = (Size > 3) ? ((Data[3] % 256) + 1) : 4;
    aom_codec_control(&codec_ctx, AV1E_SET_MIN_PARTITION_SIZE, min_partition_size);

    // Fuzz AV1E_SET_ENABLE_AB_PARTITIONS
    int enable_ab_partitions = (Size > 4) ? (Data[4] % 2) : 0;
    aom_codec_control(&codec_ctx, AV1E_SET_ENABLE_AB_PARTITIONS, enable_ab_partitions);

    // Fuzz AV1E_SET_SVC_REF_FRAME_CONFIG
    if (Size >= 20) {
        aom_svc_ref_frame_config_t svc_config;
        memset(&svc_config, 0, sizeof(svc_config));

        for (int i = 0; i < 7; i++) {
            svc_config.reference[i] = (Data[5 + i] % 2);
            svc_config.ref_idx[i] = (Data[12 + i] % 8);
        }
        for (int i = 0; i < 8; i++) {
            svc_config.refresh[i] = (Data[19 + i] % 2);
        }

        aom_codec_control(&codec_ctx, AV1E_SET_SVC_REF_FRAME_CONFIG, &svc_config);
    }

    // Cleanup
    cleanup_codec();

    return 0;
}