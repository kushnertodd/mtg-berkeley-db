/*
 * requests.js:
 * Request class
 */

class User_Select_list {
    static select_list_name = "select_user";
    static div_id = "select_user_list";
    static id = "select_user_id";

    static select_list;
    static first = true;

    static clear() {
        User_Select_list.select_list.clear();
    }


    static create() {
        let request = new Request({
            request: "account_select_all"
        });
        request.send(User_Select_list.select_users_request_success, User_Select_list.select_users_request_failure);
    }

    static init() {
        User_Select_list.select_list = new Select_list({
            name: User_Select_list.select_list_name,
            div_id: User_Select_list.div_id,
            id: User_Select_list.id,
            event_listener: User_Select_list.user_list_selected
        });
        this.create();
    }

    static select_users_request_success(result) {
        const data = JSON.stringify(result);
        let result_obj = JSON.parse(data);
        console.log("class name: " + result_obj.class_name);
        console.log(data);
        let request_name = result_obj.mtg_request.request;
        User_Select_list.user_list_populate(result_obj.mtg_request_response.account_dto_list);
    }

    static select_users_request_failure(req) {
        alert("select user request failed: '" + req + "'");
    }


    static user_list_selected() {
        if (User_Select_list.first) {
            User_Select_list.first = false;
            User_Select_list.select_list.remove(0);
        }
        let opt = User_Select_list.select_list.select_list.value;
        let request = new Request({
            request: "deck_select_all_for_account_id",
            arguments: opt
        });
        request.send(User_Select_list.user_decks_select_request_success, User_Select_list.user_decks_select_request_failure);
    }

    static user_decks_select_request_failure(req) {
        alert(`select decks request failed: '${req}'`);
    }

    static user_decks_select_request_success(result) {
        const data = JSON.stringify(result);
        let result_obj = JSON.parse(data);
        console.log(`class name: {result_obj.class_name}`);
        console.log(data);
        let request_name = result_obj.mtg_request.request;
        Deck_table.create(result_obj.mtg_request_response.deck_dto_list);
    }

    static user_list_populate(account_dto_list) {
        let select_list = User_Select_list.select_list;
        select_list.clear();
        select_list.add_option({text: "Select user", disabled: true});
        for (let i = 0; i < account_dto_list.length; i++) {
            let account = account_dto_list[i];
            select_list.add_option({
                data: account,
                text: account.username,
                value: account.account_id
            });
        }
    }

    // static user_select_request() {
    //     let user_request = this.dom[0].value;
    //     let argument_el = $('#argument_input')[0];
    //     let argument = argument_el.value;
    //     let request_descriptor = request_parameters[request];
    //     if (argument === "" && request_descriptor.needs_argument) {
    //         argument_el.placeholder = "Request requires value";
    //         return false;
    //     }
    //     let request = new Request({
    //         request: user_request,
    //         arguments: arguments,
    //
    //     });
    //     request.send(User_Select_list.user_select_request_success, User_Select_list.user_select_request_failure);
    //     return false;
    // }

    // static user_select_request_failure(req) {
    //     alert(`select user request failed: '${req}'`);
    // }
    //
    // static user_select_request_success(result) {
    //     const data = JSON.stringify(result);
    //     let result_obj = JSON.parse(data);
    //     console.log(`class name: {result_obj.class_name}`);
    //     console.log(data);
    //     let request_name = result_obj.mtg_request.request;
    //     user_list_populate(result_obj.mtg_request_response.account_dto_list);
    // }
}