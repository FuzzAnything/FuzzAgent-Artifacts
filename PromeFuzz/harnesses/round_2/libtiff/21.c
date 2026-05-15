// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Define GPS tags if not already defined */
#ifndef TIFFTAG_GPSVERSIONID
#define TIFFTAG_GPSVERSIONID 0
#endif
#ifndef TIFFTAG_GPSLATITUDEREF
#define TIFFTAG_GPSLATITUDEREF 1
#endif
#ifndef TIFFTAG_GPSLATITUDE
#define TIFFTAG_GPSLATITUDE 2
#endif
#ifndef TIFFTAG_GPSLONGITUDEREF
#define TIFFTAG_GPSLONGITUDEREF 3
#endif
#ifndef TIFFTAG_GPSLONGITUDE
#define TIFFTAG_GPSLONGITUDE 4
#endif
#ifndef TIFFTAG_GPSALTITUDEREF
#define TIFFTAG_GPSALTITUDEREF 5
#endif
#ifndef TIFFTAG_GPSALTITUDE
#define TIFFTAG_GPSALTITUDE 6
#endif
#ifndef TIFFTAG_GPSTIMESTAMP
#define TIFFTAG_GPSTIMESTAMP 7
#endif
#ifndef TIFFTAG_GPSSATELLITES
#define TIFFTAG_GPSSATELLITES 8
#endif
#ifndef TIFFTAG_GPSSTATUS
#define TIFFTAG_GPSSTATUS 9
#endif
#ifndef TIFFTAG_GPSMEASUREMODE
#define TIFFTAG_GPSMEASUREMODE 10
#endif
#ifndef TIFFTAG_GPSDOP
#define TIFFTAG_GPSDOP 11
#endif
#ifndef TIFFTAG_GPSSPEEDREF
#define TIFFTAG_GPSSPEEDREF 12
#endif
#ifndef TIFFTAG_GPSSPEED
#define TIFFTAG_GPSSPEED 13
#endif
#ifndef TIFFTAG_GPSTRACKREF
#define TIFFTAG_GPSTRACKREF 14
#endif
#ifndef TIFFTAG_GPSTRACK
#define TIFFTAG_GPSTRACK 15
#endif
#ifndef TIFFTAG_GPSIMGDIRECTIONREF
#define TIFFTAG_GPSIMGDIRECTIONREF 16
#endif
#ifndef TIFFTAG_GPSIMGDIRECTION
#define TIFFTAG_GPSIMGDIRECTION 17
#endif
#ifndef TIFFTAG_GPSMAPDATUM
#define TIFFTAG_GPSMAPDATUM 18
#endif
#ifndef TIFFTAG_GPSDESTLATITUDEREF
#define TIFFTAG_GPSDESTLATITUDEREF 19
#endif
#ifndef TIFFTAG_GPSDESTLATITUDE
#define TIFFTAG_GPSDESTLATITUDE 20
#endif
#ifndef TIFFTAG_GPSDESTLONGITUDEREF
#define TIFFTAG_GPSDESTLONGITUDEREF 21
#endif
#ifndef TIFFTAG_GPSDESTLONGITUDE
#define TIFFTAG_GPSDESTLONGITUDE 22
#endif
#ifndef TIFFTAG_GPSDESTBEARINGREF
#define TIFFTAG_GPSDESTBEARINGREF 23
#endif
#ifndef TIFFTAG_GPSDESTBEARING
#define TIFFTAG_GPSDESTBEARING 24
#endif
#ifndef TIFFTAG_GPSDESTDISTANCEREF
#define TIFFTAG_GPSDESTDISTANCEREF 25
#endif
#ifndef TIFFTAG_GPSDESTDISTANCE
#define TIFFTAG_GPSDESTDISTANCE 26
#endif
#ifndef TIFFTAG_GPSPROCESSINGMETHOD
#define TIFFTAG_GPSPROCESSINGMETHOD 27
#endif
#ifndef TIFFTAG_GPSAREAINFORMATION
#define TIFFTAG_GPSAREAINFORMATION 28
#endif
#ifndef TIFFTAG_GPSDATESTAMP
#define TIFFTAG_GPSDATESTAMP 29
#endif

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    uint32_t offset = 0;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, (uint16_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, (uint16_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, (uint16_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, (uint16_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, (uint16_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, (uint16_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_XRESOLUTION, (float)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_YRESOLUTION, (float)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_STRIPBYTECOUNTS, (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_STRIPOFFSETS, (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_FILLORDER, (uint16_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_DOCUMENTNAME, "dummy");
    TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, "fuzzed");
    TIFFSetField(tif, TIFFTAG_MAKE, "fuzzer");
    TIFFSetField(tif, TIFFTAG_MODEL, "test");
    TIFFSetField(tif, TIFFTAG_SOFTWARE, "libtiff_fuzzer");
    
    TIFFFindField(tif, TIFFTAG_IMAGEWIDTH, TIFF_LONG);
    TIFFSetField(tif, TIFFTAG_PAGENUMBER, (uint16_t)Data[offset++], (uint16_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_SUBFILETYPE, (uint32_t)Data[offset++]);
    
    uint8_t *scanline = malloc(100);
    if (scanline) {
        memcpy(scanline, Data + offset, 100);
        offset += 100;
        TIFFWriteScanline(tif, scanline, 0, 0);
        free(scanline);
    }
    
    TIFFWriteDirectory(tif);
    TIFFCreateGPSDirectory(tif);
    
    TIFFSetField(tif, TIFFTAG_GPSVERSIONID, (uint8_t)Data[offset++], (uint8_t)Data[offset++], 
                 (uint8_t)Data[offset++], (uint8_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSLATITUDEREF, Data[offset++] ? "N" : "S");
    TIFFSetField(tif, TIFFTAG_GPSLATITUDE, (uint32_t)Data[offset++], (uint32_t)Data[offset++], 
                 (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSLONGITUDEREF, Data[offset++] ? "E" : "W");
    TIFFSetField(tif, TIFFTAG_GPSLONGITUDE, (uint32_t)Data[offset++], (uint32_t)Data[offset++], 
                 (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSALTITUDEREF, (uint8_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSALTITUDE, (uint32_t)Data[offset++], (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSTIMESTAMP, (uint32_t)Data[offset++], (uint32_t)Data[offset++], 
                 (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSSATELLITES, "01");
    TIFFSetField(tif, TIFFTAG_GPSSTATUS, Data[offset++] ? "A" : "V");
    TIFFSetField(tif, TIFFTAG_GPSMEASUREMODE, (uint8_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSDOP, (uint32_t)Data[offset++], (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSSPEEDREF, Data[offset++] ? "K" : "M");
    TIFFSetField(tif, TIFFTAG_GPSSPEED, (uint32_t)Data[offset++], (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSTRACKREF, "T");
    TIFFSetField(tif, TIFFTAG_GPSTRACK, (uint32_t)Data[offset++], (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSIMGDIRECTIONREF, "M");
    TIFFSetField(tif, TIFFTAG_GPSIMGDIRECTION, (uint32_t)Data[offset++], (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSMAPDATUM, "WGS-84");
    TIFFSetField(tif, TIFFTAG_GPSDESTLATITUDEREF, Data[offset++] ? "N" : "S");
    TIFFSetField(tif, TIFFTAG_GPSDESTLATITUDE, (uint32_t)Data[offset++], (uint32_t)Data[offset++], 
                 (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSDESTLONGITUDEREF, Data[offset++] ? "E" : "W");
    TIFFSetField(tif, TIFFTAG_GPSDESTLONGITUDE, (uint32_t)Data[offset++], (uint32_t)Data[offset++], 
                 (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSDESTBEARINGREF, "T");
    TIFFSetField(tif, TIFFTAG_GPSDESTBEARING, (uint32_t)Data[offset++], (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSDESTDISTANCEREF, "K");
    TIFFSetField(tif, TIFFTAG_GPSDESTDISTANCE, (uint32_t)Data[offset++], (uint32_t)Data[offset++]);
    TIFFSetField(tif, TIFFTAG_GPSPROCESSINGMETHOD, "GPS");
    TIFFSetField(tif, TIFFTAG_GPSAREAINFORMATION, "Area");
    TIFFSetField(tif, TIFFTAG_GPSDATESTAMP, "2024:01:01");
    
    uint64_t custom_dir_offset = 0;
    TIFFWriteCustomDirectory(tif, &custom_dir_offset);
    
    TIFFClose(tif);
    remove_dummy_file();
    
    return 0;
}