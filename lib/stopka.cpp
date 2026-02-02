#include "stopka.hpp"
#include "internal/xml_helpers.hpp"

namespace ksef::FA3 {

Informacje::Informacje(std::optional<QString> stopkaFaktury) :
    _StopkaFaktury{stopkaFaktury.transform([](QString s) {
        return internal::XmlString<"StopkaFaktury">{std::move(s)};
    })}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> Informacje::elements() const
{
    return internal::elements_to_vector(
        this->_StopkaFaktury
        );
}

void Rejestry::setPelnaNazwa(QString pelnaNazwa)
{
    this->_PelnaNazwa.emplace(std::move(pelnaNazwa));
}

void Rejestry::setKRS(QString krs)
{
    this->_KRS.emplace(std::move(krs));
}

void Rejestry::setREGON(QString regon)
{
    this->_REGON.emplace(std::move(regon));
}

void Rejestry::setBDO(QString bdo)
{
    this->_BDO.emplace(std::move(bdo));
}

QVector<std::reference_wrapper<const internal::XmlNode>> Rejestry::elements() const
{
    return internal::elements_to_vector(
        this->_PelnaNazwa,
        this->_KRS,
        this->_REGON,
        this->_BDO
        );
}

std::expected<void, QString> Stopka::addInformacje(Informacje informacje)
{
    return this->_Informacje.push_back(std::move(informacje));
}

std::expected<void, QString> Stopka::addRejestry(Rejestry rejestry)
{
    return this->_Rejestry.push_back(std::move(rejestry));
}

QVector<std::reference_wrapper<const internal::XmlNode>> Stopka::elements() const
{
    return internal::elements_to_vector(
        this->_Informacje,
        this->_Rejestry
        );
}

} // namespace ksef::FA3
