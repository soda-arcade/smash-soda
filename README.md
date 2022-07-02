"Master" branch is just a copy of https://github.com/FlavioFS/ParsecSoda except for this readme. See the other branches if you're looking for changes.

<br>

This is my personal version that I use to host my games. I know very little C++ and I am still learning. I will make a lot bad decisions intentionally and a lot of mistakes unintentionally.

This is why I not make any pull request or provide code for the official version by Flavio.

<br>

### Noteworthy changes in SodaV compared to Soda

#### Removed
- Kick/ban sounds
- Taskbar flashing

#### User Interface
- Dark color scheme
- Some smaller UI elements, less padding and dead space, and overall smaller windows
- Labels/description text removed

#### Features
- Metrics on guest list
  - Colors (1 retransmit in 2 seconds is nothing. Make sure to check how much N: numbers are going up)
    - Green = No retransmits in last 2 sec
    - Yellow = At least 1 fast retransmit in the last 2 sec
    - Red = At least 1 slow retransmit in the last 2 sec
  - Stats
    - Network latency ms
    - B: Bandwidth - Mbps (Same for all but measured every 60 sec)
    - D: Decode - Queued frames/Decode latency ms
    - E: Encode - Host encode latency ms
    - N: Network - Slow retransmits/Fast retransmits
- Add ban reason on Blocked list and new filter
- Button lock
- Settings
  - Disable !sfx & !bonk (Basic version)
  - Disable Microphone (Win 11)
  - Disable Guide button
  - Disable Keyboards (Keyboard->Gamepad conversion)
- WebSocket client
  - Example use https://github.com/v6ooo/parsec-soda-overlay
- Portable mode
  - Create portable.txt
- Force selects chat box input when in background
  - For use with https://www.notion.so/v6ooo/Parsec-Soda-Access-chat-without-alt-tab-2d55f5c91b2548d2889acfdf71ebfa4b
  <br> or https://github.com/v6ooo/parsec-soda-overlay
  
#### Attempted bugfixes for
- Gamepad crash on launch
- Chat & Log crash when pruning messages
- Minimize over 9000 fps
- Public thumbnails window crash
- Incorrect Debouncer release crash
- Software audio (E.g. VB-Cable) crash
- Reset Gamepads button crash when pressing more than once
- No gamepad limit (locked to 32 Xbox & 32 DS4)
