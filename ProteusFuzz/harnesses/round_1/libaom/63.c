// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
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

// Helper to safely initialize encoder context
static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return -1;
    
    // Set minimal valid config to ensure initialization succeeds
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->rc_min_quantizer = 32;
    cfg->rc_max_quantizer = 32;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return -1;
    return 0;
}

// Helper to safely initialize decoder context
static int init_decoder(aom_codec_ctx_t *ctx, aom_codec_dec_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return -1;

    // Initialize config manually since aom_codec_dec_config_default might not be available in all versions
    memset(cfg, 0, sizeof(*cfg));
    cfg->threads = 1;

    if (aom_codec_dec_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return -1;
    return 0;
}

// Helper to create a dummy file if needed
static void create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

// Helper to ensure data is null-terminated for string operations
static char* ensure_null_terminated(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    
    // Check if already null-terminated
    for (size_t i = 0; i < size; i++) {
        if (data[i] == 0) return (char*)data;
    }
    
    // Allocate and copy with null terminator
    char *buf = (char*)malloc(size + 1);
    if (!buf) return NULL;
    memcpy(buf, data, size);
    buf[size] = '\0';
    return buf;
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create dummy file for any file-based operations
    create_dummy_file("./dummy_file", Data, Size);

    // Try Encoder APIs
    aom_codec_ctx_t enc_ctx = {0};
    aom_codec_enc_cfg_t enc_cfg = {0};

    if (init_encoder(&enc_ctx, &enc_cfg) == 0) {
        // Fuzz AV1E_SET_ENABLE_WARPED_MOTION
        int val_warp = Data[0] & 0xFF;
        aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_WARPED_MOTION, val_warp);

        // Fuzz AV1E_SET_FILM_GRAIN_TABLE
        // Ensure null-termination to prevent heap-buffer-overflow in strlen
        char *safe_data = ensure_null_terminated(Data, Size);
        if (safe_data) {
            aom_codec_control(&enc_ctx, AV1E_SET_FILM_GRAIN_TABLE, safe_data);
            if (safe_data != (char*)Data) free(safe_data);
        }

        // Fuzz AV1E_SET_ENABLE_CFL_INTRA
        int val_cfl = (Size > 1) ? (Data[1] & 0xFF) : 0;
        aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_CFL_INTRA, val_cfl);

        // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
        // Ensure null-termination for string operations
        safe_data = ensure_null_terminated(Data, Size);
        if (safe_data) {
            aom_codec_control(&enc_ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, safe_data);
            if (safe_data != (char*)Data) free(safe_data);
        }

        // Fuzz AV1E_SET_LOOPFILTER_CONTROL
        // Ensure null-termination for string operations
        safe_data = ensure_null_terminated(Data, Size);
        if (safe_data) {
            aom_codec_control(&enc_ctx, AV1E_SET_LOOPFILTER_CONTROL, safe_data);
            if (safe_data != (char*)Data) free(safe_data);
        }

        aom_codec_destroy(&enc_ctx);
    }

    // Try Decoder APIs
    aom_codec_ctx_t dec_ctx = {0};
    aom_codec_dec_cfg_t dec_cfg = {0};

    if (init_decoder(&dec_ctx, &dec_cfg) == 0) {
        // Fuzz AV1D_SET_SKIP_FILM_GRAIN
        int val_skip = (Size > 0) ? (Data[Size - 1] & 0xFF) : 0;
        aom_codec_control(&dec_ctx, AV1D_SET_SKIP_FILM_GRAIN, val_skip);

        aom_codec_destroy(&dec_ctx);
    }

    return 0;
}