// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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

static int fuzz_decode_frame(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    aom_codec_err_t res;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    if (!iface) return 0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Removed cfg.show_frame as it is not a member of aom_codec_dec_cfg_t in newer versions

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    aom_codec_iter_t iter = NULL;
    aom_image_t *img;

    // Feed data in chunks to trigger various decoder states
    size_t offset = 0;
    while (offset < size) {
        size_t chunk_size = 1024;
        if (offset + chunk_size > size) chunk_size = size - offset;

        // Attempt to decode a chunk
        res = aom_codec_decode(ctx, data + offset, chunk_size, 0);
        if (res != AOM_CODEC_OK && res != AOM_CODEC_ERROR) {
            aom_codec_destroy(ctx);
            return 0;
        }

        while ((img = aom_codec_get_frame(ctx, &iter))) {
            // Exercise the target control functions
            // Passing NULL or dummy pointers to trigger potential NULL pointer dereferences
            
            aom_codec_control(ctx, AOMD_GET_TILE_INFO, NULL);
            aom_codec_control(ctx, AOMD_GET_LAST_QUANTIZER, NULL);
            aom_codec_control(ctx, AOMD_GET_LAST_REF_USED, NULL);
            aom_codec_control(ctx, AOMD_GET_FRAME_FLAGS, NULL);
            aom_codec_control(ctx, AOMD_GET_LAST_REF_UPDATES, NULL);
            aom_codec_control(ctx, AV1D_GET_FRAME_SIZE, NULL);
        }

        offset += chunk_size;
    }

    aom_codec_destroy(ctx);
    return 1;
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    fuzz_decode_frame(&ctx, Data, Size);

    return 0;
}