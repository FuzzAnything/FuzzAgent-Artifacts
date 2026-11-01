// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
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

static std::string prepare_xml_data(const uint8_t* data, size_t size) {
    std::string result;
    result.reserve(size + 100);
    
    result += "<root>";
    
    for (size_t i = 0; i < size; ++i) {
        uint8_t c = data[i];
        if (c == '<' || c == '>' || c == '&' || c == '"' || c == '\'') {
            result += " ";
        } else if (c >= 32 && c < 127) {
            result += static_cast<char>(c);
        } else {
            result += " ";
        }
    }
    
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string xml_str = prepare_xml_data(Data, Size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    
    if (!result) {
        // Parsing failed, but we continue to test API robustness
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc; 
    }

    // Use pugi::char_t which is defined in pugixml.hpp (usually char or wchar_t)
    // We use a string literal that matches the underlying type.
    // Since we don't know if PUGIXML_WCHAR_MODE is set, we use the type alias.
    const pugi::char_t* name_ptr = PUGIXML_TEXT("child");

    // 1. pugi::xml_node::child
    pugi::xml_node n1 = root.child(name_ptr);
    // 2. pugi::xml_node::child
    pugi::xml_node n2 = n1.child(name_ptr);
    // 3. pugi::xml_node::child
    pugi::xml_node n3 = n2.child(name_ptr);
    // 4. pugi::xml_node::child
    pugi::xml_node n4 = n3.child(name_ptr);
    // 5. pugi::xml_node::child
    pugi::xml_node n5 = n4.child(name_ptr);
    // 6. pugi::xml_node::child
    pugi::xml_node n6 = n5.child(name_ptr);
    // 7. pugi::xml_node::child
    pugi::xml_node n7 = n6.child(name_ptr);
    // 8. pugi::xml_node::child
    pugi::xml_node n8 = n7.child(name_ptr);

    // 9. pugi::xml_node::children
    pugi::xml_object_range<pugi::xml_named_node_iterator> r1 = root.children(name_ptr);
    // 10. pugi::xml_node::children
    pugi::xml_object_range<pugi::xml_named_node_iterator> r2 = n1.children(name_ptr);
    // 11. pugi::xml_node::children
    pugi::xml_object_range<pugi::xml_named_node_iterator> r3 = n2.children(name_ptr);
    // 12. pugi::xml_node::children
    pugi::xml_object_range<pugi::xml_named_node_iterator> r4 = n3.children(name_ptr);

    // 13. pugi::xml_object_range::end
    auto it1_end = r1.end();
    // 14. pugi::xml_node::first_child
    pugi::xml_node fc1 = root.first_child();
    // 15. pugi::xml_object_range::end
    auto it2_end = r2.end();
    // 16. pugi::xml_object_range::end
    auto it3_end = r3.end();
    // 17. pugi::xml_object_range::end
    auto it4_end = r4.end();

    // 18. pugi::xml_node::first_child
    pugi::xml_node fc2 = n1.first_child();
    // 19. pugi::xml_node::last_child
    pugi::xml_node lc1 = root.last_child();
    // 20. pugi::xml_object_range::end
    auto it5_end = r1.end();
    // 21. pugi::xml_object_range::end
    auto it6_end = r2.end();
    // 22. pugi::xml_object_range::end
    auto it7_end = r3.end();
    // 23. pugi::xml_object_range::end
    auto it8_end = r4.end();
    // 24. pugi::xml_object_range::end
    auto it9_end = r1.end();
    // 25. pugi::xml_object_range::end
    auto it10_end = r2.end();

    // 26. pugi::xml_node::first_child
    pugi::xml_node fc3 = n2.first_child();
    // 27. pugi::xml_object_range::end
    auto it11_end = r3.end();
    // 28. pugi::xml_object_range::end
    auto it12_end = r4.end();
    // 29. pugi::xml_object_range::begin
    auto it1_begin = r1.begin();
    // 30. pugi::xml_object_range::end
    auto it13_end = r1.end();
    // 31. pugi::xml_object_range::end
    auto it14_end = r2.end();
    // 32. pugi::xml_object_range::begin
    auto it2_begin = r2.begin();

    (void)it1_end;
    (void)it2_end;
    (void)it3_end;
    (void)it4_end;
    (void)fc1;
    (void)fc2;
    (void)lc1;
    (void)it5_end;
    (void)it6_end;
    (void)it7_end;
    (void)it8_end;
    (void)it9_end;
    (void)it10_end;
    (void)fc3;
    (void)it11_end;
    (void)it12_end;
    (void)it1_begin;
    (void)it13_end;
    (void)it14_end;
    (void)it2_begin;
    (void)n1;
    (void)n2;
    (void)n3;
    (void)n4;
    (void)n5;
    (void)n6;
    (void)n7;
    (void)n8;
    (void)r1;
    (void)r2;
    (void)r3;
    (void)r4;

    return 0;
}