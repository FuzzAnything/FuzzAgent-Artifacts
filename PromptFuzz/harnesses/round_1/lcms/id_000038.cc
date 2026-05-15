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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 0
//<Quality> {"density":30,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsOpenIOhandlerFromNULL","cmsCreateXYZProfileTHR","cmsCreateLab4Profile","cmsSetHeaderRenderingIntent","cmsFloat2XYZEncoded","cmsPipelineAlloc","cmsBuildGamma","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineSetSaveAs8bitsFlag","cmsIsToneCurveDescending","cmsBuildTabulatedToneCurve16","cmsReverseToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsGetColorSpace","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8Free","cmsSaveProfileToStream","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenIOhandlerFromNULL","cmsCreateXYZProfileTHR","cmsCreateLab4Profile","cmsSetHeaderRenderingIntent","cmsFloat2XYZEncoded","cmsPipelineAlloc","cmsBuildGamma","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineSetSaveAs8bitsFlag","cmsIsToneCurveDescending","cmsBuildTabulatedToneCurve16","cmsReverseToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsGetColorSpace","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsIT8Alloc","cmsIT8DefineDblFormat","cmsIT8Free","cmsSaveProfileToStream","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
	return 0;
	}
	 // Not enough data for meaningful processing
    
    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // Open input file from memory
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create IO handler from NULL (required API usage)
    cmsIOHANDLER* null_io = cmsOpenIOhandlerFromNULL(context);
    
    // Create profiles
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(context);
    cmsHPROFILE lab_profile = cmsCreateLab4Profile(nullptr); // Default white point
    
    // Set rendering intent for XYZ profile
    cmsSetHeaderRenderingIntent(xyz_profile, INTENT_PERCEPTUAL);
    
    // Convert float XYZ to encoded (example conversion)
    cmsCIEXYZ float_xyz = {0.5, 0.5, 0.5};
    cmsUInt16Number encoded_xyz[3];
    cmsFloat2XYZEncoded(encoded_xyz, &float_xyz);
    
    // Create pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (pipeline) {
        // Build gamma curves
        cmsToneCurve* gamma_curve = cmsBuildGamma(context, 2.2);
        if (gamma_curve) {
            // Create array of tone curves for the stage
            cmsToneCurve* curves[3] = {gamma_curve, gamma_curve, gamma_curve};
            
            // Create tone curves stage
            cmsStage* tone_stage = cmsStageAllocToneCurves(context, 3, curves);
            if (tone_stage) {
                // Insert stage into pipeline
                cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, tone_stage);
            }
            
            // Set pipeline save as 8 bits flag
            cmsPipelineSetSaveAs8bitsFlag(pipeline, TRUE);
            
            // Check if tone curve is descending
            cmsIsToneCurveDescending(gamma_curve);
            
            // Build tabulated tone curve from input data
            // Use first portion of input data for tone curve values
            const size_t max_entries = 256;
            size_t entries = size / 2;
            if (entries > max_entries) entries = max_entries;
            
            cmsUInt16Number* table_data = new cmsUInt16Number[entries];
            if (table_data) {
                // Copy data from input, ensure proper alignment
                size_t bytes_to_copy = entries * sizeof(cmsUInt16Number);
                if (bytes_to_copy > size) bytes_to_copy = size;
                memcpy(table_data, data, bytes_to_copy);
                
                cmsToneCurve* tabulated_curve = cmsBuildTabulatedToneCurve16(
                    context, static_cast<cmsUInt32Number>(entries), table_data);
                
                if (tabulated_curve) {
                    // Reverse the tabulated curve
                    cmsToneCurve* reversed_curve = cmsReverseToneCurve(tabulated_curve);
                    if (reversed_curve) {
                        cmsFreeToneCurve(reversed_curve);
                    }
                    cmsFreeToneCurve(tabulated_curve);
                }
                delete[] table_data;
            }
            
            // Free gamma curve
            cmsFreeToneCurve(gamma_curve);
        }
    }
    
    // Get color space of XYZ profile
    cmsColorSpaceSignature colorspace = cmsGetColorSpace(xyz_profile);
    
    // Create transform
    cmsHTRANSFORM transform = cmsCreateTransformTHR(
        context,
        xyz_profile,
        TYPE_XYZ_16,
        lab_profile,
        TYPE_Lab_16,
        INTENT_PERCEPTUAL,
        0);
    
    if (transform) {
        // Prepare buffers for transformation
        cmsUInt16Number input_buffer[3];
        cmsUInt16Number output_buffer[3];
        
        // Use encoded XYZ values as input
        memcpy(input_buffer, encoded_xyz, sizeof(encoded_xyz));
        
        // Perform transformation
        cmsDoTransform(transform, input_buffer, output_buffer, 1);
        
        // Delete transform
        cmsDeleteTransform(transform);
    }
    
    // Allocate IT8 handle
    cmsHANDLE it8_handle = cmsIT8Alloc(context);
    if (it8_handle) {
        // Define double format
        cmsIT8DefineDblFormat(it8_handle, "%.6f");
        // Free IT8 handle
        cmsIT8Free(it8_handle);
    }
    
    // Save profile to stream
    rewind(in_file); // Reset to beginning for reading
    cmsSaveProfileToStream(xyz_profile, out_file);
    
    // Cleanup pipeline
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    // Close profiles
    cmsCloseProfile(xyz_profile);
    cmsCloseProfile(lab_profile);
    
    // Close IO handlers
    if (null_io) {
        cmsCloseIOhandler(null_io);
    }
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Delete context
    cmsDeleteContext(context);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}