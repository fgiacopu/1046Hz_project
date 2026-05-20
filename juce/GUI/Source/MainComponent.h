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

private:
    //==============================================================================
    // Your private member variables go here...
    juce::OSCReceiver receiver;
    juce::OSCSender sender;

    // Left Hand (Openness)
    juce::Slider leftHandKnob;
    juce::Label leftKnobLabel;

    // Right Hand (Chorus) - NEW
    juce::Slider rightHandKnob;
    juce::Label rightKnobLabel;

    // Left Thumb (Flanger)
    juce::Slider leftThumbSlider;
    juce::Label leftThumbLabel;

    // Right Thumb 
    juce::Slider rightThumbSlider;
    juce::Label rightThumbLabel;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};