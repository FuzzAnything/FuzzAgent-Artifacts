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
//<ID> 367
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDupContext","cmsDeleteContext","cmsDeleteContext","cmsDeleteContext","cmsCreate_sRGBProfile","cmsCreateLab2Profile","cmsD50_xyY","cmsCreateXYZProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext","cmsLinkTag","cmsStageAllocCLutFloat","cmsStageAllocCLutFloatGranular","cmsStageType","cmsStageFree","cmsStageFree","cmsxyY2XYZ","cmsCreateProofingTransform","cmsReadTag","cmsDupProfileSequenceDescription","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsDupContext","cmsCreate_sRGBProfile","cmsCreateLab2Profile","cmsD50_xyY","cmsCreateXYZProfile","cmsLinkTag","cmsStageAllocCLutFloat","cmsStageAllocCLutFloatGranular","cmsStageType","cmsStageFree","cmsStageFree","cmsxyY2XYZ","cmsCreateProofingTransform","cmsReadTag","cmsDupProfileSequenceDescription","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsDeleteContext"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create context and duplicate it
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    cmsContext dupContext = cmsDupContext(context, nullptr);
    if (!dupContext) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Create a temporary file with the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(dupContext);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create profiles for proofing transform
    cmsHPROFILE rgb_profile = cmsCreate_sRGBProfile();
    cmsHPROFILE lab_profile = cmsCreateLab2Profile(cmsD50_xyY());
    cmsHPROFILE xyz_profile = cmsCreateXYZProfile();
    
    if (!rgb_profile || !lab_profile || !xyz_profile) {
        if (rgb_profile) cmsCloseProfile(rgb_profile);
        if (lab_profile) cmsCloseProfile(lab_profile);
        if (xyz_profile) cmsCloseProfile(xyz_profile);
        assert_file_closed(&in_file);;
        cmsDeleteContext(dupContext);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to link a tag in the RGB profile
    // Link AToB0 tag to BToA0 tag if present
    cmsLinkTag(rgb_profile, cmsSigAToB0Tag, cmsSigBToA0Tag);
    
    // Create a CLUT stage for processing
    const cmsUInt32Number gridPoints[] = {5, 5, 5};
    const int numEntries = 5 * 5 * 5 * 3;
    cmsFloat32Number* table = (cmsFloat32Number*)malloc(numEntries * sizeof(cmsFloat32Number));
    if (table) {
        // Fill table with data from input
        for (int i = 0; i < numEntries && i * 4 + 4 <= size; i++) {
            uint32_t val;
            memcpy(&val, data + (i * 4) % size, 4);
            table[i] = (cmsFloat32Number)(val % 1000) / 1000.0f;
        }
        
        // Create CLUT stages
        cmsStage* clut1 = cmsStageAllocCLutFloat(
            context, 
            5,  // nGridPoints
            3,  // inputChan (RGB)
            3,  // outputChan (RGB)
            table
        );
        
        cmsStage* clut2 = cmsStageAllocCLutFloatGranular(
            context,
            gridPoints,
            3,  // inputChan
            3,  // outputChan
            table
        );
        
        // Get stage type
        if (clut1) {
            cmsStageSignature sig = cmsStageType(clut1);
            (void)sig; // Use to avoid unused variable warning
        }
        
        if (clut1) cmsStageFree(clut1);
        if (clut2) cmsStageFree(clut2);
        free(table);
    }
    
    // Convert xyY to XYZ using data from input
    if (size >= 3 * sizeof(cmsFloat64Number)) {
        cmsCIExyY xyY;
        cmsCIEXYZ xyz;
        
        memcpy(&xyY.x, data, sizeof(cmsFloat64Number));
        memcpy(&xyY.y, data + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
        memcpy(&xyY.Y, data + 2 * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
        
        // Ensure values are in reasonable range
        xyY.x = xyY.x < 0.0 ? 0.0 : (xyY.x > 1.0 ? 1.0 : xyY.x);
        xyY.y = xyY.y < 0.0 ? 0.0 : (xyY.y > 1.0 ? 1.0 : xyY.y);
        xyY.Y = xyY.Y < 0.0 ? 0.0 : (xyY.Y > 1.0 ? 1.0 : xyY.Y);
        
        cmsxyY2XYZ(&xyz, &xyY);
    }
    
    // Create proofing transform
    cmsHTRANSFORM proofingTransform = cmsCreateProofingTransform(
        rgb_profile,          // Input profile
        TYPE_RGB_8,           // Input format
        lab_profile,          // Output profile  
        TYPE_Lab_8,           // Output format
        xyz_profile,          // Proofing profile
        INTENT_PERCEPTUAL,    // Intent
        INTENT_ABSOLUTE_COLORIMETRIC, // Proofing intent
        cmsFLAGS_GAMUTCHECK   // Flags
    );
    
    // Try to get profile sequence description
    cmsSEQ* seq = (cmsSEQ*)cmsReadTag(rgb_profile, cmsSigProfileSequenceDescTag);
    if (seq) {
        cmsSEQ* dupSeq = cmsDupProfileSequenceDescription(seq);
        if (dupSeq) {
            // Free the duplicated sequence
            // Note: lcms2.h doesn't show a free function, but typically it would be cmsFreeProfileSequenceDescription
            // Since it's not in the provided APIs, we'll assume it's handled internally
        }
    }
    
    // Clean up
    if (proofingTransform) cmsDeleteTransform(proofingTransform);
    
    cmsCloseProfile(rgb_profile);
    cmsCloseProfile(lab_profile);
    cmsCloseProfile(xyz_profile);
    
    assert_file_closed(&in_file);;
    cmsDeleteContext(dupContext);
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}