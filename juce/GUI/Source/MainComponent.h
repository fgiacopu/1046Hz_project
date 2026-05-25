#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent,
    private juce::OSCReceiver::Listener<juce::OSCReceiver::MessageLoopCallback>
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void oscMessageReceived(const juce::OSCMessage& message) override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;
    class SpectrumComponent : public juce::Component,
        private juce::Timer
    {
    public:
        SpectrumComponent()
        {
            startTimerHz(30); // refresh 30 volte al secondo
        }

        void timerCallback() override
        {
            repaint();
        }

        void paint(juce::Graphics& g) override
        {
            g.fillAll(juce::Colour(0xff0f1115)); // sfondo scuro

            g.setColour(juce::Colour(0xff00e5ff)); // linea

            auto width = getWidth();
            auto height = getHeight();

            juce::Path spectrum;
            spectrum.startNewSubPath(0, height);

            for (int i = 0; i < width; ++i)
            {
                float cutoff = 0.5f; // per ora fisso

                float value = std::exp(-i * cutoff * 0.01f);
                float y = height - (value * 0.5f + 0.5f) * height;

                spectrum.lineTo(i, y);
            }

            g.strokePath(spectrum, juce::PathStrokeType(2.0f));
        }
    };

private:
    //==============================================================================
    juce::OSCReceiver receiver;
    juce::OSCSender sender;

    // Left Hand (Openness)
    juce::Slider leftHandKnob;
    juce::Label leftKnobLabel;

    // Right Hand (Chorus)
    juce::Slider rightHandKnob;
    juce::Label rightKnobLabel;

    // Left Thumb (Flanger)
    juce::Slider leftThumbSlider;
    juce::Label leftThumbLabel;

    // Right Thumb (????)
    juce::Slider rightThumbSlider;
    juce::Label rightThumbLabel;

    // Biomechanical Toggle Switches
    juce::TextButton leftHandToggle;
    juce::TextButton rightHandToggle;
    juce::TextButton leftThumbToggle;
    juce::TextButton rightThumbToggle;

    // ADSR Envelope Knobs
    juce::Slider attackKnob;
    juce::Label attackLabel;

    juce::Slider decayKnob;
    juce::Label decayLabel;

    juce::Slider sustainKnob;
    juce::Label sustainLabel;

    juce::Slider releaseKnob;
    juce::Label releaseLabel;

    SpectrumComponent spectrum;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};