#include "typy_adresowe.hpp"
#include "internal/xml_helpers.hpp"

namespace ksef::FA3 {

TAdres::TAdres(QString kodKraju, QString adresL1, std::optional<QString> adresL2, std::optional<QString> gln) :
    _KodKraju{std::move(kodKraju)},
    _AdresL1{std::move(adresL1)},
    _AdresL2{std::move(adresL2)},
    _GLN{std::move(gln)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> TAdres::elements() const
{
    return internal::elements_to_vector(
        this->_KodKraju,
        this->_AdresL1,
        this->_AdresL2,
        this->_GLN
        );
}

Adres::Adres(QString kodKraju, QString adresL1, std::optional<QString> adresL2, std::optional<QString> gln):
    _adres{TAdres{std::move(kodKraju), std::move(adresL1), std::move(adresL2), std::move(gln)}}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> Adres::elements() const
{
    return internal::elements_to_vector(
        this->_adres
        );
}

AdresKoresp::AdresKoresp(QString kodKraju, QString adresL1, std::optional<QString> adresL2, std::optional<QString> gln):
    _adres{TAdres{std::move(kodKraju), std::move(adresL1), std::move(adresL2), std::move(gln)}}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> AdresKoresp::elements() const
{
    return internal::elements_to_vector(
        this->_adres
        );
}

DaneKontaktowe::DaneKontaktowe(std::optional<QString> email, std::optional<QString> telefon) :
    _Email{std::move(email)},
    _Telefon{std::move(telefon)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> DaneKontaktowe::elements() const
{
    return internal::elements_to_vector(
        this->_Email,
        this->_Telefon
        );
}

DaneKontaktowePU::DaneKontaktowePU(std::optional<QString> email, std::optional<QString> telefon) :
    _EmailPU{std::move(email)},
    _TelefonPU{std::move(telefon)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> DaneKontaktowePU::elements() const
{
    return internal::elements_to_vector(
        this->_EmailPU,
        this->_TelefonPU
        );
}

} // namespace ksef::FA3
