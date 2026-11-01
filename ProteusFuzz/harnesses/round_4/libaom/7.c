// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"

static int read_int_from_data(const uint8_t **data_ptr, size_t *size, size_t *offset) {
    if (*offset + sizeof(int) > *size) {
        *offset = *size;
        return 0;
    }
    int v = 0;
    memcpy(&v, *data_ptr + *offset, sizeof(int));
    *offset += sizeof(int);
    return v;
}

static int fuzz_encoder_control(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_err_t ret;
    int ctrl_id;
    int val_int;
    aom_svc_params_t svc_params;
    const uint8_t *data_ptr = Data;
    size_t offset = 0;

    if (iface == NULL) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz AV1E_SET_LOOPFILTER_CONTROL
    if (offset < Size) {
        int lf_val = read_int_from_data(&data_ptr, &Size, &offset);
        ret = aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, lf_val);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_INTRABC
    if (offset < Size) {
        int intrabc_val = read_int_from_data(&data_ptr, &Size, &offset) % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, intrabc_val);
        (void)ret;
    }

    // Fuzz AV1E_SET_SVC_PARAMS
    if (offset < Size) {
        memset(&svc_params, 0, sizeof(svc_params));
        svc_params.number_spatial_layers = (read_int_from_data(&data_ptr, &Size, &offset) % 3) + 1;
        svc_params.number_temporal_layers = (read_int_from_data(&data_ptr, &Size, &offset) % 3) + 1;
        
        for (int i = 0; i < AOM_MAX_LAYERS; i++) {
            svc_params.max_quantizers[i] = read_int_from_data(&data_ptr, &Size, &offset) % 256;
            svc_params.min_quantizers[i] = read_int_from_data(&data_ptr, &Size, &offset) % 256;
            svc_params.layer_target_bitrate[i] = read_int_from_data(&data_ptr, &Size, &offset) % 10000;
        }
        for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
            svc_params.scaling_factor_num[i] = read_int_from_data(&data_ptr, &Size, &offset) % 100;
            svc_params.scaling_factor_den[i] = (read_int_from_data(&data_ptr, &Size, &offset) % 100) + 1;
        }
        for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
            svc_params.framerate_factor[i] = read_int_from_data(&data_ptr, &Size, &offset) % 100;
        }
        
        ret = aom_codec_control(&ctx, AV1E_SET_SVC_PARAMS, &svc_params);
        (void)ret;
    }

    // Fuzz AV1_SET_SKIP_LOOP_FILTER
    if (offset < Size) {
        int skip_lf = read_int_from_data(&data_ptr, &Size, &offset) % 2;
        ret = aom_codec_control(&ctx, AV1_SET_SKIP_LOOP_FILTER, skip_lf);
        (void)ret;
    }

    // Fuzz AOME_GET_LOOPFILTER_LEVEL
    if (offset < Size) {
        int *level = &val_int;
        ret = aom_codec_control(&ctx, AOME_GET_LOOPFILTER_LEVEL, level);
        (void)ret;
    }

    // General aom_codec_control fuzzing with random IDs
    if (offset < Size) {
        ctrl_id = read_int_from_data(&data_ptr, &Size, &offset);
        if (ctrl_id != 0) {
            val_int = read_int_from_data(&data_ptr, &Size, &offset);
            ret = aom_codec_control(&ctx, ctrl_id, val_int);
            (void)ret;
        }
    }

    aom_codec_destroy(&ctx);
    return 0;
}

static int fuzz_decoder_control(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_err_t ret;
    int ctrl_id;
    int val_int;
    const uint8_t *data_ptr = Data;
    size_t offset = 0;

    if (iface == NULL) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Try general control on decoder
    if (offset < Size) {
        ctrl_id = read_int_from_data(&data_ptr, &Size, &offset);
        if (ctrl_id != 0) {
            val_int = read_int_from_data(&data_ptr, &Size, &offset);
            ret = aom_codec_control(&ctx, ctrl_id, val_int);
            (void)ret;
        }
    }

    // Try specific controls that might be valid for decoder or generic
    if (offset < Size) {
        int val = read_int_from_data(&data_ptr, &Size, &offset);
        ret = aom_codec_control(&ctx, AOME_SET_CPUUSED, val);
        (void)ret;
    }

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Split input to test both encoder and decoder paths if data is large enough
    size_t split = Size / 2;
    
    if (split > 0) {
        fuzz_encoder_control(Data, split);
    }
    
    if (Size > split) {
        fuzz_decoder_control(Data + split, Size - split);
    } else {
        fuzz_decoder_control(Data, Size);
    }

    return 0;
}