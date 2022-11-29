/**
 * Helper functions
 *
 * @link       https://mickeyuk.github.io
 * @since      1.0.0
 */
function helper() { }

/**
 * Converts milliseconds to a MM::SS format.
 * 
 * @param {int} duration    The duration in milliseconds. 
 * 
 * @returns {string}        The duration in MM::SS format. 
 */
helper.msToTime = function (duration) {

    // Convert milliseconds to minutes
    let minutes = Math.floor((duration / (1000 * 60)) % 60);

    // Convert milliseconds to seconds
    let seconds = parseInt((duration / 1000) % 60);

    // Return the time in the format mm:ss
    return minutes + ":" + (seconds < 10 ? "0" : "") + seconds;

};