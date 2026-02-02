#include "faktura.hpp"
#include "internal/xml_helpers.hpp"

namespace ksef::FA3 {

QVector<std::reference_wrapper<const internal::XmlAttribute> > KodFormularza::attributes() const
{
    return {
        std::cref(this->_kodSystemowy),
        std::cref(this->_wersjaSchemy)
    };
}

std::optional<QStringView> KodFormularza::value() const
{
    return QStringLiteral("FA");
}

Naglowek::Naglowek(QDateTime DataWytworzeniaFa, std::optional<QString> SystemInfo) :
    _DataWytworzeniaFa{std::move(DataWytworzeniaFa)},
    _SystemInfo{std::move(SystemInfo)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode> > Naglowek::elements() const
{
    return internal::elements_to_vector(
        this->_KodFormularza,
        this->_WariantFormularza,
        this->_DataWytworzeniaFa,
        this->_SystemInfo
        );
}

Faktura::Faktura(Naglowek naglowek, Podmiot1 podmiot1, Podmiot2 podmiot2, Fa fa) :
    _Naglowek{std::move(naglowek)},
    _Podmiot1{std::move(podmiot1)},
    _Podmiot2{std::move(podmiot2)},
    _Fa{std::move(fa)}
{
}

std::expected<void, QString> Faktura::addPodmiot3(Podmiot3 podmiot3)
{
    return this->_Podmiot3.push_back(std::move(podmiot3));
}

void Faktura::setPodmiotUpowazniony(PodmiotUpowazniony podmiotUpowazniony)
{
    this->_PodmiotUpowazniony = std::move(podmiotUpowazniony);
}

void Faktura::setStopka(Stopka stopka)
{
    this->_Stopka = std::move(stopka);
}

void Faktura::setZalacznik(Zalacznik zalacznik)
{
    this->_Zalacznik = std::move(zalacznik);
}

QVector<std::reference_wrapper<const internal::XmlNode>> Faktura::elements() const
{
    return internal::elements_to_vector(
        this->_Naglowek,
        this->_Podmiot1,
        this->_Podmiot2,
        this->_Podmiot3,
        this->_PodmiotUpowazniony,
        this->_Fa,
        this->_Stopka,
        this->_Zalacznik
        );
}

std::optional<QStringView> Faktura::defaultNamespace() const
{
    return QStringLiteral("http://crd.gov.pl/wzor/2025/06/25/13775/");
}

const QVector<internal::XmlNamespace> &Faktura::namespaces() const
{
    static QVector<internal::XmlNamespace> namespaces{
        internal::XmlNamespace{.uri=QStringLiteral("http://crd.gov.pl/xml/schematy/dziedzinowe/mf/2022/01/05/eD/DefinicjeTypy/"), .prefix=QStringLiteral("etd")},
        internal::XmlNamespace{.uri=QStringLiteral("http://www.w3.org/2001/XMLSchema-instance"), .prefix=QStringLiteral("xsi")}
    };
    return namespaces;
}

} // namespace ksef::FA3
