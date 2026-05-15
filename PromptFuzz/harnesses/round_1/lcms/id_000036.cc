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
//<ID> 235
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsDeleteContext","cmsCreateMultiprofileTransformTHR","cmsTransform2DeviceLink","cmsBuildSegmentedToneCurve","cmsSliceSpaceFloat","cmsDictAlloc","cmsDictFree","cmsFloat2XYZEncoded","cmsGBDAlloc","cmsGDBCheckPoint","cmsGBDFree","cmsOpenIOhandlerFromFile","cmsGetPostScriptColorResource","cmsCloseIOhandler","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteTransform","cmsOpenProfileFromStreamTHR","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreateMultiprofileTransformTHR","cmsTransform2DeviceLink","cmsBuildSegmentedToneCurve","cmsSliceSpaceFloat","cmsDictAlloc","cmsDictFree","cmsFloat2XYZEncoded","cmsGBDAlloc","cmsGDBCheckPoint","cmsGBDFree","cmsOpenIOhandlerFromFile","cmsGetPostScriptColorResource","cmsCloseIOhandler","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteTransform","cmsOpenProfileFromStreamTHR","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" {
}

// Define missing constants if needed
#define TYPE_RGB_8 (COLORSPACE_SH(PT_RGB) | CHANNELS_SH(3) | BYTES_SH(1) | EXTRA_SH(1))
#define INTENT_PERCEPTUAL 0
#define cmsSigRgbData 0x52474220

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	 // Minimal size for ICC profile
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // 1. Create a profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 2. Create a transform and convert to device link
    cmsHTRANSFORM hTransform = cmsCreateMultiprofileTransformTHR(
        ctx, &hProfile, 1, TYPE_RGB_8, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    
    if (hTransform) {
        // Convert transform to device link
        cmsHPROFILE hDeviceLink = cmsTransform2DeviceLink(hTransform, 4.0, 0);
        
        // 3. Build segmented tone curve
        cmsCurveSegment segment = {0};
        segment.x0 = 0.0f;
        segment.x1 = 1.0f;
        segment.Type = 6; // Gamma curve type
        segment.Params[0] = 2.2; // Gamma value
        segment.Params[1] = 0.0;
        
        cmsToneCurve* toneCurve = cmsBuildSegmentedToneCurve(ctx, 1, &segment);
        
        // 4. Use cmsSliceSpaceFloat
        if (toneCurve) {
            const cmsUInt32Number clutPoints[] = {3, 3, 3};
            
            // Simple sampler that does nothing
            auto sampler = [](const cmsFloat32Number* in, cmsFloat32Number* out, void* cargo) -> cmsInt32Number {
                return 1;
            };
            
            cmsSliceSpaceFloat(3, clutPoints, sampler, NULL);
        }
        
        // 5. Create a dict and free it
        cmsHANDLE hDict = cmsDictAlloc(ctx);
        if (hDict) {
            cmsDictFree(hDict);
        }
        
        // 6. Convert XYZ to encoded
        cmsCIEXYZ xyz = {0.5, 0.5, 0.5};
        cmsUInt16Number encodedXYZ[3];
        cmsFloat2XYZEncoded(encodedXYZ, &xyz);
        
        // 7. GDB check point
        cmsHANDLE hGBD = cmsGBDAlloc(ctx);
        if (hGBD) {
            cmsCIELab lab = {50.0, 0.0, 0.0};
            cmsGDBCheckPoint(hGBD, &lab);
            cmsGBDFree(hGBD);
        }
        
        // 8. Get PostScript color resource using FILE* as requested
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            cmsIOHANDLER* io = cmsOpenIOhandlerFromFile(ctx, "output_file", "w");
            if (io) {
                cmsUInt32Number result = cmsGetPostScriptColorResource(
                    ctx, cmsPS_RESOURCE_CSA, hProfile, INTENT_PERCEPTUAL, 0, io);
                cmsCloseIOhandler(io);
            }
            assert_file_closed(&out_file);;
        }
        
        // Cleanup
        if (toneCurve) cmsFreeToneCurve(toneCurve);
        if (hDeviceLink) cmsCloseProfile(hDeviceLink);
        cmsDeleteTransform(hTransform);
    }
    
    // Also try to open profile from FILE*
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        cmsHPROFILE hProfile2 = cmsOpenProfileFromStreamTHR(ctx, in_file, "r");
        if (hProfile2) {
            cmsCloseProfile(hProfile2);
        }
        assert_file_closed(&in_file);;
    }
    
    // Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}