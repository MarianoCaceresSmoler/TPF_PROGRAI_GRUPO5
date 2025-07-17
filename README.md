
# 🛸 Space Invaders - Final Project for Programming I

This is a complete **Space Invaders clone** written in **C**, designed to run both on **PC** (using Allegro for graphics/audio) and on a **Raspberry Pi** (using a 16x16 matrix display and joystick). It was developed as the **Final Project** for the *Programación I* course at ITBA (Instituto Tecnologico de Buenos Aires).

---

## 🎯 Objectives

The goal of this project was to design a **modular, cross-platform** game that reimplements the classic Space Invaders with:

- 💻 PC version: graphical interface using Allegro 5.
- 🍓 Raspberry Pi version: using a 16x16 matrix LED display and a joystick as controller.

---

## ✅ Core Features (Mandatory Requirements)

- 👾 **Full game loop**: Control the ship, fire bullets, destroy aliens, barriers, and the mothership.
- 🧠 **Basic game logic**: Enemies move, shoot, and die. Bullets collide with aliens and obstacles.
- 🛡️ **Barriers**: Gradually deteriorate when hit by bullets.
- 🛸 **Mothership**: Occasionally appears and plays a sound while active.
- 🔥 **Difficulty**: Alien speed increases as time passes and as their numbers decrease.
- 💯 **Score tracking**: Points are awarded per alien and mothership destroyed. Displayed at the end of the game.
- ❤️ **Lives system**: Player loses a life upon getting hit. Gaining lives by completing levels.
- ⏸️ **Pause/Resume/Restart**: Game can be paused, restarted, or exited at any time.
- 📜 **Start and pause menu**: Basic menu system for user navigation.

---

## 🌟 Optional Features (Implemented)

- 🔊 **Sound and music**: Original-style sound effects and background music using Allegro's audio or Raspberry Pi libraries.
- 🧠 **Power-ups**: Multiple power-ups with timers and visual feedback.
- 🥇 **Animations**: Sprite animation for alien movement and explosions.
- 🎨 **Visual polish**: Sliding text animations, transitions, and fade-in menus.
- 🔁 **Mothership behavior**: Custom logic for entering/exiting screen and sound management.
- 🎮 **Joystick support** (Raspberry Pi): Input logic includes directional control and press/hold button combinations.
- 🧱 **Crossfade background video** (PC): Menu features animated video background with crossfade effect.
- 🧪 **Robust validation**: Defensive programming.
- 📜 **Personalizaed compilation**: using make to automatically detect and compile depending on the platform.

---

## ⚙️ Compilation

### On PC (Linux with Allegro installed):

In the root folder of the project
make
./game_pc

### On Raspberry Pi:

in the root folder of the project
make
./game_rpi

> Ensure the required display and joystick libraries for Raspberry Pi are installed.

---

## 🎮 Controls

| Action         | PC Keyboard       | Raspberry Pi Joystick            |
|----------------|-------------------|----------------------------------|
| Move Left      | ←                 | Tilt joystick left               |
| Move Right     | →                 | Tilt joystick right              |
| Shoot          | Space             | Short button press               |
| Pause/Resume   | P / Enter         | Long button press                |
| Restart        | R                 | Joystick up + press              |
| Exit           | ESC               | Joystick down + press            |

---

## 📷 Images In-Game
<img width="1920" height="972" alt="image" src="https://github.com/user-attachments/assets/963a5f1c-84a7-45b4-bcea-184b92e1979d" />
<img width="1920" height="972" alt="image" src="https://github.com/user-attachments/assets/11fa6a6b-0494-4e1e-a097-c934b1f703ed" />
<img width="1920" height="972" alt="image" src="https://github.com/user-attachments/assets/b629b570-be48-4cd9-a6ae-17e668b879e7" />
<img width="1920" height="972" alt="image" src="https://github.com/user-attachments/assets/ac480219-a814-417d-a768-495de7b7c302" />

---

## 👥 Authors

- **Mariano Cáceres Smoler**
- **Alex Petersen**
