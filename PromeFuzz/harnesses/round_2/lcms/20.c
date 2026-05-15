// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void fuzz_cmsNamedColorInfo(const cmsNAMEDCOLORLIST* list, cmsUInt32Number index) {
    if (list == NULL) return;
    
    char Name[256];
    char Prefix[256];
    char Suffix[256];
    cmsUInt16Number PCS[3];
    cmsUInt16Number Colorant[cmsMAXCHANNELS];
    
    cmsNamedColorInfo(list, index, Name, Prefix, Suffix, PCS, Colorant);
}

static void fuzz_cmsAppendNamedColor(cmsNAMEDCOLORLIST* list, const uint8_t* data, size_t size) {
    if (list == NULL || size < 1) return;
    
    char name[256];
    cmsUInt16Number PCS[3];
    cmsUInt16Number Colorant[cmsMAXCHANNELS];
    
    size_t name_len = size % 255;
    if (name_len == 0) name_len = 1;
    if (name_len > size) name_len = size;
    
    memcpy(name, data, name_len);
    name[name_len] = '\0';
    
    size_t data_offset = name_len;
    
    for (int i = 0; i < 3; i++) {
        if (data_offset + 1 < size) {
            PCS[i] = (data[data_offset] << 8) | data[data_offset + 1];
            data_offset += 2;
        } else {
            PCS[i] = 0;
        }
    }
    
    for (int i = 0; i < cmsMAXCHANNELS; i++) {
        if (data_offset + 1 < size) {
            Colorant[i] = (data[data_offset] << 8) | data[data_offset + 1];
            data_offset += 2;
        } else {
            Colorant[i] = 0;
        }
    }
    
    cmsAppendNamedColor(list, name, PCS, Colorant);
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;
    
    cmsNAMEDCOLORLIST* list = cmsAllocNamedColorList(ctx, 10, 3, "PREFIX", "SUFFIX");
    if (list == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    size_t offset = 0;
    while (offset < Size) {
        fuzz_cmsAppendNamedColor(list, Data + offset, Size - offset);
        offset += 10;
    }
    
    if (cmsNamedColorCount(list) > 0) {
        cmsInt32Number index = cmsNamedColorIndex(list, "TestColor");
        if (index >= 0) {
            fuzz_cmsNamedColorInfo(list, (cmsUInt32Number)index);
        }
        
        for (cmsUInt32Number i = 0; i < cmsNamedColorCount(list) && i < 5; i++) {
            fuzz_cmsNamedColorInfo(list, i);
        }
    }
    
    cmsNAMEDCOLORLIST* dup = cmsDupNamedColorList(list);
    if (dup != NULL) {
        cmsFreeNamedColorList(dup);
    }
    
    cmsFreeNamedColorList(list);
    cmsDeleteContext(ctx);
    
    return 0;
}