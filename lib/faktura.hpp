#pragma once

#include "internal/xml_core.hpp"
#include "fa.hpp"
#include "podmioty.hpp"
#include "stopka.hpp"
#include "zalacznik.hpp"

namespace ksef::FA3 {

class KodFormularza : public internal::XmlNamedNode<"KodFormularza"> {
    internal::XmlAttribute _kodSystemowy{.name=QStringLiteral("kodSystemowy"), .value=QStringLiteral("FA (3)")};
    internal::XmlAttribute _wersjaSchemy{.name=QStringLiteral("wersjaSchemy"), .value=QStringLiteral("1-0E")};
public:
    QVector<std::reference_wrapper<const internal::XmlAttribute> > attributes() const override;
    std::optional<QStringView> value() const override;
};

class Naglowek : public internal::XmlNamedNode<"Naglowek">{
    KodFormularza _KodFormularza;
    internal::XmlString<"WariantFormularza"> _WariantFormularza{QStringLiteral("3")};
    internal::XmlDateTime<"DataWytworzeniaFa"> _DataWytworzeniaFa;
    std::optional<internal::XmlString<"SystemInfo">> _SystemInfo;
public:
    Naglowek(QDateTime DataWytworzeniaFa=QDateTime::currentDateTimeUtc(), std::optional<QString> SystemInfo=std::nullopt);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Faktura : public internal::XmlNamedNode<"Faktura"> {
    Naglowek _Naglowek;
    Podmiot1 _Podmiot1;
    Podmiot2 _Podmiot2;
    internal::XmlVector<Podmiot3, 100> _Podmiot3;
    std::optional<PodmiotUpowazniony> _PodmiotUpowazniony;
    Fa _Fa;
    std::optional<Stopka> _Stopka;
    std::optional<Zalacznik> _Zalacznik;
public:
    Faktura(Naglowek naglowek, Podmiot1 podmiot1, Podmiot2 podmiot2, Fa fa);
    std::expected<void, QString> addPodmiot3(Podmiot3 podmiot3);
    void setPodmiotUpowazniony(PodmiotUpowazniony podmiotUpowazniony);
    void setStopka(Stopka stopka);
    void setZalacznik(Zalacznik zalacznik);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
    std::optional<QStringView> defaultNamespace() const override;
    const QVector<internal::XmlNamespace> &namespaces() const override;
};

} // namespace ksef::FA3
