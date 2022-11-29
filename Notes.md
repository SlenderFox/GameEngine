Repo: https://github.com/SlenderFox/OpenGL-Engine  
Following: https://learnopengl.com  
Currently at: https://learnopengl.com/Advanced-OpenGL/Depth-testing
___
Notes:
- Enable/disable console through
	Properties>Linker>System>Subsystem & Properties>Linker>Advanced>Entry Point (mainCRTStartup)
- https://www.glfw.org/docs/latest/input_guide.html
- `git rm -rf --cached .` `git add .`
- https://github.com/irisengine/iris
- Look into: https://stackoverflow.com/questions/9321/how-do-you-create-a-static-class-in-c
___
Todo:
- [x] Add all source files as notes
	- [x] Construct inheritance graph
- [x] Make light models
- [x] Move lights into unique pointer vector
	- [x] Functionalise loading light data into shader
- [x] Fix textures loaded with model only using the first texture location
- [x] Stop texture loading into shader repeating
- [x] Currently modifying spotlight is hard-coded to a shader, try improving
- [x] Move to using entities
	- [x] Cube and backpack
	- [x] Lights
- [x] Multiple rotating cubes
- [x] Functionalise light model loading
	- [x] Add bool for model loading to include textures/one in shaders to ignore texture
	- [x] Add colour option in default shader
- [x] Test colour conversions
	- [x] Comment Colour
- [x] Remove depreciated code from Mesh
- [x] Convert lights into a derivation of Entity
- [x] Upgrade debug logging
	- [x] Add shader creation logging
	- [x] Make it clear and concise
- [x] Mark functions as noexcept and constexpr
- [x] Format switches and make nullptr checking nicer
- [x] Redo destruction to avoid passing parameters
- [x] Make shader owned by model
- [x] Use const references where possible
- [x] Fix ascii printing incorrectly as UTF
- [x] Add imgui
- [x] Formatting with 120 max width
- [x] Try upgrade Colour with macros
- [x] Try out reworking entire project on the premise of static engine
- [x] Make more functions pass by reference
- [x] Update Debug to be used without \_DEBUG enabled
- [ ] Alt releases cursor
- [ ] Make Debug use imgui
	- [ ] Can open an "event" then post whatever info to it and finally close it
- [ ] Overhaul lights
	- [ ] Have each light type in it's own vector
	- [ ] Limit lights to the same as in the shader
	- [ ] Overhaul how lights are rendered in the shader
		- [ ] Min and max clipped brightness
			- [ ] Iris, instant brightness response
			- [ ] Retina, slower brightness response
- [ ] Overload entity copy constructor to load model better
- [ ] Fully utilise the input class Input
	- [x] Basic key checking
	- [ ] Key state tracking
	- [ ] Key callbacks
	- [x] Mouse callback
	- [x] Scroll callback
	- [ ] ~~Allow callbacks to pass member functions~~
- [ ] Update destroying a single texture to allow for a new one to replace it
- [ ] Create a new project for unit tests, use constexpr asserts
- [ ] Update Texture
	- [ ] Overload equality operator for Texture
	- [ ] Update how unloading and cleanup is done
- [ ] Update Shader
	- [ ] Allow a new shader to be loaded in the same object
	- [ ] Make LoadShader() clearer to read
- [ ] Update Entity
	- [ ] Just use the regular constructor
	- [ ] Try to do something about the previous model when loading a new one
