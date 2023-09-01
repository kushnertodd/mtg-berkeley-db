/*
 * requests.js:
 * Requests class
 */

class Requests {
    static request_parameters = {
        account_select_all: {"argument_count": 0},
        deck_select_all_for_account_id: {"argument_count": 1},
        deck_select_all_cards: {"argument_count": 1},
    };
    // only one request can be active at any time
    static state = null;
    //static request_url = "http://34.106.93.238:8000";
    static request_url =  "http://localhost:8000";
    request = null;
    arguments = null
    message = null;
    success_callback = null;
    failure_callback =null;

    constructor(args) {
        Requests.state = args.state;
        this.request = args.request;
        this.arguments = args.arguments;
        this.success_callback = args.success_callback;
        this.failure_callback = args.failure_callback;
        this.message = create();
    }

    create() {
        let argument_count = Requests.request_parameters[this.request];
        if (argument_count > 1) {
            return '{"class_name":"Mtg_request",' + '"request":"' + request + '",'
                + '"arguments":["' + arguments.join('","') + '"]}';
        } else if (argument_count === 1) {
            return '{"class_name":"Mtg_request",' + '"request":"' + request + '",'
                + '"arguments":["' + arguments + '"]}';
        } else {
            return '{"class_name":"Mtg_request",' + '"request":"' + request + '",'
                + '"arguments":[]}';
        }
    }

    send() {
        try {
            $.ajax({
                url: request_url,
                type: "POST",
                data: message,
                success: function (result) {
                    success_callback(result);
                    if (success_callback)
                        success_callback(result);
                    else
                        console.log("request success for '${message}'");
                }, failure: function (result) {
                    if (failure_callback)
                    failure_callback(result);
                    else
                        console.log("request failure for '${message}'");
                }
            });
        } catch (error) {
            console.error(error);
        }
    }
}