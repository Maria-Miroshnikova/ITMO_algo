#include "array.h"
#include <utility> 
#include <type_traits>
#include <cassert>

template<typename T>
bool Array<T>::has_enough_capacity() {
	return (_size < _capacity);
}

template<typename T>
void Array<T>::resize() {
	// create new resource
	_capacity *= extend_coeff;
	T* new_array = static_cast<T*>(malloc(_capacity * sizeof(T)));

	// move data from old to new
	if (!std::is_move_constructible_v<T>){
		for (int i = 0; i < _size; ++i) {
			new (new_array + i) T(_array[i]);
		}
	}
	else {
		for (int i = 0; i < _size; ++i) {
			//new (new_array + i) T(std::move(_array[i]));
			new (new_array + i) T(std::move_if_noexcept(_array[i]));
		}
	}

	// delete old resource
	delete_array();

	_array = new_array;
}

template<typename T>
const Array<T>& Array<T>::operator=(const Array<T>& source) {
	Array<T> tmp(source);

	T* tmp_array = source._array;
	int tmp_size = source._size;
	int tmp_capacity = source._capacity;

	source._array = _array;
	source._size = _size;
	source._capacity = _capacity;

	_array = tmp_array;
	_size = tmp_size;
	_capacity = tmp_capacity;

	return *this;
}

template<typename T>
const Array<T>& Array<T>::operator=(Array<T>&& source) noexcept {
	Array<T> tmp(*this);

	_array = source._array;
	_size = source._size;
	_capacity = source._capacity;

	source._array = nullptr;
	source._size = 0;
	source._capacity = 0;

	return *this;
}

template<typename T>
int Array<T>::insert(const T& value) {
	if (!has_enough_capacity())
		resize();

	new (_array + _size) T(value);

	return _size++;
};

template<typename T>
int Array<T>::insert(T&& value) {
	if (!has_enough_capacity())
		resize();

	if (!std::is_move_constructible_v<T>)
		new (_array + _size) T(value);
	else
		new (_array + _size) T(std::move_if_noexcept(value));

	return _size++;
};

template<typename T>
int Array<T>::insert(int index, const T& value) {
	if (index_out_of_size(index))
		return -1;
	
	// make all movings and size++ inside this
	if (!has_enough_capacity()) {
		resize_with_index(index, value);
		return index;
	}

	// else - move data here
	if (!std::is_move_constructible_v<T>) {
		for (int i = _size; i > index; --i) {
			new (_array + i) T(_array[i - 1]);
			_array[i - 1].~T();
		}
	}
	else {
		for (int i = _size; i > index; --i) {
			new (_array + i) T(std::move_if_noexcept(_array[i - 1]));
			_array[i - 1].~T();
			//T kek = _array[i - 1];
		}
	}
	new (_array + index) T(value);
	_size++;

	return index;
};

template<typename T>
int Array<T>::insert(int index, T&& value) {
	if (index_out_of_size(index))
		return -1;
	
	// make all movings and size++ inside this, if resize is needed
	if (!has_enough_capacity()) {
		resize_with_index(index, std::move(value));
		return index;
	}

	// else - move data here
	for (int i = _size; i > index; --i) {
		new (_array + i) T(std::move_if_noexcept(_array[i - 1]));
		_array[i - 1].~T();
	}
	new (_array + index) T(std::move_if_noexcept(value));
	_size++;

	return index;
};

template<typename T>
void Array<T>::resize_with_index(int index, const T& value) {
	// create new resource
	_capacity *= extend_coeff;
	T* new_array = static_cast<T*>(malloc(_capacity * sizeof(T)));

	if (!std::is_move_constructible_v<T>) {
		// move data from old (0 to index)
		for (int i = 0; i < index; ++i) {
			new (new_array + i) T(_array[i]);
		}

		// add new value
		new (new_array + index) T(value);

		// move data from old (index to _size)
		for (int i = index; i < _size; ++i) {
			new (new_array + i + 1) T(_array[i]);
		}
	}
	else {
		// move data from old (0 to index)
		for (int i = 0; i < index; ++i) {
			new (new_array + i) T(std::move_if_noexcept(_array[i]));
		}

		// add new value
		new (new_array + index) T(value);

		// move data from old (index to _size)
		for (int i = index; i < _size; ++i) {
			new (new_array + i + 1) T(std::move_if_noexcept(_array[i]));
		}
	}

	// delete old resource
	delete_array();

	_size++;
	_array = new_array;
}

template<typename T>
void Array<T>::resize_with_index(int index, T&& value) {
	// create new resource
	_capacity *= extend_coeff;
	T* new_array = static_cast<T*>(malloc(_capacity * sizeof(T)));

	// move data from old (0 to index)
	for (int i = 0; i < index; ++i) {
		new (new_array + i) T(std::move_if_noexcept(_array[i]));
	}

	// move new value
	new (new_array + index) T(std::move_if_noexcept(value));

	// move data from old (index to _size)
	for (int i = index; i < _size; ++i) {
		new (new_array + i + 1) T(std::move_if_noexcept(_array[i]));
	}

	// delete old resource
	delete_array();

	_size++;
	_array = new_array;
}


template<typename T>
const T& Array<T>::operator[](int index) const {
	return _array[index];
};

template<typename T>
T& Array<T>::operator[](int index) {
	return _array[index];
};

template<typename T>
void Array<T>::remove(int index) {
	assert((index >= 0) && (index < _size));

	_array[index].~T();
	_size--;

	if (!std::is_move_assignable_v<T>) {
		for (int i = index; i < _size; ++i) {
			_array[i] = _array[i + 1];
			// это же копирующий конструктор, который выдел€ет пам€ть и делает глубокую копию?
			// поэтому после присваивани€ у нас две копии и надо освободить ресурсы из оригинала?
			_array[i + 1].~T();
		}
	}
	else {
		for (int i = index; i < _size; ++i) {
			// а могли бы и конструктором?
			// »Ќ“≈–≈—Ќќ:
			// 1. можно к очищенной €чейке все равно обратитьс€ по индексу, получить на нее ссылку
			// 2. можно в нее присвоить значени€ в пол€ (в операторе =), не вызыва€ конструктор заново
			_array[i] = std::move_if_noexcept(_array[i + 1]);
			// даже после кражи нужно очистить место кражи
			_array[i + 1].~T();
		}
	}
};

template<typename T>
int Array<T>::size() const {
	return _size;
};

template<typename T>
int Array<T>::capacity() const {
	return _capacity;
};

template<typename T>
bool Array<T>::index_out_of_size(int index) {
	return (index < 0) || (index > _size);
}

template<typename T>
void Array<T>::delete_array() {
	//if (!std::is_destructible_v<T>)
	for (int i = 0; i < _size; ++i) {
		_array[i].~T();
	}

	free(_array);
}