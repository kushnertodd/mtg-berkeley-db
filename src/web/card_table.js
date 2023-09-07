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

    static buttons;
    static view_card_button;

    static button_show_cards(e) {
        e.preventDefault();
        let deck_row_selected = e.currentTarget;
        let data = deck_row_selected.data;
        let deck_id = data.deck_id;
        Table.select_row(deck_row_selected);
        //Card_table.label_set(data.name);
        let request = new Request({
            request: "deck_select_all_cards",
            arguments: deck_id
        });
        request.send(Deck_table.select_deck_cards_request_success, Deck_table.select_deck_cards_request_failure);
    }

    static clear() {
        Card_table.table.clear();
        Card_table.label_unset();
        Card_table.buttons.hide();
    }

    static create(card_list) {
        let headers = ["Card", "Color"];
        Card_description_table.clear();
        let table = Card_table.table;
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
        let cards = card_list;
        for (let i = 0; i < cards.length; i++) {
            let row_id = `r${i}`;
            let tr = table.add_row({data: cards[i], id: row_id})
            tr.addEventListener('onclick', Card_table.row_onlick_handler);
            //tr.addEventListener('contextmenu', Card_table.row_contextmenu_onlick_handler);
            table.add_td({row_id: row_id, id: "cards_name", text: cards[i].name})
            table.add_td({row_id: row_id, id: "cards_color", text: cards[i].type_id})
        }
        Card_table.label_set();
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
        Card_table.view_card_button = new Button({
            name: "Save",
            id: "card_table_view_card",
            event_listener: Card_table.button_view_card
        });
        Card_table.buttons.add_button(Card_table.view_card_button);
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
        let card_row_selected = e.currentTarget;
        let data = card_row_selected.row.data;
        let card_id = data.card_id;
        Table.select_row(card_row_selected);
    }

}