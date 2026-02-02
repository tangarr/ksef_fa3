#pragma once

#include "internal/xml_core.hpp"

namespace ksef::FA3 {

using ZwolnienieVAT = internal::XmlMarkedElement<"P_19", internal::XmlString<"P_19A">>;
using ZwolnienieDyrektywa = internal::XmlMarkedElement<"P_19", internal::XmlString<"P_19B">>;
using ZwolnienieInne = internal::XmlMarkedElement<"P_19", internal::XmlString<"P_19C">>;

class BrakZwolnienia : public internal::XmlSequence {
    internal::XmlBoolTrue<"P_19N"> _P_19N{};
public:
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
};

class Zwolnienie : public internal::XmlNamedNode<"Zwolnienie"> {
    using P_19A = internal::XmlSingleton<internal::XmlString<"P_19A">>;
    using P_19B = internal::XmlSingleton<internal::XmlString<"P_19B">>;
    using P_19C = internal::XmlSingleton<internal::XmlString<"P_19C">>;
    using P_19N = internal::XmlSingleton<internal::XmlBoolTrue<"P_19N">>;
    struct P_19 : public internal::XmlSequence {
        internal::XmlSingleton<internal::XmlBoolTrue<"P_19">> _P_19;
        internal::XmlSwitch<P_19A, P_19B, P_19C> _switch;
        QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
        P_19(P_19A przepis) : _switch{std::move(przepis)} {}
        P_19(P_19B przepis) : _switch{std::move(przepis)} {}
        P_19(P_19C przepis) : _switch{std::move(przepis)} {}
    };
    internal::XmlSwitch<P_19, P_19N> _switch;
public:
    enum class Podstawa {
        USTAWA_O_VAT,
        DYREKTYWA_2006_112_WE,
        INNE
    };
    Zwolnienie();
    Zwolnienie(Podstawa podstawa, QString przepis);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class NowySrodekTransportu : public internal::XmlNamedNode<"NowySrodekTransportu"> {
public:
    using NumerVIN = internal::XmlSingleton<internal::XmlString<"P_22B1">>;
    using NumerNadwozia = internal::XmlSingleton<internal::XmlString<"P_22B2">>;
    using NumerPodwozia = internal::XmlSingleton<internal::XmlString<"P_22B3">>;
    using NumerRamy = internal::XmlSingleton<internal::XmlString<"P_22B4">>;

    class PojazdLadowy : public internal::XmlSequence {
        internal::XmlString<"P_22B"> _P_22B;
        std::optional<internal::XmlSwitch<NumerVIN, NumerNadwozia, NumerPodwozia, NumerRamy>> _numer;
        std::optional<internal::XmlString<"P_22BT">> _P_22BT;
    public:
        PojazdLadowy(QString przebieg,
                     std::optional<internal::XmlSwitch<NumerVIN, NumerNadwozia, NumerPodwozia, NumerRamy>> numer = std::nullopt,
                     std::optional<QString> typ = std::nullopt);
        QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
    };

    class JednostkaPlywajaca : public internal::XmlSequence {
        internal::XmlString<"P_22C"> _P_22C;
        std::optional<internal::XmlString<"P_22C1">> _P_22C1;
    public:
        JednostkaPlywajaca(QString godziny, std::optional<QString> numerKadluba = std::nullopt);
        QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
    };

    class StatekPowietrzny : public internal::XmlSequence {
        internal::XmlString<"P_22D"> _P_22D;
        std::optional<internal::XmlString<"P_22D1">> _P_22D1;
    public:
        StatekPowietrzny(QString godziny, std::optional<QString> numerFabryczny = std::nullopt);
        QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
    };

private:
    internal::XmlDate<"P_22A"> _P_22A;
    internal::XmlString<"P_NrWierszaNST"> _P_NrWierszaNST;
    std::optional<internal::XmlString<"P_22BMK">> _P_22BMK;
    std::optional<internal::XmlString<"P_22BMD">> _P_22BMD;
    std::optional<internal::XmlString<"P_22BK">> _P_22BK;
    std::optional<internal::XmlString<"P_22BNR">> _P_22BNR;
    std::optional<internal::XmlString<"P_22BRP">> _P_22BRP;
    internal::XmlSwitch<PojazdLadowy, JednostkaPlywajaca, StatekPowietrzny> _switch;
public:
    NowySrodekTransportu(QDate data, QString nrWiersza, PojazdLadowy pojazd);
    NowySrodekTransportu(QDate data, QString nrWiersza, JednostkaPlywajaca jednostka);
    NowySrodekTransportu(QDate data, QString nrWiersza, StatekPowietrzny statek);
    void setMarka(QString marka);
    void setModel(QString model);
    void setKolor(QString kolor);
    void setNrRejestracyjny(QString nr);
    void setRokProdukcji(QString rok);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class NoweSrodkiTransportu : public internal::XmlNamedNode<"NoweSrodkiTransportu"> {
    class P_22 : public internal::XmlSequence {
        internal::XmlBoolTrue<"P_22"> _P_22{};
        internal::XmlBool<"P_42_5"> _P_42_5;
        internal::XmlVector<NowySrodekTransportu, 10'000> _NowySrodekTransportu; //TODO: Make sure it contains at least one element
    public:
        P_22(bool obowiazek_42_5, internal::XmlVector<NowySrodekTransportu, 10'000> nst);
        QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
    };
    using P_22N = internal::XmlSingleton<internal::XmlBoolTrue<"P_22N">>;
    internal::XmlSwitch<P_22, P_22N> _switch;
public:
    NoweSrodkiTransportu();
    NoweSrodkiTransportu(bool obowiazek_42_5, internal::XmlVector<NowySrodekTransportu, 10'000> nst);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class PMarzy : public internal::XmlNamedNode<"PMarzy"> {
    using P_PMarzy_2 = internal::XmlSingleton<internal::XmlBoolTrue<"P_PMarzy_2">>;
    using P_PMarzy_3_1 = internal::XmlSingleton<internal::XmlBoolTrue<"P_PMarzy_3_1">>;
    using P_PMarzy_3_2 = internal::XmlSingleton<internal::XmlBoolTrue<"P_PMarzy_3_2">>;
    using P_PMarzy_3_3 = internal::XmlSingleton<internal::XmlBoolTrue<"P_PMarzy_3_3">>;
    using P_PMarzyN = internal::XmlSingleton<internal::XmlBoolTrue<"P_PMarzyN">>;

    struct P_PMarzy : public internal::XmlSequence {
        internal::XmlBoolTrue<"P_PMarzy"> _P_PMarzy{};
        internal::XmlSwitch<
            P_PMarzy_2,
            P_PMarzy_3_1,
            P_PMarzy_3_2,
            P_PMarzy_3_3
            > _switch;
        P_PMarzy(decltype(_switch) value);
        QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
    };
    internal::XmlSwitch<P_PMarzy, P_PMarzyN> _switch;
public:
    enum class ProceduraMarzy {
        BLA_BIUR_PODROZY,
        TOWARY_UZYWANE,
        DZIELA_SZTUKI,
        PRZEDMIOTY_KOLEKCJONERSKIE_I_ANTYKI,
        BRAK_PROCEDURY_MARZY
    };
    PMarzy(ProceduraMarzy procedura=ProceduraMarzy::BRAK_PROCEDURY_MARZY);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
private:
    static decltype(PMarzy::_switch) createSwitch(ProceduraMarzy procedura);
};

class Adnotacje : public internal::XmlNamedNode<"Adnotacje"> {
    //Metoda kasowa
    internal::XmlBool<"P_16"> _P_16;
    //Samofakturowanie
    internal::XmlBool<"P_17"> _P_17;
    //Odwrotne obciążenie
    internal::XmlBool<"P_18"> _P_18;
    //Mechanizm podzielonej płatnośc
    internal::XmlBool<"P_18A"> _P_18A;
    Zwolnienie _Zwolnienie;
    NoweSrodkiTransportu _NoweSrodkiTransportu;
    //VAT: Faktura WE uproszczona
    internal::XmlBool<"P_23"> _P_23;
    PMarzy _PMarzy;
    enum Flagi {
        BRAK = 0,
        METODA_KASOWA = 1,
        SAMOFAKTUROWANIE = 1 << 1,
        ODWROTNE_OBCIAZENIE= 1 << 2,
        MECHANIZM_PODZIELONEJ_PLATNOSCI= 1 << 3,
        FAKTURA_WE_UPROSZCZONA= 1 << 4
    };
public:
    Adnotacje(
        Flagi flagi=BRAK,
        Zwolnienie zwolnienie={},
        NoweSrodkiTransportu noweSrodkiTransportu={},
        PMarzy proceduraMarzy={}
        );
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

} // namespace ksef::FA3
