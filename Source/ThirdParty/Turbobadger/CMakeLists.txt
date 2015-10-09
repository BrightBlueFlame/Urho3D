#
# Copyright (c) 2008-2015 the Urho3D project.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

# Define target name
set (TARGET_NAME TurbobadgerUI)

if (APPLE)
	add_definitions(-DMACOSX)
	add_definitions(-Dnullptr=0)
endif (APPLE)

set (CPP_DIRS
	src/tb
	src/tb/animation
	src/tb/image
	src/tb/parser
	src/tb/renderers
	src/tb/utf8)

file (GLOB CPP_FILES
	src/tb/*.cpp 
	src/tb/animation/*.cpp 
	src/tb/image/*.cpp 
	src/tb/parser/*.cpp 
	src/tb/renderers/*.cpp 
	src/tb/utf8/*.cpp)



file (GLOB H_FILES
	src/tb/*.h 
	src/tb/animation/*.h 
	src/tb/image/*.h 
	src/tb/parser/*.h 
	src/tb/renderers/*.h 
	src/tb/thirdparty/*.h 
	src/tb/utf8/*.h)

set (SOURCE_FILES ${CPP_FILES} ${H_FILES})

# Define dependency libs
set (INCLUDE_DIRS
	src/tb
	src/tb/animation
	src/tb/image
	src/tb/parser
	src/tb/renderers
	src/tb/thirdparty
	src/tb/utf8)


# Setup target
setup_library (STATIC)

# Install headers for building the Urho3D library
#install_header_files (DIRECTORY include/ DESTINATION ${DEST_INCLUDE_DIR}/ThirdParty/Detour FILES_MATCHING PATTERN *.h BUILD_TREE_ONLY)  # Note: the trailing slash is significant
install_header_files (DIRECTORY src/tb/ DESTINATION ${DEST_INCLUDE_DIR}/ThirdParty/tb FILES_MATCHING PATTERN *.h BUILD_TREE_ONLY)
