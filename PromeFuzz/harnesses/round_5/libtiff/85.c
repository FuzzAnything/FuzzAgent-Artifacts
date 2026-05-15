// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldIsAnonymous at tif_dirinfo.c:879:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    /* Create a dummy TIFF file to get a valid TIFF pointer */
    static const char dummy_filename[] = "./dummy_file.tiff";
    
    /* Write minimal valid TIFF header to file */
    FILE *fp = fopen(dummy_filename, "wb");
    if (!fp) {
        return 0;
    }
    
    /* Minimal TIFF header: II (little-endian), version 42, IFD offset 8 */
    uint8_t tiff_header[] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
    fwrite(tiff_header, 1, sizeof(tiff_header), fp);
    fclose(fp);
    
    /* Open the TIFF file */
    TIFF *tif = TIFFOpen(dummy_filename, "r");
    if (!tif) {
        remove(dummy_filename);
        return 0;
    }
    
    /* Use the fuzzer input to generate a tag value */
    if (Size < sizeof(uint32_t)) {
        TIFFClose(tif);
        remove(dummy_filename);
        return 0;
    }
    
    uint32_t tag;
    memcpy(&tag, Data, sizeof(uint32_t));
    
    /* Get a TIFFField pointer using TIFFFieldWithTag with valid TIFF pointer */
    const TIFFField* field = TIFFFieldWithTag(tif, tag);
    
    /* If no field found for this tag, try some known tags */
    if (field == NULL) {
        static const uint32_t known_tags[] = {
            256, 257, 258, 259, 262, 273, 277, 278, 279, 282, 283, 284, 286, 287,
            296, 297, 301, 317, 320, 338, 339, 530, 531, 532, 33421, 33422, 33432,
            33434, 33437, 34665, 34853, 34908, 34910, 34911, 34912, 34913, 34914,
            34915, 34916, 34917, 34918, 34919, 34920, 34921, 34922, 34923, 34924,
            34925, 34926, 34927, 34928, 34929, 34930, 34931, 34932, 34933, 34934,
            34935, 34936, 34937, 34938, 34939, 34940, 34941, 34942, 34943, 34944,
            34945, 34946, 34947, 34948, 34949, 34950, 34951, 34952, 34953, 34954,
            34955, 34956, 34957, 34958, 34959, 34960, 34961, 34962, 34963, 34964,
            34965, 34966, 34967, 34968, 34969, 34970, 34971, 34972, 34973, 34974,
            34975, 34976, 34977, 34978, 34979, 34980, 34981, 34982, 34983, 34984,
            34985, 34986, 34987, 34988, 34989, 34990, 34991, 34992, 34993, 34994,
            34995, 34996, 34997, 34998, 34999, 35000
        };
        
        size_t num_tags = sizeof(known_tags) / sizeof(known_tags[0]);
        if (num_tags > 0) {
            size_t tag_index = tag % num_tags;
            field = TIFFFieldWithTag(tif, known_tags[tag_index]);
        }
    }
    
    /* Only call the target functions if we have a valid field */
    if (field != NULL) {
        /* Call all six target functions with the obtained field */
        int result1 = TIFFFieldIsAnonymous(field);
        int result2 = TIFFFieldWriteCount(field);
        int result3 = TIFFFieldReadCount(field);
        int result4 = TIFFFieldPassCount(field);
        int result5 = TIFFFieldSetGetSize(field);
        int result6 = TIFFFieldSetGetCountSize(field);
        
        /* Use the results to avoid dead code elimination */
        volatile int dummy = result1 + result2 + result3 + result4 + result5 + result6;
        (void)dummy;
    }
    
    /* Cleanup */
    TIFFClose(tif);
    remove(dummy_filename);
    
    return 0;
}