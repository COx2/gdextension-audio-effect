extends Control

var file_dialog: FileDialog
var juce_audio_plugin_host: GDEXJuceAudioPluginHostAudioStream

func _ready():
	juce_audio_plugin_host = get_node("../JuceAudioPluginHostPlayer").stream
	
	# Button setup
	var button_sinewave = get_node("Button_SineWave")
	button_sinewave.text = "Sine Wave Sound"
	button_sinewave.size_flags_horizontal = Control.SIZE_SHRINK_CENTER
	button_sinewave.pressed.connect(_on_button_sine_sound_pressed)
	add_child(button_sinewave)
	
	var button_sampler = get_node("Button_LoadPlugin")
	button_sampler.text = "Load Audio Plugin"
	button_sampler.size_flags_horizontal = Control.SIZE_SHRINK_CENTER
	button_sampler.pressed.connect(_on_button_sampler_sound_pressed)
	add_child(button_sampler)
	
	# FileDialog setup
	file_dialog = FileDialog.new()
	file_dialog.access = FileDialog.ACCESS_FILESYSTEM
	file_dialog.file_mode = FileDialog.FILE_MODE_OPEN_FILE
	file_dialog.add_filter("*.vst3; VST3 File or Bundle")
	file_dialog.title = "Select a VST3 File or Bundle"
	file_dialog.size = Vector2(800, 580)
	file_dialog.file_selected.connect(_on_file_selected)
	file_dialog.set_current_path("C:/Program Files/Common Files/VST3/")
	add_child(file_dialog)
	
func _on_button_sampler_sound_pressed():
	# Show file dialog centered on screen
	file_dialog.popup_centered()

func _on_file_selected(path: String):
	print("Selected file: " + path)
	
	# Pass the path to GDExtension
	#if juce_instrument:
		#juce_instrument.load_custom_sound(path)
	#else:
		#print("juce_instrument instance not found")
		
	if juce_audio_plugin_host:
		juce_audio_plugin_host.load_audio_plugin(path)
	else:
		print("juce_audio_plugin_host instance not found")

func _on_button_sine_sound_pressed():
	return
	#if juce_instrument:
		#juce_instrument.load_sine_wave()
	#else:
		#print("juce_instrument instance not found")
