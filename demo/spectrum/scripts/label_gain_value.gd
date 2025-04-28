extends Label  # Change to extend Label instead of Node

# Enum for channel selection in the Inspector
enum Channel {LEFT, RIGHT}

# Export variables for Inspector configuration
@export var selected_channel: Channel = Channel.LEFT
@export var audio_bus_name: String = "Master"  # Default to Master bus

# References to nodes
var effect_bus_idx: int = 0  # Bus where effects are inserted
var effect_idx: int = 0  # The second effect in the chain (index 1)

# Variable to store previous gain value
var last_gain_value: float = 0.0

# Called every frame
func _process(_delta):
	# Check if the bus exists
	if effect_bus_idx < 0:
		return
		
	# Get current effect gain value
	var effect = AudioServer.get_bus_effect(effect_bus_idx, effect_idx)
	var current_gain = 0.0
	
	if effect is GDEXStereoAmplify:
		# Get value based on selected channel
		if selected_channel == Channel.LEFT:
			current_gain = effect.get_volume_db_left()  # Left channel
		else:  # RIGHT
			current_gain = effect.get_volume_db_right()  # Right channel
	
	# Only update label if gain value has changed (for performance)
	if current_gain != last_gain_value:
		_update_gain_label(current_gain)
		last_gain_value = current_gain

# Function to update the label text
func _update_gain_label(gain_value):
	# Format label text based on gain value
	var gain_text = "%.1f dB" % gain_value
	if gain_value <= -80.0:
		gain_text = "-∞ dB"

	# Add channel and bus information to the label
	var channel_text = "Left" if selected_channel == Channel.LEFT else "Right"
	self.text = channel_text + "\n" + gain_text
