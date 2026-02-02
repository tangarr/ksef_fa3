#pragma once

#include <concepts>
#include <expected>
#include <optional>
#include <utility>
#include <QString>
#include <QVector>
#include <QDate>
#include <QXmlStreamWriter>

namespace ksef::FA3 {

namespace internal {
struct XmlAttribute {
public:
    QString name;
    QString value;
};
struct XmlNamespace {
    QString uri;
    QString prefix;
};
class XmlNode {
public:
    virtual ~XmlNode() = default;
    virtual QStringView name() const = 0;
    virtual QVector<std::reference_wrapper<const internal::XmlNode>> elements() const;
    virtual QVector<std::reference_wrapper<const XmlAttribute>> attributes() const;
    virtual std::optional<QStringView> value() const;
    virtual std::optional<QStringView> defaultNamespace() const;
    virtual const QVector<internal::XmlNamespace> &namespaces() const;
    [[nodiscard]] std::expected<void, QString> write(QXmlStreamWriter &writer) const;
};

template<typename T>
concept XmlNodeDerived = std::derived_from<T, XmlNode>;

class XmlSequence {
public:
    virtual ~XmlSequence() = default;
    virtual QVector<std::reference_wrapper<const internal::XmlNode>> elements() const = 0;
};

template<typename T>
concept XmlSequenceDerived = std    ::derived_from<T, XmlSequence>;

template<typename T>
concept XmlNodeOrSequenceDerived = XmlNodeDerived<T> || XmlSequenceDerived<T>;

template<typename... Ts>
    requires (XmlNodeOrSequenceDerived<Ts> && ...)
struct XmlSwitch : std::variant<Ts...> {
    using std::variant<Ts...>::variant;
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const {
        return std::visit([](const auto& t){
            return t.elements();
        }, *this);
    }
};

template<std::size_t N>
struct FixedString {
    char16_t data[N];
    constexpr FixedString(const char (&str)[N]) {
        for (std::size_t i = 0; i < N; ++i) {
            data[i] = static_cast<unsigned char>(str[i]);
        }
    }
};

template <FixedString S>
class XmlNamedNode : public XmlNode {
public:
    QStringView name() const override {
        return QStringView(S.data);
    }
    ~XmlNamedNode() override = default;
};

template <FixedString S>
class XmlValue : public XmlNamedNode<S> {
    QString _value;
protected:
    XmlValue(QString value) : _value{std::move(value)} {};
public:
    std::optional<QStringView> value() const override {
        return QStringView(this->_value);
    }
};

template <FixedString S>
class XmlBool : public XmlValue<S> {
public:
    XmlBool(bool value) : XmlValue<S>(value ? QStringLiteral("1") : QStringLiteral("2")) {}
};

template <FixedString S>
class XmlBoolTrue : public XmlValue<S> {
public:
    XmlBoolTrue() : XmlValue<S>(QStringLiteral("1")) {}
};

template <FixedString S>
class XmlString : public XmlValue<S> {
public:
    XmlString(QString value) : XmlValue<S>(std::move(value)) {}
};

template <FixedString S>
class XmlDateTime : public XmlValue<S> {
public:
    XmlDateTime(QDateTime dateTime) : XmlValue<S>(dateTime.toUTC().toString(Qt::ISODate)) {}
};

template <FixedString S>
class XmlDate : public XmlValue<S> {
public:
    XmlDate(QDate date) : XmlValue<S>(date.toString(Qt::ISODate)) {}
};

template <FixedString S>
class XmlDecimal : public XmlValue<S> {
public:
    XmlDecimal(QString number) : XmlValue<S>{std::move(number)} {}
};

template<XmlNodeDerived T, std::size_t MAX>
class XmlVector {
    QVector<T> vector;
public:
    std::size_t size() const { return vector.size(); }
    auto begin() { return vector.begin(); }
    auto end() { return vector.end(); }
    auto begin() const { return vector.begin(); }
    auto end() const { return vector.end(); }
    std::expected<void, QString> push_back(const T& t) {
        if (vector.size() >= static_cast<long long>(MAX))
            return std::unexpected<QString>(QStringLiteral("Maximum number of %1 elements exceeded (max: %2)").arg(t.name()).arg(MAX));
        vector.push_back(t);
        return std::expected<void, QString>();
    }
    std::expected<void, QString> push_back(T&& t) {
        if (vector.size() >= static_cast<long long>(MAX))
            return std::unexpected<QString>(QStringLiteral("Maximum number of %1 elements exceeded (max: %2)").arg(t.name()).arg(MAX));
        vector.push_back(std::move(t));
        return std::expected<void, QString>();
    }
    void clear() {
        vector.clear();
    }
};

template <XmlNodeDerived T>
struct XmlSingleton : public XmlSequence {
    T value;
    template <typename... Args>
    XmlSingleton(Args&&... args) : value(std::forward<Args>(args)...) {}
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final {
        return { value };
    }
};

template <FixedString S, XmlNodeDerived T>
struct XmlMarkedElement : public internal::XmlSequence {
    internal::XmlBoolTrue<S> marker{};
    T value;
    template <typename... Args> requires std::constructible_from<T, Args...>
    XmlMarkedElement(Args&&... args) : value(std::forward<Args>(args)...) {}
    QVector<std::reference_wrapper<const internal::XmlNode>> elements() const final {
        return { marker, value };
    }
};

} // namespace internal

} // namespace ksef::FA3
