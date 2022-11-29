/**
 * Handles all the appbar stuff.
 *
 * @link       https://mickeyuk.github.io
 * @since      1.0.0
 */
function appBar() { }

/**
 * Holds all the section objects.
 */
appBar.sections = [];

/**
 * 
 */
appBar.update = function () {
    
}

/**
 * Adds a section to the appbar.
 * 
 * @param {object} section  The section object. 
 */
appBar.addSection = function (section) {

    // Abort if element for section already exists
    if (document.querySelector(`[data-id="${section.id}"]`)) return;

    // Create new element for the section
    let sectionElement = document.createElement("div");
    sectionElement.classList.add("app-bar-section");
    sectionElement.setAttribute("data-id", section.id);
    section.element = sectionElement;

    // Add the section to the app bar
    document.getElementById("app-bar-row").appendChild(sectionElement);

    // For each data item in the section
    for (let i = 0; i < section.data.length; i++) {

        // Create new element for the data item
        let dataElement = document.createElement("div");
        dataElement.classList.add("app-bar-col");

        // Add attribute to the data element
        dataElement.setAttribute("data-id", section.data[i].id);

        // Add the data element to the section element
        section.element.appendChild(dataElement);

        // Create data label element
        let dataLabel = document.createElement("div");
        dataLabel.classList.add("app-bar-label");
        dataLabel.innerHTML = section.data[i].label;

        // Add the data label to the data element
        dataElement.appendChild(dataLabel);

        // Create data value element
        let dataValue = document.createElement("div");
        dataValue.classList.add("app-bar-value");

        // Add the data value to the data element
        dataElement.appendChild(dataValue);

        // Update the data value
        appBar.setData(section.data[i], dataValue);

    }

    // Add the section to the sections array
    this.sections.push(section);

}

/**
 * Adds a data item to the app bar.
 * 
 * @param {string} sectionId    The section ID.
 * @param {object} data         The data item object. 
 */
appBar.addData = function (sectionId, data) {

    // Get the section
    let section = this.getSection(sectionId);

    // Add the data to the section
    section.data.push(data);

    // Create new element for the data item
    let dataElement = document.createElement("div");
    dataElement.classList.add("app-bar-col");

    // Add attribute to the data element
    dataElement.setAttribute("data-id", data.id);

    // Add the data element to the section element
    section.element.appendChild(dataElement);

    // Create data label element
    let dataLabel = document.createElement("div");
    dataLabel.classList.add("app-bar-label");
    dataLabel.innerHTML = section.data[i].label;

    // Add the data label to the data element
    dataElement.appendChild(dataLabel);

    // Create data value element
    let dataValue = document.createElement("div");

    // Add the data value to the data element
    dataElement.appendChild(dataValue);
    dataValue.classList.add("app-bar-value");

    // Update the data value
    appBar.setData(data, dataValue);

}

/**
 * Sets the value of a data item on the app bar.
 * 
 * @param {object} data             The data item object. 
 * @param {HTMLElement} element     The data item element.
 */
appBar.setData = function (data, element) {

    // Data type
    switch (data.type) {
                
        // Timer
        case "timer":

            // Create interval timer
            let timer = setInterval(function () {

                // Convert milliseconds to MM:SS format
                let time = helper.msToTime(data.value);

                // Decrement the value
                data.value -= 1000;

                // Reached zero?
                if (data.value <= -1) {

                    // Clear the interval
                    clearInterval(timer);

                    // Remove the timer from the timers array
                    app.timers.splice(app.timers.indexOf(timer), 1);

                }

                // Update the app bar
                element.innerHTML = time;

            }, 1000);

            // Add timer to the timers array
            app.timers.push(timer);
            
            break;
        
        // Default
        default:
            element.innerHTML = data.value;
            break;

    }

}

/**
 * Updates the value of a data item on the app bar.
 * 
 * @param {string} sectionId        The section ID.
 * @param {string} dataId           The data item ID. 
 * @param {string} value            The new value. 
 */
appBar.updateData = function (sectionId, dataId, value) {

    // Find the section
    let section = this.sections.find(section => section.id === sectionId);

    // Find the data item
    let data = section.data.find(data => data.id === dataId);

    // Update the data value
    data.value = value;

    // Update the app bar
    appBar.setValue(`[data-id="${sectionId}"] [data-id="${dataId}"] .app-bar-value`, value);

}

/**
 * Remove some data from the app bar section.
 * 
 * @param {string} sectionId    The section ID.
 * @param {string} dataId       The data item ID.
 */
appBar.removeData = function (sectionId, dataId) {

    // Find the section
    let section = this.sections.find(section => section.id === sectionId);

    // Find the data item
    let data = section.data.find(data => data.id === dataId);

    // Remove the data item from the section
    section.data.splice(section.data.indexOf(data), 1);

    // Remove the data item from the app bar
    document.querySelector(`[data-id="${sectionId}"] [data-id="${dataId}"]`).remove();

}

/**
 * Removes a section from the app bar.
 * 
 * @param {string} sectionId    The section ID.
 */
appBar.removeSection = function (sectionId) {

    // Find the section
    let section = this.sections.find(section => section.id === sectionId);

    // Remove the section from the app bar
    section.element.remove();

    // Remove the section from the sections array
    this.sections.splice(this.sections.indexOf(section), 1);

}