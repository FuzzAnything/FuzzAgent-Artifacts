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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Fixed: Removed invalid member 'allow_highbitdepth'
    // cfg.allow_lowbitdepth = 1; // Optional, default is usually 0

    if (aom_codec_dec_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    aom_codec_iter_t iter = NULL;
    // Fixed: Use aom_image_t* as returned by aom_codec_get_frame
    aom_image_t *frame = NULL;

    // Feed data to decoder
    aom_codec_err_t res = aom_codec_decode(&codec, (const uint8_t *)Data, Size, &iter);
    if (res != AOM_CODEC_OK) {
        // Continue to test control commands even if decode fails
    }

    // Extract frames if any
    while ((frame = aom_codec_get_frame(&codec, &iter)) != NULL) {
        // Frame extracted, now test control commands on this state
        int val;
        aom_s_frame_info s_frame_info;
        memset(&s_frame_info, 0, sizeof(s_frame_info));

        // 1. AOMD_GET_LAST_REF_UPDATES
        aom_codec_control(&codec, AOMD_GET_LAST_REF_UPDATES, &val);

        // 2. AOMD_GET_FRAME_CORRUPTED
        aom_codec_control(&codec, AOMD_GET_FRAME_CORRUPTED, &val);

        // 3. AOMD_GET_ALTREF_PRESENT
        aom_codec_control(&codec, AOMD_GET_ALTREF_PRESENT, &val);

        // 4. AOMD_GET_LAST_REF_USED
        aom_codec_control(&codec, AOMD_GET_LAST_REF_USED, &val);

        // 5. AOMD_GET_FRAME_FLAGS
        aom_codec_control(&codec, AOMD_GET_FRAME_FLAGS, &val);

        // 6. AOMD_GET_S_FRAME_INFO
        aom_codec_control(&codec, AOMD_GET_S_FRAME_INFO, &s_frame_info);
        
        // No need to free frame here, aom_codec_get_frame manages memory internally
        // or the caller is expected to not free it directly depending on version,
        // but typically aom_codec_destroy handles cleanup.
    }

    cleanup_codec(&codec);
    return 0;
}