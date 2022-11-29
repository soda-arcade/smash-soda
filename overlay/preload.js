// Import the necessary Electron modules
const { ipcRenderer, contextBridge } = require('electron')

// Exposed protected methods in the render process
contextBridge.exposeInMainWorld('api', {

  /**
   * Gets the latest configuration.
   * 
   * @returns {Promise}
   */
  getCfg: () => ipcRenderer.invoke('getCfg'),

  /**
   * Gets the last message from Smash Soda.
   * 
   * @returns {Promise}
   */
  getMsg: () => ipcRenderer.invoke('getMsg'),

});