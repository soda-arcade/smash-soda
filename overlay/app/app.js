/**
 * Smash Soda overlay
 *
 * @link       https://mickeyuk.github.io
 * @since      1.0.0
 */
function app() { }

/**
 * Default configuration.
 */
app.default = {

};

/**
 * Overlay configuration.
 */
app.cfg = { ...app.default };

/**
 * Stores all the interval timers.
 */
app.timers = [];

/**
 * Initializes the app.
 */
app.init = function () {
    
    

};

/**
 * Main app loop.
 */
app.update = function () {

    // Get current configuration
    api.getCfg().then((data) => {

        // Update the configuration
        app.cfg = { ...app.cfg, ...data };

        // Set overlay opacity
        document.getElementById("app").style.opacity = app.cfg.overlay.opacity;

        // Handle websocket messages
        if (app.cfg.websocketMsg) {
            app.handleMsg(app.cfg.websocketMsg);
            app.cfg.websocketMsg = null;
        }

    });

};

app.handleMsg = function (msg) {

    switch (msg.type) {

        case 'appbar-add-section':

            
            
            break;

    }

};

// Start the app
setInterval(app.update, 500);

appBar.addSection({
    id: 'appbar-section-1',
    data: [
        {
            id: 'appbar-section-1-item-1',
            label: "Hotseat",
            value: "Enabled"
        },
        {
            id: 'appbar-section-1-item-2',
            label: "Playing",
            value: "MickeyUK"
        },
        {
            id: 'appbar-section-1-item-3',
            label: "Next",
            value: "bigboi83"
        },
        {
            id: 'appbar-section-1-item-3',
            label: "Time",
            type: "timer",
            value: 30000
        },
    ]
});