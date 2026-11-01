// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f && size > 0) {
        fwrite(data, 1, size, f);
        fclose(f);
    } else if (f) {
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_186(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input to dummy file if needed
    write_dummy_file(Data, Size);

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Set default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize codec
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz various control functions with randomized values derived from input
    // Ensure Size is sufficient before accessing indices
    uint8_t val = (Size > 0) ? (Data[0] % 2) : 0;
    uint8_t val2 = (Size > 1) ? (Data[1] % 2) : 0;
    uint8_t val3 = (Size > 2) ? (Data[2] % 3) : 0; // For chroma sample position (0, 1, 2)
    uint8_t val4 = (Size > 3) ? (Data[3] % 2) : 0;
    uint8_t val5 = (Size > 4) ? (Data[4] % 256) : 0; // For transfer characteristics
    uint8_t val6 = (Size > 5) ? (Data[5] % 2) : 0;

    // AV1E_SET_ENABLE_INTRABC
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val) != AOM_CODEC_OK) {
        // Ignore errors, continue fuzzing
    }

    // AV1E_SET_ENABLE_TX_SIZE_SEARCH
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val2) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_CHROMA_SAMPLE_POSITION
    if (aom_codec_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, val3) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_ENABLE_RECT_PARTITIONS
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val4) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_TRANSFER_CHARACTERISTICS
    if (aom_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val5) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_ENABLE_RECT_TX
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, val6) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}