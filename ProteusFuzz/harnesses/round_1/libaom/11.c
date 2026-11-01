// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size);
}

static int read_dummy_file(uint8_t *data, size_t max_size, size_t *out_size) {
    FILE *f = fopen("./dummy_file", "rb");
    if (!f) {
        *out_size = 0;
        return 0;
    }
    *out_size = fread(data, 1, max_size, f);
    fclose(f);
    return (*out_size > 0);
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input to dummy file for potential file-based decoding paths
    if (!write_dummy_file(Data, Size)) {
        return 0;
    }

    // Try to decode from file
    aom_codec_ctx_t ctx = {0};
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Read data back for decoding
    uint8_t buffer[4096];
    size_t buf_size = 0;
    if (!read_dummy_file(buffer, sizeof(buffer), &buf_size)) {
        cleanup_codec(&ctx);
        return 0;
    }

    aom_codec_iter_t iter = NULL;
    const aom_image_t *frame = NULL;

    // Feed data
    aom_codec_err_t res = aom_codec_decode(&ctx, buffer, buf_size, &iter);
    if (res == AOM_CODEC_OK) {
        while ((frame = aom_codec_get_frame(&ctx, &iter)) != NULL) {
            // Process frame if needed
            // No need to free frame as it is managed by the codec context
        }
    }

    // Fuzz aom_codec_control with valid control IDs defined in aomdx.h
    int control_ids[] = {
        AOMD_GET_LAST_REF_USED,
        AOMD_GET_FWD_KF_PRESENT,
        AOMD_GET_FRAME_FLAGS,
        AOMD_GET_LAST_REF_UPDATES,
        AOMD_GET_ALTREF_PRESENT
    };
    int num_ids = sizeof(control_ids) / sizeof(control_ids[0]);

    for (int i = 0; i < num_ids; i++) {
        int ctrl_id = control_ids[i];
        if (ctrl_id == 0) continue;

        // Try with different argument types based on expected usage
        int dummy_int = 0;
        int64_t dummy_int64 = 0;
        aom_codec_err_t err = AOM_CODEC_ERROR;

        // Try passing int
        err = aom_codec_control(&ctx, ctrl_id, dummy_int);
        (void)err;

        // Try passing int64
        err = aom_codec_control(&ctx, ctrl_id, dummy_int64);
        (void)err;

        // Try passing pointer
        err = aom_codec_control(&ctx, ctrl_id, &dummy_int);
        (void)err;

        // Try passing NULL (should return INVALID_PARAM)
        err = aom_codec_control(&ctx, ctrl_id, NULL);
        (void)err;

        // Try with zero control ID (should return INVALID_PARAM)
        err = aom_codec_control(&ctx, 0, &dummy_int);
        (void)err;
    }

    // Cleanup
    cleanup_codec(&ctx);
    return 0;
}