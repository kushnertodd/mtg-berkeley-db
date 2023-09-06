/*
 * requests.js:
 * Request class
 */

class Card_description_table {
    static table_name = "card_description";
    static div_id = "card_description_table";
    static id = "card_description_table_id";

    static label;
    static table;

    static buttons;
    static save_button;
    static cancel_button;

    static button_cancel(e) {
        alert("cancel!");
        Table.unselect_row();
        Card_description_table.clear();
    }

    static button_save(e) {
        alert("save!");
        Table.unselect_row();
        Card_description_table.clear();
    }

    static clear() {
        Card_description_table.table.clear();
        Card_description_table.label_unset();
        Card_description_table.buttons.hide();
    }

    static create(card) {
        let headers = ["Item", "Value"];
        let table = Card_description_table.table;
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
            });
        let name_row_id = "r1";
        let name_tr = table.add_row({data: card.name, id: name_row_id});
        let name_item_td = table.add_td({
            row_id: name_row_id,
            id: "card-name-item",
            text: "name"
        });
        let name_input_td = table.add_td({
            row_id: name_row_id,
            id: "card-name-input",
            text: ""
        });
        let type_id_row_id = "r2";
        let name_input = document.createElement("input");
        name_input.value = card.name;
        name_input_td.id = "card-name-input";
        name_input_td.appendChild(name_input);
        let type_id_tr = table.add_row({data: card.name, id: type_id_row_id})
        let type_id_item_td = table.add_td({
            row_id: type_id_row_id,
            id: "card-color-item",
            text: "color"
        })
        let type_id_input_td = table.add_td({
            row_id: type_id_row_id,
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

    static init() {
        Card_description_table.table = new Table({
            name: Card_description_table.table_name,
            div_id: Card_description_table.div_id,
            id: Card_description_table.id
        });
        Card_description_table.label = $("#displayed_card");
        Card_description_table.buttons = new Button_set({
            name: "buttons",
            div_id: "card-description-table-buttons",
            hidden: true
        });
        Card_description_table.save_button = new Button({
            name: "Save",
            id: "card-description-table-save",
            event_listener: Card_description_table.button_save
        });
        Card_description_table.cancel_button = new Button({
            name: "Cancel",
            id: "card-description-table-cancel",
            event_listener: Card_description_table.button_cancel
        });
        Card_description_table.buttons.add_button(Card_description_table.save_button);
        Card_description_table.buttons.add_button(Card_description_table.cancel_button);
    }

    static label_set(card_name) {
        Card_description_table.label.html(`Card: ${card_name}`);
    }

    static label_unset() {
        Card_description_table.label.html("");
    }

    static show(card) {
        //Card_description_table.label_set(card.name);
        //Card_description_table.buttons.show();
    }

}