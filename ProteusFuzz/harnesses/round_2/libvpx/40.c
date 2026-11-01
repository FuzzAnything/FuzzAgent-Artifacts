// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_TUNING__value at vp8cx.h:1006:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

/* 
 * Helper to read a 32-bit integer from the fuzz input buffer safely.
 * Returns 0 if out of bounds.
 */
static uint32_t read_u32(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(uint32_t) > size) {
        return 0;
    }
    uint32_t val = 0;
    memcpy(&val, data + *offset, sizeof(uint32_t));
    *offset += sizeof(uint32_t);
    return val;
}

/* 
 * Helper to read a 16-bit integer from the fuzz input buffer safely.
 */
static uint16_t read_u16(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(uint16_t) > size) {
        return 0;
    }
    uint16_t val = 0;
    memcpy(&val, data + *offset, sizeof(uint16_t));
    *offset += sizeof(uint16_t);
    return val;
}

/* 
 * Helper to read a 8-bit integer from the fuzz input buffer safely.
 */
static uint8_t read_u8(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(uint8_t) > size) {
        return 0;
    }
    uint8_t val = data[*offset];
    *offset += sizeof(uint8_t);
    return val;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    /* Initialize configuration with some defaults manually since vpx_codec_enc_cfg_init is not available in all builds */
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.rc_end_usage = VPX_VBR;
    cfg.rc_target_bitrate = 100;
    cfg.kf_mode = VPX_KF_DISABLED;
    cfg.g_pass = VPX_RC_ONE_PASS;

    size_t offset = 0;

    /* 
     * Fuzz configuration parameters based on input data 
     * to explore different encoder states.
     */
    if (Size > offset) cfg.g_w = read_u16(Data, Size, &offset);
    if (Size > offset) cfg.g_h = read_u16(Data, Size, &offset);
    if (Size > offset) cfg.g_threads = read_u8(Data, Size, &offset) & 0x07; /* 0-7 */
    if (Size > offset) cfg.g_profile = read_u8(Data, Size, &offset) & 0x03; /* 0-3 */
    if (Size > offset) cfg.rc_target_bitrate = read_u32(Data, Size, &offset);
    if (Size > offset) cfg.kf_mode = read_u8(Data, Size, &offset) % 3; /* 0-2 */
    if (Size > offset) cfg.rc_end_usage = read_u8(Data, Size, &offset) % 4; /* 0-3 */
    
    /* Ensure minimum dimensions to avoid immediate errors */
    if (cfg.g_w < 16) cfg.g_w = 16;
    if (cfg.g_h < 16) cfg.g_h = 16;

    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        /* Fallback if VP8 is not available */
        return 0;
    }

    /* Fuzz flags */
    vpx_codec_flags_t flags = 0;
    if (Size > offset) {
        uint8_t flag_byte = read_u8(Data, Size, &offset);
        if (flag_byte & 0x01) flags |= VPX_CODEC_USE_PSNR;
        /* VPX_CODEC_USE_FRAME_CONTEXT is not a standard flag in recent libvpx versions, removed */
    }

    /* 
     * Call vpx_codec_enc_init_ver 
     * This is the primary target function.
     */
    vpx_codec_err_t init_res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);

    if (init_res == VPX_CODEC_OK) {
        /* 
         * If initialization succeeds, try to invoke control commands 
         * to fuzz vpx_codec_control_ via the macro or direct call logic.
         */

        /* Read a control ID from input to diversify calls */
        if (Size > offset) {
            int ctrl_id = (int)read_u32(Data, Size, &offset);
            
            /* 
             * Map random values to known control IDs to trigger specific handlers.
             * We also pass raw values to test error handling for invalid IDs.
             */
            int actual_ctrl_id = ctrl_id;
            
            /* Try specific known control IDs based on input bits */
            if (Size > offset) {
                uint8_t cmd_type = read_u8(Data, Size, &offset);
                switch (cmd_type % 5) {
                    case 0: actual_ctrl_id = VP8E_SET_CPUUSED; break;
                    case 1: actual_ctrl_id = VP8E_SET_TUNING; break;
                    case 2: actual_ctrl_id = VP8E_SET_TEMPORAL_LAYER_ID; break;
                    case 3: actual_ctrl_id = VP8E_SET_CPUUSED; break; /* Duplicate to ensure coverage */
                    case 4: actual_ctrl_id = ctrl_id; break; /* Raw fuzzed ID */
                }
            }

            /* 
             * Prepare arguments for the control call.
             * vpx_codec_control_ is a varargs function.
             * We use the macro vpx_codec_control for safety and standard usage.
             */
            vpx_codec_err_t ctrl_res = VPX_CODEC_ERROR;

            /* 
             * Fuzz arguments for specific control IDs.
             * Most controls expect an integer argument.
             */
            int arg_val = 0;
            if (Size > offset) {
                arg_val = (int)read_u32(Data, Size, &offset);
            }

            /* 
             * Invoke the control.
             * We use the macro which expands to vpx_codec_control_ internally.
             * Note: The macro requires the ID to be a literal or defined constant for the token pasting to work correctly.
             * Since actual_ctrl_id is a variable, the macro expansion vpx_codec_control_##id will fail if used directly with a variable.
             * Therefore, we use a switch statement with literal IDs for the macro, and call the underlying function directly for dynamic IDs.
             */
            switch (actual_ctrl_id) {
                case VP8E_SET_CPUUSED:
                    ctrl_res = vpx_codec_control(&ctx, VP8E_SET_CPUUSED, arg_val);
                    break;
                case VP8E_SET_TUNING:
                    ctrl_res = vpx_codec_control(&ctx, VP8E_SET_TUNING, arg_val);
                    break;
                case VP8E_SET_TEMPORAL_LAYER_ID:
                    ctrl_res = vpx_codec_control(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, arg_val);
                    break;
                default:
                    /* For dynamic IDs, call the underlying function directly to avoid macro expansion issues */
                    ctrl_res = vpx_codec_control_(&ctx, actual_ctrl_id, arg_val);
                    break;
            }
            (void)ctrl_res; /* Ignore return value for fuzzing stability */
        }

        /* Cleanup encoder context */
        vpx_codec_destroy(&ctx);
    }

    return 0;
}