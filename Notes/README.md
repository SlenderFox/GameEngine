Repo: https://github.com/SlenderFox/OpenGL-Engine<br>
Following: https://learnopengl.com/<br>
Currently at: https://learnopengl.com/Model-Loading/Model
___
Notes:
- Enable/disable console through
	Properties>Linker>System>Subsystem & Properties>Linker>Advanced>Entry Point (mainCRTStartup)
- https://www.glfw.org/docs/latest/input_guide.html
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
	- [x] Comment [[Colour.hpp]]
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
- [x] Try upgrade ([[Colour.hpp]]/[[Colour.cpp]]) with macros
- [ ] Make more functions pass by reference
- [ ] Holding alt releases cursor
- [ ] Make ([[Debug.hpp]]/[[Debug.cpp]]) use imgui
	- [ ] Update ([[Debug.hpp]]/[[Debug.cpp]]) to be used without \_DEBUG enabled
- [ ] Overhaul lights ([[Light.hpp]]/[[Light.cpp]])
	- [ ] Have each light type in it's own vector
	- [ ] Limit lights to the same as in the shader
	- [ ] Overhaul how lights are rendered in the shader
		- [ ] Min and max clipped brightness
			- [ ] Iris
			- [ ] Retina
- [ ] Overload entity copy constructor to load model better
- [x] Actually use the input class ([[Input.hpp]]/[[Input.cpp]])
	- [x] Keyboard input
		- [ ] Keyboard layouts
	- [x] Mouse input
	- [ ] Function callbacks
		- [ ] Allow callbacks to pass member functions
- [ ] Update destroying a single texture to allow for a new one to replace it
