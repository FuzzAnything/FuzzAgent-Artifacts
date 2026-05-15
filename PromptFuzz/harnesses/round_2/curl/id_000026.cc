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
#include <multi.h>
}
//<ID> 323
//<Prompt> []
/*<Combination>: [
*/
//<score> 132, nr_unique_branch: 228
//<Quality> {"density":11,"unique_branches":{"Curl_mime_set_subparts":[[1480,22,1480,36,0,0,4,0]],"Curl_mime_read":[[1516,11,1516,30,0,0,4,1]],"Curl_mntfy_enable":[[150,6,150,34,0,0,4,1]],"add_content_type":[[1616,31,1616,39,0,0,4,0],[1617,31,1617,39,0,0,4,0]],"read_part_content":[[696,3,696,9,0,0,4,1],[697,3,697,27,0,0,4,1],[698,3,698,27,0,0,4,1],[699,3,699,18,0,0,4,1],[701,3,701,10,0,0,4,0],[706,6,706,38,0,0,4,0],[706,6,706,38,0,0,4,1],[707,6,707,42,0,0,4,0],[707,6,707,42,0,0,4,1],[712,5,712,28,0,0,4,0],[712,5,712,28,0,0,4,1],[719,5,719,23,0,0,4,1],[723,5,723,12,0,0,4,0],[723,5,723,12,0,0,4,1],[724,10,724,24,0,0,4,0],[725,12,725,43,0,0,4,1],[737,3,737,20,0,0,4,1],[739,3,739,9,0,0,4,0],[739,3,739,9,0,0,4,1],[740,3,740,27,0,0,4,1],[741,3,741,27,0,0,4,1],[742,3,742,18,0,0,4,1],[745,3,745,10,0,0,4,0],[745,3,745,10,0,0,4,1]],"curl_formget":[[633,6,633,13,0,0,4,1],[638,6,638,13,0,0,4,0],[642,9,642,16,0,0,4,0],[646,8,646,14,0,0,4,0],[646,8,646,14,0,0,4,1],[649,8,649,30,0,0,4,1],[649,34,649,69,0,0,4,1]],"mime_mem_read":[[569,6,569,13,0,0,4,1],[572,6,572,17,0,0,4,1],[575,6,575,8,0,0,4,0]],"readback_part":[[822,9,822,16,0,0,4,0],[826,5,826,25,0,0,4,0],[826,5,826,25,0,0,4,1],[828,20,828,50,0,0,4,1],[832,5,832,31,0,0,4,0],[832,5,832,31,0,0,4,1],[833,10,833,14,0,0,4,0],[842,5,842,31,0,0,4,0],[842,5,842,31,0,0,4,1],[843,10,843,14,0,0,4,0],[843,10,843,14,0,0,4,1],[848,12,848,15,0,0,4,0],[848,12,848,15,0,0,4,1],[852,5,852,23,0,0,4,0],[852,5,852,23,0,0,4,1],[855,10,855,13,0,0,4,0],[855,10,855,13,0,0,4,1],[858,5,858,24,0,0,4,0],[858,5,858,24,0,0,4,1],[862,5,862,27,0,0,4,0],[862,5,862,27,0,0,4,1],[863,10,863,23,0,0,4,1],[867,14,867,16,0,0,4,0],[867,14,867,16,0,0,4,1],[868,7,868,13,0,0,4,0],[868,7,868,13,0,0,4,1],[871,12,871,39,0,0,4,1],[876,7,876,31,0,0,4,1],[877,7,877,31,0,0,4,1],[878,7,878,22,0,0,4,1],[879,7,879,24,0,0,4,1],[880,16,880,23,0,0,4,0],[883,5,883,23,0,0,4,0],[883,5,883,23,0,0,4,1],[885,5,885,12,0,0,4,1]],"weak_random":[[319,4,319,10,5,0,4,1]],"mime_subparts_free":[[1049,6,1049,10,0,0,4,0],[1049,14,1049,26,0,0,4,0]],"search_header":[[258,9,258,15,0,0,4,0],[258,19,258,26,0,0,4,1]],"escape_string":[[223,6,223,35,0,0,4,1],[223,40,223,44,0,0,4,1],[228,51,228,58,0,0,4,0],[228,62,228,66,0,0,4,0],[228,62,228,66,0,0,4,1],[229,20,229,22,0,0,4,0],[229,20,229,22,0,0,4,1],[229,26,229,37,0,0,4,0],[229,26,229,37,0,0,4,1],[232,8,232,10,0,0,4,0],[232,8,232,10,0,0,4,1]],"mime_subparts_read":[[906,9,906,15,0,0,4,0],[910,5,910,25,0,0,4,0],[910,5,910,25,0,0,4,1],[911,5,911,24,0,0,4,1],[918,5,918,29,0,0,4,0],[918,5,918,29,0,0,4,1],[921,10,921,13,0,0,4,0],[921,10,921,13,0,0,4,1],[924,5,924,29,0,0,4,0],[924,5,924,29,0,0,4,1],[925,10,925,14,0,0,4,0],[925,10,925,14,0,0,4,1],[931,10,931,13,0,0,4,0],[931,10,931,13,0,0,4,1],[935,5,935,27,0,0,4,0],[935,5,935,27,0,0,4,1],[936,10,936,15,0,0,4,0],[936,10,936,15,0,0,4,1],[941,14,941,16,0,0,4,0],[941,14,941,16,0,0,4,1],[942,7,942,31,0,0,4,1],[943,7,943,31,0,0,4,1],[944,7,944,22,0,0,4,1],[945,7,945,24,0,0,4,1],[947,7,947,13,0,0,4,0],[947,7,947,13,0,0,4,1],[952,5,952,23,0,0,4,0],[952,5,952,23,0,0,4,1],[954,5,954,12,0,0,4,1]],"Curl_mime_contenttype":[[1643,6,1643,14,0,0,4,1]],"Curl_getformdata":[[728,6,728,11,0,0,4,1],[732,6,732,11,0,0,4,1],[735,6,735,13,0,0,4,0],[739,9,739,16,0,0,4,0],[739,20,739,24,0,0,4,0],[739,20,739,24,0,0,4,1],[742,8,742,18,0,0,4,1],[758,22,758,29,0,0,4,0],[758,33,758,37,0,0,4,0],[758,33,758,37,0,0,4,1],[761,10,761,15,0,0,4,1],[765,10,765,17,0,0,4,0],[769,10,769,17,0,0,4,0],[769,21,769,38,0,0,4,1],[773,10,773,17,0,0,4,0],[773,21,773,32,0,0,4,0],[777,10,777,17,0,0,4,0],[780,12,780,45,0,0,4,1],[783,12,783,65,0,0,4,1],[806,17,806,46,0,0,4,1],[810,17,810,48,0,0,4,1],[820,14,820,19,0,0,4,0],[829,10,829,17,0,0,4,0],[829,21,829,39,0,0,4,1],[836,6,836,12,0,0,4,1]],"readback_bytes":[[669,6,669,23,0,0,4,0],[669,6,669,23,0,0,4,1],[675,8,675,22,0,0,4,0],[675,8,675,22,0,0,4,1],[681,6,681,18,0,0,4,1]],"Curl_mime_prepare_headers":[[1691,6,1691,48,0,0,4,1],[1696,6,1696,15,0,0,4,0],[1698,6,1698,14,0,0,4,1],[1702,6,1702,18,0,0,4,0],[1702,6,1702,18,0,0,4,1],[1704,5,1704,28,0,0,4,1],[1707,5,1707,23,0,0,4,1],[1714,5,1714,12,0,0,4,0],[1720,6,1720,38,0,0,4,0],[1720,6,1720,38,0,0,4,1],[1722,8,1722,12,0,0,4,0],[1725,11,1725,22,0,0,4,1],[1731,6,1731,72,0,0,4,0],[1732,8,1732,20,0,0,4,0],[1732,8,1732,20,0,0,4,1],[1733,10,1733,24,0,0,4,1],[1733,28,1733,38,0,0,4,1],[1734,11,1734,22,0,0,4,0],[1734,26,1734,72,0,0,4,1],[1736,8,1736,19,0,0,4,0],[1736,8,1736,19,0,0,4,1],[1736,23,1736,63,0,0,4,1],[1739,8,1739,19,0,0,4,0],[1739,8,1739,19,0,0,4,1],[1743,10,1743,20,0,0,4,0],[1745,12,1745,17,0,0,4,1],[1748,10,1748,14,0,0,4,0],[1748,18,1748,32,0,0,4,1],[1753,10,1753,14,0,0,4,0],[1757,36,1757,40,0,0,4,0],[1758,36,1758,40,0,0,4,0],[1759,36,1759,40,0,0,4,0],[1760,36,1760,44,0,0,4,1],[1761,36,1761,44,0,0,4,1],[1762,36,1762,44,0,0,4,1],[1765,10,1765,13,0,0,4,1],[1771,6,1771,17,0,0,4,0],[1771,6,1771,17,0,0,4,1],[1773,8,1773,11,0,0,4,1],[1778,6,1779,59,0,0,4,0],[1780,8,1780,21,0,0,4,1],[1782,13,1782,24,0,0,4,0],[1782,13,1782,24,0,0,4,1],[1782,28,1782,57,0,0,4,1],[1785,8,1785,11,0,0,4,1],[1795,6,1795,48,0,0,4,1],[1799,6,1799,38,0,0,4,0],[1799,6,1799,38,0,0,4,1],[1799,42,1799,46,0,0,4,0],[1803,8,1803,72,0,0,4,0],[1805,36,1805,43,0,0,4,0],[1805,36,1805,43,0,0,4,1],[1808,10,1808,13,0,0,4,1]],"curl_mime_headers":[[1410,6,1410,13,0,0,4,1]],"curl_ws_recv":[[1541,6,1541,23,0,0,4,1],[1541,34,1541,40,0,0,4,0],[1541,44,1541,51,0,0,4,1],[1545,6,1545,11,0,0,4,0],[1547,8,1547,31,0,0,4,0],[134,5,134,8,1,0,4,0],[134,12,134,49,1,0,4,0]],"setname":[[696,6,696,11,0,0,4,1],[696,15,696,19,0,0,4,0]],"content_type_match":[[1661,6,1661,17,0,0,4,0],[1661,21,1661,61,0,0,4,0],[1662,12,1662,28,0,0,4,1],[1663,5,1663,14,0,0,4,0],[1664,5,1664,14,0,0,4,1],[1665,5,1665,14,0,0,4,1],[1666,5,1666,14,0,0,4,1],[1667,5,1667,13,0,0,4,1],[1668,5,1668,13,0,0,4,1]],"Curl_mime_add_header":[[1600,6,1600,7,0,0,4,0],[1602,8,1602,11,0,0,4,0],[1608,10,1608,13,0,0,4,0]],"curl_multi_notify_enable":[[4060,6,4060,24,0,0,4,1],[83,5,83,8,1,0,4,0],[83,12,83,43,1,0,4,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_formget","curl_multi_add_handle","curl_multi_notify_enable","curl_multi_socket_action","curl_easy_upkeep","curl_ws_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_formget","curl_multi_add_handle","curl_multi_notify_enable","curl_multi_socket_action","curl_easy_upkeep","curl_ws_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


// Callback function for curl_formget
static size_t  id_000026_formget_callback(void *ptr, const char *data, size_t size) {
    // Just write the data to a file for demonstration
    FILE *out_file = fopen("output_file", "ab");
    if (out_file) {
        fwrite(data, 1, size, out_file);
        assert_file_closed(&out_file);;
    }
    return size;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    
    // Create form using the input data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Use first part of input data as form field name
    size_t name_len = size > 100 ? 100 : size / 2;
    char *field_name = (char *)malloc(name_len + 1);
    memcpy(field_name, data, name_len);
    field_name[name_len] = '\0';
    
    // Use second part as form field value
    size_t value_len = size - name_len;
    const uint8_t *value_data = data + name_len;
    
    // Create form data using curl_formadd (even though not in required list, needed for curl_formget)
    // We'll use a simple approach to create form data
    struct curl_httppost *form = NULL;
    struct curl_httppost *last = NULL;
    
    // Note: This is simplified - in reality, we'd need to build proper form structure
    // For fuzzing purposes, we create a minimal form structure
    form = (struct curl_httppost *)malloc(sizeof(struct curl_httppost));
    memset(form, 0, sizeof(struct curl_httppost));
    form->name = field_name;
    form->contents = (char *)malloc(value_len + 1);
    memcpy(form->contents, value_data, value_len);
    form->contents[value_len] = '\0';
    
    // Use curl_formget - API #1
    int formget_result = curl_formget(form, NULL,  id_000026_formget_callback);
    
    // Clean up form
    free(form->contents);
    free(form);
    free(field_name);
    
    // Set up a dummy socket for testing
    curl_socket_t dummy_socket = fuzz_fileno(stdin); // Using stdin as dummy socket
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, curl);
    
    // Enable notifications on multi handle - API #4
    // Using CURL_POLL_IN notification
    curl_multi_notify_enable(multi_handle, CURL_POLL_IN);
    
    int running_handles = 0;
    
    // Use curl_multi_socket - API #3
    curl_multi_socket(multi_handle, dummy_socket, &running_handles);
    
    // Call curl_easy_upkeep - API #2
    // Note: This should be called on a connection that has been idle
    curl_easy_upkeep(curl);
    
    // Prepare buffer for WebSocket receive
    char ws_buffer[1024];
    size_t recv_len = 0;
    const struct curl_ws_frame *meta = NULL;
    
    // Use curl_ws_recv - API #5
    // Note: This requires a WebSocket connection, which we don't have
    // We'll call it but it will fail gracefully
    CURLcode ws_result = curl_ws_recv(curl, ws_buffer, sizeof(ws_buffer), &recv_len, &meta);
    
    // Clean up
    curl_multi_remove_handle(multi_handle, curl);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    assert_fd_closed(dummy_socket);
	return 0;
}