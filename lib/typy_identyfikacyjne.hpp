#pragma once

#include "internal/xml_core.hpp"

namespace ksef::FA3 {

class DaneIdentyfikacyjne : public internal::XmlNamedNode<"DaneIdentyfikacyjne">{
    internal::XmlString<"NIP"> _NIP;
    internal::XmlString<"Nazwa"> _Nazwa;
public:
    DaneIdentyfikacyjne(QString nip, QString nazwa);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class NabywcaPolski : public internal::XmlSequence {
private:
    internal::XmlString<"NIP"> _NIP;
public:
    NabywcaPolski(QString nip);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
};

class NabywcaWewnetrzny : public internal::XmlSequence {
    internal::XmlString<"IDWew"> _IDWew;
public:
    NabywcaWewnetrzny(QString idWew);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
};

class NabywcaUE : public internal::XmlSequence {
    internal::XmlString<"KodUE"> _KodUE;
    internal::XmlString<"NrVatUE"> _NrVatUE;
public:
    NabywcaUE(QString kodUE, QString nrVatUE);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
};

class NabywcaPozaUE : public internal::XmlSequence {
    std::optional<internal::XmlString<"KodKraju">> _KodKraju;
    internal::XmlString<"NrID"> _NrID;
public:
    NabywcaPozaUE(QString nrID, std::optional<QString> kodKraju=std::nullopt);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
};

class NabywcaBezID : public internal::XmlSequence {
    internal::XmlBoolTrue<"BrakID"> _BrakID{};
public:
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
};

class DaneIdentyfikacyjne3;

class DaneIdentyfikacyjne2 : public internal::XmlNamedNode<"DaneIdentyfikacyjne">{
    internal::XmlSwitch<NabywcaPolski,
                        NabywcaUE,
                        NabywcaPozaUE,
                        NabywcaBezID> _switch;
    std::optional<internal::XmlString<"Nazwa">> _Nazwa;
public:
    explicit DaneIdentyfikacyjne2(NabywcaPolski nabywcaPolski, std::optional<QString> nazwa);
    explicit DaneIdentyfikacyjne2(NabywcaUE nabywcaUE, std::optional<QString> nazwa);
    explicit DaneIdentyfikacyjne2(NabywcaPozaUE nabywcaPozaUE, std::optional<QString> nazwa);
    explicit DaneIdentyfikacyjne2(NabywcaBezID nabywcaBezID, std::optional<QString> nazwa);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
    DaneIdentyfikacyjne3 toDaneIdentyfikacyjne3() const;
};

class DaneIdentyfikacyjne3 : public internal::XmlNamedNode<"DaneIdentyfikacyjne">{
    internal::XmlSwitch<NabywcaPolski,
                        NabywcaWewnetrzny,
                        NabywcaUE,
                        NabywcaPozaUE,
                        NabywcaBezID> _switch;
    internal::XmlString<"Nazwa"> _Nazwa;
public:
    explicit DaneIdentyfikacyjne3(NabywcaPolski nabywcaPolski, QString nazwa);
    explicit DaneIdentyfikacyjne3(NabywcaWewnetrzny nabywcaWewnetrzny, QString nazwa);
    explicit DaneIdentyfikacyjne3(NabywcaUE nabywcaUE, QString nazwa);
    explicit DaneIdentyfikacyjne3(NabywcaPozaUE nabywcaPozaUE, QString nazwa);
    explicit DaneIdentyfikacyjne3(NabywcaBezID nabywcaBezID, QString nazwa);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

} // namespace ksef::FA3
