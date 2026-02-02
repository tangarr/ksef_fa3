#include "transport.hpp"
#include "internal/xml_helpers.hpp"

namespace ksef::FA3 {

AdresPrzewoznika::AdresPrzewoznika(QString kodKraju, QString adresL1, std::optional<QString> adresL2, std::optional<QString> gln)
    : TAdres(std::move(kodKraju), std::move(adresL1), std::move(adresL2), std::move(gln))
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> AdresPrzewoznika::elements() const
{
    return TAdres::elements();
}

Przewoznik::Przewoznik(DaneIdentyfikacyjne2 daneIdentyfikacyjne, AdresPrzewoznika adresPrzewoznika)
    : _DaneIdentyfikacyjne{std::move(daneIdentyfikacyjne)},
      _AdresPrzewoznika{std::move(adresPrzewoznika)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> Przewoznik::elements() const
{
    return internal::elements_to_vector(
        this->_DaneIdentyfikacyjne,
        this->_AdresPrzewoznika
    );
}

WysylkaZ::WysylkaZ(QString kodKraju, QString adresL1, std::optional<QString> adresL2, std::optional<QString> gln)
    : TAdres(std::move(kodKraju), std::move(adresL1), std::move(adresL2), std::move(gln))
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> WysylkaZ::elements() const
{
    return TAdres::elements();
}

WysylkaPrzez::WysylkaPrzez(QString kodKraju, QString adresL1, std::optional<QString> adresL2, std::optional<QString> gln)
    : TAdres(std::move(kodKraju), std::move(adresL1), std::move(adresL2), std::move(gln))
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> WysylkaPrzez::elements() const
{
    return TAdres::elements();
}

WysylkaDo::WysylkaDo(QString kodKraju, QString adresL1, std::optional<QString> adresL2, std::optional<QString> gln)
    : TAdres(std::move(kodKraju), std::move(adresL1), std::move(adresL2), std::move(gln))
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> WysylkaDo::elements() const
{
    return TAdres::elements();
}

Transport::Transport(
    std::variant<RodzajTransportu::Rodzaj, QString> transport,
    std::variant<OpisLadunku::Ladunek, QString> ladunek
    ) :
    _switchTransport{[&]() -> decltype(_switchTransport) {
        if (auto rodzaj = std::get_if<RodzajTransportu::Rodzaj>(&transport)) {
            return RodzajTransportu{*rodzaj};
        }
        return TransportInny{std::get<QString>(std::move(transport))};
    }()},
    _switchLadunek{[&]() -> decltype(_switchLadunek) {
        if (auto rodzaj = std::get_if<OpisLadunku::Ladunek>(&ladunek)) {
            return OpisLadunku{*rodzaj};
        }
        return LadunekInny{std::get<QString>(std::move(ladunek))};
    }()}
{
}

void Transport::setPrzewoznik(Przewoznik przewoznik)
{
    this->_Przewoznik.emplace(std::move(przewoznik));
}

void Transport::setNrZleceniaTransportu(QString nrZlecenia)
{
    this->_NrZleceniaTransportu.emplace(std::move(nrZlecenia));
}

void Transport::setJednostkaOpakowania(QString jednostka)
{
    this->_JednostkaOpakowania.emplace(std::move(jednostka));
}

void Transport::setDataGodzRozpTransportu(QDateTime dataGodz)
{
    this->_DataGodzRozpTransportu.emplace(std::move(dataGodz));
}

void Transport::setDataGodzZakTransportu(QDateTime dataGodz)
{
    this->_DataGodzZakTransportu.emplace(std::move(dataGodz));
}

void Transport::setWysylkaZ(WysylkaZ wysylkaZ)
{
    this->_WysylkaZ.emplace(std::move(wysylkaZ));
}

std::expected<void, QString> Transport::addWysylkaPrzez(WysylkaPrzez wysylkaPrzez)
{
    return this->_WysylkaPrzez.push_back(std::move(wysylkaPrzez));
}

void Transport::setWysylkaDo(WysylkaDo wysylkaDo)
{
    this->_WysylkaDo.emplace(std::move(wysylkaDo));
}

QVector<std::reference_wrapper<const internal::XmlNode>> Transport::elements() const
{
    return internal::elements_to_vector(
        this->_switchTransport,
        this->_Przewoznik,
        this->_NrZleceniaTransportu,
        this->_switchLadunek,
        this->_JednostkaOpakowania,
        this->_DataGodzRozpTransportu,
        this->_DataGodzZakTransportu,
        this->_WysylkaZ,
        this->_WysylkaPrzez,
        this->_WysylkaDo
    );
}

Umowy::Umowy(std::optional<QDate> dataUmowy, std::optional<QString> nrUmowy)
{
    if (dataUmowy.has_value())
        _DataUmowy.emplace(*dataUmowy);
    if (nrUmowy.has_value())
        _NrUmowy.emplace(std::move(*nrUmowy));
}

QVector<std::reference_wrapper<const internal::XmlNode>> Umowy::elements() const
{
    return internal::elements_to_vector(
        this->_DataUmowy,
        this->_NrUmowy
    );
}

ZamowieniaWT::ZamowieniaWT(std::optional<QDate> dataZamowienia, std::optional<QString> nrZamowienia)
{
    if (dataZamowienia.has_value())
        _DataZamowienia.emplace(*dataZamowienia);
    if (nrZamowienia.has_value())
        _NrZamowienia.emplace(std::move(*nrZamowienia));
}

QVector<std::reference_wrapper<const internal::XmlNode>> ZamowieniaWT::elements() const
{
    return internal::elements_to_vector(
        this->_DataZamowienia,
        this->_NrZamowienia
    );
}

std::expected<void, QString> WarunkiTransakcji::addUmowa(Umowy umowa)
{
    return this->_Umowy.push_back(std::move(umowa));
}

std::expected<void, QString> WarunkiTransakcji::addZamowienie(ZamowieniaWT zamowienie)
{
    return this->_Zamowienia.push_back(std::move(zamowienie));
}

std::expected<void, QString> WarunkiTransakcji::addNrPartiiTowaru(QString nrPartii)
{
    return this->_NrPartiiTowaru.push_back(internal::XmlString<"NrPartiiTowaru">{std::move(nrPartii)});
}

void WarunkiTransakcji::setWarunkiDostawy(QString warunki)
{
    this->_WarunkiDostawy.emplace(std::move(warunki));
}

void WarunkiTransakcji::setKursUmowny(QString kurs, QString waluta)
{
    this->_KursUmowny.emplace(std::move(kurs), std::move(waluta));
}

std::expected<void, QString> WarunkiTransakcji::addTransport(Transport transport)
{
    return this->_Transport.push_back(std::move(transport));
}

void WarunkiTransakcji::setPodmiotPosredniczacy()
{
    this->_PodmiotPosredniczacy.emplace();
}

QVector<std::reference_wrapper<const internal::XmlNode>> WarunkiTransakcji::elements() const
{
    return internal::elements_to_vector(
        this->_Umowy,
        this->_Zamowienia,
        this->_NrPartiiTowaru,
        this->_WarunkiDostawy,
        this->_KursUmowny,
        this->_Transport,
        this->_PodmiotPosredniczacy
    );
}

WarunkiTransakcji::KursUmowny::KursUmowny(QString kurs, QString waluta) :
    _KursUmowny{std::move(kurs)},
    _WalutaUmowna{std::move(waluta)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode> > WarunkiTransakcji::KursUmowny::elements() const
{
    return internal::elements_to_vector(
        this->_KursUmowny,
        this->_WalutaUmowna
        );
}

} // namespace ksef::FA3
