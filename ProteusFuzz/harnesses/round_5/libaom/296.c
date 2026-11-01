// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Required Header Files */
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

/* 
 * Note: The provided task description indicates that the specific functions 
 * (e.g., aom_codec_control_typechecked_AV1E_SET_QM_U) have empty signatures 
 * in the prompt. However, based on standard libaom API conventions, these 
 * correspond to specific control commands passed to aom_codec_control().
 * 
 * Since the exact internal "typechecked" wrapper functions are not standard 
 * public API (usually these are internal or the user calls aom_codec_control 
 * with the enum), this fuzz driver will:
 * 1. Initialize an AV1 encoder context.
 * 2. Parse the fuzz input to determine which control command to invoke.
 * 3. Map the input to the corresponding aom_codec_control enum (e.g., AV1E_SET_QM_U).
 * 4. Invoke aom_codec_control with the parsed arguments.
 */

static int setup_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->rc_target_bitrate = 100;

    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

static int parse_u32(const uint8_t *data, size_t size, size_t *offset, uint32_t *out) {
    if (*offset + 4 > size) return 0;
    *out = (data[*offset] << 24) | (data[*offset + 1] << 16) | 
           (data[*offset + 2] << 8) | data[*offset + 3];
    *offset += 4;
    return 1;
}

static int parse_u8(const uint8_t *data, size_t size, size_t *offset, uint8_t *out) {
    if (*offset + 1 > size) return 0;
    *out = data[*offset];
    *offset += 1;
    return 1;
}

int LLVMFuzzerTestOneInput_296(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (setup_encoder(&codec, &cfg) != 0) {
        return 0;
    }

    size_t offset = 0;
    uint8_t cmd;
    
    /* Select a command based on the first byte */
    if (!parse_u8(Data, Size, &offset, &cmd)) {
        cleanup_encoder(&codec);
        return 0;
    }

    int ret = AOM_CODEC_OK;

    /* 
     * Mapping based on the requested targets:
     * 0: AV1E_SET_QM_U
     * 1: AV1E_SET_ENABLE_TX_SIZE_SEARCH
     * 2: AV1E_SET_DELTALF_MODE
     * 3: AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
     * 4: AV1E_SET_DISABLE_TRELLIS_QUANT
     * 5: AOME_SET_ARNR_STRENGTH
     * Others: Generic control or random enum
     */

    switch (cmd) {
        case 0: {
            /* AV1E_SET_QM_U: expects a pointer to a struct or array, 
               but often fuzzers pass a scalar or small buffer. 
               The API expects a pointer to a quantization matrix (uint16_t[64]).
               We will try to construct a valid buffer from input if possible.
            */
            uint16_t qm[64] = {0};
            size_t bytes_read = 0;
            for (int i = 0; i < 64 && offset + 2 <= Size; i++) {
                uint8_t b1 = Data[offset++];
                uint8_t b2 = (offset < Size) ? Data[offset++] : 0;
                qm[i] = (b1 << 8) | b2;
                bytes_read += 2;
            }
            /* If we didn't get enough data, use a default pattern */
            if (bytes_read < 128) {
                for (int i = 0; i < 64; i++) qm[i] = 128;
            }
            ret = aom_codec_control(&codec, AV1E_SET_QM_U, qm);
            break;
        }
        case 1: {
            /* AV1E_SET_ENABLE_TX_SIZE_SEARCH: expects an int (0 or 1) */
            uint32_t val = 0;
            if (parse_u32(Data, Size, &offset, &val)) {
                /* Clamp to 0 or 1 for safety, though the codec might handle others */
                ret = aom_codec_control(&codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, (int)(val & 1));
            } else {
                ret = aom_codec_control(&codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, 1);
            }
            break;
        }
        case 2: {
            /* AV1E_SET_DELTALF_MODE: expects an int (mode) */
            uint32_t val = 0;
            if (parse_u32(Data, Size, &offset, &val)) {
                ret = aom_codec_control(&codec, AV1E_SET_DELTALF_MODE, (int)val);
            } else {
                ret = aom_codec_control(&codec, AV1E_SET_DELTALF_MODE, 0);
            }
            break;
        }
        case 3: {
            /* AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE: expects an int */
            uint32_t val = 0;
            if (parse_u32(Data, Size, &offset, &val)) {
                ret = aom_codec_control(&codec, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, (int)(val & 1));
            } else {
                ret = aom_codec_control(&codec, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, 0);
            }
            break;
        }
        case 4: {
            /* AV1E_SET_DISABLE_TRELLIS_QUANT: expects an int */
            uint32_t val = 0;
            if (parse_u32(Data, Size, &offset, &val)) {
                ret = aom_codec_control(&codec, AV1E_SET_DISABLE_TRELLIS_QUANT, (int)(val & 1));
            } else {
                ret = aom_codec_control(&codec, AV1E_SET_DISABLE_TRELLIS_QUANT, 0);
            }
            break;
        }
        case 5: {
            /* AOME_SET_ARNR_STRENGTH: expects an int */
            uint32_t val = 0;
            if (parse_u32(Data, Size, &offset, &val)) {
                ret = aom_codec_control(&codec, AOME_SET_ARNR_STRENGTH, (int)val);
            } else {
                ret = aom_codec_control(&codec, AOME_SET_ARNR_STRENGTH, 0);
            }
            break;
        }
        default: {
            /* Fallback: Try to parse an arbitrary control command and argument */
            uint32_t ctrl_cmd = 0;
            uint32_t ctrl_arg = 0;
            if (parse_u32(Data, Size, &offset, &ctrl_cmd) && 
                parse_u32(Data, Size, &offset, &ctrl_arg)) {
                /* aom_codec_control takes 'int' for ctrl_id, cast is safe */
                ret = aom_codec_control(&codec, (int)ctrl_cmd, ctrl_arg);
            } else {
                /* Do nothing or try a safe default */
                ret = aom_codec_control(&codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, 0);
            }
            break;
        }
    }

    /* 
     * We do not strictly check 'ret' here because the fuzzing goal is to 
     * explore states. Errors are expected. However, we ensure cleanup happens.
     */

    cleanup_encoder(&codec);
    return 0;
}