// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void setup_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f && size > 0) {
        fwrite(data, 1, size, f);
    }
    if (f) {
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int enable_val;
    int boost_val;
    int qm_max_val;
    int filter_intra_val;
    int rtc_ext_rc_val;
    int high_motion_val;

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Get encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Set default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize encoder context
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare dummy file if needed
    setup_dummy_file(Data, Size);

    // Fuzz AV1E_SET_ENABLE_PALETTE
    if (Size > 0) {
        enable_val = Data[0] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, enable_val);
    }

    // Fuzz AV1E_SET_FRAME_PERIODIC_BOOST
    if (Size > 1) {
        boost_val = (int)Data[1];
        aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, boost_val);
    }

    // Fuzz AV1E_SET_QM_MAX
    if (Size > 2) {
        qm_max_val = (int)Data[2];
        aom_codec_control(&ctx, AV1E_SET_QM_MAX, qm_max_val);
    }

    // Fuzz AV1E_SET_ENABLE_FILTER_INTRA
    if (Size > 3) {
        filter_intra_val = Data[3] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, filter_intra_val);
    }

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC
    if (Size > 4) {
        rtc_ext_rc_val = Data[4] % 2;
        aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, rtc_ext_rc_val);
    }

    // Fuzz AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC
    if (Size > 5) {
        high_motion_val = (int)Data[5];
        aom_codec_control(&ctx, AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC, &high_motion_val);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}