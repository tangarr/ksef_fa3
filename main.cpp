// #include "mainwindow.h"
// #include <QApplication>

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     MainWindow w;
//     w.show();
//     return a.exec();
// }

// #include <QTimeZone>
// #include "schemat.hpp"

// int main() {
//     using namespace ksef::FA3;

//     Naglowek naglowek{QDateTime{QDate{2026,2,1}, QTime{}, QTimeZone::Initialization::UTC}, "SamploFaktur"};
//     Podmiot1 podmiot1{DaneIdentyfikacyjne{"9999999999", "ABC AGD sp. z o. o."}, Adres{"PL", "ul. Kwiatowa 1 m. 2", "00-001 Warszawa"}};
//     podmiot1.addDaneKontaktowe(DaneKontaktowe{"abc@abc.pl", "667444555"});
//     Podmiot2 podmiot2{DaneIdentyfikacyjne2{NabywcaPolski{"1111111111"}, "F.H.U. Jan Kowalski"}, false, false};
//     podmiot2.setAdres(Adres{"PL", "ul. Polna 1", "00-001 Warszawa"});
//     podmiot2.addDaneKontaktowe(DaneKontaktowe{"jan@kowalski.pl", "555777999"});
//     podmiot2.setNrKlienta("fdfd778343");
//     Adnotacje adnotacje{};
//     Fa fa{"PLN", QDate(2026,02,15), "FV2026/02/150", "2051", std::move(adnotacje), ksef::FA3::RodzajFaktury::Rodzaj::VAT};
//     Faktura faktura{
//         std::move(naglowek),
//         std::move(podmiot1),
//         std::move(podmiot2),
//         std::move(fa)
//     };
//     QString out;
//     QXmlStreamWriter xmlStreamWriter{&out};
//     xmlStreamWriter.setAutoFormatting(true);
//     xmlStreamWriter.writeStartDocument();
//     faktura.write(xmlStreamWriter);
//     xmlStreamWriter.writeEndDocument();
//     qDebug().noquote() << out;
// }

int main() {

}
