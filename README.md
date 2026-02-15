# KSeF FA(3)

C++23 library for generating Polish e-invoice XML conforming to the KSeF FA(3) schema (version 1-0E).

## Build

Requires Qt 6.10+ (Core) and CMake 3.19+.

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build
```

## FetchContent

```cmake
FetchContent_Declare(
  ksef_fa3
  GIT_REPOSITORY <repo-url>
  GIT_TAG        master
)
FetchContent_MakeAvailable(ksef_fa3)

target_link_libraries(my_app PRIVATE ksef_fa3)
```

## Example

```cpp
#include "schemat.hpp"
using namespace ksef::FA3;

// Header
Naglowek naglowek{QDateTime::currentDateTimeUtc(), QStringLiteral("MySystem")};

// Seller
Podmiot1 podmiot1{
    DaneIdentyfikacyjne{QStringLiteral("8882927357"), QStringLiteral("ABC sp. z o.o.")},
    Adres{QStringLiteral("PL"), QStringLiteral("ul. Kwiatowa 1"), QStringLiteral("00-001 Warszawa")}};

// Buyer
Podmiot2 podmiot2{
    DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("Jan Kowalski")},
    false, false};

// Invoice body
Fa fa{QStringLiteral("PLN"), QDate{2026, 2, 15}, QStringLiteral("FV/001/2026"),
       QStringLiteral("1230.00"), Adnotacje{}, RodzajFaktury::Rodzaj::VAT};
fa.setVatPodst(QStringLiteral("1000.00"), QStringLiteral("230.00"));

FaWiersz w{1, false};
w.setNazwaTowaru(QStringLiteral("Widget"));
w.setMiara(QStringLiteral("szt."));
w.setIlosc(QStringLiteral("10"));
w.setCenaJednostkowa(QStringLiteral("100.00"), true);
w.setWartoscNetto(QStringLiteral("1000.00"));
w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
fa.addFaWiersz(std::move(w));

// Assemble and serialize
Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
auto xml = toXML(faktura); // std::expected<QString, QString>
```

## TODO

- [ ] CI/CD pipeline
- [ ] Input validation (NIP checksum, ISO 4217 currency codes, decimal format)
- [ ] Missing setters: `FaWiersz` (GTIN, PKOB, P_12_XII, P_12_Zal_15), `ZamowienieWiersz` (7 fields)
- [ ] Min-cardinality enforcement for `Korekta::DaneFaKorygowanej` and `NoweSrodkiTransportu::P_22`
- [ ] Constrain `P_12(auto stawka)` / `P_12Z(auto stawka)` parameter types
