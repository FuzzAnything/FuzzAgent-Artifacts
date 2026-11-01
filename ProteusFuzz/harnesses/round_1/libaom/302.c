// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

// Helper to safely initialize the encoder context
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to configure encoder settings safely
    cfg.g_w = size > 0 ? (data[0] % 1024 + 16) : 16;
    cfg.g_h = size > 1 ? (data[1] % 1024 + 16) : 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = size > 2 ? (data[2] * 1000) : 1000;
    cfg.g_threads = 1;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper to cleanup encoder context
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_302(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // Fuzz AV1E_SET_MAX_PARTITION_SIZE
    int max_partition_size = (Size > 0) ? (int)(Data[0] % 16) : 0;
    aom_codec_control(ctx, AV1E_SET_MAX_PARTITION_SIZE, max_partition_size);

    // Fuzz AV1E_SET_TIMING_INFO_TYPE
    int timing_info_type = (Size > 1) ? (int)(Data[1] % 3) : 0;
    aom_codec_control(ctx, AV1E_SET_TIMING_INFO_TYPE, timing_info_type);

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTRA
    int enable_smooth_intra = (Size > 2) ? (Data[2] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, enable_smooth_intra);

    // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // This control expects a pointer to a struct, but we can pass a small buffer as a test vector
    if (Size > 4) {
        int test_vector_len = Data[3] % 100;
        // Ensure we don't read past the end of the input buffer
        if (Size - 4 < (size_t)test_vector_len) {
            test_vector_len = (int)(Size - 4);
        }
        
        uint8_t *test_vector = (uint8_t *)malloc(test_vector_len + 1);
        if (test_vector && test_vector_len > 0) {
            memcpy(test_vector, Data + 4, test_vector_len);
            test_vector[test_vector_len] = 0;
            aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, test_vector);
            free(test_vector);
        } else if (test_vector) {
            // Handle case where length is 0
            test_vector[0] = 0;
            aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, test_vector);
            free(test_vector);
        }
    }

    // Fuzz AV1E_SET_POSTENCODE_DROP_RTC
    int drop_rtc = (Size > 4) ? (Data[4] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_POSTENCODE_DROP_RTC, drop_rtc);

    // Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
    int intra_default_tx_only = (Size > 5) ? (Data[5] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, intra_default_tx_only);

    // Cleanup
    cleanup_encoder(ctx);

    return 0;
}