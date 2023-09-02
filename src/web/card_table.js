/*
 * requests.js:
 * Request class
 */

class Card_table {
    static table_name= "card";
    static div= "card_table";
    static id= "card_id";
    static card_table;

    static init() {
        Card_table.card_table = new Table({
            name: Card_table.table_name,
            div_id: Card_table.div_id,
            id: Card_table.id
        });
    }

   static create(card) {
    }

}