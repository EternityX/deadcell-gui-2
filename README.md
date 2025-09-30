# DEADCELL-GUI-2

I will not provide any support for this. This is a learning resource.

### Event-driven UI library for gamehacking. ImGui handles the platform and rendering independent code.
Rewrite of [DEADCELL-GUI](https://github.com/EternityX/DEADCELL-GUI) but much more feature complete. This is purely GUI code, nothing else.

## Features
- Event-driven
- Flex-box-like layout model (using Meta's Yoga library)
- Support for multiple windows
- Modal/popout windows
- Blur shader for title bar (DX11 only)

## Usage
See [menu.cpp](https://github.com/EternityX/DEADCELL-GUI-2/blob/main/gui/menu.cpp) for an example. ImGui must be using the shadows branch.

## Todo (will probably never happen)
- Working example and build process
- Cut down on a lot of boilerplate code related to event handling in controls
- Additional controls
- General cleanup

## Preview
Images are slightly outdated but the general design is the same

![image](https://github.com/user-attachments/assets/cbd5292f-5a03-4b6b-88bf-d62b51a1024a)
![image](https://github.com/user-attachments/assets/41601c02-6915-4f0f-a70d-23054fdb705a)

## Thanks
- czapek for a lot of help with the layout and shader code
