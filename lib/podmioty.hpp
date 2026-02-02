#pragma once

#include "internal/xml_core.hpp"
#include "typy_adresowe.hpp"
#include "typy_identyfikacyjne.hpp"

namespace ksef::FA3 {

class StatusInfoPodatnika : public internal::XmlValue<"StatusInfoPodatnika">{
public:
    enum Status {
        W_LIKWIDACJI=1,
        W_RESTRUKTURYZACJI=2,
        W_UPADLOSCI=3,
        W_SPADKU=4
    };
    static QString ToString(Status status) {
        return QString::number(status);
    }
public:
    StatusInfoPodatnika(Status status) : XmlValue(ToString(status)) {}
};

class Podmiot1 : public internal::XmlNamedNode<"Podmiot1">{
    std::optional<internal::XmlString<"PrefiksPodatnika">> _PrefiksPodatnika;
    std::optional<internal::XmlString<"NrEORI">> _NrEORI;
    DaneIdentyfikacyjne _DaneIdentyfikacyjne;
    Adres _Adres;
    std::optional<AdresKoresp> _AdresKoresp;
    internal::XmlVector<DaneKontaktowe, 3> _DaneKontaktowe;
    std::optional<StatusInfoPodatnika> _StatusInfoPodatnika;
public:
    Podmiot1(DaneIdentyfikacyjne daneIdentyfikacyjne, Adres adres);
    void setPrefiksPodatnika(QString prefiksPodatnika);
    void setNrEORI(QString nrEORI);
    void setAdresKoresp(AdresKoresp adresKoresp);
    std::expected<void, QString> addDaneKontaktowe(DaneKontaktowe daneKontaktowe);
    void setStatusInfoPodatnika(StatusInfoPodatnika::Status status);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class JST : public internal::XmlValue<"JST">{
public:
    enum Status {
        JEDNOSTKA_SAMORZADU_TERYTORIALNEGO=1,
        NIE_DOTYCZY=2,
    };
    static QString ToString(Status status) {
        return QString::number(status);
    }
public:
    JST(Status status) : XmlValue(ToString(status)) {}
};

class GV : public internal::XmlValue<"GV">{
public:
    enum Status {
        GRUPA_VAT=1,
        NIE_DOTYCZY=2,
    };
    static QString ToString(Status status) {
        return QString::number(status);
    }
public:
    GV(Status status) : XmlValue(ToString(status)) {}
};

class Podmiot2 : public internal::XmlNamedNode<"Podmiot2">{
    std::optional<internal::XmlString<"NrEORI">> _NrEORI;
    DaneIdentyfikacyjne2 _DaneIdentyfikacyjne;
    std::optional<Adres> _Adres;
    std::optional<AdresKoresp> _AdresKoresp;
    internal::XmlVector<DaneKontaktowe, 3> _DaneKontaktowe;
    std::optional<internal::XmlString<"NrKlienta">> _NrKlienta;
    std::optional<internal::XmlString<"IDNabywcy">> _IDNabywcy;
    JST _JST;
    GV _GV;
public:
    Podmiot2(DaneIdentyfikacyjne2 daneIdentyfikacyjne, bool jst, bool gv);
    void setNrEORI(QString nrEORI);
    void setAdres(Adres adres);
    void setAdresKoresp(AdresKoresp adresKoresp);
    std::expected<void, QString> addDaneKontaktowe(DaneKontaktowe daneKontaktowe);
    void setNrKlienta(QString nrKlienta);
    void setIDNabywcy(QString idNabywcy);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Rola : public internal::XmlSequence {
public:
    enum Typ {
        FAKTOR=1,
        ODBIORCA=2,
        PODMIOT_PIERWOTNY=3,
        DODATKOWY_NABYWCA=4,
        WYSTAWCA_FAKTURY=5,
        DOKONUJACY_PLATNOSCI=6,
        JST_WYDAWCA=7,
        JST_ODBIORCA=8,
        CZLONEK_GV_WYSTAWCA=9,
        CZLONEK_GV_ODBIORCA=10,
        PRACOWNIK=11
    };
    static QString ToString(Typ rola) {
        return QString::number(rola);
    }
    internal::XmlString<"Rola"> _Rola;
public:
    Rola(Typ rola);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
};

using RolaInna = internal::XmlMarkedElement<"RolaInna", internal::XmlString<"OpisRoli">>;


class Podmiot3 : public internal::XmlNamedNode<"Podmiot3">{
    std::optional<internal::XmlString<"IDNabywcy">> _IDNabywcy;
    std::optional<internal::XmlString<"NrEORI">> _NrEORI;
    DaneIdentyfikacyjne3 _DaneIdentyfikacyjne;
    std::optional<Adres> _Adres;
    std::optional<AdresKoresp> _AdresKoresp;
    internal::XmlVector<DaneKontaktowe,3> _DaneKontaktowe;
    internal::XmlSwitch<
        Rola,
        RolaInna
        > _switch;
    std::optional<internal::XmlString<"Udzial">> _Udzial;
    std::optional<internal::XmlString<"NrKlienta">> _NrKlienta;
public:
    Podmiot3(DaneIdentyfikacyjne3 daneIdentyfikacyjne, Rola::Typ rola);
    Podmiot3(DaneIdentyfikacyjne3 daneIdentyfikacyjne, QString opisRoli);
    void setNrEORI(QString nrEORI);
    void setAdres(Adres adres);
    void setAdresKoresp(AdresKoresp adresKoresp);
    std::expected<void, QString> addDaneKontaktowe(DaneKontaktowe daneKontaktowe);
    void setNrKlienta(QString nrKlienta);
    void setIDNabywcy(QString idNabywcy);
    void setUdzial(unsigned char udzial);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class DaneIdentyfikacyjnePU : public internal::XmlNamedNode<"DaneIdentyfikacyjne">{
    internal::XmlString<"NIP"> _NIP;
    internal::XmlString<"Nazwa"> _Nazwa;
public:
    explicit DaneIdentyfikacyjnePU(QString nip, QString nazwa);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class RolaPU : public internal::XmlValue<"RolaPU">{
public:
    enum Typ {
        ORGAN_EGZEKUCYJNY=1,
        KOMORNIK_SADOWY=2,
        PRZEDSTAWICIEL_PODATKOWY=3,
    };
    static QString ToString(Typ rola) {
        return QString::number(rola);
    }
public:
    RolaPU(Typ rola) : XmlValue(ToString(rola)) {}
};

class PodmiotUpowazniony : public internal::XmlNamedNode<"PodmiotUpowazniony">{
    std::optional<internal::XmlString<"NrEORI">> _NrEORI;
    DaneIdentyfikacyjnePU _DaneIdentyfikacyjne;
    Adres _Adres;
    std::optional<AdresKoresp> _AdresKoresp;
    internal::XmlVector<DaneKontaktowePU, 3> _DaneKontaktowe;
    RolaPU _RolaPU;
public:
    PodmiotUpowazniony(DaneIdentyfikacyjnePU daneIdentyfikacyjne, Adres adres, RolaPU::Typ rola);
    void setNrEORI(QString nrEORI);
    void setAdresKoresp(AdresKoresp adresKoresp);
    std::expected<void, QString> addDaneKontaktowe(DaneKontaktowePU daneKontaktowe);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

} // namespace ksef::FA3
