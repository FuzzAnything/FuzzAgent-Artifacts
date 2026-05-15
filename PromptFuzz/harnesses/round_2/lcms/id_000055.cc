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
//<ID> 420
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 3
//<Quality> {"density":15,"unique_branches":{"Type_LUTB2A_Write":[[3153,13,3153,142,0,0,4,0],[3154,17,3154,142,0,0,4,0],[3155,21,3156,92,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandler2THR","cmsGetDeviceClass","cmsGetHeaderFlags","cmsMLUalloc","cmsMLUtranslationsCount","cmsCreate_OkLabProfile","cmsCreateBCHSWabstractProfileTHR","cmsCreateTransformTHR","cmsGetNamedColorList","cmsNamedColorInfo","cmsDoTransform","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsMLUfree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandler2THR","cmsGetDeviceClass","cmsGetHeaderFlags","cmsMLUalloc","cmsMLUtranslationsCount","cmsCreate_OkLabProfile","cmsCreateBCHSWabstractProfileTHR","cmsCreateTransformTHR","cmsGetNamedColorList","cmsNamedColorInfo","cmsDoTransform","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsMLUfree","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 4) {
        return 0;
    }

    cmsContext ctx = nullptr;
    cmsHANDLE it8 = nullptr;
    cmsHPROFILE hInputProfile = nullptr;
    cmsHPROFILE hOkLabProfile = nullptr;
    cmsHPROFILE hBCHSWProfile = nullptr;
    cmsHTRANSFORM transform = nullptr;
    cmsNAMEDCOLORLIST* namedColorList = nullptr;
    cmsIOHANDLER* io = nullptr;
    cmsMLU* mlu = nullptr;
    FILE* in_file = nullptr;
    void* savedProfileBuffer = nullptr;
    cmsUInt32Number savedProfileSize = 0;
    char** sampleNames = nullptr;
    int ret = 0;
    int i;

    // 1. Create context
    ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
        goto cleanup;
    }

    // 2. Load IT8 table from input data
    it8 = cmsIT8LoadFromMem(ctx, data, (cmsUInt32Number)size);
    if (it8) {
        // 3. Enumerate IT8 data format
        ret = cmsIT8EnumDataFormat(it8, &sampleNames);
        // Note: sampleNames is owned by it8, don't free separately
    }

    // 4. Open input data as a profile using I/O handler
    in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Create I/O handler from FILE*
        io = cmsOpenIOhandlerFromFile(ctx, "input_file", "rb");
        if (io) {
            // Try to open as profile (read mode)
            hInputProfile = cmsOpenProfileFromIOhandler2THR(ctx, io, FALSE);
            
            if (hInputProfile) {
                // 5. Get device class of input profile
                cmsProfileClassSignature deviceClass = cmsGetDeviceClass(hInputProfile);
                
                // 6. Get header flags
                cmsUInt32Number flags = cmsGetHeaderFlags(hInputProfile);
                
                // 7. Create MLU for translations count
                mlu = cmsMLUalloc(ctx, 10);
                if (mlu) {
                    // 8. Get translations count
                    cmsUInt32Number translationCount = cmsMLUtranslationsCount(mlu);
                }
            }
        }
    }

    // 9. Create OkLab profile
    hOkLabProfile = cmsCreate_OkLabProfile(ctx);
    
    // 10. Create BCHSW abstract profile
    // Use first 4 bytes as seed for parameters
    float params[4];
    if (size >= sizeof(params)) {
        memcpy(params, data, sizeof(params));
    } else {
        // Default values if data is too small
        params[0] = 0.5f; // Bright
        params[1] = 1.0f; // Contrast
        params[2] = 0.0f; // Hue
        params[3] = 1.0f; // Saturation
    }
    
    hBCHSWProfile = cmsCreateBCHSWabstractProfileTHR(
        ctx,
        17, // nLUTPoints
        (cmsFloat64Number)params[0], // Bright
        (cmsFloat64Number)params[1], // Contrast
        (cmsFloat64Number)params[2], // Hue
        (cmsFloat64Number)params[3], // Saturation
        5000, // TempSrc
        6500  // TempDest
    );

    // 11. Create transform if we have profiles
    if (hOkLabProfile && hBCHSWProfile) {
        transform = cmsCreateTransformTHR(
            ctx,
            hOkLabProfile,
            TYPE_Lab_DBL,    // Input format
            hBCHSWProfile,
            TYPE_RGB_DBL,    // Output format
            INTENT_PERCEPTUAL,
            0
        );
        
        if (transform) {
            // 12. Get named color list from transform
            namedColorList = cmsGetNamedColorList(transform);
            
            if (namedColorList) {
                char name[256], prefix[256], suffix[256];
                cmsUInt16Number pcs[3], colorant[16];
                
                // 13. Get named color info for first color if exists
                cmsUInt32Number nColors = 0; // This would normally come from the list
                if (nColors > 0) {
                    cmsNamedColorInfo(namedColorList, 0, 
                                     name, prefix, suffix, 
                                     pcs, colorant);
                }
                
                // Test transform with some sample data
                double inputLab[3] = {50.0, 0.0, 0.0}; // Neutral gray
                double outputRGB[3];
                
                // 14. Perform transform
                cmsDoTransform(transform, inputLab, outputRGB, 1);
            }
        }
    }

    // 15. Save OkLab profile to memory
    if (hOkLabProfile) {
        // First call to get required size
        cmsSaveProfileToMem(hOkLabProfile, nullptr, &savedProfileSize);
        
        if (savedProfileSize > 0) {
            savedProfileBuffer = malloc(savedProfileSize);
            if (savedProfileBuffer) {
                cmsSaveProfileToMem(hOkLabProfile, savedProfileBuffer, &savedProfileSize);
            }
        }
    }

cleanup:
    // 16-20. Cleanup in reverse order
    if (savedProfileBuffer) {
        free(savedProfileBuffer);
    }
    
    if (mlu) {
        cmsMLUfree(mlu);
    }
    
    if (transform) {
        // 17. Delete transform
        cmsDeleteTransform(transform);
    }
    
    if (hInputProfile) {
        // 18. Close input profile
        cmsCloseProfile(hInputProfile);
    }
    
    if (hOkLabProfile) {
        cmsCloseProfile(hOkLabProfile);
    }
    
    if (hBCHSWProfile) {
        cmsCloseProfile(hBCHSWProfile);
    }
    
    if (io) {
        // 19. Close I/O handler
        cmsCloseIOhandler(io);
    }
    
    if (it8) {
        // 20. Free IT8
        cmsIT8Free(it8);
    }
    
    if (ctx) {
        // 21. Delete context
        cmsDeleteContext(ctx);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }

    assert_file_closed(&in_file);
	return 0;
}