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
    "card": ["name", "color"],
    "card_description": ["item", "value"],
    "deck": ["name"]
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
    create_mtg_request_list();
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

function create_mtg_request(request, arguments) {
    if (Array.isArray(arguments)) {
        return '{"class_name":"Mtg_request",' + '"request":"' + request + '",'
            + '"arguments":["' + arguments.join('","') + '"]}';
    } else {
        if (request === "select_title" || request === "select_name")
            return '{"class_name":"Mtg_request",' + '"request":"' + request + '",'
                + '"arguments":["' + arguments + '", "15", "8"]}';
        //+ '"arguments":["' + arguments + '", "0", "20"]}';
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

function create_mtg_request(request, arguments) {
    if (Array.isArray(arguments)) {
        return '{"class_name":"Mtg_request",' + '"request":"' + request + '",'
            + '"arguments":["' + arguments.join('","') + '"]}';
    } else {
        if (request === "select_title" || request === "select_name")
            return '{"class_name":"Mtg_request",' + '"request":"' + request + '",'
                + '"arguments":["' + arguments + '", "15", "8"]}';
        //+ '"arguments":["' + arguments + '", "0", "20"]}';
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
}

function card_table_row_onlick_handler(e) {
    e.preventDefault();
    let card_row_selected = e.target;
    let data = card_row_selected.row.data;
    let card_id = data.card_id;
    Table.select_row(card_row_selected);
}

function card_table_row_contextmenu_onlick_handler(e) {
    e.preventDefault();
    let card_row_selected = e.target;
    let data = card_row_selected.row.data;
    let card_id = data.card_id;
    Table.select_row(card_row_selected);
    card_set_name(data.name);
    card_description_table_create(data);
}

function deck_table_row_onlick_handler(e) {
    e.preventDefault();
    let deck_row_selected = e.target;
    let data = deck_row_selected.row.data;
    let deck_id = data.deck_id;
    Table.select_row(deck_row_selected);
}

function deck_table_row_contextmenu_onlick_handler(e) {
    e.preventDefault();
    let deck_row_selected = e.target;
    let data = deck_row_selected.row.data;
    let deck_id = data.deck_id;
    Table.select_row(deck_row_selected);
    deck_set_name(data.name);
    let payload = create_mtg_request("deck_select_all_cards", deck_id);
    send_mtg_request(payload, select_deck_cards_request_success, select_deck_cards_request_failure);
}

function deck_set_name(deck_name) {
    $(displayed_deck).html("Deck: " + deck_name);
}

function deck_unset_name() {
    $(displayed_deck).html("");
}

function card_set_name(card_name) {
    $(displayed_card).html("Card: " + card_name);
}

function card_unset_name() {
    $(displayed_card).html("");
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
    let name_item_td = card_description_table.add_td({row_id: "r1", id: "card-name-item", text: card.name});
    let name_input_td = card_description_table.add_td({row_id: "r1", id: "card-name-input", text: ""});
    let name_input = document.createElement("input");
    name_input.value = card.name;
    name_input_td.id = "card-name-input";
    name_input_td.appendChild(name_input);
    let type_id_tr = card_description_table.add_row({data: card.name, id: "r2"})
    let type_id_item_td = card_description_table.add_td({row_id: "r2", id: "card-color-item", text: card.type_id})
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
    $("#card-description-table-save")[0].addEventListener('click', card_description_table_save);
    $("#card-description-table-cancel")[0].addEventListener('click', card_description_table_cancel);
}

function card_description_table_show(card) {
    $("#card-description-table-buttons")[0].style.display = 'block';
    $("#card-description-table-save")[0].data = card;
    $("#card-description-table-cancel")[0].data = card;
}

function card_description_table_save(e) {
    alert("save!");
    let card_description_table = Table.get({name: "card_description_table"})
    card_description_table.clear();
    card_description_table_hide();
    card_unset_name();
}

function card_description_table_cancel(e) {
    alert("cancel!");
    let card_description_table = Table.get({name: "card_description_table"})
    card_description_table.clear();
    card_description_table_hide();
    card_unset_name();
}

function create_mtg_request_list() {
    let payload = create_mtg_request("account_select_all", "");
    send_mtg_request(payload, select_user_request_success, select_user_request_failure);
    return false;
}

function populate_mtg_request_list(response) {
    let select_request_list = document.getElementById("mtg_select_user");
    let deck_table = Table.get({name: "deck_table"})
    deck_table.clear();
    let card_table = Table.get({name: "card_table"})
    card_table.clear();
    for (let i = 0; i < response.account_dto_list.length; i++) {
        let account = response.account_dto_list[i];
        let el = document.createElement("option");
        el.textContent = account.username;
        el.value = account.account_id;
        select_request_list.appendChild(el);
    }
}

function display_response(request_name, result_obj) {
    switch (request_name) {
        case 'account_select_all':
            populate_mtg_request_list(result_obj.mtg_request_response);
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

// cache movie table body DOM object
let user_selection_list = undefined;

// get movie table body DOM object
function get_mtg_select_user_DOM_object() {
    if (user_selection_list === undefined) user_selection_list =
        document.getElementById("mtg_select_user");
    return user_selection_list;
}

/*
// cache movie table div DOM object
let mtg_table_div = undefined;

// get movie table div DOM object
function get_mtg_table_div_DOM_object() {
    if (mtg_table_div === undefined) {
        mtg_table_div = new Table({name: "mtg_table", div_id: "mtg_table", id: "mtg_table"});
    }
    return mtg_table_div;
}

function mtg_table_append_actor(response) {
    let tbody = get_mtg_table_tbody_DOM_object();
    // add new row
    let tr = document.createElement('TR');
    tbody.appendChild(tr);

    // add id
    let id = response.name_id;
    mtg_actor_table_cell_add_text(tr, "id", id);

    // add new movie name
    let name = response.primaryName;
    tr.actor = name;
    mtg_actor_table_cell_add_text(tr, "name", name);

    // add born date
    let born = response.birthYear;
    mtg_actor_table_cell_add_text(tr, "born", born);

    // add new died date
    let died = response.deathYear;
    mtg_actor_table_cell_add_text(tr, "died", died);

    // add new movie user rating vote count
    let user_rating = response.userRating;
    mtg_actor_table_cell_add_text(tr, "user_rating", user_rating);

    // add new movie user rating vote count
    let priority = response.priority;
    mtg_actor_table_cell_add_text(tr, "priority", priority);

}

function mtg_table_append_actors(response) {
    let name_dto_list = response.name_dto_list;
    for (let name_dto = 0;
         name_dto < name_dto_list.length;
         name_dto++) {
        mtg_table_append_actor(name_dto_list[name_dto]);
    }
}

function mtg_table_append_movie(response) {
    let tbody = get_mtg_table_tbody_DOM_object();
    // add new row
    let tr = document.createElement('TR');
    tbody.appendChild(tr);

    // add id
    let id = response.title_id;
    mtg_movie_table_cell_add_text(tr, "id", id);

    // add new movie name
    let title = response.primaryTitle;
    tr.movie = title;
    mtg_movie_table_cell_add_text(tr, "title", title);

    // add actors
    let year = response.startYear;
    mtg_movie_table_cell_add_text(tr, "year", year);

    // add new movie user ratings
    let rating = response.averageRating;
    mtg_movie_table_cell_add_text(tr, "rating", rating);

    // add new movie user rating vote count
    let num_votes =
        response.numVotes.toString()
            .replace(/\B(?=(\d{3})+(?!\d))/g, ',');
    mtg_movie_table_cell_add_text(tr, "num_votes", num_votes);

    // add new movie user rating vote count
    let user_rating = response.userRating;
    mtg_movie_table_cell_add_text(tr, "user_rating", user_rating);

    // add new movie user rating vote count
    let priority = response.priority;
    mtg_movie_table_cell_add_text(tr, "priority", priority);
}

function mtg_table_append_movies(response) {
    let title_dto_list = response.title_dto_list;
    for (let title_dto = 0; title_dto < title_dto_list.length; title_dto++) {
        mtg_table_append_movie(title_dto_list[title_dto]);
    }
}

// add new text cell to movie table row
function mtg_movie_table_cell_add_text(tr, cellName, cellText) {
    let td = document.createElement('TD');
    td.tag = cellName;
    mtg_movie_table_cell_setup_onclick_handler(td);
    tr.appendChild(td);
    //mtg_table_cell_setup_onclick_handler(td);
    td.appendChild(document.createTextNode(cellText));
}

// add new text cell to movie table row
function mtg_actor_table_cell_add_text(tr, cellName, cellText) {
    let td = document.createElement('TD');
    td.tag = cellName;
    mtg_actor_table_cell_setup_onclick_handler(td);
    tr.appendChild(td);
    //mtg_table_cell_setup_onclick_handler(td);
    td.appendChild(document.createTextNode(cellText));
}

// enable clicking movie table row to highlight row
let actor_row_selected;

function mtg_actor_table_cell_setup_onclick_handler(cell) {
    // do something on onclick event for cells
    cell.addEventListener('contextmenu', function (e) {
        e.preventDefault();
        // Get the row id where the cell exists
        actor_row_selected = this.parentNode;
        $("#actor-row-dialog-contextmenu").dialog("open");
    }, false);
}

// enable clicking movie table row to highlight row
let movie_row_selected;

function mtg_movie_table_cell_setup_onclick_handler(cell) {
    // do something on onclick event for cells
    cell.addEventListener('contextmenu', function (e) {
        e.preventDefault();
        // Get the row id where the cell exists
        movie_row_selected = this.parentNode;
        $("#movie-row-dialog-contextmenu").dialog("open");
    }, false);
}

// extract movie name from first cell of movie table row
function mtg_table_title_for_row(tr) {
    let td = mtg_table_td_for_tag(tr, "title");
    return td.textContent;
}

// extract movie name from first cell of movie table row
function mtg_table_actor_name_id_for_row(tr) {
    let td = mtg_table_td_for_tag(tr, "id");
    return td.textContent;
}

// extract movie name from first cell of movie table row
function mtg_table_movie_title_id_for_row(tr) {
    let td = mtg_table_td_for_tag(tr, "id");
    return td.textContent;
}

function mtg_table_replace_actor(actor) {
    // find and replace row
    let name_id = actor.name_id;
    let tr = mtg_table_row_by_actor_name_id(name_id);
    if (tr === undefined) {
        mtg_table_add_actor(actor);
    } else {
        let actor_title_id_td = mtg_table_td_for_tag(tr, "id");
        actor_title_id_td.textContent = actor["name_id"];

        let actor_name_td = mtg_table_td_for_tag(tr, "name");
        actor_name_td.textContent = actor["primaryName"];

        let actor_born_td = mtg_table_td_for_tag(tr, "born");
        actor_born_td.textContent = actor["birthYear"];

        let actor_died_td = mtg_table_td_for_tag(tr, "died");
        actor_died_td.textContent = actor["deathYear"];

        let actor_user_rating_td = mtg_table_td_for_tag(tr, "user_rating");
        actor_user_rating_td.textContent = actor["userRating"];

        let actor_priority_td = mtg_table_td_for_tag(tr, "priority");
        actor_priority_td.textContent = actor["priority"];
    }
}

function mtg_table_replace_movie(movie) {
    // find and replace row
    let title_id = movie.title_id;
    let tr = mtg_table_row_by_movie_title_id(title_id);
    if (tr === undefined) {
        mtg_table_add_movie(movie);
    } else {
        let movie_title_id_td = mtg_table_td_for_tag(tr, "id");
        movie_title_id_td.textContent = movie["title_id"];

        let movie_title_td = mtg_table_td_for_tag(tr, "title");
        movie_title_td.textContent = movie["primaryTitle"];

        let movie_year_td = mtg_table_td_for_tag(tr, "year");
        movie_year_td.textContent = movie["startYear"];

        let movie_rating_td = mtg_table_td_for_tag(tr, "rating");
        movie_rating_td.textContent = movie["averageRating"];

        let movie_num_votes_td = mtg_table_td_for_tag(tr, "num_votes");
        movie_num_votes_td.textContent =
            movie["numVotes"].toString()
                .replace(/\B(?=(\d{3})+(?!\d))/g, ',');

        let movie_user_rating_td = mtg_table_td_for_tag(tr, "user_rating");
        movie_user_rating_td.textContent = movie["userRating"];

        let movie_priority_td = mtg_table_td_for_tag(tr, "priority");
        movie_priority_td.textContent = movie["priority"];
    }
}

// return movie table row associated with actor name
function mtg_table_row_by_actor_name_id(name_id) {
    let rows = mtg_table_rows();
    for (let i = 1; i < rows.length; i++) {
        let row = rows[i];
        let row_name_id = mtg_table_actor_name_id_for_row(row);
        if (row_name_id === name_id) {
            return row;
        }
    }
    return undefined;
}

// return movie table row associated with movie title
function mtg_table_row_by_movie_title_id(title_id) {
    let rows = mtg_table_rows();
    for (let i = 1; i < rows.length; i++) {
        let row = rows[i];
        let row_title_id = mtg_table_movie_title_id_for_row(row);
        if (row_title_id === title_id) {
            return row;
        }
    }
    return undefined;
}

// return all movie table rows
function mtg_table_rows() {
    let tbody = get_mtg_table_tbody_DOM_object();
    return tbody.getElementsByTagName('tr');
}

function mtg_table_td_for_tag(tr, tag) {
    let childNodes = tr.childNodes;
    for (let i = 0; i < childNodes.length; i++) {
        let td = childNodes[i];
        if (td.tag === tag) {
            return td;
        }
    }
    return undefined;
}
*/
function user_selected() {
    let opt = $("#mtg_select_user")[0].value;
    let payload = create_mtg_request("deck_select_all_for_account_id", opt);
    send_mtg_request(payload, select_user_decks_request_success, select_user_decks_request_failure);
    /*
    let request_descriptor = request_parameters[opt];
    if (request_descriptor.needs_argument) {
        let argument_input_el = $("#argument_input");
        argument_input_el.show();
        // this is how to clear an input field and show the placeholder
        argument_input_el.val('');
        argument_input_el[0].placeholder = request_descriptor.placeholder;
    } else {
        $("#argument_input").hide();
    }
    $("#send_request_button").prop("disabled", false);
    */
}

/*
function save_actor_request(dialog) {
    let name_id = $("#change-actor-name-id")[0].textContent;
    let user_rating = $("#change-actor-user-rating")[0].value;
    let priority = $("#change-actor-priority")[0].value;
    let req = create_mtg_request("update_name",
        [name_id, user_rating, priority]);
    send_mtg_request(req, save_actor_request_success, save_actor_request_failure);
}

*/
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

/*
function save_actor_request_failure(req) {
    alert("save request failed: '" + req + "'");
}

function save_actor_request_success(result) {
    const data = JSON.stringify(result);
    let result_obj = JSON.parse(data);
    console.log("class name: " + result_obj.class_name);
    console.log(data);
    let request_name = result_obj.mtg_request.request;
    display_response(request_name, result_obj);
}

function save_movie_request(dialog) {
    let title_id = $("#change-movie-title-id")[0].textContent;
    let user_rating = $("#change-movie-user-rating")[0].value;
    let priority = $("#change-movie-priority")[0].value;
    let req = create_mtg_request("update_title",
        [title_id, user_rating, priority]);
    send_mtg_request(req, save_movie_request_success, save_movie_request_failure);
}


function save_movie_request_failure(req) {
    alert("save request failed: '" + req + "'");
}

function save_movie_request_success(result) {
    const data = JSON.stringify(result);
    let result_obj = JSON.parse(data);
    console.log("class name: " + result_obj.class_name);
    console.log(data);
    let request_name = result_obj.mtg_request.request;
    display_response(request_name, result_obj);
}

function search_actor_element() {
    let name_id_td = mtg_table_td_for_tag(actor_row_selected, "id");
    let name_id_text = name_id_td.textContent;
    let payload = create_mtg_request("search_title", name_id_text);
    send_mtg_request(payload, select_user_request_success, select_user_request_failure);
    return false;
}

function search_movie_element() {
    let title_id_td = mtg_table_td_for_tag(movie_row_selected, "id");
    let title_id_text = title_id_td.textContent;
    let payload = create_mtg_request("search_name", title_id_text);
    send_mtg_request(payload, select_user_request_success, select_user_request_failure);
    return false;
}

function select_actor_elements() {
    let name_id_td = mtg_table_td_for_tag(actor_row_selected, "id");
    let name_td = mtg_table_td_for_tag(actor_row_selected, "name");
    let born_td = mtg_table_td_for_tag(actor_row_selected, "born");
    let died_td = mtg_table_td_for_tag(actor_row_selected, "died");
    let user_rating_td = mtg_table_td_for_tag(actor_row_selected, "user_rating");
    let priority_td = mtg_table_td_for_tag(actor_row_selected, "priority");

    let name_id_text = name_id_td.textContent;
    let name_text = name_td.textContent;
    let born_text = born_td.textContent;
    let died_text = died_td.textContent;
    let user_rating_text = user_rating_td.textContent;
    let priority_text = priority_td.textContent;

    let name_id_el = $("#change-actor-name-id");
    name_id_el[0].innerHTML = name_id_text;
    let name_el = $("#change-actor-name");
    name_el[0].innerHTML = name_text;
    let born_el = $("#change-actor-born");
    born_el[0].innerHTML = born_text;
    let died_el = $("#change-actor-died");
    died_el[0].innerHTML = died_text;
    let user_rating_el = $("#change-actor-user-rating");
    user_rating_el[0].value = user_rating_text;
    let priority_el = $("#change-actor-priority");
    priority_el[0].value = priority_text;
}

function select_movie_elements() {
    let title_id_td = mtg_table_td_for_tag(movie_row_selected, "id");
    let title_td = mtg_table_td_for_tag(movie_row_selected, "title");
    let year_td = mtg_table_td_for_tag(movie_row_selected, "year");
    let rating_td = mtg_table_td_for_tag(movie_row_selected, "rating");
    let votes_td = mtg_table_td_for_tag(movie_row_selected, "num_votes");
    let user_rating_td = mtg_table_td_for_tag(movie_row_selected, "user_rating");
    let priority_td = mtg_table_td_for_tag(movie_row_selected, "priority");

    let title_id_text = title_id_td.textContent;
    let title_text = title_td.textContent;
    let year_text = year_td.textContent;
    let rating_text = rating_td.textContent;
    let votes_text = votes_td.textContent;
    let user_rating_text = user_rating_td.textContent;
    let priority_text = priority_td.textContent;

    let title_id_el = $("#change-movie-title-id");
    title_id_el[0].innerHTML = title_id_text;
    let title_el = $("#change-movie-title");
    title_el[0].innerHTML = title_text;
    let year_el = $("#change-movie-year");
    year_el[0].innerHTML = year_text;
    let rating_el = $("#change-movie-rating");
    rating_el[0].innerHTML = rating_text;
    let votes_el = $("#change-movie-votes");
    votes_el[0].innerHTML = votes_text;
    let user_rating_el = $("#change-movie-user-rating");
    user_rating_el[0].value = user_rating_text;
    let priority_el = $("#change-movie-priority");
    priority_el[0].value = priority_text;
}
*/
// run mtg_request
// success: return data
// fail: return null
function send_mtg_request(req, success_callback, failure_callback) {
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
    send_mtg_request(payload, select_user_request_success, select_user_request_failure);
    return false;
}

/*
function select_user_request_failure(req) {
    alert("request failed: '" + req + "'");
}

function select_user_request_success(result) {
    const data = JSON.stringify(result);
    let result_obj = JSON.parse(data);
    console.log("class name: " + result_obj.class_name);
    console.log(data);
    let request_name = result_obj.mtg_request.request;
    display_response(request_name, result_obj);
}
*/
