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
//<ID> 132
//<Prompt> []
/*<Combination>: [
*/
//<score> 77, nr_unique_branch: 85
//<Quality> {"density":22,"unique_branches":{"UnrollLabDoubleToFloat":[[731,31,731,44,1,0,4,1]],"isInkColorspace":[[194,5,194,24,0,0,4,0],[195,5,195,23,0,0,4,1],[196,5,196,24,0,0,4,1],[197,5,197,24,0,0,4,1],[198,5,198,24,0,0,4,1],[199,5,199,24,0,0,4,1],[200,5,200,24,0,0,4,1],[201,5,201,24,0,0,4,1],[202,5,202,24,0,0,4,1],[203,5,203,24,0,0,4,1],[204,5,204,24,0,0,4,1],[205,5,205,24,0,0,4,1],[206,5,206,24,0,0,4,1],[207,5,207,24,0,0,4,1],[208,5,208,24,0,0,4,1],[209,5,209,24,0,0,4,1],[210,5,210,24,0,0,4,1],[211,5,211,26,0,0,4,1],[212,5,212,26,0,0,4,1],[213,5,213,26,0,0,4,1],[214,5,214,26,0,0,4,1],[215,5,215,26,0,0,4,1],[216,5,216,26,0,0,4,1],[217,5,217,26,0,0,4,1],[218,5,218,26,0,0,4,1],[219,5,219,26,0,0,4,1],[220,5,220,27,0,0,4,1],[221,5,221,27,0,0,4,1],[222,5,222,27,0,0,4,1],[223,5,223,27,0,0,4,1],[224,5,224,27,0,0,4,1],[225,5,225,27,0,0,4,1],[227,5,227,12,0,0,4,1]],"_cmsMAT3solve":[[162,9,162,35,0,0,4,1]],"RootOfLeastSquaresFitQuadraticCurve":[[340,9,340,14,0,0,4,1],[342,15,342,20,0,0,4,0],[342,15,342,20,0,0,4,1],[363,9,363,37,0,0,4,1],[370,9,370,26,0,0,4,1],[378,14,378,20,0,0,4,0],[378,14,378,20,0,0,4,1],[385,18,385,35,0,0,4,1],[31,23,31,34,2,0,4,0],[31,31,31,32,2,0,4,1]],"cmsDetectDestinationBlackPoint":[[415,9,415,36,0,0,4,0],[417,9,417,42,0,0,4,0],[433,9,433,57,0,0,4,1],[450,9,450,59,0,0,4,0],[450,9,450,59,0,0,4,1],[451,10,451,38,0,0,4,0],[452,10,452,37,0,0,4,0],[453,10,453,38,0,0,4,1],[463,9,463,47,0,0,4,1],[489,9,489,27,0,0,4,0],[489,9,489,27,0,0,4,1],[493,15,493,22,0,0,4,0],[493,15,493,22,0,0,4,1],[506,19,506,24,0,0,4,0],[506,19,506,24,0,0,4,1],[511,9,511,38,0,0,4,0],[511,9,511,38,0,0,4,1],[522,9,522,47,0,0,4,1],[546,15,546,22,0,0,4,0],[546,15,546,22,0,0,4,1],[552,9,552,47,0,0,4,1],[565,15,565,22,0,0,4,0],[565,15,565,22,0,0,4,1],[569,13,569,21,0,0,4,0],[569,13,569,21,0,0,4,1],[569,25,569,32,0,0,4,0],[569,25,569,32,0,0,4,1],[578,9,578,14,0,0,4,0],[578,9,578,14,0,0,4,1],[588,9,588,20,0,0,4,1],[30,23,30,34,18,0,4,1],[30,31,30,32,18,0,4,1],[30,44,30,45,18,0,4,1],[30,23,30,34,19,0,4,1],[30,31,30,32,19,0,4,1],[30,44,30,45,19,0,4,1],[30,23,30,34,20,0,4,0],[30,23,30,34,20,0,4,1]],"cmsIsCLUT":[[841,8,841,31,0,0,4,1],[842,8,842,32,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsIT8Free","cmsCreate_OkLabProfile","cmsCreateBCHSWabstractProfileTHR","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandler2THR","cmsGetHeaderFlags","cmsCreateTransformTHR","cmsDeleteTransform","cmsDetectDestinationBlackPoint","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsAllocNamedColorList","cmsNamedColorInfo","cmsFreeNamedColorList","cmsCloseProfile","cmsCloseIOhandler","cmsOpenProfileFromMemTHR","cmsGetHeaderFlags","cmsDetectDestinationBlackPoint","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8EnumDataFormat","cmsIT8Free","cmsCreate_OkLabProfile","cmsCreateBCHSWabstractProfileTHR","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandler2THR","cmsGetHeaderFlags","cmsCreateTransformTHR","cmsDeleteTransform","cmsDetectDestinationBlackPoint","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsAllocNamedColorList","cmsNamedColorInfo","cmsFreeNamedColorList","cmsCloseProfile","cmsCloseIOhandler","cmsOpenProfileFromMemTHR","cmsGetHeaderFlags","cmsDetectDestinationBlackPoint","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
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
	  // Need minimum data for meaningful fuzzing
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // 1. Load IT8 table from memory
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(ctx, data, (cmsUInt32Number)size);
    
    if (hIT8) {
        // 2. Enumerate data format from IT8
        char **sampleNames = NULL;
        int formatCount = cmsIT8EnumDataFormat(hIT8, &sampleNames);
        
        // 3. Free IT8 table
        cmsIT8Free(hIT8);
    }
    
    // 4. Create OkLab profile
    cmsHPROFILE okLabProfile = cmsCreate_OkLabProfile(ctx);
    
    // 5. Create BCHSW abstract profile using data for parameters
    // Use first few bytes for parameters (safe access checked)
    cmsUInt32Number nLUTPoints = 33;  // default
    cmsFloat64Number bright = 0.0, contrast = 0.0, hue = 0.0, saturation = 0.0;
    cmsUInt32Number tempSrc = 5000, tempDest = 6500;
    
    if (size >= 40) {
        // Use data bytes to influence parameters (within reasonable ranges)
        nLUTPoints = 2 + (data[0] % 100);  // 2-101 points
        bright = ((cmsFloat64Number)((int8_t)data[1])) / 100.0;  // -1.27 to 1.27
        contrast = ((cmsFloat64Number)((int8_t)data[2])) / 100.0;
        hue = ((cmsFloat64Number)((int8_t)data[3])) / 100.0;
        saturation = ((cmsFloat64Number)((int8_t)data[4])) / 100.0;
        tempSrc = 2000 + (data[5] * 50);  // 2000-14750K
        tempDest = 2000 + (data[6] * 50);
    }
    
    cmsHPROFILE bchswProfile = cmsCreateBCHSWabstractProfileTHR(ctx, nLUTPoints, 
                                                               bright, contrast, 
                                                               hue, saturation,
                                                               tempSrc, tempDest);
    
    // 6-7. Create and close IO handler from file (reading input data)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Create IO handler from file
        cmsIOHANDLER *io = cmsOpenIOhandlerFromFile(ctx, "input_file", "rb");
        if (io) {
            // 11. Open profile from IO handler
            cmsHPROFILE ioProfile = cmsOpenProfileFromIOhandler2THR(ctx, io, FALSE);
            
            if (ioProfile) {
                // 12. Get header flags
                cmsUInt32Number flags = cmsGetHeaderFlags(ioProfile);
                
                // 13. Create and delete transform
                if (okLabProfile && bchswProfile) {
                    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, 
                                                                   okLabProfile, 
                                                                   TYPE_Lab_DBL,
                                                                   bchswProfile,
                                                                   TYPE_Lab_DBL,
                                                                   INTENT_PERCEPTUAL,
                                                                   0);
                    if (transform) {
                        // 13. Delete transform
                        cmsDeleteTransform(transform);
                    }
                }
                
                // 8. Detect destination black point
                cmsCIEXYZ blackPoint;
                if (ioProfile) {
                    cmsDetectDestinationBlackPoint(&blackPoint, ioProfile, 
                                                   INTENT_PERCEPTUAL, 0);
                }
                
                // 9. Save profile to memory
                if (bchswProfile) {
                    // First get size needed
                    cmsUInt32Number bytesNeeded = 0;
                    cmsSaveProfileToMem(bchswProfile, NULL, &bytesNeeded);
                    
                    // Allocate buffer and save if we have enough data
                    if (bytesNeeded > 0 && bytesNeeded < 10*1024*1024) {  // Limit to 10MB
                        void *buffer = malloc(bytesNeeded);
                        if (buffer) {
                            cmsSaveProfileToMem(bchswProfile, buffer, &bytesNeeded);
                            free(buffer);
                        }
                    }
                }
                
                // 10. Try to get named color info (need a named color list)
                // Create a simple named color list for testing
                cmsNAMEDCOLORLIST *namedList = cmsAllocNamedColorList(ctx, 2, 3, 
                                                                      "Prefix", "Suffix");
                if (namedList) {
                    char name[256], prefix[256], suffix[256];
                    cmsUInt16Number PCS[3], colorant[3];
                    
                    // Try to get info for first color
                    cmsNamedColorInfo(namedList, 0, name, prefix, suffix, PCS, colorant);
                    
                    // Free the named color list (using cmsFreeNamedColorList if available,
                    // but we'll use the internal lcms cleanup through context)
                    cmsFreeNamedColorList(namedList);
                }
                
                // 14. Close profile
                cmsCloseProfile(ioProfile);
            }
            
            // 6. Close IO handler
            cmsCloseIOhandler(io);
        }
        
        assert_file_closed(&in_file);;
    }
    
    // Also try to open a profile from the data directly
    if (size > 128) {  // Need minimum size for ICC profile
        cmsHPROFILE memProfile = cmsOpenProfileFromMemTHR(ctx, data, 
                                                         (cmsUInt32Number)size);
        if (memProfile) {
            // Do minimal operations
            cmsUInt32Number flags = cmsGetHeaderFlags(memProfile);
            cmsCIEXYZ blackPoint;
            cmsDetectDestinationBlackPoint(&blackPoint, memProfile, 
                                           INTENT_PERCEPTUAL, 0);
            cmsCloseProfile(memProfile);
        }
    }
    
    // 14. Close profiles
    if (okLabProfile) cmsCloseProfile(okLabProfile);
    if (bchswProfile) cmsCloseProfile(bchswProfile);
    
    // 15. Delete context
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}