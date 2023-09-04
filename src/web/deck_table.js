/*
 * requests.js:
 * Request class
 */

class Deck_table {
    static table_name = "deck";
    static div_id = "deck_table";
    static id = "deck_table_id";

    static label;
    static table;

    static clear() {
        Deck_table.table.clear();
        Deck_table.label_unset();
    }


    static create(deck_list) {
        Card_description_table.clear();
        Card_table.clear();
        let table = Deck_table.table;
        table.clear();
        table.add_row({id: "r0"})
        let header = "Deck";
        table.add_th({
            row_id: "r0",
            id: "h0",
            text: header,
            class_name: " header",
            width: '50%'
        });
        for (let i = 0; i < deck_list.length; i++) {
            let row_id = `r${i}`;
            let tr = table.add_row({data: deck_list[i], id: row_id})
            tr.addEventListener('click', Deck_table.row_onlick_handler);
            tr.addEventListener('contextmenu', Deck_table.row_contextmenu_onlick_handler);
            table.add_td({row_id: row_id, id: "d0", text: deck_list[i].name})
        }
        //Deck_table.label_set("username");
    }


    static init() {
        Deck_table.table = new Table({
            name: Deck_table.table_name,
            div_id: Deck_table.div_id,
            id: Deck_table.id
        });
        Deck_table.label = $("#displayed_deck");
    }

    static label_set(deck_name) {
        Deck_table.label.html(`Deck: ${deck_name}`);
    }

    static label_unset() {
        Deck_table.label.html("");
    }

    static row_contextmenu_onlick_handler(e) {
        e.preventDefault();
        let deck_row_selected = e.currentTarget;
        let data = deck_row_selected.data;
        let deck_id = data.deck_id;
        Table.select_row(deck_row_selected);
        Deck_table.label_set(data.name);
        let request = new Request({
            request: "deck_select_all_cards",
            arguments: deck_id
        });
        request.send(Deck_table.select_deck_cards_request_success, Deck_table.select_deck_cards_request_failure);
    }

    static row_onlick_handler(e) {
        e.preventDefault();
        let deck_row_selected = e.currentTarget;
        let data = deck_row_selected.row.data;
        let deck_id = data.deck_id;
        Table.select_row(deck_row_selected);
    }

    static select_deck_cards_request_failure(req) {
        alert(`select user request failed: '${req}'`);
    }

    static select_deck_cards_request_success(result) {
        const data = JSON.stringify(result);
        let result_obj = JSON.parse(data);
        console.log(`class name: {result_obj.class_name}`);
        console.log(data);
        let request_name = result_obj.mtg_request.request;
        Card_table.create(result_obj.mtg_request_response.card_dto_list);
    }

}