#pragma once

#include <cstdlib>

static int default_capacity = 8;
static int extend_coeff = 2;


template<typename T>
class Array final {

public:

	class Iterator final {
		T* current;
		int _direction;
		Array<T>& _array;

	public:
		Iterator(T* source, int direction, Array<T>& array_src) : current(source), _direction(direction), _array(array_src) {}

		const T& get() const;
		void set(const T& value);
		void next();
		bool hasNext() const;
	};

	class ConstIterator final {
		const T* current;
		int _direction;
		Array<T>& _array;

	public:

		ConstIterator(T* source, int direction, Array<T>& array_src) : current(source), _direction(direction), _array(array_src) {}

		const T& get() const;
		void set(const T& value);
		void next();
		bool hasNext() const;
	};

	Array() {
		_capacity = default_capacity;
		_array = static_cast<T*>(malloc(_capacity * sizeof(T)));
	};

	Array(int capacity) {
		_capacity = capacity;
		_array = static_cast<T*>(malloc(_capacity * sizeof(T)));
	};

	Array(const Array<T>& source) {
		_capacity = source._capacity;
		_array = static_cast<T*>(malloc(_capacity * sizeof(T)));

		for (int i = 0; i < source._size; ++i) {
			new (_array + i) T(source[i]);
		}
		_size = source._size;
	}

	const Array<T>& operator=(const Array<T>& source);

	// механизм перемещени
	Array(Array<T>&& source) noexcept {
		_array = source._array;
		_size = source._size;
		_capacity = source._capacity;

		source._array = nullptr;
		source._size = 0;
		source._capacity = 0;
	}

	const Array<T>& operator=(Array<T>&& source) noexcept;

	~Array() {
		delete_array();
	};

	int insert(const T& value);
	int insert(T&& value);
	int insert(int index, const T& value);
	int insert(int index, T&& value);

	const T& operator[](int index) const;
	T& operator[](int index);

	void remove(int index);

	int size() const;
	int capacity() const;

	Array<T>::template Iterator iterator();
	Array<T>::template ConstIterator iterator() const;

	Array<T>::template Iterator reverseIterator();
	Array<T>::template ConstIterator reverseIterator() const;

private:
	T* _array;
	int _size = 0;
	int _capacity = 0;

	bool has_enough_capacity();

	void resize();
	void resize_with_index(int index, const T& value);
	void resize_with_index(int index, T&& value);
	bool index_out_of_size(int index);
	void delete_array();
};

#include "array.hpp"
#include "iterator.hpp"