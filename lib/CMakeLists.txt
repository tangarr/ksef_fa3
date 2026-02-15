add_library(ksef_fa3 SHARED
    internal/xml_core.hpp
    internal/xml_core.cpp
    internal/xml_helpers.hpp
    typy_adresowe.hpp
    typy_adresowe.cpp
    typy_identyfikacyjne.hpp
    typy_identyfikacyjne.cpp
    podmioty.hpp
    podmioty.cpp
    adnotacje.hpp
    adnotacje.cpp
    fa.hpp
    fa.cpp
    platnosc.hpp
    platnosc.cpp
    transport.hpp
    transport.cpp
    stopka.hpp
    stopka.cpp
    zalacznik.hpp
    zalacznik.cpp
    faktura.hpp
    faktura.cpp
    schemat.hpp
)
target_compile_features(ksef_fa3 PUBLIC cxx_std_23)
target_compile_options(ksef_fa3 PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wnrvo -Werror=switch)
target_link_libraries(ksef_fa3 PUBLIC Qt6::Core)
target_include_directories(ksef_fa3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(ksef_fa3 PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

if(COVERAGE)
    target_compile_options(ksef_fa3 PRIVATE --coverage)
    target_link_options(ksef_fa3 PRIVATE --coverage)
endif()
