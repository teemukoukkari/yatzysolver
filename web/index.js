function get_categories() {
    let result = "";
    result += (document.getElementById("category_ones").value != "") ? "1" : "0";
    result += (document.getElementById("category_twos").value != "") ? "1" : "0";
    result += (document.getElementById("category_threes").value != "") ? "1" : "0";
    result += (document.getElementById("category_fours").value != "") ? "1" : "0";
    result += (document.getElementById("category_fives").value != "") ? "1" : "0";
    result += (document.getElementById("category_sixes").value != "") ? "1" : "0";
    result += (document.getElementById("category_one_pair").value != "") ? "1" : "0";
    result += (document.getElementById("category_two_pairs").value != "") ? "1" : "0";
    result += (document.getElementById("category_three_of_a_kind").value != "") ? "1" : "0";
    result += (document.getElementById("category_four_of_a_kind").value != "") ? "1" : "0";
    result += (document.getElementById("category_full_house").value != "") ? "1" : "0";
    result += (document.getElementById("category_small_straight").value != "") ? "1" : "0";
    result += (document.getElementById("category_large_straight").value != "") ? "1" : "0";
    result += (document.getElementById("category_chance").value != "") ? "1" : "0";
    result += (document.getElementById("category_yatzy").value != "") ? "1" : "0";
    return result;
}

function get_upper_score() {
    let result = 0;
    result += parseInt(document.getElementById("category_ones").value) || 0;
    result += parseInt(document.getElementById("category_twos").value) || 0;
    result += parseInt(document.getElementById("category_threes").value) || 0;
    result += parseInt(document.getElementById("category_fours").value) || 0;
    result += parseInt(document.getElementById("category_fives").value) || 0;
    result += parseInt(document.getElementById("category_sixes").value) || 0;
    return result;
}

function get_lower_score() {
    let result = 0;
    result += parseInt(document.getElementById("category_one_pair").value) || 0;
    result += parseInt(document.getElementById("category_two_pairs").value) || 0;
    result += parseInt(document.getElementById("category_three_of_a_kind").value) || 0;
    result += parseInt(document.getElementById("category_four_of_a_kind").value) || 0;
    result += parseInt(document.getElementById("category_full_house").value) || 0;
    result += parseInt(document.getElementById("category_small_straight").value) || 0;
    result += parseInt(document.getElementById("category_large_straight").value) || 0;
    result += parseInt(document.getElementById("category_chance").value) || 0;
    result += parseInt(document.getElementById("category_yatzy").value) || 0;
    return result;
}

function calculate_scoresheet() {
    const subtotal = get_upper_score();
    const bonus = subtotal >= 63 ? 50 : 0;
    const total = subtotal + bonus + get_lower_score();

    document.getElementById("category_subtotal").value = subtotal ? subtotal : "";
    document.getElementById("category_bonus").value = bonus ? bonus : "";
    document.getElementById("category_total").value = total ? total : "";
}

function update_category_score(category, score) {
    switch (category) {
        case "0":
            document.getElementById("category_ones").value = score;
            break;
        case "1":
            document.getElementById("category_twos").value = score;
            break;
        case "2":
            document.getElementById("category_threes").value = score;
            break;
        case "3":
            document.getElementById("category_fours").value = score;
            break;
        case "4":
            document.getElementById("category_fives").value = score;
            break;
        case "5":
            document.getElementById("category_sixes").value = score;
            break;
        case "6":
            document.getElementById("category_one_pair").value = score;
            break;
        case "7":
            document.getElementById("category_two_pairs").value = score;
            break;
        case "8":
            document.getElementById("category_three_of_a_kind").value = score;
            break;
        case "9":
            document.getElementById("category_four_of_a_kind").value = score;
            break;
        case "10":
            document.getElementById("category_full_house").value = score;
            break;
        case "11":
            document.getElementById("category_small_straight").value = score;
            break;
        case "12":
            document.getElementById("category_large_straight").value = score;
            break;
        case "13":
            document.getElementById("category_chance").value = score;
            break;
        case "14":
            document.getElementById("category_yatzy").value = score;
            break;
    }
    calculate_scoresheet()
}

function set_scoresheet_disabled(value) {
    document.getElementById("category_ones").disabled = value;
    document.getElementById("category_twos").disabled = value;
    document.getElementById("category_threes").disabled = value;
    document.getElementById("category_fours").disabled = value;
    document.getElementById("category_fives").disabled = value;
    document.getElementById("category_sixes").disabled = value;
    document.getElementById("category_subtotal").disabled = value;
    document.getElementById("category_bonus").disabled = value;
    document.getElementById("category_one_pair").disabled = value;
    document.getElementById("category_two_pairs").disabled = value;
    document.getElementById("category_three_of_a_kind").disabled = value;
    document.getElementById("category_four_of_a_kind").disabled = value;
    document.getElementById("category_full_house").disabled = value;
    document.getElementById("category_small_straight").disabled = value;
    document.getElementById("category_large_straight").disabled = value;
    document.getElementById("category_chance").disabled = value;
    document.getElementById("category_yatzy").disabled = value;
    document.getElementById("category_total").disabled = value;
}

function set_visible_stage(stage) {
    document.getElementById("row_start_of_turn").style.display = (stage >= 0 ? "table-row" : "none");
    document.getElementById("row_first_roll").style.display = (stage >= 1 ? "table-row" : "none");
    document.getElementById("row_first_hold").style.display = (stage >= 2 ? "table-row" : "none");
    document.getElementById("row_second_roll").style.display = (stage >= 3 ? "table-row" : "none");
    document.getElementById("row_second_hold").style.display = (stage >= 4 ? "table-row" : "none");
    document.getElementById("row_third_roll").style.display = (stage >= 5 ? "table-row" : "none");
    document.getElementById("row_category_select").style.display = (stage >= 6 ? "table-row" : "none");
    document.getElementById("row_end_of_turn").style.display = (stage >= 7 ? "table-row" : "none");

    document.getElementById("start_of_turn_cancel").style.display = (stage != 0 ? "block": "none");
    document.getElementById("start_of_turn_apply").style.display = (stage == 0 ? "block": "none");
    document.getElementById("first_roll_apply").style.display = (stage == 1 ? "block": "none");
    document.getElementById("first_hold_apply").style.display = (stage == 2 ? "block": "none");
    document.getElementById("second_roll_apply").style.display = (stage == 3 ? "block": "none");
    document.getElementById("second_hold_apply").style.display = (stage == 4 ? "block": "none");
    document.getElementById("third_roll_apply").style.display = (stage == 5 ? "block": "none");
    document.getElementById("category_select_apply").style.display = (stage == 6 ? "block": "none");
    document.getElementById("end_of_turn_apply").style.display = (stage == 7 ? "block": "none");
}

function set_dice_disabled(stage, value) {
    document.getElementById(stage + "_die_1").disabled = value;
    document.getElementById(stage + "_die_2").disabled = value;
    document.getElementById(stage + "_die_3").disabled = value;
    document.getElementById(stage + "_die_4").disabled = value;
    document.getElementById(stage + "_die_5").disabled = value;
}

function set_hold_disabled(stage, value) {
    document.getElementById(stage + "_hold_1").disabled = value;
    document.getElementById(stage + "_hold_2").disabled = value;
    document.getElementById(stage + "_hold_3").disabled = value;
    document.getElementById(stage + "_hold_4").disabled = value;
    document.getElementById(stage + "_hold_5").disabled = value;
}

function set_category_disabled(value) {
    document.getElementById("category").disabled = value;
}


function reset_stages() {
    set_visible_stage(0);
    for (let i = 1; i <= 5; i++) {
        document.getElementById("first_die_" + i).value = "unknown";
        document.getElementById("second_die_" + i).value = "unknown";
        document.getElementById("third_die_" + i).value = "unknown";

        document.getElementById("first_die_" + i).disabled = false;
        document.getElementById("second_die_" + i).disabled = false;
        document.getElementById("third_die_" + i).disabled = false;

        document.getElementById("first_roll_value").innerHTML = "";
        document.getElementById("second_roll_value").innerHTML = "";
        document.getElementById("third_roll_value").innerHTML = "";

        document.getElementById("first_hold_" + i).disabled = false;
        document.getElementById("second_hold_" + i).disabled = false;
        document.getElementById("first_hold_" + i).checked = false;
        document.getElementById("second_hold_" + i).checked = false;
        document.getElementById("first_holdspan_" + i).className = "hold-checkbox";
        document.getElementById("second_holdspan_" + i).className = "hold-checkbox";
    }
    set_category_disabled(false);
    set_scoresheet_disabled(false);
}

/*************************************************************************************************/

function get_dice(stage) {
    let result = "";
    for (let i = 1; i <= 5; i++) {
        let value = document.getElementById(stage + "_die_" + i).value;
        if (value == "unknown") {
            return null
        }
        result += value.toString()
    }
    return result;
}

function force_dice(stage) {
    for (let i = 1; i <= 5; i++) {
        let value = document.getElementById(stage + "_die_" + i).value;
        if (value == "unknown") {
            value = Math.floor(Math.random() * 6) + 1;
            document.getElementById(stage + "_die_" + i).value = value;
        }
    }
}

function get_hold(stage) {
    let result = "";
    for (let i = 1; i <= 5; i++) {
        let value = document.getElementById(stage + "_die_" + i).value;
        let checked = document.getElementById(stage + "_hold_" + i).checked;
        result += checked ? value.toString() : "0";
    }
    return result;
}

function set_hold(stage, hold) {
    for (let i = 0; i < hold.length; i++) {
        for (let j = 1; j <= 5; j++) {
            let value = document.getElementById(stage + "_die_" + j).value;
            let checked = document.getElementById(stage + "_hold_" + j).checked;
            if (value.toString() == hold[i].toString() && !checked) {
                document.getElementById(stage + "_hold_" + j).checked = true;
                document.getElementById(stage + "_holdspan_" + j).className = "hold-checkbox-correct"
                break;
            }
        }
    }
}

function do_hold(from_state, to_state) {
    for (let i = 1; i <= 5; i++) {
        let value = document.getElementById(from_state + "_die_" + i).value;
        let checked = document.getElementById(from_state + "_hold_" + i).checked;
        if (checked) {
            document.getElementById(to_state + "_die_" + i).value = value;
            document.getElementById(to_state + "_die_" + i).disabled = true;
        }
    }
}

function set_category(category) {
    document.getElementById("category").value = category;
    let text = document.getElementById("category").options[category].text;
    document.getElementById("best_category").innerHTML = "(" + text + ")";
}

/*************************************************************************************************/

function round(value) {
    return Math.round(value*100)/100
}

function on_scoresheet_change() {
    calculate_scoresheet()
    fetch('/api/start_of_turn?categories=' + get_categories() + "&upper_score=" + get_upper_score())
        .then(response => response.json())
        .then(data => {
            let value = round(data.value + get_upper_score() + get_lower_score())
            document.getElementById("start_of_turn_value").innerHTML = value
        })
        .catch((error) => {
            console.error('Error:', error);
        });
}

function on_roll_change(stage, final = false) {
    if (final) force_dice(stage);
    let dice = get_dice(stage)
    if (!dice) return;

    let url = "/api/" + stage + "_roll"
            + "?categories=" + get_categories()
            + "&upper_score=" + get_upper_score()
            + "&dice=" + dice;

    fetch(url)
        .then(response => response.json())
        .then(data => {
            let value = round(data.value + get_upper_score() + get_lower_score());
            document.getElementById(stage + "_roll_value").innerHTML = value;

            if (final) {
                if (stage != "third") {
                    set_hold(stage, data.hold);
                    document.getElementById(stage + "_hold_value").innerHTML = value;
                } else {
                    set_category(data.category)
                    document.getElementById("category_select_value").innerHTML = value;
                }
            }
        })
        .catch((error) => {
            console.error('Error:', error);
        });
}

function on_hold_change(stage) {
    let url = "/api/" + stage + "_hold"
            + "?categories=" + get_categories()
            + "&upper_score=" + get_upper_score()
            + "&hold=" + get_hold(stage);

    fetch(url)
        .then(response => response.json())
        .then(data => {
            let value = round(data.value + get_upper_score() + get_lower_score());
            document.getElementById(stage + "_hold_value").innerHTML = value;
        })
        .catch((error) => {
            console.error('Error:', error);
        });
}

var last_category_score = 0
function on_category_change() {
    category = document.getElementById("category").value;
    let url = "/api/category_select"
            + "?categories=" + get_categories()
            + "&upper_score=" + get_upper_score()
            + "&dice=" + get_dice("third")
            + "&category=" + category;

    fetch(url)
        .then(response => response.json())
        .then(data => {
            let value = round(data.value + get_upper_score() + get_lower_score());
            document.getElementById("category_select_value").innerHTML = value;

            let text = document.getElementById("category").options[category].text;
            document.getElementById("end_of_turn_text").innerHTML = 
                "Accept change '" + text + "' = " + data.score;
            last_category_score = data.score;
        })
        .catch((error) => {
            console.error('Error:', error);
        });
}

/*************************************************************************************************/

function apply_start_of_turn() {
    on_scoresheet_change();
    set_scoresheet_disabled(true);
    set_visible_stage(1);
}

function apply_first_roll() {
    force_dice("first");
    on_roll_change("first", true);
    set_dice_disabled("first", true);
    set_visible_stage(2);
}

function apply_first_hold() {
    on_hold_change("first");
    do_hold("first", "second");
    set_hold_disabled("first", true);
    set_visible_stage(3);

    if (!get_hold("first").includes("0")) {
        apply_second_roll();
        set_hold("second", get_hold("first"));
        apply_second_hold();
    }
}

function apply_second_roll() {
    force_dice("second");
    on_roll_change("second", true);
    set_dice_disabled("second", true);
    set_visible_stage(4)
}

function apply_second_hold() {
    on_hold_change("second");
    do_hold("second", "third")
    set_hold_disabled("second", true);
    set_visible_stage(5)

    if (!get_hold("first").includes("0")) {
        apply_third_roll();
    }
}

function apply_third_roll() {
    force_dice("third");
    on_roll_change("third", true);
    set_dice_disabled("third", true);
    set_visible_stage(6)
}

function apply_category_select() {
    on_category_change();
    set_category_disabled(true);
    set_visible_stage(7)
}

function apply_end_of_turn() {
    let category = document.getElementById("category").value;
    update_category_score(category, last_category_score);
    on_scoresheet_change();
    reset_stages()
}

/*************************************************************************************************/

document.addEventListener("DOMContentLoaded", function() {
    reset_stages();
});