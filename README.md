# 🌆 Smart City Animation - Advanced 2D Urban Simulation
> **Computer Graphics Project** | Implemented in **C/C++** using **WinBGIm (`graphics.h`)**

---

## 📌 Project Overview
This project is an advanced, realistic **2D Smart City Simulation and Animation** designed to demonstrate real-world urban dynamics, renewable energy integration, intelligent transportation systems (ITS), dynamic environmental lighting, and pedestrian safety mechanisms.

Taking inspiration from modern eco-metropolises and sustainable smart cities, this simulation is engineered with a **multi-layered parallax architecture**, **100% flicker-free double-buffered rendering (50-60 FPS)**, and a **centralized IoT Telemetry HUD**.

---

## 🎯 Alignment with Expected Learning Outcomes

| # | Learning Outcome | Implementation in Code |
|---|-------------------|------------------------|
| **1** | **Apply 2D graphics concepts** | • Screen coordinate mapping ($1280 \times 720$ resolution)<br>• Multi-layer depth staging (Sky $\rightarrow$ Distant Skyline $\rightarrow$ Renewable Energy Hill $\rightarrow$ High-Rise Landmarks $\rightarrow$ Elevated Viaduct $\rightarrow$ Sidewalk $\rightarrow$ Highway $\rightarrow$ HUD)<br>• Flicker-free hardware page swapping (`setactivepage` / `setvisualpage`)<br>• Atmospheric color grading (Day, Night, and Stormy Overcast palettes) |
| **2** | **Implement object animation & movement** | • Continuous horizontal translation with smooth wrap-around (`planeX`, `clouds`, `trainX`)<br>• Multi-lane vehicular flow with independent speeds and acceleration<br>• Dynamic rain droplet vectors and expanding road splash ripple ellipses<br>• Express train speed boost with trailing motion lines<br>• Autonomous Aerial Delivery Quadcopter Drone and E-Scooter commuter animation |
| **3** | **Develop traffic signal logic** | • **High-Tech Smart Highway Gantry (ITS)**: Replaces traditional clunky poles with overhead electronic dynamic lane displays (`[ 60 ]`, `[ 50 ]`, `[ BUS ]`, Green Arrow `v`, Caution `!`, Hold `X`)<br>• **Photoluminescent LED Crosswalk**: Ground-embedded neon illumination strips active during pedestrian crossing<br>• **Emergency Vehicle Preemption**: Auto-overrides to green wave for approaching Ambulance / Fire Engine<br>• Operator manual override via `[SPACE]` key |
| **4** | **Use graphics primitives & transformations** | • **2D Trigonometric Rotation**: Rotating 3-blade wind turbine propellers using $\theta = \theta_0 + k\frac{2\pi}{3}$ and polygon vertex transformations<br>• **Quadcopter Drone Rotors**: 4 spinning propeller blades using angular trigonometric rotation<br>• **Kinematic Walking Harmonic Oscillations**: Pedestrian leg/arm swing angles using $\theta = A \sin(\omega t)$<br>• **Parabolic Fluid Dynamics**: Central Park fountain water droplets spraying with gravity acceleration<br>• **Polygon Transformations**: Aerodynamic bullet train nose cone, jet aircraft wings/tail, tapered turbine towers, and conical light beams |
| **5** | **Build a real-world simulation using C/C++** | • Integrated IoT Control Center HUD displaying real-time clock, AQI (Air Quality Index), clean energy grid wattage (Solar + Wind), and traffic efficiency index<br>• Smart EV Fast-Charging Bay with pulsing green charging LED<br>• Full interactive diagnostics modal overlay (`[H]` key)<br>• Non-blocking audio feedback chirps (`[S]` key) |

---

## 🌟 Unique Smart City Features & Solved Overlap Physics

### 1. 🛡️ 100% Zero-Overlap Vehicle Physics (Completely Solved)
- **Dedicated Lane Distribution**:
  - *Lane 0 (Fast Lane)*: Aerodynamic Sports EV Coupe & Pearl EV Sedans ($3.5\text{ px/f}$).
  - *Lane 1 (Autonomous Transit)*: Yellow Robotaxis with spinning LiDAR pods & Electric Sedans ($3.0\text{ px/f}$).
  - *Lane 2 (Clean Logistics & Transit)*: Electric City Bus & Eco Delivery Trucks ($2.4\text{ px/f}$).
- **Hard Spacing Clamping**:
  - In each lane, if a car gets within minimum safe distance ($L_1/2 + L_2/2 + 28\text{px}$), its position is **strictly clamped** behind the lead car, and its speed is matched.
  - It is **mathematically impossible** for cars to penetrate, overlap, or glitch through each other.
- **Safe Circular Queue Wraparound**:
  - When a car reaches the right edge ($X > 1420$), it searches for the rearmost car in that lane and respawns safely behind it with at least $150\text{px}$ of buffer space.

### 2. 🚏 Smart Solar Bus Transit Station & Passenger Shelter
- High-tech roadside passenger hub located along the sidewalk:
  - **Dynamic Overhead LED Transit Display (PIS)**: Real-time animated arrival ticker alternating bus routes (`BUS 101: 2m`, `EXP 502: 5m`, `V2X OPTIMAL`).
  - **Cantilever Solar Glass Canopy**: Dark aerodynamic shelter frame with integrated photovoltaic solar roof cells and neon cyan rim trim.
  - **Tempered Frosted Windscreen & Hardwood Bench**: Resting passenger bench nestled safely under the shelter.
  - **Interactive 5G Route Totem**: Digital route column with glowing touch screen and blinking antenna beacon.
  - **Night Illumination**: Warm ceiling LED strip with downward ambient light cone casting over the sidewalk pavement.

### 3. 🚶 Sidewalk Pedestrian Kinematics (Natural X-Axis Walk)
- Citizens walk naturally along the sidewalk along the horizontal X-axis:
  - **Bi-directional Flow**: Two separate parallel walking paths (walking right at `Y = 508`, walking left at `Y = 497`) ensure pedestrians in opposite directions never collide.
  - **Harmonic Kinematic Oscillations**: Real-time arm and leg swing physics governed by sinusoidal functions ($\theta = A \sin(\omega t)$).
  - **Zero-Overlap Spacing Protection**: Pedestrians dynamically adjust walking speeds to maintain safe spacing buffer.
  - **Dynamic Rain Umbrellas**: In rainy mode, pedestrians hold color-coded protective umbrellas.

### 4. 🚁 Autonomous Aerial Delivery Drone
- Futuristic quadcopter drone flying between skyscrapers at altitude $Y \approx 205$.
- Features 4 rotating propeller blades, autonomous optical sensor eye, red/green navigation strobe LEDs, and an attached yellow **Smart Delivery Parcel Pod**.

### 5. 📺 Scrolling Digital LED Billboard on Tech Tower
- High-tech curved electronic video marquee on the AI Data Tower displaying real-time scrolling urban news:
  `"⚡ SMART CITY 2026 * 100% CLEAN ENERGY * ZERO CARBON * IOT V2X ACTIVE * AUTONOMOUS TRANSIT OPTIMAL"`

### 6. ⛲ Central Park Animated Water Fountain
- Multi-tier civic stone fountain with animated water droplets spraying in parabolic arcs with realistic gravity physics ($v_y += g$) and splashing back into the ripple pool.

### 7. 🛴 Smart Commuter on Electric Scooter (E-Scooter)
- Citizen riding an electric kick-scooter along the sidewalk with spinning alloy wheels, LED headlight, and safety helmet.

### 8. 🚨 5G V2X Emergency Priority Broadcast Wave
- When an Ambulance (`[E]`) or Fire Truck (`[F]`) is dispatched, the city grid activates **5G Emergency Priority Preemption**:
  - Animated 5G wireless waves broadcast from the vehicle to the gantry.
  - The gantry grants instant green clearance to clear the highway.

### 9. 🚅 Elevated High-Speed Metro / Bullet Train (Maglev)
- Viaduct track spanning the city with an aerodynamic bullet train. Press **`[T]`** for **Express Boost** with speed lines!

### 10. ♻️ Smart Solar Waste Compactor Bins & EV Fast Charging
- Automated solar-compacting smart waste bins with fill-level LEDs.
- Dual EV fast-charging bay with cable and pulsing green battery indicator.

---

## 🎮 Interactive Controls (Hotkeys)

| Key | Action / Function |
|:---:|:---|
| **`N`** | Toggle **Day / Night Cycle** (streetlamps, headlights, window grids, stars) |
| **`R`** | Toggle **Rain Weather** (falling streaks, water splash ripples, umbrellas) |
| **`T`** | Toggle **Bullet Train Express Boost** (speed lines & launch) |
| **`E`** | Dispatch **Smart Ambulance** (5G V2X green priority wave) |
| **`F`** | Dispatch **Fire Rescue Truck** (ladder engine & emergency clearance) |
| **`SPACE`** | **City Audio Chime / Feedback** (acoustic signal feedback chirp) |
| **`1`** | Set **Low Traffic** (1 vehicle per lane, 3 total) |
| **`2`** | Set **Optimal Traffic** (2 vehicles per lane, 6 total) |
| **`3`** | Set **Rush Hour Traffic** (3 vehicles per lane, 9 total) |
| **`P`** | Toggle **Smart EV Fast-Charging Station** (pulsing battery charge LED) |
| **`S`** | Toggle **Audio Sound Feedback** (satisfying acoustic chirps) |
| **`H`** | Open / Close **Diagnostics & Telemetry Information Modal** |
| **`ESC`** | **Exit** Simulation cleanly |

---

## 🛠️ How to Compile and Run

### Run the Compiled Executable
```powershell
.\smartcity.exe
```

### Recompile Anytime with MinGW-w64
```powershell
C:\msys64\mingw64\bin\g++.exe smartcity.cpp -o smartcity.exe -I C:\msys64\mingw64\include C:\msys64\mingw64\lib\libbgi.a -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
```
