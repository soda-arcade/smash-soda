#include "AudioSettingsWidget.h"

AudioSettingsWidget::AudioSettingsWidget(Hosting& hosting)
    : _hosting(hosting), _audioIn(_hosting.audioIn), _audioOut(_hosting.audioOut),
    _inputs(_hosting.audioIn.listInputDevices()), _outputs(_hosting.audioOut.getDevices())
{
}

bool AudioSettingsWidget::render()
{
    static float indentSize = 0;
    static ImVec2 dummySize = ImVec2(0.0f, 20.0f);
    static ImVec2 cursor;

    AppStyle::pushTitle();

    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2(800, 900));
    ImGui::Begin("Audio");
    AppStyle::pushLabel();

    static ImVec2 size;
    static ImVec2 pos;
    size = ImGui::GetContentRegionAvail();
    pos = ImGui::GetWindowPos();

    if (!_hosting.isRunning() && _hosting.isReady())
    {
        if (Config::cfg.audio.micEnabled) _audioIn.captureAudio();
        _audioOut.captureAudio();
    }

    static float targetPreview;

    // =============================================================
    //  Input devices
    // =============================================================
    if (Config::cfg.audio.micEnabled)
    {
        static UINT& currentInputDevice = _audioIn.currentDevice.id;
        ImGui::SetNextItemWidth(size.x);

        AppFonts::pushTitle();
        AppColors::pushInput();
        ImGui::Text("Microphone");
        AppColors::pop();
        AppFonts::pop();

        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        AppStyle::pushLabel();
        ImGui::Text("Source");
        AppStyle::pop();
        AppFonts::pushInput();
        ImGui::SetNextItemWidth(size.x);
        if (ImGui::BeginCombo("##input selection", _audioIn.currentDevice.name.c_str()))
        {
            for (size_t i = 0; i < _inputs.size(); i++)
            {
                bool isSelected = (currentInputDevice == i);
                if (ImGui::Selectable(_inputs[i].name.c_str(), isSelected))
                {
                    currentInputDevice = i;
                    _audioIn.selectInputDevice(i);
                    Config::cfg.audio.inputDevice = i;
                    Config::cfg.Save();
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        AppFonts::pop();

        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        static int micFrequencyIndex = frequencyToCombo(_audioIn.getFrequency());
        AppStyle::pushLabel();
        ImGui::Text("Frequency");
        AppStyle::pop();
        AppStyle::pushInput();
        ImGui::SetNextItemWidth(size.x);
        if (ImGui::Combo("###Mic Frequency", &micFrequencyIndex, " 44100 Hz\0 48000 Hz\0\0", 2))
        {
            _audioIn.reinit(comboToFrequency(micFrequencyIndex));
        }
        AppStyle::pop();

        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        AppStyle::pushLabel();
        ImGui::Text("Volume");
        AppStyle::pop();

        static int micVolume;
        static float micPreview;
        micVolume = (int)(100.0f * _audioIn.volume);
        targetPreview = AudioTools::decibelToFloat(_audioIn.popPreviewDecibel());
        micPreview = lerp(micPreview, targetPreview, easing(targetPreview - micPreview));
        if (AudioControlWidget::render("Microphone##Audio In", &micVolume, _audioIn.isEnabled, micPreview, AppIcons::micOn, AppIcons::micOff))
        {
            _audioIn.isEnabled = !_audioIn.isEnabled;
        }
        _audioIn.volume = (float)micVolume / 100.0f;
        Config::cfg.audio.micVolume = (float)micVolume;


        ImGui::Dummy(dummySize);
    }

    // =============================================================
    //  Output devices
    // =============================================================
    AppFonts::pushTitle();
    AppColors::pushInput();
    ImGui::Text("Speakers");
    AppColors::pop();
    AppFonts::pop();

    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    
    AppStyle::pushLabel();
    ImGui::Text("Source");
    AppStyle::pop();

    static size_t& currentOutputDevice = _audioOut.currentDevice.index;
    ImGui::SetNextItemWidth(size.x);
    AppFonts::pushInput();
    if (ImGui::BeginCombo("##output selection", _audioOut.currentDevice.name.c_str()))
    {
        for (size_t i = 0; i < _outputs.size(); i++)
        {
            bool isSelected = (currentOutputDevice == i);
            if (ImGui::Selectable(_outputs[i].name.c_str(), isSelected))
            {
                currentOutputDevice = i;
                _audioOut.setOutputDevice(i);
                Config::cfg.audio.outputDevice = i;
                Config::cfg.Save();
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    AppFonts::pop();

    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    static int speakersFrequencyIndex = frequencyToCombo(_audioOut.getFrequency());
    AppStyle::pushLabel();
    ImGui::Text("Frequency");
    AppStyle::pop();
    AppStyle::pushInput();
    ImGui::SetNextItemWidth(size.x);
    if (ImGui::Combo("###Speakers Frequency", &speakersFrequencyIndex, " 44100 Hz\0 48000 Hz\0\0", 2))
    {
        _audioOut.setFrequency(comboToFrequency(speakersFrequencyIndex));
    }
    AppStyle::pop();

    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    AppStyle::pushLabel();
    ImGui::Text("Volume");
    AppStyle::pop();

    static int speakersVolume;
    static float speakersPreview;
    speakersVolume = (int)(100.0f *_audioOut.volume);
    targetPreview = AudioTools::decibelToFloat(_audioOut.popPreviewDecibel());
    speakersPreview = lerp(speakersPreview, targetPreview, easing(targetPreview - speakersPreview));
    if (AudioControlWidget::render("Speakers##Audio Out", &speakersVolume, _audioOut.isEnabled, speakersPreview, AppIcons::speakersOn, AppIcons::speakersOff))
    {
        _audioOut.isEnabled = !_audioOut.isEnabled;
    }
    _audioOut.volume = (float)speakersVolume / 100.0f;
    Config::cfg.audio.speakersVolume = (float)speakersVolume;

    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

    return true;
}

int AudioSettingsWidget::frequencyToCombo(Frequency frequency)
{
    switch (frequency)
    {
    case Frequency::F48000:
        return 1;
        break;
    case Frequency::F44100:
    default:
        return 0;
        break;
    }
    return 0;
}

Frequency AudioSettingsWidget::comboToFrequency(int index)
{
    switch (index)
    {
    case 1:
        return Frequency::F48000;
        break;
    default:
    case 0:
        return Frequency::F44100;
        break;
    }
    return Frequency::F44100;
}

float AudioSettingsWidget::lerp(float val1, float val2, float t)
{
    return (1-t) * val1 + t * val2;
}

float AudioSettingsWidget::easing(float t)
{
    return 0.3f * max(abs(t), 0.4f);
}
