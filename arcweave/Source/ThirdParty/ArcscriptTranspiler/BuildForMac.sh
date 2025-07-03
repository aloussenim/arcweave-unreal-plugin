# Generic mac support for the Arcrscript transpiler is built here.
#
# This basic script could be made into a single xcodeproj file that handles dependencies.
# For the sake of expediency, this script handles a simple build of all of arcweave's
# dependencies and writes them into the Mac/Release folder.

# build the dynamic library for mac
xcodebuild -project antlr4-runtime/antlrcpp.xcodeproj -target antlr4 -configuration Release ARCHS="arm64 x86_64"

#build the static library
xcodebuild -project antlr4-runtime/antlrcpp.xcodeproj -target antlr4_static -configuration Release ARCHS="arm64 x86_64"

# and finally, the transpiler.
xcodebuild -scheme ArcscriptTranspiler -project ArcscriptTranspiler/ArcscriptTranspiler.xcodeproj -configuration Release ARCHS="arm64 x86_64"
