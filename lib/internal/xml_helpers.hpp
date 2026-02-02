#pragma once

#include "xml_core.hpp"

namespace ksef::FA3 {
namespace internal {

template<class... Ts>
std::size_t count_elements(Ts...) = delete;

inline std::size_t count_elements() {
    return 0;
}

template<XmlNodeDerived T, class... Ts>
std::size_t count_elements(const T&, const Ts&... ts) {
    return 1+count_elements(ts...);
}

template<XmlNodeDerived T, class... Ts>
std::size_t count_elements(const std::optional<T> &opt, const Ts&... ts) {
    return (opt.has_value() ? 1 : 0)+count_elements(ts...);
}

template<XmlNodeDerived T, class... Ts>
std::size_t count_elements(const QVector<T> &vec, const Ts&... ts) {
    return vec.size()+count_elements(ts...);
}

template<XmlNodeDerived T, std::size_t MAX, class... Ts>
std::size_t count_elements(const internal::XmlVector<T, MAX> &vec, const Ts&... ts) {
    return vec.size()+count_elements(ts...);
}

template<XmlSequenceDerived T, class... Ts>
std::size_t count_elements(const T &t , const Ts&... ts) {
    return static_cast<std::size_t>(t.elements().count())+count_elements(ts...);
}

template<XmlSequenceDerived T, class... Ts>
std::size_t count_elements(const std::optional<T> &opt , const Ts&... ts) {
    return (opt.has_value() ? static_cast<std::size_t>(opt->elements().count()) : 0)+count_elements(ts...);
}

template<XmlNodeOrSequenceDerived... Ss, class... Ts>
std::size_t count_elements(const XmlSwitch<Ss...> &sw, const Ts&... ts) {
    return static_cast<std::size_t>(sw.elements().count())+count_elements(ts...);
}

template<XmlNodeOrSequenceDerived... Ss, class... Ts>
std::size_t count_elements(const std::optional<XmlSwitch<Ss...>> &opt, const Ts&... ts) {
    return (opt.has_value() ? static_cast<std::size_t>(opt->elements().count()) : 0)+count_elements(ts...);
}

template<class... Ts>
void maybe_append(QVector<std::reference_wrapper<const internal::XmlNode>> &, const Ts&...) = delete;

inline void maybe_append(QVector<std::reference_wrapper<const internal::XmlNode>> &) {
}

template<XmlNodeDerived T, class... Ts>
void maybe_append(QVector<std::reference_wrapper<const internal::XmlNode>> &result, const T &t, const Ts&... ts) {
    result.push_back(t);
    maybe_append(result, ts...);
}

template<XmlNodeDerived T, class... Ts>
void maybe_append(QVector<std::reference_wrapper<const internal::XmlNode>> &result, const std::optional<T> &opt, const Ts&... ts) {
    if (opt.has_value())
        result.push_back(*opt);
    maybe_append(result, ts...);
}

template<XmlNodeDerived T, std::size_t MAX, class... Ts>
void maybe_append(QVector<std::reference_wrapper<const internal::XmlNode>> &result, const internal::XmlVector<T, MAX> &vec, const Ts&... ts) {
    for (auto const &t : vec) {
        result.push_back(t);
    }
    maybe_append(result, ts...);
}

template<XmlSequenceDerived T, class... Ts>
void maybe_append(QVector<std::reference_wrapper<const internal::XmlNode>> &result, const T &t, const Ts&... ts) {
    for (auto const &t : t.elements()) {
        result.push_back(t);
    }
    maybe_append(result, ts...);
}

template<XmlSequenceDerived T, class... Ts>
void maybe_append(QVector<std::reference_wrapper<const internal::XmlNode>> &result, const std::optional<T> &opt, const Ts&... ts) {
    if (opt.has_value()) {
        for (auto const &t : opt->elements()) {
            result.push_back(t);
        }
    }
    maybe_append(result, ts...);
}

template<XmlNodeOrSequenceDerived... Ss, class... Ts>
void maybe_append(QVector<std::reference_wrapper<const internal::XmlNode>> &result, const XmlSwitch<Ss...> &sw, const Ts&... ts) {
    for (auto const &e : sw.elements()) {
        result.push_back(e);
    }
    maybe_append(result, ts...);
}

template<XmlNodeOrSequenceDerived... Ss, class... Ts>
void maybe_append(QVector<std::reference_wrapper<const internal::XmlNode>> &result, const std::optional<XmlSwitch<Ss...>> &opt, const Ts&... ts) {
    if (opt.has_value()) {
        for (auto const &e : opt->elements()) {
            result.push_back(e);
        }
    }
    maybe_append(result, ts...);
}

template<class... Ts>
QVector<std::reference_wrapper<const internal::XmlNode>> elements_to_vector(const Ts&... elements) {
    QVector<std::reference_wrapper<const internal::XmlNode>> result;
    result.reserve(static_cast<long long>(count_elements(elements...)));
    maybe_append(result, elements...);
    return result;
}

} // namespace internal
} // namespace ksef::FA3
