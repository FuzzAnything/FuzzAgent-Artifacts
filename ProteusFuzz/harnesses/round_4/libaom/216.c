// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_216(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Fixed: allow_highbitdepth does not exist, removed.
    // cfg.allow_highbitdepth = 1; 

    if (aom_codec_dec_init(&ctx, decoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    aom_codec_iter_t iter = NULL;
    aom_image_t *frame = NULL;

    // Feed data to the decoder using the correct API
    if (aom_codec_decode(&ctx, Data, Size, NULL) != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Attempt to get frames and query control functions
    while ((frame = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        int val;
        aom_screen_content_tools_info sc_info;
        aom_s_frame_info s_info;
        aom_still_picture_info still_info;

        // 1. AOMD_GET_FWD_KF_PRESENT
        if (aom_codec_control(&ctx, AOMD_GET_FWD_KF_PRESENT, &val) == AOM_CODEC_OK) {
            (void)val;
        }

        // 2. AOMD_GET_STILL_PICTURE
        if (aom_codec_control(&ctx, AOMD_GET_STILL_PICTURE, &still_info) == AOM_CODEC_OK) {
            (void)still_info.is_still_picture;
            (void)still_info.is_reduced_still_picture_hdr;
        }

        // 3. AOMD_GET_ORDER_HINT
        if (aom_codec_control(&ctx, AOMD_GET_ORDER_HINT, &val) == AOM_CODEC_OK) {
            (void)val;
        }

        // 4. AOMD_GET_SCREEN_CONTENT_TOOLS_INFO
        if (aom_codec_control(&ctx, AOMD_GET_SCREEN_CONTENT_TOOLS_INFO, &sc_info) == AOM_CODEC_OK) {
            (void)sc_info.allow_screen_content_tools;
            (void)sc_info.allow_intrabc;
            (void)sc_info.force_integer_mv;
        }

        // 5. AV1D_GET_FRAME_HEADER_INFO
        // Assuming it returns an int or similar scalar for fuzzing safety
        if (aom_codec_control(&ctx, AV1D_GET_FRAME_HEADER_INFO, &val) == AOM_CODEC_OK) {
            (void)val;
        }

        // 6. AOMD_GET_S_FRAME_INFO
        if (aom_codec_control(&ctx, AOMD_GET_S_FRAME_INFO, &s_info) == AOM_CODEC_OK) {
            (void)s_info.is_s_frame;
            (void)s_info.is_s_frame_at_altref;
        }

        // Cleanup frame
        aom_img_free(frame);
    }

    aom_codec_destroy(&ctx);
    return 0;
}