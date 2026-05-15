// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsstrcasecmp at cmserr.c:39:15 in lcms2.h
// cmsstrcasecmp at cmserr.c:39:15 in lcms2.h
// cmsstrcasecmp at cmserr.c:39:15 in lcms2.h
// cmsstrcasecmp at cmserr.c:39:15 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTR 256

static void setup_it8_handle(cmsHANDLE *hIT8) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx) {
        *hIT8 = cmsIT8Alloc(ctx);
        // Set some properties to make the handle more interesting
        if (*hIT8) {
            cmsIT8SetPropertyUncooked(*hIT8, "KEY1", "VALUE1");
            cmsIT8SetPropertyUncooked(*hIT8, "COLOR_REP", "RGB");
            cmsIT8SetPropertyUncooked(*hIT8, "SAMPLE_ID", "TestSample");
        }
        // Note: We don't delete the context as the IT8 handle depends on it
    }
}

static void setup_named_color_list(cmsNAMEDCOLORLIST **v) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx) {
        *v = cmsAllocNamedColorList(ctx, 10, 3, "Prefix", "Suffix");
        if (*v) {
            // Add some named colors with correct parameter types
            cmsUInt16Number pcs[3] = {0, 0, 0};
            cmsUInt16Number colorant[16] = {0}; // cmsMAXCHANNELS is typically 16
            
            // Red
            colorant[0] = 255; colorant[1] = 0; colorant[2] = 0;
            cmsAppendNamedColor(*v, "Red", pcs, colorant);
            
            // Green
            colorant[0] = 0; colorant[1] = 255; colorant[2] = 0;
            cmsAppendNamedColor(*v, "Green", pcs, colorant);
            
            // Blue
            colorant[0] = 0; colorant[1] = 0; colorant[2] = 255;
            cmsAppendNamedColor(*v, "Blue", pcs, colorant);
            
            // Cyan
            colorant[0] = 0; colorant[1] = 255; colorant[2] = 255;
            cmsAppendNamedColor(*v, "Cyan", pcs, colorant);
            
            // Magenta
            colorant[0] = 255; colorant[1] = 0; colorant[2] = 255;
            cmsAppendNamedColor(*v, "Magenta", pcs, colorant);
            
            // Yellow
            colorant[0] = 255; colorant[1] = 255; colorant[2] = 0;
            cmsAppendNamedColor(*v, "Yellow", pcs, colorant);
            
            // Black
            colorant[0] = 0; colorant[1] = 0; colorant[2] = 0;
            cmsAppendNamedColor(*v, "Black", pcs, colorant);
            
            // White
            colorant[0] = 255; colorant[1] = 255; colorant[2] = 255;
            cmsAppendNamedColor(*v, "White", pcs, colorant);
        }
    }
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to decide which function to test
    uint8_t selector = Data[0] % 6;
    Data++;
    Size--;
    
    // Prepare IT8 handle
    cmsHANDLE hIT8 = NULL;
    setup_it8_handle(&hIT8);
    
    // Prepare named color list
    cmsNAMEDCOLORLIST *namedColorList = NULL;
    setup_named_color_list(&namedColorList);
    
    switch (selector) {
        case 0: { // cmsIT8GetPatchName
            if (hIT8 && Size >= sizeof(int)) {
                int nPatch;
                memcpy(&nPatch, Data, sizeof(int));
                nPatch = nPatch % 100; // Limit to reasonable range
                
                char buffer[MAXSTR];
                const char* result = cmsIT8GetPatchName(hIT8, nPatch, buffer);
                // Also test with NULL buffer
                if (Size > sizeof(int)) {
                    const char* result2 = cmsIT8GetPatchName(hIT8, nPatch, NULL);
                }
            }
            break;
        }
        
        case 1: { // cmsIT8GetPropertyMulti
            if (hIT8 && Size >= 2) {
                // Extract Key and SubKey from fuzzer data
                size_t key_len = Data[0] % (Size - 1);
                if (key_len > MAXSTR - 1) key_len = MAXSTR - 1;
                
                char Key[MAXSTR];
                if (key_len > 0) {
                    memcpy(Key, Data + 1, key_len);
                    Key[key_len] = '\0';
                } else {
                    Key[0] = '\0';
                }
                
                size_t subkey_len = Data[1] % (Size - 2 - key_len);
                if (subkey_len > MAXSTR - 1) subkey_len = MAXSTR - 1;
                
                char SubKey[MAXSTR];
                if (subkey_len > 0 && Size > 2 + key_len) {
                    memcpy(SubKey, Data + 2 + key_len, subkey_len);
                    SubKey[subkey_len] = '\0';
                } else {
                    SubKey[0] = '\0';
                }
                
                const char* result = cmsIT8GetPropertyMulti(hIT8, Key, SubKey);
                // Test with known keys
                const char* result2 = cmsIT8GetPropertyMulti(hIT8, "COLOR_REP", "RGB");
                const char* result3 = cmsIT8GetPropertyMulti(hIT8, "KEY1", NULL);
            }
            break;
        }
        
        case 2: { // cmsIT8SetPropertyUncooked
            if (hIT8 && Size >= 2) {
                size_t key_len = Data[0] % (Size - 1);
                if (key_len > MAXSTR - 1) key_len = MAXSTR - 1;
                
                char Key[MAXSTR];
                if (key_len > 0) {
                    memcpy(Key, Data + 1, key_len);
                    Key[key_len] = '\0';
                } else {
                    Key[0] = '\0';
                }
                
                size_t buffer_len = Data[1] % (Size - 2 - key_len);
                if (buffer_len > MAXSTR - 1) buffer_len = MAXSTR - 1;
                
                char Buffer[MAXSTR];
                if (buffer_len > 0 && Size > 2 + key_len) {
                    memcpy(Buffer, Data + 2 + key_len, buffer_len);
                    Buffer[buffer_len] = '\0';
                } else {
                    Buffer[0] = '\0';
                }
                
                cmsBool success = cmsIT8SetPropertyUncooked(hIT8, Key, Buffer);
            }
            break;
        }
        
        case 3: { // cmsstrcasecmp
            if (Size >= 2) {
                size_t s1_len = Data[0] % (Size - 1);
                if (s1_len > MAXSTR - 1) s1_len = MAXSTR - 1;
                
                char s1[MAXSTR];
                if (s1_len > 0) {
                    memcpy(s1, Data + 1, s1_len);
                    s1[s1_len] = '\0';
                } else {
                    s1[0] = '\0';
                }
                
                size_t s2_len = Data[1] % (Size - 2 - s1_len);
                if (s2_len > MAXSTR - 1) s2_len = MAXSTR - 1;
                
                char s2[MAXSTR];
                if (s2_len > 0 && Size > 2 + s1_len) {
                    memcpy(s2, Data + 2 + s1_len, s2_len);
                    s2[s2_len] = '\0';
                } else {
                    s2[0] = '\0';
                }
                
                int result = cmsstrcasecmp(s1, s2);
                // Test with empty strings
                int result2 = cmsstrcasecmp("", "");
                int result3 = cmsstrcasecmp("TEST", "test");
                int result4 = cmsstrcasecmp("test", "TEST");
            }
            break;
        }
        
        case 4: { // cmsNamedColorIndex
            if (namedColorList && Size >= 1) {
                size_t name_len = Data[0] % Size;
                if (name_len > MAXSTR - 1) name_len = MAXSTR - 1;
                
                char Name[MAXSTR];
                if (name_len > 0) {
                    memcpy(Name, Data + 1, name_len);
                    Name[name_len] = '\0';
                } else {
                    Name[0] = '\0';
                }
                
                cmsInt32Number index = cmsNamedColorIndex(namedColorList, Name);
                // Test with known colors
                cmsInt32Number index2 = cmsNamedColorIndex(namedColorList, "Red");
                cmsInt32Number index3 = cmsNamedColorIndex(namedColorList, "GREEN");
                cmsInt32Number index4 = cmsNamedColorIndex(namedColorList, "Blue");
                cmsInt32Number index5 = cmsNamedColorIndex(NULL, "Red");
            }
            break;
        }
        
        case 5: { // cmsIT8EnumDataFormat
            if (hIT8) {
                char **SampleNames = NULL;
                int count = cmsIT8EnumDataFormat(hIT8, &SampleNames);
                // SampleNames is owned by the IT8 handle, no need to free
            }
            break;
        }
    }
    
    // Cleanup
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    if (namedColorList) {
        cmsFreeNamedColorList(namedColorList);
    }
    
    return 0;
}