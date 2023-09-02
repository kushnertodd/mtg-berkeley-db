// after DOM loaded
//let dialog_mode;


document.addEventListener("DOMContentLoaded", function () {
    let a = 1;
});

$(document).ready(function () {
    Deck_table.init();
    Card_table.init();
    Card_description_table.init();
    user_list_create();
    $("#send_request_button").prop("disabled", true);

});


function deck_cards_select_request_failure(req) {
    alert(`select user request failed: ${req}`);
}

function deck_cards_select_request_success(result) {
    const data = JSON.stringify(result);
    let result_obj = JSON.parse(data);
    console.log(`class name: {result_obj.class_name}`);
    console.log(data);
    let request_name = result_obj.mtg_request.request;
    display_response(request_name, result_obj);
}

function user_decks_select_request_failure(req) {
    alert(`select user request failed: '${req}'`);
}

function user_decks_select_request_success(result) {
    const data = JSON.stringify(result);
    let result_obj = JSON.parse(data);
    console.log(`class name: {result_obj.class_name}`);
    console.log(data);
    let request_name = result_obj.mtg_request.request;
    Deck_table.create(result_obj.mtg_request_response.deck_dto_list);
}

function user_list_create() {
    let request = new Request({
        request: "account_select_all",
        args: null,

    });
    request.send(user_select_request_success,user_select_request_failure);
}

function user_list_populate(account_dto_list) {
    let select_request_list = document.getElementById("mtg_select_user");
    select_request_list.addEventListener('change', user_list_selected);
    Deck_table.clear();
    Card_table.clear();
    Card_description_table.clear();
    for (let i = 0; i < account_dto_list.length; i++) {
        let account = account_dto_list[i];
        let option = document.createElement("option");
        option.textContent = account.username;
        option.username = account.username;
        option.value = account.account_id;
        select_request_list.appendChild(option);
    }
}


function user_list_selected() {
    let user_select_el = $("#mtg_select_user")[0];
    let opt = user_select_el.value;
    let request = new Request({
        request: "deck_select_all_for_account_id",
        arguments: opt
    });
    request.send(user_decks_select_request_success, user_decks_select_request_failure);
}

function user_select_request() {
    let user_request = $("#mtg_select_user")[0].value;
    let argument_el = $('#argument_input')[0];
    let argument = argument_el.value;
    let request_descriptor = request_parameters[request];
    if (argument === "" && request_descriptor.needs_argument) {
        argument_el.placeholder = "Request requires value";
        return false;
    }
    let request = new Request({
        request: user_request,
        arguments: arguments,

    });
    request.send(user_select_request_success,user_select_request_failure);
    return false;
}

function user_select_request_failure(req) {
    alert(`select user request failed: '${req}'`);
}

function user_select_request_success(result) {
    const data = JSON.stringify(result);
    let result_obj = JSON.parse(data);
    console.log(`class name: {result_obj.class_name}`);
    console.log(data);
    let request_name = result_obj.mtg_request.request;
    user_list_populate(result_obj.mtg_request_response.account_dto_list);
}

