// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return -1;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input to dummy file if needed
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }

    // Initialize decoder context
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Get AV1 decoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    // Initialize decoder configuration
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    // Initialize decoder context
    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare stream info structure
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));

    // Create a packet to feed the decoder
    aom_codec_cx_pkt_t pkt;
    memset(&pkt, 0, sizeof(pkt));
    
    // Initialize the data buffer within the packet
    // aom_codec_cx_pkt_t.data is a union, we need to set the 'raw' member
    pkt.data.raw.buf = (uint8_t *)Data;
    pkt.data.raw.sz = Size;

    // Decode the packet
    aom_codec_err_t decode_res = aom_codec_decode(&ctx, &pkt, 1, NULL);

    // Fuzz target: aom_codec_get_stream_info
    // We call it multiple times to explore different states
    aom_codec_err_t res = aom_codec_get_stream_info(&ctx, &si);

    // Additional calls to explore state variations
    // Try with a modified si structure
    si.w = 0;
    si.h = 0;
    res = aom_codec_get_stream_info(&ctx, &si);

    // Try with NULL pointer (should return INVALID_PARAM)
    res = aom_codec_get_stream_info(&ctx, NULL);

    // Try with NULL context (should return INVALID_PARAM)
    res = aom_codec_get_stream_info(NULL, &si);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}