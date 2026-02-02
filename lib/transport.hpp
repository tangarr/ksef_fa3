#pragma once

#include "internal/xml_core.hpp"
#include "typy_adresowe.hpp"
#include "typy_identyfikacyjne.hpp"

namespace ksef::FA3 {

class RodzajTransportu : public internal::XmlValue<"RodzajTransportu"> {
public:
    enum Rodzaj {
        MORSKI = 1,
        KOLEJOWY = 2,
        DROGOWY = 3,
        LOTNICZY = 4,
        POCZTOWA = 5,
        STALE_INSTALACJE = 7,
        SRODLADOWA = 8
    };
    static QString ToString(Rodzaj rodzaj) {
        return QString::number(rodzaj);
    }
public:
    RodzajTransportu(Rodzaj rodzaj) : XmlValue(ToString(rodzaj)) {}
};

class OpisLadunku : public internal::XmlValue<"OpisLadunku"> {
public:
    enum Ladunek {
        BANKA = 1,
        BECZKA = 2,
        BUTLA = 3,
        KARTON = 4,
        KANISTER = 5,
        KLATKA = 6,
        KONTENER = 7,
        KOSZ = 8,
        LUBIANKA = 9,
        OPAKOWANIE_ZBIORCZE = 10,
        PACZKA = 11,
        PAKIET = 12,
        PALETA = 13,
        POJEMNIK = 14,
        POJEMNIK_MASOWE_STALE = 15,
        POJEMNIK_MASOWE_PLYNNE = 16,
        PUDELKO = 17,
        PUSZKA = 18,
        SKRZYNIA = 19,
        WOREK = 20
    };
    static QString ToString(Ladunek ladunek) {
        return QString::number(ladunek);
    }
public:
    OpisLadunku(Ladunek ladunek) : XmlValue(ToString(ladunek)) {}
};

class AdresPrzewoznika : public internal::XmlNamedNode<"AdresPrzewoznika">, TAdres {
public:
    AdresPrzewoznika(QString kodKraju, QString adresL1, std::optional<QString> adresL2 = std::nullopt, std::optional<QString> gln = std::nullopt);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Przewoznik : public internal::XmlNamedNode<"Przewoznik"> {
    DaneIdentyfikacyjne2 _DaneIdentyfikacyjne;
    AdresPrzewoznika _AdresPrzewoznika;
public:
    Przewoznik(DaneIdentyfikacyjne2 daneIdentyfikacyjne, AdresPrzewoznika adresPrzewoznika);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class WysylkaZ : public internal::XmlNamedNode<"WysylkaZ">, TAdres {
public:
    WysylkaZ(QString kodKraju, QString adresL1, std::optional<QString> adresL2 = std::nullopt, std::optional<QString> gln = std::nullopt);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class WysylkaPrzez : public internal::XmlNamedNode<"WysylkaPrzez">, TAdres {
public:
    WysylkaPrzez(QString kodKraju, QString adresL1, std::optional<QString> adresL2 = std::nullopt, std::optional<QString> gln = std::nullopt);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class WysylkaDo : public internal::XmlNamedNode<"WysylkaDo">, TAdres {
public:
    WysylkaDo(QString kodKraju, QString adresL1, std::optional<QString> adresL2 = std::nullopt, std::optional<QString> gln = std::nullopt);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Transport : public internal::XmlNamedNode<"Transport"> {
    using TransportInny = internal::XmlMarkedElement<"TransportInny",internal::XmlString<"OpisInnegoTransportu">>;
    using LadunekInny = internal::XmlMarkedElement<"LadunekInny",internal::XmlString<"OpisInnegoLadunku">>;

    internal::XmlSwitch<
        internal::XmlSingleton<RodzajTransportu>,
        TransportInny> _switchTransport;
    std::optional<Przewoznik> _Przewoznik;
    std::optional<internal::XmlString<"NrZleceniaTransportu">> _NrZleceniaTransportu;
    internal::XmlSwitch<
        internal::XmlSingleton<OpisLadunku>,
        LadunekInny> _switchLadunek;
    std::optional<internal::XmlString<"JednostkaOpakowania">> _JednostkaOpakowania;
    std::optional<internal::XmlDateTime<"DataGodzRozpTransportu">> _DataGodzRozpTransportu;
    std::optional<internal::XmlDateTime<"DataGodzZakTransportu">> _DataGodzZakTransportu;
    std::optional<WysylkaZ> _WysylkaZ;
    internal::XmlVector<WysylkaPrzez, 20> _WysylkaPrzez;
    std::optional<WysylkaDo> _WysylkaDo;
public:
    Transport(
        std::variant<RodzajTransportu::Rodzaj, QString> transport,
        std::variant<OpisLadunku::Ladunek, QString> ladunek
        );
    void setPrzewoznik(Przewoznik przewoznik);
    void setNrZleceniaTransportu(QString nrZlecenia);
    void setJednostkaOpakowania(QString jednostka);
    void setDataGodzRozpTransportu(QDateTime dataGodz);
    void setDataGodzZakTransportu(QDateTime dataGodz);
    void setWysylkaZ(WysylkaZ wysylkaZ);
    std::expected<void, QString> addWysylkaPrzez(WysylkaPrzez wysylkaPrzez);
    void setWysylkaDo(WysylkaDo wysylkaDo);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Umowy : public internal::XmlNamedNode<"Umowy"> {
    std::optional<internal::XmlDate<"DataUmowy">> _DataUmowy;
    std::optional<internal::XmlString<"NrUmowy">> _NrUmowy;
public:
    Umowy(std::optional<QDate> dataUmowy = std::nullopt, std::optional<QString> nrUmowy = std::nullopt);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class ZamowieniaWT : public internal::XmlNamedNode<"Zamowienia"> {
    std::optional<internal::XmlDate<"DataZamowienia">> _DataZamowienia;
    std::optional<internal::XmlString<"NrZamowienia">> _NrZamowienia;
public:
    ZamowieniaWT(std::optional<QDate> dataZamowienia = std::nullopt, std::optional<QString> nrZamowienia = std::nullopt);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class WarunkiTransakcji : public internal::XmlNamedNode<"WarunkiTransakcji"> {
    struct KursUmowny : public internal::XmlSequence {
        std::optional<internal::XmlDecimal<"KursUmowny">> _KursUmowny;
        std::optional<internal::XmlString<"WalutaUmowna">> _WalutaUmowna;
        KursUmowny(QString kurs, QString waluta);
        QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
    };
    internal::XmlVector<Umowy, 100> _Umowy;
    internal::XmlVector<ZamowieniaWT, 100> _Zamowienia;
    internal::XmlVector<internal::XmlString<"NrPartiiTowaru">, 1000> _NrPartiiTowaru;
    std::optional<internal::XmlString<"WarunkiDostawy">> _WarunkiDostawy;
    std::optional<KursUmowny> _KursUmowny;
    internal::XmlVector<Transport, 20> _Transport;
    std::optional<internal::XmlBoolTrue<"PodmiotPosredniczacy">> _PodmiotPosredniczacy;
public:
    WarunkiTransakcji() = default;
    std::expected<void, QString> addUmowa(Umowy umowa);
    std::expected<void, QString> addZamowienie(ZamowieniaWT zamowienie);
    std::expected<void, QString> addNrPartiiTowaru(QString nrPartii);
    void setWarunkiDostawy(QString warunki);
    void setKursUmowny(QString kurs, QString waluta);
    std::expected<void, QString> addTransport(Transport transport);
    void setPodmiotPosredniczacy();
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

} // namespace ksef::FA3
