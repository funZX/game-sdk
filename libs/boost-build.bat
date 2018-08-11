cd boost
call bootstrap.bat
b2 toolset=msvc-14.0 address-model=32  --build-dir=build -link=static -threading=multi 