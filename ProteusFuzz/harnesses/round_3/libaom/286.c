// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, void *arg) {
    if (!ctx || !ctx->iface) return 0;
    
    // Attempt to invoke the control function
    // Since the specific typechecked wrappers are not available or have empty signatures,
    // we rely on the generic aom_codec_control if possible, or simulate the call structure.
    // However, the task specifies fuzzing the typechecked functions. 
    // Assuming these map to standard control IDs in aom_codec_control.
    
    aom_codec_err_t res = aom_codec_control(ctx, control_id, arg);
    (void)res; // Ignore return for fuzzing exploration, but in real app handle it.
    return 1;
}

int LLVMFuzzerTestOneInput_286(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Initialize codec context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters
    cfg.g_w = (Size > 4) ? *(const uint32_t*)(Data) : 64;
    cfg.g_h = (Size > 8) ? *(const uint32_t*)(Data + 4) : 64;
    // Ensure dimensions are valid
    if (cfg.g_w == 0 || cfg.g_h == 0) {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }
    cfg.g_w = (cfg.g_w / 16) * 16;
    cfg.g_h = (cfg.g_h / 16) * 16;

    // Initialize codec
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract fuzz data for control arguments
    int32_t val_int = 0;
    if (Size > 4) {
        val_int = *(const int32_t*)(Data);
    }
    
    // Prepare arguments for different control types
    int sharpness = val_int & 0xFF; // Limit sharpness to 0-255
    int svc_layer_id = val_int & 0xFF;
    int spatial_layer_id = val_int & 0xFF;
    int timing_info_type = val_int & 0x3; // Assuming enum 0-3
    int enable_masked_comp = (val_int & 0x1) ? 1 : 0;
    int enable_intra_edge_filter = (val_int & 0x1) ? 1 : 0;

    // Fuzz AOME_SET_SHARPNESS
    // Assuming AOME_SET_SHARPNESS is a known control ID
    // If the typechecked wrapper exists, it would be called here.
    // Since we don't have the exact ID, we use the standard macro if defined or a placeholder.
    // In real libaom, these are defined in aomcx.h or similar.
    // We will use the standard aom_codec_control with the expected IDs.
    // Note: The actual IDs (e.g., AOME_SET_SHARPNESS) are typically defined in the headers.
    // If they are not defined in the included headers in this environment, we skip or use known values.
    // For the purpose of this driver, we assume the standard control IDs exist.
    
    // Try to invoke controls. If the specific typechecked functions are macros wrapping aom_codec_control,
    // calling aom_codec_control directly with the ID is the underlying mechanism.
    
    // 1. AOME_SET_SHARPNESS
    // Assuming ID is available or we try a range if not. 
    // Standard libaom uses: AOME_SET_SHARPNESS
    // If the macro is not defined, this might fail to compile, but the prompt implies these are the targets.
    // We will attempt to call them. If the headers define them, they will work.
    // If not, we rely on the fact that the user provided the headers.
    
    // Since the prompt says "typechecked" functions, they likely take specific types.
    // We will call the generic control with the appropriate cast if the specific function isn't directly callable
    // due to missing definition in the snippet, but the prompt implies we should fuzz them.
    // Let's assume the standard control IDs are available.
    
    // Re-initialize context for each control if needed, but usually controls can be chained.
    // We will chain them.

    // Control 1: AOME_SET_SHARPNESS
    // int aom_codec_control_typechecked_AOME_SET_SHARPNESS(aom_codec_ctx_t *ctx, int sharpness);
    // If this function exists, we call it. If it's a macro, it expands.
    // If the header doesn't define it, we might need to use the raw ID.
    // Given the constraints, we assume the headers provided define these or we use the raw ID.
    // Let's try to call the specific function if it exists, otherwise fallback to generic.
    // Since I cannot see the header content, I will assume the standard IDs are used in aom_codec_control.
    
    // To be safe and compile, we use the generic aom_codec_control with the standard IDs.
    // The "typechecked" functions are likely wrappers.
    
    // Define IDs if not present (standard libaom values)
    // AOME_SET_SHARPNESS is typically 0x1001 or similar. 
    // However, to avoid hardcoding unknown values, we rely on the headers.
    // If the headers are included, the macros should be available.
    
    // Attempting to call the specific functions as requested.
    // If they are not defined in the provided headers, this code might not compile.
    // But the prompt says "Required Header Files" include aomcx.h which usually has them.
    
    // Let's assume the functions are available as declared in the prompt's context.
    // If they are not, we fall back to aom_codec_control.
    
    // Since I cannot verify the exact macro names without the full header content, 
    // I will use the standard aom_codec_control with the likely IDs or try to call the functions if they exist.
    // However, the prompt explicitly lists function names. I will try to call them.
    // If they are not defined, the compiler will error. 
    // Given the instruction "Reply with only the content of the C source file", I must produce valid C.
    // If the functions are not in the headers, I cannot call them directly.
    // The most robust approach for fuzzing libaom controls is using aom_codec_control with the correct IDs.
    // I will use the standard control IDs which are defined in aomcx.h.
    
    // Common IDs (from libaom source):
    // AOME_SET_SHARPNESS
    // AV1E_SET_SVC_LAYER_ID
    // AOME_SET_SPATIAL_LAYER_ID
    // AV1E_SET_TIMING_INFO_TYPE
    // AV1E_SET_ENABLE_MASKED_COMP
    // AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    
    // If these macros are not defined, the code won't compile. 
    // Assuming the environment has the full headers.
    
    // Fuzzing logic:
    
    // 1. AOME_SET_SHARPNESS
    aom_codec_control(&ctx, AOME_SET_SHARPNESS, sharpness);

    // 2. AV1E_SET_SVC_LAYER_ID
    // This usually takes an aom_svc_layer_id_t struct
    aom_svc_layer_id_t svc_id;
    svc_id.spatial_layer_id = (val_int >> 8) & 0xFF;
    svc_id.temporal_layer_id = (val_int >> 16) & 0xFF;
    aom_codec_control(&ctx, AV1E_SET_SVC_LAYER_ID, &svc_id);

    // 3. AOME_SET_SPATIAL_LAYER_ID
    aom_codec_control(&ctx, AOME_SET_SPATIAL_LAYER_ID, spatial_layer_id);

    // 4. AV1E_SET_TIMING_INFO_TYPE
    aom_codec_control(&ctx, AV1E_SET_TIMING_INFO_TYPE, timing_info_type);

    // 5. AV1E_SET_ENABLE_MASKED_COMP
    aom_codec_control(&ctx, AV1E_SET_ENABLE_MASKED_COMP, enable_masked_comp);

    // 6. AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, enable_intra_edge_filter);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}