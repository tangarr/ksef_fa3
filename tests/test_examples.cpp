#include "helpers.hpp"
#include <QTimeZone>

using namespace ksef::FA3;

namespace
{

    const QString EXAMPLES_DIR = QStringLiteral(SRCDIR "/examples/");

    Stopka makeStopkaDefault()
    {
        Stopka stopka;
        stopka.addInformacje(Informacje{QStringLiteral("Kapiał zakładowy 5 000 000")});
        Rejestry rej;
        rej.setKRS(QStringLiteral("0000099999"));
        rej.setREGON(QStringLiteral("999999999"));
        rej.setBDO(QStringLiteral("000099999"));
        stopka.addRejestry(std::move(rej));
        return stopka;
    }

    Podmiot1 makePodmiot1_ABC()
    {
        Podmiot1 p1{
            DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("ABC AGD sp. z o. o.")},
            Adres{QStringLiteral("PL"), QStringLiteral("ul. Kwiatowa 1 m. 2"), QStringLiteral("00-001 Warszawa")}};
        p1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("abc@abc.pl"), QStringLiteral("667444555")});
        return p1;
    }

    Podmiot2 makePodmiot2_JanKowalski(QString nrKlienta = QStringLiteral("fdfd778343"))
    {
        Podmiot2 p2{
            DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("F.H.U. Jan Kowalski")}};
        p2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
        p2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});
        if (!nrKlienta.isEmpty())
            p2.setNrKlienta(std::move(nrKlienta));
        return p2;
    }

}

TEST(Examples, Przyklad_1)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 1}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("SamploFaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("8882927357"), QStringLiteral("ABC AGD sp. z o. o.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Kwiatowa 1 m. 2"), QStringLiteral("00-001 Warszawa")}};
    podmiot1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("abc@abc.pl"), QStringLiteral("667444555")});

    auto podmiot2 = makePodmiot2_JanKowalski();

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 2, 15},
        QStringLiteral("FV2026/02/150"),
        QStringLiteral("2051"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::VAT};
    fa.setDataDostawy(QDate{2026, 1, 27});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("1666.66"), QStringLiteral("383.34"));
    fa.setVatObniz2(QStringLiteral("0.95"), QStringLiteral("0.05"));
    fa.setFP();
    fa.addDodatkowyOpis(DodatkowyOpis{std::nullopt, QStringLiteral("preferowane godziny dowozu"), QStringLiteral("dni robocze 17:00 - 20:00")});

    {
        FaWiersz w{1, false};
        w.setUU_ID(QStringLiteral("aaaa111133339990"));
        w.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("1626.01"), true);
        w.setWartoscNetto(QStringLiteral("1626.01"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{2, false};
        w.setUU_ID(QStringLiteral("aaaa111133339991"));
        w.setNazwaTowaru(QStringLiteral("wniesienie sprzętu"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("40.65"), true);
        w.setWartoscNetto(QStringLiteral("40.65"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{3, false};
        w.setUU_ID(QStringLiteral("aaaa111133339992"));
        w.setNazwaTowaru(QStringLiteral("promocja lodówka pełna mleka"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("0.95"), true);
        w.setWartoscNetto(QStringLiteral("0.95"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(5)});
        fa.addFaWiersz(std::move(w));
    }

    Platnosc platnosc;
    platnosc.setZaplacono(QDate{2026, 1, 27});
    platnosc.setFormaPlatnosci(FormaPlatnosci::Forma::PRZELEW);
    fa.setPlatnosc(std::move(platnosc));

    WarunkiTransakcji wt;
    wt.addZamowienie(ZamowieniaWT{QDate{2026, 1, 26}, QStringLiteral("4354343")});
    fa.setWarunkiTransakcji(std::move(wt));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_1.xml"), *xml));
}

TEST(Examples, Przyklad_2)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 1}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("SamploFaktur")};

    auto podmiot1 = makePodmiot1_ABC();
    auto podmiot2 = makePodmiot2_JanKowalski();

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 3, 15},
        QStringLiteral("FK2026/03/200"),
        QStringLiteral("-200"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::KOR};
    fa.setDataDostawy(QDate{2026, 1, 27});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("-162.60"), QStringLiteral("-37.40"));

    Korekta korekta;
    korekta.setPrzyczynaKorekty(QStringLiteral("obniżka ceny o 200 zł z uwagi na uszkodzenia estetyczne"));
    korekta.setTypKorekty(TypKorekty::W_DACIE_INNEJ);
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{
        QDate{2026, 2, 15}, QStringLiteral("FV2026/02/150"),
        QStringLiteral("9999999999-20230908-8BEF280C8D35-4D")});
    fa.setKorekta(std::move(korekta));

    {
        FaWiersz w{1, true};
        w.setUU_ID(QStringLiteral("aaaa111133339990"));
        w.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("1626.01"), true);
        w.setWartoscNetto(QStringLiteral("1626.01"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{1, false};
        w.setUU_ID(QStringLiteral("aaaa111133339990"));
        w.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("1463.41"), true);
        w.setWartoscNetto(QStringLiteral("1463.41"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_2.xml"), *xml));
}

TEST(Examples, Przyklad_3)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 1}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("SamploFaktur")};

    auto podmiot1 = makePodmiot1_ABC();
    auto podmiot2 = makePodmiot2_JanKowalski();

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 3, 15},
        QStringLiteral("FK2026/03/200"),
        QStringLiteral("-200"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::KOR};
    fa.setDataDostawy(QDate{2026, 1, 27});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("-162.60"), QStringLiteral("-37.40"));

    Korekta korekta;
    korekta.setPrzyczynaKorekty(QStringLiteral("obniżka ceny o 200 zł z uwagi na uszkodzenia estetyczne"));
    korekta.setTypKorekty(TypKorekty::W_DACIE_INNEJ);
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{
        QDate{2026, 2, 15}, QStringLiteral("FV2026/02/150"),
        QStringLiteral("9999999999-20230908-8BEF280C8D35-4D")});
    fa.setKorekta(std::move(korekta));

    {
        FaWiersz w{1, false};
        w.setUU_ID(QStringLiteral("aaaa111133339990"));
        w.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("-162.60"), true);
        w.setWartoscNetto(QStringLiteral("-162.60"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_3.xml"), *xml));
}

TEST(Examples, Przyklad_4)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 1}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("SamploFaktur")};

    auto podmiot1 = makePodmiot1_ABC();

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("CDE sp. j.")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 lok. 3"), QStringLiteral("00-002 Kraków")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("cde@cde.pl"), QStringLiteral("555777999")});
    podmiot2.setNrKlienta(QStringLiteral("fdfd778343"));

    Podmiot3 podmiot3{
        DaneIdentyfikacyjne3{NabywcaPolski{QStringLiteral("2222222222")}, QStringLiteral("Bank Bankowości Bankowej S. A. BBB Faktoring")},
        Rola::FAKTOR};
    podmiot3.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Bankowa 1"), QStringLiteral("00-003 Łódź")});
    podmiot3.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("bbb@efaktoring.pl"), QStringLiteral("666888999")});

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 2, 15},
        QStringLiteral("FV2026/02/150"),
        QStringLiteral("64279.92"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::VAT};
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.addWZ(QStringLiteral("44343434/2026"));
    fa.setVatPodst(QStringLiteral("52260.10"), QStringLiteral("12019.82"));

    {
        FaWiersz w{1, false};
        w.setUU_ID(QStringLiteral("aaaa111133339990"));
        w.setP_6A(QDate{2026, 1, 3});
        w.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        w.setCN(QStringLiteral("8418 21 91"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("10"));
        w.setCenaJednostkowa(QStringLiteral("1626.01"), true);
        w.setWartoscNetto(QStringLiteral("16260.10"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{2, false};
        w.setUU_ID(QStringLiteral("aaaa111133339991"));
        w.setP_6A(QDate{2026, 1, 10});
        w.setNazwaTowaru(QStringLiteral("zamrażarka Zimnotech mk2"));
        w.setCN(QStringLiteral("8418 40 20"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("20"));
        w.setCenaJednostkowa(QStringLiteral("1000"), true);
        w.setRabat(QStringLiteral("100"));
        w.setWartoscNetto(QStringLiteral("18000"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{3, false};
        w.setUU_ID(QStringLiteral("aaaa111133339992"));
        w.setP_6A(QDate{2026, 1, 15});
        w.setNazwaTowaru(QStringLiteral("zmywarka Bryza 100"));
        w.setCN(QStringLiteral("8422 11 00"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("15"));
        w.setCenaJednostkowa(QStringLiteral("1200"), true);
        w.setWartoscNetto(QStringLiteral("18000"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }

    internal::XmlVector<Odliczenia, 100> odlVec;
    odlVec.push_back(Odliczenia{QStringLiteral("1000"), QStringLiteral("nadwyżka salda nierozliczonych środków")});
    Rozliczenie rozliczenie{
        std::nullopt,
        std::make_tuple(std::move(odlVec), QStringLiteral("1000")),
        true,
        QStringLiteral("63279.92")};
    fa.setRozliczenie(std::move(rozliczenie));

    Platnosc platnosc;
    platnosc.addTerminPlatnosci(QDate{2026, 3, 15}, std::nullopt);
    platnosc.setFormaPlatnosci(FormaPlatnosci::Forma::PRZELEW);
    RachunekBankowyFaktora rbf{QStringLiteral("73111111111111111111111111"), QStringLiteral("Bank Bankowości Bankowej S. A."), QStringLiteral("PLN")};
    rbf.setRachunekWlasnyBanku(RachunekWlasnyBanku::Typ::TYP_2);
    platnosc.addRachunekBankowyFaktora(std::move(rbf));
    fa.setPlatnosc(std::move(platnosc));

    WarunkiTransakcji wt;
    wt.addZamowienie(ZamowieniaWT{QDate{2026, 1, 26}, QStringLiteral("4354343")});
    wt.addNrPartiiTowaru(QStringLiteral("2312323/2026"));
    wt.setWarunkiDostawy(QStringLiteral("CIP"));

    Transport transport{RodzajTransportu::DROGOWY, OpisLadunku::PALETA};
    transport.setPrzewoznik(Przewoznik{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("6666666666")}, QStringLiteral("Jan Nowak Transport")},
        AdresPrzewoznika{QStringLiteral("PL"), QStringLiteral("ul. Bukowa 5"), QStringLiteral("00-004 Poznań")}});
    transport.setJednostkaOpakowania(QStringLiteral("a"));
    transport.setWysylkaZ(WysylkaZ{QStringLiteral("PL"), QStringLiteral("Sadowa 1 lok. 2"), QStringLiteral("00-001 Warszawa")});
    transport.setWysylkaDo(WysylkaDo{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 lok. 3"), QStringLiteral("00-002 Kraków")});
    wt.addTransport(std::move(transport));
    fa.setWarunkiTransakcji(std::move(wt));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.addPodmiot3(std::move(podmiot3));
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_4.xml"), *xml));
}

TEST(Examples, Przyklad_5)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 15}, QTime{9, 30, 47}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    auto podmiot1 = makePodmiot1_ABC();

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("CeDeE s.c.")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 lok. 3"), QStringLiteral("00-002 Kraków")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("cde@cde.pl"), QStringLiteral("555777999")});
    podmiot2.setNrKlienta(QStringLiteral("fdfd778343"));
    podmiot2.setIDNabywcy(QStringLiteral("0001"));

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 4, 1},
        QStringLiteral("FK2026/04/23"),
        QStringLiteral("0"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::KOR};
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));

    Korekta korekta;
    korekta.setPrzyczynaKorekty(QStringLiteral("błędna nazwa nabywcy"));
    korekta.setTypKorekty(TypKorekty::W_DACIE_FAKTURY_PIERWOTNEJ);
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{
        QDate{2026, 2, 15}, QStringLiteral("FV2026/02/150"),
        QStringLiteral("9999999999-20230908-8BEF280C8D35-4D")});
    korekta.addPodmiot2K(Podmiot2K{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("CDE sp. j.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 lok. 3"), QStringLiteral("00-002 Kraków")},
        QStringLiteral("0001")});
    fa.setKorekta(std::move(korekta));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_5.xml"), *xml));
}

TEST(Examples, Przyklad_6)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 7, 15}, QTime{9, 30, 47}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    auto podmiot1 = makePodmiot1_ABC();

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("CeDeE s.c.")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 lok. 3"), QStringLiteral("00-002 Kraków")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("cde@cde.pl"), QStringLiteral("555777999")});
    podmiot2.setNrKlienta(QStringLiteral("fdfd778343"));

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 7, 15},
        QStringLiteral("FK2026/07/243"),
        QStringLiteral("-50000"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::KOR};
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("-40650.41"), QStringLiteral("-9349.59"));

    Korekta korekta;
    korekta.setPrzyczynaKorekty(QStringLiteral("rabat 50000 z uwagi na poziom zakupów pierwszym półroczu 2026"));
    korekta.setTypKorekty(TypKorekty::W_DACIE_FAKTURY_KORYGUJACEJ);
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{QDate{2026, 1, 15}, QStringLiteral("FV2026/01/134"), QStringLiteral("9999999999-20230908-8BEF280C8D35-4D")});
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{QDate{2026, 2, 15}, QStringLiteral("FV2026/02/150"), QStringLiteral("9999999999-20230908-76B2B580D4DC-80")});
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{QDate{2026, 3, 15}, QStringLiteral("FV2026/03/143"), QStringLiteral("9999999999-20230908-4191312C0E57-09")});
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{QDate{2026, 4, 15}, QStringLiteral("FV2026/04/23"), QStringLiteral("9999999999-20230908-2B9266CEF3C4-DD")});
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{QDate{2026, 5, 15}, QStringLiteral("FV2026/05/54"), QStringLiteral("9999999999-20230908-16B99491C78B-3D")});
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{QDate{2026, 6, 15}, QStringLiteral("FV2026/06/15"), QStringLiteral("9999999999-20230908-D08FB95950BE-3E")});
    korekta.setOkresFaKorygowanej(QStringLiteral("pierwsze półrocze 2026"));
    fa.setKorekta(std::move(korekta));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_6.xml"), *xml));
}

TEST(Examples, Przyklad_7)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 7, 15}, QTime{9, 30, 47}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    auto podmiot1 = makePodmiot1_ABC();

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("CeDeE s.c.")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 lok. 3"), QStringLiteral("00-002 Kraków")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("cde@cde.pl"), QStringLiteral("555777999")});
    podmiot2.setNrKlienta(QStringLiteral("fdfd778343"));

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 7, 15},
        QStringLiteral("FK2026/07/243"),
        QStringLiteral("-50000"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::KOR};
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("-40650.41"), QStringLiteral("-9349.59"));

    Korekta korekta;
    korekta.setPrzyczynaKorekty(QStringLiteral("rabat 50000 z uwagi na poziom zakupów pierwszym półroczu 2026"));
    korekta.setTypKorekty(TypKorekty::W_DACIE_FAKTURY_KORYGUJACEJ);
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{QDate{2026, 1, 15}, QStringLiteral("FV2026/01/134"), QStringLiteral("9999999999-20230908-8BEF280C8D35-4D")});
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{QDate{2026, 2, 15}, QStringLiteral("FV2026/02/150"), QStringLiteral("9999999999-20230908-76B2B580D4DC-80")});
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{QDate{2026, 3, 15}, QStringLiteral("FV2026/03/143"), QStringLiteral("9999999999-20230908-4191312C0E57-09")});
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{QDate{2026, 4, 15}, QStringLiteral("FV2026/04/23"), QStringLiteral("9999999999-20230908-2B9266CEF3C4-DD")});
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{QDate{2026, 5, 15}, QStringLiteral("FV2026/05/54"), QStringLiteral("9999999999-20230908-16B99491C78B-3D")});
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{QDate{2026, 6, 15}, QStringLiteral("FV2026/06/15"), QStringLiteral("9999999999-20230908-D08FB95950BE-3E")});
    korekta.setOkresFaKorygowanej(QStringLiteral("pierwsze półrocze 2026"));
    fa.setKorekta(std::move(korekta));

    {
        FaWiersz w{1, false};
        w.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        w.setCN(QStringLiteral("8418 21 91"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1000"));
        fa.addFaWiersz(std::move(w));
    }

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_7.xml"), *xml));
}

TEST(Examples, Przyklad_8)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 15}, QTime{9, 30, 47}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("Komis ABC AGD sp. z o. o.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Kwiatowa 1 m. 2"), QStringLiteral("00-001 Warszawa")}};
    podmiot1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("abc@abc.pl"), QStringLiteral("667444555")});

    auto podmiot2 = makePodmiot2_JanKowalski();

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 2, 27},
        QStringLiteral("FM2026/02/150"),
        QStringLiteral("15000"),
        Adnotacje{{}, {}, {}, PMarzy{PMarzy::ProceduraMarzy::TOWARY_UZYWANE}},
        RodzajFaktury::Rodzaj::VAT};
    fa.setDataDostawy(QDate{2026, 1, 27});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setP_13_11(QStringLiteral("15000"));
    fa.setFP();

    {
        FaWiersz w{1, false};
        w.setNazwaTowaru(QStringLiteral("samochód używany marki Autex rocznik 2010"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("15000"), false);
        w.setWartoscBrutto(QStringLiteral("15000"));
        w.setGTU(7);
        fa.addFaWiersz(std::move(w));
    }

    Platnosc platnosc;
    internal::XmlVector<ZaplataCzesciowa, 100> wplaty;
    wplaty.push_back(ZaplataCzesciowa{QStringLiteral("10000"), QDate{2026, 1, 27}, FormaPlatnosci::Forma::GOTOWKA});
    platnosc.setZaplacono(true, std::move(wplaty));
    platnosc.addTerminPlatnosci(std::nullopt, TerminOpis{QStringLiteral("30"), QStringLiteral("Dzień"), QStringLiteral("Otrzymanie faktury")});
    platnosc.setFormaPlatnosci(FormaPlatnosci::Forma::PRZELEW);
    platnosc.addRachunekBankowy(RachunekBankowy{QStringLiteral("73111111111111111111111111"), QStringLiteral("Bank Bankowości Bankowej S. A."), QStringLiteral("PLN")});
    fa.setPlatnosc(std::move(platnosc));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_8.xml"), *xml));
}

TEST(Examples, Przyklad_9)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 15}, QTime{9, 30, 47}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("ABC Leasing S.A.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Kwiatowa 1 m. 2"), QStringLiteral("00-001 Warszawa")}};
    podmiot1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("abc@abc.pl"), QStringLiteral("667444555")});

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("Gmina Bzdziszewo")}};
    podmiot2.setJST();
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("Bzdziszewo 1"), QStringLiteral("00-007 Bzdziszewo")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("bzdziszewo@tuwartoinwestowac.pl"), QStringLiteral("555777999")});
    podmiot2.setNrKlienta(QStringLiteral("fdfd778343"));

    Podmiot3 podmiot3{
        DaneIdentyfikacyjne3{NabywcaPolski{QStringLiteral("2222222222")}, QStringLiteral("Szkoła Podstawowa w Bzdziszewie")},
        Rola::JST_ODBIORCA};
    podmiot3.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Akacjowa 200"), QStringLiteral("00-007 Bzdziszewo")});
    podmiot3.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("sp@bzdziszewo.p"), QStringLiteral("666888999")});

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 2, 15},
        QStringLiteral("FV2026/02/150"),
        QStringLiteral("2760"),
        Adnotacje{{}, Zwolnienie{Zwolnienie::Podstawa::USTAWA_O_VAT, QStringLiteral("art. 43 ust. 1 pkt 37 ustawy VAT")}},
        RodzajFaktury::Rodzaj::VAT};
    fa.setDataDostawy(QDate{2026, 1, 1}, QDate{2026, 1, 1});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("2000"), QStringLiteral("460"));
    fa.setP_13_7(QStringLiteral("300"));
    fa.addDodatkowyOpis(DodatkowyOpis{std::nullopt, QStringLiteral("część odsetkowa raty"), QStringLiteral("netto 200, vat 46")});

    {
        FaWiersz w{1, false};
        w.setUU_ID(QStringLiteral("aaaa111133339990"));
        w.setNazwaTowaru(QStringLiteral("rata leasingowa za 01/2026"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("2000"), true);
        w.setWartoscNetto(QStringLiteral("2000"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{2, false};
        w.setUU_ID(QStringLiteral("aaaa111133339991"));
        w.setNazwaTowaru(QStringLiteral("pakiet ubezpieczeń za 01/2026"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("300"), true);
        w.setWartoscNetto(QStringLiteral("300"));
        w.setStawkaVAT(FaWiersz::P_12{FaWiersz::P_12::Stawka::ZW});
        fa.addFaWiersz(std::move(w));
    }

    Platnosc platnosc;
    platnosc.addTerminPlatnosci(QDate{2026, 3, 15}, std::nullopt);
    platnosc.setFormaPlatnosci(FormaPlatnosci::Forma::PRZELEW);
    platnosc.addRachunekBankowy(RachunekBankowy{QStringLiteral("73111111111111111111111111"), QStringLiteral("Bank Bankowości Bankowej S. A."), QStringLiteral("PLN")});
    fa.setPlatnosc(std::move(platnosc));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.addPodmiot3(std::move(podmiot3));
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_9.xml"), *xml));
}

TEST(Examples, Przyklad_10)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 15}, QTime{9, 30, 47}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("ABC Developex sp. z o. o.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 m. 3"), QStringLiteral("00-002 Kraków")}};
    podmiot1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("abc@abc.pl"), QStringLiteral("667444555")});

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("F.H.U. Jan Kowalski")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});

    Podmiot3 podmiot3{
        DaneIdentyfikacyjne3{NabywcaPolski{QStringLiteral("3333333333")}, QStringLiteral("F.H.U. Grażyna Kowalska")},
        Rola::DODATKOWY_NABYWCA};
    podmiot3.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot3.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});
    podmiot3.setUdzial(50);

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 2, 15},
        QStringLiteral("FZ2026/02/150"),
        QStringLiteral("20000"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::ZAL};
    fa.setDataDostawy(QDate{2026, 2, 15});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("16260.16"), QStringLiteral("3739.84"));
    fa.addDodatkowyOpis(DodatkowyOpis{std::nullopt, QStringLiteral("wysokosć wpłaconego zadatku"), QStringLiteral("20000 zł")});

    Platnosc platnosc;
    platnosc.setZaplacono(QDate{2026, 2, 15});
    platnosc.setFormaPlatnosci(FormaPlatnosci::Forma::PRZELEW);
    fa.setPlatnosc(std::move(platnosc));

    Zamowienie zamowienie{QStringLiteral("375150")};
    {
        ZamowienieWiersz zw{1};
        zw.setUU_IDZ(QStringLiteral("aaaa111133339990"));
        zw.setNazwaTowaru(QStringLiteral("mieszkanie 50m^2"));
        zw.setMiara(QStringLiteral("szt."));
        zw.setIlosc(QStringLiteral("1"));
        zw.setCenaJednostkowa(QStringLiteral("300000"));
        zw.setWartoscNetto(QStringLiteral("300000"));
        zw.setKwotaVAT(QStringLiteral("69000"));
        zw.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(23)});
        zamowienie.addWiersz(std::move(zw));
    }
    {
        ZamowienieWiersz zw{2};
        zw.setUU_IDZ(QStringLiteral("aaaa111133339991"));
        zw.setNazwaTowaru(QStringLiteral("usługi dodatkowe"));
        zw.setMiara(QStringLiteral("szt."));
        zw.setIlosc(QStringLiteral("1"));
        zw.setCenaJednostkowa(QStringLiteral("5000"));
        zw.setWartoscNetto(QStringLiteral("5000"));
        zw.setKwotaVAT(QStringLiteral("1150"));
        zw.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(23)});
        zamowienie.addWiersz(std::move(zw));
    }
    fa.setZamowienie(std::move(zamowienie));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.addPodmiot3(std::move(podmiot3));
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_10.xml"), *xml));
}

TEST(Examples, Przyklad_11)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 15}, QTime{9, 30, 47}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("ABC Developex sp. z o. o.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 m. 3"), QStringLiteral("00-002 Kraków")}};
    podmiot1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("abc@abc.pl"), QStringLiteral("667444555")});

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("F.H.U. Jan Kowalski")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});

    Podmiot3 podmiot3{
        DaneIdentyfikacyjne3{NabywcaPolski{QStringLiteral("3333333333")}, QStringLiteral("F.H.U. Grażyna Kowalska")},
        Rola::DODATKOWY_NABYWCA};
    podmiot3.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot3.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});
    podmiot3.setUdzial(50);

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 3, 15},
        QStringLiteral("FK2026/03/5"),
        QStringLiteral("0"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::KOR_ZAL};
    fa.setDataDostawy(QDate{2026, 2, 15});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("-15993.6"), QStringLiteral("-3678.53"));
    fa.setVatObniz1(QStringLiteral("18214.94"), QStringLiteral("1457.19"));

    Korekta korekta;
    korekta.setPrzyczynaKorekty(QStringLiteral("błędna stawka VAT"));
    korekta.setTypKorekty(TypKorekty::W_DACIE_INNEJ);
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{
        QDate{2026, 2, 15}, QStringLiteral("FZ2026/02/150"),
        QStringLiteral("9999999999-20230908-8BEF280C8D35-4D")});
    korekta.setZaliczka(Korekta::Zaliczka{QStringLiteral("20000"), std::nullopt});
    fa.setKorekta(std::move(korekta));

    Zamowienie zamowienie{QStringLiteral("375150")};
    {
        ZamowienieWiersz zw{1, true};
        zw.setUU_IDZ(QStringLiteral("aaaa111133339990"));
        zw.setNazwaTowaru(QStringLiteral("mieszkanie 50m^2"));
        zw.setMiara(QStringLiteral("szt."));
        zw.setIlosc(QStringLiteral("1"));
        zw.setCenaJednostkowa(QStringLiteral("300000"));
        zw.setWartoscNetto(QStringLiteral("300000"));
        zw.setKwotaVAT(QStringLiteral("69000"));
        zw.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(23)});
        zamowienie.addWiersz(std::move(zw));
    }
    {
        ZamowienieWiersz zw{1};
        zw.setUU_IDZ(QStringLiteral("aaaa111133339990"));
        zw.setNazwaTowaru(QStringLiteral("mieszkanie 50m^2"));
        zw.setMiara(QStringLiteral("szt."));
        zw.setIlosc(QStringLiteral("1"));
        zw.setCenaJednostkowa(QStringLiteral("341666.67"));
        zw.setWartoscNetto(QStringLiteral("341666.67"));
        zw.setKwotaVAT(QStringLiteral("27333.33"));
        zw.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(8)});
        zamowienie.addWiersz(std::move(zw));
    }
    {
        ZamowienieWiersz zw{2, true};
        zw.setUU_IDZ(QStringLiteral("aaaa111133339991"));
        zw.setNazwaTowaru(QStringLiteral("usługi dodatkowe"));
        zw.setMiara(QStringLiteral("szt."));
        zw.setIlosc(QStringLiteral("1"));
        zw.setCenaJednostkowa(QStringLiteral("5000"));
        zw.setWartoscNetto(QStringLiteral("5000"));
        zw.setKwotaVAT(QStringLiteral("1150"));
        zw.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(23)});
        zamowienie.addWiersz(std::move(zw));
    }
    {
        ZamowienieWiersz zw{2};
        zw.setUU_IDZ(QStringLiteral("aaaa111133339991"));
        zw.setNazwaTowaru(QStringLiteral("usługi dodatkowe"));
        zw.setMiara(QStringLiteral("szt."));
        zw.setIlosc(QStringLiteral("1"));
        zw.setCenaJednostkowa(QStringLiteral("5000"));
        zw.setWartoscNetto(QStringLiteral("5000"));
        zw.setKwotaVAT(QStringLiteral("1150"));
        zw.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(23)});
        zamowienie.addWiersz(std::move(zw));
    }
    fa.setZamowienie(std::move(zamowienie));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.addPodmiot3(std::move(podmiot3));
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_11.xml"), *xml));
}

TEST(Examples, Przyklad_12)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 15}, QTime{9, 30, 47}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("ABC Developex sp. z o. o.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 m. 3"), QStringLiteral("00-002 Kraków")}};
    podmiot1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("abc@abc.pl"), QStringLiteral("667444555")});

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("F.H.U. Jan Kowalski")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});

    Podmiot3 podmiot3{
        DaneIdentyfikacyjne3{NabywcaPolski{QStringLiteral("3333333333")}, QStringLiteral("F.H.U. Grażyna Kowalska")},
        Rola::DODATKOWY_NABYWCA};
    podmiot3.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot3.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});
    podmiot3.setUdzial(50);

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 3, 17},
        QStringLiteral("FK2026/03/7"),
        QStringLiteral("10000"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::KOR_ZAL};
    fa.setDataDostawy(QDate{2026, 2, 15});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("133.28"), QStringLiteral("30.65"));
    fa.setVatObniz1(QStringLiteral("9107.47"), QStringLiteral("728.60"));

    Korekta korekta;
    korekta.setPrzyczynaKorekty(QStringLiteral("błędne zafakturowanie zaniżonej kwoty zadatku 20000 powinno być 30000"));
    korekta.setTypKorekty(TypKorekty::W_DACIE_FAKTURY_PIERWOTNEJ);
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{
        QDate{2026, 2, 15}, QStringLiteral("FZ2026/02/150"),
        QStringLiteral("9999999999-20230908-8BEF280C8D35-4D")});
    korekta.setZaliczka(Korekta::Zaliczka{QStringLiteral("20000"), std::nullopt});
    fa.setKorekta(std::move(korekta));

    Zamowienie zamowienie{QStringLiteral("375150")};
    {
        ZamowienieWiersz zw{1, true};
        zw.setUU_IDZ(QStringLiteral("aaaa111133339990"));
        zw.setNazwaTowaru(QStringLiteral("mieszkanie 50m^2"));
        zw.setMiara(QStringLiteral("szt."));
        zw.setIlosc(QStringLiteral("1"));
        zw.setCenaJednostkowa(QStringLiteral("341666.67"));
        zw.setWartoscNetto(QStringLiteral("341666.67"));
        zw.setKwotaVAT(QStringLiteral("27333.33"));
        zw.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(8)});
        zamowienie.addWiersz(std::move(zw));
    }
    {
        ZamowienieWiersz zw{1};
        zw.setUU_IDZ(QStringLiteral("aaaa111133339990"));
        zw.setNazwaTowaru(QStringLiteral("mieszkanie 50m^2"));
        zw.setMiara(QStringLiteral("szt."));
        zw.setIlosc(QStringLiteral("1"));
        zw.setCenaJednostkowa(QStringLiteral("341666.67"));
        zw.setWartoscNetto(QStringLiteral("341666.67"));
        zw.setKwotaVAT(QStringLiteral("27333.33"));
        zw.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(8)});
        zamowienie.addWiersz(std::move(zw));
    }
    {
        ZamowienieWiersz zw{2, true};
        zw.setUU_IDZ(QStringLiteral("aaaa111133339991"));
        zw.setNazwaTowaru(QStringLiteral("usługi dodatkowe"));
        zw.setMiara(QStringLiteral("szt."));
        zw.setIlosc(QStringLiteral("1"));
        zw.setCenaJednostkowa(QStringLiteral("5000"));
        zw.setWartoscNetto(QStringLiteral("5000"));
        zw.setKwotaVAT(QStringLiteral("1150"));
        zw.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(23)});
        zamowienie.addWiersz(std::move(zw));
    }
    {
        ZamowienieWiersz zw{2};
        zw.setUU_IDZ(QStringLiteral("aaaa111133339991"));
        zw.setNazwaTowaru(QStringLiteral("usługi dodatkowe"));
        zw.setMiara(QStringLiteral("szt."));
        zw.setIlosc(QStringLiteral("1"));
        zw.setCenaJednostkowa(QStringLiteral("5000"));
        zw.setWartoscNetto(QStringLiteral("5000"));
        zw.setKwotaVAT(QStringLiteral("1150"));
        zw.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(23)});
        zamowienie.addWiersz(std::move(zw));
    }
    fa.setZamowienie(std::move(zamowienie));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.addPodmiot3(std::move(podmiot3));
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_12.xml"), *xml));
}

TEST(Examples, Przyklad_13)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 15}, QTime{9, 30, 47}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("ABC Developex sp. z o. o.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 m. 3"), QStringLiteral("00-002 Kraków")}};
    podmiot1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("abc@abc.pl"), QStringLiteral("667444555")});

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("F.H.U. Jan Kowalski")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});

    Podmiot3 podmiot3{
        DaneIdentyfikacyjne3{NabywcaPolski{QStringLiteral("3333333333")}, QStringLiteral("F.H.U. Grażyna Kowalska")},
        Rola::DODATKOWY_NABYWCA};
    podmiot3.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot3.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});
    podmiot3.setUdzial(50);

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 3, 17},
        QStringLiteral("FK2026/03/7"),
        QStringLiteral("0"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::KOR_ZAL};
    fa.setDataDostawy(QDate{2026, 2, 15});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));

    Korekta korekta;
    korekta.setPrzyczynaKorekty(QStringLiteral("korekta wartości zamówienia o -10%"));
    korekta.setTypKorekty(TypKorekty::W_DACIE_INNEJ);
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{
        QDate{2026, 2, 15}, QStringLiteral("FZ2026/02/150"),
        QStringLiteral("9999999999-20230908-8BEF280C8D35-4D")});
    korekta.setZaliczka(Korekta::Zaliczka{QStringLiteral("30000"), std::nullopt});
    fa.setKorekta(std::move(korekta));

    Zamowienie zamowienie{QStringLiteral("337635")};
    {
        ZamowienieWiersz zw{1, true};
        zw.setUU_IDZ(QStringLiteral("aaaa111133339990"));
        zw.setNazwaTowaru(QStringLiteral("mieszkanie 50m^2"));
        zw.setMiara(QStringLiteral("szt."));
        zw.setIlosc(QStringLiteral("1"));
        zw.setCenaJednostkowa(QStringLiteral("341666.67"));
        zw.setWartoscNetto(QStringLiteral("341666.67"));
        zw.setKwotaVAT(QStringLiteral("27333.33"));
        zw.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(8)});
        zamowienie.addWiersz(std::move(zw));
    }
    {
        ZamowienieWiersz zw{1};
        zw.setUU_IDZ(QStringLiteral("aaaa111133339990"));
        zw.setNazwaTowaru(QStringLiteral("mieszkanie 50m^2"));
        zw.setMiara(QStringLiteral("szt."));
        zw.setIlosc(QStringLiteral("1"));
        zw.setCenaJednostkowa(QStringLiteral("307500"));
        zw.setWartoscNetto(QStringLiteral("307500"));
        zw.setKwotaVAT(QStringLiteral("24600"));
        zw.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(8)});
        zamowienie.addWiersz(std::move(zw));
    }
    {
        ZamowienieWiersz zw{2, true};
        zw.setUU_IDZ(QStringLiteral("aaaa111133339991"));
        zw.setNazwaTowaru(QStringLiteral("usługi dodatkowe"));
        zw.setMiara(QStringLiteral("szt."));
        zw.setIlosc(QStringLiteral("1"));
        zw.setCenaJednostkowa(QStringLiteral("5000"));
        zw.setWartoscNetto(QStringLiteral("5000"));
        zw.setKwotaVAT(QStringLiteral("1150"));
        zw.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(23)});
        zamowienie.addWiersz(std::move(zw));
    }
    {
        ZamowienieWiersz zw{2};
        zw.setUU_IDZ(QStringLiteral("aaaa111133339991"));
        zw.setNazwaTowaru(QStringLiteral("usługi dodatkowe"));
        zw.setMiara(QStringLiteral("szt."));
        zw.setIlosc(QStringLiteral("1"));
        zw.setCenaJednostkowa(QStringLiteral("4500"));
        zw.setWartoscNetto(QStringLiteral("4500"));
        zw.setKwotaVAT(QStringLiteral("1035"));
        zw.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(23)});
        zamowienie.addWiersz(std::move(zw));
    }
    fa.setZamowienie(std::move(zamowienie));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.addPodmiot3(std::move(podmiot3));
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_13.xml"), *xml));
}

TEST(Examples, Przyklad_24)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 28}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("SamploFaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("Elektrownia S.A.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Kwiatowa 1 m. 2"), QStringLiteral("00-001 Warszawa")}};
    podmiot1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("elektrownia@elektrownia.pl"), QStringLiteral("667444555")});

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("F.H.U. Jan Kowalski")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});
    podmiot2.setNrKlienta(QStringLiteral("99999999"));

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 2, 28},
        QStringLiteral("FV2026/02/150"),
        QStringLiteral("53.63"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::VAT};
    fa.setDataDostawy(QDate{2026, 1, 1}, QDate{2026, 2, 28});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("43.60"), QStringLiteral("10.03"));
    fa.addDodatkowyOpis(DodatkowyOpis{std::nullopt,
                                      QStringLiteral("Informacja o akcyzie"),
                                      QStringLiteral("Od 20 kWh energii elektrycznej czynnej naliczono akcyzę w kwocie 0,10 zł")});

    {
        FaWiersz w{1, false};
        w.setNazwaTowaru(QStringLiteral("Razem za energię czynną"));
        w.setWartoscNetto(QStringLiteral("18.00"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{2, false};
        w.setNazwaTowaru(QStringLiteral("Razem za usługi dystrybucji"));
        w.setWartoscNetto(QStringLiteral("25.60"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }

    internal::XmlVector<Obciazenia, 100> obcVec;
    obcVec.push_back(Obciazenia{QStringLiteral("0.00"), QStringLiteral("Odsetki")});
    internal::XmlVector<Odliczenia, 100> odlVec;
    odlVec.push_back(Odliczenia{QStringLiteral("0.00"), QStringLiteral("Nadpłata")});
    Rozliczenie rozliczenie{
        std::make_tuple(std::move(obcVec), QStringLiteral("0.00")),
        std::make_tuple(std::move(odlVec), QStringLiteral("0.00")),
        true,
        QStringLiteral("53.63")};
    fa.setRozliczenie(std::move(rozliczenie));

    Platnosc platnosc;
    platnosc.addTerminPlatnosci(QDate{2026, 3, 15}, std::nullopt);
    platnosc.setFormaPlatnosci(FormaPlatnosci::Forma::PRZELEW);
    platnosc.addRachunekBankowy(RachunekBankowy{QStringLiteral("73111111111111111111111111"), QStringLiteral("Bank Bankowości Bankowej S. A."), QStringLiteral("PLN")});
    fa.setPlatnosc(std::move(platnosc));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    Zalacznik zalacznik;
    BlokDanych blok;
    blok.addMetaDane(ZMetaDane{QStringLiteral("Miejsce poboru energii"), QStringLiteral("ul. Polna 1, 00-001 Warszawa")});
    blok.addMetaDane(ZMetaDane{QStringLiteral("Kod PPE"), QStringLiteral("999999999999999999")});
    blok.addMetaDane(ZMetaDane{QStringLiteral("Nr kontrahenta (odbiorcy)"), QStringLiteral("99999999")});
    blok.addMetaDane(ZMetaDane{QStringLiteral("Za okres od"), QStringLiteral("2026-01-01")});
    blok.addMetaDane(ZMetaDane{QStringLiteral("Za okres do"), QStringLiteral("2026-02-28")});
    blok.addMetaDane(ZMetaDane{QStringLiteral("Grupa taryfowa"), QStringLiteral("G11")});
    blok.addMetaDane(ZMetaDane{QStringLiteral("Energia zużyta w roku 2025"), QStringLiteral("999 kWh")});
    blok.addMetaDane(ZMetaDane{QStringLiteral("Energia zużyta w analogicznym okresie poprzedniego roku kalendarzowego"), QStringLiteral("118 kWh")});

    {
        TNaglowek tn;
        tn.addKol(Kol{Kol::TypKolumny::TXT, QStringLiteral("Licznik/Strefa")});
        tn.addKol(Kol{Kol::TypKolumny::DATE, QStringLiteral("Data")});
        tn.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Wskazanie bieżące")});
        tn.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Wskazanie poprzednie")});
        tn.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Mnożna")});
        tn.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Ilość")});
        tn.addKol(Kol{Kol::TypKolumny::TXT, QStringLiteral("Sposób odczytu")});
        tn.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Straty")});
        tn.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Razem")});
        ZTabela tabela{std::move(tn)};
        tabela.setOpis(QStringLiteral("Odczyty"));
        {
            ZWiersz w;
            w.addWKom(QStringLiteral("Licznik rozliczeniowy  energii czynnej nr 99999999"));
            tabela.addWiersz(std::move(w));
        }
        {
            ZWiersz w;
            w.addWKom(QStringLiteral("całodobowa"));
            w.addWKom(QStringLiteral("2026-02-28"));
            w.addWKom(QStringLiteral("1020"));
            w.addWKom(QStringLiteral("1000"));
            w.addWKom(QStringLiteral("1"));
            w.addWKom(QStringLiteral("20"));
            w.addWKom(QStringLiteral("Fizyczny"));
            w.addWKom(QStringLiteral("0"));
            w.addWKom(QStringLiteral("20"));
            tabela.addWiersz(std::move(w));
        }
        blok.addTabela(std::move(tabela));
    }

    {
        TNaglowek tn;
        tn.addKol(Kol{Kol::TypKolumny::TXT, QStringLiteral("Opis/Strefa")});
        tn.addKol(Kol{Kol::TypKolumny::TXT, QStringLiteral("j. m.")});
        tn.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Ilość")});
        tn.addKol(Kol{Kol::TypKolumny::DEC, QStringLiteral("Cena jedn. netto (zł)")});
        tn.addKol(Kol{Kol::TypKolumny::DEC, QStringLiteral("Należność netto (zł)")});
        tn.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Stawka VAT(%)")});
        ZTabela tabela{std::move(tn)};
        tabela.addTMetaDane(TMetaDane{QStringLiteral("Informacja o akcyzie"), QStringLiteral("Od 20 kWh energii elektrycznej czynnej naliczono akcyzę w kwocie 0,10 zł")});
        tabela.setOpis(QStringLiteral("Rozliczenie - sprzedaż energii"));
        {
            ZWiersz w;
            w.addWKom(QStringLiteral("Energia elektryczna czynna"));
            tabela.addWiersz(std::move(w));
        }
        {
            ZWiersz w;
            w.addWKom(QStringLiteral("całodobowa"));
            w.addWKom(QStringLiteral("kWh"));
            w.addWKom(QStringLiteral("20"));
            w.addWKom(QStringLiteral("0.9000"));
            w.addWKom(QStringLiteral("18.00"));
            w.addWKom(QStringLiteral("23"));
            tabela.addWiersz(std::move(w));
        }
        ZSuma suma;
        suma.addSKom(QStringLiteral("Ogółem wartość - sprzedaż energii:"));
        suma.addSKom(QStringLiteral("-"));
        suma.addSKom(QStringLiteral("-"));
        suma.addSKom(QStringLiteral("-"));
        suma.addSKom(QStringLiteral("18.00"));
        suma.addSKom(QStringLiteral("-"));
        tabela.setSuma(std::move(suma));
        blok.addTabela(std::move(tabela));
    }

    {
        TNaglowek tn;
        tn.addKol(Kol{Kol::TypKolumny::TXT, QStringLiteral("Opis/Strefa")});
        tn.addKol(Kol{Kol::TypKolumny::TXT, QStringLiteral("j. m.")});
        tn.addKol(Kol{Kol::TypKolumny::DATE, QStringLiteral("Data")});
        tn.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Ilość")});
        tn.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Ilość m-cy")});
        tn.addKol(Kol{Kol::TypKolumny::DEC, QStringLiteral("Współczynniki")});
        tn.addKol(Kol{Kol::TypKolumny::DEC, QStringLiteral("Cena jedn. netto (zł)")});
        tn.addKol(Kol{Kol::TypKolumny::DEC, QStringLiteral("Należność netto (zł)")});
        tn.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Stawka VAT(%)")});
        ZTabela tabela{std::move(tn)};
        tabela.setOpis(QStringLiteral("Rozliczenie - usługa dystrybucji energii"));

        auto addPair = [&](const QString &name, const QStringList &vals)
        {
            {
                ZWiersz w;
                w.addWKom(name);
                tabela.addWiersz(std::move(w));
            }
            {
                ZWiersz w;
                for (auto &v : vals)
                    w.addWKom(v);
                tabela.addWiersz(std::move(w));
            }
        };
        addPair(QStringLiteral("Opłata stała sieciowa - układ 1-fazowy"),
                {QStringLiteral("-"), QStringLiteral("zł/mc"), QStringLiteral("2026-02-28"), QStringLiteral("-"), QStringLiteral("2"), QStringLiteral("0.0000"), QStringLiteral("7.3000"), QStringLiteral("14.60"), QStringLiteral("23")});
        addPair(QStringLiteral("Opłata przejściowa <500 kWh"),
                {QStringLiteral("-"), QStringLiteral("zł/mc"), QStringLiteral("2026-02-28"), QStringLiteral("-"), QStringLiteral("2"), QStringLiteral("0.0000"), QStringLiteral("0.0300"), QStringLiteral("0.06"), QStringLiteral("23")});
        addPair(QStringLiteral("Opłata mocowa <500 kWh"),
                {QStringLiteral("-"), QStringLiteral("zł/mc"), QStringLiteral("2026-02-28"), QStringLiteral("-"), QStringLiteral("2"), QStringLiteral("0.0000"), QStringLiteral("0.0000"), QStringLiteral("0.00"), QStringLiteral("23")});
        addPair(QStringLiteral("Opłata zmienna sieciowa"),
                {QStringLiteral("całodobowa"), QStringLiteral("kWh"), QStringLiteral("2026-02-28"), QStringLiteral("20"), QStringLiteral("-"), QStringLiteral("-"), QStringLiteral("0.2500"), QStringLiteral("5.00"), QStringLiteral("23")});
        addPair(QStringLiteral("Opłata jakościowa"),
                {QStringLiteral("całodobowa"), QStringLiteral("kWh"), QStringLiteral("2026-02-28"), QStringLiteral("20"), QStringLiteral("-"), QStringLiteral("-"), QStringLiteral("0.0400"), QStringLiteral("0.80"), QStringLiteral("23")});
        addPair(QStringLiteral("Opłata OZE"),
                {QStringLiteral("całodobowa"), QStringLiteral("kWh"), QStringLiteral("2026-02-28"), QStringLiteral("20"), QStringLiteral("-"), QStringLiteral("-"), QStringLiteral("0.0040"), QStringLiteral("0.08"), QStringLiteral("23")});
        addPair(QStringLiteral("Opłata kogeneracyjna"),
                {QStringLiteral("całodobowa"), QStringLiteral("kWh"), QStringLiteral("2026-02-28"), QStringLiteral("20"), QStringLiteral("-"), QStringLiteral("-"), QStringLiteral("0.0030"), QStringLiteral("0.06"), QStringLiteral("23")});
        addPair(QStringLiteral("Opłata abonamentowa"),
                {QStringLiteral("-"), QStringLiteral("zł/mc"), QStringLiteral("2026-02-28"), QStringLiteral("-"), QStringLiteral("2"), QStringLiteral("0.0000"), QStringLiteral("2.5000"), QStringLiteral("5.00"), QStringLiteral("23")});

        ZSuma suma;
        suma.addSKom(QStringLiteral("Ogółem wartość - usługa dystrybucji:"));
        suma.addSKom(QStringLiteral("-"));
        suma.addSKom(QStringLiteral("-"));
        suma.addSKom(QStringLiteral("-"));
        suma.addSKom(QStringLiteral("-"));
        suma.addSKom(QStringLiteral("-"));
        suma.addSKom(QStringLiteral("-"));
        suma.addSKom(QStringLiteral("25.60"));
        suma.addSKom(QStringLiteral("-"));
        tabela.setSuma(std::move(suma));
        blok.addTabela(std::move(tabela));
    }

    zalacznik.addBlokDanych(std::move(blok));
    faktura.setZalacznik(std::move(zalacznik));

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_24.xml"), *xml));
}

TEST(Examples, Przyklad_14)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 8, 17}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("ABC Developex S.A.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 m. 3"), QStringLiteral("00-002 Kraków")}};
    podmiot1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("abc@abc.pl"), QStringLiteral("667444555")});

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("F.H.U. Jan Kowalski")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 8, 17},
        QStringLiteral("FV2026/08/12"),
        QStringLiteral("307635"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::ROZ};
    fa.setDataDostawy(QDate{2026, 9, 17});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("4100.16"), QStringLiteral("943.04"));
    fa.setVatObniz1(QStringLiteral("280177.59"), QStringLiteral("22414.21"));
    fa.addDodatkowyOpis(DodatkowyOpis{std::nullopt, QStringLiteral("wysokość pozostałej do zapłaty kwoty"), QStringLiteral("307635 zł")});
    fa.addDodatkowyOpis(DodatkowyOpis{std::nullopt, QStringLiteral("W terminie 2026-09-15"), QStringLiteral("co najmniej 50% pozostałej kwoty")});
    fa.addDodatkowyOpis(DodatkowyOpis{std::nullopt, QStringLiteral("W terminie 2026-10-15"), QStringLiteral("pozostała część")});
    fa.addFakturaZaliczkowa(FakturaZaliczkowa{false, QStringLiteral("9999999999-20230908-8BEF280C8D35-4D")});

    {
        FaWiersz w{1, false};
        w.setUU_ID(QStringLiteral("aaaa111133339997"));
        w.setNazwaTowaru(QStringLiteral("mieszkanie 50m^2"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("307500"), true);
        w.setWartoscNetto(QStringLiteral("307500"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(8)});
        w.setGTU(10);
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{2, false};
        w.setUU_ID(QStringLiteral("aaaa111133339998"));
        w.setNazwaTowaru(QStringLiteral("usługi dodatkowe"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("4500"), true);
        w.setWartoscNetto(QStringLiteral("4500"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }

    Platnosc platnosc;
    platnosc.addTerminPlatnosci(QDate{2026, 9, 15}, std::nullopt);
    platnosc.addTerminPlatnosci(QDate{2026, 10, 15}, std::nullopt);
    platnosc.setFormaPlatnosci(FormaPlatnosci::Forma::PRZELEW);
    platnosc.addRachunekBankowy(RachunekBankowy{QStringLiteral("73111111111111111111111111"),
                                                QStringLiteral("Bank Bankowości Bankowej S. A."),
                                                QStringLiteral("PLN")});
    fa.setPlatnosc(std::move(platnosc));

    Podmiot3 podmiot3_1{
        DaneIdentyfikacyjne3{NabywcaPolski{QStringLiteral("3333333333")}, QStringLiteral("F.H.U. Grażyna Kowalska")},
        Rola::DODATKOWY_NABYWCA};
    podmiot3_1.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot3_1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});
    podmiot3_1.setUdzial(50);

    Podmiot3 podmiot3_2{
        DaneIdentyfikacyjne3{NabywcaPolski{QStringLiteral("9999999999")}, QStringLiteral("ABC Developex sp. z o.o.")},
        Rola::PODMIOT_PIERWOTNY};
    podmiot3_2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 lok. 3"), QStringLiteral("00-002 Kraków")});
    podmiot3_2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("abc@abc.pl"), QStringLiteral("667444555")});

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.addPodmiot3(std::move(podmiot3_1));
    faktura.addPodmiot3(std::move(podmiot3_2));
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_14.xml"), *xml));
}

TEST(Examples, Przyklad_15)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 1}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("SamploFaktur")};

    auto podmiot1 = makePodmiot1_ABC();

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, std::nullopt}};

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 2, 15},
        QStringLiteral("FV2026/02/150"),
        QStringLiteral("450"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::UPR};
    fa.setDataDostawy(QDate{2026, 1, 3});
    fa.setVatPodst(QStringLiteral("365.85"), QStringLiteral("84.15"));

    {
        FaWiersz w{1, false};
        w.setNazwaTowaru(QStringLiteral("wiertarka Wiertex mk5"));
        fa.addFaWiersz(std::move(w));
    }

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_15.xml"), *xml));
}

TEST(Examples, Przyklad_16)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 1}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("SamploFaktur")};

    auto podmiot1 = makePodmiot1_ABC();

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, std::nullopt}};

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 2, 15},
        QStringLiteral("FV2026/02/150"),
        QStringLiteral("450"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::UPR};
    fa.setDataDostawy(QDate{2026, 1, 3});

    {
        FaWiersz w{1, false};
        w.setNazwaTowaru(QStringLiteral("wiertarka Wiertex mk5"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_16.xml"), *xml));
}

TEST(Examples, Przyklad_17)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 8, 17}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("ABC Developex S.A.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 lok. 3"), QStringLiteral("00-002 Kraków")}};
    podmiot1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("abc@abc.pl"), QStringLiteral("667444555")});

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("F.H.U. Jan Kowalski")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 8, 17},
        QStringLiteral("FV2026/08/12"),
        QStringLiteral("300000"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::ROZ};
    fa.setDataDostawy(QDate{2026, 9, 17});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("3998.40"), QStringLiteral("919.63"));
    fa.setVatObniz1(QStringLiteral("273224.05"), QStringLiteral("21857.92"));
    fa.addDodatkowyOpis(DodatkowyOpis{std::nullopt, QStringLiteral("wysokosć pozostałej do zapłaty kwoty"), QStringLiteral("300000 zł")});
    fa.addFakturaZaliczkowa(FakturaZaliczkowa{false, QStringLiteral("9999999999-20230908-8BEF280C8D35-4D")});

    {
        FaWiersz w{1, false};
        w.setUU_ID(QStringLiteral("aaaa111133339997"));
        w.setNazwaTowaru(QStringLiteral("mieszkanie 50m^2"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("307500"), true);
        w.setWartoscNetto(QStringLiteral("307500"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(8)});
        w.setGTU(10);
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{2, false};
        w.setUU_ID(QStringLiteral("aaaa111133339998"));
        w.setNazwaTowaru(QStringLiteral("usługi dodatkowe"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("4500"), true);
        w.setWartoscNetto(QStringLiteral("4500"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }

    Podmiot3 podmiot3_1{
        DaneIdentyfikacyjne3{NabywcaPolski{QStringLiteral("3333333333")}, QStringLiteral("F.H.U. Grażyna Kowalska")},
        Rola::DODATKOWY_NABYWCA};
    podmiot3_1.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot3_1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});
    podmiot3_1.setUdzial(50);

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.addPodmiot3(std::move(podmiot3_1));
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_17.xml"), *xml));
}

TEST(Examples, Przyklad_18)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 8, 17}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("ABC Developex S.A.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 lok. 3"), QStringLiteral("00-002 Kraków")}};
    podmiot1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("abc@abc.pl"), QStringLiteral("667444555")});

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("F.H.U. Jan Kowalski")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 8, 17},
        QStringLiteral("FK2026/09/1"),
        QStringLiteral("7635"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::KOR_ROZ};
    fa.setDataDostawy(QDate{2026, 9, 17});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("101.76"), QStringLiteral("23.41"));
    fa.setVatObniz1(QStringLiteral("6953.54"), QStringLiteral("556.29"));

    Korekta korekta;
    korekta.setPrzyczynaKorekty(QStringLiteral("błędne zafakturowanie kwoty pozostałej do zapłaty: było 300000, a powinno być 307635"));
    korekta.setTypKorekty(TypKorekty::Typ::W_DACIE_FAKTURY_PIERWOTNEJ);
    korekta.addDaneFaKorygowanej(DaneFaKorygowanej{QDate{2026, 8, 17}, QStringLiteral("FV2026/08/12"), QStringLiteral("9999999999-20230908-8BEF280C8D35-4D")});
    korekta.setZaliczka(Korekta::Zaliczka{QStringLiteral("300000"), std::nullopt});
    fa.setKorekta(std::move(korekta));

    fa.addFakturaZaliczkowa(FakturaZaliczkowa{false, QStringLiteral("9999999999-20230908-76B2B580D4DC-80")});

    {
        FaWiersz w{1, true};
        w.setUU_ID(QStringLiteral("aaaa111133339997"));
        w.setNazwaTowaru(QStringLiteral("mieszkanie 50m^2"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("307500"), true);
        w.setWartoscNetto(QStringLiteral("307500"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(8)});
        w.setGTU(10);
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{1, false};
        w.setUU_ID(QStringLiteral("aaaa111133339997"));
        w.setNazwaTowaru(QStringLiteral("mieszkanie 50m2"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("307500"), true);
        w.setWartoscNetto(QStringLiteral("307500"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(8)});
        w.setGTU(10);
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{2, true};
        w.setUU_ID(QStringLiteral("aaaa111133339998"));
        w.setNazwaTowaru(QStringLiteral("usługi dodatkowe"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("4500"), true);
        w.setWartoscNetto(QStringLiteral("4500"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{2, false};
        w.setUU_ID(QStringLiteral("aaaa111133339998"));
        w.setNazwaTowaru(QStringLiteral("usługi dodatkowe"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("4500"), true);
        w.setWartoscNetto(QStringLiteral("4500"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }

    Podmiot3 podmiot3_1{
        DaneIdentyfikacyjne3{NabywcaPolski{QStringLiteral("3333333333")}, QStringLiteral("F.H.U. Grażyna Kowalska")},
        Rola::DODATKOWY_NABYWCA};
    podmiot3_1.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot3_1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});
    podmiot3_1.setUdzial(50);

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.addPodmiot3(std::move(podmiot3_1));
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_18.xml"), *xml));
}

TEST(Examples, Przyklad_19)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 1}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("Biuro Podróży ABC sp. z o. o.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 lok. 3"), QStringLiteral("00-002 Kraków")}};
    podmiot1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("abc@abc.pl"), QStringLiteral("667444555")});

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("F.H.U. Jan Kowalski")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 2, 15},
        QStringLiteral("FM2026/02/150"),
        QStringLiteral("3000"),
        Adnotacje{{}, {}, {}, PMarzy{PMarzy::ProceduraMarzy::BLA_BIUR_PODROZY}},
        RodzajFaktury::Rodzaj::VAT};
    fa.setDataDostawy(QDate{2026, 1, 27});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("813"), QStringLiteral("187"));
    fa.setP_13_11(QStringLiteral("2000"));

    {
        FaWiersz w{1, false};
        w.setNazwaTowaru(QStringLiteral("wycieczka na Mazury usługi obce"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("2000"), false);
        w.setWartoscBrutto(QStringLiteral("2000"));
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{2, false};
        w.setNazwaTowaru(QStringLiteral("wycieczka na Mazury usługa własna"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("1"));
        w.setCenaJednostkowa(QStringLiteral("1000"), false);
        w.setWartoscBrutto(QStringLiteral("1000"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_19.xml"), *xml));
}

TEST(Examples, Przyklad_20)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 1}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    auto podmiot1 = makePodmiot1_ABC();

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("CDE sp. j.")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 lok. 3"), QStringLiteral("00-002 Kraków"), QStringLiteral("a")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("cde@cde.pl"), QStringLiteral("555777999")});
    podmiot2.setNrKlienta(QStringLiteral("fdfd778343"));

    Fa fa{
        QStringLiteral("EUR"),
        QDate{2026, 2, 15},
        QStringLiteral("FV2026/02/150"),
        QStringLiteral("16678.80"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::VAT};
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("13560"), QStringLiteral("3118.80"), QStringLiteral("14036.16"));

    {
        FaWiersz w{1, false};
        w.setUU_ID(QStringLiteral("aaaa111133339990"));
        w.setP_6A(QDate{2026, 2, 20});
        w.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        w.setCN(QStringLiteral("8418 21 91"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("10"));
        w.setCenaJednostkowa(QStringLiteral("406"), true);
        w.setWartoscNetto(QStringLiteral("4060"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        w.setKursWaluty(QStringLiteral("4.5005"));
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{2, false};
        w.setUU_ID(QStringLiteral("aaaa111133339991"));
        w.setP_6A(QDate{2026, 2, 25});
        w.setNazwaTowaru(QStringLiteral("zamrażarka Zimnotech mk2"));
        w.setCN(QStringLiteral("8418 40 20"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("20"));
        w.setCenaJednostkowa(QStringLiteral("250"), true);
        w.setWartoscNetto(QStringLiteral("5000"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        w.setKursWaluty(QStringLiteral("4.5005"));
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{3, false};
        w.setUU_ID(QStringLiteral("aaaa111133339992"));
        w.setP_6A(QDate{2026, 2, 26});
        w.setNazwaTowaru(QStringLiteral("zmywarka Bryza 100"));
        w.setCN(QStringLiteral("8422 11 00"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("15"));
        w.setCenaJednostkowa(QStringLiteral("300"), true);
        w.setWartoscNetto(QStringLiteral("4500"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        w.setKursWaluty(QStringLiteral("4.5005"));
        fa.addFaWiersz(std::move(w));
    }

    Platnosc platnosc;
    platnosc.addTerminPlatnosci(QDate{2026, 3, 15}, std::nullopt);
    platnosc.setFormaPlatnosci(FormaPlatnosci::Forma::PRZELEW);
    RachunekBankowyFaktora rbf{QStringLiteral("73111111111111111111111111"), QStringLiteral("Bank Bankowości Bankowej S. A."), QStringLiteral("PLN")};
    rbf.setRachunekWlasnyBanku(RachunekWlasnyBanku::Typ::TYP_2);
    platnosc.addRachunekBankowyFaktora(std::move(rbf));
    fa.setPlatnosc(std::move(platnosc));

    WarunkiTransakcji wt;
    wt.addZamowienie(ZamowieniaWT{QDate{2026, 1, 26}, QStringLiteral("4354343")});
    wt.addNrPartiiTowaru(QStringLiteral("2312323/2026"));
    wt.setWarunkiDostawy(QStringLiteral("CIP"));

    Transport transport{RodzajTransportu::DROGOWY, OpisLadunku::PALETA};
    transport.setPrzewoznik(Przewoznik{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("6666666666")}, QStringLiteral("Jan Nowak Transport")},
        AdresPrzewoznika{QStringLiteral("PL"), QStringLiteral("ul. Bukowa 5"), QStringLiteral("00-004 Poznań")}});
    transport.setJednostkaOpakowania(QStringLiteral("a"));
    transport.setWysylkaZ(WysylkaZ{QStringLiteral("PL"), QStringLiteral("Sadowa 1 lok. 2"), QStringLiteral("00-001 Warszawa")});
    transport.setWysylkaDo(WysylkaDo{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 lok. 3"), QStringLiteral("00-002 Kraków")});
    wt.addTransport(std::move(transport));
    fa.setWarunkiTransakcji(std::move(wt));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_20.xml"), *xml));
}

TEST(Examples, Przyklad_21)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 1}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("Samplofaktur")};

    auto podmiot1 = makePodmiot1_ABC();

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("CDE sp. j.")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 lok. 3"), QStringLiteral("00-002 Kraków"), QStringLiteral("a")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("cde@cde.pl"), QStringLiteral("555777999")});
    podmiot2.setNrKlienta(QStringLiteral("fdfd778343"));

    Fa fa{
        QStringLiteral("EUR"),
        QDate{2026, 2, 15},
        QStringLiteral("FV2026/02/150"),
        QStringLiteral("16678.80"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::VAT};
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("13560"), QStringLiteral("3118.80"), QStringLiteral("13768.14"));

    {
        FaWiersz w{1, false};
        w.setUU_ID(QStringLiteral("aaaa111133339990"));
        w.setP_6A(QDate{2026, 2, 5});
        w.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        w.setCN(QStringLiteral("8418 21 91"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("10"));
        w.setCenaJednostkowa(QStringLiteral("406"), true);
        w.setWartoscNetto(QStringLiteral("4060"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        w.setKursWaluty(QStringLiteral("4.4080"));
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{2, false};
        w.setUU_ID(QStringLiteral("aaaa111133339991"));
        w.setP_6A(QDate{2026, 2, 10});
        w.setNazwaTowaru(QStringLiteral("zamrażarka Zimnotech mk2"));
        w.setCN(QStringLiteral("8418 40 20"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("20"));
        w.setCenaJednostkowa(QStringLiteral("250"), true);
        w.setWartoscNetto(QStringLiteral("5000"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        w.setKursWaluty(QStringLiteral("4.5005"));
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{3, false};
        w.setUU_ID(QStringLiteral("aaaa111133339992"));
        w.setP_6A(QDate{2026, 2, 20});
        w.setNazwaTowaru(QStringLiteral("zmywarka Bryza 100"));
        w.setCN(QStringLiteral("8422 11 00"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("15"));
        w.setCenaJednostkowa(QStringLiteral("300"), true);
        w.setWartoscNetto(QStringLiteral("4500"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        w.setKursWaluty(QStringLiteral("4.3250"));
        fa.addFaWiersz(std::move(w));
    }

    Platnosc platnosc;
    platnosc.addTerminPlatnosci(QDate{2026, 3, 15}, std::nullopt);
    platnosc.setFormaPlatnosci(FormaPlatnosci::Forma::PRZELEW);
    RachunekBankowyFaktora rbf{QStringLiteral("73111111111111111111111111"), QStringLiteral("Bank Bankowości Bankowej S. A."), QStringLiteral("PLN")};
    rbf.setRachunekWlasnyBanku(RachunekWlasnyBanku::Typ::TYP_2);
    platnosc.addRachunekBankowyFaktora(std::move(rbf));
    fa.setPlatnosc(std::move(platnosc));

    WarunkiTransakcji wt;
    wt.addZamowienie(ZamowieniaWT{QDate{2026, 1, 26}, QStringLiteral("4354343")});
    wt.addNrPartiiTowaru(QStringLiteral("2312323/2026"));
    wt.setWarunkiDostawy(QStringLiteral("CIP"));

    Transport transport{RodzajTransportu::DROGOWY, OpisLadunku::PALETA};
    transport.setPrzewoznik(Przewoznik{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("6666666666")}, QStringLiteral("Jan Nowak Transport")},
        AdresPrzewoznika{QStringLiteral("PL"), QStringLiteral("ul. Bukowa 5"), QStringLiteral("00-004 Poznań")}});
    transport.setJednostkaOpakowania(QStringLiteral("a"));
    transport.setWysylkaZ(WysylkaZ{QStringLiteral("PL"), QStringLiteral("Sadowa 1 lok. 2"), QStringLiteral("00-001 Warszawa")});
    transport.setWysylkaDo(WysylkaDo{QStringLiteral("PL"), QStringLiteral("ul. Sadowa 1 lok. 3"), QStringLiteral("00-002 Kraków")});
    wt.addTransport(std::move(transport));
    fa.setWarunkiTransakcji(std::move(wt));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_21.xml"), *xml));
}

TEST(Examples, Przyklad_22)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 1}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("SamploFaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("ABC AGD sp. z o. o.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Kwiatowa 1 m. 2"), QStringLiteral("00-001 Warszawa")}};
    podmiot1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("abc@abc.pl"), QStringLiteral("667444555")});
    podmiot1.setPrefiksPodatnika(QStringLiteral("PL"));

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaUE{QStringLiteral("DE"), QStringLiteral("999999999")}, QStringLiteral("EFG GmbH")}};
    podmiot2.setAdres(Adres{QStringLiteral("DE"), QStringLiteral("Blümchenstraße 1"), QStringLiteral("10999 Berlin")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("johan@shmidt.de"), QStringLiteral("555777999")});

    Fa fa{
        QStringLiteral("EUR"),
        QDate{2026, 2, 15},
        QStringLiteral("FV2026/02/150"),
        QStringLiteral("4000"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::VAT};
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setP_13_6_2(QStringLiteral("4000"));

    {
        FaWiersz w{1, false};
        w.setUU_ID(QStringLiteral("aaaa111133339990"));
        w.setP_6A(QDate{2026, 1, 3});
        w.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("10"));
        w.setCenaJednostkowa(QStringLiteral("400"), true);
        w.setWartoscNetto(QStringLiteral("4000"));
        w.setStawkaVAT(FaWiersz::P_12{FaWiersz::P_12::Stawka::WDT_0});
        fa.addFaWiersz(std::move(w));
    }

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_22.xml"), *xml));
}

TEST(Examples, Przyklad_23)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 1}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("SamploFaktur")};

    auto podmiot1 = makePodmiot1_ABC();

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPozaUE{QStringLiteral("999999999"), QStringLiteral("US")}, QStringLiteral("EFG Ltd.")}};
    podmiot2.setAdres(Adres{QStringLiteral("US"), QStringLiteral("Flower (St) 1"), QStringLiteral("Seattle, WA 99999")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("john@smith.us"), QStringLiteral("555777999")});

    Fa fa{
        QStringLiteral("USD"),
        QDate{2026, 6, 15},
        QStringLiteral("FV2026/02/150"),
        QStringLiteral("8000"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::VAT};
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setP_13_6_3(QStringLiteral("8000"));

    {
        FaWiersz w{1, false};
        w.setUU_ID(QStringLiteral("aaaa111133339990"));
        w.setP_6A(QDate{2026, 1, 3});
        w.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("20"));
        w.setCenaJednostkowa(QStringLiteral("400"), true);
        w.setWartoscNetto(QStringLiteral("8000"));
        w.setStawkaVAT(FaWiersz::P_12{FaWiersz::P_12::Stawka::EX_0});
        fa.addFaWiersz(std::move(w));
    }

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_23.xml"), *xml));
}

TEST(Examples, Przyklad_25)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 28}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("SamploFaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("Elektrownia S.A.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Kwiatowa 1 m. 2"), QStringLiteral("00-001 Warszawa")}};
    podmiot1.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("elektrownia@elektrownia.pl"), QStringLiteral("667444555")});

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("1111111111")}, QStringLiteral("F.H.U. Jan Kowalski")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("ul. Polna 1"), QStringLiteral("00-001 Warszawa")});
    podmiot2.addDaneKontaktowe(DaneKontaktowe{QStringLiteral("jan@kowalski.pl"), QStringLiteral("555777999")});
    podmiot2.setNrKlienta(QStringLiteral("99999999"));

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 2, 28},
        QStringLiteral("FV2026/02/150"),
        QStringLiteral("53.63"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::VAT};
    fa.setDataDostawy(QDate{2026, 1, 1}, QDate{2026, 2, 28});
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setVatPodst(QStringLiteral("43.60"), QStringLiteral("10.03"));
    fa.addDodatkowyOpis(DodatkowyOpis{std::nullopt, QStringLiteral("Informacja o akcyzie"), QStringLiteral("Od 20 kWh energii elektrycznej czynnej naliczono akcyzę w kwocie 0,10 zł")});

    {
        FaWiersz w{1, false};
        w.setNazwaTowaru(QStringLiteral("Razem za energię czynną"));
        w.setWartoscNetto(QStringLiteral("18.00"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }
    {
        FaWiersz w{2, false};
        w.setNazwaTowaru(QStringLiteral("Razem za usługi dystrybucji"));
        w.setWartoscNetto(QStringLiteral("25.60"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        fa.addFaWiersz(std::move(w));
    }

    internal::XmlVector<Obciazenia, 100> obcVec;
    obcVec.push_back(Obciazenia{QStringLiteral("0.00"), QStringLiteral("Odsetki")});
    internal::XmlVector<Odliczenia, 100> odlVec;
    odlVec.push_back(Odliczenia{QStringLiteral("0.00"), QStringLiteral("Nadpłata")});
    Rozliczenie rozliczenie{
        std::make_tuple(std::move(obcVec), QStringLiteral("0.00")),
        std::make_tuple(std::move(odlVec), QStringLiteral("0.00")),
        true,
        QStringLiteral("53.63")};
    fa.setRozliczenie(std::move(rozliczenie));

    Platnosc platnosc;
    platnosc.addTerminPlatnosci(QDate{2026, 3, 15}, std::nullopt);
    platnosc.setFormaPlatnosci(FormaPlatnosci::Forma::PRZELEW);
    platnosc.addRachunekBankowy(RachunekBankowy{QStringLiteral("73111111111111111111111111"),
                                                QStringLiteral("Bank Bankowości Bankowej S. A."),
                                                QStringLiteral("PLN")});
    fa.setPlatnosc(std::move(platnosc));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};

    BlokDanych blok;
    blok.addMetaDane(ZMetaDane{QStringLiteral("Miejsce poboru energii"), QStringLiteral("ul. Polna 1, 00-001 Warszawa")});
    blok.addMetaDane(ZMetaDane{QStringLiteral("Kod PPE"), QStringLiteral("999999999999999999")});
    blok.addMetaDane(ZMetaDane{QStringLiteral("Nr kontrahenta (odbiorcy)"), QStringLiteral("99999999")});
    blok.addMetaDane(ZMetaDane{QStringLiteral("Za okres od"), QStringLiteral("2026-02-01")});
    blok.addMetaDane(ZMetaDane{QStringLiteral("Za okres do"), QStringLiteral("2026-02-28")});
    blok.addMetaDane(ZMetaDane{QStringLiteral("Grupa taryfowa"), QStringLiteral("G11")});
    blok.addMetaDane(ZMetaDane{QStringLiteral("Energia zużyta w roku 2025"), QStringLiteral("999 kWh")});
    blok.addMetaDane(ZMetaDane{QStringLiteral("Energia zużyta w analogicznym okresie poprzedniego roku kalendarzowego"), QStringLiteral("118 kWh")});

    {
        TNaglowek tnagl;
        tnagl.addKol(Kol{Kol::TypKolumny::TXT, QStringLiteral("Licznik/Strefa")});
        tnagl.addKol(Kol{Kol::TypKolumny::DATE, QStringLiteral("Data")});
        tnagl.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Wskazanie bieżące")});
        tnagl.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Wskazanie poprzednie")});
        tnagl.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Mnożna")});
        tnagl.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Ilość")});
        tnagl.addKol(Kol{Kol::TypKolumny::TXT, QStringLiteral("Sposób odczytu")});
        tnagl.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Straty")});
        tnagl.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Razem")});

        ZTabela tab{std::move(tnagl)};
        tab.setOpis(QStringLiteral("Odczyty"));

        ZWiersz w1;
        w1.addWKom(QStringLiteral("Licznik rozliczeniowy  energii czynnej nr 99999999"));
        tab.addWiersz(std::move(w1));

        ZWiersz w2;
        w2.addWKom(QStringLiteral("całodobowa"));
        w2.addWKom(QStringLiteral("2026-02-28"));
        w2.addWKom(QStringLiteral("1020"));
        w2.addWKom(QStringLiteral("1000"));
        w2.addWKom(QStringLiteral("1"));
        w2.addWKom(QStringLiteral("20"));
        w2.addWKom(QStringLiteral("Fizyczny"));
        w2.addWKom(QStringLiteral("0"));
        w2.addWKom(QStringLiteral("20"));
        tab.addWiersz(std::move(w2));

        blok.addTabela(std::move(tab));
    }

    {
        TNaglowek tnagl;
        tnagl.addKol(Kol{Kol::TypKolumny::TXT, QStringLiteral("Opis/Strefa")});
        tnagl.addKol(Kol{Kol::TypKolumny::TXT, QStringLiteral("j. m.")});
        tnagl.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Ilość")});
        tnagl.addKol(Kol{Kol::TypKolumny::DEC, QStringLiteral("Cena jedn. netto (zł)")});
        tnagl.addKol(Kol{Kol::TypKolumny::DEC, QStringLiteral("Należność netto (zł)")});
        tnagl.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Stawka VAT(%)")});

        ZTabela tab{std::move(tnagl)};
        tab.addTMetaDane(TMetaDane{QStringLiteral("Informacja o akcyzie"), QStringLiteral("Od 20 kWh energii elektrycznej czynnej naliczono akcyzę w kwocie 0,10 zł")});
        tab.setOpis(QStringLiteral("Rozliczenie - sprzedaż energii"));

        ZWiersz w1;
        w1.addWKom(QStringLiteral("Energia elektryczna czynna"));
        tab.addWiersz(std::move(w1));

        ZWiersz w2;
        w2.addWKom(QStringLiteral("całodobowa"));
        w2.addWKom(QStringLiteral("kWh"));
        w2.addWKom(QStringLiteral("20"));
        w2.addWKom(QStringLiteral("0.9000"));
        w2.addWKom(QStringLiteral("18.00"));
        w2.addWKom(QStringLiteral("23"));
        tab.addWiersz(std::move(w2));

        ZSuma suma;
        suma.addSKom(QStringLiteral("Ogółem wartość - sprzedaż energii:"));
        suma.addSKom(QString());
        suma.addSKom(QString());
        suma.addSKom(QString());
        suma.addSKom(QStringLiteral("18.00"));
        suma.addSKom(QString());
        tab.setSuma(std::move(suma));

        blok.addTabela(std::move(tab));
    }

    {
        TNaglowek tnagl;
        tnagl.addKol(Kol{Kol::TypKolumny::TXT, QStringLiteral("Opis/Strefa")});
        tnagl.addKol(Kol{Kol::TypKolumny::TXT, QStringLiteral("j. m.")});
        tnagl.addKol(Kol{Kol::TypKolumny::DATE, QStringLiteral("Data")});
        tnagl.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Ilość")});
        tnagl.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Ilość m-cy")});
        tnagl.addKol(Kol{Kol::TypKolumny::DEC, QStringLiteral("Współczynniki")});
        tnagl.addKol(Kol{Kol::TypKolumny::DEC, QStringLiteral("Cena jedn. netto (zł)")});
        tnagl.addKol(Kol{Kol::TypKolumny::DEC, QStringLiteral("Należność netto (zł)")});
        tnagl.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Stawka VAT(%)")});

        ZTabela tab{std::move(tnagl)};
        tab.setOpis(QStringLiteral("Rozliczenie - usługa dystrybucji energii"));

        auto addServiceRow = [&](const QString &label, const QVector<QString> &vals)
        {
            ZWiersz wLabel;
            wLabel.addWKom(label);
            tab.addWiersz(std::move(wLabel));
            ZWiersz wData;
            for (const auto &v : vals)
                wData.addWKom(v);
            tab.addWiersz(std::move(wData));
        };

        addServiceRow(QStringLiteral("Opłata stała sieciowa - układ 1-fazowy"),
                      {QString(), QStringLiteral("zł/mc"), QStringLiteral("2026-02-28"), QString(), QStringLiteral("2"), QStringLiteral("0.0000"), QStringLiteral("7.3000"), QStringLiteral("14.60"), QStringLiteral("23")});
        addServiceRow(QStringLiteral("Opłata przejściowa <500 kWh"),
                      {QString(), QStringLiteral("zł/mc"), QStringLiteral("2026-02-28"), QString(), QStringLiteral("2"), QStringLiteral("0.0000"), QStringLiteral("0.0300"), QStringLiteral("0.06"), QStringLiteral("23")});
        addServiceRow(QStringLiteral("Opłata mocowa <500 kWh"),
                      {QString(), QStringLiteral("zł/mc"), QStringLiteral("2026-02-28"), QString(), QStringLiteral("2"), QStringLiteral("0.0000"), QStringLiteral("0.0000"), QStringLiteral("0.00"), QStringLiteral("23")});
        addServiceRow(QStringLiteral("Opłata zmienna sieciowa"),
                      {QStringLiteral("całodobowa"), QStringLiteral("kWh"), QStringLiteral("2026-02-28"), QStringLiteral("20"), QString(), QString(), QStringLiteral("0.2500"), QStringLiteral("5.00"), QStringLiteral("23")});
        addServiceRow(QStringLiteral("Opłata jakościowa"),
                      {QStringLiteral("całodobowa"), QStringLiteral("kWh"), QStringLiteral("2026-02-28"), QStringLiteral("20"), QString(), QString(), QStringLiteral("0.0400"), QStringLiteral("0.80"), QStringLiteral("23")});
        addServiceRow(QStringLiteral("Opłata OZE"),
                      {QStringLiteral("całodobowa"), QStringLiteral("kWh"), QStringLiteral("2026-02-28"), QStringLiteral("20"), QString(), QString(), QStringLiteral("0.0040"), QStringLiteral("0.08"), QStringLiteral("23")});
        addServiceRow(QStringLiteral("Opłata kogeneracyjna"),
                      {QStringLiteral("całodobowa"), QStringLiteral("kWh"), QStringLiteral("2026-02-28"), QStringLiteral("20"), QString(), QString(), QStringLiteral("0.0030"), QStringLiteral("0.06"), QStringLiteral("23")});
        addServiceRow(QStringLiteral("Opłata abonamentowa"),
                      {QString(), QStringLiteral("zł/mc"), QStringLiteral("2026-02-28"), QString(), QStringLiteral("2"), QStringLiteral("0.0000"), QStringLiteral("2.5000"), QStringLiteral("5.00"), QStringLiteral("23")});

        ZSuma suma;
        suma.addSKom(QStringLiteral("Ogółem wartość - usługa dystrybucji:"));
        suma.addSKom(QString());
        suma.addSKom(QString());
        suma.addSKom(QString());
        suma.addSKom(QString());
        suma.addSKom(QString());
        suma.addSKom(QString());
        suma.addSKom(QStringLiteral("25.60"));
        suma.addSKom(QString());
        tab.setSuma(std::move(suma));

        blok.addTabela(std::move(tab));
    }

    Zalacznik zalacznik;
    zalacznik.addBlokDanych(std::move(blok));
    faktura.setZalacznik(std::move(zalacznik));
    faktura.setStopka(makeStopkaDefault());

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_25.xml"), *xml));
}

TEST(Examples, Przyklad_26)
{
    Naglowek naglowek{
        QDateTime{QDate{2026, 2, 1}, QTime{0, 0, 0}, QTimeZone::utc()},
        QStringLiteral("SamploFaktur")};

    Podmiot1 podmiot1{
        DaneIdentyfikacyjne{QStringLiteral("9999999999"), QStringLiteral("ABC sp. z o. o.")},
        Adres{QStringLiteral("PL"), QStringLiteral("ul. Kwiatowa 1, 00-001 Warszawa")}};

    Podmiot2 podmiot2{
        DaneIdentyfikacyjne2{NabywcaPolski{QStringLiteral("2222222222")}, QStringLiteral("Jan Kowalski")}};
    podmiot2.setAdres(Adres{QStringLiteral("PL"), QStringLiteral("Akacjowa 1, 00-001 Warszawa")});

    Fa fa{
        QStringLiteral("PLN"),
        QDate{2026, 2, 1},
        QStringLiteral("FA/2026/02/999"),
        QStringLiteral("31.5"),
        Adnotacje{},
        RodzajFaktury::Rodzaj::VAT};
    fa.setMiejsceWystawieniaFaktury(QStringLiteral("Warszawa"));
    fa.setDataDostawy(QDate{2026, 2, 1});
    fa.setVatObniz2(QStringLiteral("30"), QStringLiteral("1.5"));

    {
        FaWiersz w{1, false};
        w.setNazwaTowaru(QStringLiteral("Napój Owocowy 1.5l"));
        w.setMiara(QStringLiteral("szt."));
        w.setIlosc(QStringLiteral("10"));
        w.setCenaJednostkowa(QStringLiteral("3"), true);
        w.setWartoscNetto(QStringLiteral("30"));
        w.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(5)});
        fa.addFaWiersz(std::move(w));
    }

    internal::XmlVector<Obciazenia, 100> obcVec;
    obcVec.push_back(Obciazenia{QStringLiteral("5"), QStringLiteral("Kaucja (system kaucyjny) 10 x 0,50  PLN")});
    Rozliczenie rozliczenie{
        std::make_tuple(std::move(obcVec), QStringLiteral("5")),
        std::nullopt,
        true,
        QStringLiteral("36.5")};
    fa.setRozliczenie(std::move(rozliczenie));

    Faktura faktura{std::move(naglowek), std::move(podmiot1), std::move(podmiot2), std::move(fa)};

    auto xml = toXML(faktura);
    ASSERT_TRUE(xml.has_value()) << xml.error().toStdString();
    EXPECT_TRUE(compareXMLFile(EXAMPLES_DIR + QStringLiteral("FA3_przyklad_26.xml"), *xml, true));
}
