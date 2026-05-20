#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // === LEFT HAND ===
    addAndMakeVisible(leftHandKnob);
    leftHandKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftHandKnob.setRange(0.0, 1.0);
    leftHandKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    leftHandKnob.setInterceptsMouseClicks(false, false);

    // Turkish Blue Colors
    leftHandKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff00a8b5)); // Vivid Turkish Teal
    leftHandKnob.setColour(juce::Slider::thumbColourId, juce::Colour(0xff77e4d4));             // Soft Pale Turquoise
    leftHandKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xff77e4d4));
    leftHandKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(leftKnobLabel);
    leftKnobLabel.setText("Left Hand Openness", juce::dontSendNotification);
    leftKnobLabel.setJustificationType(juce::Justification::centred);
    leftKnobLabel.setColour(juce::Label::textColourId, juce::Colour(0xff77e4d4));

    addAndMakeVisible(leftThumbSlider);
    leftThumbSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    leftThumbSlider.setRange(0.0, 1.0);
    leftThumbSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    leftThumbSlider.setInterceptsMouseClicks(false, false);

    // Turkish Blue Slider Colors
    leftThumbSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff00a8b5));
    leftThumbSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff00a8b5).withAlpha(0.2f));
    leftThumbSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xff77e4d4));
    leftThumbSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(leftThumbLabel);
    leftThumbLabel.setText("Left Thumb Movement", juce::dontSendNotification);
    leftThumbLabel.setJustificationType(juce::Justification::centred);
    leftThumbLabel.setColour(juce::Label::textColourId, juce::Colour(0xff77e4d4));


    // === RIGHT HAND CONFIGURATION ===
    addAndMakeVisible(rightHandKnob);
    rightHandKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightHandKnob.setRange(0.0, 1.0);
    rightHandKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    rightHandKnob.setInterceptsMouseClicks(false, false);

    // Soft Dark Green Colors
    rightHandKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff4a7c59)); // Matte Sage Green
    rightHandKnob.setColour(juce::Slider::thumbColourId, juce::Colour(0xff9cb380));             // Soft Moss highlight
    rightHandKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xff9cb380));
    rightHandKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(rightKnobLabel);
    rightKnobLabel.setText("Right Hand (Chorus)", juce::dontSendNotification);
    rightKnobLabel.setJustificationType(juce::Justification::centred);
    rightKnobLabel.setColour(juce::Label::textColourId, juce::Colour(0xff9cb380));

    addAndMakeVisible(rightThumbSlider);
    rightThumbSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    rightThumbSlider.setRange(0.0, 1.0);
    rightThumbSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    rightThumbSlider.setInterceptsMouseClicks(false, false);

    // Soft Dark Green Slider Colors
    rightThumbSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff4a7c59));
    rightThumbSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff4a7c59).withAlpha(0.2f));
    rightThumbSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xff9cb380));
    rightThumbSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(rightThumbLabel);
    rightThumbLabel.setText("Right Thumb Movement", juce::dontSendNotification);
    rightThumbLabel.setJustificationType(juce::Justification::centred);
    rightThumbLabel.setColour(juce::Label::textColourId, juce::Colour(0xff9cb380));
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);

        receiver.connect(9000); //Listen to Python
        receiver.addListener(this); //This class handles the data
        sender.connect("127.0.0.1", 57120); //Forwording to SC
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
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
    auto pattern = message.getAddressPattern().toString();

    if (message.size() > 0 && message[0].isFloat32())
    {
        float val = message[0].getFloat32();
        sender.send(pattern, val);
        // Update UI knob safely on the main Thread
        juce::MessageManager::callAsync([this, val, pattern]() {
            if (pattern == "/hand/left/open")
                leftHandKnob.setValue(val, juce::dontSendNotification);
            else if (pattern == "/hand/right/open")
                rightHandKnob.setValue(val, juce::dontSendNotification);
            else if (pattern == "/hand/left/thumb")
                leftThumbSlider.setValue(val, juce::dontSendNotification);
            else if (pattern == "/hand/right/thumb")
                rightThumbSlider.setValue(val, juce::dontSendNotification);
            });

    }
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    auto area = getLocalBounds().reduced(20);

    // Split layout cleanly into Left and Right columns
    auto leftColumn = area.removeFromLeft(area.getWidth() * 0.5);
    auto rightColumn = area; // The remaining right-hand side

    // --- Left Column Layout ---
    auto leftTopArea = leftColumn.removeFromTop(leftColumn.getHeight() * 0.5);
    leftHandKnob.setBounds(leftTopArea.withSizeKeepingCentre(200, 200));
    leftKnobLabel.setBounds(leftHandKnob.getX(), leftHandKnob.getBottom(), leftHandKnob.getWidth(), 25);

    leftThumbSlider.setBounds(leftColumn.withSizeKeepingCentre(300, 60));
    leftThumbLabel.setBounds(leftThumbSlider.getX(), leftThumbSlider.getBottom(), leftThumbSlider.getWidth(), 25);

    // --- Right Column Layout ---
    auto rightTopArea = rightColumn.removeFromTop(rightColumn.getHeight() * 0.5);
    rightHandKnob.setBounds(rightTopArea.withSizeKeepingCentre(200, 200));
    rightKnobLabel.setBounds(rightHandKnob.getX(), rightHandKnob.getBottom(), rightHandKnob.getWidth(), 25);

    rightThumbSlider.setBounds(rightColumn.withSizeKeepingCentre(300, 60));
    rightThumbLabel.setBounds(rightThumbSlider.getX(), rightThumbSlider.getBottom(), rightThumbSlider.getWidth(), 25);
}