// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use input data to configure width/height if possible, with bounds checking
    uint16_t width = (Data[0] % 128) + 16;
    uint16_t height = (Data[1] % 128) + 16;
    cfg.g_w = width;
    cfg.g_h = height;
    // Fixed typo: g_time_base -> g_timebase
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_MATRIX_COEFFICIENTS
    // Fixed macro name: AOM_AV1E_SET_MATRIX_COEFFICIENTS -> AV1E_SET_MATRIX_COEFFICIENTS
    int matrix_coeff = (Size > 2) ? (int)Data[2] : 0;
    aom_codec_control(&ctx, AV1E_SET_MATRIX_COEFFICIENTS, matrix_coeff);

    // Fuzz AV1E_SET_PARTITION_INFO_PATH
    // Fixed macro name: AOM_AV1E_SET_PARTITION_INFO_PATH -> AV1E_SET_PARTITION_INFO_PATH
    char path[256];
    if (Size > 4) {
        size_t path_len = (Size > 255) ? 255 : Size;
        memcpy(path, Data + 4, path_len);
        path[path_len] = '\0';
    } else {
        strcpy(path, "./dummy_file");
    }
    // Write dummy data to file if needed for the API
    FILE *f = fopen(path, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
    aom_codec_control(&ctx, AV1E_SET_PARTITION_INFO_PATH, path);

    // Fuzz AV1E_SET_ENABLE_DIFF_WTD_COMP
    // Fixed macro name: AOM_AV1E_SET_ENABLE_DIFF_WTD_COMP -> AV1E_SET_ENABLE_DIFF_WTD_COMP
    int enable_diff_wtd = (Size > 5) ? (Data[5] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, enable_diff_wtd);

    // Fuzz AV1E_SET_FILM_GRAIN_TABLE
    // Fixed macro name: AOM_AV1E_SET_FILM_GRAIN_TABLE -> AV1E_SET_FILM_GRAIN_TABLE
    void *fg_table = (Size > 6) ? (void *)(Data + 6) : NULL;
    aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TABLE, fg_table);

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    // Fixed macro name: AOM_AV1E_SET_ENABLE_TX_SIZE_SEARCH -> AV1E_SET_ENABLE_TX_SIZE_SEARCH
    int enable_tx_size = (Size > 7) ? (Data[7] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_size);

    // Fuzz aom_codec_get_global_headers
    // Call after configuration
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        // Process the buffer if needed, but for fuzzing just ensure no crash
        // The caller must free the buffer
        free(global_headers->buf);
        free(global_headers);
    }

    cleanup_codec(&ctx);
    return 0;
}