/*
 * Table.js:
 * table class
 */
//alert("hi");

class Table {
  static tables = {};
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
  // usage: add_row({id: "id", text: "text"});
  add_row(args) {
    let row = document.createElement('TR');
    this.tbody.appendChild(row)
    row.id = args.id;
    row.innerText = args.text;
    this.rows[args.id] = row;
    row.tds = {};
    row.ths = {};
  }
  // usage: add_th({row_id: "row_id", id: "id", text: "text"});
  add_th(args) {
    let row = this.rows[args.row_id];
    let th = document.createElement('TH');
    row.appendChild(th)
    th.id = args.id;
    th.innerText = args.text;
    row.ths[args.id] = th;
  }
  // usage: add_td({row_id: "row_id", id: "id", text: "text"});
  add_td(args) {
    let row = this.rows[args.row_id];
    let td = document.createElement('TD');
    row.appendChild(td)
    td.id = args.id;
    td.innerText = args.text;
    row.tds[args.id] = td;
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
  static get(name) {
    return Table.tables[name];
  }
  print(name) {
    alert("table " + this.name);
  }
}

