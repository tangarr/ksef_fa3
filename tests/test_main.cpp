#include <gtest/gtest.h>
#include "helpers.hpp"
#include <QTimeZone>

namespace ksef::FA3
{

    TEST(KSeF_FA3_internal, toXML)
    {
        TestNode testNode;
        auto result = toXML(testNode).value();
        auto expected = QStringLiteral(
            "<TestNode xmlns=\"DefaultNamespace\" xmlns:NamespacePrefix=\"NamespaceURI\" Attribute=\"AttributeValue\">\n"
            "   <ChildNode>\n"
            "       ChildValue\n"
            "   </ChildNode>\n"
            "</TestNode>\n");
        EXPECT_TRUE(compareXML(expected, result));
    }

    TEST(KSeF_FA3_internal, vector)
    {
        internal::XmlVector<internal::XmlString<"XmlString">, 2> vector;
        EXPECT_TRUE(vector.push_back(QStringLiteral("string1")).has_value());
        EXPECT_TRUE(vector.push_back(QStringLiteral("string2")).has_value());
        EXPECT_FALSE(vector.push_back(QStringLiteral("string3")).has_value());
    }

    TEST(KSeF_FA3, Naglowek)
    {
        auto expected = QStringLiteral(
            "<Naglowek>\n"
            "  <KodFormularza kodSystemowy=\"FA (3)\" wersjaSchemy=\"1-0E\">FA</KodFormularza>\n"
            "  <WariantFormularza>3</WariantFormularza>\n"
            "  <DataWytworzeniaFa>2026-02-01T00:00:00Z</DataWytworzeniaFa>\n"
            "  <SystemInfo>SamploFaktur</SystemInfo>\n"
            "</Naglowek>");
        Naglowek naglowek{QDateTime{QDate{2026, 02, 01}, QTime{}, QTimeZone::UTC}, QStringLiteral("SamploFaktur")};
        auto result = toXML(naglowek).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Podmiot1)
    {
        auto expected = QStringLiteral(
            "<Podmiot1>\n"
            "  <DaneIdentyfikacyjne>\n"
            "    <NIP>8882927357</NIP>\n"
            "    <Nazwa>ABC AGD sp. z o. o.</Nazwa>\n"
            "  </DaneIdentyfikacyjne>\n"
            "  <Adres>\n"
            "    <KodKraju>PL</KodKraju>\n"
            "    <AdresL1>ul. Kwiatowa 1 m. 2</AdresL1>\n"
            "    <AdresL2>00-001 Warszawa</AdresL2>\n"
            "  </Adres>\n"
            "  <DaneKontaktowe>\n"
            "    <Email>abc@abc.pl</Email>\n"
            "    <Telefon>667444555</Telefon>\n"
            "  </DaneKontaktowe>\n"
            "</Podmiot1>\n");
        Podmiot1 podmiot1{
            DaneIdentyfikacyjne{
                QStringLiteral("8882927357"),
                QStringLiteral("ABC AGD sp. z o. o.")},
            Adres{
                QStringLiteral("PL"),
                QStringLiteral("ul. Kwiatowa 1 m. 2"),
                QStringLiteral("00-001 Warszawa")}};
        EXPECT_TRUE(podmiot1.addDaneKontaktowe(
                                DaneKontaktowe{
                                    QStringLiteral("abc@abc.pl"),
                                    QStringLiteral("667444555")})
                        .has_value());
        auto result = toXML(podmiot1).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Podmiot2_NabywcaPolski)
    {
        auto expected = QStringLiteral(
            "<Podmiot2>\n"
            "  <DaneIdentyfikacyjne>\n"
            "    <NIP>1111111111</NIP>\n"
            "    <Nazwa>F.H.U. Jan Kowalski</Nazwa>\n"
            "  </DaneIdentyfikacyjne>\n"
            "  <Adres>\n"
            "    <KodKraju>PL</KodKraju>\n"
            "    <AdresL1>ul. Polna 1</AdresL1>\n"
            "    <AdresL2>00-001 Warszawa</AdresL2>\n"
            "  </Adres>\n"
            "  <DaneKontaktowe>\n"
            "    <Email>jan@kowalski.pl</Email>\n"
            "    <Telefon>555777999</Telefon>\n"
            "  </DaneKontaktowe>\n"
            "  <NrKlienta>fdfd778343</NrKlienta>\n"
            "  <JST>2</JST>\n"
            "  <GV>2</GV>\n"
            "</Podmiot2>\n");
        Podmiot2 podmiot2{
            DaneIdentyfikacyjne2{
                NabywcaPolski{QStringLiteral("1111111111")},
                QStringLiteral("F.H.U. Jan Kowalski")}};
        podmiot2.setAdres(Adres{
            QStringLiteral("PL"),
            QStringLiteral("ul. Polna 1"),
            QStringLiteral("00-001 Warszawa")});
        EXPECT_TRUE(podmiot2.addDaneKontaktowe(
                                DaneKontaktowe{
                                    QStringLiteral("jan@kowalski.pl"),
                                    QStringLiteral("555777999")})
                        .has_value());
        podmiot2.setNrKlienta(QStringLiteral("fdfd778343"));
        auto result = toXML(podmiot2).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Podmiot2_NabywcaUE)
    {
        auto expected = QStringLiteral(
            "<Podmiot2>\n"
            "  <DaneIdentyfikacyjne>\n"
            "    <KodUE>DE</KodUE>\n"
            "    <NrVatUE>999999999</NrVatUE>\n"
            "    <Nazwa>EFG GmbH</Nazwa>\n"
            "  </DaneIdentyfikacyjne>\n"
            "  <Adres>\n"
            "    <KodKraju>DE</KodKraju>\n"
            "    <AdresL1>Blümchenstraße 1</AdresL1>\n"
            "    <AdresL2>10999 Berlin</AdresL2>\n"
            "  </Adres>\n"
            "  <DaneKontaktowe>\n"
            "    <Email>johan@shmidt.de</Email>\n"
            "    <Telefon>555777999</Telefon>\n"
            "  </DaneKontaktowe>\n"
            "  <JST>2</JST>\n"
            "  <GV>2</GV>\n"
            "</Podmiot2>\n");
        Podmiot2 podmiot2{
            DaneIdentyfikacyjne2{
                NabywcaUE{QStringLiteral("DE"), QStringLiteral("999999999")},
                QStringLiteral("EFG GmbH")}};
        podmiot2.setAdres(Adres{
            QStringLiteral("DE"),
            QStringLiteral("Blümchenstraße 1"),
            QStringLiteral("10999 Berlin")});
        EXPECT_TRUE(podmiot2.addDaneKontaktowe(
                                DaneKontaktowe{
                                    QStringLiteral("johan@shmidt.de"),
                                    QStringLiteral("555777999")})
                        .has_value());
        auto result = toXML(podmiot2).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Podmiot2_Minimal)
    {
        auto expected = QStringLiteral(
            "<Podmiot2>\n"
            "  <DaneIdentyfikacyjne>\n"
            "    <NIP>1111111111</NIP>\n"
            "    <Nazwa>Klient</Nazwa>\n"
            "  </DaneIdentyfikacyjne>\n"
            "  <JST>2</JST>\n"
            "  <GV>2</GV>\n"
            "</Podmiot2>\n");
        Podmiot2 podmiot2{
            DaneIdentyfikacyjne2{
                NabywcaPolski{QStringLiteral("1111111111")},
                QStringLiteral("Klient")}};
        auto result = toXML(podmiot2).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Podmiot3_Faktor)
    {
        auto expected = QStringLiteral(
            "<Podmiot3>\n"
            "  <DaneIdentyfikacyjne>\n"
            "    <NIP>2222222222</NIP>\n"
            "    <Nazwa>Bank Bankowości Bankowej S. A. BBB Faktoring</Nazwa>\n"
            "  </DaneIdentyfikacyjne>\n"
            "  <Adres>\n"
            "    <KodKraju>PL</KodKraju>\n"
            "    <AdresL1>ul. Bankowa 1</AdresL1>\n"
            "    <AdresL2>00-003 Łódź</AdresL2>\n"
            "  </Adres>\n"
            "  <DaneKontaktowe>\n"
            "    <Email>bbb@efaktoring.pl</Email>\n"
            "    <Telefon>666888999</Telefon>\n"
            "  </DaneKontaktowe>\n"
            "  <Rola>1</Rola>\n"
            "</Podmiot3>\n");
        Podmiot3 podmiot3{
            DaneIdentyfikacyjne3{
                NabywcaPolski{QStringLiteral("2222222222")},
                QStringLiteral("Bank Bankowości Bankowej S. A. BBB Faktoring")},
            Rola::FAKTOR};
        podmiot3.setAdres(Adres{
            QStringLiteral("PL"),
            QStringLiteral("ul. Bankowa 1"),
            QStringLiteral("00-003 Łódź")});
        EXPECT_TRUE(podmiot3.addDaneKontaktowe(
                                DaneKontaktowe{
                                    QStringLiteral("bbb@efaktoring.pl"),
                                    QStringLiteral("666888999")})
                        .has_value());
        auto result = toXML(podmiot3).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Podmiot3_DodatkowyNabywca)
    {
        auto expected = QStringLiteral(
            "<Podmiot3>\n"
            "  <DaneIdentyfikacyjne>\n"
            "    <NIP>3333333333</NIP>\n"
            "    <Nazwa>F.H.U. Grażyna Kowalska</Nazwa>\n"
            "  </DaneIdentyfikacyjne>\n"
            "  <Adres>\n"
            "    <KodKraju>PL</KodKraju>\n"
            "    <AdresL1>ul. Polna 1</AdresL1>\n"
            "    <AdresL2>00-001 Warszawa</AdresL2>\n"
            "  </Adres>\n"
            "  <DaneKontaktowe>\n"
            "    <Email>jan@kowalski.pl</Email>\n"
            "    <Telefon>555777999</Telefon>\n"
            "  </DaneKontaktowe>\n"
            "  <Rola>4</Rola>\n"
            "  <Udzial>50</Udzial>\n"
            "</Podmiot3>\n");
        Podmiot3 podmiot3{
            DaneIdentyfikacyjne3{
                NabywcaPolski{QStringLiteral("3333333333")},
                QStringLiteral("F.H.U. Grażyna Kowalska")},
            Rola::DODATKOWY_NABYWCA};
        podmiot3.setAdres(Adres{
            QStringLiteral("PL"),
            QStringLiteral("ul. Polna 1"),
            QStringLiteral("00-001 Warszawa")});
        EXPECT_TRUE(podmiot3.addDaneKontaktowe(
                                DaneKontaktowe{
                                    QStringLiteral("jan@kowalski.pl"),
                                    QStringLiteral("555777999")})
                        .has_value());
        podmiot3.setUdzial(50);
        auto result = toXML(podmiot3).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Podmiot3_RolaInna)
    {
        auto expected = QStringLiteral(
            "<Podmiot3>\n"
            "  <DaneIdentyfikacyjne>\n"
            "    <NIP>4444444444</NIP>\n"
            "    <Nazwa>Pełnomocnik sp. z o.o.</Nazwa>\n"
            "  </DaneIdentyfikacyjne>\n"
            "  <RolaInna>1</RolaInna>\n"
            "  <OpisRoli>Pełnomocnik ds. faktur</OpisRoli>\n"
            "</Podmiot3>\n");
        Podmiot3 podmiot3{
            DaneIdentyfikacyjne3{
                NabywcaPolski{QStringLiteral("4444444444")},
                QStringLiteral("Pełnomocnik sp. z o.o.")},
            QStringLiteral("Pełnomocnik ds. faktur")};
        auto result = toXML(podmiot3).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, PodmiotUpowazniony)
    {
        auto expected = QStringLiteral(
            "<PodmiotUpowazniony>\n"
            "  <DaneIdentyfikacyjne>\n"
            "    <NIP>5555555555</NIP>\n"
            "    <Nazwa>Komornik Jan Nowak</Nazwa>\n"
            "  </DaneIdentyfikacyjne>\n"
            "  <Adres>\n"
            "    <KodKraju>PL</KodKraju>\n"
            "    <AdresL1>ul. Sądowa 10</AdresL1>\n"
            "    <AdresL2>00-950 Warszawa</AdresL2>\n"
            "  </Adres>\n"
            "  <DaneKontaktowe>\n"
            "    <EmailPU>komornik@example.pl</EmailPU>\n"
            "  </DaneKontaktowe>\n"
            "  <RolaPU>2</RolaPU>\n"
            "</PodmiotUpowazniony>\n");
        PodmiotUpowazniony pu{
            DaneIdentyfikacyjnePU{
                QStringLiteral("5555555555"),
                QStringLiteral("Komornik Jan Nowak")},
            Adres{
                QStringLiteral("PL"),
                QStringLiteral("ul. Sądowa 10"),
                QStringLiteral("00-950 Warszawa")},
            RolaPU::KOMORNIK_SADOWY};
        EXPECT_TRUE(pu.addDaneKontaktowe(
                          DaneKontaktowePU{
                              QStringLiteral("komornik@example.pl"),
                              std::nullopt})
                        .has_value());
        auto result = toXML(pu).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Stopka)
    {
        auto expected = QStringLiteral(
            "<Stopka>\n"
            "  <Informacje>\n"
            "    <StopkaFaktury>Kapiał zakładowy 5 000 000</StopkaFaktury>\n"
            "  </Informacje>\n"
            "  <Rejestry>\n"
            "    <KRS>0000099999</KRS>\n"
            "    <REGON>999999999</REGON>\n"
            "    <BDO>000099999</BDO>\n"
            "  </Rejestry>\n"
            "</Stopka>\n");
        Stopka stopka;
        EXPECT_TRUE(stopka.addInformacje(
                              Informacje{QStringLiteral("Kapiał zakładowy 5 000 000")})
                        .has_value());
        Rejestry rejestry;
        rejestry.setKRS(QStringLiteral("0000099999"));
        rejestry.setREGON(QStringLiteral("999999999"));
        rejestry.setBDO(QStringLiteral("000099999"));
        EXPECT_TRUE(stopka.addRejestry(std::move(rejestry)).has_value());
        auto result = toXML(stopka).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Zalacznik)
    {
        auto expected = QStringLiteral(
            "<Zalacznik>\n"
            "  <BlokDanych>\n"
            "    <MetaDane>\n"
            "      <ZKlucz>Miejsce poboru energii</ZKlucz>\n"
            "      <ZWartosc>ul. Polna 1, 00-001 Warszawa</ZWartosc>\n"
            "    </MetaDane>\n"
            "    <MetaDane>\n"
            "      <ZKlucz>Kod PPE</ZKlucz>\n"
            "      <ZWartosc>999999999999999999</ZWartosc>\n"
            "    </MetaDane>\n"
            "    <Tabela>\n"
            "      <Opis>Odczyty</Opis>\n"
            "      <TNaglowek>\n"
            "        <Kol Typ=\"txt\">\n"
            "          <NKom>Licznik/Strefa</NKom>\n"
            "        </Kol>\n"
            "        <Kol Typ=\"int\">\n"
            "          <NKom>Ilość</NKom>\n"
            "        </Kol>\n"
            "      </TNaglowek>\n"
            "      <Wiersz>\n"
            "        <WKom>całodobowa</WKom>\n"
            "        <WKom>20</WKom>\n"
            "      </Wiersz>\n"
            "      <Suma>\n"
            "        <SKom>Razem:</SKom>\n"
            "        <SKom>20</SKom>\n"
            "      </Suma>\n"
            "    </Tabela>\n"
            "  </BlokDanych>\n"
            "</Zalacznik>\n");

        TNaglowek naglowek;
        EXPECT_TRUE(naglowek.addKol(Kol{Kol::TypKolumny::TXT, QStringLiteral("Licznik/Strefa")}).has_value());
        EXPECT_TRUE(naglowek.addKol(Kol{Kol::TypKolumny::INT, QStringLiteral("Ilość")}).has_value());

        ZTabela tabela{std::move(naglowek)};
        tabela.setOpis(QStringLiteral("Odczyty"));

        ZWiersz wiersz;
        EXPECT_TRUE(wiersz.addWKom(QStringLiteral("całodobowa")).has_value());
        EXPECT_TRUE(wiersz.addWKom(QStringLiteral("20")).has_value());
        EXPECT_TRUE(tabela.addWiersz(std::move(wiersz)).has_value());

        ZSuma suma;
        EXPECT_TRUE(suma.addSKom(QStringLiteral("Razem:")).has_value());
        EXPECT_TRUE(suma.addSKom(QStringLiteral("20")).has_value());
        tabela.setSuma(std::move(suma));

        BlokDanych blok;
        EXPECT_TRUE(blok.addMetaDane(ZMetaDane{
                                         QStringLiteral("Miejsce poboru energii"),
                                         QStringLiteral("ul. Polna 1, 00-001 Warszawa")})
                        .has_value());
        EXPECT_TRUE(blok.addMetaDane(ZMetaDane{
                                         QStringLiteral("Kod PPE"),
                                         QStringLiteral("999999999999999999")})
                        .has_value());
        EXPECT_TRUE(blok.addTabela(std::move(tabela)).has_value());

        Zalacznik zalacznik;
        EXPECT_TRUE(zalacznik.addBlokDanych(std::move(blok)).has_value());

        auto result = toXML(zalacznik).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Zwolnienie_Brak)
    {
        auto expected = QStringLiteral(
            "<Zwolnienie>\n"
            "  <P_19N>1</P_19N>\n"
            "</Zwolnienie>\n");
        Zwolnienie zwolnienie;
        auto result = toXML(zwolnienie).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Zwolnienie_VAT)
    {
        auto expected = QStringLiteral(
            "<Zwolnienie>\n"
            "  <P_19>1</P_19>\n"
            "  <P_19A>art. 43 ust. 1 pkt 37 ustawy VAT</P_19A>\n"
            "</Zwolnienie>\n");
        Zwolnienie zwolnienie{
            Zwolnienie::Podstawa::USTAWA_O_VAT,
            QStringLiteral("art. 43 ust. 1 pkt 37 ustawy VAT")};
        auto result = toXML(zwolnienie).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, NoweSrodkiTransportu_Nie)
    {
        auto expected = QStringLiteral(
            "<NoweSrodkiTransportu>\n"
            "  <P_22N>1</P_22N>\n"
            "</NoweSrodkiTransportu>\n");
        NoweSrodkiTransportu nst;
        auto result = toXML(nst).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, PMarzy_Brak)
    {
        auto expected = QStringLiteral(
            "<PMarzy>\n"
            "  <P_PMarzyN>1</P_PMarzyN>\n"
            "</PMarzy>\n");
        PMarzy pmarzy;
        auto result = toXML(pmarzy).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, PMarzy_TowaryUzywane)
    {
        auto expected = QStringLiteral(
            "<PMarzy>\n"
            "  <P_PMarzy>1</P_PMarzy>\n"
            "  <P_PMarzy_3_1>1</P_PMarzy_3_1>\n"
            "</PMarzy>\n");
        PMarzy pmarzy{PMarzy::ProceduraMarzy::TOWARY_UZYWANE};
        auto result = toXML(pmarzy).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Adnotacje_Default)
    {
        auto expected = QStringLiteral(
            "<Adnotacje>\n"
            "  <P_16>2</P_16>\n"
            "  <P_17>2</P_17>\n"
            "  <P_18>2</P_18>\n"
            "  <P_18A>2</P_18A>\n"
            "  <Zwolnienie>\n"
            "    <P_19N>1</P_19N>\n"
            "  </Zwolnienie>\n"
            "  <NoweSrodkiTransportu>\n"
            "    <P_22N>1</P_22N>\n"
            "  </NoweSrodkiTransportu>\n"
            "  <P_23>2</P_23>\n"
            "  <PMarzy>\n"
            "    <P_PMarzyN>1</P_PMarzyN>\n"
            "  </PMarzy>\n"
            "</Adnotacje>\n");
        Adnotacje adnotacje;
        auto result = toXML(adnotacje).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Adnotacje_ZwolnienieVAT_PMarzyTowary)
    {
        auto expected = QStringLiteral(
            "<Adnotacje>\n"
            "  <P_16>2</P_16>\n"
            "  <P_17>2</P_17>\n"
            "  <P_18>2</P_18>\n"
            "  <P_18A>2</P_18A>\n"
            "  <Zwolnienie>\n"
            "    <P_19>1</P_19>\n"
            "    <P_19A>art. 43 ust. 1 pkt 37 ustawy VAT</P_19A>\n"
            "  </Zwolnienie>\n"
            "  <NoweSrodkiTransportu>\n"
            "    <P_22N>1</P_22N>\n"
            "  </NoweSrodkiTransportu>\n"
            "  <P_23>2</P_23>\n"
            "  <PMarzy>\n"
            "    <P_PMarzy>1</P_PMarzy>\n"
            "    <P_PMarzy_3_1>1</P_PMarzy_3_1>\n"
            "  </PMarzy>\n"
            "</Adnotacje>\n");
        Adnotacje adnotacje{
            {},
            Zwolnienie{Zwolnienie::Podstawa::USTAWA_O_VAT, QStringLiteral("art. 43 ust. 1 pkt 37 ustawy VAT")},
            NoweSrodkiTransportu{},
            PMarzy{PMarzy::ProceduraMarzy::TOWARY_UZYWANE}};
        auto result = toXML(adnotacje).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, FaWiersz_Basic)
    {
        auto expected = QStringLiteral(
            "<FaWiersz>\n"
            "  <NrWierszaFa>1</NrWierszaFa>\n"
            "  <UU_ID>aaaa111133339990</UU_ID>\n"
            "  <P_7>lodówka Zimnotech mk1</P_7>\n"
            "  <P_8A>szt.</P_8A>\n"
            "  <P_8B>1</P_8B>\n"
            "  <P_9A>1626.01</P_9A>\n"
            "  <P_11>1626.01</P_11>\n"
            "  <P_12>23</P_12>\n"
            "</FaWiersz>\n");
        FaWiersz wiersz{1, false};
        wiersz.setUU_ID(QStringLiteral("aaaa111133339990"));
        wiersz.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        wiersz.setMiara(QStringLiteral("szt."));
        wiersz.setIlosc(QStringLiteral("1"));
        wiersz.setCenaJednostkowa(QStringLiteral("1626.01"), true);
        wiersz.setWartoscNetto(QStringLiteral("1626.01"));
        wiersz.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        auto result = toXML(wiersz).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, FaWiersz_StanPrzed)
    {
        auto expected = QStringLiteral(
            "<FaWiersz>\n"
            "  <NrWierszaFa>1</NrWierszaFa>\n"
            "  <UU_ID>aaaa111133339990</UU_ID>\n"
            "  <P_7>lodówka Zimnotech mk1</P_7>\n"
            "  <P_8A>szt.</P_8A>\n"
            "  <P_8B>1</P_8B>\n"
            "  <P_9A>1626.01</P_9A>\n"
            "  <P_11>1626.01</P_11>\n"
            "  <P_12>23</P_12>\n"
            "  <StanPrzed>1</StanPrzed>\n"
            "</FaWiersz>\n");
        FaWiersz wiersz{1, true};
        wiersz.setUU_ID(QStringLiteral("aaaa111133339990"));
        wiersz.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        wiersz.setMiara(QStringLiteral("szt."));
        wiersz.setIlosc(QStringLiteral("1"));
        wiersz.setCenaJednostkowa(QStringLiteral("1626.01"), true);
        wiersz.setWartoscNetto(QStringLiteral("1626.01"));
        wiersz.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        auto result = toXML(wiersz).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, FaWiersz_Marza_GTU)
    {
        auto expected = QStringLiteral(
            "<FaWiersz>\n"
            "  <NrWierszaFa>1</NrWierszaFa>\n"
            "  <P_7>samochód używany marki Autex rocznik 2010</P_7>\n"
            "  <P_8A>szt.</P_8A>\n"
            "  <P_8B>1</P_8B>\n"
            "  <P_9B>15000</P_9B>\n"
            "  <P_11A>15000</P_11A>\n"
            "  <GTU>GTU_07</GTU>\n"
            "</FaWiersz>\n");
        FaWiersz wiersz{1, false};
        wiersz.setNazwaTowaru(QStringLiteral("samochód używany marki Autex rocznik 2010"));
        wiersz.setMiara(QStringLiteral("szt."));
        wiersz.setIlosc(QStringLiteral("1"));
        wiersz.setCenaJednostkowa(QStringLiteral("15000"), false);
        wiersz.setWartoscBrutto(QStringLiteral("15000"));
        wiersz.setGTU(7);
        auto result = toXML(wiersz).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, FaWiersz_CN_P6A)
    {
        auto expected = QStringLiteral(
            "<FaWiersz>\n"
            "  <NrWierszaFa>2</NrWierszaFa>\n"
            "  <UU_ID>aaaa111133339991</UU_ID>\n"
            "  <P_7>zamrażarka Zimnotech mk2</P_7>\n"
            "  <CN>8418 40 20</CN>\n"
            "  <P_8A>szt.</P_8A>\n"
            "  <P_8B>20</P_8B>\n"
            "  <P_9A>1000</P_9A>\n"
            "  <P_11>18000</P_11>\n"
            "  <P_12>23</P_12>\n"
            "</FaWiersz>\n");
        FaWiersz wiersz{2, false};
        wiersz.setUU_ID(QStringLiteral("aaaa111133339991"));
        wiersz.setNazwaTowaru(QStringLiteral("zamrażarka Zimnotech mk2"));
        wiersz.setCN(QStringLiteral("8418 40 20"));
        wiersz.setMiara(QStringLiteral("szt."));
        wiersz.setIlosc(QStringLiteral("20"));
        wiersz.setCenaJednostkowa(QStringLiteral("1000"), true);
        wiersz.setWartoscNetto(QStringLiteral("18000"));
        wiersz.setStawkaVAT(FaWiersz::P_12{static_cast<unsigned char>(23)});
        auto result = toXML(wiersz).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, FaWiersz_Zwolnienie)
    {
        auto expected = QStringLiteral(
            "<FaWiersz>\n"
            "  <NrWierszaFa>2</NrWierszaFa>\n"
            "  <UU_ID>aaaa111133339991</UU_ID>\n"
            "  <P_7>pakiet ubezpieczeń za 01/2026</P_7>\n"
            "  <P_8A>szt.</P_8A>\n"
            "  <P_8B>1</P_8B>\n"
            "  <P_9A>300</P_9A>\n"
            "  <P_11>300</P_11>\n"
            "  <P_12>zw</P_12>\n"
            "</FaWiersz>\n");
        FaWiersz wiersz{2, false};
        wiersz.setUU_ID(QStringLiteral("aaaa111133339991"));
        wiersz.setNazwaTowaru(QStringLiteral("pakiet ubezpieczeń za 01/2026"));
        wiersz.setMiara(QStringLiteral("szt."));
        wiersz.setIlosc(QStringLiteral("1"));
        wiersz.setCenaJednostkowa(QStringLiteral("300"), true);
        wiersz.setWartoscNetto(QStringLiteral("300"));
        wiersz.setStawkaVAT(FaWiersz::P_12{FaWiersz::P_12::Stawka::ZW});
        auto result = toXML(wiersz).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, DaneFaKorygowanej_KSeF)
    {
        auto expected = QStringLiteral(
            "<DaneFaKorygowanej>\n"
            "  <DataWystFaKorygowanej>2026-02-15</DataWystFaKorygowanej>\n"
            "  <NrFaKorygowanej>FV2026/02/150</NrFaKorygowanej>\n"
            "  <NrKSeF>1</NrKSeF>\n"
            "  <NrKSeFFaKorygowanej>9999999999-20230908-8BEF280C8D35-4D</NrKSeFFaKorygowanej>\n"
            "</DaneFaKorygowanej>\n");
        DaneFaKorygowanej dane{
            QDate{2026, 2, 15},
            QStringLiteral("FV2026/02/150"),
            QStringLiteral("9999999999-20230908-8BEF280C8D35-4D")};
        auto result = toXML(dane).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, DaneFaKorygowanej_BezKSeF)
    {
        auto expected = QStringLiteral(
            "<DaneFaKorygowanej>\n"
            "  <DataWystFaKorygowanej>2026-02-15</DataWystFaKorygowanej>\n"
            "  <NrFaKorygowanej>FV2026/02/150</NrFaKorygowanej>\n"
            "  <NrKSeFN>1</NrKSeFN>\n"
            "</DaneFaKorygowanej>\n");
        DaneFaKorygowanej dane{
            QDate{2026, 2, 15},
            QStringLiteral("FV2026/02/150")};
        auto result = toXML(dane).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Podmiot1K)
    {
        auto expected = QStringLiteral(
            "<Podmiot1K>\n"
            "  <DaneIdentyfikacyjne>\n"
            "    <NIP>8882927357</NIP>\n"
            "    <Nazwa>ABC AGD sp. z o. o.</Nazwa>\n"
            "  </DaneIdentyfikacyjne>\n"
            "  <Adres>\n"
            "    <KodKraju>PL</KodKraju>\n"
            "    <AdresL1>ul. Kwiatowa 1 m. 2</AdresL1>\n"
            "    <AdresL2>00-001 Warszawa</AdresL2>\n"
            "  </Adres>\n"
            "</Podmiot1K>\n");
        Podmiot1K podmiot1k{
            DaneIdentyfikacyjne{
                QStringLiteral("8882927357"),
                QStringLiteral("ABC AGD sp. z o. o.")},
            Adres{
                QStringLiteral("PL"),
                QStringLiteral("ul. Kwiatowa 1 m. 2"),
                QStringLiteral("00-001 Warszawa")},
            std::nullopt};
        auto result = toXML(podmiot1k).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Podmiot2K)
    {
        auto expected = QStringLiteral(
            "<Podmiot2K>\n"
            "  <DaneIdentyfikacyjne>\n"
            "    <NIP>1111111111</NIP>\n"
            "    <Nazwa>F.H.U. Jan Kowalski</Nazwa>\n"
            "  </DaneIdentyfikacyjne>\n"
            "  <Adres>\n"
            "    <KodKraju>PL</KodKraju>\n"
            "    <AdresL1>ul. Polna 1</AdresL1>\n"
            "    <AdresL2>00-001 Warszawa</AdresL2>\n"
            "  </Adres>\n"
            "</Podmiot2K>\n");
        Podmiot2K podmiot2k{
            DaneIdentyfikacyjne2{
                NabywcaPolski{QStringLiteral("1111111111")},
                QStringLiteral("F.H.U. Jan Kowalski")},
            Adres{
                QStringLiteral("PL"),
                QStringLiteral("ul. Polna 1"),
                QStringLiteral("00-001 Warszawa")},
            std::nullopt};
        auto result = toXML(podmiot2k).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, OkresFA)
    {
        auto expected = QStringLiteral(
            "<OkresFa>\n"
            "  <P_6_Od>2026-01-01</P_6_Od>\n"
            "  <P_6_Do>2026-01-01</P_6_Do>\n"
            "</OkresFa>\n");
        OkresFA okres{QDate{2026, 1, 1}, QDate{2026, 1, 1}};
        auto result = toXML(okres).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, DodatkowyOpis)
    {
        auto expected = QStringLiteral(
            "<DodatkowyOpis>\n"
            "  <Klucz>preferowane godziny dowozu</Klucz>\n"
            "  <Wartosc>dni robocze 17:00 - 20:00</Wartosc>\n"
            "</DodatkowyOpis>\n");
        DodatkowyOpis opis{std::nullopt, QStringLiteral("preferowane godziny dowozu"), QStringLiteral("dni robocze 17:00 - 20:00")};
        auto result = toXML(opis).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, ZaliczkaCzesciowa)
    {
        auto expected = QStringLiteral(
            "<ZaliczkaCzesciowa>\n"
            "  <P_6Z>2026-01-15</P_6Z>\n"
            "  <P_15Z>5000</P_15Z>\n"
            "</ZaliczkaCzesciowa>\n");
        ZaliczkaCzesciowa zaliczka{QDate{2026, 1, 15}, QStringLiteral("5000"), std::nullopt};
        auto result = toXML(zaliczka).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, FakturaZaliczkowa_KSeF)
    {
        auto expected = QStringLiteral(
            "<FakturaZaliczkowa>\n"
            "  <NrKSeFFaZaliczkowej>9999999999-20230908-8BEF280C8D35-4D</NrKSeFFaZaliczkowej>\n"
            "</FakturaZaliczkowa>\n");
        FakturaZaliczkowa fz{false, QStringLiteral("9999999999-20230908-8BEF280C8D35-4D")};
        auto result = toXML(fz).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, FakturaZaliczkowa_PozaKSeF)
    {
        auto expected = QStringLiteral(
            "<FakturaZaliczkowa>\n"
            "  <NrKSeFZN>1</NrKSeFZN>\n"
            "  <NrFaZaliczkowej>FZ2026/01/001</NrFaZaliczkowej>\n"
            "</FakturaZaliczkowa>\n");
        FakturaZaliczkowa fz{true, QStringLiteral("FZ2026/01/001")};
        auto result = toXML(fz).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Platnosc_Zaplacono)
    {
        auto expected = QStringLiteral(
            "<Platnosc>\n"
            "  <Zaplacono>1</Zaplacono>\n"
            "  <DataZaplaty>2026-01-27</DataZaplaty>\n"
            "  <FormaPlatnosci>6</FormaPlatnosci>\n"
            "</Platnosc>\n");
        Platnosc platnosc;
        platnosc.setZaplacono(QDate{2026, 1, 27});
        platnosc.setFormaPlatnosci(FormaPlatnosci::Forma::PRZELEW);
        auto result = toXML(platnosc).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Platnosc_Termin_RachunekBankowy)
    {
        auto expected = QStringLiteral(
            "<Platnosc>\n"
            "  <TerminPlatnosci>\n"
            "    <Termin>2026-03-15</Termin>\n"
            "  </TerminPlatnosci>\n"
            "  <FormaPlatnosci>6</FormaPlatnosci>\n"
            "  <RachunekBankowy>\n"
            "    <NrRB>73111111111111111111111111</NrRB>\n"
            "    <NazwaBanku>Bank Bankowości Bankowej S. A.</NazwaBanku>\n"
            "    <OpisRachunku>PLN</OpisRachunku>\n"
            "  </RachunekBankowy>\n"
            "</Platnosc>\n");
        Platnosc platnosc;
        EXPECT_TRUE(platnosc.addTerminPlatnosci(QDate{2026, 3, 15}, std::nullopt).has_value());
        platnosc.setFormaPlatnosci(FormaPlatnosci::Forma::PRZELEW);
        EXPECT_TRUE(platnosc.addRachunekBankowy(
                                RachunekBankowy{
                                    QStringLiteral("73111111111111111111111111"),
                                    QStringLiteral("Bank Bankowości Bankowej S. A."),
                                    QStringLiteral("PLN")})
                        .has_value());
        auto result = toXML(platnosc).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Platnosc_RachunekBankowyFaktora)
    {
        auto expected = QStringLiteral(
            "<Platnosc>\n"
            "  <TerminPlatnosci>\n"
            "    <Termin>2026-03-15</Termin>\n"
            "  </TerminPlatnosci>\n"
            "  <FormaPlatnosci>6</FormaPlatnosci>\n"
            "  <RachunekBankowyFaktora>\n"
            "    <NrRB>73111111111111111111111111</NrRB>\n"
            "    <RachunekWlasnyBanku>2</RachunekWlasnyBanku>\n"
            "    <NazwaBanku>Bank Bankowości Bankowej S. A.</NazwaBanku>\n"
            "    <OpisRachunku>PLN</OpisRachunku>\n"
            "  </RachunekBankowyFaktora>\n"
            "</Platnosc>\n");
        Platnosc platnosc;
        EXPECT_TRUE(platnosc.addTerminPlatnosci(QDate{2026, 3, 15}, std::nullopt).has_value());
        platnosc.setFormaPlatnosci(FormaPlatnosci::Forma::PRZELEW);
        RachunekBankowyFaktora rbf{
            QStringLiteral("73111111111111111111111111"),
            QStringLiteral("Bank Bankowości Bankowej S. A."),
            QStringLiteral("PLN")};
        rbf.setRachunekWlasnyBanku(RachunekWlasnyBanku::Typ::TYP_2);
        EXPECT_TRUE(platnosc.addRachunekBankowyFaktora(std::move(rbf)).has_value());
        auto result = toXML(platnosc).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Platnosc_ZaplataCzesciowa)
    {
        auto expected = QStringLiteral(
            "<Platnosc>\n"
            "  <ZnacznikZaplatyCzesciowej>1</ZnacznikZaplatyCzesciowej>\n"
            "  <ZaplataCzesciowa>\n"
            "    <KwotaZaplatyCzesciowej>10000</KwotaZaplatyCzesciowej>\n"
            "    <DataZaplatyCzesciowej>2026-01-27</DataZaplatyCzesciowej>\n"
            "    <FormaPlatnosci>1</FormaPlatnosci>\n"
            "  </ZaplataCzesciowa>\n"
            "  <TerminPlatnosci>\n"
            "    <TerminOpis>\n"
            "      <Ilosc>30</Ilosc>\n"
            "      <Jednostka>Dzień</Jednostka>\n"
            "      <ZdarzeniePoczatkowe>Otrzymanie faktury</ZdarzeniePoczatkowe>\n"
            "    </TerminOpis>\n"
            "  </TerminPlatnosci>\n"
            "  <FormaPlatnosci>6</FormaPlatnosci>\n"
            "  <RachunekBankowy>\n"
            "    <NrRB>73111111111111111111111111</NrRB>\n"
            "    <NazwaBanku>Bank Bankowości Bankowej S. A.</NazwaBanku>\n"
            "    <OpisRachunku>PLN</OpisRachunku>\n"
            "  </RachunekBankowy>\n"
            "</Platnosc>\n");
        Platnosc platnosc;
        internal::XmlVector<ZaplataCzesciowa, 100> wplaty;
        EXPECT_TRUE(wplaty.push_back(
                              ZaplataCzesciowa{
                                  QStringLiteral("10000"),
                                  QDate{2026, 1, 27},
                                  FormaPlatnosci::Forma::GOTOWKA})
                        .has_value());
        platnosc.setZaplacono(true, std::move(wplaty));
        EXPECT_TRUE(platnosc.addTerminPlatnosci(
                                std::nullopt,
                                TerminOpis{
                                    QStringLiteral("30"),
                                    QStringLiteral("Dzień"),
                                    QStringLiteral("Otrzymanie faktury")})
                        .has_value());
        platnosc.setFormaPlatnosci(FormaPlatnosci::Forma::PRZELEW);
        EXPECT_TRUE(platnosc.addRachunekBankowy(
                                RachunekBankowy{
                                    QStringLiteral("73111111111111111111111111"),
                                    QStringLiteral("Bank Bankowości Bankowej S. A."),
                                    QStringLiteral("PLN")})
                        .has_value());
        auto result = toXML(platnosc).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Rozliczenie_Odliczenia)
    {
        auto expected = QStringLiteral(
            "<Rozliczenie>\n"
            "  <Odliczenia>\n"
            "    <Kwota>1000</Kwota>\n"
            "    <Powod>nadwyżka salda nierozliczonych środków</Powod>\n"
            "  </Odliczenia>\n"
            "  <SumaOdliczen>1000</SumaOdliczen>\n"
            "  <DoZaplaty>63279.92</DoZaplaty>\n"
            "</Rozliczenie>\n");
        internal::XmlVector<Odliczenia, 100> odliczenia;
        EXPECT_TRUE(odliczenia.push_back(
                                  Odliczenia{
                                      QStringLiteral("1000"),
                                      QStringLiteral("nadwyżka salda nierozliczonych środków")})
                        .has_value());
        Rozliczenie rozliczenie{
            std::nullopt,
            std::make_tuple(std::move(odliczenia), QStringLiteral("1000")),
            true,
            QStringLiteral("63279.92")};
        auto result = toXML(rozliczenie).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Transport_Full)
    {
        auto expected = QStringLiteral(
            "<Transport>\n"
            "  <RodzajTransportu>3</RodzajTransportu>\n"
            "  <Przewoznik>\n"
            "    <DaneIdentyfikacyjne>\n"
            "      <NIP>6666666666</NIP>\n"
            "      <Nazwa>Jan Nowak Transport</Nazwa>\n"
            "    </DaneIdentyfikacyjne>\n"
            "    <AdresPrzewoznika>\n"
            "      <KodKraju>PL</KodKraju>\n"
            "      <AdresL1>ul. Bukowa 5</AdresL1>\n"
            "      <AdresL2>00-004 Poznań</AdresL2>\n"
            "    </AdresPrzewoznika>\n"
            "  </Przewoznik>\n"
            "  <OpisLadunku>13</OpisLadunku>\n"
            "  <JednostkaOpakowania>a</JednostkaOpakowania>\n"
            "  <WysylkaZ>\n"
            "    <KodKraju>PL</KodKraju>\n"
            "    <AdresL1>Sadowa 1 lok. 2</AdresL1>\n"
            "    <AdresL2>00-001 Warszawa</AdresL2>\n"
            "  </WysylkaZ>\n"
            "  <WysylkaDo>\n"
            "    <KodKraju>PL</KodKraju>\n"
            "    <AdresL1>ul. Sadowa 1 lok. 3</AdresL1>\n"
            "    <AdresL2>00-002 Kraków</AdresL2>\n"
            "  </WysylkaDo>\n"
            "</Transport>\n");
        Transport transport{
            RodzajTransportu::DROGOWY,
            OpisLadunku::PALETA};
        transport.setPrzewoznik(Przewoznik{
            DaneIdentyfikacyjne2{
                NabywcaPolski{QStringLiteral("6666666666")},
                QStringLiteral("Jan Nowak Transport")},
            AdresPrzewoznika{
                QStringLiteral("PL"),
                QStringLiteral("ul. Bukowa 5"),
                QStringLiteral("00-004 Poznań")}});
        transport.setJednostkaOpakowania(QStringLiteral("a"));
        transport.setWysylkaZ(WysylkaZ{
            QStringLiteral("PL"),
            QStringLiteral("Sadowa 1 lok. 2"),
            QStringLiteral("00-001 Warszawa")});
        transport.setWysylkaDo(WysylkaDo{
            QStringLiteral("PL"),
            QStringLiteral("ul. Sadowa 1 lok. 3"),
            QStringLiteral("00-002 Kraków")});
        auto result = toXML(transport).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, WarunkiTransakcji_Simple)
    {
        auto expected = QStringLiteral(
            "<WarunkiTransakcji>\n"
            "  <Zamowienia>\n"
            "    <DataZamowienia>2026-01-26</DataZamowienia>\n"
            "    <NrZamowienia>4354343</NrZamowienia>\n"
            "  </Zamowienia>\n"
            "</WarunkiTransakcji>\n");
        WarunkiTransakcji wt;
        EXPECT_TRUE(wt.addZamowienie(
                          ZamowieniaWT{QDate{2026, 1, 26}, QStringLiteral("4354343")})
                        .has_value());
        auto result = toXML(wt).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, WarunkiTransakcji_Full)
    {
        auto expected = QStringLiteral(
            "<WarunkiTransakcji>\n"
            "  <Zamowienia>\n"
            "    <DataZamowienia>2026-01-26</DataZamowienia>\n"
            "    <NrZamowienia>4354343</NrZamowienia>\n"
            "  </Zamowienia>\n"
            "  <NrPartiiTowaru>2312323/2026</NrPartiiTowaru>\n"
            "  <WarunkiDostawy>CIP</WarunkiDostawy>\n"
            "  <Transport>\n"
            "    <RodzajTransportu>3</RodzajTransportu>\n"
            "    <Przewoznik>\n"
            "      <DaneIdentyfikacyjne>\n"
            "        <NIP>6666666666</NIP>\n"
            "        <Nazwa>Jan Nowak Transport</Nazwa>\n"
            "      </DaneIdentyfikacyjne>\n"
            "      <AdresPrzewoznika>\n"
            "        <KodKraju>PL</KodKraju>\n"
            "        <AdresL1>ul. Bukowa 5</AdresL1>\n"
            "        <AdresL2>00-004 Poznań</AdresL2>\n"
            "      </AdresPrzewoznika>\n"
            "    </Przewoznik>\n"
            "    <OpisLadunku>13</OpisLadunku>\n"
            "    <JednostkaOpakowania>a</JednostkaOpakowania>\n"
            "    <WysylkaZ>\n"
            "      <KodKraju>PL</KodKraju>\n"
            "      <AdresL1>Sadowa 1 lok. 2</AdresL1>\n"
            "      <AdresL2>00-001 Warszawa</AdresL2>\n"
            "    </WysylkaZ>\n"
            "    <WysylkaDo>\n"
            "      <KodKraju>PL</KodKraju>\n"
            "      <AdresL1>ul. Sadowa 1 lok. 3</AdresL1>\n"
            "      <AdresL2>00-002 Kraków</AdresL2>\n"
            "    </WysylkaDo>\n"
            "  </Transport>\n"
            "</WarunkiTransakcji>\n");
        WarunkiTransakcji wt;
        EXPECT_TRUE(wt.addZamowienie(
                          ZamowieniaWT{QDate{2026, 1, 26}, QStringLiteral("4354343")})
                        .has_value());
        EXPECT_TRUE(wt.addNrPartiiTowaru(QStringLiteral("2312323/2026")).has_value());
        wt.setWarunkiDostawy(QStringLiteral("CIP"));
        Transport transport{
            RodzajTransportu::DROGOWY,
            OpisLadunku::PALETA};
        transport.setPrzewoznik(Przewoznik{
            DaneIdentyfikacyjne2{
                NabywcaPolski{QStringLiteral("6666666666")},
                QStringLiteral("Jan Nowak Transport")},
            AdresPrzewoznika{
                QStringLiteral("PL"),
                QStringLiteral("ul. Bukowa 5"),
                QStringLiteral("00-004 Poznań")}});
        transport.setJednostkaOpakowania(QStringLiteral("a"));
        transport.setWysylkaZ(WysylkaZ{
            QStringLiteral("PL"),
            QStringLiteral("Sadowa 1 lok. 2"),
            QStringLiteral("00-001 Warszawa")});
        transport.setWysylkaDo(WysylkaDo{
            QStringLiteral("PL"),
            QStringLiteral("ul. Sadowa 1 lok. 3"),
            QStringLiteral("00-002 Kraków")});
        EXPECT_TRUE(wt.addTransport(std::move(transport)).has_value());
        auto result = toXML(wt).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, ZamowienieWiersz_Basic)
    {
        auto expected = QStringLiteral(
            "<ZamowienieWiersz>\n"
            "  <NrWierszaZam>1</NrWierszaZam>\n"
            "  <P_7Z>lodówka Zimnotech mk1</P_7Z>\n"
            "  <P_8AZ>szt.</P_8AZ>\n"
            "  <P_8BZ>5</P_8BZ>\n"
            "  <P_9AZ>1626.01</P_9AZ>\n"
            "  <P_11NettoZ>8130.05</P_11NettoZ>\n"
            "  <P_12Z>23</P_12Z>\n"
            "</ZamowienieWiersz>\n");
        ZamowienieWiersz wiersz{1};
        wiersz.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        wiersz.setMiara(QStringLiteral("szt."));
        wiersz.setIlosc(QStringLiteral("5"));
        wiersz.setCenaJednostkowa(QStringLiteral("1626.01"));
        wiersz.setWartoscNetto(QStringLiteral("8130.05"));
        wiersz.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(23)});
        auto result = toXML(wiersz).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, ZamowienieWiersz_StanPrzedZ)
    {
        auto expected = QStringLiteral(
            "<ZamowienieWiersz>\n"
            "  <NrWierszaZam>1</NrWierszaZam>\n"
            "  <P_7Z>lodówka Zimnotech mk1</P_7Z>\n"
            "  <P_8AZ>szt.</P_8AZ>\n"
            "  <P_8BZ>5</P_8BZ>\n"
            "  <StanPrzedZ>1</StanPrzedZ>\n"
            "</ZamowienieWiersz>\n");
        ZamowienieWiersz wiersz{1, true};
        wiersz.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        wiersz.setMiara(QStringLiteral("szt."));
        wiersz.setIlosc(QStringLiteral("5"));
        auto result = toXML(wiersz).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

    TEST(KSeF_FA3, Zamowienie)
    {
        auto expected = QStringLiteral(
            "<Zamowienie>\n"
            "  <WartoscZamowienia>8130.05</WartoscZamowienia>\n"
            "  <ZamowienieWiersz>\n"
            "    <NrWierszaZam>1</NrWierszaZam>\n"
            "    <P_7Z>lodówka Zimnotech mk1</P_7Z>\n"
            "    <P_8AZ>szt.</P_8AZ>\n"
            "    <P_8BZ>5</P_8BZ>\n"
            "    <P_9AZ>1626.01</P_9AZ>\n"
            "    <P_11NettoZ>8130.05</P_11NettoZ>\n"
            "    <P_12Z>23</P_12Z>\n"
            "  </ZamowienieWiersz>\n"
            "</Zamowienie>\n");
        Zamowienie zamowienie{QStringLiteral("8130.05")};
        ZamowienieWiersz wiersz{1};
        wiersz.setNazwaTowaru(QStringLiteral("lodówka Zimnotech mk1"));
        wiersz.setMiara(QStringLiteral("szt."));
        wiersz.setIlosc(QStringLiteral("5"));
        wiersz.setCenaJednostkowa(QStringLiteral("1626.01"));
        wiersz.setWartoscNetto(QStringLiteral("8130.05"));
        wiersz.setStawkaVAT(ZamowienieWiersz::P_12Z{static_cast<unsigned char>(23)});
        EXPECT_TRUE(zamowienie.addWiersz(std::move(wiersz)).has_value());
        auto result = toXML(zamowienie).value();
        EXPECT_TRUE(compareXML(expected, result)) << result.toStdString();
    }

}
