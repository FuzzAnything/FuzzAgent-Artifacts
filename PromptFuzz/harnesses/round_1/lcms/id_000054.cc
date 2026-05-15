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
//<ID> 334
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsSetHeaderFlags","cmsGetProfileIOhandler","cmsMD5computeID","cmsIT8LoadFromMem","cmsIT8SetPropertyMulti","cmsBuildParametricToneCurve","cmsIsToneCurveMultisegment","cmsStageAllocCLut16bit","cmsOpenProfileFromStreamTHR","cmsCloseProfile","cmsSaveProfileToFile","cmsStageFree","cmsFreeToneCurve","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsSetHeaderFlags","cmsGetProfileIOhandler","cmsMD5computeID","cmsIT8LoadFromMem","cmsIT8SetPropertyMulti","cmsBuildParametricToneCurve","cmsIsToneCurveMultisegment","cmsStageAllocCLut16bit","cmsOpenProfileFromStreamTHR","cmsCloseProfile","cmsSaveProfileToFile","cmsStageFree","cmsFreeToneCurve","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified LCMS APIs with the input data:

*/


// Include necessary LCMS headers (adapt paths as needed)
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations

    cmsContext context = nullptr;
    cmsHPROFILE profile = nullptr;
    cmsHANDLE it8 = nullptr;
    cmsToneCurve* curve = nullptr;
    cmsStage* stage = nullptr;
    cmsIOHANDLER* io_handler = nullptr;
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    
    // 1. Create context
    context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Create NULL profile using input data context
    profile = cmsCreateNULLProfileTHR(context);
    
    // 3. Set header flags using part of input data
    if (profile) {
        cmsUInt32Number flags = 0;
        if (size >= 4) {
            memcpy(&flags, data, 4);
        }
        cmsSetHeaderFlags(profile, flags);
        
        // 4. Get profile IO handler
        io_handler = cmsGetProfileIOhandler(profile);
        
        // 5. Compute MD5 ID
        cmsMD5computeID(profile);
    }
    
    // 6. Create IT8 handle from input data
    it8 = cmsIT8LoadFromMem(context, data, size > 1024 ? 1024 : size);
    if (it8) {
        // 7. Set IT8 property using input data
        const char* sample_key = "SampleKey";
        const char* sub_key = "SubKey";
        const char* buffer = "BufferValue";
        cmsIT8SetPropertyMulti(it8, sample_key, sub_key, buffer);
    }
    
    // 8. Create tone curve for testing multisegment check
    if (size >= 20) {
        cmsFloat64Number params[5] = {2.4, 0.0, 0.0, 0.0, 0.0};
        curve = cmsBuildParametricToneCurve(context, 1, params);
        
        // 9. Check if tone curve is multisegment
        if (curve) {
            cmsBool isMulti = cmsIsToneCurveMultisegment(curve);
            (void)isMulti; // Use result to avoid compiler warning
        }
    }
    
    // 10. Create CLUT stage using input data for table
    if (size >= 256) {
        const int nGridPoints = 2;
        const int inputChan = 3;
        const int outputChan = 3;
        const int tableSize = nGridPoints * nGridPoints * nGridPoints * outputChan;
        
        if (size >= tableSize * sizeof(cmsUInt16Number)) {
            const cmsUInt16Number* table = reinterpret_cast<const cmsUInt16Number*>(data);
            stage = cmsStageAllocCLut16bit(context, nGridPoints, inputChan, outputChan, table);
        }
    }
    
    // 11. Use fmemopen to read input data as FILE*
    in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // 12. Open profile from file stream
        cmsHPROFILE file_profile = cmsOpenProfileFromStreamTHR(context, in_file, "rb");
        if (file_profile) {
            cmsCloseProfile(file_profile);
        }
        assert_file_closed(&in_file);;
    }
    
    // 13. Create output file
    out_file = fopen("output_file", "wb");
    if (out_file) {
        // 14. Save profile to file if we have one
        if (profile) {
            cmsSaveProfileToFile(profile, "output_file");
        }
        assert_file_closed(&out_file);;
    }
    
    // 15. Cleanup
    if (stage) cmsStageFree(stage);
    if (curve) cmsFreeToneCurve(curve);
    if (it8) cmsIT8Free(it8);
    if (profile) cmsCloseProfile(profile);
    if (context) cmsDeleteContext(context);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}