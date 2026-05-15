// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int get_int_from_data(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) {
        return 0;
    }
    int value;
    memcpy(&value, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return value;
}

static u_int get_u_int_from_data(const uint8_t **data, size_t *size) {
    if (*size < sizeof(u_int)) {
        return 0;
    }
    u_int value;
    memcpy(&value, *data, sizeof(u_int));
    *data += sizeof(u_int);
    *size -= sizeof(u_int);
    return value;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const uint8_t *data_ptr = Data;
    size_t data_size = Size;

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = NULL;
    pcap_t *dead_handle = NULL;

    int scenario = get_int_from_data(&data_ptr, &data_size) % 6;

    switch (scenario) {
        case 0: {
            pcap_handle = pcap_create("lo", errbuf);
            if (pcap_handle == NULL) {
                break;
            }
            int dlt = get_int_from_data(&data_ptr, &data_size);
            (void)pcap_set_datalink(pcap_handle, dlt);
            pcap_close(pcap_handle);
            break;
        }
        case 1: {
            pcap_handle = pcap_create("any", errbuf);
            if (pcap_handle == NULL) {
                break;
            }
            (void)pcap_activate(pcap_handle);
            pcap_close(pcap_handle);
            break;
        }
        case 2: {
            pcap_handle = pcap_create("eth0", errbuf);
            if (pcap_handle == NULL) {
                break;
            }
            (void)pcap_get_tstamp_precision(pcap_handle);
            pcap_close(pcap_handle);
            break;
        }
        case 3: {
            FILE *fp = fopen("./dummy_file", "wb");
            if (fp != NULL) {
                if (data_size > 0) {
                    fwrite(data_ptr, 1, data_size, fp);
                }
                fclose(fp);
            }

            pcap_handle = pcap_open_offline("./dummy_file", errbuf);
            if (pcap_handle != NULL) {
                (void)pcap_minor_version(pcap_handle);
                pcap_close(pcap_handle);
            }
            unlink("./dummy_file");
            break;
        }
        case 4: {
            pcap_handle = pcap_create("wlan0", errbuf);
            if (pcap_handle == NULL) {
                break;
            }
            int tstamp_type = get_int_from_data(&data_ptr, &data_size);
            (void)pcap_set_tstamp_type(pcap_handle, tstamp_type);
            pcap_close(pcap_handle);
            break;
        }
        case 5: {
            int linktype = get_int_from_data(&data_ptr, &data_size);
            int precision = get_int_from_data(&data_ptr, &data_size);
            u_int snaplen = get_u_int_from_data(&data_ptr, &data_size);
            dead_handle = pcap_open_dead_with_tstamp_precision(linktype, precision, snaplen);
            if (dead_handle != NULL) {
                pcap_close(dead_handle);
            }
            break;
        }
    }

    return 0;
}