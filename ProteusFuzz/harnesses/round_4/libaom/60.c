// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int get_int(const uint8_t *d, size_t s, int default_val) {
    if (s < 4) return default_val;
    return (int)((d[0] << 24) | (d[1] << 16) | (d[2] << 8) | d[3]);
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure width and height safely
    cfg->g_w = (size > 0) ? data[0] : 64;
    cfg->g_h = (size > 1) ? data[1] : 64;
    
    // Ensure dimensions are multiples of 16 and non-zero
    if (cfg->g_w < 16) cfg->g_w = 16;
    if (cfg->g_h < 16) cfg->g_h = 16;
    cfg->g_w = (cfg->g_w + 15) & ~15;
    cfg->g_h = (cfg->g_h + 15) & ~15;

    cfg->g_threads = 1;
    cfg->g_error_resilient = 1;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&ctx, &cfg, Data, Size)) {
        return 0;
    }

    // 1. AV1E_SET_MAX_PARTITION_SIZE
    // Expected: int value
    int max_partition_size = get_int(Data, Size, 0);
    if (max_partition_size < 0) max_partition_size = 0;
    if (max_partition_size > 128) max_partition_size = 128; // Reasonable cap
    aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, max_partition_size);

    // 2. AV1E_SET_ENABLE_CDEF
    // Expected: int (0 or 1)
    int enable_cdef = get_int(Data, Size, 0) % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, enable_cdef);

    // 3. AV1E_SET_ENABLE_KEYFRAME_FILTERING
    // Expected: int (0 or 1)
    int enable_keyframe_filtering = get_int(Data + 4, Size > 4 ? Size - 4 : 0, 0) % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, enable_keyframe_filtering);

    // 4. AV1E_SET_LOOPFILTER_CONTROL
    // The specific struct aom_loopfilter_t is not available in the provided headers or is internal.
    // We skip this specific control to avoid compilation errors, as the type is undefined in the context.
    // In a real scenario, one would use the correct public API struct if available.

    // 5. AV1E_SET_SKIP_POSTPROC_FILTERING
    // Expected: int (0 or 1)
    int skip_postproc = get_int(Data + 18, Size > 18 ? Size - 18 : 0, 0) % 2;
    aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, skip_postproc);

    // 6. AV1E_SET_FORCE_VIDEO_MODE
    // Expected: int (0 or 1)
    int force_video_mode = get_int(Data + 22, Size > 22 ? Size - 22 : 0, 0) % 2;
    aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, force_video_mode);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}