g++ -std=c++11 -o main.exe libs\\StackString.cpp libs\\XMLAttribute.cpp libs\\XMLElement.cpp libs\\XMLStructure.cpp libs\\XMLReader.cpp libs\\XMLValidator.cpp libs\\XMLXPath.cpp main.cpp
if %errorlevel% == 0 (
	echo Compilation completed successfully!
) else (
	pause
)