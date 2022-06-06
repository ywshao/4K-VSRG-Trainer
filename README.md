# 4K-VSRG-Trainer
This is the final project for the MIR course.

## How to Play the Game
### Set Audio Device **\*Important\***
1. Open "Audio-Device-Detector.exe" and remember the "Num" of desired playback device.
2. Open "config.ini" and change x in "[Audio] device = x" to the "Num".

### Import BMS
Because of copyrights, the BMS is not provided here.
You can get them from [DEE2 EVENT LIST](https://manbow.nothing.sh/event/event.cgi), [Stella/Satelite table](https://stellabms.xyz/), or [GENOCIDE BMS difficulties review](https://nekokan.dyndns.info/~lobsak/genocide/), etc.

### Keymaps
Use arrow keys to navigate the arrow.
Use enter to select things.
Use esc to go back to the previous menu.
"xc<>" keys are respected to column 1, 2, 3, and 4.

### Menu
Train: play a bms.
Dan: play dan course.
Exit: exit to game.

#### Dan
The list of dan courses.

#### Train
1. The list of BMS directories.
2. The list of BMS files in the selected directory.

#### Pattern Parameters
Rate: the rate modifier of the BMS.
Jack: the jackness of the chart.
Jack Length: the jack lengthness of the chart.
Speed Tech: the hardness of the arrangement of streams.
Jump: the density of jumps.
Hand: the density of hands.
Quad: the density of quads.
Chord Tech: the jackness of the chart when notes per second is low and has jumps, hands, or quads.

### Gameplay
The top-left bar is the HP bar.
ScoreV1 uses step function corresponding to judge intervals.
ScoreV2 uses the Etterna-Wife-like style and is more indicative.
AvgErr is an average error.
Var is the variance of the error.
SD is the standard deviation of the error.
KPS is the average keys-per-second in 2 seconds.

### Evaluation
For dan courses, the left side is the segmented evaluation, and the right side is the whole evaluation.

## More about Configs
### More about "config.ini"
You can change [BMSPath] directory with your own one.
You can change [Texture] directory to use your own one. The texture directory must be placed in "texture".
You can change [Font] font_regular and font_judge. These fonts must be placed in "texture".
You can change the liveTime and scale of [ErrorMeter].
You can change speed of [ScrollSpeed] to set the scroll speed.
You can enable or disable [WSOLA] to switch between WSOLA and resampling.
You can enable [Debug] to see some debug informations.

### About "config.ini" in Texture
"note" is the texture of the notes.
"receiptor" is the texture of the receiptors.
"judgeNote" is the texture of the real-time replay notes.
"judgeKey" is the texture of the real-time replay keys you pressed.
"errorMeter" is the texture of the error meter.

For the number after "judgeNote", "judgeKey", and "errorMeter":
1. Marvelous
2. Perfect
3. Great
4. Good
5. Bad
6. Miss
7. Early miss (early poor)

### Create Your Own Dan Course
Please refer to files in sound_test/dan.
Change "dirPath" with the BMS directory.
Change "filePath" with the BMS files.
You can also refer to the .ini it generates in the BMS directories.

## Bugs and not Implemented Functions
In some situations, a memory leak may occur.
The end of the music may be clipped.
If the sound is too loud, the sound may be distorted.

Only supports 7+1k BMS files.
Stop sequences are not processed.
BPM changes that are not at the beginning of the bar are not processed.

## References
The notes of default_bar are modified from [R Skin v3.0 (Bars)](https://dl.orangedox.com/oKLlx4/13?dl=1)
The development log can be found [here](https://hackmd.io/AHmZyMbJQXWOBXwvI58NhQ?view) (Traditional Chinese)

## Used Libraries
SDL2
SDL_gpu
SDL_tff
NFont_gpu
SDL_FontCache
PortAudio
libsndfile
mINI
SoundTouch