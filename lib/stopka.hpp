#pragma once

#include "internal/xml_core.hpp"

namespace ksef::FA3 {

class Informacje : public internal::XmlNamedNode<"Informacje">{
    std::optional<internal::XmlString<"StopkaFaktury">> _StopkaFaktury;
public:
    Informacje(std::optional<QString> stopkaFaktury = std::nullopt);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Rejestry : public internal::XmlNamedNode<"Rejestry">{
    std::optional<internal::XmlString<"PelnaNazwa">> _PelnaNazwa;
    std::optional<internal::XmlString<"KRS">> _KRS;
    std::optional<internal::XmlString<"REGON">> _REGON;
    std::optional<internal::XmlString<"BDO">> _BDO;
public:
    void setPelnaNazwa(QString pelnaNazwa);
    void setKRS(QString krs);
    void setREGON(QString regon);
    void setBDO(QString bdo);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Stopka : public internal::XmlNamedNode<"Stopka">{
    internal::XmlVector<Informacje, 3> _Informacje;
    internal::XmlVector<Rejestry, 100> _Rejestry;
public:
    std::expected<void, QString> addInformacje(Informacje informacje);
    std::expected<void, QString> addRejestry(Rejestry rejestry);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

} // namespace ksef::FA3
