#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include "lcms2.h"
#include "lcms2_plugin.h"

namespace harness_017 {
/*
 * Fuzzing harness for LittleCMS2 library
 * Targets: Optimization, smoothing, and white point APIs in cmsopt.c, cmssm.c, and cmswtpnt.c modules
 * Semantic domain: Pipeline optimization algorithms, tone curve smoothing operations, 
 *   white point temperature conversion and adaptation, gamut boundary descriptor operations
 * Previous harness semantic domains covered:
 *   000-basic profile parsing/transformation
 *   001-proofing/PostScript
 *   002-virtual profiles/gamut
 *   003-CGATS/IT8 basic
 *   004-CIECAM02 (appearance model)
 *   005-TAC detection/sampling
 *   006-device link creation/I/O
 *   007-BCHSW abstract profiles
 *   008-advanced CGATS/IT8
 *   009-cube file device link creation/stream I/O
 *   010-pipeline/MPE operations
 *   011-PostScript color resources
 *   012-MD5/half-precision
 *   013-context management/plugin system
 *   014-color science algorithms and metrics
 *   015-named color list management
 *   016-alpha compositing/gamma correction
 * This harness focuses on: Pipeline optimization (_cmsOptimizePipeline), tone curve smoothing (cmsSmoothToneCurve),
 *   white point temperature functions (cmsWhitePointFromTemp, cmsTempFromWhitePoint, cmsAdaptToIlluminant),
 *   gamut boundary descriptor operations (cmsGBDAlloc, cmsGBDFree, cmsGDBAddPoint, cmsGDBCheckPoint, cmsGDBCompute)
 */


// Include Little CMS headers

// Declare internal functions that we want to test
// These are defined in the library but not in public headers
extern "C" {
// Pipeline optimization function from cmsopt.c
cmsBool _cmsOptimizePipeline(cmsContext ContextID,
                            cmsPipeline** PtrLut,
                            cmsUInt32Number Intent,
                            cmsUInt32Number* InputFormat,
                            cmsUInt32Number* OutputFormat,
                            cmsUInt32Number* dwFlags);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 256) {
        return 0;  // Minimum input size for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Create a context for the operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }
    
    // Phase 1: White point and temperature operations
    
    // 1.1 Get standard D50 white points
    const cmsCIEXYZ* d50_xyz = cmsD50_XYZ();
    const cmsCIExyY* d50_xyY = cmsD50_xyY();
    
    // 1.2 Convert temperature to white point
    cmsCIExyY white_point_from_temp;
    cmsFloat64Number temp_k = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(4000.0, 25000.0);
    cmsBool temp_to_wp_success = cmsWhitePointFromTemp(&white_point_from_temp, temp_k);
    
    // 1.3 Convert white point back to temperature (if previous conversion succeeded)
    cmsFloat64Number recovered_temp = 0.0;
    cmsBool wp_to_temp_success = FALSE;
    if (temp_to_wp_success) {
        wp_to_temp_success = cmsTempFromWhitePoint(&recovered_temp, &white_point_from_temp);
    }
    
    // 1.4 Test adaptation between illuminants
    cmsCIEXYZ source_xyz, dest_xyz, adapted_xyz;
    
    // Create source and destination white points from fuzzed data
    source_xyz.X = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    source_xyz.Y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    source_xyz.Z = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    
    dest_xyz.X = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    dest_xyz.Y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    dest_xyz.Z = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    
    // Convert XYZ to xyY for adaptation
    cmsCIExyY source_xyY, dest_xyY;
    cmsXYZ2xyY(&source_xyY, &source_xyz);
    cmsXYZ2xyY(&dest_xyY, &dest_xyz);
    
    // Create a test XYZ value to adapt
    cmsCIEXYZ test_xyz;
    test_xyz.X = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    test_xyz.Y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    test_xyz.Z = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    
    // Perform adaptation
    cmsBool adapt_success = cmsAdaptToIlluminant(&adapted_xyz, &source_xyz, &dest_xyz, &test_xyz);
    
    // Phase 2: Create tone curves for smoothing operations
    
    // 2.1 Create a gamma curve
    cmsFloat64Number gamma_value = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.5, 3.0);
    cmsToneCurve* gamma_curve = cmsBuildGamma(context, gamma_value);
    
    // 2.2 Create a tabulated tone curve with random values
    cmsUInt32Number tab_entries = fdp.ConsumeIntegralInRange<cmsUInt32Number>(10, 100);
    std::vector<cmsUInt16Number> tab_values(tab_entries);
    
    for (cmsUInt32Number i = 0; i < tab_entries; i++) {
        tab_values[i] = fdp.ConsumeIntegralInRange<cmsUInt16Number>(0, 65535);
    }
    
    cmsToneCurve* tab_curve = cmsBuildTabulatedToneCurve16(context, tab_entries, tab_values.data());
    
    // 2.3 Apply smoothing to the tabulated curve
    cmsBool smooth_success = FALSE;
    if (tab_curve != NULL) {
        cmsFloat64Number lambda = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 10.0);
        smooth_success = cmsSmoothToneCurve(tab_curve, lambda);
    }
    
    // Phase 3: Gamut Boundary Descriptor (GBD) operations
    
    // 3.1 Allocate GBD handle
    cmsHANDLE gbd_handle = cmsGBDAlloc(context);
    
    if (gbd_handle != NULL) {
        // 3.2 Add Lab points to GBD
        cmsUInt32Number num_points = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 20);
        
        for (cmsUInt32Number i = 0; i < num_points; i++) {
            cmsCIELab lab_point;
            lab_point.L = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 100.0);
            lab_point.a = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-128.0, 128.0);
            lab_point.b = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-128.0, 128.0);
            
            cmsBool add_success = cmsGDBAddPoint(gbd_handle, &lab_point);
            
            // Check if the added point is inside the gamut
            cmsBool check_success = cmsGDBCheckPoint(gbd_handle, &lab_point);
        }
        
        // 3.3 Compute the gamut boundary
        cmsUInt32Number gbd_flags = fdp.ConsumeIntegral<cmsUInt32Number>();
        cmsBool compute_success = cmsGDBCompute(gbd_handle, gbd_flags);
        
        // 3.4 Free the GBD handle
        cmsGBDFree(gbd_handle);
    }
    
    // Phase 4: Pipeline optimization (requires creating a pipeline first)
    
    // 4.1 Create a simple pipeline for testing optimization
    cmsPipeline* test_pipeline = cmsPipelineAlloc(context, 3, 3);
    if (test_pipeline != NULL) {
        // Add a gamma curve stage
        if (gamma_curve != NULL) {
            // Create an array of 3 tone curve pointers as required by cmsStageAllocToneCurves
            cmsToneCurve* curves[3] = {gamma_curve, gamma_curve, gamma_curve};
            cmsPipelineInsertStage(test_pipeline, cmsAT_END, cmsStageAllocToneCurves(context, 3, curves));
        }
        
        // Add a matrix stage
        cmsFloat64Number matrix[9];
        for (int i = 0; i < 9; i++) {
            matrix[i] = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(-2.0, 2.0);
        }
        
        cmsStage* matrix_stage = cmsStageAllocMatrix(context, 3, 3, matrix, NULL);
        if (matrix_stage != NULL) {
            cmsPipelineInsertStage(test_pipeline, cmsAT_END, matrix_stage);
        }
        
        // 4.2 Set up parameters for optimization
        cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
        cmsUInt32Number input_format = fdp.ConsumeIntegral<cmsUInt32Number>();
        cmsUInt32Number output_format = fdp.ConsumeIntegral<cmsUInt32Number>();
        cmsUInt32Number flags = fdp.ConsumeIntegral<cmsUInt32Number>();
        
        // 4.3 Call the internal optimization function
        // Note: _cmsOptimizePipeline is an internal function, but we're including lcms2_internal.h
        cmsBool optimize_success = _cmsOptimizePipeline(context, &test_pipeline, intent, 
                                                       &input_format, &output_format, &flags);
        
        // Clean up the pipeline
        cmsPipelineFree(test_pipeline);
    }
    
    // Phase 5: Clean up remaining resources
    
    if (gamma_curve != NULL) {
        cmsFreeToneCurve(gamma_curve);
    }
    
    if (tab_curve != NULL) {
        cmsFreeToneCurve(tab_curve);
    }
    
    // Destroy context
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_017
