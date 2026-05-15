#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <ctime>

namespace harness_011 {

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum size for date parsing operations
    if (size < 8) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to determine which date format to test
    uint8_t format_type = fdp.ConsumeIntegral<uint8_t>() % 20;
    
    // Consume various date components from fuzzer input
    int day = fdp.ConsumeIntegralInRange<int>(1, 31);
    int month_num = fdp.ConsumeIntegralInRange<int>(1, 12);
    int year_full = fdp.ConsumeIntegralInRange<int>(1970, 2100);
    int year_short = fdp.ConsumeIntegralInRange<int>(0, 99);
    int hour = fdp.ConsumeIntegralInRange<int>(0, 23);
    int minute = fdp.ConsumeIntegralInRange<int>(0, 59);
    int second = fdp.ConsumeIntegralInRange<int>(0, 60); // Allow 60 for leap seconds
    
    // Consume weekday and month names (truncated to appropriate lengths)
    std::string weekday_short = fdp.ConsumeRandomLengthString(3);
    std::string weekday_full = fdp.ConsumeRandomLengthString(9);
    std::string month_short = fdp.ConsumeRandomLengthString(3);
    
    // Consume timezone information
    uint8_t tz_type = fdp.ConsumeIntegral<uint8_t>() % 10;
    std::string tz_name = fdp.ConsumeRandomLengthString(4);
    int tz_offset_hours = fdp.ConsumeIntegralInRange<int>(-12, 12);
    int tz_offset_minutes = fdp.ConsumeIntegralInRange<int>(0, 59);
    
    // Consume separators and other formatting characters
    std::string separator1 = fdp.ConsumeRandomLengthString(2);
    std::string separator2 = fdp.ConsumeRandomLengthString(2);
    std::string separator3 = fdp.ConsumeRandomLengthString(2);
    
    // Map month numbers to month names
    const char* month_names[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    
    const char* weekday_names[] = {
        "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
    };
    
    const char* full_weekday_names[] = {
        "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
    };
    
    // Use actual month names from the consumed data or fall back to standard names
    const char* month_str = month_names[month_num - 1];
    const char* weekday_short_str = weekday_names[format_type % 7];
    const char* weekday_full_str = full_weekday_names[format_type % 7];
    
    // Build date string based on format type
    std::string date_str;
    char buffer[256];
    
    switch (format_type) {
        case 0:
            // RFC 822/1123 format: Sun, 06 Nov 1994 08:49:37 GMT
            snprintf(buffer, sizeof(buffer), "%s, %02d %s %04d %02d:%02d:%02d GMT",
                    weekday_short_str, day, month_str, year_full, hour, minute, second);
            date_str = buffer;
            break;
            
        case 1:
            // RFC 850/1036 format: Sunday, 06-Nov-94 08:49:37 GMT
            snprintf(buffer, sizeof(buffer), "%s, %02d-%s-%02d %02d:%02d:%02d GMT",
                    weekday_full_str, day, month_str, year_short, hour, minute, second);
            date_str = buffer;
            break;
            
        case 2:
            // ANSI C asctime() format: Sun Nov  6 08:49:37 1994
            snprintf(buffer, sizeof(buffer), "%s %s %2d %02d:%02d:%02d %04d",
                    weekday_short_str, month_str, day, hour, minute, second, year_full);
            date_str = buffer;
            break;
            
        case 3:
            // Without weekday: 06 Nov 1994 08:49:37 GMT
            snprintf(buffer, sizeof(buffer), "%02d %s %04d %02d:%02d:%02d GMT",
                    day, month_str, year_full, hour, minute, second);
            date_str = buffer;
            break;
            
        case 4:
            // Without weekday, RFC 850 style: 06-Nov-94 08:49:37 GMT
            snprintf(buffer, sizeof(buffer), "%02d-%s-%02d %02d:%02d:%02d GMT",
                    day, month_str, year_short, hour, minute, second);
            date_str = buffer;
            break;
            
        case 5:
            // Without timezone: 06 Nov 1994 08:49:37
            snprintf(buffer, sizeof(buffer), "%02d %s %04d %02d:%02d:%02d",
                    day, month_str, year_full, hour, minute, second);
            date_str = buffer;
            break;
            
        case 6:
            // Without timezone, RFC 850 style: 06-Nov-94 08:49:37
            snprintf(buffer, sizeof(buffer), "%02d-%s-%02d %02d:%02d:%02d",
                    day, month_str, year_short, hour, minute, second);
            date_str = buffer;
            break;
            
        case 7:
            // Weird order: 1994 Nov 6 08:49:37
            snprintf(buffer, sizeof(buffer), "%04d %s %d %02d:%02d:%02d",
                    year_full, month_str, day, hour, minute, second);
            date_str = buffer;
            break;
            
        case 8:
            // GMT first: GMT 08:49:37 06-Nov-94 Sunday
            snprintf(buffer, sizeof(buffer), "GMT %02d:%02d:%02d %02d-%s-%02d %s",
                    hour, minute, second, day, month_str, year_short, weekday_full_str);
            date_str = buffer;
            break;
            
        case 9:
            // Short year first: 94 6 Nov 08:49:37
            snprintf(buffer, sizeof(buffer), "%02d %d %s %02d:%02d:%02d",
                    year_short, day, month_str, hour, minute, second);
            date_str = buffer;
            break;
            
        case 10:
            // Date only: 1994 Nov 6
            snprintf(buffer, sizeof(buffer), "%04d %s %d", year_full, month_str, day);
            date_str = buffer;
            break;
            
        case 11:
            // Date only, RFC 850 style: 06-Nov-94
            snprintf(buffer, sizeof(buffer), "%02d-%s-%02d", day, month_str, year_short);
            date_str = buffer;
            break;
            
        case 12:
            // Date only with weekday: Sun Nov 6 94
            snprintf(buffer, sizeof(buffer), "%s %s %d %02d",
                    weekday_short_str, month_str, day, year_short);
            date_str = buffer;
            break;
            
        case 13:
            // Unusual separators: 1994.Nov.6
            snprintf(buffer, sizeof(buffer), "%04d.%s.%d", year_full, month_str, day);
            date_str = buffer;
            break;
            
        case 14:
            // Unusual separators with weekday: Sun/Nov/6/94/GMT
            snprintf(buffer, sizeof(buffer), "%s/%s/%d/%02d/GMT",
                    weekday_short_str, month_str, day, year_short);
            date_str = buffer;
            break;
            
        case 15:
            // With timezone name: Sun, 06 Nov 1994 08:49:37 CET
            snprintf(buffer, sizeof(buffer), "%s, %02d %s %04d %02d:%02d:%02d %s",
                    weekday_short_str, day, month_str, year_full, hour, minute, second,
                    tz_name.c_str());
            date_str = buffer;
            break;
            
        case 16:
            // RFC822 style timezone: Sun, 12 Sep 2004 15:05:58 -0700
            {
                char tz_sign = (tz_offset_hours >= 0) ? '+' : '-';
                int abs_hours = abs(tz_offset_hours);
                snprintf(buffer, sizeof(buffer), "%s, %02d %s %04d %02d:%02d:%02d %c%02d%02d",
                        weekday_short_str, day, month_str, year_full, hour, minute, second,
                        tz_sign, abs_hours, tz_offset_minutes);
                date_str = buffer;
            }
            break;
            
        case 17:
            // Compact numerical: 20040912 15:05:58 -0700
            {
                char tz_sign = (tz_offset_hours >= 0) ? '+' : '-';
                int abs_hours = abs(tz_offset_hours);
                snprintf(buffer, sizeof(buffer), "%04d%02d%02d %02d:%02d:%02d %c%02d%02d",
                        year_full, month_num, day, hour, minute, second,
                        tz_sign, abs_hours, tz_offset_minutes);
                date_str = buffer;
            }
            break;
            
        case 18:
            // Compact numerical date only: 20040911 +0200
            {
                char tz_sign = (tz_offset_hours >= 0) ? '+' : '-';
                int abs_hours = abs(tz_offset_hours);
                snprintf(buffer, sizeof(buffer), "%04d%02d%02d %c%02d%02d",
                        year_full, month_num, day, tz_sign, abs_hours, tz_offset_minutes);
                date_str = buffer;
            }
            break;
            
        case 19:
            // Random string from remaining bytes (to test error handling)
            date_str = fdp.ConsumeRemainingBytesAsString();
            break;
            
        default:
            // Fallback to simple format
            snprintf(buffer, sizeof(buffer), "%02d %s %04d", day, month_str, year_full);
            date_str = buffer;
            break;
    }
    
    // Call curl_getdate with the generated date string
    // The second parameter is unused (legacy) and should be NULL
    time_t result = curl_getdate(date_str.c_str(), NULL);
    
    // Also test Curl_getdate_capped if we can access it
    // Note: Curl_getdate_capped is in the internal header parsedate.h
    // We'll test it if we have access to it
    #ifdef HAVE_CURL_PARSEDATE_H
    {
        time_t capped_result;
        int capped_ret = Curl_getdate_capped(date_str.c_str(), &capped_result);
        // Ignore return value, just exercise the code
        (void)capped_ret;
        (void)capped_result;
    }
    #endif
    
    // Test edge cases with invalid dates
    if (fdp.remaining_bytes() > 10) {
        // Test with empty string
        curl_getdate("", NULL);
        
        // Test with NULL pointer (should crash or handle)
        // curl_getdate(NULL, NULL); // Don't test NULL as it would crash
        
        // Test with very long string
        std::string long_date = fdp.ConsumeRandomLengthString(1024);
        curl_getdate(long_date.c_str(), NULL);
        
        // Test with pure numbers
        char num_date[64];
        snprintf(num_date, sizeof(num_date), "%d", fdp.ConsumeIntegral<int>());
        curl_getdate(num_date, NULL);
        
        // Test with special characters
        std::string special_chars = "!@#$%^&*()_+";
        curl_getdate(special_chars.c_str(), NULL);
    }
    
    return 0;
}

} // namespace harness_011
