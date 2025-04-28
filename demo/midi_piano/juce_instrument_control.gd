extends Control

var file_dialog: FileDialog
var gdextension_instance # GDExtension instance

var juce_instrument: GDEXJuceInstrumentAudioStream

func _ready():
	juce_instrument = get_node("../JuceInstrumentPlayer").stream
	
	# Button setup
	var button_sinewave = get_node("Button_SineWave")
	button_sinewave.text = "Sine Wave Sound"
	button_sinewave.size_flags_horizontal = Control.SIZE_SHRINK_CENTER
	button_sinewave.pressed.connect(_on_button_sine_sound_pressed)
	add_child(button_sinewave)
	
	var button_sampler = get_node("Button_SamplerSound")
	button_sampler.text = "Sampler Sound"
	button_sampler.size_flags_horizontal = Control.SIZE_SHRINK_CENTER
	button_sampler.pressed.connect(_on_button_sampler_sound_pressed)
	add_child(button_sampler)
	
	# FileDialog setup
	file_dialog = FileDialog.new()
	file_dialog.access = FileDialog.ACCESS_FILESYSTEM
	file_dialog.file_mode = FileDialog.FILE_MODE_OPEN_FILE
	file_dialog.add_filter("*.wav; Wave Files")
	file_dialog.title = "Select a File"
	file_dialog.size = Vector2(800, 600)
	file_dialog.file_selected.connect(_on_file_selected)
	add_child(file_dialog)
	
	# Get GDExtension instance
	# gdextension_instance = $GDExtensionNode # Replace with your actual reference method

func _on_button_sampler_sound_pressed():
	# Show file dialog centered on screen
	file_dialog.popup_centered()

func _on_file_selected(path: String):
	print("Selected file: " + path)
	
	# Pass the path to GDExtension
	if juce_instrument:
		juce_instrument.load_custom_sound(path)
	else:
		print("juce_instrument instance not found")

func _on_button_sine_sound_pressed():
	if juce_instrument:
		juce_instrument.load_sine_wave()
	else:
		print("juce_instrument instance not found")
