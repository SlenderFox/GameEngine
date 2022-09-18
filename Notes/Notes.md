Repo: https://github.com/SlenderFox/OpenGL-Engine
Following: https://learnopengl.com/
Currently at: https://learnopengl.com/Model-Loading/Model
___
Notes:
- Enable/disable console through
	Properties>Linker>System>Subsystem & Properties>Linker>Advanced>Entry Point (mainCRTStartup)
___
Todo:
- [ ] Add all source files and assets as notes
	- [ ] Construct inheritance graph
- [x] Make light models
- [x] Move lights into unique pointer vector
	- [x] Functionalise loading light data into shader
- [x] Fix textures loaded with model only using the first texture location
- [x] Stop texture loading into shader repeating
- [ ] Update destroying a single texture to allow for a new one to replace it
- [x] Currently modifying spotlight is hard-coded to a shader, try improving
- [ ] Near and far planes affected by fov
- [x] Move to using entities
	- [x] Cube and backpack
	- [x] Lights
- [x] Multiple rotating cubes
- [ ] Functionalise light model loading
	- [x] Add bool for model loading to include textures/one in shaders to ignore texture
	- [x] Add colour option in default shader
- [x] Test colour conversions
	- [x] Comment Colour.hpp
- [x] Remove depreciated code from Mesh
- [ ] Convert lights into a derivation of Entity
- [ ] Make debug logging more concise and clear
- [ ] Mark functions as noexcept
- [ ] Overload entity copy constructor to load model better