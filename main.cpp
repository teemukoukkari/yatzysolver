#include<array>
#include<iostream>
#include<cmath>
#include<utility>
#include<cstdint>
#include<fstream>
#include<algorithm>
#include<thread>
#include<filesystem>

#include "httlib.h"

constexpr int PORT = 8080;
constexpr int NUM_THREADS = 16;

constexpr int CATEGORY_ONES = 0;
constexpr int CATEGORY_TWOS = 1;
constexpr int CATEGORY_THREES = 2;
constexpr int CATEGORY_FOURS = 3;
constexpr int CATEGORY_FIVES = 4;
constexpr int CATEGORY_SIXES = 5;
constexpr int CATEGORY_ONE_PAIR = 6;
constexpr int CATEGORY_TWO_PAIRS = 7;
constexpr int CATEGORY_THREE_OF_A_KIND = 8;
constexpr int CATEGORY_FOUR_OF_A_KIND = 9;
constexpr int CATEGORY_SMALL_STRAIGHT = 10;
constexpr int CATEGORY_LARGE_STRAIGHT = 11;
constexpr int CATEGORY_FULL_HOUSE = 12;
constexpr int CATEGORY_CHANCE = 13;
constexpr int CATEGORY_YATZY = 14;
constexpr int CATEGORY_COUNT = 15;

struct State {
    uint16_t categories;
    uint8_t upper_score;
    
    State() = default;

    State(int key) {
        categories = static_cast<uint16_t>(key % 32768);
        upper_score = static_cast<uint8_t>(key / 32768);
    }

    bool get_category(int category) const {
        return (categories >> category) & 1;
    }

    void set_category(int category) {
        categories |= 1 << category;
    }

    void increase_upper_score(int amount) {
        upper_score += amount;
        if (upper_score > 63) {
            upper_score = 63;
        }
    }

    bool is_end_state() const {
        return categories == 0b0111111111111111;
    }

    int get_bonus() const {
        return upper_score >= 63 ? 50 : 0;
    }

    static constexpr int KEY_COUNT = 2097152;
    int key() const {
        return categories + upper_score * 32768;
    }

    static std::array<std::vector<int>, 16> get_keys_by_open() {
        std::array<std::vector<int>, 16> keys_by_open;
        for (int i = 0; i < KEY_COUNT; i++) {
            State state(i);
            int open = 0;
            for (int j = 0; j < 15; j++) {
                if (!state.get_category(j)) {
                    open++;
                }
            }
            keys_by_open[open].push_back(i);
        }
        return keys_by_open;
    }
};

struct Dice {
    uint8_t die1, die2, die3, die4, die5; // MUST be in increasing order

    bool operator==(const Dice& other) const {
        return die1 == other.die1 &&
               die2 == other.die2 &&
               die3 == other.die3 &&
               die4 == other.die4 &&
               die5 == other.die5;
    }

    std::array<Dice, 32> get_holds() const {
        std::array<Dice, 32> holds;
        for (int i = 0; i < 32; i++) {
            Dice hold = *this;
            if (i & 1) { hold.die1 = 0; }
            if (i & 2) { hold.die2 = 0; }
            if (i & 4) { hold.die3 = 0; }
            if (i & 8) { hold.die4 = 0; }
            if (i & 16) { hold.die5 = 0; }
            holds[i] = hold;
        }
        return holds;
    }
    
    double calculate_probability(const Dice holds = {0,0,0,0,0}) const {
        constexpr int factorials[7] = { 1, 1, 2, 6, 24, 120 };

        std::array<int, 7> counts = {0};
        counts[die1]++;
        counts[die2]++;
        counts[die3]++;
        counts[die4]++;  
        counts[die5]++;
        counts[holds.die1]--;
        counts[holds.die2]--;
        counts[holds.die3]--;
        counts[holds.die4]--;
        counts[holds.die5]--;
        
        int total = 0;
        for (int i = 1; i < 7; i++) {
            if (counts[i] < 0) {
                return 0;
            }
            total += counts[i];
        }

        double probability = factorials[total] / (double)pow(6, total);
        for (int i = 1; i < 7; i++) {
            probability /= factorials[counts[i]];
        }
        return probability;
    }

    int score_upper(int number) const {
        return number * (
            (die1 == number) +
            (die2 == number) +
            (die3 == number) +
            (die4 == number) +
            (die5 == number)
        );
    }

    int score_one_pair() const {
        if (die5 == die4) { return 2 * die5; }
        if (die4 == die3) { return 2 * die4; }
        if (die3 == die2) { return 2 * die3; }
        if (die2 == die1) { return 2 * die2; }
        return 0;
    }

    int score_two_pairs() const {
        if (die1 == die2 && die3 == die4 && die1 != die3) { return 2 * die1 + 2 * die3; }
        if (die1 == die2 && die4 == die5 && die1 != die4) { return 2 * die1 + 2 * die4; }
        if (die2 == die3 && die4 == die5 && die2 != die4) { return 2 * die2 + 2 * die4; }
        return 0;
    }

    int score_three_of_a_kind() const {
        if (die1 == die2 && die1 == die3) { return 3 * die1; }
        if (die2 == die3 && die2 == die4) { return 3 * die2; }
        if (die3 == die4 && die3 == die5) { return 3 * die3; }
        return 0;
    }

    int score_four_of_a_kind() const {
        if (die1 == die2 && die1 == die3 && die1 == die4) { return 4 * die1; }
        if (die2 == die3 && die2 == die4 && die2 == die5) { return 4 * die2; }
        return 0;
    }

    int score_small_straight() const {
        if (die1 == 1 && die2 == 2 && die3 == 3 && die4 == 4 && die5 == 5) { return 15; }
        return 0;
    }

    int score_large_straight() const {
        if (die1 == 2 && die2 == 3 && die3 == 4 && die4 == 5 && die5 == 6) { return 20; }
        return 0;
    }

    int score_full_house() const {
        if (die1 == die2 && die1 == die3 && die4 == die5 && die1 != die4) { return 3 * die1 + 2 * die4; }
        if (die1 == die2 && die3 == die4 && die3 == die5 && die1 != die3) { return 2 * die1 + 3 * die3; }
        return 0;
    }

    int score_chance() const {
        return die1 + die2 + die3 + die4 + die5;
    }

    int score_yatzy() const {
        if (die1 == die2 && die1 == die3 && die1 == die4 && die1 == die5) { return 50; }
        return 0;
    };

    int score(int category) const {
        switch (category) {
            case CATEGORY_ONES: return score_upper(1);
            case CATEGORY_TWOS: return score_upper(2);
            case CATEGORY_THREES: return score_upper(3);
            case CATEGORY_FOURS: return score_upper(4);
            case CATEGORY_FIVES: return score_upper(5);
            case CATEGORY_SIXES: return score_upper(6);
            case CATEGORY_ONE_PAIR: return score_one_pair();
            case CATEGORY_TWO_PAIRS: return score_two_pairs();
            case CATEGORY_THREE_OF_A_KIND: return score_three_of_a_kind();
            case CATEGORY_FOUR_OF_A_KIND: return score_four_of_a_kind();
            case CATEGORY_SMALL_STRAIGHT: return score_small_straight();
            case CATEGORY_LARGE_STRAIGHT: return score_large_straight();
            case CATEGORY_FULL_HOUSE: return score_full_house();
            case CATEGORY_CHANCE: return score_chance();
            case CATEGORY_YATZY: return score_yatzy();
            default: return 0;
        }
    }
};

/*************************************************************************************************/

std::array<double, State::KEY_COUNT> initialize_states() {
    std::array<double, State::KEY_COUNT> states;
    states.fill(-1);
    return states;
}

std::array<Dice, 252> initialize_combos() {
    std::array<Dice, 252> combos;
    int i = 0;
    Dice dice;
    for (dice.die1 = 1; dice.die1 <= 6; dice.die1++) {
        for (dice.die2 = dice.die1; dice.die2 <= 6; dice.die2++) {
            for (dice.die3 = dice.die2; dice.die3 <= 6; dice.die3++) {
                for (dice.die4 = dice.die3; dice.die4 <= 6; dice.die4++) {
                    for (dice.die5 = dice.die4; dice.die5 <= 6; dice.die5++) {
                        combos[i++] = dice;
                    }
                }
            }
        }
    }
    return combos;
}

constexpr int COMBO_COUNT = 252;
std::array<double, State::KEY_COUNT> states = initialize_states();
const std::array<Dice, COMBO_COUNT> combos = initialize_combos();

void load_states(const std::string& filename) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening file " << filename << std::endl;
        exit(1);
    }

    infile.read(reinterpret_cast<char*>(states.data()), states.size() * sizeof(states[0]));
    if (!infile) {
        std::cerr << "Error reading file " << filename << std::endl;
        exit(1);
    }

    infile.close();
    std::cout << "Loaded states from file " << filename << std::endl;
}

void save_states(const std::string& filename) {
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile) {
        std::cerr << "Error writing file " << filename << std::endl;
        exit(1);
    }

    outfile.write(reinterpret_cast<const char*>(states.data()), states.size() * sizeof(states[0]));
    if (!outfile) {
        std::cerr << "Error writing file " << filename << std::endl;
        exit(1);
    }

    outfile.close();
    std::cout << "Saved states to file " << filename << std::endl;
}

double calculate_state(const State state);

double get_state_value(const State state) {
    int key = state.key();
    if (states[key] != -1) {
        return states[key];
    }

    double value = calculate_state(state);
    states[key] = value;
    return value;
}

void work_function(const std::vector<int> keys, int start, int end) {
    for (int i = start; i < end; i++) {
        State state(keys[i]);
        states[keys[i]] = calculate_state(state);
    }
}

void generate_all_states() {
    std::cout << "Starting generating all states." << std::endl;
    std::array<std::vector<int>, 16> keys_by_open = State::get_keys_by_open();
    
    std::array<std::thread, NUM_THREADS> threads;
    for (int open = 0; open < 16; open++) {
        std::cout << "Calculating states with " << open << " open categories ("
            << keys_by_open[open].size() << " keys)" << std::endl;
        for (int i = 0; i < NUM_THREADS; i++) {
            const auto& keys = keys_by_open[open];
            threads[i] = std::thread(
                work_function,
                keys,
                i * (keys.size() / NUM_THREADS),
                (i + 1) * (keys.size() / NUM_THREADS)
            );
        }
        for (int i = 0; i < NUM_THREADS; i++) {
            threads[i].join();
        }
    }
    std::cout << "Finished generating all states." << std::endl;
}

double calculate_state(const State state) {
    if (state.is_end_state()) {
        return state.get_bonus();
    }

    std::array<double, COMBO_COUNT> category_select_values;
    for (int i = 0; i < COMBO_COUNT; i++) {
        double best_value = 0;
        for (int category = 0; category < CATEGORY_COUNT; category++) {
            if (state.get_category(category)) {
                continue;
            }
        
            State new_state = state;
            new_state.set_category(category);
            if (category <= CATEGORY_SIXES) {
                new_state.increase_upper_score(combos[i].score(category));
            }

            double value = combos[i].score(category) + get_state_value(new_state);
            if (value >= best_value) {
                best_value = value;
            }
        }
        category_select_values[i] = best_value;
    }

    std::array<double, COMBO_COUNT> second_hold_values;
    for (int i = 0; i < COMBO_COUNT; i++) {
        double best_value = 0;
        for (Dice hold : combos[i].get_holds()) {
            double value = 0;
            for (int roll = 0; roll < COMBO_COUNT; roll++) {
                double probability = combos[roll].calculate_probability(hold);
                value += probability * category_select_values[roll];
            }
            if (value >= best_value) {
                best_value = value;
            }
        }
        second_hold_values[i] = best_value;
    }

    std::array<double, COMBO_COUNT> first_hold_values;
    for (int i = 0; i < COMBO_COUNT; i++) {
        double best_value = 0;
        for (Dice hold : combos[i].get_holds()) {
            double value = 0;
            for (int roll = 0; roll < COMBO_COUNT; roll++) {
                double probability = combos[roll].calculate_probability(hold);
                value += probability * second_hold_values[roll];
            }
            if (value >= best_value) {
                best_value = value;
            }
        }
        first_hold_values[i] = best_value;
    }

    double value = 0;
    for (int roll = 0; roll < COMBO_COUNT; roll++) {
        double probability = combos[roll].calculate_probability();
        value += probability * first_hold_values[roll];
    }
    return value;
}

/*************************************************************************************************/

struct QueryCache {
    std::array<double, COMBO_COUNT> second_roll;
    std::array<double, COMBO_COUNT> third_roll;
    QueryCache() {
        second_roll.fill(-1);
        third_roll.fill(-1);
    }
};

double query_category_select(const State state, const int combo, const int category) {
    State new_state = state;
    new_state.set_category(category);
    if (category <= CATEGORY_SIXES) {
        new_state.increase_upper_score(combos[combo].score(category));
    }
    return {combos[combo].score(category) + get_state_value(new_state)};
}

std::pair<double, int> query_third_roll(const State state, const int combo, QueryCache* cache = nullptr) {
    if(cache && cache->third_roll[combo] != -1) {
        return {cache->third_roll[combo], -1};
    }

    double best_value = 0;
    int best_category;
    for (int category = 0; category < CATEGORY_COUNT; category++) {
        if (state.get_category(category)) {
            continue;
        }
        double value = query_category_select(state, combo, category);
        if (value >= best_value) {
            best_value = value;
            best_category = category;
        }
    }
    if (cache) {
        cache->third_roll[combo] = best_value; 
    }
    return {best_value, best_category};
}

double query_second_hold(const State state, const Dice hold, QueryCache* cache = nullptr) {
    double value = 0;
    for (int roll = 0; roll < COMBO_COUNT; roll++) {
        double probability = combos[roll].calculate_probability(hold);
        value += probability * query_third_roll(state, roll, cache).first;
    }
    return value;
}

std::pair<double, Dice> query_second_roll(const State state, const int combo, QueryCache* cache = nullptr) {
    if (cache && cache->second_roll[combo] != -1) {
        return {cache->second_roll[combo], Dice()};
    }

    double best_value = 0;
    Dice best_hold;
    for (Dice hold : combos[combo].get_holds()) {
        double value = query_second_hold(state, hold, cache);
        if (value >= best_value) {
            best_value = value;
            best_hold = hold;
        }
    }
    if (cache) {
        cache->second_roll[combo] = best_value;
    }
    return {best_value, best_hold};
}

double query_first_hold(const State state, const Dice hold, QueryCache* cache = nullptr) {
    double value = 0;
    for (int roll = 0; roll < COMBO_COUNT; roll++) {
        double probability = combos[roll].calculate_probability(hold);
        value += probability * query_second_roll(state, roll, cache).first;
    }
    return value;
}

std::pair<double, Dice> query_first_roll(const State state, const int combo) {
    double best_value = 0;
    Dice best_hold;
    QueryCache cache{};
    for (Dice hold : combos[combo].get_holds()) {
        double value = query_first_hold(state, hold, &cache);
        if (value >= best_value) {
            best_value = value;
            best_hold = hold;
        }
    }
    return {best_value, best_hold};
}

double query_start_of_turn(const State state) {
    return get_state_value(state);
}

/*************************************************************************************************/

uint16_t categories_from_string(std::string s) {
    uint16_t categories = 0;
    for (int i = 0; i < 15; i++) {
        if (s[i] == '1') {
            categories |= (1 << i);
        }
    }
    return categories;
}

uint8_t upper_score_from_string(std::string s) {
    uint8_t upper_score = std::stoi(s);
    if (upper_score > 63) {
        upper_score = 63;
    }
    return upper_score;
}

int category_from_string(std::string s) {
    return std::stoi(s);
}

Dice dice_from_string(std::string s) {
    Dice dice = {
        static_cast<uint8_t>(s[0] - '0'),
        static_cast<uint8_t>(s[1] - '0'),
        static_cast<uint8_t>(s[2] - '0'),
        static_cast<uint8_t>(s[3] - '0'),
        static_cast<uint8_t>(s[4] - '0')
    };
    return dice;
}

std::string dice_to_string(Dice dice) {
    return (dice.die1 ? std::to_string(dice.die1) : "")
        + (dice.die2 ? std::to_string(dice.die2) : "")
        + (dice.die3 ? std::to_string(dice.die3) : "")
        + (dice.die4 ? std::to_string(dice.die4) : "")
        + (dice.die5 ? std::to_string(dice.die5) : "");
}

int combo_from_string(std::string s) {
    std::sort(s.begin(), s.end());
    Dice dice = dice_from_string(s);
    for (int i = 0; i < COMBO_COUNT; i++) {
        if (combos[i] == dice) {
            return i;
        }
    }
    return -1;
}

void server() {
    httplib::Server svr;
    svr.set_mount_point("", "./web");

    svr.Get("/api/start_of_turn", [](const httplib::Request &req, httplib::Response &res) {
        State state;
        state.categories = categories_from_string(req.get_param_value("categories"));
        state.upper_score = upper_score_from_string(req.get_param_value("upper_score"));

        double value = query_start_of_turn(state);
        res.set_content(
            "{\"value\":" + std::to_string(value) + "}",
            "application/json"
        );
    });

    svr.Get("/api/first_roll", [](const httplib::Request &req, httplib::Response &res) {
        State state;
        state.categories = categories_from_string(req.get_param_value("categories"));
        state.upper_score = upper_score_from_string(req.get_param_value("upper_score"));
        int combo = combo_from_string(req.get_param_value("dice"));

        auto [value, hold] = query_first_roll(state, combo);
        res.set_content(
            "{\"value\":" + std::to_string(value) + ",\"hold\":\"" + dice_to_string(hold) + "\"}",
            "application/json"
        );
    });

    svr.Get("/api/first_hold", [](const httplib::Request &req, httplib::Response &res) {
        State state;
        state.categories = categories_from_string(req.get_param_value("categories"));
        state.upper_score = upper_score_from_string(req.get_param_value("upper_score"));
        Dice hold = dice_from_string(req.get_param_value("hold"));

        double value = query_first_hold(state, hold);
        res.set_content(
            "{\"value\":" + std::to_string(value) + "}",
            "application/json"
        );
    });

    svr.Get("/api/second_roll", [](const httplib::Request &req, httplib::Response &res) {
        State state;
        state.categories = categories_from_string(req.get_param_value("categories"));
        state.upper_score = upper_score_from_string(req.get_param_value("upper_score"));
        int combo = combo_from_string(req.get_param_value("dice"));

        auto [value, hold] = query_second_roll(state, combo);
        res.set_content(
            "{\"value\":" + std::to_string(value) + ",\"hold\":\"" + dice_to_string(hold) + "\"}",
            "application/json"
        );
    });

    svr.Get("/api/second_hold", [](const httplib::Request &req, httplib::Response &res) {
        State state;
        state.categories = categories_from_string(req.get_param_value("categories"));
        state.upper_score = upper_score_from_string(req.get_param_value("upper_score"));
        Dice hold = dice_from_string(req.get_param_value("hold"));

        double value = query_second_hold(state, hold);
        res.set_content(
            "{\"value\":" + std::to_string(value) + "}",
            "application/json"
        );
    });

    svr.Get("/api/third_roll", [](const httplib::Request &req, httplib::Response &res) {
        State state;
        state.categories = categories_from_string(req.get_param_value("categories"));
        state.upper_score = upper_score_from_string(req.get_param_value("upper_score"));
        int combo = combo_from_string(req.get_param_value("dice"));

        auto [value, category] = query_third_roll(state, combo);
        res.set_content(
            "{\"value\":" + std::to_string(value) + ",\"category\":" + std::to_string(category) + "}",
            "application/json"
        );
    });

    svr.Get("/api/category_select", [](const httplib::Request &req, httplib::Response &res) {
        State state;
        state.categories = categories_from_string(req.get_param_value("categories"));
        state.upper_score = upper_score_from_string(req.get_param_value("upper_score"));
        int combo = combo_from_string(req.get_param_value("dice"));
        int category = category_from_string(req.get_param_value("category"));

        double value = query_category_select(state, combo, category);
        int score = combos[combo].score(category);
        res.set_content(
            "{\"value\":" + std::to_string(value) + ",\"score\":" + std::to_string(score) + "}",
            "application/json"
        );
    });

    std::cout << "Listening on 127.0.0.0:" << PORT << "/" << std::endl;
    svr.listen("127.0.0.1", PORT);
}

/*************************************************************************************************/

int main() {
    if (!std::filesystem::exists("states.bin")) {
        std::cout << "File states.bin not found. Generating it now." << std::endl;
        generate_all_states();
        save_states("states.bin");
    } else {
        load_states("states.bin");
    }
    server();
    return 0;
}