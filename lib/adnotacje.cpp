#include "adnotacje.hpp"
#include "internal/xml_helpers.hpp"

namespace ksef::FA3 {


QVector<std::reference_wrapper<const internal::XmlNode>> BrakZwolnienia::elements() const
{
    return internal::elements_to_vector(
        this->_P_19N
        );
}

Zwolnienie::Zwolnienie() :
    _switch{P_19N{}}
{
}

Zwolnienie::Zwolnienie(Podstawa podstawa, QString przepis) :
    _switch{[&]() -> decltype(_switch) {
        switch (podstawa) {
        case Podstawa::USTAWA_O_VAT:
            return P_19{P_19A{std::move(przepis)}};
        case Podstawa::DYREKTYWA_2006_112_WE:
            return P_19{P_19B{std::move(przepis)}};
        case Podstawa::INNE:
            return P_19{P_19C{std::move(przepis)}};
        }
        std::unreachable();
    }()}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> Zwolnienie::elements() const
{
    return internal::elements_to_vector(
        this->_switch
        );
}

NowySrodekTransportu::PojazdLadowy::PojazdLadowy(QString przebieg,
                           std::optional<internal::XmlSwitch<NumerVIN, NumerNadwozia, NumerPodwozia, NumerRamy>> numer,
                           std::optional<QString> typ) :
    _P_22B{std::move(przebieg)},
    _numer{std::move(numer)},
    _P_22BT{std::move(typ).transform([](QString s) {
        return internal::XmlString<"P_22BT">{std::move(s)};
    })}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> NowySrodekTransportu::PojazdLadowy::elements() const {
    return internal::elements_to_vector(
        this->_P_22B,
        this->_numer,
        this->_P_22BT
        );
}

NowySrodekTransportu::JednostkaPlywajaca::JednostkaPlywajaca(QString godziny, std::optional<QString> numerKadluba) :
    _P_22C{std::move(godziny)},
    _P_22C1{std::move(numerKadluba).transform([](QString s) {
        return internal::XmlString<"P_22C1">{std::move(s)};
    })}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> NowySrodekTransportu::JednostkaPlywajaca::elements() const {
    return internal::elements_to_vector(
        this->_P_22C,
        this->_P_22C1
        );
}

NowySrodekTransportu::StatekPowietrzny::StatekPowietrzny(QString godziny, std::optional<QString> numerFabryczny) :
    _P_22D{std::move(godziny)},
    _P_22D1{std::move(numerFabryczny).transform([](QString s) {
        return internal::XmlString<"P_22D1">{std::move(s)};
    })}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> NowySrodekTransportu::StatekPowietrzny::elements() const {
    return internal::elements_to_vector(
        this->_P_22D,
        this->_P_22D1
        );
}

NowySrodekTransportu::NowySrodekTransportu(QDate data, QString nrWiersza, PojazdLadowy pojazd) :
    _P_22A{data},
    _P_NrWierszaNST{std::move(nrWiersza)},
    _switch{std::move(pojazd)}
{
}

NowySrodekTransportu::NowySrodekTransportu(QDate data, QString nrWiersza, JednostkaPlywajaca jednostka) :
    _P_22A{data},
    _P_NrWierszaNST{std::move(nrWiersza)},
    _switch{std::move(jednostka)}
{
}

NowySrodekTransportu::NowySrodekTransportu(QDate data, QString nrWiersza, StatekPowietrzny statek) :
    _P_22A{data},
    _P_NrWierszaNST{std::move(nrWiersza)},
    _switch{std::move(statek)}
{
}

void NowySrodekTransportu::setMarka(QString marka) {
    _P_22BMK.emplace(std::move(marka));
}

void NowySrodekTransportu::setModel(QString model) {
    _P_22BMD.emplace(std::move(model));
}

void NowySrodekTransportu::setKolor(QString kolor) {
    _P_22BK.emplace(std::move(kolor));
}

void NowySrodekTransportu::setNrRejestracyjny(QString nr) {
    _P_22BNR.emplace(std::move(nr));
}

void NowySrodekTransportu::setRokProdukcji(QString rok) {
    _P_22BRP.emplace(std::move(rok));
}

QVector<std::reference_wrapper<const internal::XmlNode>> NowySrodekTransportu::elements() const {
    return internal::elements_to_vector(
        this->_P_22A,
        this->_P_NrWierszaNST,
        this->_P_22BMK,
        this->_P_22BMD,
        this->_P_22BK,
        this->_P_22BNR,
        this->_P_22BRP,
        this->_switch
        );
}

NoweSrodkiTransportu::NoweSrodkiTransportu() :
    _switch{P_22N{}}
{
}

NoweSrodkiTransportu::NoweSrodkiTransportu(bool obowiazek_42_5, internal::XmlVector<NowySrodekTransportu, 10'000> nst) :
    _switch{P_22{obowiazek_42_5, std::move(nst)}}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> NoweSrodkiTransportu::elements() const
{
    return internal::elements_to_vector(
        this->_switch
        );
}

PMarzy::PMarzy(ProceduraMarzy procedura) :
    _switch{createSwitch(procedura)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> PMarzy::elements() const
{
    return internal::elements_to_vector(
        this->_switch
        );
}

decltype(PMarzy::_switch) PMarzy::createSwitch(ProceduraMarzy procedura)
{
    switch (procedura) {
    case ProceduraMarzy::BLA_BIUR_PODROZY:
        return P_PMarzy(P_PMarzy_2{});
    case ProceduraMarzy::TOWARY_UZYWANE:
        return P_PMarzy(P_PMarzy_3_1{});
    case ProceduraMarzy::DZIELA_SZTUKI:
        return P_PMarzy(P_PMarzy_3_2{});
    case ProceduraMarzy::PRZEDMIOTY_KOLEKCJONERSKIE_I_ANTYKI:
        return P_PMarzy(P_PMarzy_3_3{});
    case ProceduraMarzy::BRAK_PROCEDURY_MARZY:
        return P_PMarzyN{};
    }
    std::unreachable();
}

Adnotacje::Adnotacje(Flagi flagi, Zwolnienie zwolnienie, NoweSrodkiTransportu noweSrodkiTransportu, PMarzy proceduraMarzy) :
    _P_16{(flagi & Adnotacje::METODA_KASOWA) != 0},
    _P_17{(flagi & Adnotacje::SAMOFAKTUROWANIE) != 0},
    _P_18{(flagi & Adnotacje::ODWROTNE_OBCIAZENIE) != 0},
    _P_18A{(flagi & Adnotacje::MECHANIZM_PODZIELONEJ_PLATNOSCI) != 0},
    _Zwolnienie{std::move(zwolnienie)},
    _NoweSrodkiTransportu{std::move(noweSrodkiTransportu)},
    _P_23{(flagi & Adnotacje::FAKTURA_WE_UPROSZCZONA) != 0},
    _PMarzy{std::move(proceduraMarzy)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode>> Adnotacje::elements() const
{
    return internal::elements_to_vector(
        this->_P_16,
        this->_P_17,
        this->_P_18,
        this->_P_18A,
        this->_Zwolnienie,
        this->_NoweSrodkiTransportu,
        this->_P_23,
        this->_PMarzy
        );

}

PMarzy::P_PMarzy::P_PMarzy(decltype(PMarzy::P_PMarzy::_switch) value) :
    _switch{std::move(value)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode> > PMarzy::P_PMarzy::elements() const {
    return internal::elements_to_vector(
        this->_P_PMarzy,
        this->_switch
        );
}

QVector<std::reference_wrapper<const internal::XmlNode> > Zwolnienie::P_19::elements() const {
    return internal::elements_to_vector(
        this->_P_19,
        this->_switch
        );
}

NoweSrodkiTransportu::P_22::P_22(bool obowiazek_42_5, internal::XmlVector<NowySrodekTransportu, 10'000> nst) :
    _P_42_5{obowiazek_42_5},
    _NowySrodekTransportu{std::move(nst)}
{
}

QVector<std::reference_wrapper<const internal::XmlNode> > NoweSrodkiTransportu::P_22::elements() const
{
    return internal::elements_to_vector(
        this->_P_22,
        this->_P_42_5,
        this->_NowySrodekTransportu
        );
}

} // namespace ksef::FA3
