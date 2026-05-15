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
//<ID> 133
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 7
//<Quality> {"density":13,"unique_branches":{"CubeSize":[[471,13,471,21,0,0,4,0],[480,9,480,27,0,0,4,0]],"cmsCreateBCHSWabstractProfileTHR":[[914,9,914,21,0,0,4,0]],"cmsStageAllocCLut16bitGranular":[[581,9,581,15,0,0,4,0],[588,9,588,32,0,0,4,0]],"CLutElemTypeFree":[[538,9,538,22,0,0,4,1]],"_cmsFreeInterpParams":[[179,9,179,18,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsIT8Free","cmsCreate_OkLabProfile","cmsCreateBCHSWabstractProfileTHR","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandler2THR","cmsGetHeaderFlags","cmsDetectDestinationBlackPoint","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsNamedColorInfo","cmsCloseIOhandler","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsIT8Free","cmsCreate_OkLabProfile","cmsCreateBCHSWabstractProfileTHR","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandler2THR","cmsGetHeaderFlags","cmsDetectDestinationBlackPoint","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsNamedColorInfo","cmsCloseIOhandler","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to achieve the described event:

*/


// Include necessary lcms headers

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return for insufficient data
    if (size < 100) {
	return 0;
	}
	
    
    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // Variables to track allocations for cleanup
    cmsHANDLE it8 = nullptr;
    cmsIOHANDLER* io_handler = nullptr;
    cmsHPROFILE profiles[3] = {nullptr, nullptr, nullptr}; // For OkLab, BCHSW, and input profile
    cmsHTRANSFORM transform = nullptr;
    cmsNAMEDCOLORLIST* namedColorList = nullptr;
    void* saved_profile_buffer = nullptr;
    
    // 1. Load IT8 data from memory
    it8 = cmsIT8LoadFromMem(ctx, data, size);
    
    // 2. If IT8 loaded successfully, enumerate data format
    if (it8) {
        char** sampleNames = nullptr;
        int formatCount = cmsIT8EnumDataFormat(it8, &sampleNames);
        // Free IT8 after use
        cmsIT8Free(it8);
        it8 = nullptr;
    }
    
    // 3. Create OkLab profile
    profiles[0] = cmsCreate_OkLabProfile(ctx);
    
    // 4. Create BCHSW abstract profile with parameters from input data
    // Use first few bytes for parameters (with bounds checking)
    size_t paramOffset = 0;
    if (size >= 12) {
        cmsUInt32Number nLUTPoints = 256;
        if (size >= paramOffset + 4) nLUTPoints = *reinterpret_cast<const cmsUInt32Number*>(data + paramOffset) % 1024;
        paramOffset += 4;
        
        cmsFloat64Number bright = 1.0;
        cmsFloat64Number contrast = 1.0;
        cmsFloat64Number hue = 0.0;
        cmsFloat64Number saturation = 1.0;
        cmsUInt32Number tempSrc = 5000;
        cmsUInt32Number tempDest = 6500;
        
        if (size >= paramOffset + 8) bright = *reinterpret_cast<const cmsFloat64Number*>(data + paramOffset);
        paramOffset += 8;
        if (size >= paramOffset + 8) contrast = *reinterpret_cast<const cmsFloat64Number*>(data + paramOffset);
        paramOffset += 8;
        if (size >= paramOffset + 8) hue = *reinterpret_cast<const cmsFloat64Number*>(data + paramOffset);
        paramOffset += 8;
        if (size >= paramOffset + 8) saturation = *reinterpret_cast<const cmsFloat64Number*>(data + paramOffset);
        paramOffset += 8;
        if (size >= paramOffset + 4) tempSrc = *reinterpret_cast<const cmsUInt32Number*>(data + paramOffset) % 10000;
        paramOffset += 4;
        if (size >= paramOffset + 4) tempDest = *reinterpret_cast<const cmsUInt32Number*>(data + paramOffset) % 10000;
        
        // Ensure reasonable values
        bright = (bright < 0.0) ? 0.0 : (bright > 2.0) ? 2.0 : bright;
        contrast = (contrast < 0.0) ? 0.0 : (contrast > 2.0) ? 2.0 : contrast;
        saturation = (saturation < 0.0) ? 0.0 : (saturation > 2.0) ? 2.0 : saturation;
        
        profiles[1] = cmsCreateBCHSWabstractProfileTHR(ctx, nLUTPoints, bright, contrast, 
                                                      hue, saturation, tempSrc, tempDest);
    }
    
    // 5. Create FILE* from input data for reading
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // 6. Create IO handler from file
        io_handler = cmsOpenIOhandlerFromFile(ctx, "input_file", "rb");
        
        if (io_handler) {
            // 7. Open profile from IO handler
            profiles[2] = cmsOpenProfileFromIOhandler2THR(ctx, io_handler, FALSE);
            
            if (profiles[2]) {
                // 8. Get header flags
                cmsUInt32Number flags = cmsGetHeaderFlags(profiles[2]);
                
                // 9. Detect destination black point
                cmsCIEXYZ blackPoint;
                cmsBool blackPointResult = cmsDetectDestinationBlackPoint(&blackPoint, profiles[2], 
                                                                         INTENT_PERCEPTUAL, 0);
                
                // 10. Save profile to memory
                cmsUInt32Number bytesNeeded = 0;
                // First call to get required size
                cmsSaveProfileToMem(profiles[2], nullptr, &bytesNeeded);
                
                if (bytesNeeded > 0 && bytesNeeded < 10 * 1024 * 1024) { // Limit to 10MB
                    saved_profile_buffer = malloc(bytesNeeded);
                    if (saved_profile_buffer) {
                        cmsSaveProfileToMem(profiles[2], saved_profile_buffer, &bytesNeeded);
                    }
                }
                
                // 11. Try to get named color info
                // Note: This requires a named color list, which we don't have from input
                // We'll create a dummy call with NULL parameters
                char name[256] = {0};
                char prefix[256] = {0};
                char suffix[256] = {0};
                cmsUInt16Number pcs[3] = {0};
                cmsUInt16Number colorant[16] = {0};
                
                // This will likely fail but demonstrates the API call
                cmsBool namedColorResult = cmsNamedColorInfo(nullptr, 0, name, prefix, suffix, pcs, colorant);
            }
            
            // Close IO handler
            cmsCloseIOhandler(io_handler);
            io_handler = nullptr;
        }
        
        assert_file_closed(&in_file);;
    }
    
    // 12. Create a FILE* for output (if needed by other operations)
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // We could use out_file for something, but the APIs don't require it
        assert_file_closed(&out_file);;
    }
    
    // 13. Create and delete a transform (if we have valid profiles)
    if (profiles[0] && profiles[1]) {
        transform = cmsCreateTransformTHR(ctx, profiles[0], TYPE_Lab_8, 
                                         profiles[1], TYPE_Lab_8, 
                                         INTENT_PERCEPTUAL, cmsFLAGS_NOCACHE);
        
        if (transform) {
            // Test the transform with some data
            unsigned char input[3] = {128, 128, 128};
            unsigned char output[3] = {0};
            cmsDoTransform(transform, input, output, 1);
            
            // Delete the transform
            cmsDeleteTransform(transform);
            transform = nullptr;
        }
    }
    
    // 14. Clean up all profiles
    for (int i = 0; i < 3; i++) {
        if (profiles[i]) {
            cmsCloseProfile(profiles[i]);
            profiles[i] = nullptr;
        }
    }
    
    // 15. Free saved profile buffer
    if (saved_profile_buffer) {
        free(saved_profile_buffer);
        saved_profile_buffer = nullptr;
    }
    
    // 16. Delete context
    cmsDeleteContext(ctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}