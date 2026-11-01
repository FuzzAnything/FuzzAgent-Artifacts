// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    // AV1E_SET_MATRIX_COEFFICIENTS
    // Type: aom_matrix_coefficients_t (enum)
    if (size >= sizeof(aom_matrix_coefficients_t)) {
        aom_matrix_coefficients_t coeffs;
        memcpy(&coeffs, data, sizeof(coeffs));
        aom_codec_control(ctx, AV1E_SET_MATRIX_COEFFICIENTS, &coeffs);
    }

    // AV1E_SET_PARTITION_INFO_PATH
    // Type: const char *
    if (size > 0 && size < 256) {
        char path[256];
        memcpy(path, data, size);
        path[size] = '\0';
        aom_codec_control(ctx, AV1E_SET_PARTITION_INFO_PATH, path);
    }

    // AV1E_SET_ENABLE_ONESIDED_COMP
    // Type: int
    if (size >= sizeof(int)) {
        int val;
        memcpy(&val, data, sizeof(val));
        aom_codec_control(ctx, AV1E_SET_ENABLE_ONESIDED_COMP, &val);
    }

    // AV1E_SET_LOOPFILTER_CONTROL
    // Skipping due to lack of public struct definition in provided headers
    // to ensure compilation.

    // AV1E_SET_FILM_GRAIN_TABLE
    // Skipping due to lack of 'aom_film_grain_params_t' definition in provided headers
    // to ensure compilation.

    return 1;
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Test aom_codec_get_global_headers
    aom_fixed_buf_t *headers = aom_codec_get_global_headers(&ctx);
    if (headers) {
        if (headers->buf) {
            free(headers->buf);
        }
        free(headers);
    }

    cleanup_codec(&ctx);
    return 0;
}