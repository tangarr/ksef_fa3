#pragma once

#include "internal/xml_core.hpp"
#include "typy_adresowe.hpp"
#include "typy_identyfikacyjne.hpp"
#include "adnotacje.hpp"
#include "platnosc.hpp"
#include "transport.hpp"

namespace ksef::FA3 {

class OkresFA : public internal::XmlNamedNode<"OkresFa"> {
    internal::XmlDate<"P_6_Od"> _P_6_Od;
    internal::XmlDate<"P_6_Do"> _P_6_Do;
public:
    OkresFA(QDate okres_od, QDate okres_do);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class RodzajFaktury : public internal::XmlValue<"RodzajFaktury">{
public:
    enum class Rodzaj {
        VAT,
        KOR,
        ZAL,
        ROZ,
        UPR,
        KOR_ZAL,
        KOR_ROZ
    };
    static constexpr QString ToString(Rodzaj rodzaj) {
        switch (rodzaj) {
        case Rodzaj::VAT:
            return QStringLiteral("VAT");
        case Rodzaj::KOR:
            return QStringLiteral("KOR");
        case Rodzaj::ZAL:
            return QStringLiteral("ZAL");
        case Rodzaj::ROZ:
            return QStringLiteral("ROZ");
        case Rodzaj::UPR:
            return QStringLiteral("UPR");
        case Rodzaj::KOR_ZAL:
            return QStringLiteral("KOR_ZAL");
        case Rodzaj::KOR_ROZ:
            return QStringLiteral("KOR_ROZ");
        };
        std::unreachable();
    }
public:
    RodzajFaktury(Rodzaj rodzaj) : XmlValue(ToString(rodzaj)) {}
};

class TypKorekty : public internal::XmlValue<"TypKorekty">{
public:
    enum Typ {
        W_DACIE_FAKTURY_PIERWOTNEJ=1,
        W_DACIE_FAKTURY_KORYGUJACEJ=2,
        W_DACIE_INNEJ=3,
    };
    static constexpr QString ToString(Typ rola) {
        return QString::number(rola);
    }
public:
    TypKorekty(Typ typ) : XmlValue(ToString(typ)) {}
};

class DaneFaKorygowanej : public internal::XmlNamedNode<"DaneFaKorygowanej">{
    using NrKSeF = internal::XmlMarkedElement<"NrKSeF", internal::XmlString<"NrKSeFFaKorygowanej">>;
    using NrKSeFN = internal::XmlSingleton<internal::XmlBoolTrue<"NrKSeFN">>;

    internal::XmlDate<"DataWystFaKorygowanej"> _DataWystFaKorygowanej;
    internal::XmlString<"NrFaKorygowanej"> _NrFaKorygowanej;
    internal::XmlSwitch<NrKSeF, NrKSeFN> _switch;
public:
    DaneFaKorygowanej(QDate dataWystFaKorygowanej, QString nrFaKorygowanej);
    DaneFaKorygowanej(QDate dataWystFaKorygowanej, QString nrFaKorygowanej, QString nrKSeFFaKorygowanej);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};


class Podmiot1K : public internal::XmlNamedNode<"Podmiot1K">{
    std::optional<internal::XmlString<"PrefiksPodatnika">> _PrefiksPodatnika;
    DaneIdentyfikacyjne _DaneIdentyfikacyjne;
    Adres _Adres;
public:
    Podmiot1K(DaneIdentyfikacyjne daneIdentyfikacyjne, Adres adres, std::optional<QString> prefiksPodatnika);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};


class Podmiot2K : public internal::XmlNamedNode<"Podmiot2K">{
    DaneIdentyfikacyjne2 _DaneIdentyfikacyjne;
    std::optional<Adres> _Adres;
    std::optional<internal::XmlString<"IDNabywcy">> _IDNabywcy;
public:
    Podmiot2K(DaneIdentyfikacyjne2 daneIdentyfikacyjne, std::optional<Adres> adres, std::optional<QString> idNabywcy);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class ZaliczkaCzesciowa : public internal::XmlNamedNode<"ZaliczkaCzesciowa">{
    internal::XmlDate<"P_6Z"> _P_6Z;
    internal::XmlDecimal<"P_15Z"> _P_15Z;
    std::optional<internal::XmlDecimal<"KursWalutyZW">> _KursWalutyZW;
public:
    ZaliczkaCzesciowa(QDate data, QString kwota, std::optional<QString> kurs_wymiany);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};


class DodatkowyOpis : public internal::XmlNamedNode<"DodatkowyOpis">{
    std::optional<internal::XmlString<"NrWiersza">> _NrWiersza;
    internal::XmlString<"Klucz"> _Klucz;
    internal::XmlString<"Wartosc"> _Wartosc;
public:
    DodatkowyOpis(std::optional<QString> nrWiersza, QString klucz, QString wartosc);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};


class FakturaZaliczkowa : public internal::XmlNamedNode<"FakturaZaliczkowa">{
    using NrKSeFZN = internal::XmlMarkedElement<"NrKSeFZN", internal::XmlString<"NrFaZaliczkowej">>;
    using NrKSeFFaZaliczkowej = internal::XmlSingleton<internal::XmlString<"NrKSeFFaZaliczkowej">>;
    internal::XmlSwitch<NrKSeFZN, NrKSeFFaZaliczkowej> _switch;
public:
    FakturaZaliczkowa(bool wystawiona_poza_ksef, QString nrFaZaliczkowej);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};


class FaWiersz : public internal::XmlNamedNode<"FaWiersz">{
public:
    class P_12 : public internal::XmlValue<"P_12">{
    public:
        enum class Stawka {
            KR_0,
            WDT_0,
            EX_0,
            ZW,
            OO,
            NP_I,
            NP_II
        };
        static QString ToString(unsigned char stawka) {
            return QString::number(stawka);
        }
        static constexpr QString ToString(Stawka stawka) {
            switch (stawka) {
            case Stawka::KR_0:
                return QStringLiteral("0 KR");
            case Stawka::WDT_0:
                return QStringLiteral("0 WDT");
            case Stawka::EX_0:
                return QStringLiteral("0 EX");
            case Stawka::ZW:
                return QStringLiteral("zw");
            case Stawka::OO:
                return QStringLiteral("oo");
            case Stawka::NP_I:
                return QStringLiteral("np I");
            case Stawka::NP_II:
                return QStringLiteral("np II");
            }
            std::unreachable();
        }
    public:
        P_12(auto stawka) : XmlValue{P_12::ToString(stawka)} {}
    };

    class GTU : public internal::XmlValue<"GTU">{
        static QString SymbolGTU(unsigned char gtu) {
            return QStringLiteral("GTU_%1").arg(static_cast<int>(gtu), 2, 10, '0');
        }
    public:
        GTU(unsigned char symbol) : XmlValue{SymbolGTU(symbol)} {}
    };

    class Procedura : public internal::XmlValue<"Procedura">{
    public:
        enum class Symbol {
            WSTO_EE,
            IED,
            TT_D,
            I_42,
            I_63,
            B_SPV,
            B_SPV_DOSTAWA,
            B_MPV_PROWIZJA
        };
        static constexpr QString ToString(unsigned char stawka) {
            return QString::number(stawka);
        }
        static constexpr QString ToString(Symbol symbol) {
            switch (symbol) {
            case Symbol::WSTO_EE:
                return QStringLiteral("WSTO_EE");
            case Symbol::IED:
                return QStringLiteral("IED");
            case Symbol::TT_D:
                return QStringLiteral("TT_D");
            case Symbol::I_42:
                return QStringLiteral("I_42");
            case Symbol::I_63:
                return QStringLiteral("I_63");
            case Symbol::B_SPV:
                return QStringLiteral("B_SPV");
            case Symbol::B_SPV_DOSTAWA:
                return QStringLiteral("B_SPV_DOSTAWA");
            case Symbol::B_MPV_PROWIZJA:
                return QStringLiteral("B_MPV_PROWIZJA");
            }
            std::unreachable();
        }        
    public:
        Procedura(Symbol procedura) : XmlValue{Procedura::ToString(procedura)} {}
    };

private:
    internal::XmlString<"NrWierszaFa"> _NrWierszaFa;
    std::optional<internal::XmlString<"UU_ID">> _UU_ID;
    std::optional<internal::XmlDate<"P_6A">> _P_6A;
    std::optional<internal::XmlString<"P_7">> _P_7;
    std::optional<internal::XmlString<"Indeks">> _Indeks;
    std::optional<internal::XmlString<"GTIN">> _GTIN;
    std::optional<internal::XmlString<"PKWiU">> _PKWiU;
    std::optional<internal::XmlString<"CN">> _CN;
    std::optional<internal::XmlString<"PKOB">> _PKOB;
    std::optional<internal::XmlString<"P_8A">> _P_8A;
    std::optional<internal::XmlDecimal<"P_8B">> _P_8B;
    std::optional<internal::XmlDecimal<"P_9A">> _P_9A;
    std::optional<internal::XmlDecimal<"P_9B">> _P_9B;
    std::optional<internal::XmlDecimal<"P_10">> _P_10;
    std::optional<internal::XmlDecimal<"P_11">> _P_11;
    std::optional<internal::XmlDecimal<"P_11A">> _P_11A;
    std::optional<internal::XmlDecimal<"P_11Vat">> _P_11Vat;
    std::optional<P_12> _P_12;
    std::optional<internal::XmlString<"P_12_XII">> _P_12_XII;
    std::optional<internal::XmlBoolTrue<"P_12_Zal_15">> _P_12_Zal_15;
    std::optional<internal::XmlDecimal<"KwotaAkcyzy">> _KwotaAkcyzy;
    std::optional<GTU> _GTU;
    std::optional<Procedura> _Procedura;
    std::optional<internal::XmlDecimal<"KursWaluty">> _KursWaluty;
    std::optional<internal::XmlBoolTrue<"StanPrzed">> _StanPrzed;
public:
    FaWiersz(unsigned int numerWiersza, bool stan_przed_korekty);
    void setUU_ID(QString UU_ID);
    void setNazwaTowaru(QString nazwa);
    void setIndeks(QString indeks);
    void setPKWiU(QString pkwiu);
    void setCN(QString cn);
    void setMiara(QString miara);
    void setIlosc(QString ilosc);
    void setCenaJednostkowa(QString cena_jednostkowa, bool netto);
    void setWartoscNetto(QString wartosc);
    void setWartoscBrutto(QString wartosc);
    void setKwotaVAT(QString kwota);
    void setStawkaVAT(P_12 stawka_vat);
    void setP_6A(QDate date);
    void setRabat(QString kwota);
    void setKursWaluty(QString kurs);
    void setGTU(unsigned char gtu);
    void setProcedura(Procedura::Symbol procedura);
    void setKwotaAkcyzy(QString kwota);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Korekta : public internal::XmlSequence {
public:
    class Zaliczka : public internal::XmlSequence {
        internal::XmlDecimal<"P_15ZK"> _P_15ZK;
        std::optional<internal::XmlDecimal<"KursWalutyZK">> _KursWalutyZK;
    public:
        Zaliczka(QString kwota, std::optional<QString> kursWaluty);
        QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
    };
private:
    std::optional<internal::XmlString<"PrzyczynaKorekty">> _PrzyczynaKorekty;
    std::optional<TypKorekty> _TypKorekty;
    internal::XmlVector<DaneFaKorygowanej, 50'000> _DaneFaKorygowanej; //TODO: Make sure to push at least 1 element
    std::optional<internal::XmlString<"OkresFaKorygowanej">> _OkresFaKorygowanej;
    std::optional<internal::XmlString<"NrFaKorygowany">> _NrFaKorygowany;
    std::optional<Podmiot1K> _Podmiot1K;
    internal::XmlVector<Podmiot2K, 101> _Podmiot2K;
    std::optional<Zaliczka> _Zaliczka;
public:
    void setPrzyczynaKorekty(QString przyczyna);
    void setTypKorekty(TypKorekty::Typ typ);
    std::expected<void, QString> addDaneFaKorygowanej(DaneFaKorygowanej dane);
    void setOkresFaKorygowanej(QString okres);
    void setNrFaKorygowany(QString nr);
    void setPodmiot1K(Podmiot1K podmiot);
    std::expected<void, QString> addPodmiot2K(Podmiot2K podmiot);
    void setZaliczka(Zaliczka zaliczka);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final;
};

class ZamowienieWiersz : public internal::XmlNamedNode<"ZamowienieWiersz"> {
public:
    class P_12Z : public internal::XmlValue<"P_12Z">{
    public:
        using Stawka = FaWiersz::P_12::Stawka;
        static QString ToString(unsigned char stawka) {
            return QString::number(stawka);
        }
        static constexpr QString ToString(Stawka stawka) {
            return FaWiersz::P_12::ToString(stawka);
        }
    public:
        P_12Z(auto stawka) : XmlValue{P_12Z::ToString(stawka)} {}
    };

    class GTUZ : public internal::XmlValue<"GTUZ">{
        static QString SymbolGTU(unsigned char gtu) {
            return QStringLiteral("GTU_%1").arg(static_cast<int>(gtu), 2, '0');
        }
    public:
        GTUZ(unsigned char symbol) : XmlValue{SymbolGTU(symbol)} {}
    };

    class ProceduraZ : public internal::XmlValue<"ProceduraZ">{
    public:
        enum class Symbol {
            WSTO_EE,
            IED,
            TT_D,
            B_SPV,
            B_SPV_DOSTAWA,
            B_MPV_PROWIZJA
        };
        static constexpr QString ToString(Symbol symbol) {
            switch (symbol) {
            case Symbol::WSTO_EE:
                return QStringLiteral("WSTO_EE");
            case Symbol::IED:
                return QStringLiteral("IED");
            case Symbol::TT_D:
                return QStringLiteral("TT_D");
            case Symbol::B_SPV:
                return QStringLiteral("B_SPV");
            case Symbol::B_SPV_DOSTAWA:
                return QStringLiteral("B_SPV_DOSTAWA");
            case Symbol::B_MPV_PROWIZJA:
                return QStringLiteral("B_MPV_PROWIZJA");
            }
        }
    public:
        ProceduraZ(Symbol procedura) : XmlValue{ProceduraZ::ToString(procedura)} {}
    };

private:
    internal::XmlString<"NrWierszaZam"> _NrWierszaZam;
    std::optional<internal::XmlString<"UU_IDZ">> _UU_IDZ;
    std::optional<internal::XmlString<"P_7Z">> _P_7Z;
    std::optional<internal::XmlString<"IndeksZ">> _IndeksZ;
    std::optional<internal::XmlString<"GTINZ">> _GTINZ;
    std::optional<internal::XmlString<"PKWiUZ">> _PKWiUZ;
    std::optional<internal::XmlString<"CNZ">> _CNZ;
    std::optional<internal::XmlString<"PKOBZ">> _PKOBZ;
    std::optional<internal::XmlString<"P_8AZ">> _P_8AZ;
    std::optional<internal::XmlDecimal<"P_8BZ">> _P_8BZ;
    std::optional<internal::XmlDecimal<"P_9AZ">> _P_9AZ;
    std::optional<internal::XmlDecimal<"P_11NettoZ">> _P_11NettoZ;
    std::optional<internal::XmlDecimal<"P_11VatZ">> _P_11VatZ;
    std::optional<P_12Z> _P_12Z;
    std::optional<internal::XmlString<"P_12Z_XII">> _P_12Z_XII;
    std::optional<internal::XmlBoolTrue<"P_12Z_Zal_15">> _P_12Z_Zal_15;
    std::optional<GTUZ> _GTUZ;
    std::optional<ProceduraZ> _ProceduraZ;
    std::optional<internal::XmlDecimal<"KwotaAkcyzyZ">> _KwotaAkcyzyZ;
    std::optional<internal::XmlBoolTrue<"StanPrzedZ">> _StanPrzedZ;
public:
    ZamowienieWiersz(unsigned int numerWiersza, bool stan_przed_korekty = false);
    void setUU_IDZ(QString uuid);
    void setNazwaTowaru(QString nazwa);
    void setIndeks(QString indeks);
    void setPKWiU(QString pkwiu);
    void setMiara(QString miara);
    void setIlosc(QString ilosc);
    void setCenaJednostkowa(QString cena);
    void setWartoscNetto(QString wartosc);
    void setKwotaVAT(QString kwota);
    void setStawkaVAT(P_12Z stawka);
    void setKwotaAkcyzy(QString kwota);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Zamowienie : public internal::XmlNamedNode<"Zamowienie"> {
    internal::XmlDecimal<"WartoscZamowienia"> _WartoscZamowienia;
    internal::XmlVector<ZamowienieWiersz, 10'000> _ZamowienieWiersz;
public:
    Zamowienie(QString wartoscZamowienia);
    std::expected<void, QString> addWiersz(ZamowienieWiersz wiersz);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

class Fa : public internal::XmlNamedNode<"Fa">{
    using P_6 = internal::XmlSingleton<internal::XmlDate<"P_6">>;

    template <internal::FixedString S1, internal::FixedString S2, internal::FixedString S3>
    struct SeqVat : public internal::XmlSequence {
        internal::XmlDecimal<S1> wartosc_netto;
        internal::XmlDecimal<S2> vat;
        std::optional<internal::XmlDecimal<S3>> vat_pln;
        SeqVat(QString netto, QString v, std::optional<QString> vpln = std::nullopt)
            : wartosc_netto{std::move(netto)}, vat{std::move(v)},
              vat_pln{std::move(vpln)} {}
        QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final {
            if (vat_pln.has_value()) {
                return {
                    this->wartosc_netto,
                    this->vat,
                    *this->vat_pln
                };
            }
            return {
                this->wartosc_netto,
                this->vat
            };
        }
    };

    struct VatPodst : public SeqVat<"P_13_1","P_14_1","P_14_1W"> { using SeqVat::SeqVat; };
    struct VatObniz1 : public SeqVat<"P_13_2","P_14_2","P_14_2W"> { using SeqVat::SeqVat; };
    struct VatObniz2 : public SeqVat<"P_13_3","P_14_3","P_14_3W"> { using SeqVat::SeqVat; };
    struct VatRyczaltTaxi : public SeqVat<"P_13_4","P_14_4","P_14_4W"> { using SeqVat::SeqVat; };
    struct VatProcSzczeg : public internal::XmlSequence {
        internal::XmlDecimal<"P_13_5"> P_13_5;
        std::optional<internal::XmlDecimal<"P_14_5">> P_14_5;
        VatProcSzczeg(QString netto, std::optional<QString> vat = std::nullopt)
            : P_13_5{std::move(netto)}, P_14_5{std::move(vat)} {}
        QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final {
            if (P_14_5.has_value()) {
                return {
                    this->P_13_5,
                    *this->P_14_5
                };
            }
            return {
                this->P_13_5
            };
        }
    };

    using DataDostawySwitch = internal::XmlSwitch <P_6, internal::XmlSingleton<OkresFA>>;
    internal::XmlString<"KodWaluty"> _KodWaluty;
    internal::XmlDate<"P_1"> _P_1;
    std::optional<internal::XmlString<"P_1M">> _P_1M;
    internal::XmlString<"P_2"> _P_2;
    internal::XmlVector<internal::XmlString<"WZ">, 1000> _WZ;
    std::optional<DataDostawySwitch> _DataDostawy;
    std::optional<VatPodst> _VatPodst;
    std::optional<VatObniz1> _VatObniz1;
    std::optional<VatObniz2> _VatObniz2;
    std::optional<VatRyczaltTaxi> _VatRyczaltTaxi;
    std::optional<VatProcSzczeg> _VatProcSzczeg;
    std::optional<internal::XmlDecimal<"P_13_6_1">> _P_13_6_1;
    std::optional<internal::XmlDecimal<"P_13_6_2">> _P_13_6_2;
    std::optional<internal::XmlDecimal<"P_13_6_3">> _P_13_6_3;
    std::optional<internal::XmlDecimal<"P_13_7">> _P_13_7;
    std::optional<internal::XmlDecimal<"P_13_8">> _P_13_8;
    std::optional<internal::XmlDecimal<"P_13_9">> _P_13_9;
    std::optional<internal::XmlDecimal<"P_13_10">> _P_13_10;
    std::optional<internal::XmlDecimal<"P_13_11">> _P_13_11;
    internal::XmlDecimal<"P_15"> _P_15;
    std::optional<internal::XmlDecimal<"KursWalutyZ">> _KursWalutyZ;
    Adnotacje _Adnotacje;
    RodzajFaktury _RodzajFaktury;
    std::optional<Korekta> _Korekta;
    internal::XmlVector<ZaliczkaCzesciowa, 31> _ZaliczkaCzesciowa;
    std::optional<internal::XmlBoolTrue<"FP">> _FP;
    std::optional<internal::XmlBoolTrue<"TP">> _TP;
    internal::XmlVector<DodatkowyOpis, 10'000> _DodatkowyOpis;
    internal::XmlVector<FakturaZaliczkowa, 100> _FakturaZaliczkowa;
    std::optional<internal::XmlBoolTrue<"ZwrotAkcyzy">> _ZwrotAkcyzy;
    internal::XmlVector<FaWiersz, 10'000> _FaWiersz;
    std::optional<Rozliczenie> _Rozliczenie;
    std::optional<Platnosc> _Platnosc;
    std::optional<WarunkiTransakcji> _WarunkiTransakcji;
    std::optional<Zamowienie> _Zamowienie;
public:
    Fa(QString kodWaluty,
       QDate data,
       QString numerFaktury,
       QString wartosc,
       Adnotacje adnotacje,
       RodzajFaktury::Rodzaj rodzaj);
    void setMiejsceWystawieniaFaktury(QString miejsce);
    std::expected<void, QString> addWZ(QString numerWZ);
    void setDataDostawy(QDate dataDostawy);
    void setDataDostawy(QDate dostawa_od, QDate dostawa_do);
    void setVatPodst(QString netto, QString vat, std::optional<QString> vat_pln = std::nullopt);
    void setVatObniz1(QString netto, QString vat, std::optional<QString> vat_pln = std::nullopt);
    void setVatObniz2(QString netto, QString vat, std::optional<QString> vat_pln = std::nullopt);
    void setVatRyczaltTaxi(QString netto, QString vat, std::optional<QString> vat_pln = std::nullopt);
    void setVatProcSzczeg(QString netto, std::optional<QString> vat = std::nullopt);
    void setP_13_6_1(QString kwota);
    void setP_13_6_2(QString kwota);
    void setP_13_6_3(QString kwota);
    void setP_13_7(QString kwota);
    void setP_13_8(QString kwota);
    void setP_13_9(QString kwota);
    void setP_13_10(QString kwota);
    void setP_13_11(QString kwota);
    void setKursWalutyZ(QString kurs);
    void setKorekta(Korekta korekta);
    std::expected<void, QString> addZaliczkaCzesciowa(ZaliczkaCzesciowa zaliczka);
    void setFP();
    void setTP();
    std::expected<void, QString> addDodatkowyOpis(DodatkowyOpis opis);
    std::expected<void, QString> addFakturaZaliczkowa(FakturaZaliczkowa fz);
    void setZwrotAkcyzy();
    std::expected<void, QString> addFaWiersz(FaWiersz wiersz);
    void setRozliczenie(Rozliczenie rozliczenie);
    void setPlatnosc(Platnosc platnosc);
    void setWarunkiTransakcji(WarunkiTransakcji wt);
    void setZamowienie(Zamowienie zamowienie);
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const override;
};

} // namespace ksef::FA3
