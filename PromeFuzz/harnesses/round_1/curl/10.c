// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    switch (selector % 6) {
        case 0: {
            if (Size - data_offset < sizeof(CURLoption)) break;
            CURLoption opt_id;
            memcpy(&opt_id, Data + data_offset, sizeof(CURLoption));
            data_offset += sizeof(CURLoption);
            
            const struct curl_easyoption *opt = curl_easy_option_by_id(opt_id);
            if (opt) {
                long param = 0;
                if (Size - data_offset >= sizeof(long)) {
                    memcpy(&param, Data + data_offset, sizeof(long));
                }
                curl_easy_setopt(curl, opt_id, param);
            }
            break;
        }
        
        case 1: {
            if (Size - data_offset < 4) break;
            uint32_t name_len = *(uint32_t*)(Data + data_offset);
            data_offset += sizeof(uint32_t);
            
            if (name_len > 0 && name_len <= Size - data_offset) {
                char *name = (char*)malloc(name_len + 1);
                if (name) {
                    memcpy(name, Data + data_offset, name_len);
                    name[name_len] = '\0';
                    const struct curl_easyoption *opt = curl_easy_option_by_name(name);
                    if (opt && Size - data_offset - name_len >= sizeof(long)) {
                        long param = 0;
                        memcpy(&param, Data + data_offset + name_len, sizeof(long));
                        curl_easy_setopt(curl, opt->id, param);
                    }
                    free(name);
                }
            }
            break;
        }
        
        case 2: {
            const struct curl_easyoption *opt = NULL;
            while ((opt = curl_easy_option_next(opt)) != NULL) {
                if (opt->id % 7 == selector) {
                    long param = 0;
                    if (Size - data_offset >= sizeof(long)) {
                        memcpy(&param, Data + data_offset, sizeof(long));
                    }
                    curl_easy_setopt(curl, opt->id, param);
                    break;
                }
            }
            break;
        }
        
        case 3: {
            curl_easy_setopt(curl, CURLOPT_URL, "./dummy_file");
            if (Size - data_offset >= sizeof(long)) {
                long follow = 0;
                memcpy(&follow, Data + data_offset, sizeof(long));
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, follow);
            }
            curl_easy_perform(curl);
            break;
        }
        
        case 4: {
            curl_easy_setopt(curl, CURLOPT_URL, "./dummy_file");
            curl_easy_perform(curl);
            
            if (Size - data_offset >= sizeof(CURLINFO)) {
                CURLINFO info;
                memcpy(&info, Data + data_offset, sizeof(CURLINFO));
                data_offset += sizeof(CURLINFO);
                
                switch (info) {
                    case CURLINFO_RESPONSE_CODE: {
                        long code;
                        curl_easy_getinfo(curl, info, &code);
                        break;
                    }
                    case CURLINFO_TOTAL_TIME: {
                        double time;
                        curl_easy_getinfo(curl, info, &time);
                        break;
                    }
                    case CURLINFO_SIZE_DOWNLOAD: {
                        double size;
                        curl_easy_getinfo(curl, info, &size);
                        break;
                    }
                    default: {
                        long dummy;
                        curl_easy_getinfo(curl, info, &dummy);
                        break;
                    }
                }
            }
            break;
        }
        
        case 5: {
            if (Size - data_offset >= 2 * sizeof(CURLoption)) {
                CURLoption opt1, opt2;
                memcpy(&opt1, Data + data_offset, sizeof(CURLoption));
                data_offset += sizeof(CURLoption);
                memcpy(&opt2, Data + data_offset, sizeof(CURLoption));
                
                const struct curl_easyoption *opt_info1 = curl_easy_option_by_id(opt1);
                const struct curl_easyoption *opt_info2 = curl_easy_option_by_id(opt2);
                
                if (opt_info1 && opt_info2) {
                    long param1 = 0, param2 = 0;
                    if (Size - data_offset - sizeof(CURLoption) >= 2 * sizeof(long)) {
                        memcpy(&param1, Data + data_offset + sizeof(CURLoption), sizeof(long));
                        memcpy(&param2, Data + data_offset + sizeof(CURLoption) + sizeof(long), sizeof(long));
                    }
                    curl_easy_setopt(curl, opt1, param1);
                    curl_easy_setopt(curl, opt2, param2);
                    curl_easy_perform(curl);
                }
            }
            break;
        }
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}