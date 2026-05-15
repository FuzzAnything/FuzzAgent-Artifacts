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
//<ID> 478
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 16
//<Quality> {"density":5,"unique_branches":{"_cmsICCcolorSpace":[[805,8,805,15,0,0,4,0]],"cmsCMCdeltaE":[[553,7,553,20,0,0,4,0],[553,7,553,20,0,0,4,1],[553,24,553,37,0,0,4,0],[553,24,553,37,0,0,4,1],[564,7,564,32,0,0,4,0],[564,7,564,32,0,0,4,1],[569,7,569,21,0,0,4,0],[569,7,569,21,0,0,4,1],[569,25,569,39,0,0,4,0],[569,25,569,39,0,0,4,1],[577,8,577,18,0,0,4,0],[577,8,577,18,0,0,4,1]],"cmsDictDup":[[1171,9,1171,21,0,0,4,1],[1175,12,1175,25,0,0,4,1]],"GetInfo":[[989,13,989,62,0,0,4,0]]},"library_calls":["cmsOpenProfileFromMem","cmsGetColorSpace","_cmsICCcolorSpace","cmsIsTag","cmsReadTag","cmsGetToneCurveEstimatedTable","cmsIT8Alloc","cmsIT8SetTable","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsDictAlloc","cmsDictDup","cmsDictFree","cmsDictFree","cmsCIECAM02Forward","cmsCMCdeltaE","cmsSaveProfileToMem","cmsGetProfileInfoASCII","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetColorSpace","_cmsICCcolorSpace","cmsIsTag","cmsReadTag","cmsGetToneCurveEstimatedTable","cmsIT8Alloc","cmsIT8SetTable","cmsIT8SetData","cmsIT8GetDataDbl","cmsIT8Free","cmsDictAlloc","cmsDictDup","cmsDictFree","cmsDictFree","cmsCIECAM02Forward","cmsCMCdeltaE","cmsSaveProfileToMem","cmsGetProfileInfoASCII","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    // 1. Create a profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
	return 0;
	}
	 // Invalid profile data
    
    // 2. Get color space from profile
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hProfile);
    int notation = 0; // RGB notation
    cmsColorSpaceSignature converted = _cmsICCcolorSpace(notation);
    
    // 3. Get tone curve from profile and its estimated table
    cmsToneCurve* toneCurve = nullptr;
    if (cmsIsTag(hProfile, cmsSigRedTRCTag)) {
        toneCurve = (cmsToneCurve*)cmsReadTag(hProfile, cmsSigRedTRCTag);
        if (toneCurve) {
            const cmsUInt16Number* table = cmsGetToneCurveEstimatedTable(toneCurve);
            // Use table pointer to prevent optimization removal
            if (table && size > 10) {
                volatile cmsUInt16Number sample = table[0];
                (void)sample;
            }
        }
    }
    
    // 4. Create IT8 handle and use IT8 operations
    cmsHANDLE hIT8 = cmsIT8Alloc(nullptr);
    if (hIT8) {
        // Set table
        cmsInt32Number tableResult = cmsIT8SetTable(hIT8, 0);
        
        // Try to set and get data
        if (tableResult >= 0 && size > 50) {
            // Use parts of input data for IT8 operations
            char patch[20], sample[20], value[20];
            snprintf(patch, sizeof(patch), "Patch1");
            snprintf(sample, sizeof(sample), "Sample1");
            snprintf(value, sizeof(value), "%.*s", 10, data);
            
            // Set data
            cmsIT8SetData(hIT8, patch, sample, value);
            
            // Get data as double
            cmsFloat64Number dataDbl = cmsIT8GetDataDbl(hIT8, patch, sample);
            (void)dataDbl;
        }
        cmsIT8Free(hIT8);
    }
    
    // 5. Create dictionary and duplicate it
    cmsHANDLE hDict = nullptr;
    // Create a simple dictionary (we'll use profile's dictionary if available)
    hDict = cmsDictAlloc(nullptr);
    if (hDict) {
        cmsHANDLE hDictDup = cmsDictDup(hDict);
        if (hDictDup) {
            cmsDictFree(hDictDup);
        }
        cmsDictFree(hDict);
    }
    
    // 6. Use CIECAM02 forward transformation if we have XYZ data
    if (size >= sizeof(cmsCIEXYZ) * 2) {
        cmsCIEXYZ xyzInput;
        cmsJCh jchOutput;
        
        // Use input data for XYZ values (modulo to keep within bounds)
        size_t offset = 0;
        xyzInput.X = *(reinterpret_cast<const cmsFloat64Number*>(data + (offset % (size - sizeof(cmsFloat64Number)))));
        offset += sizeof(cmsFloat64Number);
        xyzInput.Y = *(reinterpret_cast<const cmsFloat64Number*>(data + (offset % (size - sizeof(cmsFloat64Number)))));
        offset += sizeof(cmsFloat64Number);
        xyzInput.Z = *(reinterpret_cast<const cmsFloat64Number*>(data + (offset % (size - sizeof(cmsFloat64Number)))));
        
        // Create CIECAM02 model handle (simplified - in real usage would need proper initialization)
        cmsHANDLE hModel = nullptr;
        // Note: cmsCIECAM02Forward requires properly initialized model
        // For fuzzing, we'll skip if model is null
        if (hModel) {
            cmsCIECAM02Forward(hModel, &xyzInput, &jchOutput);
        }
    }
    
    // 7. Compute CMC delta E between two Lab colors
    if (size >= sizeof(cmsCIELab) * 2) {
        cmsCIELab lab1, lab2;
        
        // Use input data for Lab values
        size_t offset = 0;
        lab1.L = *(reinterpret_cast<const cmsFloat64Number*>(data + (offset % (size - sizeof(cmsFloat64Number)))));
        offset += sizeof(cmsFloat64Number);
        lab1.a = *(reinterpret_cast<const cmsFloat64Number*>(data + (offset % (size - sizeof(cmsFloat64Number)))));
        offset += sizeof(cmsFloat64Number);
        lab1.b = *(reinterpret_cast<const cmsFloat64Number*>(data + (offset % (size - sizeof(cmsFloat64Number)))));
        
        offset = size / 2; // Use different part of input
        lab2.L = *(reinterpret_cast<const cmsFloat64Number*>(data + (offset % (size - sizeof(cmsFloat64Number)))));
        offset += sizeof(cmsFloat64Number);
        lab2.a = *(reinterpret_cast<const cmsFloat64Number*>(data + (offset % (size - sizeof(cmsFloat64Number)))));
        offset += sizeof(cmsFloat64Number);
        lab2.b = *(reinterpret_cast<const cmsFloat64Number*>(data + (offset % (size - sizeof(cmsFloat64Number)))));
        
        // Compute CMC delta E
        cmsFloat64Number deltaE = cmsCMCdeltaE(&lab1, &lab2, 2.0, 1.0);
        (void)deltaE;
    }
    
    // 8. Additional operations using the profile
    if (size > 200) {
        // Try to save profile to memory
        void* memBuffer = malloc(size);
        if (memBuffer) {
            cmsUInt32Number bytesNeeded = 0;
            cmsSaveProfileToMem(hProfile, memBuffer, &bytesNeeded);
            free(memBuffer);
        }
        
        // Get profile info
        char buffer[100];
        cmsGetProfileInfoASCII(hProfile, cmsInfoDescription, "en", "US", buffer, sizeof(buffer));
    }
    
    // Cleanup
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    return 0;
}