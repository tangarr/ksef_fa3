#include "podmioty.hpp"
#include "internal/xml_helpers.hpp"

namespace ksef::FA3 {

Podmiot1::Podmiot1(DaneIdentyfikacyjne daneIdentyfikacyjne, Adres adres) :
    _DaneIdentyfikacyjne{std::move(daneIdentyfikacyjne)},
    _Adres{std::move(adres)}
{
}

void Podmiot1::setPrefiksPodatnika(QString prefiksPodatnika)
{
    this->_PrefiksPodatnika.emplace(std::move(prefiksPodatnika));
}

void Podmiot1::setNrEORI(QString nrEORI)
{
    this->_NrEORI.emplace(std::move(nrEORI));
}

void Podmiot1::setAdresKoresp(AdresKoresp adresKoresp)
{
    this->_AdresKoresp.emplace(std::move(adresKoresp));
}

std::expected<void, QString> Podmiot1::addDaneKontaktowe(DaneKontaktowe daneKontaktowe)
{
    return this->_DaneKontaktowe.push_back(std::move(daneKontaktowe));
}

void Podmiot1::setStatusInfoPodatnika(StatusInfoPodatnika::Status status)
{
    this->_StatusInfoPodatnika = status;
}

QVector<std::reference_wrapper<const internal::XmlNode>> Podmiot1::elements() const
{
    return internal::elements_to_vector(
        this->_PrefiksPodatnika,
        this->_NrEORI,
        this->_DaneIdentyfikacyjne,
        this->_Adres,
        this->_AdresKoresp,
        this->_DaneKontaktowe,
        this->_StatusInfoPodatnika
        );
}

Podmiot2::Podmiot2(DaneIdentyfikacyjne2 daneIdentyfikacyjne, bool jst, bool gv):
    _DaneIdentyfikacyjne{std::move(daneIdentyfikacyjne)},
    _JST{jst ? JST::JEDNOSTKA_SAMORZADU_TERYTORIALNEGO : JST::NIE_DOTYCZY},
    _GV{gv ? GV::GRUPA_VAT : GV::NIE_DOTYCZY}
{
}

void Podmiot2::setNrEORI(QString nrEORI)
{
    this->_NrEORI.emplace(std::move(nrEORI));
}

void Podmiot2::setAdres(Adres adres)
{
    this->_Adres.emplace(std::move(adres));
}

void Podmiot2::setAdresKoresp(AdresKoresp adresKoresp)
{
    this->_AdresKoresp = std::move(adresKoresp);
}

std::expected<void, QString> Podmiot2::addDaneKontaktowe(DaneKontaktowe daneKontaktowe)
{
    return this->_DaneKontaktowe.push_back(std::move(daneKontaktowe));
}

void Podmiot2::setNrKlienta(QString nrKlienta)
{
    this->_NrKlienta.emplace(std::move(nrKlienta));
}

void Podmiot2::setIDNabywcy(QString idNabywcy)
{
    this->_IDNabywcy.emplace(std::move(idNabywcy));
}

QVector<std::reference_wrapper<const internal::XmlNode>> Podmiot2::elements() const
{
    return internal::elements_to_vector(
        this->_NrEORI,
        this->_DaneIdentyfikacyjne,
        this->_Adres,
        this->_AdresKoresp,
        this->_DaneKontaktowe,
        this->_NrKlienta,
        this->_IDNabywcy,
        this->_JST,
        this->_GV
        );
}

Podmiot3::Podmiot3(DaneIdentyfikacyjne3 daneIdentyfikacyjne, Rola::Typ rola) :
    _DaneIdentyfikacyjne{std::move(daneIdentyfikacyjne)},
    _switch{Rola{rola}}
{
}

Podmiot3::Podmiot3(DaneIdentyfikacyjne3 daneIdentyfikacyjne, QString opisRoli) :
    _DaneIdentyfikacyjne{std::move(daneIdentyfikacyjne)},
    _switch{RolaInna{std::move(opisRoli)}}
{
}

void Podmiot3::setNrEORI(QString nrEORI)
{
    this->_NrEORI.emplace(std::move(nrEORI));
}

void Podmiot3::setAdres(Adres adres)
{
    this->_Adres.emplace(std::move(adres));
}

void Podmiot3::setAdresKoresp(AdresKoresp adresKoresp)
{
    this->_AdresKoresp.emplace(std::move(adresKoresp));
}

std::expected<void, QString> Podmiot3::addDaneKontaktowe(DaneKontaktowe daneKontaktowe)
{
    return this->_DaneKontaktowe.push_back(std::move(daneKontaktowe));
}

void Podmiot3::setNrKlienta(QString nrKlienta)
{
    this->_NrKlienta.emplace(std::move(nrKlienta));
}

void Podmiot3::setIDNabywcy(QString idNabywcy)
{
    this->_IDNabywcy.emplace(std::move(idNabywcy));
}

void Podmiot3::setUdzial(unsigned char udzial)
{
    this->_Udzial.emplace(QString::number(udzial));

}

QVector<std::reference_wrapper<const internal::XmlNode>> Podmiot3::elements() const
{
    return internal::elements_to_vector(
        this->_IDNabywcy,
        this->_NrEORI,
        this->_DaneIdentyfikacyjne,
        this->_Adres,
        this->_AdresKoresp,
        this->_DaneKontaktowe,
        this->_switch,
        this->_Udzial,
        this->_NrKlienta
        );
}

DaneIdentyfikacyjnePU::DaneIdentyfikacyjnePU(QString nip, QString nazwa) :
    _NIP{std::move(nip)},
    _Nazwa{std::move(nazwa)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> DaneIdentyfikacyjnePU::elements() const
{
    return internal::elements_to_vector(
        this->_NIP,
        this->_Nazwa
        );

}

PodmiotUpowazniony::PodmiotUpowazniony(DaneIdentyfikacyjnePU daneIdentyfikacyjne, Adres adres, RolaPU::Typ rola) :
    _DaneIdentyfikacyjne{std::move(daneIdentyfikacyjne)},
    _Adres{std::move(adres)},
    _RolaPU{rola}
{
}

void PodmiotUpowazniony::setNrEORI(QString nrEORI)
{
    this->_NrEORI.emplace(std::move(nrEORI));
}

void PodmiotUpowazniony::setAdresKoresp(AdresKoresp adresKoresp)
{
    this->_AdresKoresp.emplace(std::move(adresKoresp));
}

std::expected<void, QString> PodmiotUpowazniony::addDaneKontaktowe(DaneKontaktowePU daneKontaktowe)
{
    return this->_DaneKontaktowe.push_back(std::move(daneKontaktowe));
}

QVector<std::reference_wrapper<const internal::XmlNode>> PodmiotUpowazniony::elements() const
{
    return internal::elements_to_vector(
        this->_NrEORI,
        this->_DaneIdentyfikacyjne,
        this->_Adres,
        this->_AdresKoresp,
        this->_DaneKontaktowe,
        this->_RolaPU
        );

}


Rola::Rola(Typ rola) :
    _Rola{ToString(rola)}
{

}

QVector<std::reference_wrapper<const internal::XmlNode> > Rola::elements() const
{
    return {
        _Rola
    };
}


} // namespace ksef::FA3
