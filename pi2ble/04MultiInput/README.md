# Multi-Input Data Visualization

A real-time data visualization program that simulates and displays multiple sensor inputs (temperature, wind speed, and humidity) using sine waves with OpenCV.

## Features

- **Real-time Data Simulation**: Generates realistic sensor data using sine waves with noise
- **Multi-sensor Display**: Simultaneously shows temperature, wind speed, and humidity
- **Interactive Graphs**: Real-time line graphs with current value indicators
- **Configurable Parameters**: Adjustable base values, amplitudes, and frequencies
- **Modular Design**: Well-structured code split into multiple files for maintainability

## Project Structure

```
04MultiInput/
├── 04MultiInput.cpp      # Main application file
├── SensorData.h          # Data structures and configuration
├── SensorSimulator.h     # Sensor simulation class header
├── SensorSimulator.cpp   # Sensor simulation implementation
├── DataVisualizer.h      # Visualization class header
├── DataVisualizer.cpp    # Visualization implementation
├── Makefile             # Build configuration
└── README.md            # This file
```

## Build Requirements

- OpenCV 4.x
- C++11 compatible compiler (g++, clang++)
- pkg-config
- Make

## Building

```bash
# Clean previous builds
make clean

# Build the program
make

# Build with debug symbols
make debug

# Build and run
make run
```

## Usage

```bash
./04MultiInput
```

## Sensor Data Configuration

The program simulates three types of sensor data:

### Temperature
- **Range**: -30°C to 50°C (clamped)
- **Base Value**: 20°C
- **Variation**: ±15°C
- **Cycle Period**: ~20 seconds

### Wind Speed
- **Range**: 0 to 100 km/h (clamped)
- **Base Value**: 15 km/h
- **Variation**: ±10 km/h
- **Cycle Period**: ~13 seconds

### Humidity
- **Range**: 0% to 100% (clamped)
- **Base Value**: 60%
- **Variation**: ±25%
- **Cycle Period**: ~25 seconds

## Display Features

### Header Section
- Real-time current values for all sensors
- Color-coded sensor readings
- Data point counter

### Graph Section
- Three separate real-time line graphs
- Grid lines for easy reading
- Min/max value labels
- Current value indicators (colored dots)
- Individual scaling for each sensor type

### Color Coding
- **Red**: Temperature data
- **Blue**: Wind speed data
- **Green**: Humidity data

## Controls

- **q** or **ESC**: Quit the application

## Technical Details

### Data Generation
- Uses sine waves with different frequencies and phases
- Adds Gaussian noise for realistic variation
- Updates every 50ms (20 Hz refresh rate)
- Maintains a rolling buffer of 200 data points

### Visualization
- 1200x800 pixel window
- Real-time rendering using OpenCV
- Smooth line graphs with anti-aliasing
- Professional dark theme

## Customization

You can modify the sensor parameters in the `SensorConfig` structure in `SensorData.h`:

```cpp
struct SensorConfig {
    // Temperature configuration
    double tempBase = 20.0;        // Base temperature (°C)
    double tempAmplitude = 15.0;   // Temperature variation amplitude
    double tempFrequency = 0.1;    // Temperature change frequency
    
    // Similar parameters for wind and humidity...
};
```

## Troubleshooting

1. **Build Errors**: Ensure OpenCV is properly installed and pkg-config can find it
2. **Window Not Appearing**: Check that you have a display environment (not running headless)
3. **Performance Issues**: Reduce update frequency or data buffer size in the configuration

## Future Enhancements

- Add data logging to files
- Implement real sensor input interfaces
- Add statistical analysis (mean, std deviation)
- Support for additional sensor types
- Export data functionality
- Configuration file support
