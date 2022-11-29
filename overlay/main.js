/**
 * Import all the required Electron components
 */
const { app, BrowserWindow, ipcMain, globalShortcut } = require('electron')

/**
 * Import all the required Node.js components
 */
const path = require('path')
const http = require('http')
const parse = require('url')
const WebSocket = require('ws')

/**
 * The main application window
 */
let mainWindow;

/**
 * The overlay configuration
 */
let cfg = {

	// Websocket settings
  	websocketPort: 9002,
  	websocketHost: '127.0.0.1',
  	websocketPath: '/',
	websocketPassword: '',
	websocketMsg: null,
  
	// Overlay settings
	overlay: {
		opacity: 1,
		visible: true,
	},

	// Overlay windows/modules settings
	windows: {

		// Chat window
		chat: {
			visible: true,
			active: false,
			collection: [],
		},

	},

};

/**
 * Smash Soda debug messages
 */
const debugMessages = false;

/**
 * Allowed remote client addresses
 */
const allowedRemoteAddress = ['127.0.0.1'];

/**
 * Connected clients
 */
let connections = [];

/**
 * Smash Soda connection client
 */
let connectionId = 0;

/**
 * Last message received from Soda
 */
let sodaMsg = null;

/**
 * Allowed protocols for the websocket server
 */
const allowedProtocols = {
	soda: true,
	overlay: true
}

/**
 * Create the HTTP server
 */
const server = http.createServer(function(request, response) {
	console.log((new Date()) + ' Received request for ' + request.url);
	response.writeHead(404);
	response.end();
});

/**
 * Create the WebSocket server
 */
const wss = new WebSocket.Server({
	noServer: true
});

/**
 * Listen on the websocket port
 */
server.listen(cfg.websocketPort, cfg.websocketHost, function() {
	console.log((new Date()) + ' Server is listening on '+ cfg.websocketHost +":"+ cfg.websocketPort + cfg.websocketPath);
});

/**
 * Creates the application window and sets it
 * to always be on top of other windows
 */
function createWindow() {
	
  // Create the browser window.
  mainWindow = new BrowserWindow({
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      preload: path.join(__dirname, 'preload.js'),
      devTools: true
    },
    width: 1280,
    height: 720,
    fullscreen: true,
    transparent: true,
    frame: false,
    icon: 'img/icon.ico',
  })

  // and load the index.html of the app.
  mainWindow.loadFile('index.html')
    //.then(() => { mainWindow.webContents.openDevTools(); });

  // Always on top
  mainWindow.setPosition(0, 0)
  mainWindow.setAlwaysOnTop(true, "screen-saver")
  mainWindow.setVisibleOnAllWorkspaces(true)
  mainWindow.setFullScreenable(false)
  mainWindow.setMenuBarVisibility(false)
  mainWindow.maximize()
  mainWindow.show()
  mainWindow.setIgnoreMouseEvents(true)
  mainWindow.setKiosk(true)
  
}

/**
 * Registers all the shortcut keys
 */
function registerShortcuts() {

	// Toggle chat window interactivity
	globalShortcut.register('CommandOrControl+Shift+C', () => {

		if (cfg.windows.chat.visible == true) {
			cfg.windows.chat.active = !cfg.windows.chat.active;
			mainWindow.setIgnoreMouseEvents(!cfg.windows.chat.active);
		}

	});

	// Toggle chat window visibility
	globalShortcut.register('CommandOrControl+Shift+Alt+C', () => {

		cfg.windows.chat.visible = !cfg.windows.chat.visible;
		if (cfg.windows.chat.active) {
			cfg.windows.chat.active = false;
		}
			
	});

	// Overlay opacity up
	globalShortcut.register('CommandOrControl+Shift+Up', () => {

		if (cfg.overlay.opacity < 1) {
			cfg.overlay.opacity += 0.1;
		}

	});

	// Overlay opacity down
	globalShortcut.register('CommandOrControl+Shift+Down', () => {
		
		if (cfg.overlay.opacity > .2) {
			cfg.overlay.opacity -= 0.1;
		}

	});

}

/**
 * Sends a message to all connected clients
 * 
 * @param {Server} con			The websocket connection
 * @param {string} message		The message to send
 * @param {bool} invert 		Whether to invert the message
 */
function sendMessage(con, message, invert = false) {
	
    if (connections.length > 0) {
		for (const c of connections) {
			if (con.id != c.id && (
				(invert == true && con.connectionType == c.connectionType) ||
				(invert == false && con.connectionType != c.connectionType)
				)) {
				c.send(message);
			}
		}
	}
	
}

/**
 * Authenticates the client
 * 
 * @param {WebSocket} ws		The websocket connection
 * @param {string} data			The data to authenticate 
 * @param {bool} isBinary		Whether the data is binary 
 * 
 * @returns {bool} Whether the client is authenticated
 */
function authenticate(ws, data, isBinary) {

	if (!ws.authenticated) {

		let close = true;

		if (!isBinary) {

			const msg = JSON.parse(data.toString());

			if (msg.type == "identify") {
				
				if (msg.password == cfg.websocketPassword) {
					console.log(`(${ws.id}) Authenticated (${ws.remoteAddress})`);
					clearTimeout(ws.authenticationTimer);
					ws.authenticated = true;
					close = false;
					if (msg.userid) {
						ws.hostUser = {
							id: msg.id,
							userid: msg.userid,
							username: msg.username
						};
					}
					else {
						
						for (let i = connections.length - 1; i >= 0; i--) {
							if (!connections[i].hostUser) continue;
							let c = connections[i];
							ws.send(`{"type":"identify","id":${c.hostUser.id},"userid":${c.hostUser.userid},"username":"${c.hostUser.username}"}`);
						}

					}
				}

			}
		}

		// Close connection if not authenticated
		if (close) {
			console.log(`(${ws.id}) Incorrect Auth. Terminating connection (${ws.remoteAddress})`);
			clearTimeout(ws.authenticationTimer);
			ws.terminate();
		}
		return false;
	}
	return true;
}

/**
 * When a client disconnects.
 * 
 * @param {string} code		The close code
 * @param {string} reason	The close reason 
 */
function on_close(code, reason) {
    console.log(`(${this.id}) ${this.connectionType} disconnected (${this.remoteAddress})`);
    // sendMessage(this, `(${this.id}) ${this.connectionType} disconnected`);
    for (let i=0; i<connections.length; i++) {
        if (connections[i].id == this.id) {
            connections.splice(i, 1);
        }
    }
}

/**
 * On message sent
 * 
 * @param {string} data		The message data
 * @param {bool} isBinary 
 * @returns 
 */
function on_message(data, isBinary) {
	let auth = authenticate(this, data, isBinary);
	if (!this.authenticated && !auth) return;
	if (isBinary) return;
	const msg = data.toString();
	if (debugMessages) console.log(msg);
	sendMessage(this, msg);
	if (this.connectionType == "soda") {

		let jmsg = JSON.parse(msg);
		cfg.websocketMsg = jmsg;

		/*
		switch (jmsg.type) {
		
			case "chat":
				cfg.windows.chat.collection.push(jmsg);
				sendMessage(this, msg, true);
				//let c = connections[0];
				//this.send(`type: 'chat',id: ${c.hostUser.id},userid: ${c.hostUser.userid},username:"${c.hostUser.username}",content: "Received from overlay!"}`);
				break;
		}
		*/

	}
}

/**
 * When a client connects
 */
wss.on('connection', function connection(ws, request, connectionType) {
	ws.id = connectionId++;
	ws.connectionType = connectionType;
	ws.authenticated = false;
	ws.authenticationTimer = setTimeout(() => {
		console.log(`(${ws.id})Failed to Auth. Terminating connection (${ws.remoteAddress})`);
		ws.terminate();
	}, 3000);
	connections.push(ws);
	console.log(`(${ws.id}) ${ws.connectionType} connected (${ws.remoteAddress})`);
	ws.on("close", on_close);
	ws.on("message", on_message);
	// sendMessage(connectionType, `(${ws.id}) ${ws.connectionType} connected`, true);

});

/**
 * Upgrades the connection to a websocket
 */
server.on('upgrade', async function upgrade(request, socket, head) {
	let connectionType = "";
	if (request.url != cfg.websocketPath) {
		console.log(`Rejected connection. Path incorrect (${socket.remoteAddress})`);
		socket.destroy();
		return;
	}
	if (allowedRemoteAddress.indexOf(socket.remoteAddress) == -1) {
		console.log(`Rejected connection. IP not accepted (${socket.remoteAddress})`);
		socket.destroy();
		return;
	}
	if (!request.headers["sec-websocket-protocol"] || !allowedProtocols[request.headers["sec-websocket-protocol"]] ) {
		console.log(`Rejected connection. Protocol not accepted (${socket.remoteAddress})`);
		socket.destroy();
		return;
	}
	connectionType = request.headers["sec-websocket-protocol"];
	wss.handleUpgrade(request, socket, head, function done(ws) {
		ws.remoteAddress = socket.remoteAddress;
		wss.emit('connection', ws, request, connectionType);
	});
});

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {

	// Create the main application window
	createWindow();

	// Register all the shortcut keys
	registerShortcuts();

}).then(() => {

	app.on('activate', function () {
		// On macOS it's common to re-create a window in the app when the
		// dock icon is clicked and there are no other windows open.
		if (BrowserWindow.getAllWindows().length === 0) createWindow()
	});

})

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on('window-all-closed', function () {
	if (process.platform !== 'darwin') app.quit()
})

/**
 * Makes config accessible to the renderer
 */
ipcMain.handle('getCfg', async (event, arg) => {
	return cfg;
});

/**
 * 
 */
ipcMain.handle('sendMessage', async (event, arg) => {
	
	//let c = connections[0];
	//sendMessage(c, `{type: 'chat',id: ${c.hostUser.id},userid: ${c.hostUser.userid},username:"${c.hostUser.username}",content: "Did this work!?"}`, true);
	
});