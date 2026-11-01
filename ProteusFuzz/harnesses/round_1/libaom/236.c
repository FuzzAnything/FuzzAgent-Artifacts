// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
        // Do not free the codec struct itself if it was allocated on the stack.
        // aom_codec_destroy handles internal allocations.
    }
}

int LLVMFuzzerTestOneInput_236(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Allocate codec context on stack to avoid double-free or invalid free issues
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 0;

    aom_codec_err_t ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Decode input data if available
    if (Size > 0) {
        aom_codec_decode(&ctx, Data, Size, NULL);
    }

    // Fuzz control functions
    int show_frame_flag;
    int show_existing_frame_flag;
    int base_q_idx;
    int skip_film_grain;
    aom_image_t *img = NULL;

    // AOMD_GET_SHOW_FRAME_FLAG
    ret = aom_codec_control(&ctx, AOMD_GET_SHOW_FRAME_FLAG, &show_frame_flag);
    (void)ret;

    // AOMD_GET_SHOW_EXISTING_FRAME_FLAG
    ret = aom_codec_control(&ctx, AOMD_GET_SHOW_EXISTING_FRAME_FLAG, &show_existing_frame_flag);
    (void)ret;

    // AOMD_GET_BASE_Q_IDX
    ret = aom_codec_control(&ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);
    (void)ret;

    // AV1D_SET_SKIP_FILM_GRAIN
    skip_film_grain = (Size % 2);
    ret = aom_codec_control(&ctx, AV1D_SET_SKIP_FILM_GRAIN, &skip_film_grain);
    (void)ret;

    // AV1_GET_NEW_FRAME_IMAGE
    ret = aom_codec_control(&ctx, AV1_GET_NEW_FRAME_IMAGE, &img);
    if (ret == AOM_CODEC_OK && img != NULL) {
        aom_img_free(img);
    }

    cleanup_codec(&ctx);

    return 0;
}