// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_191(const uint8_t *Data, size_t Size) {
    if (Size < 13) return 0;

    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    aom_codec_enc_cfg_t cfg;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    /* Safe access to Data with bounds checking */
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.g_threads = (Data[4] % 4) + 1;
    cfg.rc_target_bitrate = (Data[5] % 1000) + 100;
    cfg.g_error_resilient = (Data[6] % 2);

    ret = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    write_dummy_file(Data, Size);

    int val1 = (int)(Data[7] % 2);
    int val2 = (int)(Data[8] % 100) + 1;
    int val3 = (int)(Data[9] % 4);
    int val4 = (int)(Data[10] % 2);
    int val5 = (int)(Data[11] % 63) + 1;
    int val6 = (int)(Data[12] % 100);

    aom_codec_control(&codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val1);
    aom_codec_control(&codec, AOME_SET_MAX_INTRA_BITRATE_PCT, val2);
    aom_codec_control(&codec, AV1E_SET_CDF_UPDATE_MODE, val3);
    aom_codec_control(&codec, AV1E_SET_BITRATE_ONE_PASS_CBR, val4);
    aom_codec_control(&codec, AOME_SET_CQ_LEVEL, val5);
    aom_codec_control(&codec, AV1E_SET_MODE_COST_UPD_FREQ, val6);

    aom_codec_destroy(&codec);

    return 0;
}