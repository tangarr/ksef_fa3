#pragma once

#include "internal/xml_core.hpp"

namespace ksef::FA3 {

class TAdres : public internal::XmlSequence {
    internal::XmlString<"KodKraju"> _KodKraju;
    internal::XmlString<"AdresL1"> _AdresL1;
    std::optional<internal::XmlString<"AdresL2">> _AdresL2;
    std::optional<internal::XmlString<"GLN">> _GLN;
public:
    TAdres(QString kodKraju, QString adresL1, std::optional<QString> adresL2 = std::nullopt, std::optional<QString> gln = std::nullopt);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Adres : public internal::XmlNamedNode<"Adres"> {
private:
    TAdres _adres;
public:
    Adres(QString kodKraju, QString adresL1, std::optional<QString> adresL2 = std::nullopt, std::optional<QString> gln = std::nullopt);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class AdresKoresp : public internal::XmlNamedNode<"AdresKoresp"> {
private:
    TAdres _adres;
public:
    AdresKoresp(QString kodKraju, QString adresL1, std::optional<QString> adresL2 = std::nullopt, std::optional<QString> gln = std::nullopt);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class DaneKontaktowe : public internal::XmlNamedNode<"DaneKontaktowe">{
    std::optional<internal::XmlString<"Email">> _Email;
    std::optional<internal::XmlString<"Telefon">> _Telefon;
public:
    DaneKontaktowe(std::optional<QString> email, std::optional<QString> telefon);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class DaneKontaktowePU : public internal::XmlNamedNode<"DaneKontaktowe">{
    std::optional<internal::XmlString<"EmailPU">> _EmailPU;
    std::optional<internal::XmlString<"TelefonPU">> _TelefonPU;
public:
    DaneKontaktowePU(std::optional<QString> email, std::optional<QString> telefon);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

} // namespace ksef::FA3
