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
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *decoder_if = aom_codec_av1_dx();
    if (!decoder_if) return 0;

    aom_codec_dec_cfg_t cfg;
    // Initialize config structure manually since aom_codec_dec_cfg_default may not exist in all versions
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Use valid members for aom_codec_dec_cfg_t
    cfg.allow_lowbitdepth = 1; 

    aom_codec_err_t ret = aom_codec_dec_init(&ctx, decoder_if, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt;
    aom_image_t *img;

    write_dummy_file(Data, Size);

    ret = aom_codec_decode(&ctx, Data, Size, NULL);
    
    while ((pkt = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Access the image correctly via the frame union member
            img = &pkt->data.frame;
            
            // 1. AOMD_GET_FRAME_CORRUPTED
            int corrupted = 0;
            aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &corrupted);

            // 2. AOMD_GET_FWD_KF_PRESENT
            int fwd_kf_present = 0;
            aom_codec_control(&ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);

            // 3. AOMD_GET_SHOW_EXISTING_FRAME_FLAG
            int show_existing = 0;
            aom_codec_control(&ctx, AOMD_GET_SHOW_EXISTING_FRAME_FLAG, &show_existing);

            // 4. AOMD_GET_FRAME_FLAGS
            int frame_flags = 0;
            aom_codec_control(&ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);

            // 5. AOMD_GET_LAST_REF_UPDATES
            int ref_updates = 0;
            aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, &ref_updates);

            // 6. AOMD_GET_ALTREF_PRESENT
            int altref_present = 0;
            aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);
            
            // Release the frame
            aom_img_free(img);
        }
    }

    aom_codec_destroy(&ctx);

    return 0;
}