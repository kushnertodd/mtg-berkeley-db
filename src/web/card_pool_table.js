/*
 * requests.js:
 * Request class
 */

class Card_pool_table {
    static table_name = "card_pool";
    static div_id = "card_pool_table";
    static id = "card_pool_table_id";

    static label;
    static table;

    static buttons;
    static add_card_button;
    static selected_row;

    static select_add_card_request_failure(req) {
        alert(`select user request failed: '${req}'`);
    }

    static select_add_card_request_success(result) {
        const data = JSON.stringify(result);
        let result_obj = JSON.parse(data);
        console.log(`class name: {result_obj.class_name}`);
        console.log(data);
        let request_name = result_obj.mtg_request.request;
        //Card_table.label_set();
        Deck_table.add_cards_button.enable();
        //Card_table.create(result_obj.mtg_request_response.card_dto_list);
        let select_all_request = new Request({
            request: "deck_select_all_cards",
            arguments: Deck_table.deck_id
        });
        select_all_request.send(Deck_table.select_deck_cards_request_success, Deck_table.select_deck_cards_request_failure);
        let select_other_request = new Request({
            request: "deck_select_other_cards",
            arguments: Deck_table.deck_id
        });
        select_other_request.send(Deck_table.select_deck_other_cards_request_success, Deck_table.select_deck_other_cards_request_failure);

    }

    static button_add_card(e) {
        e.preventDefault();
        let card_pool_row_selected = Card_pool_table.selected_row;
        let card_id = card_pool_row_selected.data.card_id;
        let deck_id = Deck_table.deck_id;
        let request = new Request({
            request: "deck_add_card",
            arguments: [deck_id, card_id]
        });
        request.send(Card_pool_table.select_add_card_request_success, Card_pool_table.select_add_card_request_failure);
    }

    static clear() {
        Card_pool_table.table.clear();
        Card_pool_table.label_unset();
        Card_pool_table.buttons.hide();
    }

    static create(card_list) {
        let headers = ["Card", "Color"];
        Card_description_table.clear();
        let table = Card_pool_table.table;
        table.clear();
        table.add_row({id: "r0"})
        let cellWidths = ['50%', '50%'];
        for (let i = 0; i < headers.length; i++)
            table.add_th({
                    row_id: "r0",
                    id: `h${i}`,
                    text: headers[i],
                    class_name: " header",
                    width: cellWidths[i]
                }
            );
        let card_pool = card_list;
        for (let i = 0; i < card_pool.length; i++) {
            let row_id = `r${i}`;
            let tr = table.add_row({data: card_pool[i], id: row_id})
            tr.addEventListener('click', Card_pool_table.row_onlick_handler);
            //tr.addEventListener('contextmenu', Card_pool_table.row_contextmenu_onlick_handler);
            table.add_td({row_id: row_id, id: "card_pool_name", text: card_pool[i].name})
            table.add_td({row_id: row_id, id: "card_pool_color", text: card_pool[i].type_id})
        }
        Card_pool_table.label_set();
        Card_pool_table.buttons.show();
    }

    static init() {
        Card_pool_table.table = new Table({
            name: Card_pool_table.table_name,
            div_id: Card_pool_table.div_id,
            id: Card_pool_table.id
        });
        Card_pool_table.label = $("#displayed_card_pool");
        Card_pool_table.buttons = new Button_set({
            name: "buttons",
            div_id: "card_pool_table_buttons",
            hidden: true
        });
        Card_pool_table.add_card_button = new Button({
            name: "Add card",
            id: "card_table_add_card",
            event_listener: Card_pool_table.button_add_card,
            disabled: true
        });
        Card_pool_table.buttons.add_button(Card_pool_table.add_card_button);
    }

    static label_set() {
        let deck_name = Deck_table.deck_name;
        Card_pool_table.label.html(`Cards for deck: ${deck_name}`);
    }

    static label_unset() {
        Card_pool_table.label.html("");
    }

    static row_contextmenu_onlick_handler(e) {
        e.preventDefault();
        let card_row_selected = e.currentTarget;
        let data = card_row_selected.data;
        let card_id = data.card_id;
        Table.select_row(card_row_selected);
        Card_description_table.label_set();
        Card_description_table.create(data);
    }

    static row_onlick_handler(e) {
        e.preventDefault();
        let card_pool_row_selected = e.currentTarget;
        let data = card_pool_row_selected.data;
        let card_id = data.card_id;
        let table = Card_pool_table.table;
        table.select_row(card_pool_row_selected);
        Card_pool_table.selected_row = card_pool_row_selected;
        Card_pool_table.add_card_button.enable();
    }
}