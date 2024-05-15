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


# Cmake things
# ============

configure:
	@cmake . -B build_/

clean:
	@echo "Cleaning ..."
	rm -rf build_/

build_project: 
	@echo "Building ..."
	@cd build_/ ; cmake --build . --config Release -j 10


# Code Quality
# ============

format_code:
	@echo "Format code ..."
	@find ./src -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) -exec sh -c 'printf "%s\n" "$$0"; clang-format -i "$$0"' {} \;
	@find ./MonteCarloEngine -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) -exec sh -c 'printf "%s\n" "$$0"; clang-format -i "$$0"' {} \;


# Unitest
# =======

test:
	@echo "testing ..."
	@cd build_/; ./test





