/*
  let request_names = [
    "lookup_name",
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
    lookup_name: {"label": "Lookup actor", "needs_argument": true, "placeholder": "Enter actor id"},
    lookup_title: {"label": "Lookup movie", "needs_argument": true, "placeholder": "Enter movie id"},
    search_title: {"label": "Search for actor movies", "needs_argument": true, "placeholder": "Enter actor id"},
    search_name: {"label": "Search for movie actors", "needs_argument": true, "placeholder": "Enter movie id"},
    select_name: {"label": "Select all actor names", "needs_argument": true, "placeholder": "Enter actor name"},
    select_title: {"label": "Select all movie titles", "needs_argument": true, "placeholder": "Enter movie name"}
};

let table_headers = {
    "name": ["id", "name", "born", "died", "user rating", "priority"],
    "title": ["id", "title", "year", "rating", "votes", "user rating", "priority"]
}

// after DOM loaded
let dialog_mode;

$(document).ready(function () {
    $("#argument_input").hide();
    create_imdb_request_list();
    $("#send_request_button").prop("disabled", true);

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
});

function clear_table(tbody) {
    if (tbody.children.length > 0) {
        let rows = tbody.getElementsByTagName('tr');
        for (let row = rows.length - 1; row >= 0; row--) {
            tbody.removeChild(rows[row]);
        }
    }
}

function create_imdb_request(request, arguments) {
    if (Array.isArray(arguments)) {
        return '{"class_name":"Imdb_request",' + '"request":"' + request + '",'
            + '"arguments":["' + arguments.join('","') + '"]}';
    } else {
        if (request === "select_title" || request === "select_name")
            return '{"class_name":"Imdb_request",' + '"request":"' + request + '",'
                + '"arguments":["' + arguments + '", "15", "8"]}';
        //+ '"arguments":["' + arguments + '", "0", "20"]}';
        else {
            let request_descriptor = request_parameters[request];
            if (request_descriptor.needs_argument) {
                return '{"class_name":"Imdb_request",' + '"request":"' + request + '",'
                    + '"arguments":["' + arguments + '"]}';
            } else {
                return '{"class_name":"Imdb_request",' + '"request":"' + request + '",'
                    + '"arguments":[]}';
            }
        }
    }
}

// https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model/Traversing_an_HTML_table_with_JavaScript_and_DOM_Interfaces
// https://stackoverflow.com/questions/14643617/create-table-using-javascript
function create_imdb_table(table_type) {
    <!-- list of header column names -->
    let headers = table_headers[table_type];
    // disable context menu on right click
    let tbody = get_imdb_table_tbody_DOM_object();
    clear_table(tbody);
    let tr = document.createElement('TR');
    tbody.appendChild(tr);
    // might be the right way to set columns widths:
    // https://stackoverflow.com/questions/928849/setting-table-column-width
    let cellWidths = ['30%', '15%', '40%', '15%'];
    for (let i = 0; i < headers.length; i++) {
        let th = document.createElement('TH');
        tr.appendChild(th);
        th.className += " header";
        th.width = cellWidths[i];
        th.appendChild(document.createTextNode(headers[i]));
    }
}

function create_imdb_request_list() {
    let select_request_list = document.getElementById("imdb_select_request");
    for (const opt in request_parameters) {
        let el = document.createElement("option");
        let request_descriptor = request_parameters[opt];
        el.textContent = request_descriptor.label;
        el.value = opt;
        select_request_list.appendChild(el);
    }
}

function display_response(request_name, result_obj) {
    switch (request_name) {
        case 'lookup_name':
            create_imdb_table("name");
            imdb_table_append_actor(result_obj.imdb_request_response);
            break;
        case 'lookup_title':
            create_imdb_table("title");
            imdb_table_append_movie(result_obj.imdb_request_response);
            break;
        case 'search_name':
            create_imdb_table("name");
            imdb_table_append_actors(result_obj.imdb_request_response);
            break;
        case 'search_title':
            create_imdb_table("title");
            imdb_table_append_movies(result_obj.imdb_request_response);
            break;
        case 'select_name':
            create_imdb_table("name");
            imdb_table_append_actors(result_obj.imdb_request_response);
            break;
        case 'select_title':
            create_imdb_table("title");
            imdb_table_append_movies(result_obj.imdb_request_response);
            break;
        case 'update_name':
            imdb_table_replace_actor(result_obj.imdb_request_response);
            break;
        case 'update_title':
            imdb_table_replace_movie(result_obj.imdb_request_response);
            break;
        default:
            console.log("unexpected result request: " + request_name);
    }
}

// cache movie table body DOM object
let movie_selection_list = undefined;

// get movie table body DOM object
function get_imdb_select_request_DOM_object() {
    if (movie_selection_list === undefined) movie_selection_list =
        document.getElementById("imdb_select_request");
    return movie_selection_list;
}

// cache movie table div DOM object
let imdb_table_div = undefined;

// get movie table div DOM object
function get_imdb_table_div_DOM_object() {
    if (imdb_table_div === undefined) {
        imdb_table_div = document.getElementById("imdb_table");
    }
    return imdb_table_div;
}

// cache movie table DOM object
let imdb_table = undefined;

// get movie table DOM object
function get_imdb_table_DOM_object() {
    if (imdb_table === undefined) {
        let imdb_table_div = get_imdb_table_div_DOM_object();
        imdb_table = document.createElement('TABLE');
        imdb_table_div.appendChild(imdb_table);
        imdb_table.id = "imdb_table";
    }
    return imdb_table;
}

// cache movie table DOM object
let imdb_table_tbody = undefined;

// get movie table DOM object
function get_imdb_table_tbody_DOM_object() {
    if (imdb_table_tbody === undefined) {
        let table = get_imdb_table_DOM_object();
        imdb_table_tbody = document.createElement('TBODY');
        table.appendChild(imdb_table_tbody);
    }
    return imdb_table_tbody;
}

function imdb_table_append_actor(response) {
    let tbody = get_imdb_table_tbody_DOM_object();
    // add new row
    let tr = document.createElement('TR');
    tbody.appendChild(tr);

    // add id
    let id = response.name_id;
    imdb_actor_table_cell_add_text(tr, "id", id);

    // add new movie name
    let name = response.primaryName;
    tr.actor = name;
    imdb_actor_table_cell_add_text(tr, "name", name);

    // add born date
    let born = response.birthYear;
    imdb_actor_table_cell_add_text(tr, "born", born);

    // add new died date
    let died = response.deathYear;
    imdb_actor_table_cell_add_text(tr, "died", died);

    // add new movie user rating vote count
    let user_rating = response.userRating;
    imdb_actor_table_cell_add_text(tr, "user_rating", user_rating);

    // add new movie user rating vote count
    let priority = response.priority;
    imdb_actor_table_cell_add_text(tr, "priority", priority);

}

function imdb_table_append_actors(response) {
    let name_dto_list = response.name_dto_list;
    for (let name_dto = 0;
         name_dto < name_dto_list.length;
         name_dto++) {
        imdb_table_append_actor(name_dto_list[name_dto]);
    }
}

function imdb_table_append_movie(response) {
    let tbody = get_imdb_table_tbody_DOM_object();
    // add new row
    let tr = document.createElement('TR');
    tbody.appendChild(tr);

    // add id
    let id = response.title_id;
    imdb_movie_table_cell_add_text(tr, "id", id);

    // add new movie name
    let title = response.primaryTitle;
    tr.movie = title;
    imdb_movie_table_cell_add_text(tr, "title", title);

    // add actors
    let year = response.startYear;
    imdb_movie_table_cell_add_text(tr, "year", year);

    // add new movie user ratings
    let rating = response.averageRating;
    imdb_movie_table_cell_add_text(tr, "rating", rating);

    // add new movie user rating vote count
    let num_votes =
        response.numVotes.toString()
            .replace(/\B(?=(\d{3})+(?!\d))/g, ',');
    imdb_movie_table_cell_add_text(tr, "num_votes", num_votes);

    // add new movie user rating vote count
    let user_rating = response.userRating;
    imdb_movie_table_cell_add_text(tr, "user_rating", user_rating);

    // add new movie user rating vote count
    let priority = response.priority;
    imdb_movie_table_cell_add_text(tr, "priority", priority);
}

function imdb_table_append_movies(response) {
    let title_dto_list = response.title_dto_list;
    for (let title_dto = 0; title_dto < title_dto_list.length; title_dto++) {
        imdb_table_append_movie(title_dto_list[title_dto]);
    }
}

// add new text cell to movie table row
function imdb_movie_table_cell_add_text(tr, cellName, cellText) {
    let td = document.createElement('TD');
    td.tag = cellName;
    imdb_movie_table_cell_setup_onclick_handler(td);
    tr.appendChild(td);
    //imdb_table_cell_setup_onclick_handler(td);
    td.appendChild(document.createTextNode(cellText));
}

// add new text cell to movie table row
function imdb_actor_table_cell_add_text(tr, cellName, cellText) {
    let td = document.createElement('TD');
    td.tag = cellName;
    imdb_actor_table_cell_setup_onclick_handler(td);
    tr.appendChild(td);
    //imdb_table_cell_setup_onclick_handler(td);
    td.appendChild(document.createTextNode(cellText));
}

// enable clicking movie table row to highlight row
let actor_row_selected;

function imdb_actor_table_cell_setup_onclick_handler(cell) {
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

function imdb_movie_table_cell_setup_onclick_handler(cell) {
    // do something on onclick event for cells
    cell.addEventListener('contextmenu', function (e) {
        e.preventDefault();
        // Get the row id where the cell exists
        movie_row_selected = this.parentNode;
        $("#movie-row-dialog-contextmenu").dialog("open");
    }, false);
}

// extract movie name from first cell of movie table row
function imdb_table_title_for_row(tr) {
    let td = imdb_table_td_for_tag(tr, "title");
    return td.textContent;
}

// extract movie name from first cell of movie table row
function imdb_table_actor_name_id_for_row(tr) {
    let td = imdb_table_td_for_tag(tr, "id");
    return td.textContent;
}

// extract movie name from first cell of movie table row
function imdb_table_movie_title_id_for_row(tr) {
    let td = imdb_table_td_for_tag(tr, "id");
    return td.textContent;
}

function imdb_table_replace_actor(actor) {
    // find and replace row
    let name_id = actor.name_id;
    let tr = imdb_table_row_by_actor_name_id(name_id);
    if (tr === undefined) {
        imdb_table_add_actor(actor);
    } else {
        let actor_title_id_td = imdb_table_td_for_tag(tr, "id");
        actor_title_id_td.textContent = actor["name_id"];

        let actor_name_td = imdb_table_td_for_tag(tr, "name");
        actor_name_td.textContent = actor["primaryName"];

        let actor_born_td = imdb_table_td_for_tag(tr, "born");
        actor_born_td.textContent = actor["birthYear"];

        let actor_died_td = imdb_table_td_for_tag(tr, "died");
        actor_died_td.textContent = actor["deathYear"];

        let actor_user_rating_td = imdb_table_td_for_tag(tr, "user_rating");
        actor_user_rating_td.textContent = actor["userRating"];

        let actor_priority_td = imdb_table_td_for_tag(tr, "priority");
        actor_priority_td.textContent = actor["priority"];
    }
}

function imdb_table_replace_movie(movie) {
    // find and replace row
    let title_id = movie.title_id;
    let tr = imdb_table_row_by_movie_title_id(title_id);
    if (tr === undefined) {
        imdb_table_add_movie(movie);
    } else {
        let movie_title_id_td = imdb_table_td_for_tag(tr, "id");
        movie_title_id_td.textContent = movie["title_id"];

        let movie_title_td = imdb_table_td_for_tag(tr, "title");
        movie_title_td.textContent = movie["primaryTitle"];

        let movie_year_td = imdb_table_td_for_tag(tr, "year");
        movie_year_td.textContent = movie["startYear"];

        let movie_rating_td = imdb_table_td_for_tag(tr, "rating");
        movie_rating_td.textContent = movie["averageRating"];

        let movie_num_votes_td = imdb_table_td_for_tag(tr, "num_votes");
        movie_num_votes_td.textContent =
            movie["numVotes"].toString()
                .replace(/\B(?=(\d{3})+(?!\d))/g, ',');

        let movie_user_rating_td = imdb_table_td_for_tag(tr, "user_rating");
        movie_user_rating_td.textContent = movie["userRating"];

        let movie_priority_td = imdb_table_td_for_tag(tr, "priority");
        movie_priority_td.textContent = movie["priority"];
    }
}

// return movie table row associated with actor name
function imdb_table_row_by_actor_name_id(name_id) {
    let rows = imdb_table_rows();
    for (let i = 1; i < rows.length; i++) {
        let row = rows[i];
        let row_name_id = imdb_table_actor_name_id_for_row(row);
        if (row_name_id === name_id) {
            return row;
        }
    }
    return undefined;
}

// return movie table row associated with movie title
function imdb_table_row_by_movie_title_id(title_id) {
    let rows = imdb_table_rows();
    for (let i = 1; i < rows.length; i++) {
        let row = rows[i];
        let row_title_id = imdb_table_movie_title_id_for_row(row);
        if (row_title_id === title_id) {
            return row;
        }
    }
    return undefined;
}

// return all movie table rows
function imdb_table_rows() {
    let tbody = get_imdb_table_tbody_DOM_object();
    return tbody.getElementsByTagName('tr');
}

function imdb_table_td_for_tag(tr, tag) {
    let childNodes = tr.childNodes;
    for (let i = 0; i < childNodes.length; i++) {
        let td = childNodes[i];
        if (td.tag === tag) {
            return td;
        }
    }
    return undefined;
}

function request_selected() {
    let opt = $("#imdb_select_request")[0].value;
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
}

function save_actor_request(dialog) {
    let name_id = $("#change-actor-name-id")[0].textContent;
    let user_rating = $("#change-actor-user-rating")[0].value;
    let priority = $("#change-actor-priority")[0].value;
    let req = create_imdb_request("update_name",
        [name_id, user_rating, priority]);
    send_imdb_request(req, save_actor_request_success, save_actor_request_failure);
}


function save_actor_request_failure(req) {
    alert("save request failed: '" + req + "'");
}

function save_actor_request_success(result) {
    const data = JSON.stringify(result);
    let result_obj = JSON.parse(data);
    console.log("class name: " + result_obj.imdb_request_response.class_name);
    console.log(data);
    let request_name = result_obj.imdb_request.request;
    display_response(request_name, result_obj);
}

function save_movie_request(dialog) {
    let title_id = $("#change-movie-title-id")[0].textContent;
    let user_rating = $("#change-movie-user-rating")[0].value;
    let priority = $("#change-movie-priority")[0].value;
    let req = create_imdb_request("update_title",
        [title_id, user_rating, priority]);
    send_imdb_request(req, save_movie_request_success, save_movie_request_failure);
}


function save_movie_request_failure(req) {
    alert("save request failed: '" + req + "'");
}

function save_movie_request_success(result) {
    const data = JSON.stringify(result);
    let result_obj = JSON.parse(data);
    console.log("class name: " + result_obj.imdb_request_response.class_name);
    console.log(data);
    let request_name = result_obj.imdb_request.request;
    display_response(request_name, result_obj);
}

function search_actor_element() {
    let name_id_td = imdb_table_td_for_tag(actor_row_selected, "id");
    let name_id_text = name_id_td.textContent;
    let payload = create_imdb_request("search_title", name_id_text);
    send_imdb_request(payload, send_movie_request_success, send_movie_request_failure);
}

function search_movie_element() {
    let title_id_td = imdb_table_td_for_tag(movie_row_selected, "id");
    let title_id_text = title_id_td.textContent;
    let payload = create_imdb_request("search_name", title_id_text);
    send_imdb_request(payload, send_movie_request_success, send_movie_request_failure);
}

function select_actor_elements() {
    let name_id_td = imdb_table_td_for_tag(actor_row_selected, "id");
    let name_td = imdb_table_td_for_tag(actor_row_selected, "name");
    let born_td = imdb_table_td_for_tag(actor_row_selected, "born");
    let died_td = imdb_table_td_for_tag(actor_row_selected, "died");
    let user_rating_td = imdb_table_td_for_tag(actor_row_selected, "user_rating");
    let priority_td = imdb_table_td_for_tag(actor_row_selected, "priority");

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
    let title_id_td = imdb_table_td_for_tag(movie_row_selected, "id");
    let title_td = imdb_table_td_for_tag(movie_row_selected, "title");
    let year_td = imdb_table_td_for_tag(movie_row_selected, "year");
    let rating_td = imdb_table_td_for_tag(movie_row_selected, "rating");
    let votes_td = imdb_table_td_for_tag(movie_row_selected, "num_votes");
    let user_rating_td = imdb_table_td_for_tag(movie_row_selected, "user_rating");
    let priority_td = imdb_table_td_for_tag(movie_row_selected, "priority");

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

// run imdb_request
// success: return data
// fail: return null
function send_imdb_request(req, success_callback, failure_callback) {
    try {
        $.ajax({
            url: "http://34.106.93.238:8000/imdb-request-dto",
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

function send_movie_request() {
    let request = $("#imdb_select_request")[0].value;
    let argument_el = $('#argument_input')[0];
    let argument = argument_el.value;
    let request_descriptor = request_parameters[request];
    if (argument === "" && request_descriptor.needs_argument) {
        argument_el.placeholder = "Request requires value";
        return;
    }
    let payload = create_imdb_request(request, argument);
    send_imdb_request(payload, send_movie_request_success, send_movie_request_failure);
}

function send_movie_request_failure(req) {
    alert("request failed: '" + req + "'");
}

function send_movie_request_success(result) {
    const data = JSON.stringify(result);
    let result_obj = JSON.parse(data);
    console.log("class name: " + result_obj.imdb_request_response.class_name);
    console.log(data);
    let request_name = result_obj.imdb_request.request;
    display_response(request_name, result_obj);
}
