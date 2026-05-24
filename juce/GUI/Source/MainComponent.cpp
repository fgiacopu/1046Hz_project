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

    leftHandKnob.textFromValueFunction = [](double value)
        {
            double minFreq = 200.0;
            double maxFreq = 8000.0;

            double freq = minFreq * std::pow(maxFreq / minFreq, value);

            return juce::String((int)freq) + " Hz";
        };

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

    leftThumbSlider.textFromValueFunction = [](double value)
        {
            int percentage = value * 100.0;
            return juce::String(percentage) + " %";
        };

    // Blue - Glasslike Slider Colors
    leftThumbSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff00a8b5).withAlpha(0.6f));          // Smoked glass track
    leftThumbSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff77e4d4));                          // Glass handle
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

    rightHandKnob.textFromValueFunction = [](double value)
        {
            int percentage = value * 100.0;
            return juce::String(percentage) + " %";
        };

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

    rightThumbSlider.textFromValueFunction = [](double value)
        {
            int percentage = value * 100.0;
            return juce::String(percentage) + " %";
        };

    // Green - Glasslike Slider Colors
    rightThumbSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff4a7c59).withAlpha(0.6f));          // Smoked glass track
    rightThumbSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff9cb380));                            // Glass handle
    rightThumbSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xff9cb380));
    rightThumbSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(rightThumbLabel);
    rightThumbLabel.setText("Right Thumb Movement\n(TODO)", juce::dontSendNotification);
    rightThumbLabel.setJustificationType(juce::Justification::centred);
    rightThumbLabel.setColour(juce::Label::textColourId, juce::Colour(0xff9cb380));
    

    
    // === ENVELOPE CONFIGURATION (A, S, D, R) ===
    juce::Colour envColor(0xffa363c9);

    // Attack
    addAndMakeVisible(attackKnob);
    attackKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    attackKnob.setRange(0.0, 1.0);
    attackKnob.setValue(0.0);
    attackKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    attackKnob.setNumDecimalPlacesToDisplay(3);
    attackKnob.setColour(juce::Slider::rotarySliderOutlineColourId, envColor.withAlpha(0.12f));
    attackKnob.setColour(juce::Slider::rotarySliderFillColourId, envColor.withAlpha(0.6f));
    attackKnob.setColour(juce::Slider::thumbColourId, envColor);
    attackKnob.setColour(juce::Slider::textBoxTextColourId, envColor);
    attackKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(attackLabel);
    attackLabel.setText("A", juce::dontSendNotification);
    attackLabel.setJustificationType(juce::Justification::centred);
    attackLabel.setColour(juce::Label::textColourId, envColor);

    // Decay
    addAndMakeVisible(decayKnob);
    decayKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    decayKnob.setRange(0.0, 1.0);
    decayKnob.setValue(0.0);
    decayKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    decayKnob.setNumDecimalPlacesToDisplay(3);
    decayKnob.setColour(juce::Slider::rotarySliderOutlineColourId, envColor.withAlpha(0.12f));
    decayKnob.setColour(juce::Slider::rotarySliderFillColourId, envColor.withAlpha(0.6f));
    decayKnob.setColour(juce::Slider::thumbColourId, envColor);
    decayKnob.setColour(juce::Slider::textBoxTextColourId, envColor);
    decayKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(decayLabel);
    decayLabel.setText("D", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centred);
    decayLabel.setColour(juce::Label::textColourId, envColor);

    // Sustain
    addAndMakeVisible(sustainKnob);
    sustainKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    sustainKnob.setRange(0.0, 1.0);
    sustainKnob.setValue(0.0);
    sustainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    sustainKnob.setNumDecimalPlacesToDisplay(3);
    sustainKnob.setColour(juce::Slider::rotarySliderOutlineColourId, envColor.withAlpha(0.12f));
    sustainKnob.setColour(juce::Slider::rotarySliderFillColourId, envColor.withAlpha(0.6f));
    sustainKnob.setColour(juce::Slider::thumbColourId, envColor);
    sustainKnob.setColour(juce::Slider::textBoxTextColourId, envColor);
    sustainKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(sustainLabel);
    sustainLabel.setText("S", juce::dontSendNotification);
    sustainLabel.setJustificationType(juce::Justification::centred);
    sustainLabel.setColour(juce::Label::textColourId, envColor);

    // Release
    addAndMakeVisible(releaseKnob);
    releaseKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    releaseKnob.setRange(0.0, 1.0);
    releaseKnob.setValue(0.0);
    releaseKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    releaseKnob.setNumDecimalPlacesToDisplay(3);
    releaseKnob.setColour(juce::Slider::rotarySliderOutlineColourId, envColor.withAlpha(0.12f));
    releaseKnob.setColour(juce::Slider::rotarySliderFillColourId, envColor.withAlpha(0.6f));
    releaseKnob.setColour(juce::Slider::thumbColourId, envColor);
    releaseKnob.setColour(juce::Slider::textBoxTextColourId, envColor);
    releaseKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(releaseLabel);
    releaseLabel.setText("R", juce::dontSendNotification);
    releaseLabel.setJustificationType(juce::Justification::centred);
    releaseLabel.setColour(juce::Label::textColourId, envColor);

    // Toggle Switches
    juce::Colour offGrey(0xff2a2d34);

    // Left Hand Toggle
    addAndMakeVisible(leftHandToggle);
    //leftHandToggle.setButtonText("Leftn/Hand");
    leftHandToggle.setClickingTogglesState(true);
    leftHandToggle.setColour(juce::TextButton::buttonColourId, offGrey);
    leftHandToggle.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff77e4d4).withAlpha(0.6f));
    leftHandToggle.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
    leftHandToggle.setToggleState(false, juce::dontSendNotification);

    // Right Hand Toggle
    addAndMakeVisible(rightHandToggle);
    //rightHandToggle.setButtonText("Rightn/Hand");
    rightHandToggle.setClickingTogglesState(true);
    rightHandToggle.setColour(juce::TextButton::buttonColourId, offGrey);
    rightHandToggle.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff9cb380).withAlpha(0.6f));
    rightHandToggle.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
    rightHandToggle.setToggleState(false, juce::dontSendNotification);


    // Left Thumb Toggle
    addAndMakeVisible(leftThumbToggle);
    //leftThumbToggle.setButtonText("Leftn/Thumb");
    leftThumbToggle.setClickingTogglesState(true);
    leftThumbToggle.setColour(juce::TextButton::buttonColourId, offGrey);
    leftThumbToggle.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff77e4d4).withAlpha(0.6f));
    leftThumbToggle.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
    leftThumbToggle.setToggleState(false, juce::dontSendNotification);

    // Right Thumb Toggle
    addAndMakeVisible(rightThumbToggle);
    //rightThumbToggle.setButtonText("Rightn/Thumb");
    rightThumbToggle.setClickingTogglesState(true);
    rightThumbToggle.setColour(juce::TextButton::buttonColourId, offGrey);
    rightThumbToggle.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff9cb380).withAlpha(0.6f));
    rightThumbToggle.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
    rightThumbToggle.setToggleState(false, juce::dontSendNotification);

    // Biomechanical Controls
    auto sendAll = [this]()
        {
            sender.send("/hand/left/open", static_cast<float>(leftHandKnob.getValue()));
            sender.send("/hand/right/open", static_cast<float>(rightHandKnob.getValue()));
            sender.send("/hand/left/thumb", static_cast<float>(leftThumbSlider.getValue()));
            sender.send("/hand/right/thumb", static_cast<float>(rightThumbSlider.getValue()));
        };

    // Toggle
    leftHandToggle.onClick = [this, sendAll]()
        {
            if (!leftHandToggle.getToggleState())
                sender.send("/hand/left/open", 0.0f);

            sendAll();
        };

    rightHandToggle.onClick = [this, sendAll]()
        {
            if (!rightHandToggle.getToggleState())
                sender.send("/hand/right/open", 0.0f);

            sendAll();
        };

    leftThumbToggle.onClick = [this, sendAll]()
        {
            if (!leftThumbToggle.getToggleState())
                sender.send("/hand/left/thumb", 0.0f);

            sendAll();
        };

    rightThumbToggle.onClick = [this, sendAll]()
        {
            if (!rightThumbToggle.getToggleState())
                sender.send("/hand/right/thumb", 0.0f);

            sendAll();
        };

    leftHandKnob.onValueChange = sendAll;
    rightHandKnob.onValueChange = sendAll;
    leftThumbSlider.onValueChange = sendAll;
    rightThumbSlider.onValueChange = sendAll;


    // ADSR Envelope Controls
    attackKnob.onValueChange = [this]() {
        sender.send("/attack", static_cast<float>(attackKnob.getValue()));
        };

    decayKnob.onValueChange = [this]() {
        sender.send("/decay", static_cast<float>(decayKnob.getValue()));
        };

    sustainKnob.onValueChange = [this]() {
        sender.send("/sustaain", static_cast<float>(sustainKnob.getValue()));
        };

    releaseKnob.onValueChange = [this]() {
        sender.send("/releasee", static_cast<float>(releaseKnob.getValue()));
        };


    // Setting the size of the component after adding any child components is necessary
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
        sender.connect("127.0.0.1", 57130); //Forwording to SC
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

        // From Supercollider: Envelope Defaults
        if (pattern == "/attack" || pattern == "/decay" || pattern == "/sustaain" || pattern == "/releasee")
        {
            juce::MessageManager::callAsync([this, val, pattern]() {
                if (pattern == "/attack")
                    attackKnob.setValue(val, juce::dontSendNotification);
                else if (pattern == "/decay")
                    decayKnob.setValue(val, juce::dontSendNotification);
                else if (pattern == "/sustaain")
                    sustainKnob.setValue(val, juce::dontSendNotification);
                else if (pattern == "/releasee")
                    releaseKnob.setValue(val, juce::dontSendNotification);
                });

            return;
        }

        // From Python: Biomechanical Inputs
        // These update the GUI and then get forwarded to Supercollider, but only if the switch is ON.
        bool shouldProcess = true;

        if (pattern == "/hand/left/open" && !leftHandToggle.getToggleState()) shouldProcess = false;
        else if (pattern == "/hand/right/open" && !rightHandToggle.getToggleState()) shouldProcess = false;
        else if (pattern == "/hand/left/thumb" && !leftThumbToggle.getToggleState()) shouldProcess = false;
        else if (pattern == "/hand/right/thumb" && !rightThumbToggle.getToggleState()) shouldProcess = false;

        if (shouldProcess)
        {
            // Forwarding to SuperCollider
            sender.send(pattern, val);

            // Update UI safely on the main Thread
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
    // After adding any child components, their positions have to be updated here.

    // Add side and top margins
    auto area = getLocalBounds().reduced(40, 30);

    // Split the window into three distinct vertical zones
    auto topRowArea = area.removeFromTop(area.getHeight() * 0.33f);
    auto middleRowArea = area.removeFromTop(area.getHeight() * 0.5f);
    auto bottomRowArea = area;

    // Save the Y-coordinate of the line that separates the knobs and sliders
    int dividingLineY = middleRowArea.getBottom();
    int centerX = getLocalBounds().getCentreX();

    // Dividing top row (A, D, S, R)
    auto colWidth = topRowArea.getWidth() / 4;
    auto aZone = topRowArea.removeFromLeft(colWidth);
    auto dZone = topRowArea.removeFromLeft(colWidth);
    auto sZone = topRowArea.removeFromLeft(colWidth);
    auto rZone = topRowArea;

    int envKnobSize = 100;

    attackKnob.setBounds(aZone.withSizeKeepingCentre(envKnobSize, envKnobSize).translated(0, -10));
    attackLabel.setBounds(aZone.getX() + 10, attackKnob.getBottom(), aZone.getWidth() - 20, 30);

    decayKnob.setBounds(dZone.withSizeKeepingCentre(envKnobSize, envKnobSize).translated(0, -10));
    decayLabel.setBounds(dZone.getX() + 10, decayKnob.getBottom(), dZone.getWidth() - 20, 30);

    sustainKnob.setBounds(sZone.withSizeKeepingCentre(envKnobSize, envKnobSize).translated(0, -10));
    sustainLabel.setBounds(sZone.getX() + 10, sustainKnob.getBottom(), sZone.getWidth() - 20, 30);

    releaseKnob.setBounds(rZone.withSizeKeepingCentre(envKnobSize, envKnobSize).translated(0, -10));
    releaseLabel.setBounds(rZone.getX() + 10, releaseKnob.getBottom(), rZone.getWidth() - 20, 30);


    // Dividing middle row (Hand openness knobs)
    auto leftKnobZone = middleRowArea.removeFromLeft(middleRowArea.getWidth() * 0.5f);
    auto rightKnobZone = middleRowArea;

    int mainKnobSize = 150;

    leftHandKnob.setBounds(leftKnobZone.withSizeKeepingCentre(mainKnobSize, mainKnobSize).translated(0, -10));
    leftKnobLabel.setBounds(leftKnobZone.getX() + 10, leftHandKnob.getBottom() + 2, leftKnobZone.getWidth() - 20, 45);

    rightHandKnob.setBounds(rightKnobZone.withSizeKeepingCentre(mainKnobSize, mainKnobSize).translated(0, -10));
    rightKnobLabel.setBounds(rightKnobZone.getX() + 10, rightHandKnob.getBottom() + 2, rightKnobZone.getWidth() - 20, 45);


    // Divide bottom row (Thumb movement sliders)
    auto leftSliderZone = bottomRowArea.removeFromLeft(bottomRowArea.getWidth() * 0.5f);
    auto rightSliderZone = bottomRowArea;

    leftThumbSlider.setBounds(leftSliderZone.withSizeKeepingCentre(280, 50).translated(0, -10));
    leftThumbLabel.setBounds(leftSliderZone.getX() + 10, leftThumbSlider.getBottom() + 2, leftSliderZone.getWidth() - 20, 45);

    rightThumbSlider.setBounds(rightSliderZone.withSizeKeepingCentre(280, 50).translated(0, -10));
    rightThumbLabel.setBounds(rightSliderZone.getX() + 10, rightThumbSlider.getBottom() + 2, rightSliderZone.getWidth() - 20, 45);


    // 2x2 Toggle grid
    int btnSize = 35; // Size of the square toggle buttons
    int gap = 2;      // Pixels of space between the buttons

    // Top-Left (Left Hand)
    leftHandToggle.setBounds(centerX - btnSize - gap, dividingLineY - btnSize - gap, btnSize, btnSize);

    // Top-Right (Right Hand)
    rightHandToggle.setBounds(centerX + gap, dividingLineY - btnSize - gap, btnSize, btnSize);

    // Bottom-Left (Left Thumb)
    leftThumbToggle.setBounds(centerX - btnSize - gap, dividingLineY + gap, btnSize, btnSize);

    // Bottom-Right (Right Thumb)
    rightThumbToggle.setBounds(centerX + gap, dividingLineY + gap, btnSize, btnSize);
}