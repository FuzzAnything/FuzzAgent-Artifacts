// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        return 0;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 64;
    cfg.h = 64;

    if (aom_codec_dec_init(&ctx, decoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Attempt to invoke the target control functions
    // These functions typically retrieve state from the decoder context.
    
    // 1. AOMD_GET_SHOW_EXISTING_FRAME_FLAG
    int show_existing_frame_flag = 0;
    aom_codec_control(&ctx, AOMD_GET_SHOW_EXISTING_FRAME_FLAG, &show_existing_frame_flag);

    // 2. AOMD_GET_SHOW_FRAME_FLAG
    int show_frame_flag = 0;
    aom_codec_control(&ctx, AOMD_GET_SHOW_FRAME_FLAG, &show_frame_flag);

    // 3. AOMD_GET_SB_SIZE
    aom_superblock_size_t sb_size = AOM_SUPERBLOCK_SIZE_64X64;
    aom_codec_control(&ctx, AOMD_GET_SB_SIZE, &sb_size);

    // 4. AOMD_GET_FRAME_FLAGS
    uint32_t frame_flags = 0;
    aom_codec_control(&ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);

    // 5. AOMD_GET_LAST_REF_UPDATES
    uint8_t last_ref_updates[32];
    memset(last_ref_updates, 0, sizeof(last_ref_updates));
    aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, last_ref_updates);

    // 6. AOMD_GET_FRAME_CORRUPTED
    int frame_corrupted = 0;
    aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &frame_corrupted);

    // To exercise more states, we attempt to decode a frame using the fuzzing data.
    // We pass the data directly to aom_codec_decode.
    if (Size > 0) {
        aom_codec_decode(&ctx, Data, Size, NULL);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}