// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control_typechecked_AOMD_GET_S_FRAME_INFO at aomdx.h:586:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_FRAME_FLAGS at aomdx.h:564:1 in aomdx.h
// aom_codec_control_typechecked_AV1D_GET_FRAME_HEADER_INFO at aomdx.h:531:1 in aomdx.h
// aom_codec_control_typechecked_AV1_GET_NEW_FRAME_IMAGE at aom.h:114:1 in aom.h
// aom_codec_control_typechecked_AOMD_GET_SHOW_FRAME_FLAG at aomdx.h:589:1 in aomdx.h
// aom_codec_control_typechecked_AV1_COPY_NEW_FRAME_IMAGE at aom.h:117:1 in aom.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

int LLVMFuzzerTestOneInput_265(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(&ctx, decoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    const uint8_t *data_ptr = Data;
    size_t data_size = Size;

    // Attempt to decode a frame to populate internal state for control queries
    aom_codec_decode(&ctx, data_ptr, data_size, NULL);

    // Consume packets to ensure state is updated
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_frame(&ctx, &iter))) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Frame decoded, state is ready for queries
        }
    }

    // Prepare arguments for control functions based on fuzz data
    int flags = 0;
    aom_s_frame_info s_frame_info;
    memset(&s_frame_info, 0, sizeof(s_frame_info));
    
    // Interpret fuzz data to diversify control calls
    if (Size > 0) {
        flags = Data[0];
    }
    if (Size > 1) {
        s_frame_info.is_s_frame = Data[1];
    }
    if (Size > 2) {
        s_frame_info.is_s_frame_at_altref = Data[2];
    }

    // 1. AOMD_GET_S_FRAME_INFO
    // Signature: aom_codec_control_typechecked_AOMD_GET_S_FRAME_INFO(ctx, ctrl, data)
    aom_codec_control_typechecked_AOMD_GET_S_FRAME_INFO(&ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);

    // 2. AOMD_GET_FRAME_FLAGS
    // Signature: aom_codec_control_typechecked_AOMD_GET_FRAME_FLAGS(ctx, ctrl, data)
    aom_codec_control_typechecked_AOMD_GET_FRAME_FLAGS(&ctx, AOMD_GET_FRAME_FLAGS, &flags);

    // 3. AV1D_GET_FRAME_HEADER_INFO (Requires aom_tile_data*)
    aom_tile_data tile_data;
    memset(&tile_data, 0, sizeof(tile_data));
    aom_codec_control_typechecked_AV1D_GET_FRAME_HEADER_INFO(&ctx, AV1D_GET_FRAME_HEADER_INFO, &tile_data);

    // 4. AV1_GET_NEW_FRAME_IMAGE (Requires aom_image_t*)
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    aom_codec_control_typechecked_AV1_GET_NEW_FRAME_IMAGE(&ctx, AV1_GET_NEW_FRAME_IMAGE, &img);

    // 5. AOMD_GET_SHOW_FRAME_FLAG
    aom_codec_control_typechecked_AOMD_GET_SHOW_FRAME_FLAG(&ctx, AOMD_GET_SHOW_FRAME_FLAG, &flags);

    // 6. AV1_COPY_NEW_FRAME_IMAGE (Requires aom_image_t*)
    aom_codec_control_typechecked_AV1_COPY_NEW_FRAME_IMAGE(&ctx, AV1_COPY_NEW_FRAME_IMAGE, &img);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}