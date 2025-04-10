## TODO
- Improve folder structure
- Separate sample game code from engine code

### Input system
- Support mapping and handling the same keys for different scopes/contexts/game states
- Handle mouse events

### Scripting system
- Expose more types and functions
- Make script variables configurable via scene file
- Address lua uint64 problem

### Configuration
- Change the format of config/scene files (probably JSON)
- Add config files to remove hard coded values

### Dev mode
- Entity editor
- Asset browser
- Log window
- In-game profiling

### Rendering
- UI rendering
- Tilemaps
- Particle system
- Fonts
- Support for multiple resolutions
- Custom rendering layers

### Events
- Stop resetting the EventBus on every update (implement removal of subscribers)

### Physics
- Add Box2D physics library
- Add colliders with other shapes

### Audio
- Add an audio library
