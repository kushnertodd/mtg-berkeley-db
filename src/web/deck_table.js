/*
 * requests.js:
 * Request class
 */

class Deck_table {
    static table_name= "deck";
    static div= "table";
    static id= "deck_id";

    static label;
    static table;

    static clear() {
        Card_description_table.table.clear();
        Card_description_table.label_unset();
    }


   static create(deck_list) {
       let headers = table_headers["deck"];
       let table = Deck_table.table;
       table.clear();
       let card_table = Card_table.table;
       card_table.clear();
       table.add_row({id: "r0"})
       // might be the right way to set columns widths:
       // https://stackoverflow.com/questions/928849/setting-table-column-width
       let cellWidths = ['50%', '50%'];
       for (let i = 0; i < headers.length; i++)
           table.add_th({row_id: "r0",
               id: "h${i}",
               text: headers[i],
               class_name: " header",
               width: cellWidths[i]});
       for (let i = 0; i < deck_list.length; i++) {
           let row_id = "r${i}";
           let tr = table.add_row({data: decks[i], id: row_id})
           tr.addEventListener('click', row_onlick_handler);
           tr.addEventListener('contextmenu', row_contextmenu_onlick_handler);
           table.add_td({row_id: row_id, id: "d0", text: decks[i].name})
       }
       user_name_set();
    }


    static init() {
        Deck_table.table = new Table({
            name: Deck_table.table_name,
            div_id: Deck_table.div_id,
            id: Deck_table.id
        });
    }
    static label_set(deck_name) {
        $(displayed_deck).html("Deck: ${deck_name}");
    }

    static label_unset() {
        $(displayed_deck).html("");
    }
static row_contextmenu_onlick_handler(e) {
        e.preventDefault();
        let deck_row_selected = e.currentTarget;
        let data = deck_row_selected.data;
        let deck_id = data.deck_id;
        Table.select_row(deck_row_selected);
        deck_name_set(data.name);
        let request = new Request({request : "deck_select_all_cards",
            arguments: deck_id});
        request.send(select_deck_cards_request_success, select_deck_cards_request_failure);
    }

static row_onlick_handler(e) {
        e.preventDefault();
        let deck_row_selected = e.currentTarget;
        let data = deck_row_selected.row.data;
        let deck_id = data.deck_id;
        Table.select_row(deck_row_selected);
    }
}