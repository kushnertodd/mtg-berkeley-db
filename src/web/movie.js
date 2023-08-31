/*
  let request_names = [
    "deck_select_all_for_account_id",
    "lookup_ratings",
    "lookup_title",
    "search_name",
    "search_ratings",
    "search_title",
    "select_name",
    "select_ratings",
    "select_title"
  ];
*/
let request_parameters = {
    account_select_all: {"needs_argument": false},
    deck_select_all_for_account_id: {"needs_argument": true},
    deck_select_all_cards: {"needs_argument": true},
};

let table_headers = {
    "card": ["Card", "Color"],
    "card_description": ["Item", "Value"],
    "deck": ["Deck"]
}

// after DOM loaded
let dialog_mode;

document.addEventListener("DOMContentLoaded", function () {
    let a = 1;
});

let card_table = undefined;
let deck_table = undefined;

$(document).ready(function () {
    let card_table = new Table({name: "card_table", div_id: "card_table", id: "card_table"});
    let card_description_table = new Table({
        name: "card_description_table",
        div_id: "card_description_table",
        id: "card_description_table"
    });
    let deck_table = new Table({name: "deck_table", div_id: "deck_table", id: "deck_table"});
    user_list_create();
    $("#send_request_button").prop("disabled", true);
    card_description_table_init();
    /*
        // change movie dialog

        $("#movie-row-dialog-contextmenu").dialog({
            autoOpen: false, height: 90, width: 380, modal: true, closeOnEscape: true,
        });

        let change_movie_dialog_form = $("#change-movie-dialog-form");
        change_movie_dialog_form.dialog({
            autoOpen: false, height: 600, width: 350, modal: true, buttons: {
                "Save": function () {
                    save_movie_request(this);
                    $("#change-movie-dialog-form").dialog("close");
                }, Cancel: function () {
                    $("#change-movie-dialog-form").dialog("close");
                }
            }, close: function () {
                change_movie_form[0].reset();
            }
        });
        let change_movie_form = change_movie_dialog_form.find("form")
            .on("submit", function (event) {
                event.preventDefault();
            });

        $("#contextmenu-change-movie").button().on("click", function () {
            dialog_mode = "change";
            let opt = {
                autoOpen: false, height: 550, width: 380, modal: true, closeOnEscape: true,
            };
            $("#change-movie-dialog-form").dialog(opt).dialog("open");
            $("#movie-row-dialog-contextmenu").dialog("close");
            select_movie_elements();
        });
        $("#contextmenu-search-movie").button().on("click", function () {
            dialog_mode = "search";
            $("#change-movie-dialog-form").dialog("close");
            $("#movie-row-dialog-contextmenu").dialog("close");
            search_movie_element();
        });

        // change actor dialog

        $("#actor-row-dialog-contextmenu").dialog({
            autoOpen: false, height: 90, width: 380, modal: true, closeOnEscape: true,
        });

        let change_actor_dialog_form = $("#change-actor-dialog-form");
        change_actor_dialog_form.dialog({
            autoOpen: false, height: 600, width: 350, modal: true, buttons: {
                "Save": function () {
                    save_actor_request(this);
                    $("#change-actor-dialog-form").dialog("close");
                }, Cancel: function () {
                    $("#change-actor-dialog-form").dialog("close");
                }
            }, close: function () {
                change_actor_form[0].reset();
            }
        });
        let change_actor_form = change_actor_dialog_form.find("form")
            .on("submit", function (event) {
                event.preventDefault();
            });

        $("#contextmenu-change-actor").button().on("click", function () {
            dialog_mode = "change";
            let opt = {
                autoOpen: false, height: 550, width: 380, modal: true, closeOnEscape: true,
            };
            $("#change-actor-dialog-form").dialog(opt).dialog("open");
            $("#actor-row-dialog-contextmenu").dialog("close");
            select_actor_elements();
        });
        $("#contextmenu-search-actor").button().on("click", function () {
            dialog_mode = "search";
            $("#change-actor-dialog-form").dialog("close");
            $("#actor-row-dialog-contextmenu").dialog("close");
            search_actor_element();
        });
        */
});

function card_description_table_button_cancel(e) {
    alert("cancel!");
    let card_description_table = Table.get({name: "card_description_table"})
    card_description_table.clear();
    card_description_table_hide();
    card_name_unset();
}

function card_description_table_button_save(e) {
    alert("save!");
    let card_description_table = Table.get({name: "card_description_table"});
    card_description_table.clear();
    card_description_table_hide();
    card_name_unset();
}

function card_description_table_create(card) {
    let headers = table_headers["card_description"];
    let card_description_table = Table.get({name: "card_description_table"})
    card_description_table.clear();
    card_description_table.add_row({id: "r0"})
    // might be the right way to set columns widths:
    // https://stackoverflow.com/questions/928849/setting-table-column-width
    let cellWidths = ['50%', '50%'];
    for (let i = 0; i < headers.length; i++)
        card_description_table.add_th({
            row_id: "r0",
            id: "h" + i,
            text: headers[i],
            class_name: " header",
            width: cellWidths[i]
        });
    let name_tr = card_description_table.add_row({data: card.name, id: "r1"});
    let name_item_td = card_description_table.add_td({row_id: "r1", id: "card-name-item", text: "name"});
    let name_input_td = card_description_table.add_td({row_id: "r1", id: "card-name-input", text: ""});
    let name_input = document.createElement("input");
    name_input.value = card.name;
    name_input_td.id = "card-name-input";
    name_input_td.appendChild(name_input);
    let type_id_tr = card_description_table.add_row({data: card.name, id: "r2"})
    let type_id_item_td = card_description_table.add_td({row_id: "r2", id: "card-color-item", text: "color"})
    let type_id_input_td = card_description_table.add_td({row_id: "r2", id: "card-color-input", text: ""});
    let type_id_input = document.createElement("input");
    type_id_input.value = card.type_id;
    type_id_input_td.id = "card-type-id-input";
    type_id_input_td.defaultValue = card.type_id;
    type_id_input_td.appendChild(type_id_input);
    card_description_table_show(card);
}

function card_description_table_hide() {
    $("#card-description-table-buttons")[0].style.display = 'none';
}

function card_description_table_init() {
    card_description_table_hide();
    $("#card-description-table-save")[0].addEventListener('click', card_description_table_button_save);
    $("#card-description-table-cancel")[0].addEventListener('click', card_description_table_button_cancel);
}

function card_description_table_show(card) {
    $("#card-description-table-buttons")[0].style.display = 'block';
    $("#card-description-table-save")[0].data = card;
    $("#card-description-table-cancel")[0].data = card;
}

function card_name_set(card_name) {
    $(displayed_card).html("Card: " + card_name);
}

function card_name_unset() {
    $(displayed_card).html("");
}

function card_table_clear() {
    let card_table = Table.get({name: "card_table"})
    card_table.clear();
}

function card_table_create(response) {
    let headers = table_headers["card"];
    let card_table = Table.get({name: "card_table"})
    card_table.clear();
    card_table.add_row({id: "r0"})
    // might be the right way to set columns widths:
    // https://stackoverflow.com/questions/928849/setting-table-column-width
    let cellWidths = ['50%', '50%'];
    for (let i = 0; i < headers.length; i++)
        card_table.add_th({row_id: "r0", id: "h" + i, text: headers[i], class_name: " header", width: cellWidths[i]});
    let cards = response.card_dto_list;
    for (let i = 0; i < cards.length; i++) {
        row_id = "r" + i;
        let tr = card_table.add_row({data: cards[i], id: row_id})
        tr.addEventListener('onclick', card_table_row_onlick_handler);
        tr.addEventListener('contextmenu', card_table_row_contextmenu_onlick_handler);
        card_table.add_td({row_id: row_id, id: "cards-name", text: cards[i].name})
        card_table.add_td({row_id: row_id, id: "cards-color", text: cards[i].type_id})
    }
}

function card_table_row_contextmenu_onlick_handler(e) {
    e.preventDefault();
    let card_row_selected = e.currentTarget;
    let data = card_row_selected.data;
    let card_id = data.card_id;
    Table.select_row(card_row_selected);
    card_name_set(data.name);
    card_description_table_create(data);
}

function create_mtg_request(request, arguments) {
    if (Array.isArray(arguments)) {
        return '{"class_name":"Mtg_request",' + '"request":"' + request + '",'
            + '"arguments":["' + arguments.join('","') + '"]}';
    } else {
        if (request === "select_title" || request === "select_name")
            return '{"class_name":"Mtg_request",' + '"request":"' + request + '",'
                + '"arguments":["' + arguments + '", "15", "8"]}';
        else {
            let request_descriptor = request_parameters[request];
            if (request_descriptor.needs_argument) {
                return '{"class_name":"Mtg_request",' + '"request":"' + request + '",'
                    + '"arguments":["' + arguments + '"]}';
            } else {
                return '{"class_name":"Mtg_request",' + '"request":"' + request + '",'
                    + '"arguments":[]}';
            }
        }
    }
}

function deck_table_clear() {
    let deck_table = Table.get({name: "deck_table"})
    deck_table.clear();
}

// https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model/Traversing_an_HTML_table_with_JavaScript_and_DOM_Interfaces
// https://stackoverflow.com/questions/14643617/create-table-using-javascript
function deck_table_create(response) {
    let headers = table_headers["deck"];
    let deck_table = Table.get({name: "deck_table"})
    deck_table.clear();
    let card_table = Table.get({name: "card_table"})
    card_table.clear();
    deck_table.add_row({id: "r0"})
    // might be the right way to set columns widths:
    // https://stackoverflow.com/questions/928849/setting-table-column-width
    let cellWidths = ['50%', '50%'];
    for (let i = 0; i < headers.length; i++)
        deck_table.add_th({row_id: "r0", id: "h" + i, text: headers[i], class_name: " header", width: cellWidths[i]});
    let decks = response.deck_dto_list;
    for (let i = 0; i < decks.length; i++) {
        row_id = "r" + i;
        let tr = deck_table.add_row({data: decks[i], id: row_id})
        tr.addEventListener('click', deck_table_row_onlick_handler);
        tr.addEventListener('contextmenu', deck_table_row_contextmenu_onlick_handler);
        deck_table.add_td({row_id: row_id, id: "d0", text: decks[i].name})
    }
    user_name_set();
}

function card_table_row_onlick_handler(e) {
    e.preventDefault();
    let card_row_selected = e.currentTarget;
    let data = card_row_selected.row.data;
    let card_id = data.card_id;
    Table.select_row(card_row_selected);
}


function deck_name_set(deck_name) {
    $(displayed_deck).html("Deck: " + deck_name);
}

function deck_name_unset() {
    $(displayed_deck).html("");
}

function deck_table_row_contextmenu_onlick_handler(e) {
    e.preventDefault();
    let deck_row_selected = e.currentTarget;
    let data = deck_row_selected.data;
    let deck_id = data.deck_id;
    Table.select_row(deck_row_selected);
    deck_name_set(data.name);
    let payload = create_mtg_request("deck_select_all_cards", deck_id);
    mtg_request_send(payload, select_deck_cards_request_success, select_deck_cards_request_failure);
}

function deck_table_row_onlick_handler(e) {
    e.preventDefault();
    let deck_row_selected = e.currentTarget;
    let data = deck_row_selected.row.data;
    let deck_id = data.deck_id;
    Table.select_row(deck_row_selected);
}

function display_response(request_name, result_obj) {
    switch (request_name) {
        case 'account_select_all':
            user_list_populate(result_obj.mtg_request_response);
            break;
        case 'deck_select_all_for_account_id':
            deck_table_create(result_obj.mtg_request_response);
            break;
        case 'deck_select_all_cards':
            card_table_create(result_obj.mtg_request_response);
            break;
        default:
            console.log("unexpected result request: " + request_name);
    }
}

// run mtg_request
// success: return data
// fail: return null
function mtg_request_send(req, success_callback, failure_callback) {
    try {
        $.ajax({
            //url: "http://34.106.93.238:8000/mtg-request-dto",
            url: "http://localhost:8000/mtg-request-dto",
            type: "POST",
            data: req,
            success: function (result) {
                success_callback(result);
            }, failure: function (result) {
                failure_callback(result);
            }
        });
    } catch (error) {
        console.error(error);
    }
}


function select_deck_cards_request_failure(req) {
    alert("select user request failed: '" + req + "'");
}

function select_deck_cards_request_success(result) {
    const data = JSON.stringify(result);
    let result_obj = JSON.parse(data);
    console.log("class name: " + result_obj.class_name);
    console.log(data);
    let request_name = result_obj.mtg_request.request;
    display_response(request_name, result_obj);
}

function select_user_request_failure(req) {
    alert("select user request failed: '" + req + "'");
}

function select_user_request_success(result) {
    const data = JSON.stringify(result);
    let result_obj = JSON.parse(data);
    console.log("class name: " + result_obj.class_name);
    console.log(data);
    let request_name = result_obj.mtg_request.request;
    display_response(request_name, result_obj);
}

function select_user_decks_request_failure(req) {
    alert("select user request failed: '" + req + "'");
}

function select_user_decks_request_success(result) {
    const data = JSON.stringify(result);
    let result_obj = JSON.parse(data);
    console.log("class name: " + result_obj.class_name);
    console.log(data);
    let request_name = result_obj.mtg_request.request;
    display_response(request_name, result_obj);
}

function select_user_request() {
    let request = $("#mtg_select_user")[0].value;
    let argument_el = $('#argument_input')[0];
    let argument = argument_el.value;
    let request_descriptor = request_parameters[request];
    if (argument === "" && request_descriptor.needs_argument) {
        argument_el.placeholder = "Request requires value";
        return false;
    }
    let payload = create_mtg_request(request, argument);
    mtg_request_send(payload, select_user_request_success, select_user_request_failure);
    return false;
}

function user_list_create() {
    let payload = create_mtg_request("account_select_all", "");
    mtg_request_send(payload, select_user_request_success, select_user_request_failure);
    return false;
}

function user_list_populate(response) {
    let select_request_list = document.getElementById("mtg_select_user");
    deck_table_clear();
    card_table_clear();
    for (let i = 0; i < response.account_dto_list.length; i++) {
        let account = response.account_dto_list[i];
        let el = document.createElement("option");
        el.textContent = account.username;
        el.username = account.username;
        el.value = account.account_id;
        select_request_list.appendChild(el);
    }
}


function user_list_selected() {
    let select_user_el = $("#mtg_select_user")[0];
    let opt = select_user_el.value;
    let payload = create_mtg_request("deck_select_all_for_account_id", opt);
    mtg_request_send(payload, select_user_decks_request_success, select_user_decks_request_failure);
}

function user_name_set() {
    $(displayed_user).html("Decks:");
}

function user_name_unset() {
    $(displayed_user).html("");
}

