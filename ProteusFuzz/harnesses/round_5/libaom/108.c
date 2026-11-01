// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_decode_and_control(const uint8_t *data, size_t size) {
    if (size == 0) return 0;

    aom_codec_ctx_t ctx;
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1; 

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    aom_codec_iter_t iter = NULL;
    const aom_image_t *frame; // Fixed: aom_codec_frame_t does not exist, use aom_image_t

    // Feed data to decoder
    aom_codec_decode(&ctx, data, size, NULL);

    // Iterate over decoded frames
    while ((frame = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        int ret;
        int val;
        aom_tile_info tile_info;
        aom_enc_frame_flags_t flags; // Fixed: Use aom_enc_frame_flags_t
        int last_ref_updates[8];
        int corrupted;

        // 1. AOMD_GET_ALTREF_PRESENT
        ret = aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &val);
        (void)ret; (void)val;

        // 2. AV1D_GET_FRAME_SIZE
        aom_image_t img;
        ret = aom_codec_control(&ctx, AV1D_GET_FRAME_SIZE, &img);
        (void)ret; (void)img;

        // 3. AOMD_GET_TILE_INFO
        ret = aom_codec_control(&ctx, AOMD_GET_TILE_INFO, &tile_info);
        (void)ret; (void)tile_info;

        // 4. AOMD_GET_FRAME_FLAGS
        ret = aom_codec_control(&ctx, AOMD_GET_FRAME_FLAGS, &flags);
        (void)ret; (void)flags;

        // 5. AOMD_GET_LAST_REF_UPDATES
        ret = aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, last_ref_updates);
        (void)ret; (void)last_ref_updates;

        // 6. AOMD_GET_FRAME_CORRUPTED
        ret = aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &corrupted);
        (void)ret; (void)corrupted;
    }

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    fuzz_decode_and_control(Data, Size);

    if (Size > 20) {
        fuzz_decode_and_control(Data + 5, Size - 5);
    }

    if (Size > 1000) {
        fuzz_decode_and_control(Data, 1000);
    }

    return 0;
}