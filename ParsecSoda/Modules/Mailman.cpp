#include "Mailman.h"

/// <summary>
/// GET request
/// </summary>
/// <param name="url"></param>
/// <returns></returns>
future<string> Mailman::GET(string url) {
    return async(launch::async, [url]() {

        // Get protocol from URL
		const string protocol = url.substr(0, url.find("://"));

		// Remove protocol from URL
		string _url = url.substr(url.find("://") + 3);

		// Get domain from URL
		const string domain = _url.substr(0, _url.find("/"));

		// Get path after domain
		string path = _url.substr(_url.find("/"));

		// Is protocol HTTPS or HTTP?
		const bool isHttps = protocol == "https";

        // Response
        void* response = nullptr;
        size_t responseSize = 0;
        uint16_t status = 0;

		string result = "";

        const bool rekt = MTY_HttpRequest(
            domain.c_str(), 0, isHttps, "GET", path.c_str(), "Content-Type: application/json",
            NULL, 0,
            5000,
            &response, &responseSize, &status
        );

        if (responseSize > 0 && status == 200) {
            string responseStr = (const char*)response;
			result = responseStr;
        }

		return result;
		
    });
}

/// <summary>
/// POST request
/// </summary>
/// <param name="url"></param>
/// <param name="data"></param>
/// <returns></returns>
future<string> Mailman::POST(string url, string data) {

	return async(launch::async, [url, data]() {

        // Get protocol from URL
        const string protocol = url.substr(0, url.find("://"));

        // Remove protocol from URL
        string _url = url.substr(url.find("://") + 3);

        // Get domain from URL
        const string domain = _url.substr(0, _url.find("/"));

        // Get path after domain
        string path = _url.substr(_url.find("/"));

        // Is protocol HTTPS or HTTP?
        const bool isHttps = protocol == "https";

		size_t bodySize = sizeof(char) * data.length();

        // Response
        void* response = nullptr;
        size_t responseSize = 0;
        uint16_t status = 0;

        string result = "";

        const bool rekt = MTY_HttpRequest(
			domain.c_str(), 0, isHttps, "POST", path.c_str(), "Content-Type: application/json",
			data.c_str(), bodySize,
            5000,
            &response, &responseSize, &status
        );

        if (responseSize > 0 && status == 200) {
            string responseStr = (const char*)response;
            result = responseStr;
        }

        return result;

	});

}