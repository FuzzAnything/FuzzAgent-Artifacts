// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

static int fuzz_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    const aom_codec_iface_t *iface;
    aom_codec_err_t err;
    aom_fixed_buf_t *global_headers = NULL;
    int ret = 0;

    if (size < 11) return 0;

    // Use the correct function name for AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Use fuzzed data to configure the encoder
    cfg.g_w = (data[0] % 256) + 16;
    cfg.g_h = (data[1] % 256) + 16;
    cfg.g_timebase.num = (data[2] % 100) + 1;
    cfg.g_timebase.den = (data[3] % 100) + 1;
    cfg.g_threads = (data[4] % 4) + 1;
    cfg.g_lag_in_frames = (data[5] % 10);
    cfg.rc_target_bitrate = (data[6] % 1000) + 100;

    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Fuzz control functions
    int32_t quantizer = 0;
    int32_t tile_rows = 0;
    int32_t quantizer_one_pass = 0;
    int32_t cpu_used = 0;
    int32_t bitrate_cbr = 0;

    // AOME_GET_LAST_QUANTIZER
    aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER, &quantizer);

    // AV1E_SET_TILE_ROWS
    tile_rows = (data[7] % 8) + 1;
    aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, &tile_rows);

    // AV1E_SET_QUANTIZER_ONE_PASS
    quantizer_one_pass = (data[8] % 64);
    aom_codec_control(&ctx, AV1E_SET_QUANTIZER_ONE_PASS, &quantizer_one_pass);

    // AOME_SET_CPUUSED
    cpu_used = (data[9] % 8);
    aom_codec_control(&ctx, AOME_SET_CPUUSED, &cpu_used);

    // AV1E_SET_BITRATE_ONE_PASS_CBR
    bitrate_cbr = (data[10] % 1000) + 100;
    aom_codec_control(&ctx, AV1E_SET_BITRATE_ONE_PASS_CBR, &bitrate_cbr);

    // Get global headers
    global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        if (global_headers->buf) {
            // Simulate usage
            memset(global_headers->buf, 0, global_headers->sz);
            free(global_headers->buf);
        }
        free(global_headers);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return ret;
}

int LLVMFuzzerTestOneInput_268(const uint8_t *Data, size_t Size) {
    if (Size < 11) return 0;

    // Fuzz encoder
    fuzz_encoder(Data, Size);

    return 0;
}