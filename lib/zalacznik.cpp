#include "zalacznik.hpp"
#include "internal/xml_helpers.hpp"

namespace ksef::FA3 {

ZMetaDane::ZMetaDane(QString klucz, QString wartosc) :
    _ZKlucz{std::move(klucz)},
    _ZWartosc{std::move(wartosc)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> ZMetaDane::elements() const
{
    return internal::elements_to_vector(
        this->_ZKlucz,
        this->_ZWartosc
        );
}

std::expected<void, QString> ZTekst::addAkapit(QString akapit)
{
    return this->_Akapit.push_back(internal::XmlString<"Akapit">{std::move(akapit)});
}

QVector<std::reference_wrapper<const internal::XmlNode>> ZTekst::elements() const
{
    return internal::elements_to_vector(
        this->_Akapit
        );
}

TMetaDane::TMetaDane(QString klucz, QString wartosc) :
    _TKlucz{std::move(klucz)},
    _TWartosc{std::move(wartosc)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> TMetaDane::elements() const
{
    return internal::elements_to_vector(
        this->_TKlucz,
        this->_TWartosc
        );
}

Kol::Kol(TypKolumny typ, QString nazwa) :
    _Typ{.name = QStringLiteral("Typ"), .value = ToString(typ)},
    _NKom{std::move(nazwa)}
{
}

QVector<std::reference_wrapper<const internal::XmlAttribute>> Kol::attributes() const
{
    return {std::cref(this->_Typ)};
}

QVector<std::reference_wrapper<const internal::XmlNode>> Kol::elements() const
{
    return internal::elements_to_vector(
        this->_NKom
        );
}

std::expected<void, QString> TNaglowek::addKol(Kol kol)
{
    return this->_Kol.push_back(std::move(kol));
}

QVector<std::reference_wrapper<const internal::XmlNode>> TNaglowek::elements() const
{
    return internal::elements_to_vector(
        this->_Kol
        );
}

std::expected<void, QString> ZWiersz::addWKom(QString value)
{
    return this->_WKom.push_back(internal::XmlString<"WKom">{std::move(value)});
}

QVector<std::reference_wrapper<const internal::XmlNode>> ZWiersz::elements() const
{
    return internal::elements_to_vector(
        this->_WKom
        );
}

std::expected<void, QString> ZSuma::addSKom(QString value)
{
    return this->_SKom.push_back(internal::XmlString<"SKom">{std::move(value)});
}

QVector<std::reference_wrapper<const internal::XmlNode>> ZSuma::elements() const
{
    return internal::elements_to_vector(
        this->_SKom
        );
}

ZTabela::ZTabela(TNaglowek naglowek) :
    _TNaglowek{std::move(naglowek)}
{
}

std::expected<void, QString> ZTabela::addTMetaDane(TMetaDane metaDane)
{
    return this->_TMetaDane.push_back(std::move(metaDane));
}

void ZTabela::setOpis(QString opis)
{
    this->_Opis.emplace(std::move(opis));
}

std::expected<void, QString> ZTabela::addWiersz(ZWiersz wiersz)
{
    return this->_Wiersz.push_back(std::move(wiersz));
}

void ZTabela::setSuma(ZSuma suma)
{
    this->_Suma = std::move(suma);
}

QVector<std::reference_wrapper<const internal::XmlNode>> ZTabela::elements() const
{
    return internal::elements_to_vector(
        this->_TMetaDane,
        this->_Opis,
        this->_TNaglowek,
        this->_Wiersz,
        this->_Suma
        );
}

void BlokDanych::setZNaglowek(QString naglowek)
{
    this->_ZNaglowek.emplace(std::move(naglowek));
}

std::expected<void, QString> BlokDanych::addMetaDane(ZMetaDane metaDane)
{
    return this->_MetaDane.push_back(std::move(metaDane));
}

void BlokDanych::setTekst(ZTekst tekst)
{
    this->_Tekst = std::move(tekst);
}

std::expected<void, QString> BlokDanych::addTabela(ZTabela tabela)
{
    return this->_Tabela.push_back(std::move(tabela));
}

QVector<std::reference_wrapper<const internal::XmlNode>> BlokDanych::elements() const
{
    return internal::elements_to_vector(
        this->_ZNaglowek,
        this->_MetaDane,
        this->_Tekst,
        this->_Tabela
        );
}

std::expected<void, QString> Zalacznik::addBlokDanych(BlokDanych blokDanych)
{
    return this->_BlokDanych.push_back(std::move(blokDanych));
}

QVector<std::reference_wrapper<const internal::XmlNode>> Zalacznik::elements() const
{
    return internal::elements_to_vector(
        this->_BlokDanych
        );
}

} // namespace ksef::FA3
