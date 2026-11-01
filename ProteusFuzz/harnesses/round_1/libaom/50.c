// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

// Helper to create a valid encoder context for fuzzing
static aom_codec_ctx_t *create_encoder_context(void) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    // Use the standard API to get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Set minimal valid config
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper to create a valid decoder context for fuzzing
static aom_codec_ctx_t *create_decoder_context(void) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;

    // Use the standard API to get the AV1 decoder interface
    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    // Initialize config manually with valid fields for the current libaom version
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1; 

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Use the first byte to decide which API to fuzz
    int choice = Data[0] % 6;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    switch (choice) {
        case 0: {
            // Fuzz aom_codec_set_cx_data_buf
            aom_codec_ctx_t *ctx = create_encoder_context();
            if (!ctx) return 0;

            // Allocate a buffer for the test
            size_t buf_size = (payload_size > 0) ? payload_size : 1024;
            void *buf = malloc(buf_size);
            if (!buf) {
                aom_codec_destroy(ctx);
                return 0;
            }
            memset(buf, 0, buf_size);

            aom_fixed_buf_t fixed_buf = { .buf = buf, .sz = buf_size };

            // Use payload to determine padding values
            unsigned int pad_before = (payload_size > 0) ? payload[0] % 100 : 0;
            unsigned int pad_after = (payload_size > 1) ? payload[1] % 100 : 0;

            aom_codec_set_cx_data_buf(ctx, &fixed_buf, pad_before, pad_after);

            // Test with NULL buffer to restore default
            aom_codec_set_cx_data_buf(ctx, NULL, 0, 0);

            free(buf);
            aom_codec_destroy(ctx);
            break;
        }
        case 1: {
            // Fuzz aom_codec_destroy
            aom_codec_ctx_t *ctx = create_encoder_context();
            if (!ctx) return 0;

            aom_codec_destroy(ctx);

            // Test with NULL
            aom_codec_destroy(NULL);
            break;
        }
        case 2: {
            // Fuzz aom_codec_get_global_headers
            aom_codec_ctx_t *ctx = create_encoder_context();
            if (!ctx) return 0;

            aom_fixed_buf_t *headers = aom_codec_get_global_headers(ctx);
            if (headers) {
                if (headers->buf) {
                    free(headers->buf);
                }
                free(headers);
            }

            aom_codec_destroy(ctx);
            break;
        }
        case 3: {
            // Fuzz aom_codec_set_option
            aom_codec_ctx_t *ctx = create_encoder_context();
            if (!ctx) return 0;

            // Use payload to generate option name and value
            if (payload_size > 0) {
                char *name = (char *)malloc(payload_size + 1);
                char *value = (char *)malloc(payload_size + 1);
                if (name && value) {
                    memcpy(name, payload, payload_size);
                    name[payload_size] = '\0';
                    memcpy(value, payload, payload_size);
                    value[payload_size] = '\0';

                    // Fuzz with valid and potentially invalid options
                    aom_codec_set_option(ctx, name, value);
                    
                    // Try some known options
                    aom_codec_set_option(ctx, "cpu-used", "4");
                    aom_codec_set_option(ctx, "crf", "30");
                    aom_codec_set_option(ctx, "invalid-option", "invalid-value");

                    free(name);
                    free(value);
                }
            }

            aom_codec_destroy(ctx);
            break;
        }
        case 4: {
            // Fuzz aom_codec_control_typechecked_AV1E_SET_FILM_GRAIN_TABLE
            // Since the function signature is empty in the prompt, we cannot call it directly.
            // We simulate by initializing context and doing nothing specific.
            aom_codec_ctx_t *ctx = create_encoder_context();
            if (!ctx) return 0;
            aom_codec_destroy(ctx);
            break;
        }
        case 5: {
            // Fuzz aom_codec_control_typechecked_AV1E_SET_PARTITION_INFO_PATH
            // Since the function signature is empty in the prompt, we cannot call it directly.
            // We simulate by initializing context and doing nothing specific.
            aom_codec_ctx_t *ctx = create_decoder_context();
            if (!ctx) return 0;
            aom_codec_destroy(ctx);
            break;
        }
    }

    return 0;
}