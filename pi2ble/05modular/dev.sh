#!/bin/bash

# ==============================================================================
# Telemetry System Development Script
# ==============================================================================
# This script automatically sets up all dependencies and builds the project.
# 
# FIRST RUN (fresh clone):
#   ./dev.sh              # Auto-setup vcpkg, install yaml-cpp, build & run
#
# REQUIREMENTS:
#   - macOS or Linux
#   - CMake 3.15+
#   - C++ compiler (gcc/clang)
#   - Git (for vcpkg submodules)
#
# WHAT IT DOES:
#   1. Bootstraps vcpkg if needed
#   2. Installs yaml-cpp dependency
#   3. Smart builds (only when files change)
#   4. Runs the specified executable
# ==============================================================================

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

PROJECT_DIR="/Users/jngu3012/dev/incartel/pi2ble/05modular"
BUILD_DIR="$PROJECT_DIR/build"

# Function to clean
clean() {
    echo -e "${BLUE}🧹 Cleaning...${NC}"
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        echo -e "${GREEN}✅ Cleaned build directory${NC}"
    else
        echo -e "${YELLOW}ℹ️  Build directory already clean${NC}"
    fi
}

# Function to setup vcpkg and dependencies
setup_dependencies() {
    echo -e "${BLUE}📦 Setting up dependencies...${NC}"
    
    # Check if vcpkg bootstrap is needed
    if [ ! -f "$PROJECT_DIR/vcpkg/vcpkg" ]; then
        echo -e "${YELLOW}🔧 Bootstrapping vcpkg...${NC}"
        cd "$PROJECT_DIR/vcpkg"
        
        # Bootstrap vcpkg based on OS
        if [[ "$OSTYPE" == "darwin"* ]]; then
            ./bootstrap-vcpkg.sh
        elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
            ./bootstrap-vcpkg.sh
        else
            echo -e "${RED}❌ Unsupported OS: $OSTYPE${NC}"
            cd "$PROJECT_DIR"
            return 1
        fi
        
        cd "$PROJECT_DIR"
        
        if [ ! -f "$PROJECT_DIR/vcpkg/vcpkg" ]; then
            echo -e "${RED}❌ vcpkg bootstrap failed${NC}"
            return 1
        fi
        
        echo -e "${GREEN}✅ vcpkg bootstrapped successfully${NC}"
    fi
    
    # Install yaml-cpp if not already installed
    if [ ! -d "$PROJECT_DIR/vcpkg/installed" ] || [ ! -d "$PROJECT_DIR/vcpkg/installed/*/lib" ]; then
        echo -e "${YELLOW}📚 Installing yaml-cpp...${NC}"
        cd "$PROJECT_DIR/vcpkg"
        ./vcpkg install yaml-cpp
        cd "$PROJECT_DIR"
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}✅ yaml-cpp installed successfully${NC}"
        else
            echo -e "${RED}❌ Failed to install yaml-cpp${NC}"
            return 1
        fi
    else
        echo -e "${GREEN}✅ Dependencies already installed${NC}"
    fi
    
    return 0
}

# Function to build
build() {
    echo -e "${BLUE}🔨 Building...${NC}"
    
    # Setup dependencies first
    if ! setup_dependencies; then
        echo -e "${RED}❌ Dependency setup failed${NC}"
        return 1
    fi
    
    # Create build directory if needed
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    # Configure and build
    echo -e "${YELLOW}🔧 Configuring CMake with vcpkg toolchain...${NC}"
    if cmake -DCMAKE_TOOLCHAIN_FILE="$PROJECT_DIR/vcpkg/scripts/buildsystems/vcpkg.cmake" ..; then
        echo -e "${GREEN}✅ CMake configuration successful${NC}"
        
        # Try to build, but don't fail completely if some targets fail
        cmake --build .
        
        # Check if at least AppConfig was built successfully
        if [ -f "AppConfig" ]; then
            echo -e "${GREEN}✅ AppConfig built successfully${NC}"
            cd "$PROJECT_DIR"
            return 0
        else
            echo -e "${RED}❌ AppConfig build failed${NC}"
            cd "$PROJECT_DIR"
            return 1
        fi
    else
        echo -e "${RED}❌ CMake configuration failed${NC}"
        cd "$PROJECT_DIR"
        return 1
    fi
}

# Function to run executable
run_app() {
    local app_name=$1
    local exe_name=$2
    
    if [ ! -f "$BUILD_DIR/$exe_name" ]; then
        echo -e "${RED}❌ $exe_name not found. Building first...${NC}"
        if ! build; then
            return 1
        fi
    fi
    
    echo -e "${BLUE}🚀 Running $app_name...${NC}"
    echo -e "${YELLOW}==================================================${NC}"
    cd "$BUILD_DIR"
    ./$exe_name
    local exit_code=$?
    cd "$PROJECT_DIR"
    echo -e "${YELLOW}==================================================${NC}"
    
    if [ $exit_code -eq 0 ]; then
        echo -e "${GREEN}✅ $app_name completed successfully${NC}"
    else
        echo -e "${RED}❌ $app_name failed${NC}"
    fi
    return $exit_code
}

# Function to smart build (only if needed)
smart_build() {
    # Check if AppConfig executable exists (don't require others)
    if [ -f "$BUILD_DIR/AppConfig" ]; then
        # Check if source files are newer than AppConfig executable
        local needs_rebuild=false
        
        for src_file in *.cpp *.h CMakeLists.txt *.yaml; do
            if [ -f "$src_file" ] && [ "$src_file" -nt "$BUILD_DIR/AppConfig" ]; then
                needs_rebuild=true
                break
            fi
        done
        
        if [ "$needs_rebuild" = true ]; then
            echo -e "${YELLOW}📁 Source files changed, rebuilding...${NC}"
            build
        else
            echo -e "${GREEN}✅ Build is up to date${NC}"
        fi
    else
        echo -e "${YELLOW}📁 No build found, building...${NC}"
        build
    fi
}

# Main command handler
case "${1:-dev}" in
    "dev"|"appconfig")
        smart_build && run_app "AppConfig" "AppConfig"
        ;;
    "main"|"app")
        smart_build && run_app "Main Application" "Main"
        ;;
    "telemetry")
        smart_build && run_app "TelemetryConfig" "TelemetryConfig"
        ;;
    # Example: Add new executable like this:
    # "newprogram"|"new")
    #     smart_build && run_app "NewProgram" "NewProgram"
    #     ;;
    "build")
        build
        ;;
    "setup")
        setup_dependencies
        ;;
    "clean")
        clean
        ;;
    "rebuild")
        clean && build
        ;;
    "test"|"all")
        if smart_build; then
            echo -e "${BLUE}🧪 Running all executables...${NC}"
            run_app "AppConfig" "AppConfig"
            echo
            run_app "TelemetryConfig" "TelemetryConfig"
            echo
            run_app "Main Application" "Main"
        fi
        ;;
    "help"|*)
        echo -e "${BLUE}📦 Telemetry System - npm-style runner${NC}"
        echo -e "${YELLOW}Usage: ./dev.sh [command]${NC}"
        echo
        echo -e "${GREEN}Commands:${NC}"
        echo -e "  ${BLUE}dev${NC}         - Smart build & run AppConfig (auto-setup deps)"
        echo -e "  ${BLUE}main${NC}        - Smart build & run Main application"
        echo -e "  ${BLUE}telemetry${NC}   - Smart build & run TelemetryConfig"
        echo -e "  ${BLUE}test${NC}        - Smart build & run all executables"
        echo -e "  ${BLUE}build${NC}       - Force build project (auto-setup deps)"
        echo -e "  ${BLUE}setup${NC}       - Setup vcpkg and install dependencies only"
        echo -e "  ${BLUE}clean${NC}       - Clean build directory"
        echo -e "  ${BLUE}rebuild${NC}     - Clean + build"
        echo
        echo -e "${YELLOW}Examples:${NC}"
        echo -e "  ./dev.sh           # Auto-setup & run AppConfig (first run)"
        echo -e "  ./dev.sh main      # Smart build & run Main"
        echo -e "  ./dev.sh setup     # Just setup dependencies"
        echo -e "  ./dev.sh rebuild   # Clean rebuild"
        echo
        echo -e "${GREEN}💡 First run automatically sets up vcpkg and installs yaml-cpp${NC}"
        echo -e "${GREEN}💡 Smart building only rebuilds when source files change${NC}"
        ;;
esac
