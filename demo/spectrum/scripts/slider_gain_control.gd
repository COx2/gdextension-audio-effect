extends VSlider  # Extend HSlider instead of Node

# Enum for channel selection in the Inspector
enum Channel {LEFT, RIGHT}

# Export variables for Inspector configuration
@export var selected_channel: Channel = Channel.LEFT
@export var audio_bus_name: String = "Master"  # Default to Master bus

# References to audio
var effect_bus_idx: int = 0  # Bus where effects are inserted
var effect_idx: int = 0  # The second effect in the chain (index 1)

func _ready():
	# Configure slider properties
	self.min_value = -80.0
	self.max_value = 24.0
	self.step = 0.1
	
	# Get the audio bus index based on the name selected in the Inspector
	effect_bus_idx = AudioServer.get_bus_index(audio_bus_name)
	
	# Check if the bus exists
	if effect_bus_idx < 0:
		push_error("Audio bus '" + audio_bus_name + "' not found!")
		return
	
	# Initialize slider with current effect parameter value
	var effect = AudioServer.get_bus_effect(effect_bus_idx, effect_idx)
	
	if effect is GDEXStereoAmplify:
		# Set initial value based on selected channel
		if selected_channel == Channel.LEFT:
			self.value = effect.get_volume_db_left()  # Left channel
		else:  # RIGHT
			self.value = effect.get_volume_db_right()  # Right channel
	
	# Connect the value_changed signal to our handler
	self.value_changed.connect(_on_value_changed)

# Called when slider value changes
func _on_value_changed(new_value):
	# Check if the bus exists
	if effect_bus_idx < 0:
		return
		
	# Get the effect and update its parameter
	var effect = AudioServer.get_bus_effect(effect_bus_idx, effect_idx)
	
	if effect is GDEXStereoAmplify:
		# Update value based on selected channel
		if selected_channel == Channel.LEFT:
			effect.set_volume_db_left(new_value)  # Left channel
		else:  # RIGHT
			effect.set_volume_db_right(new_value)  # Right channel
