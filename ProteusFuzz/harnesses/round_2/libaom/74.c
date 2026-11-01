// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control_typechecked_AOMD_GET_S_FRAME_INFO at aomdx.h:586:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_FRAME_FLAGS at aomdx.h:564:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_ALTREF_PRESENT at aomdx.h:567:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_LAST_REF_UPDATES at aomdx.h:483:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_LAST_REF_USED at aomdx.h:489:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_FRAME_CORRUPTED at aomdx.h:486:1 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

static int try_fuzz_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t err;
    aom_codec_iter_t iter = NULL;
    aom_image_t *img;
    aom_s_frame_info s_frame_info;
    int frame_flags;
    int altref_present;
    int last_ref_updates;
    int last_ref_used;
    int frame_corrupted;

    if (size < 1) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Feed data to decoder
    err = aom_codec_decode(&ctx, data, size, NULL);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Iterate over decoded frames
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // Fuzz AOMD_GET_S_FRAME_INFO
        aom_codec_control_typechecked_AOMD_GET_S_FRAME_INFO(&ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);

        // Fuzz AOMD_GET_FRAME_FLAGS
        aom_codec_control_typechecked_AOMD_GET_FRAME_FLAGS(&ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);

        // Fuzz AOMD_GET_ALTREF_PRESENT
        aom_codec_control_typechecked_AOMD_GET_ALTREF_PRESENT(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);

        // Fuzz AOMD_GET_LAST_REF_UPDATES
        aom_codec_control_typechecked_AOMD_GET_LAST_REF_UPDATES(&ctx, AOMD_GET_LAST_REF_UPDATES, &last_ref_updates);

        // Fuzz AOMD_GET_LAST_REF_USED
        aom_codec_control_typechecked_AOMD_GET_LAST_REF_USED(&ctx, AOMD_GET_LAST_REF_USED, &last_ref_used);

        // Fuzz AOMD_GET_FRAME_CORRUPTED
        aom_codec_control_typechecked_AOMD_GET_FRAME_CORRUPTED(&ctx, AOMD_GET_FRAME_CORRUPTED, &frame_corrupted);
    }

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Try decoding with the provided data
    try_fuzz_decoder(Data, Size);

    return 0;
}