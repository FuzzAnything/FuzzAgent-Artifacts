// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 1;
    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) return 1;
    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

static int fuzz_control(aom_codec_ctx_t *codec, int ctrl_id, uint8_t *data, size_t size) {
    int ret = AOM_CODEC_OK;
    
    if (size < 1) return 0;
    
    // Interpret data based on control type
    switch (ctrl_id) {
        case AV1E_SET_CHROMA_SUBSAMPLING_X: {
            int val = data[0] % 3; // 0, 1, or 2
            ret = aom_codec_control(codec, AV1E_SET_CHROMA_SUBSAMPLING_X, val);
            break;
        }
        case AV1E_SET_ENABLE_SMOOTH_INTRA: {
            int val = data[0] % 2; // 0 or 1
            ret = aom_codec_control(codec, AV1E_SET_ENABLE_SMOOTH_INTRA, val);
            break;
        }
        case AV1E_SET_MIN_CR: {
            if (size < 2) return 0;
            int16_t val = (int16_t)((data[0] << 8) | data[1]);
            ret = aom_codec_control(codec, AV1E_SET_MIN_CR, val);
            break;
        }
        case AV1E_SET_INTER_DCT_ONLY: {
            int val = data[0] % 2; // 0 or 1
            ret = aom_codec_control(codec, AV1E_SET_INTER_DCT_ONLY, val);
            break;
        }
        case AV1E_SET_QM_V: {
            // QM_V expects a pointer to a quantization matrix
            if (size < 64) return 0;
            uint8_t qm[64];
            memcpy(qm, data, 64);
            ret = aom_codec_control(codec, AV1E_SET_QM_V, qm);
            break;
        }
        default:
            return 0;
    }
    
    return ret;
}

int LLVMFuzzerTestOneInput_186(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    
    // Initialize encoder
    if (init_encoder(&codec, &cfg) != 0) return 0;
    
    // Extract control ID from first byte
    // We map to the 5 valid controls available in the headers: 
    // 0: CHROMA_SUBSAMPLING_X
    // 1: ENABLE_SMOOTH_INTRA
    // 2: MIN_CR
    // 3: INTER_DCT_ONLY
    // 4: QM_V
    // Note: AV1E_SET_ENABLE_DIST_8X8 is removed as it is not defined in the provided headers.
    int ctrl_id = Data[0] % 5; 
    int actual_ctrl_id = 0;
    
    switch (ctrl_id) {
        case 0: actual_ctrl_id = AV1E_SET_CHROMA_SUBSAMPLING_X; break;
        case 1: actual_ctrl_id = AV1E_SET_ENABLE_SMOOTH_INTRA; break;
        case 2: actual_ctrl_id = AV1E_SET_MIN_CR; break;
        case 3: actual_ctrl_id = AV1E_SET_INTER_DCT_ONLY; break;
        case 4: actual_ctrl_id = AV1E_SET_QM_V; break;
    }
    
    // Fuzz the control function with remaining data
    fuzz_control(&codec, actual_ctrl_id, (uint8_t*)&Data[1], Size - 1);
    
    // Cleanup
    cleanup_encoder(&codec);
    
    return 0;
}