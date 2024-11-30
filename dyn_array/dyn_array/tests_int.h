#pragma once
#pragma once

#include <cstdint>
#include <gtest/gtest.h>

#include "array.h"

/*
class IntArrayMovable {

private:

	int* _ptr;
	int _size;

	public:
		IntArrayMovable(int size) {
			_size = size;
			_ptr = new int[_size];
			for (int i = 0; i < _size; ++i)
				_ptr[i] = i;
		}

		IntArrayMovable(const IntArrayMovable& source) {

		}

		IntArrayMovable(IntArrayMovable&& source) {

		}

		const IntArrayMovable& operator=(const IntArrayMovable& source) {

		}

		const IntArrayMovable& operator=(IntArrayMovable&& source) {

		}
}; */
/*
TEST(ArrayConstruct, DefaultSizeEqualsZero) {
	Array<int> arrInt;
	ASSERT_EQ(arrInt.size(), 0);
}

TEST(ArrayConstruct, CapacitySet) {
	Array<int> arrInt;
	ASSERT_EQ(arrInt.capacity(), default_capacity);
}

TEST(ArrayConstruct, CopyingAnotherArray) {
	Array<int> arrInt1(5);
	arrInt1.insert(1);
	arrInt1.insert(2);
	Array<int> arrInt2 = arrInt1;
	ASSERT_EQ(arrInt2.capacity(), 5);
	ASSERT_EQ(arrInt2.size(), 2);
	ASSERT_EQ(arrInt2[0], 1);
	ASSERT_EQ(arrInt2[1], 2);
	arrInt1.insert(3);
	ASSERT_EQ(arrInt2.size(), 2);
}

TEST(ArrayConstruct, MovingAnotherArray) {
	Array<int> arrInt1(5);
	arrInt1.insert(1);
	arrInt1.insert(2);
	Array<int> arrInt2 = std::move(arrInt1);
	ASSERT_EQ(arrInt2.capacity(), 5);
	ASSERT_EQ(arrInt2.size(), 2);
	ASSERT_EQ(arrInt2[0], 1);
	ASSERT_EQ(arrInt2[1], 2);
	ASSERT_EQ(arrInt1.capacity(), 0);
	ASSERT_EQ(arrInt1.size(), 0);
}

TEST(ArrayAssign, MovingAnotherArray) {
	Array<int> arrInt1(5);
	arrInt1.insert(1);
	arrInt1.insert(2);
	Array<int> arrInt2;
	arrInt2 = std::move(arrInt1);
	ASSERT_EQ(arrInt2.capacity(), 5);
	ASSERT_EQ(arrInt2.size(), 2);
	ASSERT_EQ(arrInt2[0], 1);
	ASSERT_EQ(arrInt2[1], 2);
	ASSERT_EQ(arrInt1.capacity(), 0);
	ASSERT_EQ(arrInt1.size(), 0);
}

TEST(ArrayInsert, InsertToEnd) {
	Array<int> arrInt1;
	arrInt1.insert(1);
	ASSERT_EQ(arrInt1.size(), 1);
	ASSERT_EQ(arrInt1[0], 1);
	arrInt1.insert(2);
	ASSERT_EQ(arrInt1.size(), 2);
	ASSERT_EQ(arrInt1[1], 2);
}

TEST(ArrayInsert, InsertToStart) {
	Array<int> arrInt1;
	arrInt1.insert(0, 1);
	ASSERT_EQ(arrInt1.size(), 1);
	ASSERT_EQ(arrInt1[0], 1);
	arrInt1.insert(0, 2);
	ASSERT_EQ(arrInt1.size(), 2);
	ASSERT_EQ(arrInt1[0], 2);
}

TEST(ArrayInsert, InsertToIndex) {
	Array<int> arrInt1;
	arrInt1.insert(1);
	arrInt1.insert(2);
	arrInt1.insert(1, 3);
	ASSERT_EQ(arrInt1.size(), 3);
	ASSERT_EQ(arrInt1[1], 3);
}

TEST(ArrayInsert, ExtendCapacity) {
	Array<int> arrInt1(1);
	arrInt1.insert(1);
	arrInt1.insert(2);
	ASSERT_EQ(arrInt1.capacity(), 2);
	arrInt1.insert(3);
	ASSERT_EQ(arrInt1.capacity(), 4);
	arrInt1.insert(4);
	ASSERT_EQ(arrInt1.capacity(), 4);

}

TEST(ArrayRemove, RemoveFromStart) {
	Array<int> arrInt1;
	arrInt1.insert(1);
	arrInt1.insert(2);
	arrInt1.remove(0);
	ASSERT_EQ(arrInt1.size(), 1);
	ASSERT_EQ(arrInt1[0], 2);
	arrInt1.remove(0);
	ASSERT_EQ(arrInt1.size(), 0);

}

TEST(ArrayRemove, RemoveFromEnd) {
	Array<int> arrInt1;
	arrInt1.insert(1);
	arrInt1.insert(2);
	arrInt1.remove(1);
	ASSERT_EQ(arrInt1.size(), 1);
	ASSERT_EQ(arrInt1[0], 1);
	arrInt1.remove(0);
	ASSERT_EQ(arrInt1.size(), 0);

}

TEST(ArrayRemove, RemoveFromIndex) {
	Array<int> arrInt1;
	arrInt1.insert(1);
	arrInt1.insert(2);
	arrInt1.insert(3);
	arrInt1.remove(1);
	ASSERT_EQ(arrInt1.size(), 2);
	ASSERT_EQ(arrInt1[0], 1);
	ASSERT_EQ(arrInt1[1], 3);
}

TEST(ArrayIndex, IndexRead) {
	Array<int> arrInt1;
	arrInt1.insert(1);
	arrInt1.insert(2);
	ASSERT_EQ(arrInt1[0], 1);
	ASSERT_EQ(arrInt1[1], 2);

}

TEST(ArrayIndex, IndexWrite) {
	Array<int> arrInt1;
	arrInt1.insert(1);
	arrInt1.insert(2);
	arrInt1[0] = 3;
	arrInt1[1] = 4;
	ASSERT_EQ(arrInt1[0], 3);
	ASSERT_EQ(arrInt1[1], 4);

}

TEST(ArrayIterator, ForwardIterate) {
	Array<int> arrInt1;
	arrInt1.insert(1);
	arrInt1.insert(2);
	arrInt1.insert(3);
	int32_t i = 1;
	for (auto it = arrInt1.iterator(); it.hasNext(); it.next()) {
		ASSERT_EQ(it.get(), i);
		i++;
	}
}

TEST(ArrayIterator, ReverseIterate) {
	Array<int> arrInt1;
	arrInt1.insert(1);
	arrInt1.insert(2);
	arrInt1.insert(3);
	int32_t i = 3;
	for (auto it = arrInt1.reverseIterator(); it.hasNext(); it.next()) {
		ASSERT_EQ(it.get(), i);
		i--;
	}
}

TEST(ArrayIterator, Write) {
	Array<int> arrInt1;
	arrInt1.insert(1);
	arrInt1.insert(2);
	arrInt1.insert(3);
	int32_t i = 3;
	for (auto it = arrInt1.iterator(); it.hasNext(); it.next()) {
		it.set(i);
		ASSERT_EQ(it.get(), i);
		i--;
	}
}*/