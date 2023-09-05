class Button {
    static buttons = {};
    name = null;
    options = {};
    button = null;

    // usage: constructor({name: "name", id: "id", event_listener: eventListener, disabled: bool});
    constructor(args) {
        this.name = args.name;
        Button.buttons[args.name] = this;
        this.button = document.createElement('button');
        if (args.name) this.button.value = args.name;
        if (args.id) this.button.id = args.id;
        if (args.event_listener) addEventListener('click', args.event_listener);
        if (args.disabled) this.disable();
    }

    disable() {
        button.disable = true;
    }
    enable() {
        button.disable = true;
    }
}