#include "AppIcons.h"


ID3D11ShaderResourceView* AppIcons::play;
ID3D11ShaderResourceView* AppIcons::stop;

ID3D11ShaderResourceView* AppIcons::yes;
ID3D11ShaderResourceView* AppIcons::no;

ID3D11ShaderResourceView* AppIcons::send;
ID3D11ShaderResourceView* AppIcons::back;

ID3D11ShaderResourceView* AppIcons::lock;
ID3D11ShaderResourceView* AppIcons::unlock;

ID3D11ShaderResourceView* AppIcons::padOn;
ID3D11ShaderResourceView* AppIcons::padOff;
ID3D11ShaderResourceView* AppIcons::plug;

ID3D11ShaderResourceView* AppIcons::micOn;
ID3D11ShaderResourceView* AppIcons::micOff;
ID3D11ShaderResourceView* AppIcons::speakersOn;
ID3D11ShaderResourceView* AppIcons::speakersOff;

ID3D11ShaderResourceView* AppIcons::video;

ID3D11ShaderResourceView* AppIcons::refresh;
ID3D11ShaderResourceView* AppIcons::sort;
ID3D11ShaderResourceView* AppIcons::submit;
ID3D11ShaderResourceView* AppIcons::saveOn;
ID3D11ShaderResourceView* AppIcons::saveOff;
ID3D11ShaderResourceView* AppIcons::edit;
ID3D11ShaderResourceView* AppIcons::image;
ID3D11ShaderResourceView* AppIcons::trash;
ID3D11ShaderResourceView* AppIcons::move;

ID3D11ShaderResourceView* AppIcons::chat;
ID3D11ShaderResourceView* AppIcons::users;
ID3D11ShaderResourceView* AppIcons::log;
ID3D11ShaderResourceView* AppIcons::logoff;

ID3D11ShaderResourceView* AppIcons::userOn;
ID3D11ShaderResourceView* AppIcons::userOff;
ID3D11ShaderResourceView* AppIcons::block;
ID3D11ShaderResourceView* AppIcons::kick;

ID3D11ShaderResourceView* AppIcons::info;
ID3D11ShaderResourceView* AppIcons::keymap; //-- CodeSomnia Add--
ID3D11ShaderResourceView* AppIcons::logo;

ID3D11ShaderResourceView* AppIcons::puppet;
ID3D11ShaderResourceView* AppIcons::xinput;
ID3D11ShaderResourceView* AppIcons::dinput;
ID3D11ShaderResourceView* AppIcons::ds4;
ID3D11ShaderResourceView* AppIcons::windows;
ID3D11ShaderResourceView* AppIcons::sdl;

ID3D11ShaderResourceView* AppIcons::xbox;
ID3D11ShaderResourceView* AppIcons::xbox1;
ID3D11ShaderResourceView* AppIcons::xbox2;
ID3D11ShaderResourceView* AppIcons::xbox3;
ID3D11ShaderResourceView* AppIcons::xbox4;

ID3D11ShaderResourceView* AppIcons::vpad;

ID3D11ShaderResourceView* AppIcons::btc;
ID3D11ShaderResourceView* AppIcons::ltc;
ID3D11ShaderResourceView* AppIcons::eth;

ID3D11ShaderResourceView* AppIcons::settings;
ID3D11ShaderResourceView* AppIcons::websocket;
ID3D11ShaderResourceView* AppIcons::editReason;
ID3D11ShaderResourceView* AppIcons::buttonLock;

ID3D11ShaderResourceView* AppIcons::library;
ID3D11ShaderResourceView* AppIcons::overlay;

ID3D11ShaderResourceView* AppIcons::hotseat;
ID3D11ShaderResourceView* AppIcons::tournament;

ID3D11ShaderResourceView* AppIcons::crown;
ID3D11ShaderResourceView* AppIcons::star;
ID3D11ShaderResourceView* AppIcons::eye;

Texture AppIcons::_play;
Texture AppIcons::_stop;

Texture AppIcons::_yes;
Texture AppIcons::_no;

Texture AppIcons::_send;
Texture AppIcons::_back;

Texture AppIcons::_padOn;
Texture AppIcons::_padOff;
Texture AppIcons::_plug;

Texture AppIcons::_lock;
Texture AppIcons::_unlock;

Texture AppIcons::_micOn;
Texture AppIcons::_micOff;
Texture AppIcons::_speakersOn;
Texture AppIcons::_speakersOff;

Texture AppIcons::_video;

Texture AppIcons::_refresh;
Texture AppIcons::_sort;
Texture AppIcons::_submit;
Texture AppIcons::_saveOn;
Texture AppIcons::_saveOff;
Texture AppIcons::_edit;
Texture AppIcons::_image;
Texture AppIcons::_trash;
Texture AppIcons::_move;

Texture AppIcons::_chat;
Texture AppIcons::_users;
Texture AppIcons::_log;
Texture AppIcons::_logoff;

Texture AppIcons::_userOn;
Texture AppIcons::_userOff;
Texture AppIcons::_block;
Texture AppIcons::_kick;

Texture AppIcons::_keymap;//-- CodeSomnia Add--
Texture AppIcons::_info;
Texture AppIcons::_logo;

Texture AppIcons::_puppet;
Texture AppIcons::_xinput;
Texture AppIcons::_dinput;
Texture AppIcons::_ds4;
Texture AppIcons::_windows;
Texture AppIcons::_sdl;

Texture AppIcons::_xbox;
Texture AppIcons::_xbox1;
Texture AppIcons::_xbox2;
Texture AppIcons::_xbox3;
Texture AppIcons::_xbox4;

Texture AppIcons::_vpad;

Texture AppIcons::_btc;
Texture AppIcons::_ltc;
Texture AppIcons::_eth;

Texture AppIcons::_settings;
Texture AppIcons::_websocket;
Texture AppIcons::_editReason;
Texture AppIcons::_buttonLock;

Texture AppIcons::_library;
Texture AppIcons::_overlay;

Texture AppIcons::_hotseat;
Texture AppIcons::_tournament;

Texture AppIcons::_crown;
Texture AppIcons::_star;
Texture AppIcons::_eye;

void AppIcons::init(ID3D11Device* pd3dDevice)
{
    _play.loadFromFile(pd3dDevice, "./Icons/play.png");
    _stop.loadFromFile(pd3dDevice, "./Icons/stop.png");

    _yes.loadFromFile(pd3dDevice, "./Icons/check-on.png");
    _no.loadFromFile(pd3dDevice, "./Icons/close-circle.png");

    _lock.loadFromFile(pd3dDevice, "./Icons/lock.png");
    _unlock.loadFromFile(pd3dDevice, "./Icons/unlock.png");

    _padOn.loadFromFile(pd3dDevice, "./Icons/pad-on.png");
    _padOff.loadFromFile(pd3dDevice, "./Icons/pad-off.png");
    _plug.loadFromFile(pd3dDevice, "./Icons/plug.png");

    _send.loadFromFile(pd3dDevice, "./Icons/send.png");
    _back.loadFromFile(pd3dDevice, "./Icons/back.png");

    _micOn.loadFromFile(pd3dDevice, "./Icons/microphone-on.png");
    _micOff.loadFromFile(pd3dDevice, "./Icons/microphone-off.png");
    _speakersOn.loadFromFile(pd3dDevice, "./Icons/speakers-on.png");
    _speakersOff.loadFromFile(pd3dDevice, "./Icons/speakers-off.png");

    _video.loadFromFile(pd3dDevice, "./Icons/video.png");

    _refresh.loadFromFile(pd3dDevice, "./Icons/refresh.png");
    _sort.loadFromFile(pd3dDevice, "./Icons/sort.png");
    _submit.loadFromFile(pd3dDevice, "./Icons/submit.png");
    _saveOn.loadFromFile(pd3dDevice, "./Icons/save-on.png");
    _saveOff.loadFromFile(pd3dDevice, "./Icons/save-off.png");
    _edit.loadFromFile(pd3dDevice, "./Icons/edit.png");
    _image.loadFromFile(pd3dDevice, "./Icons/image.png");
    _trash.loadFromFile(pd3dDevice, "./Icons/trash.png");
    _move.loadFromFile(pd3dDevice, "./Icons/move.png");

    _chat.loadFromFile(pd3dDevice, "./Icons/chat.png");
    _users.loadFromFile(pd3dDevice, "./Icons/users.png");
    _log.loadFromFile(pd3dDevice, "./Icons/log.png");
    _logoff.loadFromFile(pd3dDevice, "./Icons/logoff.png");

    _userOn.loadFromFile(pd3dDevice, "./Icons/user-on.png");
    _userOff.loadFromFile(pd3dDevice, "./Icons/user-off.png");
    _block.loadFromFile(pd3dDevice, "./Icons/block.png");
    _kick.loadFromFile(pd3dDevice, "./Icons/kick.png");

    _info.loadFromFile(pd3dDevice, "./Icons/info.png");
    _keymap.loadFromFile(pd3dDevice, "./icons/keymap.png");//-- CodeSomnia Add--
    _logo.loadFromFile(pd3dDevice, "./Icons/logo.png");

    _puppet.loadFromFile(pd3dDevice, "./Icons/puppet.png");
    _xinput.loadFromFile(pd3dDevice, "./Icons/xinput.png");
    _dinput.loadFromFile(pd3dDevice, "./Icons/dinput.png");
    _ds4.loadFromFile(pd3dDevice, "./Icons/ds4.png");
    _windows.loadFromFile(pd3dDevice, "./Icons/windows.png");
    _sdl.loadFromFile(pd3dDevice, "./Icons/sdl.png");

    _xbox.loadFromFile(pd3dDevice, "./Icons/xbox.png");
    _xbox1.loadFromFile(pd3dDevice, "./Icons/xbox1.png");
    _xbox2.loadFromFile(pd3dDevice, "./Icons/xbox2.png");
    _xbox3.loadFromFile(pd3dDevice, "./Icons/xbox3.png");
    _xbox4.loadFromFile(pd3dDevice, "./Icons/xbox4.png");

    _vpad.loadFromFile(pd3dDevice, "./Icons/vpad.png");

    _btc.loadFromFile(pd3dDevice, "./Icons/btc.png");
    _ltc.loadFromFile(pd3dDevice, "./Icons/ltc.png");
    _eth.loadFromFile(pd3dDevice, "./Icons/eth.png");

    _settings.loadFromFile(pd3dDevice, "./Icons/settings.png");
    _websocket.loadFromFile(pd3dDevice, "./Icons/websocket.png");
    _editReason.loadFromFile(pd3dDevice, "./Icons/editreason.png");
    _buttonLock.loadFromFile(pd3dDevice, "./Icons/buttonlock.png");

    _library.loadFromFile(pd3dDevice, "./Icons/library.png");
    _overlay.loadFromFile(pd3dDevice, "./Icons/overlay.png");

    _hotseat.loadFromFile(pd3dDevice, "./Icons/hotseat.png");
    _tournament.loadFromFile(pd3dDevice, "./Icons/tournament.png");

	_crown.loadFromFile(pd3dDevice, "./Icons/crown.png");
	_star.loadFromFile(pd3dDevice, "./Icons/star.png");
	_eye.loadFromFile(pd3dDevice, "./Icons/eye.png");

    play = _play.texture;
    stop = _stop.texture;
    
    yes = _yes.texture;
    no = _no.texture;
    
    lock = _lock.texture;
    unlock = _unlock.texture;

    padOn = _padOn.texture;
    padOff = _padOff.texture;
    plug = _plug.texture;
    
    send = _send.texture;
    back = _back.texture;
    
    micOn = _micOn.texture;
    micOff = _micOff.texture;
    speakersOn = _speakersOn.texture;
    speakersOff = _speakersOff.texture;
    
    video = _video.texture;

    refresh = _refresh.texture;
    sort = _sort.texture;
    submit = _submit.texture;
    saveOn = _saveOn.texture;
    saveOff = _saveOff.texture;
    edit = _edit.texture;
    image = _image.texture;
    trash = _trash.texture;
    move = _move.texture;

    chat = _chat.texture;
    users = _users.texture;
    log = _log.texture;
    logoff = _logoff.texture;
    block = _block.texture;
    kick = _kick.texture;

    userOn = _userOn.texture;
    userOff = _userOff.texture;

    info = _info.texture;
    keymap = _keymap.texture;//-- CodeSomnia Add--
    logo = _logo.texture;

    puppet = _puppet.texture;
    xinput = _xinput.texture;
    dinput = _dinput.texture;
    ds4 = _ds4.texture;
    windows = _windows.texture;
    sdl = _sdl.texture;

    xbox = _xbox.texture;
    xbox1 = _xbox1.texture;
    xbox2 = _xbox2.texture;
    xbox3 = _xbox3.texture;
    xbox4 = _xbox4.texture;

    vpad = _vpad.texture;

    btc = _btc.texture;
    ltc = _ltc.texture;
    eth = _eth.texture;
    
    settings = _settings.texture;
    websocket = _websocket.texture;
    editReason = _editReason.texture;
    buttonLock = _buttonLock.texture;

    library = _library.texture;
    overlay = _overlay.texture;

    hotseat = _hotseat.texture;
    tournament = _tournament.texture;

	crown = _crown.texture;
	star = _star.texture;
	eye = _eye.texture;
}
