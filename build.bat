g++ -std=c++11 -o main.exe libs\\XMLAttribute.cpp libs\\XMLElement.cpp libs\\XMLStructure.cpp libs\\XMLReader.cpp libs\\XMLValidator.cpp main.cpp
if %errorlevel% == 0 (
	echo Compilation completed successfully!
) else (
	pause
)