/*
 * requests.js:
 * Request class
 */

class Card_table {
    static table_name = "card";
    static div_id = "card_table";
    static id = "card_table_id";

    static label;
    static table;
    static selected_row;
    static card_name;
    static card_id;

    static buttons;
    static remove_card_button;

    static button_remove_card(e) {
        e.preventDefault();
        let card_row_selected = Card_table.selected_row;
        let card_id = card_row_selected.data.card_id;
        let deck_id = Deck_table.deck_id;
        let request = new Request({
            request: "deck_remove_card",
            arguments: [deck_id, card_id]
        });
        request.send(Card_table.remove_deck_card_request_success, Card_table.remove_deck_card_request_failure);
    }

    static remove_deck_card_request_failure(req) {
        alert(`remove deck card request failed: '${req}'`);
    }

    static remove_deck_card_request_success(result) {
        let result_obj = Request.parse_response(result);
        let request_name = result_obj.mtg_request.request;
        //Deck_table.add_cards_button.enable();
        let select_all_request = new Request({
            request: "deck_select_all_cards",
            arguments: Deck_table.deck_id
        });
        select_all_request.send(Card_table.select_deck_cards_request_success, Card_table.select_deck_cards_request_failure);
        let select_other_request = new Request({
            request: "deck_select_other_cards",
            arguments: Deck_table.deck_id
        });
        select_other_request.send(Card_pool_table.select_deck_other_cards_request_success, Card_pool_table.select_deck_other_cards_request_failure);
    }

    static clear() {
        //Card_description_table.clear();
        Card_pool_table.clear();
        Card_table.table.clear();
        Card_table.label_unset();
        Card_table.buttons.hide();
    }

    static create(card_list) {
        let headers = ["Card", "Color"];
        Card_table.clear();
        let table = Card_table.table;
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
        if (card_list) {
            for (let i = 0; i < card_list.length; i++) {
                let row_id = `r${i}`;
                let tr = table.add_row({data: card_list[i], id: row_id})
                tr.addEventListener('click', Card_table.row_onlick_handler);
                //tr.addEventListener('contextmenu', Card_table.row_contextmenu_onlick_handler);
                table.add_td({row_id: row_id, id: "cards_name", text: card_list[i].name})
                table.add_td({row_id: row_id, id: "cards_color", text: card_list[i].type_id})
            }
            Card_table.label_set();
            //Card_table.buttons.hide();
            Card_table.editing();
        } else
            Card_table.remove_card_button.disable();
    }

    static editing() {
        if (Deck_table.editing) {
            Card_table.buttons.show();
            Card_table.remove_card_button.disable();
        } else
            Card_table.buttons.hide();
    }

    static init() {
        Card_table.table = new Table({
            name: Card_table.table_name,
            div_id: Card_table.div_id,
            id: Card_table.id
        });
        Card_table.label = $("#displayed_deck");
        Card_table.buttons = new Button_set({
            name: "buttons",
            div_id: "card_table_buttons",
            hidden: true
        });
        Card_table.remove_card_button = new Button({
            name: "Remove card",
            id: "card_table_remove_card_id",
            event_listener: Card_table.button_remove_card,
            disabled: true
        });
        Card_table.buttons.add_button(Card_table.remove_card_button);
    }

    static label_set() {
        let deck_name = Deck_table.deck_name;
        Card_table.label.html(`Deck: ${deck_name}`);
    }

    static label_unset() {
        Card_table.label.html("");
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
        if (Deck_table.editing) {
            let card_row_selected = e.currentTarget;
            Card_table.selected_row = card_row_selected;
            Card_table.table.select_row(card_row_selected);
            let card = card_row_selected.data;
            Card_table.card_id = card.card_id;
            Card_table.card_name = card.name;
            Card_table.remove_card_button.enable();
        }
    }

    static select_deck_cards_request_failure(req) {
        alert(`select user request failed: '${req}'`);
    }

    static select_deck_cards_request_success(result) {
        let result_obj = Request.parse_response(result);
        //let request_name = result_obj.mtg_request.request;
        //Card_table.label_set();
        //Deck_table.add_cards_button.enable();
        Card_table.create(result_obj.mtg_request_response.card_dto_list);
    }
}