#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <boost/serialization/split_free.hpp>

// sf::Vector2<T>
namespace boost {
namespace serialization {

template <typename Archive, typename T>
inline void serialize(Archive& ar, sf::Vector2<T>& v,
                      const unsigned int version) {
    ar & v.x;
    ar & v.y;
}

template <class Archive>
void serialize(Archive& ar, sf::Time& time, const unsigned int version) {
    std::int64_t microseconds = time.asMicroseconds();
    ar & microseconds;

    if (Archive::is_loading::value) {
        time = sf::microseconds(microseconds);
    }
}

}  // namespace serialization
}  // namespace boost
