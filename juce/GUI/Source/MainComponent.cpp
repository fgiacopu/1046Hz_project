#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    //User Iterface Setup knob
    addAndMakeVisible(handKnob);
    handKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    handKnob.setRange(0.0, 1.0);
    handKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    handKnob.setInterceptsMouseClicks(false, false); //so the knob can't be moved with the mouse

    addAndMakeVisible(knobLabel);
    knobLabel.setText("Hand Openness", juce::dontSendNotification);
    knobLabel.setJustificationType(juce::Justification::centred);
    
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);

        receiver.connect(9000); //Listen to Python
        receiver.addListener(this); //This class handles the data
        sender.connect("127.0.0.1", 57130); //Forwording to SC
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

void MainComponent::oscMessageReceived(const juce::OSCMessage& message)
{
    if (message.getAddressPattern() == "/hand/left/open")
    {
        if (message.size() > 0 && message[0].isFloat32())
        {
            float val = message[0].getFloat32();
            sender.send("/hand/left/open", val);
            // Update UI knob safely on the main Thread
            juce::MessageManager::callAsync([this, val]() {
                handKnob.setValue(val, juce::dontSendNotification); //preventing the app from crashing when network data tries to move a UI knob
                });
        }

    }
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    auto area = getLocalBounds();
    handKnob.setBounds(area.withSizeKeepingCentre(250, 250));
    knobLabel.setBounds(handKnob.getX(), handKnob.getBottom() - 10, handKnob.getWidth(), 25);
}
