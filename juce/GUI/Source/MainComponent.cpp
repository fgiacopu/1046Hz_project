#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // === LEFT HAND ===
    addAndMakeVisible(leftHandKnob);
    leftHandKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftHandKnob.setRange(0.0, 1.0);
    leftHandKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    leftHandKnob.setInterceptsMouseClicks(true, false);

    // Blue - Glasslike Colors
    leftHandKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff00a8b5).withAlpha(0.12f)); // Frosted glass ring
    leftHandKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff00a8b5).withAlpha(0.6f));    // Glass fill tint
    leftHandKnob.setColour(juce::Slider::thumbColourId, juce::Colour(0xff77e4d4));                               // Opaque neon reflection
    leftHandKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xff77e4d4));
    leftHandKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(leftKnobLabel);
    leftKnobLabel.setText("Left Hand Openness\n(Cut-off Frequency of Low Pass Filter)", juce::dontSendNotification);
    leftKnobLabel.setJustificationType(juce::Justification::centred);
    leftKnobLabel.setColour(juce::Label::textColourId, juce::Colour(0xff77e4d4));

    addAndMakeVisible(leftThumbSlider);
    leftThumbSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    leftThumbSlider.setRange(0.0, 1.0);
    leftThumbSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    leftThumbSlider.setInterceptsMouseClicks(true, false);

    // Blue - Glasslike Slider Colors
    leftThumbSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff00a8b5).withAlpha(0.15f));          // Smoked glass track
    leftThumbSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff77e4d4));                            // Glass handle
    leftThumbSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xff77e4d4));
    leftThumbSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(leftThumbLabel);
    leftThumbLabel.setText("Left Thumb Movement\n(Flanger)", juce::dontSendNotification);
    leftThumbLabel.setJustificationType(juce::Justification::centred);
    leftThumbLabel.setColour(juce::Label::textColourId, juce::Colour(0xff77e4d4));


    // === RIGHT HAND CONFIGURATION ===
    addAndMakeVisible(rightHandKnob);
    rightHandKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightHandKnob.setRange(0.0, 1.0);
    rightHandKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    rightHandKnob.setInterceptsMouseClicks(true, false);

    // Green - Glasslike Colors
    rightHandKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff4a7c59).withAlpha(0.12f)); // Frosted glass ring
    rightHandKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff4a7c59).withAlpha(0.6f));    // Glass fill tint
    rightHandKnob.setColour(juce::Slider::thumbColourId, juce::Colour(0xff9cb380));                               // Opaque moss reflection
    rightHandKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xff9cb380));
    rightHandKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(rightKnobLabel);
    rightKnobLabel.setText("Right Hand Openness\n(Chorus)", juce::dontSendNotification);
    rightKnobLabel.setJustificationType(juce::Justification::centred);
    rightKnobLabel.setColour(juce::Label::textColourId, juce::Colour(0xff9cb380));

    addAndMakeVisible(rightThumbSlider);
    rightThumbSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    rightThumbSlider.setRange(0.0, 1.0);
    rightThumbSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    rightThumbSlider.setInterceptsMouseClicks(true, false);

    // Green - Glasslike Slider Colors
    rightThumbSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff4a7c59).withAlpha(0.15f));          // Smoked glass track
    rightThumbSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff9cb380));                            // Glass handle
    rightThumbSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xff9cb380));
    rightThumbSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(rightThumbLabel);
    rightThumbLabel.setText("Right Thumb Movement\n(TODO)", juce::dontSendNotification);
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
    juce::Colour topColor(0xff1a1d24);
    juce::Colour bottomColor(0xff111317);

    juce::ColourGradient gradient(topColor, 0.0f, 0.0f,
        bottomColor, 0.0f, static_cast<float>(getHeight()),
        false);
    g.setGradientFill(gradient);
    g.fillAll();
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    // Add side and top margins so elements don't hug the window borders
    auto area = getLocalBounds().reduced(40, 30);

    // Split the window into two distinct horizontal zones
    auto topRowArea = area.removeFromTop(area.getHeight() * 0.55);
    auto bottomRowArea = area; // What remains at the bottom

    // --- 1. DIVIDE TOP ROW (KNOBS) ---
    auto leftKnobZone = topRowArea.removeFromLeft(topRowArea.getWidth() * 0.5);
    auto rightKnobZone = topRowArea;

    // Left Knob & Label
    leftHandKnob.setBounds(leftKnobZone.withSizeKeepingCentre(180, 180).translated(0, -15));
    // Position label directly under the knob with ample room for long text strings
    leftKnobLabel.setBounds(leftKnobZone.getX() + 10, leftHandKnob.getBottom() + 2, leftKnobZone.getWidth() - 20, 45);

    // Right Knob & Label
    rightHandKnob.setBounds(rightKnobZone.withSizeKeepingCentre(180, 180).translated(0, -15));
    rightKnobLabel.setBounds(rightKnobZone.getX() + 10, rightHandKnob.getBottom() + 2, rightKnobZone.getWidth() - 20, 45);


    // --- 2. DIVIDE BOTTOM ROW (SLIDERS) ---
    auto leftSliderZone = bottomRowArea.removeFromLeft(bottomRowArea.getWidth() * 0.5);
    auto rightSliderZone = bottomRowArea;

    // Left Slider & Label
    leftThumbSlider.setBounds(leftSliderZone.withSizeKeepingCentre(280, 50).translated(0, -15));
    leftThumbLabel.setBounds(leftSliderZone.getX() + 10, leftThumbSlider.getBottom() + 2, leftSliderZone.getWidth() - 20, 45);

    // Right Slider & Label
    rightThumbSlider.setBounds(rightSliderZone.withSizeKeepingCentre(280, 50).translated(0, -15));
    rightThumbLabel.setBounds(rightSliderZone.getX() + 10, rightThumbSlider.getBottom() + 2, rightSliderZone.getWidth() - 20, 45);
}