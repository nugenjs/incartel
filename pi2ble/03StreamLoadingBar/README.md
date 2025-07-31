# Video Stream with Progress Bar

A C++ program using OpenCV that streams video and displays a moving progress bar.

## Features

- **Video File Support**: Play video files with progress tracking
- **Camera Support**: Stream from camera with cyclic progress bar
- **Progress Bar**: Visual progress indicator with percentage and frame information
- **Auto-restart**: Video files automatically loop when finished
- **Keyboard Controls**: Press 'q' or ESC to quit

## Build Requirements

- OpenCV 4.x
- C++11 compatible compiler
- pkg-config

## Building

```bash
make clean
make
```

## Usage

### Play a video file:
```bash
./03StreamLoadingBar path/to/your/video.mp4
```

### Use camera (default):
```bash
./03StreamLoadingBar
```

## Controls

- **q** or **ESC**: Quit the application

## Progress Bar Features

- **Green bar**: Shows current progress
- **Percentage**: Displays completion percentage
- **Frame counter**: Shows current frame / total frames
- **Auto-loop**: Video files restart automatically when finished

## For Camera Mode

When using a camera (no video file specified), the progress bar shows a 5-second cyclic animation since there's no defined "end" to the stream.

## Troubleshooting

1. **Video won't open**: Make sure the video file path is correct and the format is supported by OpenCV
2. **Camera not found**: Ensure your camera is connected and not being used by another application
3. **Build errors**: Check that OpenCV is properly installed and pkg-config can find it
