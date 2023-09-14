/*
 * requests.js:
 * Request class
 */

class Request {
    static request_parameters = {
        account_select_all: {"argument_count": 0},
        deck_add_card: {"argument_count": 2},
        deck_remove_card: {"argument_count": 2},
        deck_select_all_for_account_id: {"argument_count": 1},
        deck_select_all_cards: {"argument_count": 1},
        deck_select_other_cards: {"argument_count": 1}
    };
    // only one request can be active at any time
    static state = null;
    //static request_url = "http://34.106.93.238:8000";
    static request_url = "http://localhost:8000";
    request = null;
    arguments = null
    message = null;

    constructor(args) {
        Request.state = args.state;
        this.request = args.request;
        this.arguments = args.arguments;
        this.message = this.create();
    }

    create() {
        let argument_count = Request.request_parameters[this.request].argument_count;
        switch (argument_count) {
            case 0:
                return '{"class_name":"Mtg_request","request":"' + this.request
                    + '","arguments":[]}';
            case 1:
                return '{"class_name":"Mtg_request","request":"' + this.request
                    + '","arguments":["' + this.arguments + '"]}';
            default:
                return '{"class_name":"Mtg_request","request":"' + this.request
                    + '","arguments":["' + this.arguments.join('","') + '"]}';
        }
    }

    static parse_response(result) {
        const data = JSON.stringify(result);
        let result_obj = JSON.parse(data);
        console.log(`class name: {result_obj.class_name}`);
        console.log(data);
        return result_obj;
    }

    send(success_callback, failure_callback) {
        try {
            $.ajax({
                url: Request.request_url,
                type: "POST",
                data: this.message,
                success: function (result) {
                    if (success_callback)
                        success_callback(result);
                    else
                        console.log(`request success for '${this.message}'`);
                }, failure: function (result) {
                    if (failure_callback)
                        failure_callback(result);
                    else
                        console.log(`request failure for '${this.message}'`);
                }
            });
        } catch (error) {
            console.error(error);
        }
    }
}