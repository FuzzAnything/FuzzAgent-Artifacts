// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_subparts at mime.c:1489:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize curl
    CURL *easy = curl_easy_init();
    if (!easy) {
        return 0;
    }

    // Test curl_mime_init
    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        return 0;
    }

    // Use first byte to determine number of parts to create
    uint8_t num_parts = Data[0] % 10;
    if (num_parts == 0) num_parts = 1;

    size_t data_offset = 1;
    curl_mimepart *parts[10];
    curl_mime *submimes[10];

    for (int i = 0; i < num_parts && data_offset < Size; i++) {
        // Add a part
        parts[i] = curl_mime_addpart(mime);
        if (!parts[i]) {
            continue;
        }

        // Determine action based on next byte
        uint8_t action = Data[data_offset % Size];
        data_offset++;

        switch (action % 4) {
            case 0: {
                // Set data from memory
                size_t chunk_size = (Size - data_offset) / (num_parts - i);
                if (chunk_size > 0 && data_offset < Size) {
                    size_t actual_size = chunk_size < (Size - data_offset) ? 
                                        chunk_size : (Size - data_offset);
                    curl_mime_data(parts[i], 
                                  (const char *)(Data + data_offset), 
                                  actual_size);
                    data_offset += actual_size;
                }
                break;
            }
            case 1: {
                // Set data from file
                size_t chunk_size = (Size - data_offset) / (num_parts - i);
                if (chunk_size > 0 && data_offset < Size) {
                    size_t actual_size = chunk_size < (Size - data_offset) ? 
                                        chunk_size : (Size - data_offset);
                    write_dummy_file(Data + data_offset, actual_size);
                    curl_mime_filedata(parts[i], "./dummy_file");
                    remove_dummy_file();
                    data_offset += actual_size;
                }
                break;
            }
            case 2: {
                // Create subparts
                submimes[i] = curl_mime_init(easy);
                if (submimes[i]) {
                    curl_mimepart *subpart = curl_mime_addpart(submimes[i]);
                    if (subpart && data_offset < Size) {
                        size_t chunk_size = (Size - data_offset) / (num_parts - i);
                        if (chunk_size > 0) {
                            size_t actual_size = chunk_size < (Size - data_offset) ? 
                                                chunk_size : (Size - data_offset);
                            curl_mime_data(subpart, 
                                          (const char *)(Data + data_offset), 
                                          actual_size);
                            data_offset += actual_size;
                        }
                        curl_mime_subparts(parts[i], submimes[i]);
                    } else {
                        curl_mime_free(submimes[i]);
                        submimes[i] = NULL;
                    }
                }
                break;
            }
            case 3: {
                // Set empty data
                curl_mime_data(parts[i], NULL, 0);
                break;
            }
        }
    }

    // Clean up
    curl_mime_free(mime);
    curl_easy_cleanup(easy);

    return 0;
}