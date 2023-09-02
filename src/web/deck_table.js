/*
 * requests.js:
 * Request class
 */

class Deck_table {
    static table_name= "deck";
    static div= "deck_table";
    static id= "deck_id";
    static deck_table;

    static init() {
        Deck_table.deck_table = new Table({
            name: Deck_table.table_name,
            div_id: Deck_table.div_id,
            id: Deck_table.id
        });
    }

   static create(deck) {
    }

}