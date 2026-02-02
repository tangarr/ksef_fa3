#pragma once

#include "internal/xml_core.hpp"

namespace ksef::FA3 {

class Obciazenia : public internal::XmlNamedNode<"Obciazenia">{
    internal::XmlDecimal<"Kwota"> _Kwota;
    internal::XmlString<"Powod"> _Powod;
public:
    Obciazenia(QString kwota, QString powod);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Odliczenia : public internal::XmlNamedNode<"Odliczenia">{
    internal::XmlDecimal<"Kwota"> _Kwota;
    internal::XmlString<"Powod"> _Powod;
public:
    Odliczenia(QString kwota, QString powod);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Rozliczenie : public internal::XmlNamedNode<"Rozliczenie">{
    using DoZaplaty = internal::XmlSingleton<internal::XmlDecimal<"DoZaplaty">>;
    using DoRozliczenia = internal::XmlSingleton<internal::XmlDecimal<"DoRozliczenia">>;

    internal::XmlVector<Obciazenia, 100> _Obciazenia;
    std::optional<internal::XmlDecimal<"SumaObciazen">> _SumaObciazen;
    internal::XmlVector<Odliczenia, 100> _Odliczenia;
    std::optional<internal::XmlDecimal<"SumaOdliczen">> _SumaOdliczen;
    internal::XmlSwitch<DoZaplaty, DoRozliczenia> _switch;
public:
    Rozliczenie(
        std::optional<std::tuple<internal::XmlVector<Obciazenia, 100>, QString>> obciazenia,
        std::optional<std::tuple<internal::XmlVector<Odliczenia, 100>, QString>> odliczenia,
        bool do_zaplaty,
        QString kwota
        );
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};


class TerminOpis : public internal::XmlNamedNode<"TerminOpis">{
    internal::XmlString<"Ilosc"> _Ilosc;
    internal::XmlString<"Jednostka"> _Jednostka;
    internal::XmlString<"ZdarzeniePoczatkowe"> _ZdarzeniePoczatkowe;
public:
    TerminOpis(QString ilosc, QString jednostka, QString zdarzeniePoczatkowe);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};


class TerminPlatnosci : public internal::XmlNamedNode<"TerminPlatnosci">{
    std::optional<internal::XmlDate<"Termin">> _Termin;
    std::optional<TerminOpis> _TerminOpis;
public:
    TerminPlatnosci(std::optional<QDate> termin, std::optional<TerminOpis> opis);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};


class FormaPlatnosci : public internal::XmlValue<"FormaPlatnosci">{
public:
    enum class Forma {
        GOTOWKA=1,
        KARTA=2,
        BON=3,
        CZEK=4,
        KREDYT=5,
        PRZELEW=6,
        PLATNOSC_MOBILNA=7
    };
    static QString ToString(Forma forma) {
        return QString::number(std::to_underlying(forma));
    }
public:
    FormaPlatnosci(Forma forma) : XmlValue(FormaPlatnosci::ToString(forma)) {}
};

using PlatnoscInna = internal::XmlMarkedElement<"PlatnoscInna", internal::XmlString<"OpisPlatnosci">>;

class ZaplataCzesciowa : public internal::XmlNamedNode<"ZaplataCzesciowa">{
    internal::XmlDecimal<"KwotaZaplatyCzesciowej"> _KwotaZaplatyCzesciowej;
    internal::XmlDate<"DataZaplatyCzesciowej"> _DataZaplatyCzesciowej;
    internal::XmlSwitch<internal::XmlSingleton<FormaPlatnosci>, PlatnoscInna> _switch;
public:
    ZaplataCzesciowa(QString kwota, QDate data, FormaPlatnosci::Forma formaPlatnosci);
    ZaplataCzesciowa(QString kwota, QDate data, QString opisPlatnosci);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class RachunekWlasnyBanku : public internal::XmlValue<"RachunekWlasnyBanku">{
public:
    enum class Typ {
        TYP_1=1,
        TYP_2=2,
        TYP_3=3,
    };
    static QString ToString(Typ typ) {
        return QString::number(std::to_underlying(typ));
    }
public:
    RachunekWlasnyBanku(Typ typ) : XmlValue(RachunekWlasnyBanku::ToString(typ)) {}
};


//TODO: Refactor it, maybe to template
class TRachunekBankowy : public internal::XmlSequence {
    internal::XmlString<"NrRB"> _NrRB;
    std::optional<internal::XmlString<"SWIFT">> _SWIFT;
    std::optional<RachunekWlasnyBanku> _RachunekWlasnyBanku;
    std::optional<internal::XmlString<"NazwaBanku">> _NazwaBanku;
    std::optional<internal::XmlString<"OpisRachunku">> _OpisRachunku;
public:
    TRachunekBankowy(QString nrRB, std::optional<QString> nazwaBanku, std::optional<QString> opisRachunku);
    void setSWIFT(QString swift);
    void setRachunekWlasnyBanku(RachunekWlasnyBanku::Typ typ);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
};

class RachunekBankowy : public internal::XmlNamedNode<"RachunekBankowy"> {
    TRachunekBankowy _rachunekBankowy;
public:
    RachunekBankowy(QString nrRB, std::optional<QString> nazwaBanku=std::nullopt, std::optional<QString> opisRachunku=std::nullopt);
    void setSWIFT(QString swift);
    void setRachunekWlasnyBanku(RachunekWlasnyBanku::Typ typ);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class RachunekBankowyFaktora : public internal::XmlNamedNode<"RachunekBankowyFaktora"> {
    TRachunekBankowy _rachunekBankowy;
public:
    RachunekBankowyFaktora(QString nrRB, std::optional<QString> nazwaBanku=std::nullopt, std::optional<QString> opisRachunku=std::nullopt);
    void setSWIFT(QString swift);
    void setRachunekWlasnyBanku(RachunekWlasnyBanku::Typ typ);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Skonto : public internal::XmlNamedNode<"Skonto">{
    internal::XmlString<"WarunkiSkonta"> _WarunkiSkonta;
    internal::XmlString<"WysokoscSkonta"> _WysokoscSkonta;
public:
    Skonto(QString warunkiSkonta, QString wysokoscSkonta);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Platnosc : public internal::XmlNamedNode<"Platnosc">{    
    using Zaplacono = internal::XmlMarkedElement<"Zaplacono", internal::XmlDate<"DataZaplaty">>;
    struct ZnacznikZaplatyCzesciowej : public internal::XmlSequence {
        internal::XmlBool<"ZnacznikZaplatyCzesciowej"> _ZnacznikZaplatyCzesciowej;
        internal::XmlVector<ZaplataCzesciowa, 100> _ZaplataCzesciowa;
        ZnacznikZaplatyCzesciowej(bool zaplata_czesciowa, internal::XmlVector<ZaplataCzesciowa, 100> wplaty);
        QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
    };
    std::optional<internal::XmlSwitch<Zaplacono, ZnacznikZaplatyCzesciowej>> _switchZaplata;
    internal::XmlVector<TerminPlatnosci, 100> _TerminPlatnosci;
    std::optional<internal::XmlSwitch<internal::XmlSingleton<FormaPlatnosci>, PlatnoscInna>> _switchFormaPlatnosci;
    internal::XmlVector<RachunekBankowy, 100> _RachunekBankowy;
    internal::XmlVector<RachunekBankowyFaktora, 20> _RachunekBankowyFaktora;
    std::optional<Skonto> _Skonto;
    std::optional<internal::XmlString<"LinkDoPlatnosci">> _LinkDoPlatnosci;
    std::optional<internal::XmlString<"IPKSeF">> _IPKSeF;
public:
    void setZaplacono(QDate dataZaplaty);
    void setZaplacono(bool zaplata_czesciowa, internal::XmlVector<ZaplataCzesciowa, 100>);
    std::expected<void, QString> addTerminPlatnosci(std::optional<QDate> terminPlatnosci, std::optional<TerminOpis> opis);
    void setFormaPlatnosci(FormaPlatnosci::Forma forma);
    void setFormaPlatnosci(QString innaFormaPlatnosci);
    std::expected<void, QString> addRachunekBankowy(RachunekBankowy rachunekBankowy);
    std::expected<void, QString> addRachunekBankowyFaktora(RachunekBankowyFaktora rachunekBankowy);
    void setSkonto(Skonto skonto);
    void setLinkDoPlatnosci(QString link);
    void setIPKSeF(QString ipksef);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

} // namespace ksef::FA3
