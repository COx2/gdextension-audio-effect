extends Node2D

const VU_COUNT = 16
const FREQ_MAX = 11050.0

const WIDTH = 800  # 全体の幅
const SPACE = 64   # 左右のスペクトラム間の間隔
const CHANNEL_WIDTH = (WIDTH - SPACE) / 2.0  # チャンネルあたりの幅
const HEIGHT = 250
const HEIGHT_SCALE = 8.0
const MIN_DB = 60
const ANIMATION_SPEED = 0.1

var spectrum: AudioEffectSpectrumAnalyzerInstance
var min_values_left: Array[float] = []
var max_values_left: Array[float] = []
var min_values_right: Array[float] = []
var max_values_right: Array[float] = []

func _draw() -> void:
	@warning_ignore("integer_division")
	var w := CHANNEL_WIDTH / VU_COUNT
	
	# 左チャンネルの描画
	for i in VU_COUNT:
		var min_height = min_values_left[i]
		var max_height = max_values_left[i]
		var height = lerp(min_height, max_height, ANIMATION_SPEED)

		# 左チャンネルのバー
		draw_rect(
				Rect2(w * i, HEIGHT - height, w - 2, height),
				Color.from_hsv(float(VU_COUNT * 0.6 + i * 0.5) / VU_COUNT, 0.5, 0.6)
		)
		draw_line(
				Vector2(w * i, HEIGHT - height),
				Vector2(w * i + w - 2, HEIGHT - height),
				Color.from_hsv(float(VU_COUNT * 0.6 + i * 0.5) / VU_COUNT, 0.5, 1.0),
				2.0,
				true
		)

		# 左チャンネルの反射
		draw_rect(
				Rect2(w * i, HEIGHT, w - 2, height),
				Color.from_hsv(float(VU_COUNT * 0.6 + i * 0.5) / VU_COUNT, 0.5, 0.6) * Color(1, 1, 1, 0.125)
		)
		draw_line(
				Vector2(w * i, HEIGHT + height),
				Vector2(w * i + w - 2, HEIGHT + height),
				Color.from_hsv(float(VU_COUNT * 0.6 + i * 0.5) / VU_COUNT, 0.5, 1.0) * Color(1, 1, 1, 0.125),
				2.0,
				true
		)
	
	# 右チャンネルの描画（左チャンネル + 間隔の位置から始める）
	for i in VU_COUNT:
		var min_height = min_values_right[i]
		var max_height = max_values_right[i]
		var height = lerp(min_height, max_height, ANIMATION_SPEED)

		# 右チャンネルのバー
		draw_rect(
				Rect2(CHANNEL_WIDTH + SPACE + w * i, HEIGHT - height, w - 2, height),
				Color.from_hsv(float(VU_COUNT * 0.6 + i * 0.5) / VU_COUNT, 0.5, 0.6)
		)
		draw_line(
				Vector2(CHANNEL_WIDTH + SPACE + w * i, HEIGHT - height),
				Vector2(CHANNEL_WIDTH + SPACE + w * i + w - 2, HEIGHT - height),
				Color.from_hsv(float(VU_COUNT * 0.6 + i * 0.5) / VU_COUNT, 0.5, 1.0),
				2.0,
				true
		)

		# 右チャンネルの反射
		draw_rect(
				Rect2(CHANNEL_WIDTH + SPACE + w * i, HEIGHT, w - 2, height),
				Color.from_hsv(float(VU_COUNT * 0.6 + i * 0.5) / VU_COUNT, 0.5, 0.6) * Color(1, 1, 1, 0.125)
		)
		draw_line(
				Vector2(CHANNEL_WIDTH + SPACE + w * i, HEIGHT + height),
				Vector2(CHANNEL_WIDTH + SPACE + w * i + w - 2, HEIGHT + height),
				Color.from_hsv(float(VU_COUNT * 0.6 + i * 0.5) / VU_COUNT, 0.5, 1.0) * Color(1, 1, 1, 0.125),
				2.0,
				true
		)


func _process(_delta: float) -> void:
	var data_left: Array[float] = []
	var data_right: Array[float] = []
	var prev_hz := 0.0

	for i in range(1, VU_COUNT + 1):
		var hz := i * FREQ_MAX / VU_COUNT
		
		# 左チャンネルの振幅を取得
		var magnitude_left := spectrum.get_magnitude_for_frequency_range(prev_hz, hz).x
		var energy_left := clampf((MIN_DB + linear_to_db(magnitude_left)) / MIN_DB, 0, 1)
		var height_left := energy_left * HEIGHT * HEIGHT_SCALE
		data_left.append(height_left)
		
		# 右チャンネルの振幅を取得
		var magnitude_right := spectrum.get_magnitude_for_frequency_range(prev_hz, hz).y
		var energy_right := clampf((MIN_DB + linear_to_db(magnitude_right)) / MIN_DB, 0, 1)
		var height_right := energy_right * HEIGHT * HEIGHT_SCALE
		data_right.append(height_right)
		
		prev_hz = hz

	# 左チャンネルの値を更新
	for i in VU_COUNT:
		if data_left[i] > max_values_left[i]:
			max_values_left[i] = data_left[i]
		else:
			max_values_left[i] = lerpf(max_values_left[i], data_left[i], ANIMATION_SPEED)

		if data_left[i] <= 0.0:
			min_values_left[i] = lerpf(min_values_left[i], 0.0, ANIMATION_SPEED)
	
	# 右チャンネルの値を更新
	for i in VU_COUNT:
		if data_right[i] > max_values_right[i]:
			max_values_right[i] = data_right[i]
		else:
			max_values_right[i] = lerpf(max_values_right[i], data_right[i], ANIMATION_SPEED)

		if data_right[i] <= 0.0:
			min_values_right[i] = lerpf(min_values_right[i], 0.0, ANIMATION_SPEED)

	# サウンドは連続的に再生されるため、グラフは毎フレーム更新する必要があります
	queue_redraw()


func _ready() -> void:
	spectrum = AudioServer.get_bus_effect_instance(0, 1)
	
	# 両チャンネルの配列を初期化
	min_values_left.resize(VU_COUNT)
	max_values_left.resize(VU_COUNT)
	min_values_left.fill(0.0)
	max_values_left.fill(0.0)
	
	min_values_right.resize(VU_COUNT)
	max_values_right.resize(VU_COUNT)
	min_values_right.fill(0.0)
	max_values_right.fill(0.0)
