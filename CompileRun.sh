#!/bin/bash

# Get full path
PROJECT_DIR="$(cd "$(dirname "$0")"; pwd)"
SRC_DIR="$PROJECT_DIR/src"
OBJ_DIR="$PROJECT_DIR/obj"
BIN_DIR="$PROJECT_DIR/bin"
EXE="$BIN_DIR/eVotingSystem.exe"

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

mkdir -p "$OBJ_DIR"
mkdir -p "$BIN_DIR"

echo -e "${GREEN}🔄 Compiling changed files only...${NC}"

# Compile changed files only
find "$SRC_DIR" -name "*.cpp" | while read -r cpp_file; do
    rel_path="${cpp_file#$SRC_DIR/}"
    obj_file="$OBJ_DIR/${rel_path%.cpp}.o"
    mkdir -p "$(dirname "$obj_file")"

    if [ ! -f "$obj_file" ] || [ "$cpp_file" -nt "$obj_file" ]; then
        echo "📦 Compiling: $rel_path"
        g++ -std=c++17 -I"$PROJECT_DIR/include" -c "$cpp_file" -o "$obj_file"
        if [ $? -ne 0 ]; then
            echo -e "${RED}Compilation failed: $rel_path${NC}"
            exit 1
        fi
    fi
done

echo -e "${GREEN}🔗 Linking object files...${NC}"
g++ "$OBJ_DIR"/**/*.o "$OBJ_DIR"/*.o -o "$EXE"
if [ $? -ne 0 ]; then
    echo -e "${RED}Linking failed.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Build complete.${NC}"
echo -e "${GREEN}🚀 Running your program now...${NC}"
echo

# Run program in same terminal
"$EXE"

echo -e "\n${GREEN}🏁 Program finished. Press enter to exit.${NC}"
read
