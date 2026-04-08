#include "typy_identyfikacyjne.hpp"
#include "internal/xml_helpers.hpp"

namespace ksef::FA3 {

DaneIdentyfikacyjne::DaneIdentyfikacyjne(QString nip, QString nazwa) :
    _NIP{std::move(nip)},
    _Nazwa{std::move(nazwa)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> DaneIdentyfikacyjne::elements() const
{
    return internal::elements_to_vector(
        this->_NIP,
        this->_Nazwa
        );
}

DaneIdentyfikacyjne2::DaneIdentyfikacyjne2(NabywcaPolski nabywcaPolski, std::optional<QString> nazwa) :
    _switch{std::move(nabywcaPolski)},
    _Nazwa{std::move(nazwa)}
{
}

DaneIdentyfikacyjne2::DaneIdentyfikacyjne2(NabywcaUE nabywcaUE, std::optional<QString> nazwa):
    _switch{std::move(nabywcaUE)},
    _Nazwa{std::move(nazwa)}
{
}

DaneIdentyfikacyjne2::DaneIdentyfikacyjne2(NabywcaPozaUE nabywcaPozaUE, std::optional<QString> nazwa):
    _switch{std::move(nabywcaPozaUE)},
    _Nazwa{std::move(nazwa)}
{
}

DaneIdentyfikacyjne2::DaneIdentyfikacyjne2(NabywcaBezID nabywcaBezID, std::optional<QString> nazwa):
    _switch{std::move(nabywcaBezID)},
    _Nazwa{std::move(nazwa)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> DaneIdentyfikacyjne2::elements() const
{
    return internal::elements_to_vector(
        this->_switch,
        this->_Nazwa
        );
}

DaneIdentyfikacyjne3 DaneIdentyfikacyjne2::toDaneIdentyfikacyjne3() const
{
    QString nazwa = _Nazwa.has_value() ? _Nazwa->value()->toString() : QString{};
    return std::visit([&nazwa](const auto &id) -> DaneIdentyfikacyjne3 {
        return DaneIdentyfikacyjne3{id, nazwa};
    }, _switch);
}

DaneIdentyfikacyjne3::DaneIdentyfikacyjne3(NabywcaPolski nabywcaPolski, QString nazwa) :
    _switch{std::move(nabywcaPolski)},
    _Nazwa{std::move(nazwa)}
{
}

DaneIdentyfikacyjne3::DaneIdentyfikacyjne3(NabywcaWewnetrzny nabywcaWewnetrzny, QString nazwa) :
    _switch{std::move(nabywcaWewnetrzny)},
    _Nazwa{std::move(nazwa)}
{
}

DaneIdentyfikacyjne3::DaneIdentyfikacyjne3(NabywcaUE nabywcaUE, QString nazwa):
    _switch{std::move(nabywcaUE)},
    _Nazwa{std::move(nazwa)}
{
}

DaneIdentyfikacyjne3::DaneIdentyfikacyjne3(NabywcaPozaUE nabywcaPozaUE, QString nazwa):
    _switch{std::move(nabywcaPozaUE)},
    _Nazwa{std::move(nazwa)}
{
}

DaneIdentyfikacyjne3::DaneIdentyfikacyjne3(NabywcaBezID nabywcaBezID, QString nazwa):
    _switch{std::move(nabywcaBezID)},
    _Nazwa{std::move(nazwa)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> DaneIdentyfikacyjne3::elements() const
{
    return internal::elements_to_vector(
        this->_switch,
        this->_Nazwa
        );
}

NabywcaPolski::NabywcaPolski(QString nip) :
    _NIP{std::move(nip)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode> > NabywcaPolski::elements() const {
    return internal::elements_to_vector(
        this->_NIP
        );
}

NabywcaWewnetrzny::NabywcaWewnetrzny(QString idWew) :
    _IDWew{std::move(idWew)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode> > NabywcaWewnetrzny::elements() const
{
    return internal::elements_to_vector(
        this->_IDWew
        );
}

NabywcaUE::NabywcaUE(QString kodUE, QString nrVatUE) :
    _KodUE{std::move(kodUE)},
    _NrVatUE{std::move(nrVatUE)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode> > NabywcaUE::elements() const
{
    return internal::elements_to_vector(
        this->_KodUE,
        this->_NrVatUE
        );

}

NabywcaPozaUE::NabywcaPozaUE(QString nrID, std::optional<QString> kodKraju) :
    _KodKraju{std::move(kodKraju)},
    _NrID{std::move(nrID)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode> > NabywcaPozaUE::elements() const
{
    return internal::elements_to_vector(
        this->_KodKraju,
        this->_NrID
        );
}

QVector<std::reference_wrapper<const internal::XmlNode> > NabywcaBezID::elements() const
{
    return internal::elements_to_vector(
        this->_BrakID
        );
}

} // namespace ksef::FA3
