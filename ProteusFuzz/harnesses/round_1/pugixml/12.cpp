// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <string>
#include <string_view>

static pugi::xml_document* create_document_from_fuzz_data(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    if (size == 0) {
        doc->append_child("root");
        return doc;
    }

    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc->load_string(xml_str.c_str());
    
    if (!result) {
        delete doc;
        doc = new pugi::xml_document();
        doc->append_child("root");
    }

    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    pugi::xml_document* doc = create_document_from_fuzz_data(Data, Size);
    if (!doc) return 0;

    pugi::xml_node root = doc->first_child();
    if (!root) {
        root = doc->append_child("root");
    }

    // Helper to safely extract a null-terminated string from fuzz data
    // Returns a static buffer if extraction fails or data is invalid
    static char safe_buf[256];
    auto get_safe_string = [&](size_t offset, size_t max_len) -> const char* {
        if (offset >= Size) return "default";
        size_t len = 0;
        while (offset + len < Size && len < max_len && Data[offset + len] != 0) {
            len++;
        }
        if (len == 0) return "default";
        
        // Copy to safe buffer to ensure null termination and bounds
        size_t copy_len = (len < 255) ? len : 255;
        std::memcpy(safe_buf, Data + offset, copy_len);
        safe_buf[copy_len] = '\0';
        return safe_buf;
    };

    // Helper to safely extract a string_view from fuzz data
    auto get_safe_string_view = [&](size_t offset, size_t max_len) -> std::string_view {
        if (offset >= Size) return std::string_view("default");
        size_t len = 0;
        while (offset + len < Size && len < max_len && Data[offset + len] != 0) {
            len++;
        }
        if (len == 0) return std::string_view("default");
        return std::string_view(reinterpret_cast<const char*>(Data + offset), len);
    };

    // Helper to get a number from fuzz data
    auto get_int = [&](size_t offset) -> int {
        if (offset >= Size) return 42;
        return static_cast<int>(Data[offset]) % 1000;
    };

    // Helper to get a float from fuzz data
    auto get_float = [&](size_t offset) -> float {
        if (offset >= Size) return 3.14f;
        return static_cast<float>(Data[offset]) / 10.0f;
    };

    // Helper to get a double from fuzz data
    auto get_double = [&](size_t offset) -> double {
        if (offset >= Size) return 2.71828;
        return static_cast<double>(Data[offset]) / 10.0;
    };

    // Helper to get a bool from fuzz data
    auto get_bool = [&](size_t offset) -> bool {
        if (offset >= Size) return true;
        return Data[offset] % 2 == 1;
    };

    // Sequence of operations as specified
    // 1. append_attribute + set_value (5 overloads)
    {
        pugi::xml_attribute attr1 = root.append_attribute(get_safe_string(0, 20));
        if (attr1) {
            attr1.set_value(get_safe_string(10, 20));
        }
    }
    
    {
        pugi::xml_attribute attr2 = root.append_attribute(get_safe_string_view(30, 20));
        if (attr2) {
            attr2.set_value(get_int(50));
        }
    }
    
    {
        pugi::xml_attribute attr3 = root.append_attribute(get_safe_string(70, 20));
        if (attr3) {
            attr3.set_value(get_float(90));
        }
    }
    
    {
        pugi::xml_attribute attr4 = root.append_attribute(get_safe_string_view(110, 20));
        if (attr4) {
            attr4.set_value(get_double(130));
        }
    }
    
    {
        pugi::xml_attribute attr5 = root.append_attribute(get_safe_string(150, 20));
        if (attr5) {
            attr5.set_value(get_bool(170));
        }
    }

    // 2. append_attribute + set_value (5 overloads)
    {
        pugi::xml_attribute attr6 = root.append_attribute(get_safe_string(190, 20));
        if (attr6) {
            attr6.set_value(get_safe_string(210, 20));
        }
    }
    
    {
        pugi::xml_attribute attr7 = root.append_attribute(get_safe_string_view(230, 20));
        if (attr7) {
            attr7.set_value(get_int(250));
        }
    }
    
    {
        pugi::xml_attribute attr8 = root.append_attribute(get_safe_string(270, 20));
        if (attr8) {
            attr8.set_value(get_float(290));
        }
    }
    
    {
        pugi::xml_attribute attr9 = root.append_attribute(get_safe_string_view(310, 20));
        if (attr9) {
            attr9.set_value(get_double(330));
        }
    }
    
    {
        pugi::xml_attribute attr10 = root.append_attribute(get_safe_string(350, 20));
        if (attr10) {
            attr10.set_value(get_bool(370));
        }
    }

    // 3. append_attribute + set_value (5 overloads)
    {
        pugi::xml_attribute attr11 = root.append_attribute(get_safe_string(390, 20));
        if (attr11) {
            attr11.set_value(get_safe_string(410, 20));
        }
    }
    
    {
        pugi::xml_attribute attr12 = root.append_attribute(get_safe_string_view(430, 20));
        if (attr12) {
            attr12.set_value(get_int(450));
        }
    }
    
    {
        pugi::xml_attribute attr13 = root.append_attribute(get_safe_string(470, 20));
        if (attr13) {
            attr13.set_value(get_float(490));
        }
    }
    
    {
        pugi::xml_attribute attr14 = root.append_attribute(get_safe_string_view(510, 20));
        if (attr14) {
            attr14.set_value(get_double(530));
        }
    }
    
    {
        pugi::xml_attribute attr15 = root.append_attribute(get_safe_string(550, 20));
        if (attr15) {
            attr15.set_value(get_bool(570));
        }
    }

    // 4. append_attribute + set_value (5 overloads)
    {
        pugi::xml_attribute attr16 = root.append_attribute(get_safe_string(590, 20));
        if (attr16) {
            attr16.set_value(get_safe_string(610, 20));
        }
    }
    
    {
        pugi::xml_attribute attr17 = root.append_attribute(get_safe_string_view(630, 20));
        if (attr17) {
            attr17.set_value(get_int(650));
        }
    }
    
    {
        pugi::xml_attribute attr18 = root.append_attribute(get_safe_string(670, 20));
        if (attr18) {
            attr18.set_value(get_float(690));
        }
    }
    
    {
        pugi::xml_attribute attr19 = root.append_attribute(get_safe_string_view(710, 20));
        if (attr19) {
            attr19.set_value(get_double(730));
        }
    }
    
    {
        pugi::xml_attribute attr20 = root.append_attribute(get_safe_string(750, 20));
        if (attr20) {
            attr20.set_value(get_bool(770));
        }
    }

    // 5. append_attribute + set_value (5 overloads)
    {
        pugi::xml_attribute attr21 = root.append_attribute(get_safe_string(790, 20));
        if (attr21) {
            attr21.set_value(get_safe_string(810, 20));
        }
    }
    
    {
        pugi::xml_attribute attr22 = root.append_attribute(get_safe_string_view(830, 20));
        if (attr22) {
            attr22.set_value(get_int(850));
        }
    }
    
    {
        pugi::xml_attribute attr23 = root.append_attribute(get_safe_string(870, 20));
        if (attr23) {
            attr23.set_value(get_float(890));
        }
    }
    
    {
        pugi::xml_attribute attr24 = root.append_attribute(get_safe_string_view(910, 20));
        if (attr24) {
            attr24.set_value(get_double(930));
        }
    }
    
    {
        pugi::xml_attribute attr25 = root.append_attribute(get_safe_string(950, 20));
        if (attr25) {
            attr25.set_value(get_bool(970));
        }
    }

    // 6. append_attribute + set_value (5 overloads)
    {
        pugi::xml_attribute attr26 = root.append_attribute(get_safe_string(990, 20));
        if (attr26) {
            attr26.set_value(get_safe_string(1010, 20));
        }
    }
    
    {
        pugi::xml_attribute attr27 = root.append_attribute(get_safe_string_view(1030, 20));
        if (attr27) {
            attr27.set_value(get_int(1050));
        }
    }
    
    {
        pugi::xml_attribute attr28 = root.append_attribute(get_safe_string(1070, 20));
        if (attr28) {
            attr28.set_value(get_float(1090));
        }
    }
    
    {
        pugi::xml_attribute attr29 = root.append_attribute(get_safe_string_view(1110, 20));
        if (attr29) {
            attr29.set_value(get_double(1130));
        }
    }
    
    {
        pugi::xml_attribute attr30 = root.append_attribute(get_safe_string(1150, 20));
        if (attr30) {
            attr30.set_value(get_bool(1170));
        }
    }

    delete doc;
    return 0;
}