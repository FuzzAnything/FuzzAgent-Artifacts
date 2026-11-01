// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int try_decode_and_query(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_err_t res;
    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;

    if (!ctx || !data || size == 0) return 0;

    // Initialize decoder config with defaults
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Feed data in chunks to trigger various states
    size_t offset = 0;
    while (offset < size) {
        size_t chunk = (size - offset > 1024) ? 1024 : (size - offset);
        const uint8_t *chunk_data = data + offset;

        res = aom_codec_decode(ctx, chunk_data, chunk, NULL);
        if (res == AOM_CODEC_ERROR) {
            // Continue trying to feed more data even after error to explore states
        }

        // Iterate over decoded frames
        while ((pkt = aom_codec_get_frame(ctx, &iter)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Correctly cast the union member to aom_image_t*
                aom_image_t *img = (aom_image_t *)pkt->data.frame.buf;
                if (img) {
                    // Query 1: AOMD_GET_ALTREF_PRESENT
                    int altref_present = 0;
                    aom_codec_control(ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);

                    // Query 2: AOMD_GET_S_FRAME_INFO
                    aom_s_frame_info s_frame_info = {0};
                    aom_codec_control(ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);

                    // Query 3: AOMD_GET_STILL_PICTURE
                    aom_still_picture_info still_info = {0};
                    aom_codec_control(ctx, AOMD_GET_STILL_PICTURE, &still_info);

                    // Query 4: AOMD_GET_FWD_KF_PRESENT
                    int fwd_kf_present = 0;
                    aom_codec_control(ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);

                    // Query 5: AOMD_GET_FRAME_FLAGS
                    int frame_flags = 0;
                    aom_codec_control(ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);

                    // Query 6: AOMD_GET_ORDER_HINT
                    int order_hint = 0;
                    aom_codec_control(ctx, AOMD_GET_ORDER_HINT, &order_hint);
                }
            }
        }
        offset += chunk;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_191(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file if needed (though not strictly used in this driver)
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Allocate codec context
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;

    // Attempt to decode and query
    try_decode_and_query(ctx, Data, Size);

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}