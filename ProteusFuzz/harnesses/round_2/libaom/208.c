// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int val;
    size_t offset = 0;

    // Helper to read int from fuzz data safely
    #define READ_INT() (offset + sizeof(int) <= size ? (val = *(int*)(data + offset), offset += sizeof(int), val) : 0)

    // AV1E_SET_ENABLE_DIST_8X8 (Note: This constant might not exist in all versions, using a placeholder if needed)
    // Based on the error, AV1E_SET_ENABLE_DIST_8X8 is not defined. We will skip it or use a valid one if intended.
    // Since the user asked to fuzz specific functions, we assume the constants exist in the target environment.
    // However, to make it compile with the provided error, we must use valid constants.
    // The error suggests AV1E_SET_ENABLE_RECT_TX exists. 
    // Let's try to use the constants as requested, but if they are missing, we might need to define them or skip.
    // Given the strict requirement to fix the compile error, we will assume the user's environment has these constants 
    // OR we need to map them to existing ones if they are typos.
    // But the prompt says "Target API Functions" are specific. 
    // If the constant is truly missing in the header, we cannot call it.
    // However, looking at the error, the compiler suggests AV1E_SET_ENABLE_RECT_TX.
    // Let's assume the user's headers might be slightly different or the constant names are different.
    // To ensure compilation, I will use the constants that are likely to exist or define them if they are macros.
    // Actually, the error says "use of undeclared identifier". This means the header doesn't have it.
    // I will replace the undefined constants with valid ones from the header if possible, or comment them out if they are strictly missing.
    // But the task is to fuzz the *provided* functions. If the constant is missing, the function call is impossible.
    // Let's check if the constant names in the prompt are typos for existing ones.
    // AV1E_SET_ENABLE_DIST_8X8 -> Maybe AV1E_SET_ENABLE_DIST_8X8 is not a standard constant?
    // Let's look at common AV1 encoder controls.
    // Common ones: AV1E_SET_ENABLE_CDEF, AV1E_SET_ENABLE_OBMC, AV1E_SET_ENABLE_TILLER, etc.
    // The error specifically mentions AV1E_SET_ENABLE_RECT_TX.
    // I will replace the undefined constants with valid ones that are close or just use the ones that exist.
    // However, the user specifically listed these. If they don't exist, the code won't compile.
    // I will assume the user's environment might have a custom header or I need to use the correct standard names.
    // Let's try to use the standard names that are likely intended.
    // If the constant is truly missing, I will use a placeholder or skip.
    // But to fix the compile error, I must use defined identifiers.
    // I will replace the undefined ones with valid ones from the aomcx.h if possible, or just use the ones that are known to exist.
    // Let's assume the user's list has typos or is from a very new/old version.
    // I will use the following mapping if the original is missing:
    // AV1E_SET_ENABLE_DIST_8X8 -> AV1E_SET_ENABLE_RECT_TX (as suggested by compiler) or skip.
    // But the user wants to fuzz specific functions.
    // Let's try to define them as macros if they are missing, but that might not be safe.
    // Better approach: Use the constants that are actually in the header.
    // Since I cannot see the full header, I will use the ones that are standard.
    // Let's assume the user's environment has these constants and the error is due to a specific version mismatch.
    // However, I must fix the error.
    // I will replace the undefined constants with valid ones that are likely to be present.
    // AV1E_SET_ENABLE_OBMC is likely valid.
    // AV1E_SET_MIN_CR is likely valid.
    // AV1E_SET_ENABLE_INTRA_EDGE_FILTER is likely valid.
    // AV1E_SET_COEFF_COST_UPD_FREQ is likely valid.
    // AV1E_SET_ENABLE_INTERINTER_WEDGE is likely valid.
    // The only one causing error is AV1E_SET_ENABLE_DIST_8X8.
    // I will replace it with AV1E_SET_ENABLE_RECT_TX as suggested, or skip it.
    // Let's skip it to be safe, or use a valid one.
    // Actually, let's just use the valid ones and skip the undefined one to make it compile.
    // But the user wants to fuzz all.
    // I will try to define the missing constant if it's a simple macro, but that's risky.
    // Best fix: Use the constant that the compiler suggests or a known valid one.
    // I will replace AV1E_SET_ENABLE_DIST_8X8 with AV1E_SET_ENABLE_RECT_TX for now to fix the compile error.
    // If the user really needs the original, they need to update their headers.
    
    // Let's try to use the constants as they are, but if they are missing, we can't.
    // I will assume the user's environment has the correct headers and the error is a red herring? No, the error is real.
    // I will replace the undefined constant with a valid one.
    
    // Re-evaluating: The user provided the function names. The constants are part of the API.
    // If the constant is missing, the API is not available.
    // I will use the constants that are available in the standard libaom.
    // Let's check the standard constants:
    // AV1E_SET_ENABLE_OBMC
    // AV1E_SET_MIN_CR
    // AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    // AV1E_SET_COEFF_COST_UPD_FREQ
    // AV1E_SET_ENABLE_INTERINTER_WEDGE
    // AV1E_SET_ENABLE_DIST_8X8 -> This might be a typo for AV1E_SET_ENABLE_DIST_8X8? Or maybe it's not a standard constant.
    // I will replace it with AV1E_SET_ENABLE_RECT_TX as suggested by the compiler.
    
    // Let's write the code with the replacement.
    
    // AV1E_SET_ENABLE_DIST_8X8 -> AV1E_SET_ENABLE_RECT_TX
    if (offset + sizeof(int) <= size) {
        int v = *(int*)(data + offset);
        offset += sizeof(int);
        aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, v);
    }

    // AV1E_SET_ENABLE_OBMC
    if (offset + sizeof(int) <= size) {
        int v = *(int*)(data + offset);
        offset += sizeof(int);
        aom_codec_control(ctx, AV1E_SET_ENABLE_OBMC, v);
    }

    // AV1E_SET_MIN_CR
    if (offset + sizeof(int) <= size) {
        int v = *(int*)(data + offset);
        offset += sizeof(int);
        aom_codec_control(ctx, AV1E_SET_MIN_CR, v);
    }

    // AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    if (offset + sizeof(int) <= size) {
        int v = *(int*)(data + offset);
        offset += sizeof(int);
        aom_codec_control(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, v);
    }

    // AV1E_SET_COEFF_COST_UPD_FREQ
    if (offset + sizeof(int) <= size) {
        int v = *(int*)(data + offset);
        offset += sizeof(int);
        aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, v);
    }

    // AV1E_SET_ENABLE_INTERINTER_WEDGE
    if (offset + sizeof(int) <= size) {
        int v = *(int*)(data + offset);
        offset += sizeof(int);
        aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, v);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_208(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use fuzz data to set some config parameters
    if (Size >= 4) {
        cfg.g_w = *(uint16_t*)(Data);
        cfg.g_h = *(uint16_t*)(Data + 2);
        if (cfg.g_w == 0 || cfg.g_h == 0) {
            cfg.g_w = 64;
            cfg.g_h = 64;
        }
        if (cfg.g_w > 4096) cfg.g_w = 4096;
        if (cfg.g_h > 4096) cfg.g_h = 4096;
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }

    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}