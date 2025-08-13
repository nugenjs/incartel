Ingress telemetry data, draws data onto a frame, and writes to framebuffer

TelemetryConfig:
  DrawConfig
    x
    y
    color
    format
  Ingress
    key




main
read telementryconfig yaml and store in var
main loop
retrieves ingress data store in var
draw onto image with ingressdata and drawconfig
write image to framebuffer


need:
main
telemetryconfigyaml reader
ingressdata reader
drawer
framebuffer writer



Needs:
vcpkg
```
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh     # Linux/macOS
.\bootstrap-vcpkg.bat    # Windows
```
