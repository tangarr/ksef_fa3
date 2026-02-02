#pragma once

#include "internal/xml_core.hpp"

namespace ksef::FA3 {

class ZMetaDane : public internal::XmlNamedNode<"MetaDane"> {
    internal::XmlString<"ZKlucz"> _ZKlucz;
    internal::XmlString<"ZWartosc"> _ZWartosc;
public:
    ZMetaDane(QString klucz, QString wartosc);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class ZTekst : public internal::XmlNamedNode<"Tekst"> {
    internal::XmlVector<internal::XmlString<"Akapit">, 10> _Akapit;
public:
    std::expected<void, QString> addAkapit(QString akapit);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class TMetaDane : public internal::XmlNamedNode<"TMetaDane"> {
    internal::XmlString<"TKlucz"> _TKlucz;
    internal::XmlString<"TWartosc"> _TWartosc;
public:
    TMetaDane(QString klucz, QString wartosc);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Kol : public internal::XmlNamedNode<"Kol"> {
public:
    enum class TypKolumny {
        DATE,
        DATETIME,
        DEC,
        INT,
        TIME,
        TXT
    };
private:
    static constexpr QString ToString(TypKolumny typ) {
        switch (typ) {
        case TypKolumny::DATE:
            return QStringLiteral("date");
        case TypKolumny::DATETIME:
            return QStringLiteral("datetime");
        case TypKolumny::DEC:
            return QStringLiteral("dec");
        case TypKolumny::INT:
            return QStringLiteral("int");
        case TypKolumny::TIME:
            return QStringLiteral("time");
        case TypKolumny::TXT:
            return QStringLiteral("txt");
        }
        std::unreachable();
    }
    internal::XmlAttribute _Typ;
    internal::XmlString<"NKom"> _NKom;
public:
    Kol(TypKolumny typ, QString nazwa = QString());
    QVector<std::reference_wrapper<const internal::XmlAttribute>> attributes() const override;
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class TNaglowek : public internal::XmlNamedNode<"TNaglowek"> {
    internal::XmlVector<Kol, 20> _Kol;
public:
    std::expected<void, QString> addKol(Kol kol);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class ZWiersz : public internal::XmlNamedNode<"Wiersz"> {
    internal::XmlVector<internal::XmlString<"WKom">, 20> _WKom;
public:
    std::expected<void, QString> addWKom(QString value);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class ZSuma : public internal::XmlNamedNode<"Suma"> {
    internal::XmlVector<internal::XmlString<"SKom">, 20> _SKom;
public:
    std::expected<void, QString> addSKom(QString value);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class ZTabela : public internal::XmlNamedNode<"Tabela"> {
    internal::XmlVector<TMetaDane, 1000> _TMetaDane;
    std::optional<internal::XmlString<"Opis">> _Opis;
    TNaglowek _TNaglowek;
    internal::XmlVector<ZWiersz, 1000> _Wiersz;
    std::optional<ZSuma> _Suma;
public:
    ZTabela(TNaglowek naglowek);
    std::expected<void, QString> addTMetaDane(TMetaDane metaDane);
    void setOpis(QString opis);
    std::expected<void, QString> addWiersz(ZWiersz wiersz);
    void setSuma(ZSuma suma);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class BlokDanych : public internal::XmlNamedNode<"BlokDanych"> {
    std::optional<internal::XmlString<"ZNaglowek">> _ZNaglowek;
    internal::XmlVector<ZMetaDane, 1000> _MetaDane;
    std::optional<ZTekst> _Tekst;
    internal::XmlVector<ZTabela, 1000> _Tabela;
public:
    void setZNaglowek(QString naglowek);
    std::expected<void, QString> addMetaDane(ZMetaDane metaDane);
    void setTekst(ZTekst tekst);
    std::expected<void, QString> addTabela(ZTabela tabela);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Zalacznik : public internal::XmlNamedNode<"Zalacznik"> {
    internal::XmlVector<BlokDanych, 1000> _BlokDanych;
public:
    std::expected<void, QString> addBlokDanych(BlokDanych blokDanych);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

} // namespace ksef::FA3
