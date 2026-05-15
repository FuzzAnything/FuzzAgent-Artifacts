// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadGPSDirectory at tif_dirread.c:5470:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    uint32_t width = 0, height = 0;
    uint16_t bitspersample = 0, samplesperpixel = 0;
    uint16_t compression = 0, photometric = 0;
    uint32_t rowsperstrip = 0;
    float xresolution = 0.0f, yresolution = 0.0f;
    uint16_t resolutionunit = 0;
    uint32_t* stripoffsets = NULL;
    uint32_t* stripbytecounts = NULL;
    uint16_t* colormap[3] = {NULL, NULL, NULL};
    uint32_t exififd = 0, gpsifd = 0;
    uint32_t tilewidth = 0, tilelength = 0;
    uint32_t subfiletype = 0;
    uint16_t planarconfig = 0;
    uint16_t orientation = 0;
    uint32_t imagelength = 0;
    uint32_t imagewidth = 0;
    uint16_t fillorder = 0;
    uint32_t group3options = 0;
    uint32_t group4options = 0;
    uint16_t predictor = 0;
    uint16_t extrasamples = 0;
    uint16_t* sampleinfo = NULL;
    uint32_t stonits = 0;
    float* refblackwhite = NULL;
    uint16_t* transferfunction[3] = {NULL, NULL, NULL};
    uint16_t inkset = 0;
    uint16_t* inknames = NULL;
    uint32_t numberofinks = 0;
    uint16_t dotrange[2] = {0, 0};
    uint16_t* ycbcrcoeffs = NULL;
    uint16_t ycbcrsubsampling[2] = {0, 0};
    uint16_t ycbcrpositioning = 0;
    float* whitepoint = NULL;
    float* primarychromaticities = NULL;
    uint16_t* halftonehints = NULL;
    uint32_t tileoffsets = 0;
    uint32_t tilebytecounts = 0;
    uint32_t jpegtables = 0;
    uint32_t jpegquality = 0;
    uint32_t jpegcolormode = 0;
    uint32_t jpegtablesmode = 0;
    uint32_t faxmode = 0;
    uint32_t faxfillfunc = 0;
    uint32_t badfaxlines = 0;
    uint32_t cleanfaxdata = 0;
    uint32_t consecutivabadfaxlines = 0;
    char* copyright = NULL;
    char* datetime = NULL;
    char* artist = NULL;
    char* hostcomputer = NULL;
    char* imagemescription = NULL;
    char* make = NULL;
    char* model = NULL;
    char* software = NULL;
    char* documentname = NULL;
    char* pagename = NULL;
    char* page_number[2] = {NULL, NULL};
    char* targetprinter = NULL;
    char* richTIFFIPTC = NULL;
    char* xmp = NULL;
    char* iptc = NULL;
    char* photoshop = NULL;
    char* iccprofile = NULL;
    uint32_t iccprofilesize = 0;
    uint16_t* transferrange = NULL;
    uint32_t stilewidth = 0;
    uint32_t stilelength = 0;
    uint32_t sgdalmetadata = 0;
    uint32_t sgdalnodata = 0;
    uint32_t subifd = 0;
    uint32_t* subifd_offsets = NULL;
    uint32_t jpegproc = 0;
    uint32_t jpegifoffset = 0;
    uint32_t jpegifbytelength = 0;
    uint32_t jpegrestartinterval = 0;
    uint32_t jpeglosslesspredictors = 0;
    uint32_t jpegpointtransforms = 0;
    uint32_t jpegqtables = 0;
    uint32_t jpegdctables = 0;
    uint32_t jpegactables = 0;
    uint32_t ycbcrcoefficients = 0;
    uint32_t ycbcrsubsampling_h = 0;
    uint32_t ycbcrsubsampling_v = 0;
    uint32_t ycbcrpositioning_val = 0;
    uint32_t referenceblackwhite = 0;
    uint32_t inknames_len = 0;
    uint32_t numberinks = 0;
    uint32_t dotrange_val = 0;
    uint32_t halftonehints_val = 0;
    uint32_t tileoffsets_val = 0;
    uint32_t tilebytecounts_val = 0;
    uint32_t jpegtables_val = 0;
    uint32_t badfaxlines_val = 0;
    uint32_t cleanfaxdata_val = 0;
    uint32_t consecutivabadfaxlines_val = 0;
    uint32_t stilewidth_val = 0;
    uint32_t stilelength_val = 0;
    uint32_t sgdalmetadata_val = 0;
    uint32_t sgdalnodata_val = 0;
    uint32_t subifd_val = 0;
    uint32_t jpegifoffset_val = 0;
    uint32_t jpegifbytelength_val = 0;
    uint32_t jpegrestartinterval_val = 0;
    uint32_t jpeglosslesspredictors_val = 0;
    uint32_t jpegpointtransforms_val = 0;
    uint32_t jpegqtables_val = 0;
    uint32_t jpegdctables_val = 0;
    uint32_t jpegactables_val = 0;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    TIFFGetField(tif, TIFFTAG_STRIPOFFSETS, &stripoffsets);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    TIFFGetField(tif, TIFFTAG_STRIPBYTECOUNTS, &stripbytecounts);
    TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xresolution);
    
    TIFFReadGPSDirectory(tif, 0);
    
    TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yresolution);
    TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT, &resolutionunit);
    TIFFGetField(tif, TIFFTAG_COLORMAP, &colormap[0], &colormap[1], &colormap[2]);
    TIFFGetField(tif, TIFFTAG_EXIFIFD, &exififd);
    TIFFGetField(tif, TIFFTAG_GPSIFD, &gpsifd);
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tilewidth);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &tilelength);
    TIFFGetField(tif, TIFFTAG_SUBFILETYPE, &subfiletype);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarconfig);
    
    TIFFSetDirectory(tif, 0);
    
    TIFFGetField(tif, TIFFTAG_ORIENTATION, &orientation);
    
    TIFFReadEXIFDirectory(tif, 0);
    
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imagelength);
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imagewidth);
    TIFFGetField(tif, TIFFTAG_FILLORDER, &fillorder);
    TIFFGetField(tif, TIFFTAG_GROUP3OPTIONS, &group3options);
    TIFFGetField(tif, TIFFTAG_GROUP4OPTIONS, &group4options);
    TIFFGetField(tif, TIFFTAG_PREDICTOR, &predictor);
    TIFFGetField(tif, TIFFTAG_EXTRASAMPLES, &extrasamples, &sampleinfo);
    TIFFGetField(tif, TIFFTAG_STONITS, &stonits);
    TIFFGetField(tif, TIFFTAG_REFERENCEBLACKWHITE, &refblackwhite);
    
    const TIFFField *field = TIFFFindField(tif, TIFFTAG_TRANSFERFUNCTION, TIFF_SHORT);
    
    TIFFGetField(tif, TIFFTAG_TRANSFERFUNCTION, &transferfunction[0], &transferfunction[1], &transferfunction[2]);
    TIFFGetField(tif, TIFFTAG_INKSET, &inkset);
    TIFFGetField(tif, TIFFTAG_INKNAMES, &inknames_len, &inknames);
    TIFFGetField(tif, TIFFTAG_NUMBEROFINKS, &numberofinks);
    TIFFGetField(tif, TIFFTAG_DOTRANGE, &dotrange[0], &dotrange[1]);
    TIFFGetField(tif, TIFFTAG_YCBCRCOEFFICIENTS, &ycbcrcoeffs);
    TIFFGetField(tif, TIFFTAG_YCBCRSUBSAMPLING, &ycbcrsubsampling[0], &ycbcrsubsampling[1]);
    TIFFGetField(tif, TIFFTAG_YCBCRPOSITIONING, &ycbcrpositioning);
    TIFFGetField(tif, TIFFTAG_WHITEPOINT, &whitepoint);
    TIFFGetField(tif, TIFFTAG_PRIMARYCHROMATICITIES, &primarychromaticities);
    TIFFGetField(tif, TIFFTAG_HALFTONEHINTS, &halftonehints[0], &halftonehints[1]);
    
    TIFFClose(tif);
    TIFFClose(tif);
    
    remove_dummy_file();
    return 0;
}