# =============================================================================
# Global MakeFile of the project
# These instructions are meant to help building and installing the program.
# 
# Author: Charles-Auguste GOURIO
#
# To run a command on a terminal, please use:
#
# $ make <command>
#
# =============================================================================

setup:
	@sudo apt install clang-format
	@sudo apt-get install libgtest-dev
	@cd /usr/src/gtest; sudo cmake CMakeLists.txt; sudo make
	@cd /usr/src/gtest; sudo cp -r include/gtest /usr/local/include/.
	@cd /usr/src/gtest; sudo cp -r lib/*.a /usr/local/lib/.


# Cmake things
# ============

configure:
	@cmake . -B build/

clean:
	@echo "Cleaning ..."
	rm -rf build/

build_project: 
	@echo "Building ..."
	@cd build/ ; cmake --build . --config Release -j 10


# Code Quality
# ============

format_code:
	@echo "Format code ..."
	@find ./src -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) -exec sh -c 'printf "%s\n" "$$0"; clang-format -i "$$0"' {} \;
	
	
# Unitest
# =======

test:
	@echo "testing ..."
	@cd build/; ./test





