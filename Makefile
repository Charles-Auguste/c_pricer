# C++ commands
# ============
# Build the whole cmake project
build:
	@cmake . -B build_/
	@make -C build_/. -j 10

execute_test:
	@(cd build_/; ./run)

clean:
	@rm -rf build_/



