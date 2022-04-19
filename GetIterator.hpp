#if !defined TURTLE_TRAFFIC_CTRL_GET_ITERATOR_HPP
#define TURTLE_TRAFFIC_CTRL_GET_ITERATOR_HPP
#include "HttpException.hpp"
#include <unordered_map>

template<typename Key, typename Value>
static std::unordered_map<Key, Value>::iterator GetIterator(std::unordered_map<Key, Value>& m, const Key& KeyData) {
	typename std::unordered_map<Key, Value>::iterator it = m.find(KeyData);
	if (it == m.end()) throw HttpException(404);
	return it;
}
template<typename Key, typename Value>
static std::unordered_map<Key, Value>::const_iterator GetIterator(const std::unordered_map<Key, Value>& m, const Key& KeyData) {
	typename std::unordered_map<Key, Value>::const_iterator it = m.find(KeyData);
	if (it == m.end()) throw HttpException(404);
	return it;
}
#endif
