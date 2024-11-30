#include <vector>
#include <iostream>

#include <locale>
#include <random>

std::random_device rd;
std::mt19937 mersenne(rd());

class comma : public std::numpunct<char> {
public:
	comma() : numpunct<char>() {}
protected:
	char do_decimal_point() const {
		return ',';
	}
};

class Dices {
public:
	int _count;
	int _planes;
	int _modifier;

public:
	Dices(int count, int planes, int modifier = 0) {
		_count = count;
		_planes = planes;
		_modifier = modifier;
	}

	int roll() const {
		int result = 0;
		for (int i = 0; i < _count; ++i) {
			//result += rand() % _planes + 1;
			result += mersenne() % _planes + 1;
		}

		return result + _modifier;
	}
	
};

template <typename... Args>
int dice(const Dices& head, const Args& ... tail) {
	return dice(head) + dice(tail);
}

int dice(const Dices& dices) {
	return dices.roll();
}

void test() {

	std::vector<Dices> test_cases({ Dices(1, 6), Dices(2, 6), Dices(3, 6), Dices(1, 10), Dices(2, 10), Dices(3, 10) });
	int count_exprmnt = 1000000;
	//int count_exprmnt = 10000000;
	int result;

	std::srand(time(0));
	rand();

	for (int i = 0; i < test_cases.size(); ++i) {
		const Dices& current = test_cases[i];

		std::cout << current._count << "d" << current._planes << std::endl;
		
		int* result_counts = new int[current._planes * current._count];
		for (int k = 0; k < current._count * current._planes; ++k) {
			result_counts[k] = 0;
		}

		for (int j = 0; j < count_exprmnt; ++j) {
			result = dice(current);
			result_counts[result - 1] += 1;
		}
		
		for (int k = current._count - 1; k < current._count * current._planes; ++k) {
			//std::cout << k + 1 << ": " << result_counts[k] * 1.0 / count_exprmnt << std::endl;
			std::cout << result_counts[k] * 1.0 / count_exprmnt << std::endl;
		}
		std::cout << "--------------------" << std::endl;

		delete[] result_counts;
		//break;
	}
}

int main() {
	
	std::locale loccomma(std::cout.getloc(), new comma);
	std::cout.imbue(loccomma);

	test();
}