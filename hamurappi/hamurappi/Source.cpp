#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <ctime>

using namespace std;


const int WHEAT_DEF = 2800;
const int AKRS_DEF = 1000;
const int PEOPLE_DEF = 100;

struct Resources {
	int wheat = WHEAT_DEF;
	int akrs = AKRS_DEF;
	int people = PEOPLE_DEF;
};

bool operator == (const Resources& res, const Resources& new_res)
{
	return((res.akrs == new_res.akrs) && (res.people == new_res.people) && (res.wheat == new_res.wheat));
}

struct Decision {
	int akrs_to_bargain;
	int wheat_to_eat;
	int wheat_to_grow;
};

struct GameState {
	int current_round_number = 0;
	
	int dead_people_prev = 0;
	int people_come_prev = 0;
	bool was_chuma = false;
	int fertility_prev;
	int rats_damage_prev = 0;

	int wheat_income = 0;
	int akr_cost;
	float rats_damage_coeff = 0;


	Resources res;
	float mortality_persentage = 0;

	void init_round() {
		current_round_number += 1;
		fertility_prev = rand() % (6 - 1 + 1) + 1;
		akr_cost = rand() % (26 - 17 + 1) + 17;
		was_chuma = (((rand() * 1.0 / RAND_MAX)) < 0.15);
		rats_damage_coeff = (rand() % (7 + 1)) * 1.0 / 100;
	}

	bool check_decision(const Decision& decision, Resources& new_res) {
		// купить
		if (decision.akrs_to_bargain > 0) {
			int akrs_summ = akr_cost * decision.akrs_to_bargain;
			if (akrs_summ > res.wheat)
				return false;
			new_res.akrs += decision.akrs_to_bargain;
			new_res.wheat -= akrs_summ;
		}
		// продать
		else {
			if (-decision.akrs_to_bargain > res.akrs)
				return false;
			int akrs_summ = akr_cost * decision.akrs_to_bargain;
			new_res.akrs += decision.akrs_to_bargain;
			new_res.wheat -= akrs_summ;
		}

		if ((decision.wheat_to_eat > new_res.wheat) || (decision.wheat_to_eat < 0)) {
			return false;
		}

		int akrs_need_to_grow = decision.wheat_to_grow / 2;
		int max_akrs_to_grow_by_people = res.people * 10;
		bool kek = (akrs_need_to_grow > max_akrs_to_grow_by_people);
		if ((akrs_need_to_grow > new_res.akrs)
			|| (decision.wheat_to_grow < 0)
			|| (decision.wheat_to_grow > (new_res.wheat - decision.wheat_to_eat))
			|| (akrs_need_to_grow > max_akrs_to_grow_by_people)) {
			return false;
		}

		return true;
	}

	float post_decision_events(const Decision& decision) {
		wheat_in(decision);

		rats_make_damage();

		int res_people_before_feed = res.people;
		people_eat(decision);

		people_come();

		chuma();

		return count_mortality(res_people_before_feed);
	}

	void wheat_in(const Decision& decision) {
		wheat_income = decision.wheat_to_grow * fertility_prev * 2;
		res.wheat += wheat_income;
	}

	void rats_make_damage() {
		rats_damage_prev = rats_damage_coeff * res.wheat;
		res.wheat -= rats_damage_prev;
	}

	void people_eat(const Decision& decision) {
		int people_feed;
		if (res.wheat < decision.wheat_to_eat) {
			people_feed = res.wheat / 20;
			res.wheat = 0;
		}
		else {
			int max_feed_require = res.people * 20;
			// съедят не все, если есть излишки
			if (max_feed_require < decision.wheat_to_eat) {
				res.wheat -= max_feed_require;
			}
			// съедят все, но умрут
			else {
				res.wheat -= decision.wheat_to_eat;
			}
			
			people_feed = decision.wheat_to_eat / 20;
		}

		dead_people_prev = res.people - people_feed;
		res.people = people_feed;
	}

	void chuma() {
		if (was_chuma)
			res.people /= 2;
	}

	void people_come() {
		people_come_prev = dead_people_prev / 2 + (5 - fertility_prev) * res.wheat / 600 + 1;
		if (people_come_prev < 0)
			people_come_prev = 0;
		else if (people_come_prev > 50)
			people_come_prev = 50;
	}

	float count_mortality(int people_before_feed) {
		float mortality_current = dead_people_prev * 1.0 / people_before_feed;
		mortality_persentage += mortality_current;
		return mortality_current;
	}
};


std::ostream& operator << (std::ostream& os, const GameState& state)
{
	if (state.current_round_number == 1) {
		return os << "-----------------------\n"
			<< "Round " << state.current_round_number << endl
			<< "People: " << state.res.people << endl
			<< "Wheat left: " << state.res.wheat << endl
			<< "Akrs: " << state.res.akrs << endl
			<< "Cost of akr: " << state.akr_cost
			<< endl;
	}

	return os << "-----------------------\n"
		<< "Round " << state.current_round_number << endl
		<< ((state.dead_people_prev == 0) ? "" : to_string(state.dead_people_prev)) 
		<< ((state.dead_people_prev == 0) ? "" : " people died from hunger\n") 
		<< ((state.was_chuma) ? "Chuma killed half of people\n" : "")
		<< "People: " << state.res.people << endl
		<< "Wheat income: " << state.wheat_income //<< endl
	//	<< "Akrs fertility: " << state.fertility_prev
		<< ((state.rats_damage_prev == 0) ? "" : "\nRats damaged wheat: ")
		<< ((state.rats_damage_prev == 0) ? "" : to_string(state.rats_damage_prev))
		<< "\nWheat left: " << state.res.wheat << endl
		<< "Akrs: " << state.res.akrs << endl
		<< "Cost of akr: " << state.akr_cost
		<< endl;
}

enum GAME_RESULT {
	TERRIBLE,
	BAD,
	NOT_BAD,
	FINE
};

struct Game {

	GameState state;
	string save_filename = "game_save.txt";
public:
	void play() {
		// проверить загрузку, спросить - надо ли загрузить
		
		char is_exit;
		bool is_correct_input;
		
		bool was_load = offer_load();


		//state.init_round();
		//cout << state;
		//return;

		if (!was_load)
			state.init_round();

		while (state.current_round_number <= 10) {

			cout << state;

			cout << "Do you want to continue game? Y/N: ";
			cin >> is_exit;
			if (is_exit == 'N') {
				save();
				cout << "Game was saved.";
				return;
			}

			Decision decision;
			get_round_input(decision);

			// здесь будут записаны акры после сделки и пшеница после сделки
			// но не будет съеденной и взрощенной пшеницы
			Resources new_res(state.res);
			is_correct_input = state.check_decision(decision, new_res);
			while (!is_correct_input) {
				print_incorrect_input();
				get_round_input(decision);
				new_res = state.res;
				is_correct_input = state.check_decision(decision, new_res);
			}

			state.res = new_res;
			float mortality = state.post_decision_events(decision);
			
			if (mortality > 0.45) {
				cout << "\nGAME OVER." << endl;
				cout << state;
				cout << "More than 45% of people died. " << state.current_round_number << endl;
				cout << "TRY AGAIN!" << endl << endl;
				return;
			}

			state.init_round();
			save();
		}

		estimate_game_result();
	}

	bool offer_load() {
		if (ifstream(save_filename))
		{
			cout << "Do you want to load previous game? (Y/N): ";
			char is_load;
			cin >> is_load;
			if (is_load == 'Y') {
				load();
				return true;
			}
			return false;
		}
		return false;
	}

	void load() {
		ifstream file;
		file.open(save_filename);

		if (!file.is_open()) {
			cout << "ERROR: save file was not opened to LOAD!";
			return;
		}

		file >> state.res.akrs;
		file >> state.res.people;
		file >> state.res.wheat;
		file >> state.akr_cost;
		file >> state.current_round_number;
		file >> state.dead_people_prev;
		file >> state.fertility_prev;
		file >> state.mortality_persentage;
		file >> state.people_come_prev;
		file >> state.rats_damage_prev;
		file >> state.was_chuma;
		file >> state.wheat_income;
		file >> state.rats_damage_coeff;

		file.close();
	}

	void save() {
		ofstream file;
		file.open(save_filename);

		if (!file.is_open()) {
			cout << "ERROR: save file was not opened to SAVE!";
			return;
		}

		file << state.res.akrs << endl;
		file << state.res.people << endl;
		file << state.res.wheat << endl;
		file << state.akr_cost << endl;
		file << state.current_round_number << endl;
		file << state.dead_people_prev << endl;
		file << state.fertility_prev << endl;
		file << state.mortality_persentage << endl;
		file << state.people_come_prev << endl;
		file << state.rats_damage_prev << endl;
		file << state.was_chuma << endl;
		file << state.wheat_income << endl;
		file << state.rats_damage_coeff << endl;

		file.close();
	}

	void get_round_input(Decision& decision) {
		cout << "--\n";
		cout << "What is your decision?" << endl;

		cout << "How much akrs to buy (<0 if for sell): ";
		cin >> decision.akrs_to_bargain;

		cout << "How much wheat to eat: ";
		cin >> decision.wheat_to_eat;

		cout << "How much wheat to grow: ";
		cin >> decision.wheat_to_grow;
	}

	void print_incorrect_input() {
		cout << "The decision is impossible: we have only "
			<< state.res.akrs << " akrs, "
			<< state.res.wheat << " wheat, "
			<< state.res.people << " people!"
			<< endl;
	}

	int estimate_game_result_function() {
		float P = state.mortality_persentage * 1.0 / 10;
		
		if (state.res.people == 0)
			return GAME_RESULT::TERRIBLE;
		
		int L = state.res.akrs / state.res.people;

		if ((P > 0.33) && (L < 7)) {
			return GAME_RESULT::TERRIBLE;
		}
		else if ((P > 0.1) && (L < 9)) {
			return GAME_RESULT::BAD;
		}
		else if ((P > 0.03) && (L < 10)) {
			return GAME_RESULT::NOT_BAD;
		}
		else {
			return GAME_RESULT::FINE;
		}
	}

	void estimate_game_result() {
		int result = estimate_game_result_function();

		cout << "----------------------------\n"
			<< "--------- GAME END ---------\n";

		switch (result)
		{
		case GAME_RESULT::TERRIBLE:
			cout << "Due to your incompetence, you were exiled out of the city by people.";
		case GAME_RESULT::BAD:
			cout << "It was tough time, people will not vote for you again.";
		case GAME_RESULT::NOT_BAD:
			cout << "Not bad result! Lots of people vote for you!";
		case GAME_RESULT::FINE:
			cout << "Fantastic result! People vote for you!";
		default:
			cout << "ERROR: result estimation returns bad code.";
		};
	}
};

//------------ TESTS --------------------


#include <vector>
#include <cassert>

// ! тесты не проверяют полный раунд, инициализацию раунда, генерацию случайных значений

void test_estimate_game_result_function() {
	Game game;

	vector<Resources> resources = {
		{WHEAT_DEF, 11, 1},
		{WHEAT_DEF, 20, 2},
		{WHEAT_DEF, 19, 2}, // x < 10
		{WHEAT_DEF, 27, 3}, 
		{WHEAT_DEF, 8, 1},	// x < 9
		{WHEAT_DEF, 7, 1},
		{WHEAT_DEF, 6, 1},	// x < 7
		{WHEAT_DEF, 5, 1},
	};

	// все значения <= 0.45 * 10
	// на 0.3 и прочих точных значениях границ начинаются проблемы
	vector<vector<float>> mortality_persentage = {
		{0, 0.1, 0.29},
		{0.301, 0.99},		// x > 0.03
		{1.01, 3.2999 },	// x > 0.1
		{3.3005, 4.5}			// x > 0.33
	};

	vector<vector<GAME_RESULT>> expected_result = {
		{GAME_RESULT::FINE, GAME_RESULT::FINE, GAME_RESULT::FINE, GAME_RESULT::FINE, GAME_RESULT::FINE, GAME_RESULT::FINE, GAME_RESULT::FINE, GAME_RESULT::FINE},
		{GAME_RESULT::FINE, GAME_RESULT::FINE, GAME_RESULT::NOT_BAD, GAME_RESULT::NOT_BAD, GAME_RESULT::NOT_BAD, GAME_RESULT::NOT_BAD, GAME_RESULT::NOT_BAD, GAME_RESULT::NOT_BAD},
		{GAME_RESULT::FINE, GAME_RESULT::FINE, GAME_RESULT::NOT_BAD, GAME_RESULT::NOT_BAD, GAME_RESULT::BAD, GAME_RESULT::BAD, GAME_RESULT::BAD, GAME_RESULT::BAD},
		{GAME_RESULT::FINE, GAME_RESULT::FINE, GAME_RESULT::NOT_BAD, GAME_RESULT::NOT_BAD, GAME_RESULT::BAD, GAME_RESULT::BAD, GAME_RESULT::TERRIBLE, GAME_RESULT::TERRIBLE}
	};

	// Проверяем:
	// ветвление
	// (вычисления P и L не проверяем напрямую)

	for (int i = 0; i < mortality_persentage.size(); ++i) {
		for (int j = 0; j < mortality_persentage[i].size(); ++j) {
			game.state.mortality_persentage = mortality_persentage[i][j];
			for (int k = 0; k < resources.size(); ++k) {
				game.state.res = resources[k];
				//cout << "i= " << i << " j= " << j << " k= " << k << " result= " << game.estimate_game_result_function() << " expected= " << expected_result[i][k] << endl;
				assert(game.estimate_game_result_function() == expected_result[i][k]);
			}
		}
	}
}

/*
void test_mortality_percentage() {

}

void test_mortality() {

} */

void test_people_eat() {
	GameState state;
	
	vector<Resources> resources = {
		{WHEAT_DEF, AKRS_DEF, PEOPLE_DEF},
		{WHEAT_DEF, AKRS_DEF, PEOPLE_DEF},
		{WHEAT_DEF, AKRS_DEF, PEOPLE_DEF},
		{WHEAT_DEF, AKRS_DEF, 1},
		{WHEAT_DEF, AKRS_DEF, 1},
		{WHEAT_DEF, AKRS_DEF, 1}
	};

	// akrs, eat, grow
	vector<Decision> decisions = {
		{0, 2000, 0},
		{0, 1999, 0},
		{0, 0, 0},
		{0, 20, 0},
		{0, 21, 0},
		{0, 19, 0}
	};

	vector<int> expected_people_deat = {
		0, 1, 100, 0, 0, 1
	};

	vector<Resources> expected_res = {
		{WHEAT_DEF - 2000, AKRS_DEF, PEOPLE_DEF},
		{WHEAT_DEF - 1999, AKRS_DEF, PEOPLE_DEF - 1},
		{WHEAT_DEF, AKRS_DEF, 0},
		{WHEAT_DEF - 20, AKRS_DEF, 1},
		{WHEAT_DEF - 20, AKRS_DEF, 1},
		{WHEAT_DEF - 19, AKRS_DEF, 0},
	};

	// Проверить:
	// пшеницы хватает
	// пшеницы не хватает
	// правильно вычисляется
	// правильно обновляется пшеница
	// правильно обновляются люди
	// правильно обновляются умершие
	for (int i = 0; i < decisions.size(); ++i) {
		state.res = resources[i];
		state.people_eat(decisions[i]);
		assert(state.res.people == expected_res[i].people);
		assert(state.dead_people_prev == expected_people_deat[i]);
		assert(state.res.wheat == expected_res[i].wheat);
	}

}

void test_rats_make_damage() {
	GameState state;
	float rats_damage = 0.04;
	state.rats_damage_coeff = rats_damage;
	GameState state_def = state;

	vector<Resources> resources = {
		{WHEAT_DEF, AKRS_DEF, PEOPLE_DEF},
		{3000, AKRS_DEF, PEOPLE_DEF},
		{0, AKRS_DEF, PEOPLE_DEF}
	};

	// wheat, arks, people
	vector<Resources> expected_res = {
		{WHEAT_DEF - 112, AKRS_DEF, PEOPLE_DEF},
		{3000 - 120, AKRS_DEF, PEOPLE_DEF},
		{0, AKRS_DEF, PEOPLE_DEF}
	};

	vector<int> expected_damage = {
		112, 120, 0
	};

	// Проверить:
	// пшеница res.wheat уменьшается
	// обновляется rats_damage_prev
	// подсчет верный
	for (int i = 0; i < expected_res.size(); ++i) {
		state.res = resources[i];
		state.rats_make_damage();
		assert(state.res.wheat == expected_res[i].wheat);
		assert(state.rats_damage_prev == expected_damage[i]);
		state = state_def;
	}
}

void test_wheat_in() {
	GameState state;
	int t_fertility = 5;
	state.fertility_prev = t_fertility;
	GameState state_def = state;

	// akrs, eat, grow
	vector<Decision> decisions = {
		{0, 0, 100},
		{0, 0, 0},
		{0, 0, 2}
	};

	// wheat, arks, people
	vector<Resources> expected_res = {
		{WHEAT_DEF + 1000, AKRS_DEF, PEOPLE_DEF},
		{WHEAT_DEF, AKRS_DEF, PEOPLE_DEF},
		{WHEAT_DEF + 20, AKRS_DEF, PEOPLE_DEF}
	};

	vector<int> expected_income = {
		1000, 0, 20
	};

	// Проверить:
	// пшеница res.wheat увеличивается
	// обновляется wheat_income
	// подсчет верный
	for (int i = 0; i < decisions.size(); ++i) {
		state.wheat_in(decisions[i]);
		assert(state.res.wheat == expected_res[i].wheat);
		assert(state.wheat_income == expected_income[i]);
		state = state_def;
	}

}

void test_people_come() {
	GameState state;

	vector<Resources> resources = {
		{WHEAT_DEF, AKRS_DEF, PEOPLE_DEF},
		{WHEAT_DEF, AKRS_DEF, PEOPLE_DEF},
		{600, AKRS_DEF, PEOPLE_DEF},
		{600, AKRS_DEF, PEOPLE_DEF},
		{1200, AKRS_DEF, PEOPLE_DEF},
		{1200, AKRS_DEF, PEOPLE_DEF},
		{1200, AKRS_DEF, PEOPLE_DEF},
		{599, AKRS_DEF, PEOPLE_DEF},
		{600, AKRS_DEF, PEOPLE_DEF},
		{600, AKRS_DEF, PEOPLE_DEF},
		{600 * 50, AKRS_DEF, PEOPLE_DEF},
	};

	vector<int> fetiliry = {
		5, 5, 6, 6, 6 , 6, 6, 4, 4, 4, 4
	};
	vector<int> people_dead = {
		0, 2, 0, 2, 0, 2, 4, 2, 100, 200, 0
	};

	vector<int> exptected_people_come = {
		1, 2, 0, 1 , 0, 0, 1, 2, 50, 50, 50
	};

	// Проверяем:
	// правильное вычисление по формуле
	// границы: сброс до 50, если выше
	// сброс до 0, если меньше 0

	for (int i = 0; i < resources.size(); ++i) {
		state.res = resources[i];
		state.fertility_prev = fetiliry[i];
		state.dead_people_prev = people_dead[i];
		state.people_come();
		assert(state.people_come_prev == exptected_people_come[i]);
	}
}

void test_chuma() {
	GameState state;

	vector<Resources> resources = {
		{WHEAT_DEF, AKRS_DEF, PEOPLE_DEF},
		{WHEAT_DEF, AKRS_DEF, 1},
		{WHEAT_DEF, AKRS_DEF, 2},
		{WHEAT_DEF, AKRS_DEF, 3}
	};

	vector<int> expected_people_after_chuma = {
		50, 0, 1, 1
	};

	// Проверка:
	// если чума была - число людей уменьшилось
	// правильное вычисление с округлением вниз
	// если не было - число людей не изменилось

	state.was_chuma = true;
	for (int i = 0; i < resources.size(); ++i) {
		state.res = resources[i];
		state.chuma();
		assert(state.res.people == expected_people_after_chuma[i]);
	}

	state.was_chuma = false;
	for (int i = 0; i < resources.size(); ++i) {
		state.res = resources[i];
		state.chuma();
		assert(state.res.people == resources[i].people);
	}
}

void test_post_decision_events() {
	test_wheat_in();
	test_rats_make_damage();
	test_people_eat();
	test_people_come();
	test_chuma();
	//test_mortality();
	//test_mortality_percentage();
	test_estimate_game_result_function();
}

void test_check_decision_akrs() {
	GameState state;
	int t_fertility = 5;
	int t_cost = 20;
	state.fertility_prev = t_fertility;
	state.akr_cost = t_cost;
	Resources new_res(state.res);

	// АКРЫ
	// корректное значение (0)
	// слишком дорого для покупки,
	// слишком много для продажи,
	// корректная покупка (акры, пшеница)
	// корректная продажа (акры, пшеница)

	// akrs, eat, grow
	vector<Decision> decisions_akrs = {
		{0, 0, 0},
		{ 10000000, 0, 0 },
		{-10000, 0, 0},
		{2, 0, 0},
		{-2, 0, 0} };

	// wheat, arks, people
	vector<Resources> expected_res_akrs = {
		{WHEAT_DEF, AKRS_DEF, PEOPLE_DEF},
		{WHEAT_DEF, AKRS_DEF, PEOPLE_DEF},
		{WHEAT_DEF, AKRS_DEF, PEOPLE_DEF},
		{WHEAT_DEF - 40, AKRS_DEF + 2, PEOPLE_DEF},
		{WHEAT_DEF + 40, AKRS_DEF - 2, PEOPLE_DEF}
	};
	vector<bool> expected_bool_akrs = {
		1, 0, 0, 1, 1
	};

	for (int i = 0; i < decisions_akrs.size(); ++i) {
		new_res = state.res;
		assert(state.check_decision(decisions_akrs[i], new_res) == expected_bool_akrs[i]);
		assert(new_res == expected_res_akrs[i]);
	}
}

void test_check_decision_eat() {
	GameState state;
	int t_fertility = 5;
	int t_cost = 20;
	state.fertility_prev = t_fertility;
	state.akr_cost = t_cost;
	Resources new_res(state.res);

	// ПШЕНИЦА ДЛЯ ЕДЫ
	// корректное отложили для еды из общего числа
	// корректное отложили для еды из общего числа после покупки
	// отрицательное значение
	// слишком большое значение
	// 
	// akrs, eat, grow
	vector<Decision> decisions_eat = {
		{0, 100, 0},
		{ 2, 100, 0 },
		{0, -2, 0},
		{0, 3000, 0},
	 };

	// wheat, arks, people
	vector<Resources> expected_res_eat = {
		{WHEAT_DEF, AKRS_DEF, PEOPLE_DEF},
		{WHEAT_DEF - 40, AKRS_DEF + 2, PEOPLE_DEF},
		{WHEAT_DEF, AKRS_DEF, PEOPLE_DEF},
		{WHEAT_DEF, AKRS_DEF, PEOPLE_DEF}
	};
	vector<bool> expected_bool_eat = {
		1, 1, 0, 0
	};

	for (int i = 0; i < decisions_eat.size(); ++i) {
		new_res = state.res;
		assert(state.check_decision(decisions_eat[i], new_res) == expected_bool_eat[i]);
		assert(new_res == expected_res_eat[i]);
	}
}

void test_check_decision_grow() {
	GameState state;
	int t_fertility = 5;
	int t_cost = 20;
	state.fertility_prev = t_fertility;
	state.akr_cost = t_cost;
	Resources new_res(state.res);

	// ПШЕНИЦА ДЛЯ ПОСАДКИ
	// отрицательное значение
	// слишком большое значение
	// слишком большое значение (после отложения на еду)
	// слишком большое значение (не хватает людей, хватает акров)
	// слишком большое значение (не хватает акров, хватает людей)
	// корректное значение (>0)
		// 
	// akrs, eat, grow
	vector<Decision> decisions_grow = {
		{0, 0, -20},
		{ 0, 0, 3000 },
		{0, 801, 2000},
		{2, 0, 2004},
		{-2, 0, 2000},
		{0, 0, 2000}
	};

	vector<bool> expected_bool_eat = {
		0, 0, 0, 0, 0, 1
	};

	for (int i = 0; i < decisions_grow.size(); ++i) {
		new_res = state.res;
		assert(state.check_decision(decisions_grow[i], new_res) == expected_bool_eat[i]);
	}
}

void test_check_decision() {
	test_check_decision_akrs();
	test_check_decision_eat();
	test_check_decision_grow();
}

/*
Использовать после теста на сейв.
*/
void test_load() {
	Game game;
	game.offer_load();
	//game.load();
	cout << game.state;
}

/*
Сейв для проверки, что bool и float нормально записываются и считываются без форматирования
*/
void test_save() {
	GameState state;
	state.was_chuma = true;
	state.mortality_persentage = 0.0204;
	Game game;
	game.state = state;
	game.save();
}

void test() {
	test_check_decision();
	test_post_decision_events();
}

// ---------------- TESTS END -----------------------

int main() {
	

	//test_save();
	//test_load();
	test();
	return 1;

	Game game;
	srand(time(0));
	game.play();
}