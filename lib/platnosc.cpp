#include "platnosc.hpp"
#include "internal/xml_helpers.hpp"

namespace ksef::FA3 {

Obciazenia::Obciazenia(QString kwota, QString powod) :
    _Kwota{std::move(kwota)},
    _Powod{std::move(powod)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> Obciazenia::elements() const
{
    return internal::elements_to_vector(
        this->_Kwota,
        this->_Powod
        );
}

Odliczenia::Odliczenia(QString kwota, QString powod) :
    _Kwota{std::move(kwota)},
    _Powod{std::move(powod)}
{

}

QVector<std::reference_wrapper<const internal::XmlNode>> Odliczenia::elements() const
{
    return internal::elements_to_vector(
        this->_Kwota,
        this->_Powod
        );
}

Rozliczenie::Rozliczenie(
    std::optional<std::tuple<internal::XmlVector<Obciazenia, 100>, QString> > obciazenia,
    std::optional<std::tuple<internal::XmlVector<Odliczenia, 100>, QString> > odliczenia,
    bool do_zaplaty,
    QString kwota) :
    _switch{do_zaplaty ? decltype(_switch){DoZaplaty{std::move(kwota)}}
                       : decltype(_switch){DoRozliczenia{std::move(kwota)}}}
{
    if (obciazenia) {
        auto [vec, suma] = std::move(*obciazenia);
        this->_Obciazenia = std::move(vec);
        this->_SumaObciazen.emplace(std::move(suma));
    }
    if (odliczenia) {
        auto [vec, suma] = std::move(*odliczenia);
        this->_Odliczenia = std::move(vec);
        this->_SumaOdliczen.emplace(std::move(suma));
    }
}

QVector<std::reference_wrapper<const internal::XmlNode>> Rozliczenie::elements() const {
    return internal::elements_to_vector(
        this->_Obciazenia,
        this->_SumaObciazen,
        this->_Odliczenia,
        this->_SumaOdliczen,
        this->_switch
        );
}

TerminOpis::TerminOpis(QString ilosc, QString jednostka, QString zdarzeniePoczatkowe) :
    _Ilosc{std::move(ilosc)},
    _Jednostka{std::move(jednostka)},
    _ZdarzeniePoczatkowe{std::move(zdarzeniePoczatkowe)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> TerminOpis::elements() const {
    return internal::elements_to_vector(
        this->_Ilosc,
        this->_Jednostka,
        this->_ZdarzeniePoczatkowe
        );
}

TerminPlatnosci::TerminPlatnosci(std::optional<QDate> termin, std::optional<TerminOpis> opis) :
    _Termin{std::move(termin).transform([](QDate d) {
        return internal::XmlDate<"Termin">{d};
    })},
    _TerminOpis{std::move(opis)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> TerminPlatnosci::elements() const
{
    return internal::elements_to_vector(
        this->_Termin,
        this->_TerminOpis
        );

}

ZaplataCzesciowa::ZaplataCzesciowa(QString kwota, QDate data, FormaPlatnosci::Forma formaPlatnosci) :
    _KwotaZaplatyCzesciowej{std::move(kwota)},
    _DataZaplatyCzesciowej{data},
    _switch{formaPlatnosci}
{
}

ZaplataCzesciowa::ZaplataCzesciowa(QString kwota, QDate data, QString opisPlatnosci) :
    _KwotaZaplatyCzesciowej{std::move(kwota)},
    _DataZaplatyCzesciowej{data},
    _switch{PlatnoscInna{std::move(opisPlatnosci)}}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> ZaplataCzesciowa::elements() const {
    return internal::elements_to_vector(
        this->_KwotaZaplatyCzesciowej,
        this->_DataZaplatyCzesciowej,
        this->_switch
        );
}

QVector<std::reference_wrapper<const internal::XmlNode>> TRachunekBankowy::elements() const
{
    return internal::elements_to_vector(
        this->_NrRB,
        this->_SWIFT,
        this->_RachunekWlasnyBanku,
        this->_NazwaBanku,
        this->_OpisRachunku
        );
}

TRachunekBankowy::TRachunekBankowy(QString nrRB, std::optional<QString> nazwaBanku, std::optional<QString> opisRachunku) :
    _NrRB{std::move(nrRB)},
    _NazwaBanku{std::move(nazwaBanku)},
    _OpisRachunku{std::move(opisRachunku)}
{
}

void TRachunekBankowy::setSWIFT(QString swift)
{
    this->_SWIFT.emplace(std::move(swift));
}

void TRachunekBankowy::setRachunekWlasnyBanku(RachunekWlasnyBanku::Typ typ)
{
    this->_RachunekWlasnyBanku.emplace(typ);
}

RachunekBankowy::RachunekBankowy(QString nrRB, std::optional<QString> nazwaBanku, std::optional<QString> opisRachunku) :
    _rachunekBankowy{std::move(nrRB), std::move(nazwaBanku), std::move(opisRachunku)}
{
}

void RachunekBankowy::setSWIFT(QString swift)
{
    this->_rachunekBankowy.setSWIFT(std::move(swift));
}

void RachunekBankowy::setRachunekWlasnyBanku(RachunekWlasnyBanku::Typ typ)
{
    this->_rachunekBankowy.setRachunekWlasnyBanku(typ);
}

QVector<std::reference_wrapper<const internal::XmlNode>> RachunekBankowy::elements() const
{
    return internal::elements_to_vector(
        this->_rachunekBankowy
        );
}

RachunekBankowyFaktora::RachunekBankowyFaktora(QString nrRB, std::optional<QString> nazwaBanku, std::optional<QString> opisRachunku) :
    _rachunekBankowy{std::move(nrRB), std::move(nazwaBanku), std::move(opisRachunku)}
{
}

void RachunekBankowyFaktora::setSWIFT(QString swift)
{
    this->_rachunekBankowy.setSWIFT(std::move(swift));
}

void RachunekBankowyFaktora::setRachunekWlasnyBanku(RachunekWlasnyBanku::Typ typ)
{
    this->_rachunekBankowy.setRachunekWlasnyBanku(typ);
}

QVector<std::reference_wrapper<const internal::XmlNode>> RachunekBankowyFaktora::elements() const
{
    return internal::elements_to_vector(
        this->_rachunekBankowy
        );
}

Skonto::Skonto(QString warunkiSkonta, QString wysokoscSkonta) :
    _WarunkiSkonta{std::move(warunkiSkonta)},
    _WysokoscSkonta{std::move(wysokoscSkonta)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> Skonto::elements() const
{
    return internal::elements_to_vector(
        this->_WarunkiSkonta,
        this->_WysokoscSkonta
        );
}

void Platnosc::setZaplacono(QDate dataZaplaty)
{
    this->_switchZaplata.emplace(Zaplacono{dataZaplaty});
}

void Platnosc::setZaplacono(bool zaplata_czesciowa, internal::XmlVector<ZaplataCzesciowa, 100> wplaty)
{
    this->_switchZaplata.emplace(ZnacznikZaplatyCzesciowej{zaplata_czesciowa, std::move(wplaty)});
}

std::expected<void, QString> Platnosc::addTerminPlatnosci(std::optional<QDate> terminPlatnosci, std::optional<TerminOpis> opis)
{
    return this->_TerminPlatnosci.push_back(
        TerminPlatnosci{terminPlatnosci, std::move(opis)}
        );
}

void Platnosc::setFormaPlatnosci(FormaPlatnosci::Forma forma)
{
    this->_switchFormaPlatnosci.emplace(forma);
}

void Platnosc::setFormaPlatnosci(QString innaFormaPlatnosci)
{
    this->_switchFormaPlatnosci.emplace(PlatnoscInna{std::move(innaFormaPlatnosci)});
}

std::expected<void, QString> Platnosc::addRachunekBankowy(RachunekBankowy rachunekBankowy)
{
    return this->_RachunekBankowy.push_back(std::move(rachunekBankowy));
}

std::expected<void, QString> Platnosc::addRachunekBankowyFaktora(RachunekBankowyFaktora rachunekBankowy)
{
    return this->_RachunekBankowyFaktora.push_back(std::move(rachunekBankowy));
}

void Platnosc::setSkonto(Skonto skonto)
{
    this->_Skonto.emplace(std::move(skonto));
}

void Platnosc::setLinkDoPlatnosci(QString link)
{
    this->_LinkDoPlatnosci.emplace(std::move(link));
}

void Platnosc::setIPKSeF(QString ipksef)
{
    this->_IPKSeF.emplace(std::move(ipksef));
}

QVector<std::reference_wrapper<const internal::XmlNode>> Platnosc::elements() const {
    return internal::elements_to_vector(
        this->_switchZaplata,
        this->_TerminPlatnosci,
        this->_switchFormaPlatnosci,
        this->_RachunekBankowy,
        this->_RachunekBankowyFaktora,
        this->_Skonto,
        this->_LinkDoPlatnosci,
        this->_IPKSeF
        );
}


Platnosc::ZnacznikZaplatyCzesciowej::ZnacznikZaplatyCzesciowej(bool zaplata_czesciowa, internal::XmlVector<ZaplataCzesciowa, 100> wplaty) :
    _ZnacznikZaplatyCzesciowej{zaplata_czesciowa},
    _ZaplataCzesciowa{std::move(wplaty)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode> > Platnosc::ZnacznikZaplatyCzesciowej::elements() const {
    return internal::elements_to_vector(
        _ZnacznikZaplatyCzesciowej,
        _ZaplataCzesciowa
        );
}


} // namespace ksef::FA3
