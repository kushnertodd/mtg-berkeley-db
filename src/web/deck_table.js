/*
 * requests.js:
 * Request class
 */

class Deck_table {
    static table_name = "deck";
    static div_id = "deck_table";
    static id = "deck_table_id";

    static editing;
    static label;
    static table;
    static selected_row;
    static deck_name;
    static deck_id;

    static buttons;
    //static show_cards_button;
    static edit_deck_button;

    static show_cards(deck_id) {
        let request = new Request({
            request: "deck_select_all_cards",
            arguments: deck_id
        });
        request.send(Card_table.select_deck_cards_request_success, Card_table.select_deck_cards_request_failure);
    }

    static button_edit_deck(e) {
        e.preventDefault();
        Deck_table.editing = true;
        Deck_table.edit_deck_button.disable();
        Card_table.editing();
        let deck_id = Deck_table.deck_id;
        let request = new Request({
            request: "deck_select_other_cards",
            arguments: deck_id
        });
        // TODO: somehow disable edit button here
        request.send(Card_pool_table.select_deck_other_cards_request_success, Card_pool_table.select_deck_other_cards_request_failure);
    }

    static clear() {
        //Card_description_table.clear();
        Card_table.clear();
        Card_pool_table.clear();
        Deck_table.table.clear();
        Deck_table.label_unset();
        Deck_table.buttons.hide();
    }


    static create(deck_list) {
        Deck_table.clear();
        let table = Deck_table.table;
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
            let deck = deck_list[i];
            let row_id = `r${i}`;
            let tr = table.add_row({data: deck, id: row_id})
            tr.addEventListener('click', Deck_table.row_onlick_handler);
            //tr.addEventListener('contextmenu', Deck_table.row_contextmenu_onlick_handler);
            table.add_td({row_id: row_id, id: "d0", text: deck_list[i].name})
        }
        Deck_table.label_set();
        Deck_table.buttons.hide();
    }


    static init() {
        Deck_table.table = new Table({
            name: Deck_table.table_name,
            div_id: Deck_table.div_id,
            id: Deck_table.id
        });
        Deck_table.label = $("#displayed_user");
        Deck_table.buttons = new Button_set({
            name: "buttons",
            div_id: "deck_table_buttons",
            hidden: true
        });
        // Deck_table.show_cards_button = new Button({
        //     name: "Show cards",
        //     id: "deck_table_show_cards",
        //     event_listener: Deck_table.button_show_cards,
        //     disabled: true
        // });
        // Deck_table.buttons.add_button(Deck_table.show_cards_button);
        Deck_table.edit_deck_button = new Button({
            name: "Edit deck",
            id: "deck_table_edit_deck_id",
            event_listener: Deck_table.button_edit_deck,
            disabled: true
        });
        Deck_table.buttons.add_button(Deck_table.edit_deck_button);
    }

    static label_set() {
        let username = User_Select_list.username;
        Deck_table.label.html(`${username}'s decks:`);
    }

    static label_unset() {
        Deck_table.label.html("");
    }

    // static row_contextmenu_onlick_handler(e) {
    //     // TODO: add unselecting row if already selected, disable show/add card button, remove card/card pool tables
    //     e.preventDefault();
    //     let deck_row_selected = e.currentTarget;
    //     let data = deck_row_selected.data;
    //     let deck_id = data.deck_id;
    //     let table = Deck_table.table;
    //     table.select_row(deck_row_selected);
    //     let request = new Request({
    //         request: "deck_select_all_cards",
    //         arguments: deck_id
    //     });
    //     request.send(Deck_table.select_deck_cards_request_success, Deck_table.select_deck_cards_request_failure);
    // }

    static row_onlick_handler(e) {
        e.preventDefault();
        let deck_row_selected = e.currentTarget;
        Deck_table.selected_row = deck_row_selected;
        Deck_table.table.select_row(deck_row_selected);
        let deck = deck_row_selected.data;
        let deck_id = deck.deck_id;
        Deck_table.deck_id = deck_id;
        Deck_table.deck_name = deck.name;
        Deck_table.buttons.show();
        Deck_table.edit_deck_button.enable();
        Deck_table.editing = false;
        Deck_table.show_cards(deck_id);
    }


    static user_decks_select_request_failure(req) {
        alert(`select decks request failed: '${req}'`);
    }

    static user_decks_select_request_success(result) {
        let result_obj = Request.parse_response(result);
        let request_name = result_obj.mtg_request.request;
        Deck_table.create(result_obj.mtg_request_response.deck_dto_list);
    }


}