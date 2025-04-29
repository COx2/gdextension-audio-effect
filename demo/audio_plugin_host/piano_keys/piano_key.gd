class_name PianoKey
extends Control

var pitch_scale: float
var note_number: int
#var juce_instrument: GDEXJuceInstrumentAudioStream
var juce_audio_plugin_host: GDEXJuceAudioPluginHostAudioStream

@onready var key: ColorRect = $Key
@onready var start_color: Color = key.color

func setup(pitch_index: int) -> void:
	note_number = pitch_index
	name = "PianoKey" + str(pitch_index)

func activate() -> void:
	key.color = (Color.YELLOW + start_color) / 2
	juce_audio_plugin_host.midi_note_on(1, note_number, 1.0)

func deactivate() -> void:
	key.color = start_color
	juce_audio_plugin_host.midi_note_off(1, note_number, 1.0)
