/*
 * Table.js:
 * table class
 */

//alert("hi");

class Table {
    static tables = {};
    selected_row = null;

    // usage: constructor({name: "name", div_id: "id", id: "id"});
    constructor(args) {
        this.name = args.name;
        Table.tables[args.name] = this;
        this.dom = document.getElementById(args.div_id);
        let table = document.createElement('TABLE');
        this.dom.appendChild(table);
        table.id = args.id;
        this.tbody = document.createElement('TBODY');
        table.appendChild(this.tbody);
        this.rows = {};
    }

    // usage: add_row({id: "id", data: obj, text: "text"});
    add_row(args) {
        let row = document.createElement('TR');
        this.tbody.appendChild(row)
        row.table = this;
        row.id = args.id;
        if (args.data) row.data = args.data;
        if (args.text) row.innerText = args.text;
        this.rows[args.id] = row;
        row.tds = {};
        row.ths = {};
    }

    // usage: add_td({row_id: "row_id", id: "id", text: "text"});
    add_td(args) {
        let row = this.rows[args.row_id];
        let td = document.createElement('TD');
        td.row = row;
        row.appendChild(td)
        td.id = args.id;
        td.appendChild(document.createTextNode(args.text));
        row.tds[args.id] = td;
    }

    // usage: add_th({row_id: "row_id", id: "id", text: "text", class_name: "class_name", width: "cell_width"});
    add_th(args) {
        let row = this.rows[args.row_id];
        let th = document.createElement('TH');
        th.row = row;
        row.appendChild(th)
        th.id = args.id;
        th.innerText = args.text;
        if (args.class_name) th.className += args.class_name;
        if (args.width) th.width = args.width;
        row.ths[args.id] = th;
    }

    // usage: clear();
    clear() {
        let tbody = this.tbody;
        if (tbody.children.length > 0) {
            let rows = tbody.getElementsByTagName('tr');
            for (let row = rows.length - 1; row >= 0; row--) {
                tbody.removeChild(rows[row]);
            }
        }
    }

    // usage: get_row({row_id: "row_id"});
    get_row(args) {
        return this.rows[args.row_id];
    }

    // usage: get_th({row_id: "row_id", id: "id"});
    get_th(args) {
        return this.rows[args.row_id].ths[args.id];
    }

    // usage: get_td({row_id: "row_id", id: "id"});
    get_td(args) {
        return this.rows[args.row_id].tds[args.id];
    }

    // usage: get({name: "name"});
    static get(args) {
        return Table.tables[args.name];
    }

    print(name) {
        alert("table " + this.name);
    }

    static select_row(tr) {
        Table.unselect_row();
        this.selected_row = tr;
        tr.style.backgroundColor = "Gold";
        tr.className += " selected";
    }

    static unselect_row() {
        if (this.selected_row) {
            this.selected_row = null;
            tr.style.backgroundColor = "";
            tr.classList.remove("selected");
        }
    }
}

