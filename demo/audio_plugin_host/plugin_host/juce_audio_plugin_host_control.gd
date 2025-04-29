extends Control

var file_dialog: FileDialog
var juce_audio_plugin_host: GDEXJuceAudioPluginHostAudioStream

func _ready():
	juce_audio_plugin_host = get_node("../JuceAudioPluginHostPlayer").stream
	
	# Button setup
	var button_sampler = get_node("Button_LoadAudioPlugin")
	button_sampler.text = "Load Audio Plugin"
	button_sampler.size_flags_horizontal = Control.SIZE_SHRINK_CENTER
	button_sampler.pressed.connect(_on_button_load_audio_plugin_pressed)
	add_child(button_sampler)
	
	var button_open_plugin_editor = get_node("Button_OpenPluginEditor")
	button_open_plugin_editor.text = "Open Plugin Editor"
	button_open_plugin_editor.size_flags_horizontal = Control.SIZE_SHRINK_CENTER
	button_open_plugin_editor.pressed.connect(_on_button_open_plugin_editor_pressed)
	add_child(button_open_plugin_editor)
	
	# FileDialog setup
	file_dialog = FileDialog.new()
	file_dialog.access = FileDialog.ACCESS_FILESYSTEM
	file_dialog.file_mode = FileDialog.FILE_MODE_OPEN_ANY
	file_dialog.add_filter("*.vst3; VST3 File or Bundle")
	file_dialog.title = "Select a VST3 File or Bundle"
	file_dialog.size = Vector2(800, 580)
	file_dialog.file_selected.connect(_on_file_or_bundle_selected)
	file_dialog.dir_selected.connect(_on_file_or_bundle_selected)
	file_dialog.set_current_path("C:/Program Files/Common Files/VST3/")
	add_child(file_dialog)
	
func _on_button_load_audio_plugin_pressed():
	# Show file dialog centered on screen
	file_dialog.popup_centered()

func _on_file_or_bundle_selected(path: String):
	print("Selected file or bundle: " + path)
	if juce_audio_plugin_host:
		juce_audio_plugin_host.load_audio_plugin(path)
	else:
		print("juce_audio_plugin_host instance not found")

func _on_button_open_plugin_editor_pressed():
	if juce_audio_plugin_host:
		juce_audio_plugin_host.open_plugin_editor()
	else:
		print("juce_audio_plugin_host instance not found")
	return
