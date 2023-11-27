/*
app-wide functions
*/

// https://html-online.com/articles/simple-popup-box/
// https://www.htmlexe.com/help/popupwindows
function button_message_popup(message) {
    return window.confirm(message);
}

// add movie into movie table and movie selection list
function movie_display_add(movie) {
    // https://www.valentinog.com/blog/html-table/
    let movieName = movie.primaryTitle; //value.movies[i].primaryTitle;
    console.log(`add_new_movie:     name '${movieName}'`);
    // add new movie table row
    movie_table_add_movie(movie);
    // add new selection list movie option
    movie_selection_list_add_movie(movieName);
    // set current new movie field
    //movie_current_field_set_movie(movieName);
}

// all movies loaded in to loadedMovies
function movie_display_add_list(movies) {
    // sort loaded movies by name
    movie_sort_list(movies);
    for (let i = 0; i < movies.length; i++) {
        let movie = movies[i];
        movie_display_add(movie);
    }
    movie_display_select_by_index(1);
}

// select movie in table and dropdown list by index
function movie_display_select_by_index(index) {
    movie_table_select_by_index(index);
    movie_selection_list_select_by_index(index);
}

// select movie in table and dropdown list by index
function movie_display_select_by_movie_name(movieName) {
    movie_table_select_by_movie_name(movieName);
    movie_selection_list_select_by_movie_name(movieName);
}

/*
Movie selection list functions
*/

// add movie option to movie selection list
// TODO: insert in sorted order (and/or provide option)
// https://www.w3schools.com/jsref/tryit.asp?filename=tryjsref_option_create
function movie_selection_list_add_movie(movieName) {
    let movieSelectionList = movie_selection_list_DOM_object();
    let option = document.createElement('option');
    movieSelectionList.appendChild(option);
    option.value = movieName;
    let optionText = document.createTextNode(movieName);
    option.appendChild(optionText);
    //movie_selection_list_option_setup_onclick_handler(option)
}

// cache movie table body DOM object
var movieSelectionList = undefined;

// get movie table body DOM object
function movie_selection_list_DOM_object() {
    if (movieSelectionList === undefined)
        movieSelectionList = document.getElementById("movies-select-list");
    return movieSelectionList;
}

// highlights movie table row for newly selected movie dropdown list option
function movie_selection_list_movie_selected() {
    // get current movie name from selected dropdown list movie option
    let movieName = movie_selection_list_selected_name();

    // highlight that movie in the table
    movie_table_select_by_movie_name(movieName);
    // set current movie name to selected option
    //movie_current_field_set_movie(movieName);
}

// select index'th movie in movie selection list
function movie_selection_list_select_by_index(index) {
    let movieSelectionList = movie_selection_list_DOM_object();
    movieSelectionList.selectedIndex = index - 1;
}

// select movie in movie selection list
function movie_selection_list_select_by_movie_name(movieName) {
    let movieSelectionList = movie_selection_list_DOM_object();
    movieSelectionList.value = movieName;
}

// return movie selection list current selection
function movie_selection_list_selected_name() {
    let movieSelectionList = movie_selection_list_DOM_object()
    return movieSelectionList.value;
}

// enable clicking movie table row to highlight row
function movie_selection_list_option_setup_onclick_handler(option) {
    // do something on onclick event for cells
    option.onselect = function () {
        // Get the row id where the cell exists
        let index = this.parentNode.rowIndex;
        movie_table_select_by_index(index);
        movie_selection_list_select_by_index(index);
        $("#dialog").dialog("open");
    }
    option.addEventListener('contextmenu', function (e) {
        // do something here...
        e.preventDefault();
        alert("option context menu");
        let index = this.parentNode.rowIndex;
        movie_table_select_by_index(index);
        movie_selection_list_select_by_index(index);
        $("#dialog-contextmenu").dialog("open");
    }, false);
}

// all movies loaded in to loadedMovies
function movie_sort_list(movies) {
    // sort loaded movies by name
    movies.sort(function (a, b) {
        if (a.primaryTitle < b.primaryTitle)
            return -1;
        else if (a.primaryTitle > b.primaryTitle)
            return 1;
        else
            return 0;
    });
}

// add movie to movie table
// TODO: add sorted and/or give option
function movie_table_add_movie(movie) {
    let tbody = movie_table_body_DOM_object();
    // add new row
    let tr = document.createElement('TR');
    tbody.appendChild(tr);

    // add new movie name
    let movieName = movie["primaryTitle"];
    tr.movie = movie;
    movie_table_cell_add_text(tr, "name", movieName);

    // add new movie user title type
    let movieTitleType = movie["titleType"];
    movie_table_cell_add_text(tr, "titleType", movieTitleType);

    // add year
    let startYear = movie["startYear"];
    movie_table_cell_add_startYear(tr, startYear);

    // add runtimeMinutes
    let runtimeMinutes = movie["runtimeMinutes"];
    movie_table_cell_add_runtimeMinutes(tr, runtimeMinutes);

    // add runtimeMinutes
    let genres = movie["genres"];
    movie_table_cell_add_text(tr, "genres", genres);
}

/*
find movie row td for tag
 */
function movie_table_td_for_tag(tr, tag) {
    let childNodes = tr.childNodes;
    for (let i = 0; i < childNodes.length; i++) {
        let td = childNodes[i];
        if (td.tag === tag) {
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
        movie_table_select_by_index(index);
        movie_selection_list_select_by_index(index);
    let href = movie._links.movie.href;
    let row = movie_table_row_index_by_movie_href(href);
    if (row === undefined) {
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
    if (tr === undefined) {
        movie_table_add_movie(movie);
    } else {
        // add new movie name
        let movieNameTd = movie_table_td_for_tag(tr, "name");
        movieNameTd.textContent = movie["name"];

        // add new movie type
        let movieTitleTypeTd = movie_table_td_for_tag(tr, "titleType");
        movieTitleTypeTd.textContent = movie["titleType"];

        // add year
        let movieYearsTd = movie_table_td_for_tag(tr, "startYear");
        movieYearsTd.textContent = movie["startYear"];

        // add services
        let movieRuntimeMinutesTd = movie_table_td_for_tag(tr, "runtimeMinutes");
        movieRuntimeMinutesTd.childNodes[0] = document.createTextNode(runtimeMinutes);

        // add genres
        let movieGenresTd = movie_table_td_for_tag(tr, "genres");
        movieGenresTd.textContent = movie["genres"];
    }
}

// cache movie table body DOM object
var movieTableBody = undefined;

// get movie table body DOM object
function movie_table_body_DOM_object() {
    if (movieTableBody === undefined) {
        let table = movie_table_DOM_object();
        let tbodybytag = table.getElementsByTagName("tbody");
        movieTableBody = tbodybytag[0];
    }
    return movieTableBody;
}

function movie_table_button_delete() {
    let movieName = movie_selection_list_selected_name()
    let tr = movie_table_row_by_movie_name(movieName);
    let movie = tr.movie;
    let href = movie._links.movie.href;
    href = href.replace(/^.*8080/g, '');
    let result = button_message_popup(`Are you sure you want to delete '${movieName}'?`);
    if (!result) {
        return;
    }
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.open("DELETE", href, true);

    xmlhttp.onreadystatechange = function () {
        if (this.readyState === 4 && this.status === 204) {
            //Use parse() method to convert JSON string to JSON object
            //var responseJsonObj = JSON.parse(this.responseText);

            //use response
            //movie_table_delete_movie(movie);
            rest_refresh_movies();
        }
    };
    xmlhttp.send(null);
}

function movie_table_button_insert() {
    $("#dialog-form").dialog("open");
}

function movie_table_button_save() {
    let title_el = $("#title");
    let title_text = title_el[0].value;
    let type_el = $("#type");
    let type_text = type_el[0].value;
    let startYear_el = $("#startYear");
    let startYear_text = startYear_el[0].value;
    let runtimeMinutes_el = $("#runtimeMinutes");
    let runtimeMinutes_text = runtimeMinutes_el[0].value;
    let genres_el = $("#genres");
    let genres_text = genres_el[0].value;
    let data = {};
    data.name = title_text;
    data.titleType = type_text;
    data.startYear = startYear_text;
    data.runtimeMinutes = runtimeMinutes_text;
    data.genres = genres_text;
    let json = JSON.stringify(data);
    if (dialog_mode === "change") {
        let movie = title_el[0].movie;
        let href = movie._links.movie.href;
        href = href.replace(/^.*8080/g, '');
        let movieName = movie_selection_list_selected_name();
        let result = button_message_popup(`Are you sure you want to change '${movieName}'?`);
        if (!result) {
            return;
        }
        let xmlhttp = new XMLHttpRequest();
        xmlhttp.open("PATCH", href, true);
        xmlhttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

        xmlhttp.onreadystatechange = function () {
            if (this.readyState === 4 && this.status === 200) {
                //Use parse() method to convert JSON string to JSON object
                //let responseJsonObj = JSON.parse(this.responseText);

                //use response
                rest_refresh_movies();
            }
        };
        xmlhttp.send(json);
    } else if (dialog_mode === "insert") {
        let result = button_message_popup(`Are you sure you want to add '${title_text}'?`);
        if (!result) {
            return;
        }
        let xmlhttp = new XMLHttpRequest();
        xmlhttp.open("POST", "/movies", true);
        xmlhttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

        xmlhttp.onreadystatechange = function () {
            if (this.readyState === 4 && this.status === 201) {
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
function movie_table_cell_add_startYear(tr, startYear) {
    //startYear = startYear.replace(/^"|"$/g, '');
    movie_table_cell_add_text(tr, "startYear", startYear);
}

// add new cell with multiple items to movie table row
function movie_table_cell_add_multiple_items(tr, items) {
    // add startYear to table cell
    // add items to table cell
    if (items !== undefined) {
        let td = document.createElement('TD');
        movie_table_cell_setup_onclick_handler(td);
        let itemsLength = items.length;
        if (itemsLength === 0) {
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
                span.appendChild(document.createTextNode(item));
            }
        }
    }
}

function movie_table_cell_add_runtimeMinutes(tr, runtimeMinutes) {
    let td = document.createElement('TD');
    td.tag = "runtimeMinutes";
    tr.appendChild(td);
    movie_table_cell_setup_onclick_handler(td);
    td.appendChild(document.createTextNode(runtimeMinutes));
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
        movie_table_select_by_index(index);
        movie_selection_list_select_by_index(index);
        $("#dialog").dialog("open");
    }
    cell.addEventListener('contextmenu', function (e) {
        // do something here...
        e.preventDefault();
        //alert("cell context menu");
        let index = this.parentNode.rowIndex;
        movie_table_select_by_index(index);
        movie_selection_list_select_by_index(index);
        $("#dialog-contextmenu").dialog("open");
    }, false);
}

// deselect all non-header table body rows
function movie_table_deselect_all_rows() {
    let rows = movie_table_rows();
    for (let row = 1; row < rows.length; row++) {
        movie_table_deselect_row(rows[row]);
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
    if (movieTableDiv === undefined)
        movieTableDiv = document.getElementById("movietablediv");
    return movieTableDiv;
}

// cache movie table DOM object
var movieTable = null;

// get movie table DOM object
function movie_table_DOM_object() {
    // if (!movieTable) {
    //     let movie_table_div = movie_table_div_DOM_object()
    //     // movieTable = document.getElementById("movietable");
    //     movieTable = movie_table_div.getElementsByTagName("table");
    // }
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
    let movie_table_div = movie_table_div_DOM_object();
    // disable context menu on right click
    movieTableDiv.addEventListener('contextmenu', function (e) {
        // do something here...
        e.preventDefault();
        //("table context menu");
    }, false);

    let movieSelectionList = movie_selection_list_DOM_object();
    // disable context menu on right click
    movieSelectionList.addEventListener('contextmenu', function (e) {
        // do something here...
        e.preventDefault();
        //alert("selection list context menu");
    }, false);
    movieSelectionList.onchange = function () {
        //alert("selection list on change");
        value = movie_selection_list_selected_name();
        movie_table_select_by_movie_name(value);
        $("#dialog").dialog("open");
    }
    let table = movie_table_DOM_object();
    let tableBody;
    if (!table) {
        table = document.createElement('TABLE');
        movieTable = table;
        movie_table_div.appendChild(table);
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
    return firstTdChildNodesText.data;
}

// extract movie name from first cell of movie table row
function movie_table_movie_href_for_row(row) {
    return row.movie._links.movie.href;
}

// return index'th row in movie table
function movie_table_row_by_index(index) {
    let movieTableRows = movie_table_rows();
    return movieTableRows[index];
}

// return movie table row associated with movie href
function movie_table_row_by_movie_href(href) {
    let movieTableRows = movie_table_rows();
    for (let row = 1; row < movieTableRows.length; row++) {
        let checkRow = movieTableRows[row];
        //let checkMovieName = movie_table_movie_name_for_row(checkRow);
        //if (checkMovieName == movieName) {
        let checkMovieHref = movie_table_movie_href_for_row(checkRow);
        if (checkMovieHref === href) {
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
        if (checkMovieHref === href) {
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
        if (checkMovieName === movieName) {
            return checkRow;
        }
    }
    return undefined;
}

// return all movie table rows
function movie_table_rows() {
    let tbody = movie_table_body_DOM_object();
    return tbody.getElementsByTagName('tr');
}

// highlight movie table row associated with movie name
function movie_table_select_by_movie_name(movieName) {
    let rowSelected = movie_table_row_by_movie_name(movieName);
    if (rowSelected === undefined) {
        alert(`movie_table_process_selected_row_by_movie_named: movie ${movieName} not found in table `)
    } else {
        movie_table_select_row(rowSelected)
        //movie_selection_list_select_by_movie_name(movieName);
    }
}

// highlight selected movie row
function movie_table_select_row(rowSelected) {
    movie_table_deselect_all_rows();
    //rowSelected.style.backgroundColor = "yellow";
    rowSelected.className += " selected";
}

// select index'th movie table row associated with movie name
function movie_table_select_by_index(index) {
    let rowSelected = movie_table_row_by_index(index);
    if (rowSelected === undefined) {
        alert(`movie_table_process_selected_row_by_movie_named: movie ${movieName} not found in table `)
    } else {
        movie_table_select_row(rowSelected)
        //movie_selection_list_select_by_movie_name(movieName);
    }
}

/*
REST functions
*/

// used while reading movies, startYear, and runtimeMinutes
//var loadedMovies = [];
//var nmovies;
//var nmoviesLeft;

function rest_refresh_movies() {
    let table_header_names = ["Title", "Type", "Year", "Runtime (min)", "Genres"];
    movie_table_generate_header(table_header_names);
    rest_read_movies();
}

// load data from rest get-movies call
// read movie records via test and load them into movie table
function rest_read_movies() {
    rest_select_movies(rest_select_movies_success_callback, rest_select_movies_failure_callback);
    // let movies = rest_select_movies();
    // if (movies) {
    //     movie_display_add_list(movies);
    // }
}

// run mtg_request
// success: return data
// fail: return null
function rest_mtg_request(req, success_callback, failure_callback) {
    let res =
        $.post(
            "http://localhost:8000/mtg-request-dto",
            JSON.stringify(req),
            function (data, status, xhr) {
                if (success_callback)
                    success_callback(data);
            }, "json")
            .done(function () {
                console.log("movies read");
            })
            .fail(function () {
                if (failure_callback)
                    failure_callback();
                console.log("rest_mtg_request error");
                return null;
            })
            .always(function () {
                console.log("movies complete");
            });
}

// read all actor records
function rest_lookup_actor(name_id) {
    let req =
        {
            "class_name": "Mtg_request",
            "request": "lookup_name",
            "arguments": [name_id]
        };
    let data = rest_mtg_request(req);
    if (data)
        return data.name;
    else
        return null;
}

// read all movie records
function rest_lookup_movie(title_id) {
    let req =
        {
            "class_name": "Mtg_request",
            "request": "lookup_title",
            "arguments": [title_id]
        };
    let data = rest_mtg_request(req);
    if (data)
        return data.title;
    else
        return null;
}

// read all actor records
function rest_search_actors(title_id) {
    let req =
        {
            "class_name": "Mtg_request",
            "request": "search_name",
            "arguments": [title_id]
        };
    let data = rest_mtg_request(req);
    if (data)
        return data.names;
    else
        return null;
}

// read all movie records
function rest_search_movies(name_id) {
    let req =
        {
            "class_name": "Mtg_request",
            "request": "search_title",
            "arguments": [name_id]
        };
    let data = rest_mtg_request(req);
    if (data)
        return data.titles;
    else
        return null;
}

// read all actor records
function rest_select_actors() {
    let req =
        {
            "class_name": "Mtg_request",
            "request": "select_name",
            "arguments": []
        };
    let data = rest_mtg_request(req);
    if (data)
        return data.names;
    else
        return null;
}

// read all movie records
function rest_select_movies_success_callback(movies) {
    if (movies)
        movie_display_add_list(movies.titles);
    else
        alert("rest_select_movies_success_callback: null movies")
}

function rest_select_movies_failure_callback() {
    alert("is the server running?")
}

function rest_select_movies(rest_select_movies_success_callback, rest_select_movies_failure_callback) {
    let req =
        {
            "class_name": "Mtg_request",
            "request": "select_title",
            "arguments": []
        };
    rest_mtg_request(req, rest_select_movies_success_callback, rest_select_movies_failure_callback);
}

function select_elements() {
    let movieName = movie_selection_list_selected_name()
    let rowSelected = movie_table_row_by_movie_name(movieName);
    //let rowSelected = movie_table_row_by_index(index);
    let title_td = rowSelected.childNodes[0];
    let type_td = rowSelected.childNodes[1];
    let startYear_td = rowSelected.childNodes[2];
    let runtimeMinutes_td = rowSelected.childNodes[3];
    let genres_td = rowSelected.childNodes[4];
    let title_text = title_td.textContent;
    let type_text = type_td.textContent;
    let startYear_text = startYear_td.textContent;
    let runtimeMinutes_text = runtimeMinutes_td.textContent;
    //let runtimeMinutes_url_text = runtimeMinutes_td.childNodes[0].href;
    let genres_text = genres_td.textContent;
    let title_el = $("#title");
    title_el[0].value = title_text;
    title_el[0].movie = rowSelected.movie;
    let type_el = $("#type");
    type_el[0].value = type_text;
    let startYear_el = $("#startYear");
    startYear_el[0].value = startYear_text;
    let runtimeMinutes_el = $("#runtimeMinutes");
    runtimeMinutes_el[0].value = runtimeMinutes_text;
    let genres_el = $("#genres");
    genres_el[0].value = genres_text;
}

// https://jqueryui.com/resources/demos/dialog/modal-form.html
function target_popup(form) {
    window.open('', 'formpopup', 'width=400,height=400,resizeable,scrollbars');
    form.target = 'formpopup';
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
                //movie_table_button_save();
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
        //movie_table_button_save();
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
        //movie_table_button_delete();
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
        //movie_table_button_delete();
        $("#dialog-contextmenu").dialog("close");
    });

    // on new movie dropdown list selection, set that movie in the movie table and current movie field
    $("#movies").change(function () {
        movie_selection_list_movie_selected();
    });
    //let table_header_names = ["Title", "Type", "Year", "Runtime (min)", "Genres"];
    //movie_table_generate_header(table_header_names);
    //rest_read_movies();
    rest_refresh_movies();
});

