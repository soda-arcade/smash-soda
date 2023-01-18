#include "Overlay.h"

/// <summary>
/// Launch the overlay
/// </summary>
void Overlay::start() {

	// Start overlay
	_isActive = true;

}

/// <summary>
/// Handles the overlay loop
/// </summary>
void Overlay::run() {

	// Is overlay active?
	if (_isActive) {

	}

}

/// <summary>
/// Stops the overlay.
/// </summary>
void Overlay::stop() {

	// Stop overlay
	if (_isActive) {

		// Kill process
		_tsystem(_T("taskkill /F /T /IM SmashSodaOverlay.exe"));

		// Stop loop
		_isActive = false;

	}

}