
cmake_minimum_required(VERSION 3.15)

project(LFOKeys VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Disable VST2 before adding JUCE
#set(JUCE_PLUGINHOST_VST2 OFF CACHE BOOL "Disable VST2 plugin format" FORCE)
add_compile_definitions(JUCE_VST3_CAN_REPLACE_VST2=0)



# Path to JUCE (adjust if necessary)
set(JUCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../JUCE")
add_subdirectory(${JUCE_DIR} JUCE)

set(SOURCES
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/Engine.cpp
    Source/EnvelopeEditor.cpp
    Source/FunctionTable.cpp
    Source/SegmentGenerator.cpp
    Source/SystemMidiInputHandler.cpp
)




juce_add_plugin(LFOKeys
    COMPANY_NAME              "IDK"
    IS_SYNTH                  FALSE
    NEEDS_MIDI_INPUT          TRUE
    NEEDS_MIDI_OUTPUT         FALSE
    IS_MIDI_EFFECT            FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    PLUGIN_MANUFACTURER_CODE JuC3
    PLUGIN_CODE               LFK1
    FORMATS                   VST3
    PRODUCT_NAME              "LFOKeys"
)



juce_generate_juce_header(LFOKeys)

target_sources(LFOKeys PRIVATE ${SOURCES})

target_link_libraries(LFOKeys PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_plugin_client
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)
