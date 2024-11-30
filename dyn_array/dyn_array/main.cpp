#include <gtest/gtest.h>
#include "tests_int.h"
#include "tests_string.h"

class StringNotMovableNoDefaultConstructor {

public:
	std::string _str;

	StringNotMovableNoDefaultConstructor(const std::string& src) {
		_str = src;
	}

	StringNotMovableNoDefaultConstructor(const StringNotMovableNoDefaultConstructor& src) {
		_str = src._str;
	}

	StringNotMovableNoDefaultConstructor(const StringNotMovableNoDefaultConstructor&& src) = delete;

	const StringNotMovableNoDefaultConstructor& operator=(const StringNotMovableNoDefaultConstructor& src) {
		_str = src._str;
		return *this;
	}

	const StringNotMovableNoDefaultConstructor& operator=(const StringNotMovableNoDefaultConstructor&& src) = delete;
};

void test_memo_destructor() {
	Array<std::string> arr1;
 	arr1.insert("1");
	arr1.insert("2");
//	arr1.~Array();

	Array<int> arr2;
	arr2.insert(1);
	arr2.insert(2);
}


void test_memo_copy_consrt() {
	Array<std::string> arr1;
	arr1.insert("1");
	arr1.insert("2");
	Array<std::string> arr2(arr1);
	arr1.insert("3");

	Array<int> arr1int;
	arr1int.insert(1);
	arr1int.insert(2);
	Array<int> arr2int(arr1int);
	arr1int.insert(3);
}

void test_memo_copy_operator() {
	Array<std::string> arr1;
	arr1.insert("1");
	arr1.insert("2");
	Array<std::string> arr2 = arr1;
	arr1.insert("3");

	Array<int> arr1int;
	arr1int.insert(1);
	arr1int.insert(2);
	Array<int> arr2int = arr1int;
	arr1int.insert(3);
}

void test_memo_copy_consrt_move() {
	Array<std::string> arr1;
	arr1.insert("1");
	arr1.insert("2");
	Array<std::string> arr2(std::move(arr1));
	//arr1.insert("3");

	Array<int> arr1int;
	arr1int.insert(1);
	arr1int.insert(2);
	Array<int> arr2int(std::move(arr1int));
	//arr1int.insert(3);
}

void test_memo_copy_operator_move() {
	Array<std::string> arr1;
	arr1.insert("1");
	arr1.insert("2");
	Array<std::string> arr2 = std::move(arr1);
	//arr1.insert("3");

	Array<int> arr1int;
	arr1int.insert(1);
	arr1int.insert(2);
	Array<int> arr2int = std::move(arr1int);
	//arr1int.insert(3);
}

void test_memo_constructors() {
	//test_memo_destructor();
	test_memo_copy_consrt();
	test_memo_copy_operator();
	test_memo_copy_consrt_move();
	test_memo_copy_operator_move();
}

void test_memo_insert_no_index_no_resize() {
	Array<std::string> arr1(5);
	std::string str1= "1";
	std::string str2 = "2";

	arr1.insert(str1);
	arr1.insert(str2);

	Array<int> arr1int(5);
	int kek1 = 1;
	int kek2 = 2;
	arr1int.insert(kek1);
	arr1int.insert(kek2);
}

void test_memo_insert_no_index_no_resize_move() {
	Array<std::string> arr1(5);
	arr1.insert("1");
	//arr1.insert("2");

	std::string str = "3";
	arr1.insert(std::move(str));

	Array<int> arr1int(5);
	arr1int.insert(1);
	arr1int.insert(2);
}

// будем ли мы когда-нибудь делать мув из чьего-нибудь поля или из элемента массива?...
void test_memo_insert_no_index_no_resize_move_from_tmp() {
	Array<std::string> arr1(5);
	arr1.insert("1");
	arr1.insert("2");

	Array<std::string> arr2(5);
	std::string& ref = arr1[0];
	arr1.insert(std::move(ref));
}


void test_memo_insert_no_index_resize() {
	Array<std::string> arr1(1);
	std::string str1 = "1";
	std::string str2 = "2";

	arr1.insert(str1);
	arr1.insert(str2);

	Array<int> arr1int(2);
	int kek1 = 1;
	int kek2 = 2;
	int kek3 = 3;
	arr1int.insert(kek1);
	arr1int.insert(kek2);
	arr1int.insert(kek3);
}

void test_memo_insert_no_index_resize_move() {
	Array<std::string> arr1(1);
	arr1.insert("1");
	//arr1.insert("2");

	std::string str = "3";
	arr1.insert(std::move(str));

	Array<int> arr1int(1);
	arr1int.insert(1);
	arr1int.insert(2);
}


void test_memo_insert_no_index_resize_not_movable() {
	Array<StringNotMovableNoDefaultConstructor> arr1(1);
	StringNotMovableNoDefaultConstructor str1("1");
	
	 arr1.insert(str1); // без ресайза и без мува
	 arr1.insert(str1); // c резайсом, без мува нового элемента
	 assert(arr1[0]._str == "1");
	 assert(arr1[1]._str == "1");

	StringNotMovableNoDefaultConstructor str2("2"); //  с ресайзом и с мувом нового элемента (несостоявшимся, как и должно)
	arr1.insert(std::move(str2));
	assert(arr1[2]._str == "2");
}

// вставка в начало
void test_memo_insert_index_no_resize() {
	Array<std::string> arr1(5);
	std::string str1 = "1";
	std::string str2 = "2";
	std::string str3 = "3";

	arr1.insert(0, str1);
	arr1.insert(0, str2);
	arr1.insert(0, str3);
	assert(arr1.size() == 3);
	assert(arr1[0] == "3");
	assert(arr1[1] == "2");
	assert(arr1[2] == "1");
	
	Array<int> arr1int(5);
	int kek1 = 1;
	int kek2 = 2;
	int kek3 = 3;
	arr1int.insert(kek1);
	arr1int.insert(0, kek2);
	arr1int.insert(0, kek3);
	assert(arr1int.size() == 3);
	assert(arr1int[0] == 3);
	assert(arr1int[1] == 2);
	assert(arr1int[2] == 1);
}

// вставка в середину
void test_memo_insert_index_no_resize_move() {
	Array<std::string> arr1(5);

	std::string str3 = "3";
	arr1.insert(0, std::string("1"));
	arr1.insert(0, "2");
	arr1.insert(1, std::move(str3));
	assert(arr1.size() == 3);
	assert(arr1[0] == "2");
	assert(arr1[1] == "3");
	assert(arr1[2] == "1");
	assert(str3 == "");

	Array<int> arr1int(5);
	int kek1 = 1;
	arr1int.insert(0, std::move(kek1));
	arr1int.insert(0, 2);
	arr1int.insert(1, int(3));
	assert(arr1int.size() == 3);
	assert(arr1int[0] == 2);
	assert(arr1int[1] == 3);
	assert(arr1int[2] == 1);
//	assert(kek1 == 0);
}

// вставка в начало
void test_memo_insert_index_resize() {
	Array<std::string> arr1(1);
	std::string str1 = "1";
	std::string str2 = "2";
	std::string str3 = "3";

	arr1.insert(0, str1);
	arr1.insert(0, str2);
	arr1.insert(0, str3);
	assert(arr1.size() == 3);
	assert(arr1[0] == "3");
	assert(arr1[1] == "2");
	assert(arr1[2] == "1");

	Array<int> arr1int(1);
	int kek1 = 1;
	int kek2 = 2;
	int kek3 = 3;
	arr1int.insert(0, kek1);
	arr1int.insert(0, kek2);
	arr1int.insert(0, kek3);
	assert(arr1int.size() == 3);
	assert(arr1int[0] == 3);
	assert(arr1int[1] == 2);
	assert(arr1int[2] == 1);
}

// вставка в конец
void test_memo_insert_index_resize_move() {
	Array<std::string> arr1(1);

	std::string str3 = "3";
	arr1.insert(0, std::string("1"));
	arr1.insert(0, "2");
	arr1.insert(1, std::move(str3));
	assert(arr1.size() == 3);
	assert(arr1[0] == "2");
	assert(arr1[1] == "3");
	assert(arr1[2] == "1");
	assert(str3 == "");

	Array<int> arr1int(1);
	int kek1 = 1;
	arr1int.insert(0, std::move(kek1));
	arr1int.insert(0, 2);
	arr1int.insert(1, int(3));
	assert(arr1int.size() == 3);
	assert(arr1int[0] == 2);
	assert(arr1int[1] == 3);
	assert(arr1int[2] == 1);
}

void test_memo_insert_index_resize_move_not_movable() {
	Array<StringNotMovableNoDefaultConstructor> arr1(1);
	StringNotMovableNoDefaultConstructor str1("1");

	arr1.insert(0, str1); // без ресайза и без мува
	arr1.insert(0, str1); // c резайсом, без мува нового элемента
	assert(arr1[0]._str == "1");
	assert(arr1[1]._str == "1");

	StringNotMovableNoDefaultConstructor str2("2"); //  с ресайзом и с мувом нового элемента (несостоявшимся, как и должно)
	arr1.insert(1, std::move(str2));
	assert(arr1.size() == 3);
	assert(arr1[0]._str == "1");
	assert(arr1[1]._str == "2");
	assert(arr1[2]._str == "1");
}

void test_memo_insert() {
	test_memo_insert_no_index_no_resize();
	test_memo_insert_no_index_no_resize_move();
	test_memo_insert_no_index_no_resize_move_from_tmp();
	
	test_memo_insert_no_index_resize();
	test_memo_insert_no_index_resize_move();
	test_memo_insert_no_index_resize_not_movable();

	// -- start утечка на string если не вызывать деструктор после кражи в insert(idx, value)!
	// даже если с i-го места украли данные, нужно это место почистить перед реконструктором.
	test_memo_insert_index_no_resize();
	test_memo_insert_index_no_resize_move();
	// -- end утечка на string

	test_memo_insert_index_resize();
	test_memo_insert_index_resize_move();
	test_memo_insert_index_resize_move_not_movable();
}

void test_memo_remove() {
	/// делаем массив без мувов, с ресайзом 2 раза и вставками в начало
	
	Array<std::string> arr1(1);
	std::string str1 = "1";
	std::string str2 = "2";
	std::string str3 = "3";

	arr1.insert(0, str1);
	arr1.insert(0, str2);
	arr1.insert(0, str3);
	assert(arr1.size() == 3);
	assert(arr1[0] == "3");
	assert(arr1[1] == "2");
	assert(arr1[2] == "1");

	Array<int> arr1int(1);
	int kek1 = 1;
	int kek2 = 2;
	int kek3 = 3;
	arr1int.insert(0, kek1);
	arr1int.insert(0, kek2);
	arr1int.insert(0, kek3);
	assert(arr1int.size() == 3);
	assert(arr1int[0] == 3);
	assert(arr1int[1] == 2);
	assert(arr1int[2] == 1);

	////
	arr1.remove(0);
	assert(arr1.size() == 2);
	assert(arr1[0] == "2");
	assert(arr1[1] == "1");
	arr1.remove(1);
	assert(arr1.size() == 1);
	assert(arr1[0] == "2");

	arr1int.remove(0);
	assert(arr1int.size() == 2);
	assert(arr1int[0] == 2);
	assert(arr1int[1] == 1);
	arr1int.remove(1);
	assert(arr1int.size() == 1);
	assert(arr1int[0] == 2);

}

void test_memo_remove_not_movable() {
	// массив с ресайзом
	Array<StringNotMovableNoDefaultConstructor> arr1(1);
	StringNotMovableNoDefaultConstructor str1("1");

	arr1.insert(0, str1); // без ресайза и без мува
	arr1.insert(0, str1); // c резайсом, без мува нового элемента
	assert(arr1[0]._str == "1");
	assert(arr1[1]._str == "1");

	StringNotMovableNoDefaultConstructor str2("2"); //  с ресайзом и с мувом нового элемента (несостоявшимся, как и должно)
	arr1.insert(1, std::move(str2));
	assert(arr1.size() == 3);
	assert(arr1[0]._str == "1");
	assert(arr1[1]._str == "2");
	assert(arr1[2]._str == "1");

	//

	arr1.remove(1); // удаляем из середины
	assert(arr1.size() == 2);
	assert(arr1[0]._str == "1");
	assert(arr1[1]._str == "1");

	arr1.remove(0); // удаляем из начала
	assert(arr1.size() == 1);
	assert(arr1[0]._str == "1");
}

void test_memo() {
//	test_memo_constructors();
//	test_memo_insert();
	test_memo_remove();
	test_memo_remove_not_movable();
}

int main(int argc, char* argv[])
{
//	testing::InitGoogleTest(&argc, argv);
//	return RUN_ALL_TESTS();

	test_memo();
	return 0;
}