/*
 * requests.js:
 * Request class
 */

class Card_table {
    static table_name = "card";
    static div_id = "card_table";
    static id = "card_id";

    static label;
    static table;

    static clear() {
        Card_table.table.clear();
        Card_table.label_unset();
    }

    static create(card_list) {
        let headers = Table.header(Card_description.table_name);
        let table = Card_table.table;
        table.clear();
        table.add_row({id: "r0"})
        let cellWidths = ['50%', '50%'];
        for (let i = 0; i < headers.length; i++)
            table.add_th({
                    row_id: "r0",
                    id: "h" + i,
                    text: headers[i],
                    class_name: " header",
                    width: cellWidths[i]
                }
            );
        let cards = card_list;
        for (let i = 0; i < cards.length; i++) {
            let row_id = "r${i}";
            let tr = table.add_row({data: cards[i], id: row_id})
            tr.addEventListener('onclick', Card_table.row_onlick_handler);
            tr.addEventListener('contextmenu', Card_table.row_contextmenu_onlick_handler);
            table.add_td({row_id: row_id, id: "cards-name", text: cards[i].name})
            table.add_td({row_id: row_id, id: "cards-color", text: cards[i].type_id})
        }
    }

    static init() {
        Card_table.table = new Table({
            name: Card_table.table_name,
            div_id: Card_table.div_id,
            id: Card_table.id
        });
    }


    label_set() {
        $(displayed_user).html("Decks:");
    }

    label_unset() {
        $(displayed_user).html("");
    }

    static row_contextmenu_onlick_handler(e) {
        e.preventDefault();
        let card_row_selected = e.currentTarget;
        let data = card_row_selected.data;
        let card_id = data.card_id;
        Table.select_row(card_row_selected);
        card_name_set(data.name);
        Card_description_table.create(data);
    }

    static row_onlick_handler(e) {
        e.preventDefault();
        let card_row_selected = e.currentTarget;
        let data = card_row_selected.row.data;
        let card_id = data.card_id;
        Table.select_row(card_row_selected);
    }

    static show(card) {
        Card_table.label_set();
    }
}