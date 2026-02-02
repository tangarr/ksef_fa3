#include <gtest/gtest.h>
#include <QString>
#include <QFile>
#include <QXmlStreamReader>
#include <format>
#include "schemat.hpp"

namespace ksef::FA3 {
class TestNode : public internal::XmlNode {
private:
    internal::XmlAttribute _attribute{"Attribute", "AttributeValue"};
    internal::XmlString<"ChildNode"> _childNode{"ChildValue"};
public:
    QStringView name() const override {
        return QStringLiteral("TestNode");
    }
    QVector<std::reference_wrapper<const FA3::internal::XmlNode> > elements() const override{
        return { _childNode };
    }
    QVector<std::reference_wrapper<const FA3::internal::XmlAttribute> > attributes() const override {
        return { _attribute };
    }
    std::optional<QStringView> defaultNamespace() const override {
        return std::make_optional(QStringLiteral("DefaultNamespace"));
    }
    const QVector<FA3::internal::XmlNamespace> &namespaces() const override {
        static QVector<internal::XmlNamespace> vector { internal::XmlNamespace{"NamespaceURI", "NamespacePrefix"} };
        return vector;
    }
};

inline std::expected<QString, QString> toXML(internal::XmlNode &node) {
    QString result;
    QXmlStreamWriter writer(&result);
    auto write_result = node.write(writer);
    if (!write_result.has_value()) {
        return std::unexpected{write_result.error()};
    }
    return std::expected<QString, QString>{result};
}

inline testing::AssertionResult compareXML(const QString &expectedXml, const QString &actualXml, bool ignore_namespaces = false) {
    QXmlStreamReader expected(expectedXml);
    QXmlStreamReader actual(actualXml);

    while (!expected.atEnd()) {
        QXmlStreamReader::TokenType expType = expected.readNext();
        QXmlStreamReader::TokenType actType = actual.readNext();

        while (expected.isWhitespace()) {
            expType = expected.readNext();
        }
        while (actual.isWhitespace()) {
            actType = actual.readNext();
        }

        if (expType != actType) {
            return testing::AssertionFailure()
            << std::format(
                "Token type mismatch. Expected: {}, Actual: {} at line{}",
                expected.tokenString().toStdString(),
                actual.tokenString().toStdString(),
                expected.lineNumber()
                );
        }
        if (expected.isStartElement()) {
            if (expected.name() != actual.name()) {
                return testing::AssertionFailure()
                << std::format(
                    "Element name mismatch. Expected: {}, Actual: {}",
                    expected.name().toString().toStdString(),
                    actual.name().toString().toStdString()
                    );
            }
            if (expected.attributes() != actual.attributes()) {
                return testing::AssertionFailure()
                << std::format(
                    "Attributes mismatch in element: {}",
                    expected.name().toString().toStdString());
            }
            {
                auto expNs = expected.namespaceDeclarations();
                auto actNs = actual.namespaceDeclarations();
                std::sort(expNs.begin(), expNs.end(), [](const auto &a, const auto &b) {
                    return a.prefix() < b.prefix();
                });
                std::sort(actNs.begin(), actNs.end(), [](const auto &a, const auto &b) {
                    return a.prefix() < b.prefix();
                });
                if (expNs != actNs) {
                    auto message = std::format(
                        "Namespaces mismatch in element: {}",
                        expected.name().toString().toStdString());
                    if (ignore_namespaces) {
                        std::cout << "WARNING: " << message << "\n";
                    }
                    else {
                        return testing::AssertionFailure() << message;
                    }
                }
            }
        }
        if (expected.isCharacters() && !expected.isWhitespace()) {
            auto expected_trimmed = expected.text().trimmed();
            auto actual_trimmed = actual.text().trimmed();
            if (expected_trimmed != actual_trimmed) {
                return testing::AssertionFailure()
                << std::format(
                    "Text mismatch. Expected: {}, Actual: {}",
                    expected_trimmed.toString().toStdString(),
                    actual_trimmed.toString().toStdString()
                    );
            }
        }
    }
    while (!actual.atEnd()) {
        actual.readNext();
        if (actual.isWhitespace()) {
            continue;
        }
        return testing::AssertionFailure()
               << std::format(
                      "Actual XML has extra content at the end ({}).",
                      actual.tokenString().toStdString()
                      );
    }
    return testing::AssertionSuccess();
}

inline testing::AssertionResult compareXMLFile(const QString &filePath, const QString &actualXml, bool ignore_namespaces = false) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return testing::AssertionFailure() << "Could not open file: " << filePath.toStdString();
    }
    QString expectedXml = QString::fromUtf8(file.readAll());
    return compareXML(expectedXml, actualXml, ignore_namespaces);
}

}
