/*
 * requests.js:
 * Request class
 */

class Card_description_table {
    static table_name = "card_description";
    static div_id = "card_description_table";
    static id = "card_description_id";
    static buttons;
    static cancel_button;
    static card_description_table;
    static displayed_card_label;
    static save_button;

    static button_cancel(e) {
        alert("cancel!");
        Card_description_table.clear();
        Card_description_table.hide();
        Card_description_table.displayed_card_label_unset();
    }

    static button_save(e) {
        alert("save!");
        Card_description_table.card_description_table.clear();
        Card_description_table.hide();
        Card_description_table.displayed_card_label_unset();
    }

    static card_label_set(card_name) {
        Card_description_table.displayed_card_label.html("Card: " + card_name);
    }

    static card_label_unset() {
        Card_description_table.displayed_card_label.html("");
    }

    static create(card) {
        let card_description_table = Card_description_table..card_description_table;
        let headers = Table.header(Card_description.table_name);
        card_description_table.clear();
        card_description_table.add_row({id: "r0"})
        let cellWidths = ['50%', '50%'];
        for (let i = 0; i < headers.length; i++)
            card_description_table.add_th({
                row_id: "r0",
                id: "h" + i,
                text: headers[i],
                class_name: " header",
                width: cellWidths[i]
            });
        let name_tr = card_description_table.add_row({data: card.name, id: "r1"});
        let name_item_td = card_description_table.add_td({
            row_id: "r1",
            id: "card-name-item",
            text: "name"
        });
        let name_input_td = card_description_table.add_td({
            row_id: "r1",
            id: "card-name-input",
            text: ""
        });
        let name_input = document.createElement("input");
        name_input.value = card.name;
        name_input_td.id = "card-name-input";
        name_input_td.appendChild(name_input);
        let type_id_tr = card_description_table.add_row({data: card.name, id: "r2"})
        let type_id_item_td = card_description_table.add_td({
            row_id: "r2",
            id: "card-color-item",
            text: "color"
        })
        let type_id_input_td = card_description_table.add_td({
            row_id: "r2",
            id: "card-color-input",
            text: ""
        });
        let type_id_input = document.createElement("input");
        type_id_input.value = card.type_id;
        type_id_input_td.id = "card-type-id-input";
        type_id_input_td.defaultValue = card.type_id;
        type_id_input_td.appendChild(type_id_input);
        Card_description_table.show(card);
    }

    static hide() {
        Card_description_table.buttons[0].style.display = 'none';
        Card_description_table.card_label_unset();
    }

    static init() {
        Card_description_table.card_description_table = new Table({
            name: Card_description_table.table_name,
            div_id: Card_description_table.div_id,
            id: Card_description_table.id
        });
        Card_description_table.buttons = $("#card-description-table-buttons");
        Card_description_table.cancel_button = $("#card-description-table-cancel");
        Card_description_table.displayed_card_label = $(displayed_card);
        Card_description_table.save_button = $("#card-description-table-save");
        Card_description_table.hide();
        Card_description_table.save_button[0].addEventListener('click', Card_description_table.button_save);
        Card_description_table.cancel_button[0].addEventListener('click', Card_description_table.button_cancel);
    }

    static show(card) {
        Card_description_table.buttons[0].style.display = 'block';
        Card_description_table.card_label_set();
        Card_description_table.save_button[0].data = card;
        Card_description_table.cancel_button[0].data = card;
    }

}