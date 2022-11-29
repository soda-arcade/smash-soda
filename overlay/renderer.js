let cfg;

/**
 * This updates the overlay every 500ms
 */
function update() {

    // Get current configuration
    api.getCfg().then((data) => {

        // Set the configuration
        cfg = data;

        // Set overlay opacity
        document.getElementById("overlay").style.opacity = cfg.overlay.opacity;

        // If host toggles chat window
        toggleChatWindow();

        // Update the chat log
        updateChatLog();

    });

}
setInterval(update, 100);

/**
 * Handles the chat window toggling
 */
function toggleChatWindow() {

    var window = document.getElementById("chat-window");

    // Toggle chat window visibility
    if (!cfg.windows.chat.visible) {
        window.classList.add("hidden");
    } else {
        window.classList.remove("hidden");
    }

    // Toggle chat window interaction
    if (cfg.windows.chat.visible) {

        if (cfg.windows.chat.active) {

            // Show the chat window
            window.classList.add("active");

            // Select the input field for chat
            document.getElementById("chat-input").focus();

        } else {

            // Hide the chat window
            window.classList.remove("active");

            // Unselect the input field for chat
            document.getElementById("chat-input").blur();

        }
    }

}

/**
 * Handles the chat log.
 */
function updateChatLog() {

    // Get the chat log
    let chatLog = document.getElementById("chat-log");

    // Get latest chat message
    if (chatLog.children.length < cfg.windows.chat.collection.length) {

        // Get the latest message
        var message = cfg.windows.chat.collection[cfg.windows.chat.collection.length - 1];

        // Create chat message element
        let chatMsg = document.createElement("div");
        chatMsg.classList.add("chat-msg");

        // Create chat message username element
        let chatMsgUser = document.createElement("div");
        chatMsgUser.classList.add("chat-msg__name");
        chatMsgUser.innerHTML = message.username;
        chatMsg.appendChild(chatMsgUser);

        // Create chat message text element
        let chatMsgText = document.createElement("div");
        chatMsgText.classList.add("chat-msg__text");
        chatMsgText.innerHTML = message.content;
        chatMsg.appendChild(chatMsgText);

        // Add chat message to chat log
        document.getElementById("chat-log").appendChild(chatMsg);

    }

    // Smooth scroll to bottom of chat log
    document.getElementById("chat-log").scrollTo({ top: document.getElementById("chat-log").scrollHeight, behavior: 'smooth' })

}