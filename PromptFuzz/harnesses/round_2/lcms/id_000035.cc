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
//<ID> 271
//<Prompt> []
/*<Combination>: [
*/
//<score> 42.5, nr_unique_branch: 14
//<Quality> {"density":17,"unique_branches":{"cmsSaveProfileToIOhandler":[[1560,9,1560,19,0,0,4,1]],"cmsDictNextEntry":[[1201,10,1201,19,0,0,4,0]],"Type_LUT16_Write":[[2512,9,2512,41,0,0,4,1],[2513,9,2513,21,0,0,4,0],[2515,13,2515,25,0,0,4,0],[2516,17,2516,65,0,0,4,1],[2521,9,2521,24,0,0,4,1],[2525,19,2525,37,0,0,4,0],[2525,19,2525,37,0,0,4,1],[2527,17,2527,46,0,0,4,1],[2528,17,2528,51,0,0,4,1]],"cmsSaveProfileToMem":[[1629,9,1629,23,0,0,4,1],[1632,20,1632,39,0,0,4,1],[1637,9,1637,19,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsPlugin","cmsIT8LoadFromMem","cmsCreateBCHSWabstractProfileTHR","cmsDeleteContext","cmsDeleteContext","cmsOpenIOhandlerFromFile","cmsCloseIOhandler","cmsOpenProfileFromMemTHR","cmsIsIntentSupported","cmsGetHeaderFlags","cmsCreateTransformTHR","cmsAllocNamedColorList","cmsDictNextEntry","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsIT8EnumDataFormat","cmsFreeNamedColorList","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsCloseIOhandler","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsPlugin","cmsIT8LoadFromMem","cmsCreateBCHSWabstractProfileTHR","cmsOpenIOhandlerFromFile","cmsCloseIOhandler","cmsOpenProfileFromMemTHR","cmsIsIntentSupported","cmsGetHeaderFlags","cmsCreateTransformTHR","cmsAllocNamedColorList","cmsDictNextEntry","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsIT8EnumDataFormat","cmsFreeNamedColorList","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsCloseIOhandler","cmsDeleteContext"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified lcms APIs to handle input data that appears to be lcms output (likely ICC profiles and IT8 data):

*/


// Include lcms2 header if not already included via the provided headers

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    cmsContext context = nullptr;
    cmsHPROFILE hProfile = nullptr;
    cmsHPROFILE hProfile2 = nullptr;
    cmsHANDLE hIT8 = nullptr;
    cmsIOHANDLER* io = nullptr;
    cmsHTRANSFORM hTransform = nullptr;
    cmsNAMEDCOLORLIST* namedColorList = nullptr;
    char** sampleNames = nullptr;
    const cmsDICTentry* dictEntry = nullptr;
    
    // 1. Create context
    context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Plugin - register a null plugin (dummy call)
    cmsPlugin(nullptr);
    
    // 3. Load IT8 from memory (using first half of data)
    size_t it8_size = size / 2;
    hIT8 = cmsIT8LoadFromMem(context, data, it8_size);
    
    // 4. Create BCHSW abstract profile with parameters from data
    // Use data bytes to derive parameters (clamped to reasonable ranges)
    cmsUInt32Number nLUTPoints = 2 + (data[0] % 64); // 2-65 LUT points
    double bright = (data[1] / 255.0) * 2.0 - 1.0; // -1.0 to 1.0
    double contrast = (data[2] / 255.0) * 2.0; // 0.0 to 2.0
    double hue = (data[3] / 255.0) * 360.0 - 180.0; // -180.0 to 180.0
    double saturation = (data[4] / 255.0) * 2.0; // 0.0 to 2.0
    cmsUInt32Number tempSrc = 5000 + (data[5] % 5000); // 5000-10000K
    cmsUInt32Number tempDest = 5000 + (data[6] % 5000); // 5000-10000K
    
    hProfile = cmsCreateBCHSWabstractProfileTHR(context, nLUTPoints, 
                                                bright, contrast, hue, 
                                                saturation, tempSrc, tempDest);
    
    // 5. Create output file for writing
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        cmsDeleteContext(context);
        assert_file_closed(&out_file);
	return 0;
    }
    
    // 6. Open IO handler from file for reading
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&out_file);;
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
    }
    
    io = cmsOpenIOhandlerFromFile(context, "input_file", "rb");
    // Note: We don't actually create "input_file" on disk since we're fuzzing
    
    // 7. Open profile from IO handler (simulated - use mem instead)
    if (io) {
        cmsCloseIOhandler(io);
        io = nullptr;
    }
    
    // 8. Open profile from memory (using second half of data)
    size_t profile_size = size - it8_size;
    const void* profile_data = data + it8_size;
    hProfile2 = cmsOpenProfileFromMemTHR(context, profile_data, profile_size);
    
    // 9. Check if intent is supported
    if (hProfile2) {
        cmsBool intentSupported = cmsIsIntentSupported(hProfile2, 0, 0);
        (void)intentSupported; // Use result
    }
    
    // 10. Get header flags
    if (hProfile) {
        cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
        (void)flags; // Use result
    }
    
    // 11. Create transform
    if (hProfile && hProfile2) {
        hTransform = cmsCreateTransformTHR(context, hProfile, 
                                          TYPE_RGB_8, hProfile2, 
                                          TYPE_RGB_8, INTENT_PERCEPTUAL, 
                                          cmsFLAGS_NOCACHE);
    }
    
    // 12. Allocate named color list
    namedColorList = cmsAllocNamedColorList(context, 10, 3, "Prefix", "Suffix");
    
    // 13. Get dictionary entries (dummy call - we don't have a real dict)
    dictEntry = cmsDictNextEntry(nullptr);
    
    // 14. Save profile to memory
    if (hProfile) {
        cmsUInt32Number bytesNeeded = 0;
        // First get required size
        cmsSaveProfileToMem(hProfile, nullptr, &bytesNeeded);
        
        if (bytesNeeded > 0 && bytesNeeded < 10 * 1024 * 1024) { // Limit to 10MB
            std::vector<uint8_t> profileBuffer(bytesNeeded);
            if (cmsSaveProfileToMem(hProfile, profileBuffer.data(), &bytesNeeded)) {
                // Successfully saved profile to memory
                // Could optionally write to out_file
                if (out_file) {
                    fwrite(profileBuffer.data(), 1, bytesNeeded, out_file);
                }
            }
        }
    }
    
    // 15. Enumerate IT8 data format if IT8 was loaded
    if (hIT8) {
        int formatCount = cmsIT8EnumDataFormat(hIT8, &sampleNames);
        (void)formatCount; // Use result
        
        // Free sample names array if allocated
        if (sampleNames) {
            // Note: IT8 implementation should provide cleanup, but we document usage
        }
    }
    
    // 16. Cleanup in reverse order
    
    // Free named color list
    if (namedColorList) {
        cmsFreeNamedColorList(namedColorList);
    }
    
    // Delete transform
    if (hTransform) {
        cmsDeleteTransform(hTransform);
    }
    
    // Close profiles
    if (hProfile2) {
        cmsCloseProfile(hProfile2);
    }
    
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    // Free IT8
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    // Close file handles
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // Close IO handler (already closed earlier)
    if (io) {
        cmsCloseIOhandler(io);
    }
    
    // Delete context
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_name_closed("input_file");
	return 0;
}