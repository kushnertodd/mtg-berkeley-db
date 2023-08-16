/*
app-wide functions
*/

// all movies loaded in to loadedMovies
function app_append_all_movies(movies) {
    // sort loaded movies by name
    app_sort_movies(movies);
    for (let i = 0; i < movies.length; i++) {
        let movie = movies[i];
        app_append_movie(movie);
    }
    app_select_movie_by_index(1);
}

// add movie into movie table and movie selection list
function app_append_movie(movie) {
    // https://www.valentinog.com/blog/html-table/
    let movieName = movie.name; //value.movies[i].name;
    console.log(`add_new_movie:     name '${movieName}'`);
    // add new movie table row
    movie_table_append_movie(movie);
    // add new selection list movie option
    movie_selection_list_append_movie(movieName);
    // set current new movie field
    //movie_current_field_set_movie(movieName);
}

// select movie in table and dropdown list by index
function app_select_movie_by_index(index) {
    movie_table_select_movie_by_index(index);
    movie_selection_list_select_movie_by_index(index);
}

// select movie in table and dropdown list by index
function app_select_movie_by_movie_name(movieName) {
    movie_table_select_movie_by_name(movieName);
    movie_selection_list_select_movie_by_movie_name(movieName);
}

// all movies loaded in to loadedMovies
function app_sort_movies(movies) {
    // sort loaded movies by name
    movies.sort(function (a, b) {
        if (a.name < b.name)
            return -1;
        else if (a.name > b.name)
            return 1;
        else
            return 0;
    });
}

/*
Movie selection list functions
*/

// add movie option to movie selection list
// TODO: insert in sorted order (and/or provide option)
// https://www.w3schools.com/jsref/tryit.asp?filename=tryjsref_option_create
function movie_selection_list_append_movie(movieName) {
    let movieSelectionList = movie_selection_list_DOM_object();
    let option = document.createElement('option');
    option.value = movieName;
    let optionText = document.createTextNode(movieName);
    option.appendChild(optionText);
    movieSelectionList.appendChild(option);
}

// cache movie table body DOM object
var movieSelectionList = undefined;

// get movie table body DOM object
function movie_selection_list_DOM_object() {
    if (movieSelectionList == undefined)
        movieSelectionList = document.getElementById("movies");
    return movieSelectionList;
}

// highlights movie table row for newly selected movie dropdown list option
function movie_selection_list_movie_selected() {
    // get current movie name from selected dropdown list movie option
    let movieName = movie_selection_list_selected_name();

    // highlight that movie in the table
    movie_table_select_movie_by_name(movieName);
    // set current movie name to selected option
    //movie_current_field_set_movie(movieName);
}

// select index'th movie in movie selection list
function movie_selection_list_select_movie_by_index(index) {
    let movieSelectionList = movie_selection_list_DOM_object();
    movieSelectionList.selectedIndex = index - 1;
}

// select movie in movie selection list
function movie_selection_list_select_movie_by_movie_name(movieName) {
    let movieSelectionList = movie_selection_list_DOM_object();
    movieSelectionList.value = movieName;
}

// return movie selection list current selection
function movie_selection_list_selected_name() {
    let movieSelectionList = movie_selection_list_DOM_object()
    let movieName = movieSelectionList.value;
    return movieName;
}

/*
find movie row td for tag
 */
function movie_table_td_for_tag(tr, tag) {
    let childNodes = tr.childNodes;
    for (let i = 0; i < childNodes.length; i++) {
        let td = childNodes[i];
        if (td.tag == tag) {
            return td;
        }
    }
    return undefined;
}

/*
Movie table functions
*/

/*
function movie_table_delete_movie(movie) {
        // Get the row id where the cell exists
        let index = this.parentNode.rowIndex;
        movie_table_select_movie_by_index(index);
        movie_selection_list_select_movie_by_index(index);
    let href = movie._links.movie.href;
    let row = movie_table_row_index_by_movie_href(href);
    if (row == undefined) {
        alert("Cannot delete movie " + movie["name"] + ", not found");
    } else {
        // delete movie name
        rest_refresh_movies();
    }
}
*/

function movie_table_replace_movie(movie) {
    // find and replace row
    let href = movie._links.movie.href;
    let tr = movie_table_row_by_movie_href(href);
    if (tr == undefined) {
        movie_table_append_movie(movie);
    } else {
        // add new movie name
        let movieNameTd = movie_table_td_for_tag(tr, "name");
        movieNameTd.textContent = movie["name"];

        // add new movie user ratings
        let movieUserRatingsTd = movie_table_td_for_tag(tr, "userRatings");
        movieUserRatingsTd.textContent = movie["userRatings"];

        // add actors
        let movieActorsTd = movie_table_td_for_tag(tr, "actors");
        movieActorsTd.textContent = movie["actors"];

        // add services
        let service = movie["service"];
        let url = movie["url"];
        let movieServiceTd = movie_table_td_for_tag(tr, "service");
        movieServiceTd.childNodes[0] = movie_table_generate_anchor(service, url);

        // add comments
        let movieCommentsTd = movie_table_td_for_tag(tr, "comments");
        movieCommentsTd.textContent = movie["comments"];
    }
}

// add movie to movie table
// TODO: add sorted and/or give option
function movie_table_append_movie(movie) {
    let tbody = movie_table_body_DOM_object();
    // add new row
    let tr = document.createElement('TR');
    tbody.appendChild(tr);

    // add new movie name
    let movieName = movie["name"];
    tr.movie = movie;
    movie_table_cell_add_text(tr, "name", movieName);

    // add new movie user ratings
    let movieUserRatings = movie["userRatings"];
    movie_table_cell_add_text(tr, "userRatings", movieUserRatings);

    // add actors
    let actors = movie["actors"];
    movie_table_cell_add_actors(tr, actors);

    // add services
    let service = movie["service"];
    let url = movie["url"];
    movie_table_cell_add_services(tr, service, url);

    // add services
    let comments = movie["comments"];
    movie_table_cell_add_text(tr, "comments", comments);
}

// cache movie table body DOM object
var movieTableBody = undefined;

// get movie table body DOM object
function movie_table_body_DOM_object() {
    if (movieTableBody == undefined) {
        let table = movie_table_DOM_object();
        let tbodybytag = table.getElementsByTagName("tbody");
        movieTableBody = tbodybytag[0];
    }
    return movieTableBody;
}

// https://html-online.com/articles/simple-popup-box/
// https://www.htmlexe.com/help/popupwindows
function button_message_popup(message) {
    let result = window.confirm(message);
    return result;
}

function movie_table_button_insert(form) {
    $("#dialog-form").dialog("open");
}

// https://jqueryui.com/resources/demos/dialog/modal-form.html
function target_popup(form) {
    window.open('', 'formpopup', 'width=400,height=400,resizeable,scrollbars');
    form.target = 'formpopup';
}

function movie_table_button_delete() {
    let movieName = movie_selection_list_selected_name()
    let tr = movie_table_row_by_movie_name(movieName);
    let movie = tr.movie;
    let href = movie._links.movie.href;
    href = href.replace(/^.*8080/g, '');
    let result = button_message_popup(`Are you sure you want to delete '${movieName}'?`);
    if (result == false) {
        return;
    }
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.open("DELETE", href, true);

    xmlhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 204) {
            //Use parse() method to convert JSON string to JSON object
            //var responseJsonObj = JSON.parse(this.responseText);

            //use response
            //movie_table_delete_movie(movie);
            rest_refresh_movies();
        }
    };
    xmlhttp.send(null);
}

function movie_table_button_save() {
    let title_el = $("#title");
    let title_text = title_el[0].value;
    let ratings_el = $("#ratings");
    let ratings_text = ratings_el[0].value;
    let actors_el = $("#actors");
    let actors_text = actors_el[0].value;
    let service_el = $("#service");
    let service_text = service_el[0].value;
    let service_url_el = $("#service-url");
    let service_url_text = service_url_el[0].value;
    let comments_el = $("#comments");
    let comments_text = comments_el[0].value;
    let data = {};
    data.name = title_text;
    data.userRatings = ratings_text;
    data.actors = actors_text;
    data.service = service_text;
    data.url = service_url_text;
    data.comments = comments_text;
    let json = JSON.stringify(data);
    let dialog = $("#dialog-form");
    if (dialog_mode == "change") {
        let movie = title_el[0].movie;
        let href = movie._links.movie.href;
        href = href.replace(/^.*8080/g, '');
        let movieName = movie_selection_list_selected_name();
        let result = button_message_popup(`Are you sure you want to change '${movieName}'?`);
        if (result == false) {
            return;
        }
        let xmlhttp = new XMLHttpRequest();
        xmlhttp.open("PATCH", href, true);
        xmlhttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

        xmlhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                //Use parse() method to convert JSON string to JSON object
                let responseJsonObj = JSON.parse(this.responseText);

                //use response
                rest_refresh_movies();
            }
        };
        xmlhttp.send(json);
    } else if (dialog_mode == "insert") {
        let result = button_message_popup(`Are you sure you want to add '${title_text}'?`);
        if (result == false) {
            return;
        }
        let xmlhttp = new XMLHttpRequest();
        xmlhttp.open("POST", "/movies", true);
        xmlhttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

        xmlhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 201) {
                //Use parse() method to convert JSON string to JSON object
                let responseJsonObj = JSON.parse(this.responseText);

                //use response
                rest_refresh_movies();
            }
        };
        xmlhttp.send(json);
    } else {
        alert("what the heck was that? " + dialog_mode);
    }
}

// add new actor cell to movie table row
function movie_table_cell_add_actors(tr, actors) {
    //actors = actors.replace(/^"|"$/g, '');
    movie_table_cell_add_text(tr, "actors", actors);
}

// add new cell with multiple items to movie table row
function movie_table_cell_add_multiple_items(tr, items, hasUrl) {
    // add actors to table cell
    // add items to table cell
    if (items !== undefined) {
        let td = document.createElement('TD');
        if (!hasUrl)
            movie_table_cell_setup_onclick_handler(td);
        let itemsLength = items.length;
        if (itemsLength == 0) {
            td.appendChild(document.createTextNode(""));
            tr.appendChild(td);
        } else {
            let span = document.createElement('SPAN');
            //tr.appendChild(td);
            td.appendChild(span);
            tr.appendChild(td);
            for (let j = 0; j < itemsLength; j++) {
                if (j > 0) {
                    // it seems the anchors must be separated by br in a span, not a td
                    let br = document.createElement('BR');
                    span.appendChild(br);
                }
                let item = items[j];
                if (hasUrl) {
                    span.appendChild(movie_table_generate_anchor(item.name, item.url));
                } else {
                    span.appendChild(document.createTextNode(item));
                }
            }
        }
    }
}

function movie_table_cell_add_services(tr, service, url) {
    let td = document.createElement('TD');
    td.tag = "service";
    tr.appendChild(td);
    movie_table_cell_setup_onclick_handler(td);
    td.appendChild(movie_table_generate_anchor(service, url));
}

// add new text cell to movie table row
function movie_table_cell_add_text(tr, cellName, cellText) {
    let td = document.createElement('TD');
    td.tag = cellName;
    tr.appendChild(td);
    movie_table_cell_setup_onclick_handler(td);
    cellText = cellText.replace(/^"|"$/g, '');
    td.appendChild(document.createTextNode(cellText));
}

// enable clicking movie table row to highlight row
function movie_table_cell_setup_onclick_handler(cell) {
    // do something on onclick event for cells
    cell.onclick = function () {
        // Get the row id where the cell exists
        let index = this.parentNode.rowIndex;
        movie_table_select_movie_by_index(index);
        movie_selection_list_select_movie_by_index(index);
        $("#dialog").dialog("open");
    }
    //cell.addEventListener('contextmenu', function (e) {
    cell.addEventListener('contextmenu', function (e) {
        // do something here...
        e.preventDefault();
        //alert("context menu");
        let index = this.parentNode.rowIndex;
        movie_table_select_movie_by_index(index);
        movie_selection_list_select_movie_by_index(index);
        $("#dialog-contextmenu").dialog("open");
    }, false);
}

// deselect all non-header table body rows
function movie_table_deselect_all_rows() {
    let rowsNotSelected = movie_table_rows();
    for (let row = 1; row < rowsNotSelected.length; row++) {
        let rowNotSelected = rowsNotSelected[row];
        movie_table_deselect_row(rowNotSelected);
    }
}

// unhighlight selected movie row
function movie_table_deselect_row(rowSelected) {
    rowSelected.style.backgroundColor = "";
    rowSelected.classList.remove("selected");
}

// cache movie table div DOM object
var movieTableDiv = undefined;

// get movie table div DOM object
function movie_table_div_DOM_object() {
    if (movieTableDiv == undefined)
        movieTableDiv = document.getElementById("movietablediv");
    return movieTableDiv;
}

// cache movie table DOM object
var movieTable = undefined;

// get movie table DOM object
function movie_table_DOM_object() {
    if (movieTable == undefined)
        movieTable = document.getElementById("movietable");
    return movieTable;
}

// https://stackoverflow.com/questions/5519747/how-to-add-anchor-tags-dynamically-to-a-div-in-javascript
// https://www.techiedelight.com/dynamically-generate-anchor-tag-javascript/
function movie_table_generate_anchor(text, url) {
    let anchor = document.createElement('a');
    anchor.target = '_blank';
    anchor.href = url;
    // anchor.innerHTML/.innerText = ... ?
    let textNode = document.createTextNode(text);
    anchor.appendChild(textNode);
    return anchor;
}

// https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model/Traversing_an_HTML_table_with_JavaScript_and_DOM_Interfaces
// https://stackoverflow.com/questions/14643617/create-table-using-javascript
function movie_table_generate_header(headers) {
    let movieTableDiv = movie_table_div_DOM_object();
    // disable context menu on right click
    /*
        movieTableDiv.addEventListener('contextmenu', function (e) {
                // do something here...
                e.preventDefault();
                alert("context menu");
            }, false);
    */
    let table = movie_table_DOM_object();
    let tableBody = undefined;
    if (table == undefined) {
        table = document.createElement('TABLE');
        movieTableDiv.appendChild(table);
        table.border = '1';
        table.id = "movietable";
        tableBody = document.createElement('TBODY');
        table.appendChild(tableBody);
    } else {
        //tableBody = table.getElementsByTagName('tbody');
        tableBody = table.childNodes[0];
        let movieTableRows = tableBody.getElementsByTagName('tr');
        for (let row = movieTableRows.length - 1; row >= 0; row--) {
            tableBody.removeChild(movieTableRows[row]);
        }
        let movieSelectionList = movie_selection_list_DOM_object();
        let movieSelectionListOptions = movieSelectionList.getElementsByTagName('option');
        for (let row = movieSelectionListOptions.length - 1; row >= 0; row--) {
            movieSelectionList.removeChild(movieSelectionListOptions[row]);
        }
    }
    let tr = document.createElement('TR');
    tableBody.appendChild(tr);

    // might be the right way to set columns widths:
    // https://stackoverflow.com/questions/928849/setting-table-column-width
    let cellWidths = ['30%', '15%', '40%', '15%'];
    for (let i = 0; i < headers.length; i++) {
        let td = document.createElement('TD');
        tr.appendChild(td);
        td.className += " header";
        td.width = cellWidths[i];
        td.appendChild(document.createTextNode(headers[i]));
    }
}

// extract movie name from first cell of movie table row
function movie_table_movie_name_for_row(row) {
    let firstTd = row.children[0];
    let firstTdChildNodes = firstTd.childNodes;
    let firstTdChildNodesText = firstTdChildNodes[0];
    let firstTdChildNodesTextData = firstTdChildNodesText.data;
    return firstTdChildNodesTextData;
}

// extract movie name from first cell of movie table row
function movie_table_movie_href_for_row(row) {
    let href = row.movie._links.movie.href;
    return href;
}

// return index'th row in movie table
function movie_table_row_by_index(index) {
    let movieTableRows = movie_table_rows();
    let rowSelected = movieTableRows[index];
    return rowSelected;
}

// return movie table row associated with movie href
function movie_table_row_by_movie_href(href) {
    let movieTableRows = movie_table_rows();
    for (let row = 1; row < movieTableRows.length; row++) {
        let checkRow = movieTableRows[row];
        //let checkMovieName = movie_table_movie_name_for_row(checkRow);
        //if (checkMovieName == movieName) {
        let checkMovieHref = movie_table_movie_href_for_row(checkRow);
        if (checkMovieHref == href) {
            return checkRow;
        }
    }
    return undefined;
}

// return movie table row index associated with movie href
function movie_table_row_index_by_movie_href(href) {
    let movieTableRows = movie_table_rows();
    for (let row = 1; row < movieTableRows.length; row++) {
        let checkRow = movieTableRows[row];
        //let checkMovieName = movie_table_movie_name_for_row(checkRow);
        //if (checkMovieName == movieName) {
        let checkMovieHref = movie_table_movie_href_for_row(checkRow);
        if (checkMovieHref == href) {
            return row;
        }
    }
    return undefined;
}

// return movie table row associated with movie name
function movie_table_row_by_movie_name(movieName) {
    let movieTableRows = movie_table_rows();
    for (let row = 1; row < movieTableRows.length; row++) {
        let checkRow = movieTableRows[row];
        let checkMovieName = movie_table_movie_name_for_row(checkRow);
        if (checkMovieName == movieName) {
            return checkRow;
        }
    }
    return undefined;
}

// return all movie table rows
function movie_table_rows() {
    let tbody = movie_table_body_DOM_object();
    let movieTableRows = tbody.getElementsByTagName('tr');
    return movieTableRows;
}

// highlight movie table row associated with movie name
function movie_table_select_movie_by_name(movieName) {
    let rowSelected = movie_table_row_by_movie_name(movieName);
    if (rowSelected == undefined) {
        alert(`movie_table_process_selected_row_by_movie_named: movie ${movieName} not found in table `)
    } else {
        movie_table_select_row(rowSelected)
        //movie_selection_list_select_movie_by_movie_name(movieName);
    }
}

// highlight selected movie row
function movie_table_select_row(rowSelected) {
    movie_table_deselect_all_rows();
    //rowSelected.style.backgroundColor = "yellow";
    rowSelected.className += " selected";
}

// select index'th movie table row associated with movie name
function movie_table_select_movie_by_index(index) {
    let rowSelected = movie_table_row_by_index(index);
    if (rowSelected == undefined) {
        alert(`movie_table_process_selected_row_by_movie_named: movie ${movieName} not found in table `)
    } else {
        movie_table_select_row(rowSelected)
        //movie_selection_list_select_movie_by_movie_name(movieName);
    }
}

/*
REST functions
*/

// used while reading movies, actors, and services
var loadedMovies = [];
var nmovies;
var nmoviesLeft;

function rest_refresh_movies() {
    let table_header_names = ["Title", "User ratings", "Actors", "Service", "Comments"];
    movie_table_generate_header(table_header_names);
    rest_read_movies();
}

// load data from rest get-movies call
// read movie records via test and load them into movie table
function rest_read_movies() {
    let req =
        {
            "class_name": "Mtg_request",
            "request": "select_title",
            "arguments": []
        };
    let movies;
    let res =
        $.post(
            "http://localhost:8000/mtg-request-dto",
            JSON.stringify(req),
            function (data, status, xhr) {
                alert("success");
            }, "json")
        .done(function () {
            console.log("movies read");
        })
        .fail(function () {
            console.log("movies error");
        })
        .always(function () {
            console.log("movies complete");
        });
}

function select_elements() {
    let movieName = movie_selection_list_selected_name()
    let rowSelected = movie_table_row_by_movie_name(movieName);
    //let rowSelected = movie_table_row_by_index(index);
    let title_td = rowSelected.childNodes[0];
    let ratings_td = rowSelected.childNodes[1];
    let actors_td = rowSelected.childNodes[2];
    let service_td = rowSelected.childNodes[3];
    let comments_td = rowSelected.childNodes[4];
    let title_text = title_td.textContent;
    let ratings_text = ratings_td.textContent;
    let actors_text = actors_td.textContent;
    let service_text = service_td.textContent;
    let service_url_text = service_td.childNodes[0].href;
    let comments_text = comments_td.textContent;
    let title_el = $("#title");
    title_el[0].value = title_text;
    title_el[0].movie = rowSelected.movie;
    let ratings_el = $("#ratings");
    ratings_el[0].value = ratings_text;
    let actors_el = $("#actors");
    actors_el[0].value = actors_text;
    let service_el = $("#service");
    service_el[0].value = service_text;
    let service_url_el = $("#service-url");
    service_url_el[0].value = service_url_text;
    let comments_el = $("#comments");
    comments_el[0].value = comments_text;
}

// after DOM loaded
var dialog_mode;
$(document).ready(function () {
    $("#dialog-contextmenu").dialog({
        autoOpen: false,
        height: 90,
        width: 380,
        modal: true,
        closeOnEscape: true,
    });
    $("#dialog-form").dialog({
        autoOpen: false,
        height: 600,
        width: 350,
        modal: true,
        buttons: {
            "Save": function () {
                movie_table_button_save();
                $("#dialog-form").dialog("close");
            },
            Cancel: function () {
                $("#dialog-form").dialog("close");
            }
        },
        close: function () {
            form[0].reset();
            //allFields.removeClass( "ui-state-error" );
        }
    });

    var form = $("#dialog-form").find("form").on("submit", function (event) {
        event.preventDefault();
        movie_table_button_save();
    });
    $("#change-movie").button().on("click", function () {
        dialog_mode = "change";
        $("#dialog-form").dialog("open");
        select_elements();
    });
    $("#insert-movie").button().on("click", function () {
        dialog_mode = "insert";
        $("#dialog-form").dialog("open");
        //select_elements();
    });
    $("#delete-movie").button().on("click", function () {
        movie_table_button_delete();
    });
    $("#contextmenu-change-movie").button().on("click", function () {
        dialog_mode = "change";
        $("#dialog-form").dialog("open");
        $("#dialog-contextmenu").dialog("close");
        select_elements();
    });
    $("#contextmenu-insert-movie").button().on("click", function () {
        dialog_mode = "insert";
        $("#dialog-form").dialog("open");
        $("#dialog-contextmenu").dialog("close");
        //select_elements();
    });
    $("#contextmenu-delete-movie").button().on("click", function () {
        movie_table_button_delete();
        $("#dialog-contextmenu").dialog("close");
    });

    // on new movie dropdown list selection, set that movie in the movie table and current movie field
    $("#movies").change(function () {
        movie_selection_list_movie_selected();
    });
    //let table_header_names = ["Title", "User ratings", "Actors", "Services", "Comments"];
    //movie_table_generate_header(table_header_names);
    //rest_read_movies();
    rest_refresh_movies();
});

