// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
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
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 0;
    cfg.w = 64;
    cfg.h = 64;

    aom_codec_err_t err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz aom_codec_decode
    err = aom_codec_decode(&ctx, Data, Size, NULL);
    (void)err;

    // Attempt to retrieve S_FRAME info
    aom_s_frame_info s_frame_info;
    memset(&s_frame_info, 0, sizeof(s_frame_info));
    err = aom_codec_control(&ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);
    (void)err;

    // Attempt to retrieve last quantizer (int value)
    int quantizer;
    memset(&quantizer, 0, sizeof(quantizer));
    err = aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &quantizer);
    (void)err;

    // Attempt to retrieve last ref used (int array)
    int ref_used[8];
    memset(ref_used, 0, sizeof(ref_used));
    err = aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, ref_used);
    (void)err;

    // Attempt to retrieve last ref updates (int array)
    int ref_updates[8];
    memset(ref_updates, 0, sizeof(ref_updates));
    err = aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, ref_updates);
    (void)err;

    // Attempt to retrieve altref present (int value)
    int altref_present;
    memset(&altref_present, 0, sizeof(altref_present));
    err = aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);
    (void)err;

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}