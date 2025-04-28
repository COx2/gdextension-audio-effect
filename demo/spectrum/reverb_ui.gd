extends Node2D

# Export variables for Inspector configuration
@export var audio_bus_name: String = "Master"  # Default to Master bus

# References to audio
var effect_bus_idx: int = 0  # Bus where effects are inserted
var effect_idx: int = 2  # The second effect in the chain (index 2)

# State of parameter
var value_room_size: float = 0.0
var value_damping: float = 0.0
var value_width: float = 0.0
var value_dry_level: float = 0.0
var value_wet_level: float = 0.0
var value_freeze_mode: float = 0.0

func _ready():
	# Methods to get references to child nodes
	
	# Method 1: Using get_node()
	var room_size_node: SpinBox = get_node("RoomSize")
	var damping_node: SpinBox = get_node("Damping")
	var width_node: SpinBox = get_node("Width")
	var wet_level_node: SpinBox = get_node("WetLevel")
	var dry_level_node: SpinBox = get_node("DryLevel")
	var freeze_mode_node: CheckButton = get_node("FreezeMode")
	
	# Get the audio bus index based on the name selected in the Inspector
	effect_bus_idx = AudioServer.get_bus_index(audio_bus_name)
	
	# Check if the bus exists
	if effect_bus_idx < 0:
		push_error("Audio bus '" + audio_bus_name + "' not found!")
		return
	
	# Initialize slider with current effect parameter value
	var effect = AudioServer.get_bus_effect(effect_bus_idx, effect_idx)
	
	if effect is GDEXJuceReverb:
		self.value_room_size = effect.get_room_size()
		self.value_damping = effect.get_damping()
		self.value_width = effect.get_width()
		self.value_dry_level = effect.get_dry_level()
		self.value_wet_level = effect.get_wet_level()
		self.value_freeze_mode = effect.get_freeze_mode()
		
		room_size_node.value = self.value_room_size
		damping_node.value = self.value_damping
		width_node.value = self.value_width
		wet_level_node.value = self.value_wet_level
		dry_level_node.value = self.value_dry_level
		freeze_mode_node.set_pressed(self.value_freeze_mode)
	
	# Connect the value_changed signal to our handler
	room_size_node.value_changed.connect(_on_room_size_changed)
	damping_node.value_changed.connect(_on_damping_changed)
	width_node.value_changed.connect(_on_width_changed)
	wet_level_node.value_changed.connect(_on_wet_level_changed)
	dry_level_node.value_changed.connect(_on_dry_level_changed)
	freeze_mode_node.toggled.connect(_on_freeze_mode_changed)

# Called when slider value changes
func _on_room_size_changed(new_value):
	# Check if the bus exists
	if effect_bus_idx < 0:
		return
		
	# Get the effect and update its parameter
	var effect = AudioServer.get_bus_effect(effect_bus_idx, effect_idx)
	
	if effect is GDEXJuceReverb:
		effect.set_room_size(new_value)
		self.value_room_size = new_value
		
func _on_damping_changed(new_value):
	# Check if the bus exists
	if effect_bus_idx < 0:
		return
		
	# Get the effect and update its parameter
	var effect = AudioServer.get_bus_effect(effect_bus_idx, effect_idx)
	
	if effect is GDEXJuceReverb:
		effect.set_damping(new_value)
		self.value_damping = new_value
		
func _on_width_changed(new_value):
	# Check if the bus exists
	if effect_bus_idx < 0:
		return
		
	# Get the effect and update its parameter
	var effect = AudioServer.get_bus_effect(effect_bus_idx, effect_idx)
	
	if effect is GDEXJuceReverb:
		effect.set_width(new_value)
		self.value_width = new_value
		
func _on_wet_level_changed(new_value):
	# Check if the bus exists
	if effect_bus_idx < 0:
		return
		
	# Get the effect and update its parameter
	var effect = AudioServer.get_bus_effect(effect_bus_idx, effect_idx)
	
	if effect is GDEXJuceReverb:
		effect.set_wet_level(new_value)
		self.value_wet_level = new_value
		
func _on_dry_level_changed(new_value):
	# Check if the bus exists
	if effect_bus_idx < 0:
		return
		
	# Get the effect and update its parameter
	var effect = AudioServer.get_bus_effect(effect_bus_idx, effect_idx)
	
	if effect is GDEXJuceReverb:
		effect.set_dry_level(new_value)
		self.value_dry_level = new_value
		
func _on_freeze_mode_changed(new_value):
	# Check if the bus exists
	if effect_bus_idx < 0:
		return
		
	# Get the effect and update its parameter
	var effect = AudioServer.get_bus_effect(effect_bus_idx, effect_idx)
	
	if effect is GDEXJuceReverb:
		effect.set_freeze_mode(new_value)
		self.value_freeze_mode = new_value
