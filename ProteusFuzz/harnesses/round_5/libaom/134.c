// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control_typechecked_AOMD_GET_ALTREF_PRESENT at aomdx.h:567:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_SHOW_EXISTING_FRAME_FLAG at aomdx.h:583:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_SHOW_FRAME_FLAG at aomdx.h:589:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_FRAME_FLAGS at aomdx.h:564:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_LAST_REF_UPDATES at aomdx.h:483:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_FRAME_CORRUPTED at aomdx.h:486:1 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t res;
    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt;

    // Initialize decoder configuration
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    // Get the AV1 decoder interface
    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    // Initialize the codec context
    res = aom_codec_dec_init(&codec, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Feed data to the decoder if available
    if (Size > 0) {
        aom_codec_decode(&codec, Data, Size, NULL);
    }

    // Attempt to get packets
    while ((pkt = aom_codec_get_frame(&codec, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // 1. AOMD_GET_ALTREF_PRESENT
            int altref_present = 0;
            aom_codec_control_typechecked_AOMD_GET_ALTREF_PRESENT(&codec, AOMD_GET_ALTREF_PRESENT, &altref_present);

            // 2. AOMD_GET_SHOW_EXISTING_FRAME_FLAG
            int show_existing = 0;
            aom_codec_control_typechecked_AOMD_GET_SHOW_EXISTING_FRAME_FLAG(&codec, AOMD_GET_SHOW_EXISTING_FRAME_FLAG, &show_existing);

            // 3. AOMD_GET_SHOW_FRAME_FLAG
            int show_frame = 0;
            aom_codec_control_typechecked_AOMD_GET_SHOW_FRAME_FLAG(&codec, AOMD_GET_SHOW_FRAME_FLAG, &show_frame);

            // 4. AOMD_GET_FRAME_FLAGS
            int frame_flags = 0;
            aom_codec_control_typechecked_AOMD_GET_FRAME_FLAGS(&codec, AOMD_GET_FRAME_FLAGS, &frame_flags);

            // 5. AOMD_GET_LAST_REF_UPDATES
            int last_ref_updates = 0;
            aom_codec_control_typechecked_AOMD_GET_LAST_REF_UPDATES(&codec, AOMD_GET_LAST_REF_UPDATES, &last_ref_updates);

            // 6. AOMD_GET_FRAME_CORRUPTED
            int frame_corrupted = 0;
            aom_codec_control_typechecked_AOMD_GET_FRAME_CORRUPTED(&codec, AOMD_GET_FRAME_CORRUPTED, &frame_corrupted);
        }
    }

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}