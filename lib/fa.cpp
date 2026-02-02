#include "fa.hpp"
#include "internal/xml_helpers.hpp"

namespace ksef::FA3 {

OkresFA::OkresFA(QDate okres_od, QDate okres_do) :
    _P_6_Od{okres_od},
    _P_6_Do{okres_do}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> OkresFA::elements() const
{
    return internal::elements_to_vector(
        this->_P_6_Od,
        this->_P_6_Do
        );
}

DaneFaKorygowanej::DaneFaKorygowanej(QDate dataWystFaKorygowanej, QString nrFaKorygowanej) :
    _DataWystFaKorygowanej{dataWystFaKorygowanej},
    _NrFaKorygowanej{std::move(nrFaKorygowanej)},
    _switch{NrKSeFN{}}
{
}

DaneFaKorygowanej::DaneFaKorygowanej(QDate dataWystFaKorygowanej, QString nrFaKorygowanej, QString nrKSeFFaKorygowanej) :
    _DataWystFaKorygowanej{dataWystFaKorygowanej},
    _NrFaKorygowanej{std::move(nrFaKorygowanej)},
    _switch{NrKSeF{std::move(nrKSeFFaKorygowanej)}}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> DaneFaKorygowanej::elements() const
{
    return internal::elements_to_vector(
        this->_DataWystFaKorygowanej,
        this->_NrFaKorygowanej,
        this->_switch
        );
}


Podmiot1K::Podmiot1K(DaneIdentyfikacyjne daneIdentyfikacyjne, Adres adres, std::optional<QString> prefiksPodatnika) :
    _PrefiksPodatnika{std::move(prefiksPodatnika).transform([](QString s) {
        return internal::XmlString<"PrefiksPodatnika">{std::move(s)};
    })},
    _DaneIdentyfikacyjne{std::move(daneIdentyfikacyjne)},
    _Adres{std::move(adres)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> Podmiot1K::elements() const {
    return internal::elements_to_vector(
        this->_PrefiksPodatnika,
        this->_DaneIdentyfikacyjne,
        this->_Adres
        );

}

Podmiot2K::Podmiot2K(DaneIdentyfikacyjne2 daneIdentyfikacyjne, std::optional<Adres> adres, std::optional<QString> idNabywcy) :
    _DaneIdentyfikacyjne{std::move(daneIdentyfikacyjne)},
    _Adres{std::move(adres)},
    _IDNabywcy{std::move(idNabywcy).transform([](QString s) {
        return internal::XmlString<"IDNabywcy">{std::move(s)};
    })}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> Podmiot2K::elements() const {
    return internal::elements_to_vector(
        this->_DaneIdentyfikacyjne,
        this->_Adres,
        this->_IDNabywcy
        );
}

ZaliczkaCzesciowa::ZaliczkaCzesciowa(QDate data, QString kwota, std::optional<QString> kurs_wymiany) :
    _P_6Z{data},
    _P_15Z{std::move(kwota)},
    _KursWalutyZW{std::move(kurs_wymiany).transform([](QString s) {
        return internal::XmlDecimal<"KursWalutyZW">{std::move(s)};
    })}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> ZaliczkaCzesciowa::elements() const {
    return internal::elements_to_vector(
        this->_P_6Z,
        this->_P_15Z,
        this->_KursWalutyZW
        );
}

DodatkowyOpis::DodatkowyOpis(std::optional<QString> nrWiersza, QString klucz, QString wartosc) :
    _NrWiersza{std::move(nrWiersza).transform([](QString s) {
        return internal::XmlString<"NrWiersza">{std::move(s)};
    })},
    _Klucz{std::move(klucz)},
    _Wartosc{std::move(wartosc)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> DodatkowyOpis::elements() const {
    return internal::elements_to_vector(
        this->_NrWiersza,
        this->_Klucz,
        this->_Wartosc
        );
}

FakturaZaliczkowa::FakturaZaliczkowa(bool wystawiona_poza_ksef, QString nrFaZaliczkowej) :
    _switch{[&]() -> decltype(_switch) {
            if (wystawiona_poza_ksef) {
                return NrKSeFZN{std::move(nrFaZaliczkowej)};
            }
            return NrKSeFFaZaliczkowej{std::move(nrFaZaliczkowej)};
        }()
    }
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> FakturaZaliczkowa::elements() const {
    return internal::elements_to_vector(
        this->_switch
        );
}

FaWiersz::FaWiersz(unsigned int numerWiersza, bool stan_przed_korekty) :
    _NrWierszaFa{QString::number(numerWiersza)}
{
    if (stan_przed_korekty) {
        this->_StanPrzed.emplace();
    }
}

void FaWiersz::setUU_ID(QString UU_ID)
{
    this->_UU_ID.emplace(std::move(UU_ID));
}

void FaWiersz::setNazwaTowaru(QString nazwa)
{
    this->_P_7.emplace(std::move(nazwa));
}

void FaWiersz::setIndeks(QString indeks)
{
    this->_Indeks.emplace(std::move(indeks));
}

void FaWiersz::setPKWiU(QString pkwiu)
{
    this->_PKWiU.emplace(std::move(pkwiu));
}

void FaWiersz::setCN(QString cn)
{
    this->_CN.emplace(std::move(cn));
}

void FaWiersz::setMiara(QString miara)
{
    this->_P_8A.emplace(std::move(miara));
}

void FaWiersz::setIlosc(QString ilosc)
{
    this->_P_8B.emplace(std::move(ilosc));
}

void FaWiersz::setCenaJednostkowa(QString cena_jednostkowa, bool netto)
{
    if (netto) {
        this->_P_9A.emplace(std::move(cena_jednostkowa));
    }
    else {
        this->_P_9B.emplace(std::move(cena_jednostkowa));
    }
}

void FaWiersz::setWartoscNetto(QString wartosc)
{
    this->_P_11.emplace(std::move(wartosc));
}

void FaWiersz::setWartoscBrutto(QString wartosc)
{
    this->_P_11A.emplace(std::move(wartosc));
}

void FaWiersz::setKwotaVAT(QString kwota)
{
    this->_P_11Vat.emplace(std::move(kwota));
}

void FaWiersz::setStawkaVAT(P_12 stawka_vat)
{
    this->_P_12.emplace(std::move(stawka_vat));
}

void FaWiersz::setP_6A(QDate date)
{
    this->_P_6A.emplace(date);
}

void FaWiersz::setRabat(QString kwota)
{
    this->_P_10.emplace(std::move(kwota));
}

void FaWiersz::setKursWaluty(QString kurs)
{
    this->_KursWaluty.emplace(std::move(kurs));
}

void FaWiersz::setGTU(unsigned char gtu)
{
    this->_GTU.emplace(gtu);
}

void FaWiersz::setProcedura(Procedura::Symbol procedura)
{
    this->_Procedura.emplace(procedura);
}

void FaWiersz::setKwotaAkcyzy(QString kwota)
{
    this->_KwotaAkcyzy.emplace(std::move(kwota));
}

QVector<std::reference_wrapper<const internal::XmlNode>> FaWiersz::elements() const {
    return internal::elements_to_vector(
        this->_NrWierszaFa,
        this->_UU_ID,
        this->_P_6A,
        this->_P_7,
        this->_Indeks,
        this->_GTIN,
        this->_PKWiU,
        this->_CN,
        this->_PKOB,
        this->_P_8A,
        this->_P_8B,
        this->_P_9A,
        this->_P_9B,
        this->_P_10,
        this->_P_11,
        this->_P_11A,
        this->_P_11Vat,
        this->_P_12,
        this->_P_12_XII,
        this->_P_12_Zal_15,
        this->_KwotaAkcyzy,
        this->_GTU,
        this->_Procedura,
        this->_KursWaluty,
        this->_StanPrzed
        );
}

Fa::Fa(QString kodWaluty,
       QDate data,
       QString numerFaktury,
       QString wartosc,
       Adnotacje adnotacje,
       RodzajFaktury::Rodzaj rodzaj
       ) :
    _KodWaluty{std::move(kodWaluty)},
    _P_1{data},
    _P_2{std::move(numerFaktury)},
    _P_15{std::move(wartosc)},
    _Adnotacje{std::move(adnotacje)},
    _RodzajFaktury{rodzaj}
{
}

void Fa::setMiejsceWystawieniaFaktury(QString miejsce) {
    this->_P_1M.emplace(std::move(miejsce));
}

std::expected<void, QString> Fa::addWZ(QString numerWZ)
{
    return this->_WZ.push_back(std::move(numerWZ));
}

void Fa::setDataDostawy(QDate dataDostawy)
{
    this->_DataDostawy.emplace(P_6{dataDostawy});
}

void Fa::setDataDostawy(QDate dostawa_od, QDate dostawa_do)
{
    this->_DataDostawy.emplace(internal::XmlSingleton<OkresFA>{dostawa_od, dostawa_do});
}

void Fa::setVatPodst(QString netto, QString vat, std::optional<QString> vat_pln)
{
    this->_VatPodst.emplace(std::move(netto), std::move(vat), std::move(vat_pln));
}

void Fa::setVatObniz1(QString netto, QString vat, std::optional<QString> vat_pln)
{
    this->_VatObniz1.emplace(std::move(netto), std::move(vat), std::move(vat_pln));
}

void Fa::setVatObniz2(QString netto, QString vat, std::optional<QString> vat_pln)
{
    this->_VatObniz2.emplace(std::move(netto), std::move(vat), std::move(vat_pln));
}

void Fa::setVatRyczaltTaxi(QString netto, QString vat, std::optional<QString> vat_pln)
{
    this->_VatRyczaltTaxi.emplace(std::move(netto), std::move(vat), std::move(vat_pln));
}

void Fa::setVatProcSzczeg(QString netto, std::optional<QString> vat)
{
    this->_VatProcSzczeg.emplace(std::move(netto), std::move(vat));
}

void Fa::setP_13_6_1(QString kwota)
{
    this->_P_13_6_1.emplace(std::move(kwota));
}

void Fa::setP_13_6_2(QString kwota)
{
    this->_P_13_6_2.emplace(std::move(kwota));
}

void Fa::setP_13_6_3(QString kwota)
{
    this->_P_13_6_3.emplace(std::move(kwota));
}

void Fa::setP_13_7(QString kwota)
{
    this->_P_13_7.emplace(std::move(kwota));
}

void Fa::setP_13_8(QString kwota)
{
    this->_P_13_8.emplace(std::move(kwota));
}

void Fa::setP_13_9(QString kwota)
{
    this->_P_13_9.emplace(std::move(kwota));
}

void Fa::setP_13_10(QString kwota)
{
    this->_P_13_10.emplace(std::move(kwota));
}

void Fa::setP_13_11(QString kwota)
{
    this->_P_13_11.emplace(std::move(kwota));
}

void Fa::setKursWalutyZ(QString kurs)
{
    this->_KursWalutyZ.emplace(std::move(kurs));
}

void Fa::setKorekta(Korekta korekta)
{
    this->_Korekta.emplace(std::move(korekta));
}

std::expected<void, QString> Fa::addZaliczkaCzesciowa(ZaliczkaCzesciowa zaliczka)
{
    return this->_ZaliczkaCzesciowa.push_back(std::move(zaliczka));
}

void Fa::setFP()
{
    this->_FP.emplace();
}

void Fa::setTP()
{
    this->_TP.emplace();
}

std::expected<void, QString> Fa::addDodatkowyOpis(DodatkowyOpis opis)
{
    return this->_DodatkowyOpis.push_back(std::move(opis));
}

std::expected<void, QString> Fa::addFakturaZaliczkowa(FakturaZaliczkowa fz)
{
    return this->_FakturaZaliczkowa.push_back(std::move(fz));
}

void Fa::setZwrotAkcyzy()
{
    this->_ZwrotAkcyzy.emplace();
}

std::expected<void, QString> Fa::addFaWiersz(FaWiersz wiersz)
{
    return this->_FaWiersz.push_back(std::move(wiersz));
}

void Fa::setRozliczenie(Rozliczenie rozliczenie)
{
    this->_Rozliczenie.emplace(std::move(rozliczenie));
}

void Fa::setPlatnosc(Platnosc platnosc)
{
    this->_Platnosc.emplace(std::move(platnosc));
}

void Fa::setWarunkiTransakcji(WarunkiTransakcji wt)
{
    this->_WarunkiTransakcji.emplace(std::move(wt));
}

void Fa::setZamowienie(Zamowienie zamowienie)
{
    this->_Zamowienie.emplace(std::move(zamowienie));
}

QVector<std::reference_wrapper<const internal::XmlNode>> Fa::elements() const {    
    return internal::elements_to_vector(
        this->_KodWaluty,
        this->_P_1,
        this->_P_1M,
        this->_P_2,
        this->_WZ,
        this->_DataDostawy,
        this->_VatPodst,
        this->_VatObniz1,
        this->_VatObniz2,
        this->_VatRyczaltTaxi,
        this->_VatProcSzczeg,
        this->_P_13_6_1,
        this->_P_13_6_2,
        this->_P_13_6_3,
        this->_P_13_7,
        this->_P_13_8,
        this->_P_13_9,
        this->_P_13_10,
        this->_P_13_11,
        this->_P_15,
        this->_KursWalutyZ,
        this->_Adnotacje,
        this->_RodzajFaktury,
        this->_Korekta,
        this->_ZaliczkaCzesciowa,
        this->_FP,
        this->_TP,
        this->_DodatkowyOpis,
        this->_FakturaZaliczkowa,
        this->_ZwrotAkcyzy,
        this->_FaWiersz,
        this->_Rozliczenie,
        this->_Platnosc,
        this->_WarunkiTransakcji,
        this->_Zamowienie
        );
}

void Korekta::setPrzyczynaKorekty(QString przyczyna)
{
    this->_PrzyczynaKorekty.emplace(std::move(przyczyna));
}

void Korekta::setTypKorekty(TypKorekty::Typ typ)
{
    this->_TypKorekty.emplace(typ);
}

std::expected<void, QString> Korekta::addDaneFaKorygowanej(DaneFaKorygowanej dane)
{
    return this->_DaneFaKorygowanej.push_back(std::move(dane));
}

void Korekta::setOkresFaKorygowanej(QString okres)
{
    this->_OkresFaKorygowanej.emplace(std::move(okres));
}

void Korekta::setNrFaKorygowany(QString nr)
{
    this->_NrFaKorygowany.emplace(std::move(nr));
}

void Korekta::setPodmiot1K(Podmiot1K podmiot)
{
    this->_Podmiot1K.emplace(std::move(podmiot));
}

std::expected<void, QString> Korekta::addPodmiot2K(Podmiot2K podmiot)
{
    return this->_Podmiot2K.push_back(std::move(podmiot));
}

void Korekta::setZaliczka(Zaliczka zaliczka)
{
    this->_Zaliczka.emplace(std::move(zaliczka));
}

QVector<std::reference_wrapper<const internal::XmlNode> > Korekta::elements() const
{
    return internal::elements_to_vector(
        this->_PrzyczynaKorekty,
        this->_TypKorekty,
        this->_DaneFaKorygowanej,
        this->_OkresFaKorygowanej,
        this->_NrFaKorygowany,
        this->_Podmiot1K,
        this->_Podmiot2K,
        this->_Zaliczka
        );
}

Korekta::Zaliczka::Zaliczka(QString kwota, std::optional<QString> kursWaluty) :
    _P_15ZK{std::move(kwota)},
    _KursWalutyZK{std::move(kursWaluty)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode> > Korekta::Zaliczka::elements() const
{
    return internal::elements_to_vector(
        this->_P_15ZK,
        this->_KursWalutyZK
        );
}


ZamowienieWiersz::ZamowienieWiersz(unsigned int numerWiersza, bool stan_przed_korekty) :
    _NrWierszaZam{QString::number(numerWiersza)}
{
    if (stan_przed_korekty) {
        this->_StanPrzedZ.emplace();
    }
}

void ZamowienieWiersz::setUU_IDZ(QString uuid)
{
    this->_UU_IDZ.emplace(std::move(uuid));
}

void ZamowienieWiersz::setNazwaTowaru(QString nazwa)
{
    this->_P_7Z.emplace(std::move(nazwa));
}

void ZamowienieWiersz::setIndeks(QString indeks)
{
    this->_IndeksZ.emplace(std::move(indeks));
}

void ZamowienieWiersz::setPKWiU(QString pkwiu)
{
    this->_PKWiUZ.emplace(std::move(pkwiu));
}

void ZamowienieWiersz::setMiara(QString miara)
{
    this->_P_8AZ.emplace(std::move(miara));
}

void ZamowienieWiersz::setIlosc(QString ilosc)
{
    this->_P_8BZ.emplace(std::move(ilosc));
}

void ZamowienieWiersz::setCenaJednostkowa(QString cena)
{
    this->_P_9AZ.emplace(std::move(cena));
}

void ZamowienieWiersz::setWartoscNetto(QString wartosc)
{
    this->_P_11NettoZ.emplace(std::move(wartosc));
}

void ZamowienieWiersz::setKwotaVAT(QString kwota)
{
    this->_P_11VatZ.emplace(std::move(kwota));
}

void ZamowienieWiersz::setStawkaVAT(P_12Z stawka)
{
    this->_P_12Z.emplace(std::move(stawka));
}

void ZamowienieWiersz::setKwotaAkcyzy(QString kwota)
{
    this->_KwotaAkcyzyZ.emplace(std::move(kwota));
}

QVector<std::reference_wrapper<const internal::XmlNode>> ZamowienieWiersz::elements() const {
    return internal::elements_to_vector(
        this->_NrWierszaZam,
        this->_UU_IDZ,
        this->_P_7Z,
        this->_IndeksZ,
        this->_GTINZ,
        this->_PKWiUZ,
        this->_CNZ,
        this->_PKOBZ,
        this->_P_8AZ,
        this->_P_8BZ,
        this->_P_9AZ,
        this->_P_11NettoZ,
        this->_P_11VatZ,
        this->_P_12Z,
        this->_P_12Z_XII,
        this->_P_12Z_Zal_15,
        this->_GTUZ,
        this->_ProceduraZ,
        this->_KwotaAkcyzyZ,
        this->_StanPrzedZ
        );
}

Zamowienie::Zamowienie(QString wartoscZamowienia) :
    _WartoscZamowienia{std::move(wartoscZamowienia)}
{
}

std::expected<void, QString> Zamowienie::addWiersz(ZamowienieWiersz wiersz)
{
    return this->_ZamowienieWiersz.push_back(std::move(wiersz));
}

QVector<std::reference_wrapper<const internal::XmlNode>> Zamowienie::elements() const {
    return internal::elements_to_vector(
        this->_WartoscZamowienia,
        this->_ZamowienieWiersz
        );
}

} // namespace ksef::FA3
