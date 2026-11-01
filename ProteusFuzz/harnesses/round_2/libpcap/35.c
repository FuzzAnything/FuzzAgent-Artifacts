// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/* Helper to create a dummy file if needed */
static void create_dummy_file(const char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        const char *dummy_content = "dummy pcap content";
        write(fd, dummy_content, strlen(dummy_content));
        close(fd);
    }
}

/* Helper to safely free device list */
static void safe_freealldevs(pcap_if_t *alldevs) {
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

/* Helper to safely close pcap handle */
static void safe_close_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure the input data is null-terminated for string operations */
    char *source_str = (char *)malloc(Size + 1);
    if (!source_str) return 0;
    memcpy(source_str, Data, Size);
    source_str[Size] = '\0';

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    /* 1. Test pcap_findalldevs_ex */
    pcap_if_t *alldevs = NULL;
    struct pcap_rmtauth auth = {0};
    /* Try with NULL auth and a potentially malformed source */
    int ret = pcap_findalldevs_ex(source_str, &auth, &alldevs, errbuf);
    if (ret == 0 && alldevs != NULL) {
        safe_freealldevs(alldevs);
    }
    /* Try with NULL auth pointer */
    ret = pcap_findalldevs_ex(source_str, NULL, &alldevs, errbuf);
    if (ret == 0 && alldevs != NULL) {
        safe_freealldevs(alldevs);
    }

    /* 2. Test pcap_create */
    pcap_t *handle = NULL;
    handle = pcap_create(source_str, errbuf);
    safe_close_pcap(handle);
    /* Try with NULL source (defaults to "any") */
    handle = pcap_create(NULL, errbuf);
    safe_close_pcap(handle);

    /* 3. Test pcap_createsrcstr */
    char source_buf[1024];
    int type = (Size > 0) ? (int)Data[0] : 0;
    /* Create dummy strings for host, port, name from input data */
    char host[256], port[256], name[256];
    memset(host, 0, sizeof(host));
    memset(port, 0, sizeof(port));
    memset(name, 0, sizeof(name));
    size_t copy_len = Size < 255 ? Size : 255;
    memcpy(host, Data, copy_len);
    memcpy(port, Data, copy_len);
    memcpy(name, Data, copy_len);
    
    ret = pcap_createsrcstr(source_buf, type, host, port, name, errbuf);
    /* If success, try to parse it back */
    if (ret == 0) {
        int parsed_type;
        char parsed_host[256], parsed_port[256], parsed_name[256];
        memset(&parsed_host, 0, sizeof(parsed_host));
        memset(&parsed_port, 0, sizeof(parsed_port));
        memset(&parsed_name, 0, sizeof(parsed_name));
        
        /* 4. Test pcap_parsesrcstr */
        ret = pcap_parsesrcstr(source_buf, &parsed_type, parsed_host, parsed_port, parsed_name, errbuf);
    }
    
    /* Try parsing the original source string directly */
    int parsed_type;
    char p_host[256], p_port[256], p_name[256];
    memset(p_host, 0, sizeof(p_host));
    memset(p_port, 0, sizeof(p_port));
    memset(p_name, 0, sizeof(p_name));
    ret = pcap_parsesrcstr(source_str, &parsed_type, p_host, p_port, p_name, errbuf);

    /* 5. Test pcap_open */
    /* Ensure dummy file exists for file-based source testing */
    create_dummy_file("./dummy_file");
    
    /* Try opening with source string (might be a file or remote) */
    handle = pcap_open(source_str, 65535, 0, 1000, &auth, errbuf);
    safe_close_pcap(handle);
    
    /* Try with NULL auth */
    handle = pcap_open(source_str, 65535, 0, 1000, NULL, errbuf);
    safe_close_pcap(handle);

    /* 6. Test pcap_open_live */
    handle = pcap_open_live(source_str, 65535, 1, 1000, errbuf);
    safe_close_pcap(handle);
    
    /* Try with NULL source */
    handle = pcap_open_live(NULL, 65535, 1, 1000, errbuf);
    safe_close_pcap(handle);

    free(source_str);
    return 0;
}