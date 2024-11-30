#pragma once

#include <cstdint>
#include <gtest/gtest.h>

#include "array.h"

#include <string>
/*
TEST(ArrayConstructStringDEBUGG, CopyConstructAnonymObj) {
	// check in debug
	//Array<std::string> arr2(std::move(Array<std::string> (4)));
	Array<std::string> arr2(Array<std::string>(4));
	ASSERT_EQ(arr2.capacity(), 4);
	ASSERT_EQ(arr2.size(), 0);
	arr2.insert("3");
	ASSERT_EQ(arr2.size(), 1);
}

TEST(ArrayConstructStringHH, CopyConstruct) {
	Array<std::string> arr1(5);
	arr1.insert("1");
	arr1.insert("2");
	Array<std::string> arr2(arr1);
	ASSERT_EQ(arr2.capacity(), 5);
	ASSERT_EQ(arr2.size(), 2);
	ASSERT_EQ(arr2[0], "1");
	ASSERT_EQ(arr2[1], "2");
	arr1.insert("3");
	ASSERT_EQ(arr2.size(), 2);

	arr1[0] = "000";
	ASSERT_EQ(arr2[0], "1");
	ASSERT_EQ(arr1[0], "000");
}

TEST(ArrayConstructStringHH, CopyOperator) {
	Array<std::string> arr1(5);
	arr1.insert("1");
	arr1.insert("2");
	Array<std::string> arr2 = arr1;
	ASSERT_EQ(arr2.capacity(), 5);
	ASSERT_EQ(arr2.size(), 2);
	ASSERT_EQ(arr2[0], "1");
	ASSERT_EQ(arr2[1], "2");
	arr1.insert("3");
	ASSERT_EQ(arr2.size(), 2);

	arr1[0] = "000";
	ASSERT_EQ(arr2[0], "1");
	ASSERT_EQ(arr1[0], "000");
}
*/

/*TEST(ArrayConstructStringDEBUG, CopyConstructMove) {
	Array<std::string> arr1(5);
	arr1.insert("1");
	arr1.insert("2");
	Array<std::string> arr2(std::move(arr1));
	ASSERT_EQ(arr2.capacity(), 5);
	ASSERT_EQ(arr2.size(), 2);
	ASSERT_EQ(arr2[0], "1");
	ASSERT_EQ(arr2[1], "2");
	ASSERT_EQ(arr1.size(), 0);
	ASSERT_EQ(arr1.capacity(), 0);
}*/


/*TEST(ArrayConstructStringDEBUG, CopyOperatorMove) {
	Array<std::string> arr1(5);
	arr1.insert("1");
	arr1.insert("2");
	Array<std::string> arr2 = std::move(arr1);
	ASSERT_EQ(arr2.capacity(), 5);
	ASSERT_EQ(arr2.size(), 2);
	ASSERT_EQ(arr2[0], "1");
	ASSERT_EQ(arr2[1], "2");
	ASSERT_EQ(arr1.size(), 0);
	ASSERT_EQ(arr1.capacity(), 0);
}*/

/*
TEST(ArrayInsertStringDEBUGG, InsertToEndMovFe) {
	Array<std::string> arr1;
	arr1.insert("1");
	ASSERT_EQ(arr1.size(), 1);
	ASSERT_EQ(arr1[0], "1");
	arr1.insert("2");
	ASSERT_EQ(arr1.size(), 2);
	ASSERT_EQ(arr1[1], "2");
}*/

/*
TEST(ArrayInsertString, InsertToStart) {
	Array<std::string> arr1;
	arr1.insert(0, "1");
	ASSERT_EQ(arr1.size(), 1);
	ASSERT_EQ(arr1[0], "1");
	arr1.insert(0, "2");
	ASSERT_EQ(arr1.size(), "2");
	ASSERT_EQ(arr1[0], 2);
}

TEST(ArrayInsertString, InsertToIndex) {
	Array<std::string> arr1;
	arr1.insert("1");
	arr1.insert("2");
	arr1.insert(1, "3");
	ASSERT_EQ(arr1.size(), 3);
	ASSERT_EQ(arr1[1], "3");
}

TEST(ArrayInsertString, ExtendCapacity) {
	Array<std::string> arr1(1);
	arr1.insert("1");
	arr1.insert("2");
	ASSERT_EQ(arr1.capacity(), 2);
	arr1.insert("3");
	ASSERT_EQ(arr1.capacity(), 4);
	arr1.insert("4");
	ASSERT_EQ(arr1.capacity(), 4);

}

TEST(ArrayRemoveString, RemoveFromStart) {
	Array<std::string> arr1;
	arr1.insert("1");
	arr1.insert("2");
	arr1.remove(0);
	ASSERT_EQ(arr1.size(), 1);
	ASSERT_EQ(arr1[0], "2");
	arr1.remove(0);
	ASSERT_EQ(arr1.size(), 0);

}

TEST(ArrayRemoveString, RemoveFromEnd) {
	Array<std::string> arr1;
	arr1.insert("1");
	arr1.insert("2");
	arr1.remove(1);
	ASSERT_EQ(arr1.size(), 1);
	ASSERT_EQ(arr1[0], "1");
	arr1.remove(0);
	ASSERT_EQ(arr1.size(), 0);

}

TEST(ArrayRemoveString, RemoveFromIndex) {
	Array<std::string> arr1;
	arr1.insert("1");
	arr1.insert("2");
	arr1.insert("3");
	arr1.remove(1);
	ASSERT_EQ(arr1.size(), 2);
	ASSERT_EQ(arr1[0], "1");
	ASSERT_EQ(arr1[1], "3");
}*/