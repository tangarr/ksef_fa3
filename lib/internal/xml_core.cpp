#include "xml_core.hpp"

namespace ksef::FA3 {
namespace internal {

QVector<std::reference_wrapper<const XmlAttribute> > XmlNode::attributes() const {
    return {};
}

QVector<std::reference_wrapper<const internal::XmlNode>> XmlNode::elements() const {
    return {};
}

std::optional<QStringView> XmlNode::value() const {
    return std::nullopt;
}

std::optional<QStringView> XmlNode::defaultNamespace() const {
    return std::nullopt;
}

const QVector<XmlNamespace> &XmlNode::namespaces() const
{
    static QVector<XmlNamespace> empty;
    return empty;
}

std::expected<void, QString> XmlNode::write(QXmlStreamWriter &writer) const {
    writer.writeStartElement(this->name());
    if (auto const& defaultNamespace = this->defaultNamespace(); defaultNamespace.has_value()) {
        writer.writeDefaultNamespace(*defaultNamespace);
    }
    for (auto const &namespace_ : this->namespaces()) {
        writer.writeNamespace(namespace_.uri, namespace_.prefix);
    }
    for (auto const &attributeWrapper : this->attributes()) {
        auto const &attribute = attributeWrapper.get();
        writer.writeAttribute(attribute.name, attribute.value);
    }
    for (auto const &elementWrapper: this->elements()) {
        auto result = elementWrapper.get().write(writer);
        if (!result.has_value()) {
            return std::unexpected{result.error()};
        }
    }
    if (auto const &value=this->value(); value.has_value()) {
        writer.writeCharacters(*value);
    }
    writer.writeEndElement();
    if (writer.error() != QXmlStreamWriter::Error::None) {
        return std::unexpected{writer.errorString()};
    }
    return std::expected<void, QString>{};
}

} // namespace internal
} // namespace ksef::FA3
