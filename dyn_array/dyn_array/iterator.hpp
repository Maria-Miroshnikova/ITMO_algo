#include "array.h"

template<typename T>
Array<T>::template Iterator Array<T>::iterator() {
	return Array<T>::Iterator(_array, 1, *this);
}

template<typename T>
Array<T>::template ConstIterator Array<T>::iterator() const {
	return Array<T>::ConstIterator(_array, 1, *this);
}

template<typename T>
Array<T>::template Iterator Array<T>::reverseIterator() {
	return Array<T>::Iterator(_array + _size - 1, -1, *this);
}

template<typename T>
Array<T>::template ConstIterator Array<T>::reverseIterator() const {
	return Array<T>::ConstIterator(_array + _size - 1, -1, *this);
}

template<typename T>
const T& Array<T>::Iterator::get() const {
	return *current;
}

template<typename T>
void Array<T>::Iterator::set(const T& value) {
	*current = value;
}

template<typename T>
void Array<T>::Iterator::next() {
	current += _direction;
}

template<typename T>
bool Array<T>::Iterator::hasNext() const {
	if (_direction > 0)
		return (current == (_array._array + _array.size() - 1));
	return current == _array._array;
}

template<typename T>
const T& Array<T>::ConstIterator::get() const {
	return *current;
}

template<typename T>
void Array<T>::ConstIterator::set(const T& value) {
	*current = value;
}

template<typename T>
void Array<T>::ConstIterator::next() {
	current += _direction;
}

template<typename T>
bool Array<T>::ConstIterator::hasNext() const {
	if (_direction > 0)
		return (current == (_array._array + _array.size() - 1));
	return current == _array._array;
}
