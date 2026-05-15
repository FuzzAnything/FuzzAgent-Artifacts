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
//<ID> 457
//<Prompt> []
/*<Combination>: [
*/
//<score> 135, nr_unique_branch: 60
//<Quality> {"density":15,"unique_branches":{"CheckOne":[[1164,37,1164,48,0,0,4,0],[1164,37,1164,48,0,0,4,1],[1166,13,1166,30,0,0,4,0],[1166,13,1166,30,0,0,4,1],[1167,13,1167,51,0,0,4,0],[1167,13,1167,51,0,0,4,1]],"cmsGetTransformContextID":[[1423,9,1423,22,0,0,4,1]],"cmsPipelineDup":[[1504,9,1504,35,0,0,4,0]],"CurvesDup":[[1288,9,1288,21,0,0,4,1],[1292,15,1292,34,0,0,4,0],[1292,15,1292,34,0,0,4,1]],"FixColorSpaces":[[1038,9,1038,44,0,0,4,1]],"cmsTransform2DeviceLink":[[1211,9,1211,29,0,0,4,1],[1217,9,1217,20,0,0,4,0],[1219,13,1219,58,0,0,4,1],[1226,9,1226,20,0,0,4,1],[1229,9,1229,51,0,0,4,1],[1236,9,1236,50,0,0,4,1],[1245,9,1245,18,0,0,4,1],[1264,10,1264,42,0,0,4,1],[1270,9,1270,38,0,0,4,1],[1275,9,1275,27,0,0,4,0],[1275,9,1275,27,0,0,4,1],[1284,9,1284,27,0,0,4,0],[1284,9,1284,27,0,0,4,1],[1294,13,1294,31,0,0,4,0],[1294,35,1294,78,0,0,4,0],[1295,18,1295,109,0,0,4,1],[1299,13,1299,30,0,0,4,0],[1299,34,1299,76,0,0,4,0],[1300,18,1300,110,0,0,4,1],[1307,9,1307,27,0,0,4,1],[1312,9,1312,44,0,0,4,1],[1316,9,1316,46,0,0,4,1],[1319,9,1319,52,0,0,4,1],[1323,9,1323,39,0,0,4,0],[1323,9,1323,39,0,0,4,1],[1324,9,1324,39,0,0,4,0],[1326,16,1326,84,0,0,4,1],[1329,9,1329,40,0,0,4,0],[1329,9,1329,40,0,0,4,1],[1331,13,1331,43,0,0,4,0],[1333,17,1333,89,0,0,4,1],[1341,9,1341,41,0,0,4,0],[1341,45,1341,71,0,0,4,1],[1346,9,1346,40,0,0,4,1],[1350,14,1350,87,0,0,4,1]],"cmsNamedColorInfo":[[869,9,869,15,0,0,4,0],[870,9,870,15,0,0,4,0],[874,9,874,17,0,0,4,0]],"FindCombination":[[1179,15,1179,38,0,0,4,0],[1179,15,1179,38,0,0,4,1],[1183,13,1183,31,0,0,4,0],[1183,13,1183,31,0,0,4,1],[1184,13,1184,37,0,0,4,0],[1184,13,1184,37,0,0,4,1],[1184,41,1184,78,0,0,4,0],[1184,41,1184,78,0,0,4,1],[1186,13,1186,31,0,0,4,0],[1186,13,1186,31,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsDeleteContext","cmsCreateProofingTransform","cmsTransform2DeviceLink","cmsGetProfileInfoUTF8","cmsMLUalloc","cmsMLUsetWide","cmsMLUgetUTF8","cmsMLUfree","cmsCloseProfile","cmsDeleteTransform","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorInfo","cmsFreeNamedColorList","cmsDictAlloc","cmsDictGetEntryList","cmsDictNextEntry","cmsDictFree","cmsPlugin","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreateProofingTransform","cmsTransform2DeviceLink","cmsGetProfileInfoUTF8","cmsMLUalloc","cmsMLUsetWide","cmsMLUgetUTF8","cmsMLUfree","cmsCloseProfile","cmsDeleteTransform","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorInfo","cmsFreeNamedColorList","cmsDictAlloc","cmsDictGetEntryList","cmsDictNextEntry","cmsDictFree","cmsPlugin","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Consume input data by creating a profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Create a simple transform using the profile
    cmsHTRANSFORM hTransform = cmsCreateProofingTransform(
        hProfile, TYPE_RGB_8,
        hProfile, TYPE_RGB_8,
        hProfile, INTENT_PERCEPTUAL, INTENT_PERCEPTUAL, 0);
    
    // Use cmsTransform2DeviceLink (if transform was created successfully)
    if (hTransform) {
        cmsHPROFILE hDeviceLink = cmsTransform2DeviceLink(hTransform, 4.0, 0);
        if (hDeviceLink) {
            // Extract MLU information from the device link profile
            char buffer[256];
            cmsUInt32Number mluSize = cmsGetProfileInfoUTF8(hDeviceLink, cmsInfoDescription, 
                                                          "en", "US", buffer, sizeof(buffer));
            
            // Create a minimal MLU for testing cmsMLUgetUTF8
            cmsMLU* mlu = cmsMLUalloc(ctx, 1);
            if (mlu) {
                cmsMLUsetWide(mlu, "en", "US", L"Test");
                char mluBuffer[256];
                cmsMLUgetUTF8(mlu, "en", "US", mluBuffer, sizeof(mluBuffer));
                cmsMLUfree(mlu);
            }
            cmsCloseProfile(hDeviceLink);
        }
        cmsDeleteTransform(hTransform);
    }
    
    // Create and use named color list
    cmsNAMEDCOLORLIST* namedColorList = cmsAllocNamedColorList(ctx, 10, 3, "Prefix", "Suffix");
    if (namedColorList) {
        cmsUInt16Number pcs[3] = {0x8000, 0x8000, 0x8000};
        cmsUInt16Number colorant[16] = {0};
        
        // Append a named color
        cmsBool appendResult = cmsAppendNamedColor(namedColorList, "TestColor", pcs, colorant);
        
        // Get information about the named color
        char name[256], prefix[256], suffix[256];
        cmsUInt16Number outPCS[3], outColorant[16];
        cmsBool infoResult = cmsNamedColorInfo(namedColorList, 0, name, prefix, suffix, outPCS, outColorant);
        
        cmsFreeNamedColorList(namedColorList);
    }
    
    // Create a dictionary and iterate through entries
    cmsHANDLE hDict = cmsDictAlloc(ctx);
    if (hDict) {
        const cmsDICTentry* entry = cmsDictGetEntryList(hDict);
        while (entry) {
            const cmsDICTentry* next = cmsDictNextEntry(entry);
            entry = next;
        }
        cmsDictFree(hDict);
    }
    
    // Call cmsPlugin with NULL (safe no-op)
    cmsBool pluginResult = cmsPlugin(NULL);
    
    // Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}