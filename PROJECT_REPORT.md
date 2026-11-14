# Project Report: Smart City Simulation System

---

## 1. Project Description
The Smart City Simulation System is an advanced 2D graphical application developed using C/C++ and the `graphics.h` (WinBGIm) library. It simulates a modern, eco-friendly, and intelligent urban metropolis integrating autonomous transportation, renewable energy systems, dynamic environmental effects, and civic infrastructure. The system features a multi-layered parallax cityscape with real-time animations including continuous multi-lane autonomous vehicle traffic with zero-overlap collision physics, a high-tech roadside Smart Solar Bus Transit Station & Passenger Shelter with a live dynamic departure display (PIS), natural sidewalk pedestrian kinematics walking continuously along the X-axis with harmonic leg and arm swings, an elevated high-speed Maglev/bullet train with express boost, an autonomous aerial quadcopter delivery drone with spinning rotors, an animated Central Park water fountain with gravity physics, commercial jet aircraft with navigation strobes, rotating wind turbines, and rooftop solar arrays. It also simulates dynamic Day/Night lighting cycles and rain weather with ground splash ripples and pedestrian umbrellas. Users can interact with the application through comprehensive keyboard controls to switch environmental conditions, dispatch emergency vehicles with 5G V2X priority clearance, toggle traffic densities, and access real-time IoT telemetry diagnostics.

---

## 2. Tools and Technologies
● **Programming Language:** C++ (C++11 / C++17)  
● **Graphics Library:** graphics.h (WinBGIm for MinGW-w64 / GCC)  
● **IDE / Code Editor:** Visual Studio Code  
● **Compiler:** GCC (G++ / MinGW-w64)  
● **Header Files:** `<graphics.h>`, `<conio.h>`, `<vector>`, `<algorithm>`, `<cmath>`, `<cstdio>`, `<cstdlib>`, `<ctime>`, and `<windows.h>`  
● **Animation:** Hardware double-buffering page flipping (`setactivepage` / `setvisualpage`), frame-based particle kinematics, 2D trigonometric polygon transformations, harmonic walking oscillations, and parabolic fluid physics  
● **Operating System:** Windows (Windows 10 / 11)  
● **Build Tools:** Command-line compilation using PowerShell / MinGW-w64 G++  

---

## 3. Code
The project is implemented using C++ and the `graphics.h` library. The source code contains structured modular functions for procedural building generation, 2D trigonometric polygon transformations (rotating wind turbines and quadcopter rotors), zero-overlap autonomous vehicle spacing physics, sidewalk pedestrian walking kinematics along the X-axis, dynamic Day/Night atmospheric lighting, rain particle systems, and roadside smart transit station modeling. It utilizes hardware double-buffering (`activePage` swapping between 0 and 1) to eliminate visual flickering at 50–60 FPS. It also includes an event-driven keyboard controller to manage interactive simulation features.

**Main source file:** `smartcity.cpp`

```cpp
int main()
{
    srand((unsigned)time(NULL));

    // Initialize graphical window (1280x720 resolution)
    initwindow(
        W, 
        H, 
        "Smart City Animation - Advanced Urban Simulation | Computer Graphics",
        50,
        30
    );

    // Initialize simulation subsystems and particle arrays
    initStars();
    initClouds();
    initRain();
    initVehicles();
    initPedestrians();

    bool running = true;

    // Main real-time simulation and animation loop
    while (running)
    {
        // Render the complete frame using hardware double buffering
        renderFrame();

        // Interactive keyboard event controller
        if (kbhit())
        {
            char key = getch();

            switch (key)
            {
                case 27: // ESC: cleanly exit simulation
                    running = false;
                    break;

                case 'n':
                case 'N': // Toggle Day / Night lighting cycle
                    nightMode = !nightMode;
                    playAudioFeedback(600, 20);
                    break;

                case 'r':
                case 'R': // Toggle Rain weather simulation
                    rainMode = !rainMode;
                    playAudioFeedback(500, 20);
                    break;

                case 't':
                case 'T': // Toggle Bullet Train Express Boost
                    expressTrainActive = !expressTrainActive;
                    playAudioFeedback(900, 30);
                    break;

                case 'e':
                case 'E': // Dispatch Smart Ambulance with 5G V2X preemption
                    if (!ambulanceActive)
                    {
                        ambulanceActive = true;
                        ambulance.x = -160.0f;
                        ambulance.strobeTimer = 0;
                        playAudioFeedback(750, 40);
                    }
                    break;

                case 'f':
                case 'F': // Dispatch Fire Rescue Truck
                    if (!fireTruckActive)
                    {
                        fireTruckActive = true;
                        fireTruck.x = -200.0f;
                        fireTruck.strobeTimer = 0;
                        playAudioFeedback(450, 40);
                    }
                    break;

                case ' ': // Spacebar: City Audio Chime / Feedback
                    playAudioFeedback(650, 30);
                    break;

                case '1': // Traffic Density: Low (3 vehicles)
                    trafficDensity = 1;
                    initVehicles();
                    playAudioFeedback(550, 15);
                    break;

                case '2': // Traffic Density: Optimal (6 vehicles)
                    trafficDensity = 2;
                    initVehicles();
                    playAudioFeedback(650, 15);
                    break;

                case '3': // Traffic Density: Rush Hour (9 vehicles)
                    trafficDensity = 3;
                    initVehicles();
                    playAudioFeedback(750, 15);
                    break;

                case 'p':
                case 'P': // Toggle EV Fast-Charging Station Telemetry
                    evChargingActive = !evChargingActive;
                    playAudioFeedback(800, 20);
                    break;

                case 's':
                case 'S': // Toggle Acoustic Audio Feedback Chirps
                    soundEnabled = !soundEnabled;
                    if (soundEnabled) Beep(1000, 30);
                    break;

                case 'h':
                case 'H': // Toggle Diagnostics & Telemetry Modal Overlay
                    showDiagnostics = !showDiagnostics;
                    playAudioFeedback(700, 20);
                    break;
            }
        }

        delay(20); // Frame timing delay (~50 FPS)
    }

    closegraph();
    return 0;
}
```

---

## 5. Uses and Applications
● The Smart City Simulation System has several uses and applications:  
● **Educational Purpose:** Helps engineering and computer science students understand modern concepts of sustainable urban planning, clean renewable energy integration, and IoT-driven smart cities.  
● **Intelligent Transportation System (ITS) Visualization:** Demonstrates continuous multi-lane autonomous vehicle traffic flow, lane-based spacing physics, emergency vehicle 5G preemption, and high-tech roadside transit shelters.  
● **Computer Graphics Learning:** Serves as a practical benchmark for understanding 2D coordinate transformation, double-buffering page flipping, trigonometric vertex rotation, harmonic walking kinematics, and particle systems.  
● **Urban & Environmental Simulation:** Illustrates the visual transition between day and night environmental lighting, dynamic weather conditions (rain and road splash physics), and clean energy telemetry (solar and wind power generation).  
● **Programming Practice:** Enhances proficiency in C++, object state encapsulation, collision avoidance algorithms, queue wrap-around logic, and interactive event handling.  
● **Interactive Demonstration:** Allows users, educators, and evaluators to actively interact with the simulation using keyboard hotkeys to test real-time scenarios (weather changes, rush hour traffic, emergency dispatches, and diagnostic telemetry).  

---

## 6. Photo
*(Insert project screenshots here)*

- **Figure 1:** Daytime Overview of the Smart City (Continuous Autonomous Highway, Elevated Bullet Train, and Wind Turbines)  
- **Figure 2:** Smart Solar Bus Transit Station & Sidewalk Pedestrians walking along the X-Axis  
- **Figure 3:** Dynamic Night Mode (Glowing Architectural Windows, Streetlights, Starfield, and Vehicle Headlights)  
- **Figure 4:** Dynamic Rain Weather Simulation (Falling Rain Streaks, Ground Splash Ripples, and Umbrellas)  
- **Figure 5:** Emergency Vehicle Preemption System (5G V2X Priority Wave clearing traffic for Ambulance / Fire Engine)  
- **Figure 6:** IoT Telemetry HUD & Interactive Diagnostics Modal Overlay (`[H]` key)  

---

## 7. Conclusion
The Smart City Simulation System is a comprehensive and successful 2D computer graphics project developed using C++ and the `graphics.h` (WinBGIm) library. It brings to life a sustainable, high-tech urban environment through complex 2D animations, realistic mathematical models, and real-time user interactivity. Through this project, we gained extensive practical knowledge in computer graphics fundamentals, double-buffered rendering techniques, trigonometric polygon transformations, particle physics, and state-machine-driven event control. It significantly elevated our understanding of algorithmic problem-solving, structured C++ programming, and user-centric simulation design. Overall, the system serves as an engaging, educational, and visually impressive demonstration of next-generation smart city infrastructure and interactive computer graphics.
