#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <lcms2.h>
}
//<ID> 263
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 4
//<Quality> {"density":38,"unique_branches":{"cmsFloat2XYZEncoded":[[383,9,383,19,0,0,4,0],[390,9,390,35,0,0,4,0],[396,9,396,35,0,0,4,0],[402,9,402,35,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenIOhandlerFromNULL","cmsDeleteContext","cmsCreateXYZProfileTHR","cmsCreateLab4Profile","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext","cmsSetHeaderRenderingIntent","cmsPipelineAlloc","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext","cmsBuildGamma","cmsBuildTabulatedToneCurve16","cmsIsToneCurveDescending","cmsReverseToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineSetSaveAs8bitsFlag","cmsCreateTransformTHR","cmsGetColorSpace","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8Free","cmsFloat2XYZEncoded","cmsDoTransform","cmsSaveProfileToStream","cmsDeleteTransform","cmsFreeToneCurve","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenIOhandlerFromNULL","cmsCreateXYZProfileTHR","cmsCreateLab4Profile","cmsSetHeaderRenderingIntent","cmsPipelineAlloc","cmsBuildGamma","cmsBuildTabulatedToneCurve16","cmsIsToneCurveDescending","cmsReverseToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineSetSaveAs8bitsFlag","cmsCreateTransformTHR","cmsGetColorSpace","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8Free","cmsFloat2XYZEncoded","cmsDoTransform","cmsSaveProfileToStream","cmsDeleteTransform","cmsFreeToneCurve","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // Need minimum data for operations

    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	

    // Create NULL IO handler
    cmsIOHANDLER* io = cmsOpenIOhandlerFromNULL(ctx);
    if (!io) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Create profiles
    cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(ctx);
    cmsHPROFILE labProfile = cmsCreateLab4Profile(NULL); // NULL uses default white point
    
    if (!xyzProfile || !labProfile) {
        cmsCloseProfile(labProfile);
        cmsCloseProfile(xyzProfile);
        cmsCloseIOhandler(io);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Set rendering intent using data[0] % 4
    cmsSetHeaderRenderingIntent(xyzProfile, data[0] % 4);

    // Create pipeline for color transformation
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3); // XYZ to Lab conversion
    if (!pipeline) {
        cmsCloseProfile(labProfile);
        cmsCloseProfile(xyzProfile);
        cmsCloseIOhandler(io);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Build tone curves from input data
    cmsFloat64Number gamma = 1.0 + (data[1] / 255.0) * 4.0; // Gamma between 1.0 and 5.0
    cmsToneCurve* toneCurve = cmsBuildGamma(ctx, gamma);
    
    // Create tabulated tone curve if we have enough data
    if (size >= 12) {
        cmsUInt16Number values[6];
        for (int i = 0; i < 6 && (i*2+2) < size; i++) {
            values[i] = (data[i*2] << 8) | data[i*2+1];
        }
        cmsToneCurve* tabulatedCurve = cmsBuildTabulatedToneCurve16(ctx, 6, values);
        if (tabulatedCurve) {
            // Check if descending and possibly reverse
            if (cmsIsToneCurveDescending(tabulatedCurve)) {
                cmsToneCurve* reversed = cmsReverseToneCurve(tabulatedCurve);
                if (reversed) {
                    cmsFreeToneCurve(tabulatedCurve);
                    tabulatedCurve = reversed;
                }
            }
            cmsFreeToneCurve(tabulatedCurve);
        }
    }

    // Create array of tone curves for pipeline stage
    cmsToneCurve* curves[3] = {toneCurve, toneCurve, toneCurve};
    cmsStage* toneCurveStage = cmsStageAllocToneCurves(ctx, 3, curves);
    
    if (toneCurveStage) {
        cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, toneCurveStage);
        cmsPipelineSetSaveAs8bitsFlag(pipeline, (data[2] & 1) ? TRUE : FALSE);
    }

    // Create transform using the profiles
    cmsUInt32Number intent = data[3] % 4;
    cmsHTRANSFORM transform = cmsCreateTransformTHR(
        ctx, xyzProfile, TYPE_XYZ_16,
        labProfile, TYPE_Lab_16,
        intent, 0
    );

    // Test color space
    cmsColorSpaceSignature cs = cmsGetColorSpace(xyzProfile);

    // Create IT8 handle and define format
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    if (it8) {
        cmsIT8DefineDblFormat(it8, "%.6f");
        cmsIT8Free(it8);
    }

    // Process some color data if we have enough
    if (transform && size >= 18) {
        // Prepare input/output buffers
        cmsUInt16Number input[3], output[3];
        
        // Create encoded XYZ values from input data
        cmsCIEXYZ xyz;
        xyz.X = (data[4] / 255.0) * 100.0; // Scale to reasonable range
        xyz.Y = (data[5] / 255.0) * 100.0;
        xyz.Z = (data[6] / 255.0) * 100.0;
        
        cmsFloat2XYZEncoded(input, &xyz);
        
        // Perform transformation
        cmsDoTransform(transform, input, output, 1);
    }

    // Save profile to a file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        cmsSaveProfileToStream(labProfile, out_file);
        assert_file_closed(&out_file);;
    }

    // Read input data from memory stream for additional testing
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd; // Use fd to avoid unused variable warning
        
        // Read a byte to consume data
        uint8_t byte;
        if (fread(&byte, 1, 1, in_file) == 1) {
            // Data consumed
        }
        assert_file_closed(&in_file);;
    }

    // Cleanup - order matters!
    if (transform) cmsDeleteTransform(transform);
    cmsFreeToneCurve(toneCurve);
    cmsPipelineFree(pipeline);
    cmsCloseProfile(labProfile);
    cmsCloseProfile(xyzProfile);
    cmsCloseIOhandler(io);
    cmsDeleteContext(ctx);

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
}