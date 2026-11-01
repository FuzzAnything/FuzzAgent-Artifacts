// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
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

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_decode_path(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Removed cfg.cpu_used as it does not exist in aom_codec_dec_cfg
    // cfg.cpu_used = 0; 

    res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz aom_codec_decode
    if (Size > 0) {
        aom_codec_decode(&ctx, Data, Size, NULL);
    }

    // Fuzz aom_codec_get_stream_info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_get_stream_info(&ctx, &si);

    cleanup_decoder(&ctx);
    return 0;
}

static int fuzz_peek_path(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_iface_t *iface;
    aom_codec_stream_info_t si;
    aom_codec_err_t res;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    memset(&si, 0, sizeof(si));
    // Try both Annex-B and raw formats
    for (int i = 0; i < 2; i++) {
        si.is_annexb = (i == 0) ? 1 : 0;
        res = aom_codec_peek_stream_info(iface, Data, Size, &si);
        (void)res;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Path 1: Full decode workflow
    fuzz_decode_path(Data, Size);

    // Path 2: Peek stream info without full decode
    fuzz_peek_path(Data, Size);

    return 0;
}