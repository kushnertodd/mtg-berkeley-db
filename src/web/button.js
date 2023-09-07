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
        this.button.style.width = "60px";
        this.button.style.height = "50px";
        this.button.style.margin = "4px";
        //this.button.style.fontWeight="bold";
        this.button.style.fontFamily = "Verdana,Arial,sans-serif";
        if (args.name) this.button.innerHTML = args.name;
        if (args.name) this.button.value = args.name;
        if (args.id) this.button.id = args.id;
        if (args.event_listener) this.button.addEventListener('click', args.event_listener);
        if (args.disabled) this.disable();
    }

    disable() {
        this.button.disable = true;
        this.button.style.color = "#b0b0b0";
    }

    enable() {
        this.button.disable = true;
        this.button.style.color = "#000000";
    }
}