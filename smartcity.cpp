// ============================================================================
//               SMART CITY SIMULATION - ADVANCED 2D ANIMATION
//               Computer Graphics Project | C/C++ & WinBGIm (graphics.h)
// ============================================================================
// Features:
// 1. High-Tech Smart Highway Gantry & Autonomous V2X Sensor Grid (replaces old traffic signals)
// 2. Multi-Lane Autonomous Highway with Zero-Overlap Hard Spacing Physics
// 3. Smart Photoluminescent LED-Embedded Crosswalk (Active Neon Pavement Strips)
// 4. Autonomous Aerial Delivery Quadcopter Drone with 4 Spinning Rotors & Parcel
// 5. Scrolling Digital LED Message Billboard / Video Wall on Tech Tower
// 6. Central Park Plaza with Animated Water Fountain (Parabolic Gravity Physics)
// 7. Smart AI 5G Street Poles with Integrated CCTV Domes & Conical Light Beams
// 8. Smart Commuter on Electric Scooter (E-Scooter) with Spinning Wheels & Headlight
// 9. Elevated High-Speed Metro / Bullet Train (Maglev) with Express Boost ([T])
// 10. Renewable Energy Grid: 2D Trigonometric Rotating Wind Turbines & Rooftop Solar
// 11. Commercial Jet Aircraft with Navigation Strobes & Vapor Contrails
// 12. Smart EV Fast-Charging Bay with Pulsing Battery Telemetry ([P])
// 13. Dynamic Day/Night Cycle ([N]) & Rain Weather Simulation with Road Ripples ([R])
// 14. Emergency Preemption System with 5G V2X Priority Wave Broadcast ([E] / [F])
// 15. Audio Feedback Sound Effects Toggle ([S]) & Diagnostics Overlay Modal ([H])
// ============================================================================

#include <graphics.h>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

// ============================================================================
// CONSTANTS & DIMENSIONS
// ============================================================================
const int W = 1280;
const int H = 720;

const int TOP_HUD_H     = 38;
const int SKY_TOP       = 38;
const int SKY_BOTTOM    = 480;

const int VIADUCT_Y     = 426;
const int VIADUCT_H     = 16;

const int SIDEWALK_TOP  = 480;
const int SIDEWALK_BOT  = 525;
const int ROAD_TOP      = 525;
const int ROAD_BOT      = 675;
const int BOTTOM_HUD_Y  = 676;

const int STOP_LINE_X   = 345;
const int GANTRY_X      = 405;
const int CROSSWALK_X1  = 358;
const int CROSSWALK_X2  = 425;

const int LANE_Y[3]       = { 552, 600, 650 };
const float LANE_SCALE[3] = { 0.82f, 0.96f, 1.10f };

const double PI = 3.14159265358979323846;

// ============================================================================
// SIMULATION STATE & TELEMETRY
// ============================================================================
bool nightMode          = false;
bool rainMode           = false;
int  trafficDensity     = 2;          // 1: Low, 2: Med, 3: High
bool ambulanceActive    = false;
bool fireTruckActive    = false;
bool expressTrainActive = false;
bool evChargingActive   = true;
bool showDiagnostics    = false;
bool soundEnabled       = true;

// Time & Telemetry Counters
unsigned long frameCount = 0;
int clockHours           = 14;
int clockMinutes         = 28;
int clockSeconds         = 0;

// Renewable Energy Generation (kW)
int solarOutputKW        = 640;
int windOutputKW         = 780;

// ============================================================================
// AUTONOMOUS TRAFFIC MANAGEMENT STATE (SMART GANTRY & V2X)
// ============================================================================
enum GantryState {
    GANTRY_FLOW,       // All lanes flowing freely (Green Arrow / Speed limit 60)
    GANTRY_WARNING,    // Approaching pedestrian crossing or slow-down (Yellow !)
    GANTRY_PEDESTRIAN  // Crosswalk active: Stop line active, ground LEDs neon cyan/red
};

GantryState gantryState = GANTRY_FLOW;
int gantryTimer = 0;

const int DURATION_FLOW      = 220; // ~5.5 seconds flowing
const int DURATION_WARNING   = 45;  // ~1.1 seconds caution
const int DURATION_CROSSING  = 160; // ~4.0 seconds pedestrian crossing

// ============================================================================
// DATA STRUCTURES
// ============================================================================
struct Vehicle {
    float x;
    int   lane;
    float speed;
    float maxSpeed;
    int   type;          // 0: Sedan, 1: Bus, 2: Eco Truck, 3: Robotaxi, 4: Coupe
    int   bodyColor;
    int   accentColor;
    bool  isBraking;
    float length;
};

struct EmergencyVehicle {
    float x;
    int   lane;
    float speed;
    int   strobeTimer;
    bool  active;
};

struct Pedestrian {
    float x;
    float y;
    float speed;
    int   shirtColor;
    int   pantColor;
    int   skinColor;
    int   umbrellaColor;
    float walkPhase;
    bool  crossing;
    bool  waiting;
    int   direction; // 1: down, -1: up
};

struct Cloud {
    float x;
    float y;
    float speed;
    int   radius;
};

struct Star {
    int x;
    int y;
    int twinkleOffset;
};

struct RainDrop {
    float x;
    float y;
    float speed;
    int   length;
};

struct RoadRipple {
    int   x;
    int   y;
    int   radius;
    int   maxRadius;
    int   age;
};

struct WaterDrop {
    float x, y, vx, vy;
};

// Global Collections
vector<Vehicle>      vehicles;
vector<Pedestrian>   pedestrians;
vector<Cloud>        clouds;
vector<Star>         stars;
vector<RainDrop>     rainDrops;
vector<RoadRipple>   ripples;
vector<WaterDrop>    fountainDrops;

EmergencyVehicle ambulance = { -160.0f, 1, 5.8f, 0, false };
EmergencyVehicle fireTruck = { -200.0f, 0, 5.2f, 0, false };

// Wind Turbine Propeller State
float windTurbineAngle = 0.0f;

// Elevated Metro Train State
float trainX = -260.0f;
float trainBaseSpeed = 3.6f;

// Passenger Aircraft State
float planeX = -120.0f;
float planeY = 92.0f;

// Autonomous Aerial Delivery Drone State
float droneX = 220.0f;
float droneY = 205.0f;
float droneSpeed = 1.4f;
int   droneDirection = 1;
float droneRotorAngle = 0.0f;

// Smart Commuter E-Scooter State
float scooterX = 60.0f;

// Marquee Billboard Ticker Scroll
int marqueeScroll = 0;

// Double Buffering Page
int activePage = 0;

// ============================================================================
// AUDIO FEEDBACK HELPER
// ============================================================================
void playAudioFeedback(int freq, int durationMs) {
    if (soundEnabled) {
        Beep(freq, durationMs);
    }
}

// ============================================================================
// GRAPHICS HELPER PRIMITIVES
// ============================================================================
void filledRect(int x1, int y1, int x2, int y2, int fillColor, int borderColor = -1) {
    setfillstyle(SOLID_FILL, fillColor);
    if (borderColor == -1)
        setcolor(fillColor);
    else
        setcolor(borderColor);
    bar(x1, y1, x2, y2);
    if (borderColor != -1)
        rectangle(x1, y1, x2, y2);
}

void filledCircle(int x, int y, int radius, int fillColor, int borderColor = -1) {
    setfillstyle(SOLID_FILL, fillColor);
    if (borderColor == -1)
        setcolor(fillColor);
    else
        setcolor(borderColor);
    fillellipse(x, y, radius, radius);
}

void drawText(int x, int y, const char* str, int textColor = WHITE, int fontSize = 1) {
    setcolor(textColor);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, fontSize);
    outtextxy(x, y, (char*)str);
}

float getVehicleLength(int type, int lane) {
    if (type == 1) return 115.0f * LANE_SCALE[lane]; // Bus
    if (type == 2) return 92.0f  * LANE_SCALE[lane]; // Truck
    return 62.0f * LANE_SCALE[lane];                 // Cars
}

// ============================================================================
// INITIALIZATION ROUTINES
// ============================================================================
void initStars() {
    stars.clear();
    int count = 45;
    for (int i = 0; i < count; i++) {
        Star s;
        s.x = (i * 29 + 17) % (W - 20) + 10;
        s.y = (i * 37 + 11) % (SKY_BOTTOM - 200) + SKY_TOP + 10;
        s.twinkleOffset = (i * 7) % 20;
        stars.push_back(s);
    }
}

void initClouds() {
    clouds.clear();
    Cloud c1 = { 120.0f,  75.0f, 0.40f, 26 };
    Cloud c2 = { 460.0f, 110.0f, 0.30f, 32 };
    Cloud c3 = { 820.0f,  68.0f, 0.45f, 28 };
    Cloud c4 = { 1100.0f, 125.0f, 0.35f, 24 };
    clouds.push_back(c1);
    clouds.push_back(c2);
    clouds.push_back(c3);
    clouds.push_back(c4);
}

void initRain() {
    rainDrops.clear();
    for (int i = 0; i < 160; i++) {
        RainDrop r;
        r.x = rand() % W;
        r.y = rand() % (ROAD_BOT - SKY_TOP) + SKY_TOP;
        r.speed = 10.0f + (rand() % 8);
        r.length = 10 + (rand() % 6);
        rainDrops.push_back(r);
    }
}

// Fixed, robust, zero-overlap vehicle placement per lane
void initVehicles() {
    vehicles.clear();
    int perLane = (trafficDensity == 1) ? 1 : (trafficDensity == 2 ? 2 : 3);

    for (int lane = 0; lane < 3; lane++) {
        float laneSpacing = (float)W / perLane;
        for (int i = 0; i < perLane; i++) {
            Vehicle v;
            v.lane = lane;
            // Stagger start X positions between lanes so vehicles never spawn near each other
            v.x = i * laneSpacing + (lane * 95.0f);

            if (lane == 0) { // Fast Lane: Sports EV Coupe or Modern EV Sedan
                v.type = (i % 2 == 0) ? 4 : 0;
                v.maxSpeed = 3.5f;
                v.bodyColor = (i % 2 == 0) ? COLOR(220, 38, 38) : COLOR(240, 240, 245);
            } else if (lane == 1) { // Autonomous Transit Lane: Robotaxi or EV Sedan
                v.type = (i % 2 == 0) ? 3 : 0;
                v.maxSpeed = 3.0f;
                v.bodyColor = (i % 2 == 0) ? COLOR(255, 193, 7) : COLOR(30, 144, 255);
            } else { // Clean Commercial Lane: Electric Transit Bus or Eco Logistics Truck
                v.type = (i % 2 == 0) ? 1 : 2;
                v.maxSpeed = 2.4f;
                v.bodyColor = (i % 2 == 0) ? COLOR(0, 168, 204) : COLOR(56, 142, 60);
            }

            v.accentColor = WHITE;
            v.speed = v.maxSpeed;
            v.isBraking = false;
            v.length = getVehicleLength(v.type, v.lane);
            vehicles.push_back(v);
        }
    }
}

void initPedestrians() {
    pedestrians.clear();
    int count = 5;
    int shirts[5] = { COLOR(30, 136, 229), COLOR(229, 57, 53), COLOR(67, 160, 71), COLOR(251, 140, 0), COLOR(142, 36, 170) };
    int pants[5]  = { COLOR(38, 50, 56), COLOR(55, 71, 79), COLOR(33, 33, 33), COLOR(69, 90, 100), COLOR(40, 53, 64) };
    int umbrellas[5] = { COLOR(230, 50, 50), COLOR(30, 140, 230), COLOR(250, 190, 20), COLOR(160, 40, 200), COLOR(40, 180, 100) };

    for (int i = 0; i < count; i++) {
        Pedestrian p;
        p.x = CROSSWALK_X1 + 10 + i * 14;
        p.y = SIDEWALK_TOP + 12 + (i % 2) * 16;
        p.speed = 1.1f + (i * 0.12f);
        p.shirtColor = shirts[i % 5];
        p.pantColor = pants[i % 5];
        p.skinColor = COLOR(245, 195, 150);
        p.umbrellaColor = umbrellas[i % 5];
        p.walkPhase = (float)(i * 1.2);
        p.crossing = false;
        p.waiting = true;
        p.direction = 1;
        pedestrians.push_back(p);
    }
}

// ============================================================================
// ENVIRONMENT: SKY, SUN, MOON, STARS, CLOUDS, AIRPLANE
// ============================================================================
void drawSky() {
    int skyCol;
    if (rainMode) {
        skyCol = nightMode ? COLOR(16, 20, 32) : COLOR(88, 106, 122);
    } else {
        skyCol = nightMode ? COLOR(18, 22, 46) : COLOR(100, 195, 230);
    }
    filledRect(0, SKY_TOP, W, SKY_BOTTOM, skyCol);

    if (nightMode) {
        // Star field
        for (size_t i = 0; i < stars.size(); i++) {
            int twinkle = (frameCount + stars[i].twinkleOffset) % 24;
            int starCol = (twinkle < 8) ? WHITE : ((twinkle < 16) ? COLOR(180, 200, 240) : COLOR(110, 130, 180));
            putpixel(stars[i].x, stars[i].y, starCol);
            putpixel(stars[i].x + 1, stars[i].y, starCol);
        }

        // Luminous Moon
        int moonX = 1060;
        int moonY = 105;
        filledCircle(moonX, moonY, 44, COLOR(32, 38, 70));
        filledCircle(moonX, moonY, 36, COLOR(245, 245, 220));
        filledCircle(moonX - 8, moonY - 6, 6, COLOR(225, 225, 200));
        filledCircle(moonX + 10, moonY + 8, 8, COLOR(225, 225, 200));
        filledCircle(moonX - 4, moonY + 12, 5, COLOR(225, 225, 200));
    } else {
        if (!rainMode) {
            // Radiant Golden Sun
            int sunX = 1060;
            int sunY = 105;
            filledCircle(sunX, sunY, 48, COLOR(255, 235, 120));
            filledCircle(sunX, sunY, 38, COLOR(255, 205, 45));

            // Radiating Solar Rays (Trigonometric transformations)
            setcolor(COLOR(255, 225, 80));
            for (int i = 0; i < 12; i++) {
                double a = i * (PI / 6.0) + (frameCount * 0.015);
                int x1 = sunX + (int)(46 * cos(a));
                int y1 = sunY + (int)(46 * sin(a));
                int x2 = sunX + (int)(62 * cos(a));
                int y2 = sunY + (int)(62 * sin(a));
                line(x1, y1, x2, y2);
            }
        }
    }
}

void drawClouds() {
    for (size_t i = 0; i < clouds.size(); i++) {
        Cloud& c = clouds[i];
        c.x += c.speed;
        if (c.x > W + 80)
            c.x = -80.0f;

        int cx = (int)c.x;
        int cy = (int)c.y;
        int r = c.radius;

        int cloudCol;
        if (rainMode) {
            cloudCol = nightMode ? COLOR(35, 42, 58) : COLOR(120, 135, 148);
        } else {
            cloudCol = nightMode ? COLOR(48, 55, 82) : COLOR(245, 250, 255);
        }

        filledCircle(cx, cy, r, cloudCol);
        filledCircle(cx + r, cy + 3, (int)(r * 0.8f), cloudCol);
        filledCircle(cx - r, cy + 4, (int)(r * 0.75f), cloudCol);
        filledCircle(cx + (int)(r * 0.4f), cy - (int)(r * 0.5f), (int)(r * 0.75f), cloudCol);
        filledCircle(cx - (int)(r * 0.4f), cy - (int)(r * 0.45f), (int)(r * 0.70f), cloudCol);
    }
}

void drawAirplane() {
    planeX += 1.6f;
    if (planeX > W + 100)
        planeX = -100.0f;

    int px = (int)planeX;
    int py = (int)(planeY + 3.0 * sin(planeX * 0.015));

    // Vapor Contrails
    if (!rainMode) {
        setcolor(nightMode ? COLOR(40, 50, 80) : COLOR(225, 240, 250));
        line(px - 90, py, px - 25, py);
        line(px - 85, py + 4, px - 25, py + 4);
    }

    // Fuselage
    int planeBody = nightMode ? COLOR(190, 195, 210) : WHITE;
    filledRect(px - 22, py - 3, px + 22, py + 5, planeBody, COLOR(120, 130, 145));

    // Aerodynamic Nose
    int nosePts[6] = { px + 22, py - 3, px + 32, py + 2, px + 22, py + 5 };
    setfillstyle(SOLID_FILL, planeBody);
    setcolor(COLOR(120, 130, 145));
    fillpoly(3, nosePts);

    // Wings
    int wingPts[6] = { px - 4, py + 1, px + 8, py + 1, px - 12, py + 16 };
    fillpoly(3, wingPts);

    // Tail Fin
    int tailPts[6] = { px - 22, py - 3, px - 14, py - 3, px - 24, py - 14 };
    fillpoly(3, tailPts);

    // Navigation Strobes
    bool strobe = ((frameCount / 10) % 2 == 0);
    if (strobe) {
        filledCircle(px - 12, py + 16, 2, RED);
        filledCircle(px + 6, py - 2, 2, GREEN);
        filledCircle(px - 24, py - 14, 2, WHITE);
    }
}

// ============================================================================
// UNIQUE SMART CITY FEATURE 1: AUTONOMOUS AERIAL DELIVERY DRONE
// ============================================================================
void updateAndDrawDeliveryDrone() {
    droneX += droneSpeed * droneDirection;
    droneY = 205.0f + 12.0f * (float)sin(droneX * 0.02);
    if (droneX > 860.0f) droneDirection = -1;
    if (droneX < 180.0f) droneDirection = 1;

    droneRotorAngle += 0.55f;
    int dx = (int)droneX;
    int dy = (int)droneY;

    // Carbon fiber central chassis
    filledRect(dx - 14, dy - 4, dx + 14, dy + 4, COLOR(38, 42, 52), COLOR(0, 229, 255));
    // Autonomous AI Optical Eye / LiDAR sensor
    filledCircle(dx + droneDirection * 9, dy, 2, COLOR(0, 229, 255));

    // Structural rotor cross-struts
    setcolor(COLOR(150, 155, 165));
    line(dx - 24, dy - 7, dx + 24, dy - 7);
    line(dx - 18, dy - 1, dx - 24, dy - 7);
    line(dx + 18, dy - 1, dx + 24, dy - 7);

    // 4 Spinning Propellers (using 2D Trigonometric Rotation)
    int rotorX[2] = { dx - 24, dx + 24 };
    for (int i = 0; i < 2; i++) {
        int rx = rotorX[i];
        int ry = dy - 7;
        int bladeSpan = (int)(11.0f * cos(droneRotorAngle + i * 1.57f));
        setcolor(nightMode ? COLOR(190, 220, 255) : COLOR(80, 85, 95));
        line(rx - bladeSpan, ry, rx + bladeSpan, ry);
        filledCircle(rx, ry, 2, COLOR(40, 45, 50));
        // Strobe Navigation LEDs
        bool blink = ((frameCount / 8 + i) % 2 == 0);
        filledCircle(rx, ry - 3, 2, (i == 0) ? (blink ? RED : COLOR(80, 0, 0)) : (blink ? GREEN : COLOR(0, 80, 0)));
    }

    // Smart Delivery Parcel Cargo Pod suspended beneath
    filledRect(dx - 7, dy + 4, dx + 7, dy + 15, COLOR(255, 193, 7), BLACK);
    setcolor(BLACK);
    line(dx - 5, dy + 9, dx + 5, dy + 9);
    // Green delivery badge
    filledCircle(dx, dy + 12, 2, COLOR(76, 175, 80));
}

// ============================================================================
// RENEWABLE ENERGY: ROTATING WIND TURBINES (2D TRIGONOMETRIC ROTATION)
// ============================================================================
void drawSingleWindTurbine(int hubX, int hubY, int towerBaseY, int bladeRadius, float angle) {
    // Tapered Concrete Tower
    int towerPts[8] = {
        hubX - 4, hubY,
        hubX + 4, hubY,
        hubX + 9, towerBaseY,
        hubX - 9, towerBaseY
    };
    int towerCol = nightMode ? COLOR(60, 68, 85) : COLOR(225, 230, 235);
    setfillstyle(SOLID_FILL, towerCol);
    setcolor(nightMode ? COLOR(40, 45, 60) : COLOR(160, 168, 175));
    fillpoly(4, towerPts);

    // Nacelle Hub
    int nacelleCol = nightMode ? COLOR(80, 88, 105) : COLOR(245, 245, 250);
    filledRect(hubX - 10, hubY - 5, hubX + 10, hubY + 5, nacelleCol, COLOR(120, 130, 140));
    filledCircle(hubX, hubY, 7, nacelleCol, COLOR(120, 130, 140));

    // 3 Aerodynamic Blades rotated 120 degrees apart
    int bladeCol = nightMode ? COLOR(160, 168, 185) : WHITE;
    int bladeBorder = nightMode ? COLOR(50, 58, 75) : COLOR(170, 178, 185);

    for (int k = 0; k < 3; k++) {
        double theta = angle + k * (2.0 * PI / 3.0);
        int tipX = hubX + (int)(bladeRadius * cos(theta));
        int tipY = hubY + (int)(bladeRadius * sin(theta));

        double perp = theta + PI / 2.0;
        int midDist = (int)(bladeRadius * 0.35);
        int midX = hubX + (int)(midDist * cos(theta));
        int midY = hubY + (int)(midDist * sin(theta));

        int leftX  = midX + (int)(4.5 * cos(perp));
        int leftY  = midY + (int)(4.5 * sin(perp));
        int rightX = midX - (int)(4.5 * cos(perp));
        int rightY = midY - (int)(4.5 * sin(perp));

        int bladePts[8] = {
            hubX, hubY,
            leftX, leftY,
            tipX, tipY,
            rightX, rightY
        };

        setfillstyle(SOLID_FILL, bladeCol);
        setcolor(bladeBorder);
        fillpoly(4, bladePts);
    }

    filledCircle(hubX, hubY, 4, COLOR(200, 40, 40));

    // Night Warning Beacon atop turbine
    if (nightMode) {
        bool flash = ((frameCount / 18) % 2 == 0);
        if (flash) {
            filledCircle(hubX, hubY - 7, 3, RED);
        }
    }
}

void drawWindFarm() {
    int hillCol = nightMode ? COLOR(24, 52, 35) : COLOR(76, 175, 80);
    int hillRidgePts[14] = {
        910, SKY_BOTTOM,
        960, 415,
        1080, 375,
        1220, 395,
        W, 410,
        W, SKY_BOTTOM,
        910, SKY_BOTTOM
    };
    setfillstyle(SOLID_FILL, hillCol);
    setcolor(hillCol);
    fillpoly(7, hillRidgePts);

    // Pine Trees
    int pineCol = nightMode ? COLOR(16, 38, 25) : COLOR(46, 125, 50);
    for (int tx = 950; tx <= 1240; tx += 48) {
        int ty = 415 - (int)(25.0 * sin((tx - 950) * 0.015));
        int treePts[6] = { tx, ty - 26, tx - 9, ty, tx + 9, ty };
        setfillstyle(SOLID_FILL, pineCol);
        setcolor(pineCol);
        fillpoly(3, treePts);
        filledRect(tx - 2, ty, tx + 2, ty + 7, COLOR(80, 50, 30));
    }

    // Rotate Blades
    windTurbineAngle += rainMode ? 0.075f : 0.045f;
    if (windTurbineAngle > 2.0 * PI)
        windTurbineAngle -= 2.0 * PI;

    drawSingleWindTurbine(1040, 320, 415, 48, windTurbineAngle);
    drawSingleWindTurbine(1185, 295, 420, 56, windTurbineAngle + 0.85f);
}

// ============================================================================
// CITY ARCHITECTURE, SKYLINE & SCROLLING LED BILLBOARD
// ============================================================================
void drawDistantSkyline() {
    int distCol = nightMode ? COLOR(26, 32, 54) : COLOR(145, 185, 205);
    int heights[14] = { 110, 160, 95, 190, 130, 210, 140, 175, 120, 185, 105, 150, 90, 130 };

    int bx = 0;
    for (int i = 0; i < 14; i++) {
        filledRect(bx, SKY_BOTTOM - heights[i], bx + 95, SKY_BOTTOM, distCol);
        bx += 92;
    }
}

void drawWindowGrid(int x1, int y1, int x2, int y2, int rows, int cols, int seedVal) {
    int gapX = (x2 - x1) / (cols + 1);
    int gapY = (y2 - y1) / (rows + 1);

    for (int r = 1; r <= rows; r++) {
        for (int c = 1; c <= cols; c++) {
            int wx = x1 + c * gapX - 5;
            int wy = y1 + r * gapY - 6;

            int winCol;
            if (nightMode) {
                int hash = (r * 17 + c * 31 + seedVal) % 10;
                if (hash < 6)
                    winCol = COLOR(255, 230, 130);
                else if (hash < 8)
                    winCol = COLOR(130, 225, 255);
                else
                    winCol = COLOR(35, 40, 55);
            } else {
                winCol = COLOR(140, 210, 235);
            }

            filledRect(wx, wy, wx + 11, wy + 13, winCol, nightMode ? COLOR(25, 30, 42) : COLOR(90, 110, 125));
        }
    }
}

void drawSkyscrapers() {
    // -------------------------------------------------------------
    // Landmark 1: Telecommunications Needle & Observation Spire
    // -------------------------------------------------------------
    int tBaseX = 65;
    int tPodY  = 185;
    int towerCol = nightMode ? COLOR(50, 58, 75) : COLOR(210, 218, 228);

    int needlePts[8] = {
        tBaseX - 3, tPodY,
        tBaseX + 3, tPodY,
        tBaseX + 16, SKY_BOTTOM,
        tBaseX - 16, SKY_BOTTOM
    };
    setfillstyle(SOLID_FILL, towerCol);
    setcolor(nightMode ? COLOR(30, 36, 50) : COLOR(140, 150, 165));
    fillpoly(4, needlePts);

    filledRect(tBaseX - 28, tPodY - 14, tBaseX + 28, tPodY + 14, towerCol, COLOR(30, 36, 50));
    int podGlass = nightMode ? COLOR(255, 235, 140) : COLOR(100, 185, 220);
    filledRect(tBaseX - 24, tPodY - 6, tBaseX + 24, tPodY + 4, podGlass);

    setcolor(towerCol);
    line(tBaseX, tPodY - 14, tBaseX, tPodY - 75);
    line(tBaseX - 1, tPodY - 14, tBaseX - 1, tPodY - 75);

    bool beaconOn = ((frameCount / 15) % 2 == 0);
    if (beaconOn) {
        filledCircle(tBaseX, tPodY - 77, 4, RED);
        if (nightMode)
            filledCircle(tBaseX, tPodY - 77, 8, COLOR(120, 20, 20));
    }

    // -------------------------------------------------------------
    // Landmark 2: Smart AI Tech Tower with Scrolling LED Billboard
    // -------------------------------------------------------------
    int b2X = 135, b2W = 105, b2H = 260;
    int b2Y = SKY_BOTTOM - b2H;
    int b2Col = nightMode ? COLOR(32, 38, 56) : COLOR(55, 71, 95);
    filledRect(b2X, b2Y, b2X + b2W, SKY_BOTTOM, b2Col, COLOR(20, 25, 38));
    filledRect(b2X + 15, b2Y - 18, b2X + b2W - 15, b2Y, b2Col, COLOR(20, 25, 38));

    // Curved Electronic Video Wall / Scrolling LED Ticker Billboard
    filledRect(b2X + 6, b2Y + 10, b2X + b2W - 6, b2Y + 36, COLOR(12, 18, 30), COLOR(0, 229, 255));
    marqueeScroll++;
    const char* tickerMsg = "SMART CITY 2026 * 100% CLEAN ENERGY * ZERO CARBON * IOT V2X ACTIVE * AUTONOMOUS TRANSIT OPTIMAL * ";
    int msgLen = (int)strlen(tickerMsg);
    int charOffset = (marqueeScroll / 4) % (msgLen - 11);
    char sliceBuf[16];
    strncpy(sliceBuf, tickerMsg + charOffset, 11);
    sliceBuf[11] = '\0';
    drawText(b2X + 10, b2Y + 18, sliceBuf, COLOR(0, 229, 255), 1);

    drawWindowGrid(b2X + 6, b2Y + 45, b2X + b2W - 6, SKY_BOTTOM - 20, 4, 3, 101);

    // -------------------------------------------------------------
    // Landmark 3: Central Smart Hospital (Emergency Center)
    // -------------------------------------------------------------
    int b3X = 252, b3W = 130, b3H = 220;
    int b3Y = SKY_BOTTOM - b3H;
    int b3Col = nightMode ? COLOR(55, 60, 75) : COLOR(235, 238, 245);
    filledRect(b3X, b3Y, b3X + b3W, SKY_BOTTOM, b3Col, COLOR(180, 190, 205));

    // Illuminated Medical Red Cross
    int cx = b3X + b3W / 2;
    int cy = b3Y + 30;
    filledRect(cx - 18, cy - 18, cx + 18, cy + 18, WHITE, RED);
    filledRect(cx - 5, cy - 14, cx + 5, cy + 14, RED);
    filledRect(cx - 14, cy - 5, cx + 14, cy + 5, RED);

    drawText(b3X + 22, b3Y + 54, "HOSPITAL", nightMode ? COLOR(255, 120, 120) : RED, 1);
    drawWindowGrid(b3X + 8, b3Y + 70, b3X + b3W - 8, SKY_BOTTOM - 15, 4, 4, 303);

    // -------------------------------------------------------------
    // Landmark 4: Metropolis Glass Tower & Rooftop Helipad
    // -------------------------------------------------------------
    int b4X = 395, b4W = 100, b4H = 290;
    int b4Y = SKY_BOTTOM - b4H;
    int b4Col = nightMode ? COLOR(30, 42, 60) : COLOR(70, 95, 125);
    filledRect(b4X, b4Y, b4X + b4W, SKY_BOTTOM, b4Col, COLOR(20, 30, 45));
    filledRect(b4X + 18, b4Y - 22, b4X + b4W - 18, b4Y, b4Col);

    // Rooftop Drone / Helicopter Landing Pad ("H")
    filledRect(b4X + 22, b4Y - 20, b4X + b4W - 22, b4Y - 4, COLOR(50, 55, 65), COLOR(255, 215, 0));
    circle(b4X + b4W / 2, b4Y - 12, 6);
    drawText(b4X + b4W / 2 - 3, b4Y - 16, "H", COLOR(255, 215, 0), 1);

    drawWindowGrid(b4X + 6, b4Y + 10, b4X + b4W - 6, SKY_BOTTOM - 20, 6, 3, 404);

    // -------------------------------------------------------------
    // Landmark 5: Civic Smart Hub & City Hall
    // -------------------------------------------------------------
    int b5X = 510, b5W = 145, b5H = 230;
    int b5Y = SKY_BOTTOM - b5H;
    int b5Col = nightMode ? COLOR(48, 52, 66) : COLOR(215, 195, 165);
    filledRect(b5X, b5Y, b5X + b5W, SKY_BOTTOM, b5Col, COLOR(60, 65, 80));

    filledCircle(b5X + b5W / 2, b5Y - 10, 26, nightMode ? COLOR(70, 75, 95) : COLOR(170, 150, 120));
    drawText(b5X + 35, b5Y + 12, "CITY HALL", nightMode ? COLOR(255, 230, 150) : COLOR(60, 40, 20), 1);
    drawWindowGrid(b5X + 10, b5Y + 32, b5X + b5W - 10, SKY_BOTTOM - 20, 4, 4, 505);

    // -------------------------------------------------------------
    // Landmark 6: Cyber Innovation Plaza
    // -------------------------------------------------------------
    int b6X = 670, b6W = 115, b6H = 255;
    int b6Y = SKY_BOTTOM - b6H;
    int b6Col = nightMode ? COLOR(35, 45, 65) : COLOR(105, 130, 165);
    filledRect(b6X, b6Y, b6X + b6W, SKY_BOTTOM, b6Col, COLOR(25, 35, 50));
    drawWindowGrid(b6X + 8, b6Y + 15, b6X + b6W - 8, SKY_BOTTOM - 20, 5, 3, 606);

    // -------------------------------------------------------------
    // Landmark 7: Eco-Living Residential Tower (with Rooftop Solar)
    // -------------------------------------------------------------
    int b7X = 800, b7W = 120, b7H = 240;
    int b7Y = SKY_BOTTOM - b7H;
    int b7Col = nightMode ? COLOR(40, 50, 60) : COLOR(130, 160, 140);
    filledRect(b7X, b7Y, b7X + b7W, SKY_BOTTOM, b7Col, COLOR(30, 40, 45));

    // Rooftop Photovoltaic Solar Array
    int solarCol = nightMode ? COLOR(20, 35, 60) : COLOR(25, 75, 140);
    filledRect(b7X + 10, b7Y - 18, b7X + b7W - 10, b7Y - 4, solarCol, COLOR(180, 200, 220));
    setcolor(COLOR(100, 160, 220));
    for (int sx = b7X + 24; sx < b7X + b7W - 12; sx += 18) {
        line(sx, b7Y - 18, sx, b7Y - 4);
    }
    line(b7X + 10, b7Y - 11, b7X + b7W - 10, b7Y - 11);

    // Vertical Green Balconies
    int foliageCol = nightMode ? COLOR(20, 55, 30) : COLOR(50, 165, 70);
    for (int by = b7Y + 25; by < SKY_BOTTOM - 30; by += 45) {
        filledRect(b7X + 12, by, b7X + 38, by + 12, foliageCol);
        filledRect(b7X + b7W - 38, by, b7X + b7W - 12, by + 12, foliageCol);
    }
    drawWindowGrid(b7X + 44, b7Y + 15, b7X + b7W - 44, SKY_BOTTOM - 20, 5, 2, 707);
}

// ============================================================================
// ELEVATED HIGH-SPEED METRO / BULLET TRAIN VIADUCT
// ============================================================================
void drawElevatedViaduct() {
    int girderCol = nightMode ? COLOR(55, 62, 78) : COLOR(200, 205, 215);
    int borderCol = nightMode ? COLOR(30, 35, 45) : COLOR(130, 135, 145);

    filledRect(0, VIADUCT_Y, W, VIADUCT_Y + VIADUCT_H, girderCol, borderCol);
    setcolor(borderCol);
    line(0, VIADUCT_Y - 3, W, VIADUCT_Y - 3);
    line(0, VIADUCT_Y - 6, W, VIADUCT_Y - 6);

    int pillarX[7] = { 60, 240, 430, 620, 810, 1000, 1190 };
    for (int i = 0; i < 7; i++) {
        int px = pillarX[i];
        filledRect(px - 10, VIADUCT_Y + VIADUCT_H, px + 10, SIDEWALK_BOT - 5, girderCol, borderCol);
        filledRect(px - 18, VIADUCT_Y + VIADUCT_H, px + 18, VIADUCT_Y + VIADUCT_H + 7, girderCol, borderCol);
        filledRect(px - 14, SIDEWALK_BOT - 8, px + 14, SIDEWALK_BOT, girderCol, borderCol);
    }
}

void drawBulletTrain() {
    float currentSpeed = expressTrainActive ? (trainBaseSpeed * 2.1f) : trainBaseSpeed;
    trainX += currentSpeed;
    if (trainX > W + 280)
        trainX = -280.0f;

    int tx = (int)trainX;
    int ty = VIADUCT_Y - 26;
    int trainLen = 240;
    int trainH = 23;

    if (expressTrainActive) {
        setcolor(nightMode ? COLOR(0, 200, 255) : COLOR(100, 180, 255));
        for (int i = 0; i < 6; i++) {
            int lineY = ty + 4 + i * 3;
            line(tx - 65, lineY, tx - 10, lineY);
        }
    }

    int bodyCol = nightMode ? COLOR(225, 230, 240) : WHITE;
    filledRect(tx - trainLen + 25, ty, tx, ty + trainH, bodyCol, COLOR(60, 65, 75));

    int nosePts[6] = {
        tx, ty,
        tx + 32, ty + trainH - 3,
        tx, ty + trainH
    };
    setfillstyle(SOLID_FILL, bodyCol);
    setcolor(COLOR(60, 65, 75));
    fillpoly(3, nosePts);

    int winPts[6] = {
        tx + 4, ty + 2,
        tx + 22, ty + 10,
        tx + 4, ty + 10
    };
    setfillstyle(SOLID_FILL, nightMode ? COLOR(255, 240, 150) : COLOR(40, 120, 180));
    fillpoly(3, winPts);

    filledRect(tx - trainLen + 25, ty + 12, tx + 20, ty + 15, COLOR(0, 140, 220));
    filledRect(tx - trainLen + 25, ty + 16, tx + 18, ty + 18, COLOR(220, 40, 40));

    setcolor(BLACK);
    line(tx - 80, ty, tx - 80, ty + trainH);
    line(tx - 160, ty, tx - 160, ty + trainH);

    int passWinCol = nightMode ? COLOR(255, 235, 130) : COLOR(80, 160, 205);
    for (int wx = tx - trainLen + 35; wx < tx - 15; wx += 22) {
        if (abs(wx - (tx - 80)) > 8 && abs(wx - (tx - 160)) > 8) {
            filledRect(wx, ty + 3, wx + 14, ty + 10, passWinCol, COLOR(40, 45, 55));
        }
    }

    filledCircle(tx + 27, ty + trainH - 4, 3, COLOR(255, 255, 200));
    if (nightMode) {
        int beamPts[6] = {
            tx + 30, ty + trainH - 4,
            tx + 95, ty + trainH - 12,
            tx + 95, ty + trainH + 4
        };
        setfillstyle(SOLID_FILL, COLOR(80, 90, 70));
        setcolor(COLOR(80, 90, 70));
        fillpoly(3, beamPts);
    }
}

// ============================================================================
// UNIQUE SMART CITY FEATURE 2: CENTRAL PARK ANIMATED WATER FOUNTAIN
// ============================================================================
void drawParkFountain() {
    int fx = 640;
    int fy = SIDEWALK_BOT - 10;

    // Stone Basin
    filledRect(fx - 24, fy - 6, fx + 24, fy + 4, COLOR(140, 145, 155), BLACK);
    filledCircle(fx, fy - 2, 22, nightMode ? COLOR(25, 75, 120) : COLOR(70, 160, 225), COLOR(90, 95, 105));
    // Center Pedestal
    filledRect(fx - 4, fy - 18, fx + 4, fy - 2, COLOR(160, 165, 175), BLACK);
    filledCircle(fx, fy - 18, 6, COLOR(185, 190, 200), BLACK);

    // Spawn water droplets
    if ((int)fountainDrops.size() < 36) {
        for (int i = 0; i < 4; i++) {
            WaterDrop wd;
            wd.x = (float)fx;
            wd.y = (float)(fy - 18);
            wd.vx = ((rand() % 100) - 50) / 40.0f;
            wd.vy = -3.2f - ((rand() % 35) / 25.0f);
            fountainDrops.push_back(wd);
        }
    }

    // Update and draw parabolic arcs with gravity
    setcolor(nightMode ? COLOR(160, 220, 255) : COLOR(220, 245, 255));
    for (size_t i = 0; i < fountainDrops.size(); ) {
        WaterDrop& wd = fountainDrops[i];
        wd.x += wd.vx;
        wd.y += wd.vy;
        wd.vy += 0.22f; // Gravity acceleration

        putpixel((int)wd.x, (int)wd.y, nightMode ? COLOR(160, 220, 255) : COLOR(220, 245, 255));
        putpixel((int)wd.x + 1, (int)wd.y, nightMode ? COLOR(160, 220, 255) : COLOR(220, 245, 255));

        if (wd.y >= (float)(fy - 2)) {
            fountainDrops.erase(fountainDrops.begin() + i);
        } else {
            i++;
        }
    }
}

// ============================================================================
// UNIQUE SMART CITY FEATURE 3: SMART COMMUTER ON ELECTRIC SCOOTER
// ============================================================================
void drawCommuterScooter() {
    scooterX += 1.8f;
    if (scooterX > W + 40)
        scooterX = -50.0f;

    int sx = (int)scooterX;
    int sy = SIDEWALK_BOT - 14;

    // Small alloy wheels
    filledCircle(sx - 11, sy + 6, 4, BLACK);
    filledCircle(sx + 13, sy + 6, 4, BLACK);
    filledCircle(sx - 11, sy + 6, 2, COLOR(190, 190, 190));
    filledCircle(sx + 13, sy + 6, 2, COLOR(190, 190, 190));

    // Green Eco-Deck & Handlebar Stem
    setcolor(COLOR(0, 200, 115));
    line(sx - 11, sy + 5, sx + 11, sy + 5);
    line(sx + 11, sy + 5, sx + 13, sy - 14);
    line(sx + 10, sy - 14, sx + 16, sy - 14);
    // LED Headlight
    filledCircle(sx + 15, sy - 12, 2, COLOR(255, 255, 200));

    // Commuter (Body & Safety Helmet)
    setcolor(COLOR(45, 55, 75));
    line(sx - 2, sy + 5, sx + 3, sy - 6);
    line(sx + 3, sy - 6, sx + 3, sy - 20);
    line(sx + 3, sy - 16, sx + 13, sy - 14);
    // Orange Safety Helmet
    filledCircle(sx + 3, sy - 26, 6, COLOR(255, 87, 34));
    filledCircle(sx + 3, sy - 23, 4, COLOR(245, 195, 150));
}

// ============================================================================
// SIDEWALK, LANDSCAPING, 5G AI SMART POLES, EV CHARGER & SMART BINS
// ============================================================================
void drawTrees() {
    int treeX[4] = { 25, 235, 485, 905 };
    int foliageCol = nightMode ? COLOR(25, 75, 40) : COLOR(46, 155, 65);
    int trunkCol   = COLOR(95, 60, 35);

    for (int i = 0; i < 4; i++) {
        int tx = treeX[i];
        int ty = SIDEWALK_TOP + 12;
        filledRect(tx - 4, ty, tx + 4, ty + 28, trunkCol);
        filledCircle(tx, ty - 6, 18, foliageCol);
        filledCircle(tx - 12, ty, 14, foliageCol);
        filledCircle(tx + 12, ty, 14, foliageCol);
        filledCircle(tx, ty - 16, 13, foliageCol);
    }
}

void drawParkBench() {
    int bx = 715;
    int by = SIDEWALK_TOP + 18;
    int woodCol = COLOR(140, 85, 45);
    filledRect(bx, by, bx + 36, by + 5, woodCol, BLACK);
    filledRect(bx, by - 10, bx + 36, by - 6, woodCol, BLACK);
    setcolor(BLACK);
    line(bx + 4, by + 5, bx + 4, by + 16);
    line(bx + 32, by + 5, bx + 32, by + 16);
    line(bx + 4, by - 6, bx + 4, by + 5);
    line(bx + 32, by - 6, bx + 32, by + 5);
}

void drawSmartWasteBin() {
    int wx = 320;
    int wy = SIDEWALK_TOP + 16;
    // Solar compactor smart waste bin
    filledRect(wx, wy, wx + 18, wy + 26, COLOR(45, 50, 60), BLACK);
    // Solar panel top
    filledRect(wx - 1, wy - 3, wx + 19, wy, COLOR(25, 75, 140), BLACK);
    // Green Fill-Level Status LED
    filledCircle(wx + 9, wy + 8, 2, COLOR(0, 230, 118));
    // Recycle Emblem
    setcolor(COLOR(0, 230, 118));
    circle(wx + 9, wy + 18, 4);
}

void drawEVChargingBay() {
    int cx = 1110;
    int cy = SIDEWALK_TOP + 4;

    int padCol = nightMode ? COLOR(45, 55, 65) : COLOR(165, 175, 185);
    filledRect(cx - 30, cy, cx + 55, SIDEWALK_BOT - 2, padCol, COLOR(0, 200, 115));

    filledRect(cx - 24, cy + 4, cx - 10, cy + 34, nightMode ? COLOR(30, 38, 50) : COLOR(220, 225, 230), BLACK);
    int screenCol = evChargingActive ? COLOR(0, 230, 118) : COLOR(255, 82, 82);
    filledRect(cx - 22, cy + 8, cx - 12, cy + 18, screenCol);

    drawText(cx - 26, cy + 38, "EV FAST CHARGE", nightMode ? COLOR(0, 230, 118) : COLOR(0, 140, 70), 1);

    int carCol = COLOR(0, 136, 204);
    filledRect(cx, cy + 12, cx + 46, cy + 28, carCol, BLACK);
    filledCircle(cx + 10, cy + 28, 5, BLACK);
    filledCircle(cx + 36, cy + 28, 5, BLACK);
    filledRect(cx + 10, cy + 4, cx + 32, cy + 12, COLOR(120, 195, 235), BLACK);

    setcolor(COLOR(0, 230, 118));
    line(cx - 10, cy + 22, cx + 2, cy + 22);

    if (evChargingActive) {
        bool pulse = ((frameCount / 12) % 2 == 0);
        filledCircle(cx - 17, cy + 13, 3, pulse ? COLOR(0, 255, 128) : COLOR(0, 180, 80));
    }
}

// 5G AI Smart Street Pole with CCTV Dome & Environmental Sensor
void drawSmartPole(int x) {
    int poleCol = nightMode ? COLOR(55, 62, 75) : COLOR(110, 120, 130);
    setcolor(poleCol);

    line(x, SIDEWALK_BOT - 4, x, SIDEWALK_TOP - 18);
    line(x, SIDEWALK_TOP - 18, x + 16, SIDEWALK_TOP - 26);
    line(x + 16, SIDEWALK_TOP - 26, x + 24, SIDEWALK_TOP - 20);

    int headX = x + 24;
    int headY = SIDEWALK_TOP - 20;

    // AI CCTV Surveillance Dome on mid-pole
    filledCircle(x, SIDEWALK_TOP - 6, 4, COLOR(30, 35, 45));
    // Blinking blue security indicator
    filledCircle(x, SIDEWALK_TOP - 6, 2, ((frameCount / 12) % 2 == 0) ? COLOR(0, 229, 255) : COLOR(0, 100, 180));

    // 5G Micro-Cell Antenna Fin at pole summit
    line(x, SIDEWALK_TOP - 18, x, SIDEWALK_TOP - 32);
    filledCircle(x, SIDEWALK_TOP - 32, 2, COLOR(0, 229, 255));

    if (nightMode) {
        // Conical Light Beam cast down onto sidewalk and road
        int beamPts[8] = {
            headX - 4, headY + 4,
            headX + 4, headY + 4,
            headX + 42, ROAD_TOP + 45,
            headX - 32, ROAD_TOP + 45
        };
        setfillstyle(SOLID_FILL, COLOR(45, 48, 52));
        setcolor(COLOR(45, 48, 52));
        fillpoly(4, beamPts);

        filledCircle(headX, headY, 6, COLOR(255, 240, 150));
        filledCircle(headX, headY, 11, COLOR(100, 95, 60));
    } else {
        filledCircle(headX, headY, 5, COLOR(205, 215, 220));
    }
}

void drawSmartPoles() {
    int lx[5] = { 40, 275, 785, 960, 1220 };
    for (int i = 0; i < 5; i++) {
        drawSmartPole(lx[i]);
    }
}

// ============================================================================
// UNIQUE SMART CITY FEATURE 4: SMART HIGHWAY GANTRY (ITS) & V2X SENSOR GRID
// Replaces traditional clunky traffic lights with an intelligent autonomous overpass
// ============================================================================
void updateAutonomousGantryLogic() {
    bool emergencyApproaching = false;
    if (ambulanceActive && ambulance.x > 0 && ambulance.x < GANTRY_X + 120)
        emergencyApproaching = true;
    if (fireTruckActive && fireTruck.x > 0 && fireTruck.x < GANTRY_X + 120)
        emergencyApproaching = true;

    if (emergencyApproaching) {
        gantryState = GANTRY_FLOW;
        gantryTimer = 0;
        return;
    }

    gantryTimer++;
    if (gantryState == GANTRY_FLOW && gantryTimer >= DURATION_FLOW) {
        gantryState = GANTRY_WARNING;
        gantryTimer = 0;
    } else if (gantryState == GANTRY_WARNING && gantryTimer >= DURATION_WARNING) {
        gantryState = GANTRY_PEDESTRIAN;
        gantryTimer = 0;
    } else if (gantryState == GANTRY_PEDESTRIAN && gantryTimer >= DURATION_CROSSING) {
        gantryState = GANTRY_FLOW;
        gantryTimer = 0;
    }
}

void drawSmartHighwayGantry() {
    int gx = GANTRY_X;
    int gantryTopY = ROAD_TOP - 160;

    // Structural Cantilever Mast & High-Tech Truss
    int trussCol = COLOR(42, 48, 58);
    filledRect(gx - 4, gantryTopY, gx + 8, ROAD_TOP, trussCol, BLACK);
    // Cantilever overpass arm spanning across the highway
    filledRect(gx - 55, gantryTopY + 15, gx + 8, gantryTopY + 28, trussCol, BLACK);

    // AI 5G V2X Telemetry Antennas on Gantry Top
    line(gx - 40, gantryTopY + 15, gx - 40, gantryTopY - 10);
    line(gx - 10, gantryTopY + 15, gx - 10, gantryTopY - 12);
    filledCircle(gx - 40, gantryTopY - 10, 3, COLOR(0, 229, 255));
    filledCircle(gx - 10, gantryTopY - 12, 3, COLOR(0, 229, 255));

    // Animated Wireless 5G Waves Radiating from Gantry to Autonomous Cars
    int waveR = (frameCount * 2) % 36;
    setcolor(COLOR(0, 229, 255));
    arc(gx - 25, gantryTopY - 10, 200, 340, waveR + 8);

    // Large Digital Overhead Message Display Matrix
    int matX1 = gx - 55;
    int matY1 = gantryTopY + 28;
    int matX2 = gx + 4;
    int matY2 = matY1 + 75;
    filledRect(matX1, matY1, matX2, matY2, COLOR(15, 20, 28), COLOR(0, 229, 255));

    // Dynamic Lane Guidance Signals & Status Matrix per lane
    if (gantryState == GANTRY_FLOW) {
        // Lane 0: 60 km/h | Green Flow Arrow
        drawText(matX1 + 6, matY1 + 6, "L0: [60] v", COLOR(0, 255, 150), 1);
        // Lane 1: 50 km/h | Green Flow Arrow
        drawText(matX1 + 6, matY1 + 24, "L1: [50] v", COLOR(0, 255, 150), 1);
        // Lane 2: Bus/Eco Priority
        drawText(matX1 + 6, matY1 + 42, "L2: [BUS] v", COLOR(0, 229, 255), 1);
        // Grid Status
        drawText(matX1 + 6, matY1 + 60, "V2X: OPTIMAL", COLOR(0, 255, 150), 1);
    } else if (gantryState == GANTRY_WARNING) {
        drawText(matX1 + 6, matY1 + 6, "L0: [!] SLOW", YELLOW, 1);
        drawText(matX1 + 6, matY1 + 24, "L1: [!] SLOW", YELLOW, 1);
        drawText(matX1 + 6, matY1 + 42, "L2: [!] YIELD", YELLOW, 1);
        drawText(matX1 + 6, matY1 + 60, "V2X: CAUTION", YELLOW, 1);
    } else { // GANTRY_PEDESTRIAN (Crosswalk Active)
        drawText(matX1 + 6, matY1 + 6, "L0: [X] HOLD", RED, 1);
        drawText(matX1 + 6, matY1 + 24, "L1: [X] HOLD", RED, 1);
        drawText(matX1 + 6, matY1 + 42, "L2: [X] HOLD", RED, 1);
        drawText(matX1 + 6, matY1 + 60, "PEDESTRIANS", COLOR(255, 80, 80), 1);
    }

    // Optical LiDAR Crosswalk Scanner Beam
    if (gantryState == GANTRY_PEDESTRIAN) {
        // Soft animated scanning laser fan down to the crosswalk
        int scanX = CROSSWALK_X1 + ((frameCount * 3) % (CROSSWALK_X2 - CROSSWALK_X1));
        setcolor(COLOR(0, 229, 255));
        line(gx - 25, matY2, scanX, ROAD_TOP + 5);
        line(gx - 25, matY2, scanX + 4, ROAD_TOP + 5);
    }

    // V2I 5G Priority Broadcast Wave when Emergency Vehicle is active
    bool emergencyApproaching = (ambulanceActive || fireTruckActive);
    if (emergencyApproaching) {
        drawText(matX1 - 18, gantryTopY - 24, "5G EMERGENCY WAVE", COLOR(0, 229, 255), 1);
        int emX = ambulanceActive ? (int)ambulance.x + 35 : (int)fireTruck.x + 45;
        if (emX > 20 && emX < gx + 50) {
            setcolor(COLOR(0, 229, 255));
            line(emX, ROAD_TOP + 40, gx - 25, gantryTopY + 20);
        }
    }
}

// ============================================================================
// UNIQUE SMART CITY FEATURE 5: SMART PHOTOLUMINESCENT LED CROSSWALK
// ============================================================================
void drawSmartRoadway() {
    // Sidewalk Granite Paving
    int sidewalkCol = nightMode ? COLOR(55, 60, 70) : COLOR(195, 192, 185);
    filledRect(0, SIDEWALK_TOP, W, SIDEWALK_BOT, sidewalkCol);
    filledRect(0, SIDEWALK_BOT - 4, W, SIDEWALK_BOT, COLOR(120, 125, 132));

    // Roadway Asphalt
    int roadCol = nightMode ? COLOR(32, 35, 42) : COLOR(58, 62, 68);
    filledRect(0, ROAD_TOP, W, ROAD_BOT, roadCol);

    // Upper Solid White Shoulder Line
    setcolor(WHITE);
    line(0, ROAD_TOP + 3, W, ROAD_TOP + 3);

    // Dashed Lane Dividers with Reflective Cat's Eye Studs
    for (int x = 0; x < W; x += 75) {
        filledRect(x, ROAD_TOP + 48, x + 38, ROAD_TOP + 51, COLOR(255, 215, 0));
        filledRect(x, ROAD_TOP + 98, x + 38, ROAD_TOP + 101, COLOR(255, 215, 0));
        if (nightMode) {
            putpixel(x + 19, ROAD_TOP + 49, COLOR(255, 200, 50));
            putpixel(x + 19, ROAD_TOP + 99, COLOR(255, 200, 50));
        }
    }

    // Lower Solid White Edge Line
    setcolor(WHITE);
    line(0, ROAD_BOT - 3, W, ROAD_BOT - 3);

    // Smart Stop Line
    bool isPedCrossing = (gantryState == GANTRY_PEDESTRIAN);
    int stopLineCol = isPedCrossing ? RED : WHITE;
    filledRect(STOP_LINE_X - 4, ROAD_TOP + 3, STOP_LINE_X, ROAD_BOT - 3, stopLineCol);
    if (isPedCrossing) {
        // Glowing Neon Red Ground Light Barrier
        setcolor(COLOR(255, 80, 80));
        line(STOP_LINE_X - 6, ROAD_TOP + 3, STOP_LINE_X - 6, ROAD_BOT - 3);
        line(STOP_LINE_X + 2, ROAD_TOP + 3, STOP_LINE_X + 2, ROAD_BOT - 3);
    }

    // Smart Photoluminescent Ground-Embedded LED Crosswalk Stripes
    int stripeCol;
    if (isPedCrossing) {
        stripeCol = COLOR(0, 229, 255); // Neon Cyan illumination when pedestrians cross!
    } else if (gantryState == GANTRY_WARNING) {
        stripeCol = COLOR(255, 215, 0); // Warning Amber
    } else {
        stripeCol = WHITE;              // Standard Ambient
    }

    for (int sx = CROSSWALK_X1; sx <= CROSSWALK_X2; sx += 18) {
        filledRect(sx, ROAD_TOP + 6, sx + 11, ROAD_BOT - 6, stripeCol);
        if (isPedCrossing) {
            // Neon perimeter edge glow
            setcolor(COLOR(100, 255, 255));
            rectangle(sx - 1, ROAD_TOP + 5, sx + 12, ROAD_BOT - 5);
        }
    }
}

// ============================================================================
// VEHICLE DRAWING ROUTINES (EV SEDAN, ELECTRIC BUS, ECO TRUCK, ROBOTAXI, COUPE)
// ============================================================================
void drawCarSedan(int x, int y, int bodyCol, float scale, bool isBraking) {
    int w = (int)(62 * scale);
    int h = (int)(22 * scale);

    filledRect(x, y - h / 2, x + w, y + h / 2, bodyCol, BLACK);

    int roofX1 = x + (int)(15 * scale);
    int roofX2 = x + (int)(46 * scale);
    int roofY  = y - (int)(h * 0.95f);

    int cabinPts[8] = {
        roofX1 - (int)(6 * scale), y - h / 2,
        roofX1, roofY,
        roofX2, roofY,
        roofX2 + (int)(9 * scale), y - h / 2
    };
    setfillstyle(SOLID_FILL, bodyCol);
    setcolor(BLACK);
    fillpoly(4, cabinPts);

    int winCol = nightMode ? COLOR(80, 150, 190) : COLOR(130, 205, 235);
    filledRect(roofX1 - 2, roofY + 3, roofX1 + (int)(13 * scale), y - h / 2, winCol);
    filledRect(roofX1 + (int)(16 * scale), roofY + 3, roofX2 + 3, y - h / 2, winCol);

    int wheelR = (int)(7 * scale);
    filledCircle(x + (int)(13 * scale), y + h / 2 + 1, wheelR, BLACK);
    filledCircle(x + w - (int)(13 * scale), y + h / 2 + 1, wheelR, BLACK);
    filledCircle(x + (int)(13 * scale), y + h / 2 + 1, (int)(3 * scale), COLOR(190, 195, 200));
    filledCircle(x + w - (int)(13 * scale), y + h / 2 + 1, (int)(3 * scale), COLOR(190, 195, 200));

    filledCircle(x + w - 2, y, (int)(3 * scale), COLOR(255, 250, 200));
    int tailCol = isBraking ? COLOR(255, 30, 30) : (nightMode ? COLOR(200, 30, 30) : COLOR(130, 20, 20));
    filledCircle(x + 2, y, (int)(3 * scale), tailCol);

    if (nightMode) {
        int conePts[6] = {
            x + w, y - 2,
            x + w + (int)(85 * scale), y - (int)(18 * scale),
            x + w + (int)(85 * scale), y + (int)(18 * scale)
        };
        setfillstyle(SOLID_FILL, COLOR(65, 68, 62));
        setcolor(COLOR(65, 68, 62));
        fillpoly(3, conePts);
    }
}

void drawElectricBus(int x, int y, float scale, bool isBraking) {
    int w = (int)(115 * scale);
    int h = (int)(34 * scale);

    filledRect(x, y - h / 2, x + w, y + h / 2 - (int)(6 * scale), COLOR(0, 168, 204), BLACK);
    filledRect(x, y + h / 2 - (int)(6 * scale), x + w, y + h / 2, COLOR(20, 45, 65), BLACK);

    filledRect(x + w - (int)(14 * scale), y - h / 2 + 4, x + w - 2, y + 2, COLOR(90, 180, 220), BLACK);

    int winW = (int)(14 * scale);
    for (int i = 0; i < 5; i++) {
        int wx = x + (int)(8 * scale) + i * (winW + (int)(4 * scale));
        filledRect(wx, y - h / 2 + 5, wx + winW, y + 2, COLOR(90, 180, 220), BLACK);
        filledCircle(wx + winW / 2, y - 1, (int)(3 * scale), nightMode ? COLOR(255, 220, 150) : COLOR(40, 50, 60));
    }

    filledRect(x + w - (int)(42 * scale), y - h / 2 - (int)(5 * scale), x + w - 4, y - h / 2 + 2, BLACK);
    drawText(x + w - (int)(40 * scale), y - h / 2 - (int)(4 * scale), "101 METRO", COLOR(255, 180, 0), 1);

    setcolor(BLACK);
    line(x + w - (int)(24 * scale), y - h / 2 + 4, x + w - (int)(24 * scale), y + h / 2 - 2);

    int wheelR = (int)(9 * scale);
    filledCircle(x + (int)(20 * scale), y + h / 2 + 2, wheelR, BLACK);
    filledCircle(x + w - (int)(22 * scale), y + h / 2 + 2, wheelR, BLACK);
    filledCircle(x + (int)(20 * scale), y + h / 2 + 2, (int)(4 * scale), COLOR(180, 185, 190));
    filledCircle(x + w - (int)(22 * scale), y + h / 2 + 2, (int)(4 * scale), COLOR(180, 185, 190));

    filledCircle(x + w - 2, y + 5, (int)(3 * scale), COLOR(255, 255, 200));
    int tailCol = isBraking ? COLOR(255, 20, 20) : (nightMode ? COLOR(200, 20, 20) : COLOR(120, 15, 15));
    filledCircle(x + 2, y + 5, (int)(3 * scale), tailCol);

    if (nightMode) {
        int conePts[6] = {
            x + w, y + 3,
            x + w + (int)(95 * scale), y - (int)(20 * scale),
            x + w + (int)(95 * scale), y + (int)(22 * scale)
        };
        setfillstyle(SOLID_FILL, COLOR(65, 68, 62));
        setcolor(COLOR(65, 68, 62));
        fillpoly(3, conePts);
    }
}

void drawEcoTruck(int x, int y, float scale, bool isBraking) {
    int cargoW = (int)(62 * scale);
    int cabW   = (int)(28 * scale);
    int h      = (int)(28 * scale);

    filledRect(x, y - h / 2, x + cargoW, y + h / 2, COLOR(56, 142, 60), BLACK);

    int panelX1 = x + (int)(12 * scale);
    int panelX2 = x + cargoW - (int)(12 * scale);
    filledRect(panelX1, y - (int)(8 * scale), panelX2, y + (int)(8 * scale), WHITE, COLOR(40, 110, 45));
    filledCircle((panelX1 + panelX2) / 2, y, (int)(5 * scale), COLOR(76, 175, 80));

    filledRect(x + cargoW, y - h / 2 + (int)(4 * scale), x + cargoW + cabW, y + h / 2, COLOR(76, 175, 80), BLACK);
    filledRect(x + cargoW + (int)(6 * scale), y - h / 2 + (int)(7 * scale), x + cargoW + cabW - (int)(4 * scale), y, COLOR(100, 185, 220), BLACK);

    int wheelR = (int)(8 * scale);
    filledCircle(x + (int)(14 * scale), y + h / 2 + 2, wheelR, BLACK);
    filledCircle(x + cargoW + cabW - (int)(10 * scale), y + h / 2 + 2, wheelR, BLACK);
    filledCircle(x + (int)(14 * scale), y + h / 2 + 2, (int)(3 * scale), COLOR(180, 185, 190));
    filledCircle(x + cargoW + cabW - (int)(10 * scale), y + h / 2 + 2, (int)(3 * scale), COLOR(180, 185, 190));

    filledCircle(x + cargoW + cabW - 2, y + 6, (int)(3 * scale), COLOR(255, 255, 190));
    int tailCol = isBraking ? COLOR(255, 20, 20) : (nightMode ? COLOR(200, 20, 20) : COLOR(120, 15, 15));
    filledCircle(x + 2, y + 6, (int)(3 * scale), tailCol);

    if (nightMode) {
        int conePts[6] = {
            x + cargoW + cabW, y + 4,
            x + cargoW + cabW + (int)(88 * scale), y - (int)(18 * scale),
            x + cargoW + cabW + (int)(88 * scale), y + (int)(20 * scale)
        };
        setfillstyle(SOLID_FILL, COLOR(65, 68, 62));
        setcolor(COLOR(65, 68, 62));
        fillpoly(3, conePts);
    }
}

void drawRobotaxi(int x, int y, float scale, bool isBraking) {
    drawCarSedan(x, y, COLOR(255, 193, 7), scale, isBraking);

    for (int i = 0; i < 4; i++) {
        int tx = x + (int)(18 * scale) + i * (int)(6 * scale);
        filledRect(tx, y - 2, tx + (int)(3 * scale), y + 2, BLACK);
    }

    int podX = x + (int)(30 * scale);
    int podY = y - (int)(22 * scale * 0.95f) - (int)(4 * scale);
    filledRect(podX - 4, podY, podX + 4, podY + 5, COLOR(40, 45, 50), BLACK);
    bool lidarBlink = ((frameCount / 6) % 2 == 0);
    filledCircle(podX, podY, 3, lidarBlink ? COLOR(0, 229, 255) : COLOR(0, 120, 180));
}

void drawSportsCoupe(int x, int y, float scale, bool isBraking) {
    drawCarSedan(x, y, COLOR(220, 38, 38), scale, isBraking);
    setcolor(COLOR(30, 30, 30));
    line(x + 5, y - 4, x + (int)(60 * scale) - 5, y - 4);
    filledRect(x + 2, y - (int)(14 * scale), x + 8, y - (int)(12 * scale), BLACK);
    line(x + 5, y - (int)(12 * scale), x + 5, y - (int)(7 * scale));
}

void drawAmbulanceVehicle(int x, int y, bool strobe) {
    int w = 72;
    int h = 28;

    filledRect(x, y - h / 2, x + w, y + h / 2, WHITE, BLACK);
    filledRect(x, y + 2, x + w, y + 8, COLOR(235, 45, 45));

    filledRect(x + 40, y - h / 2 + 4, x + 64, y + 1, COLOR(100, 180, 220), BLACK);

    int cx = x + 20;
    int cy = y - 1;
    filledRect(cx - 3, cy - 8, cx + 3, cy + 8, RED);
    filledRect(cx - 8, cy - 3, cx + 8, cy + 3, RED);

    int lightBarX = x + 30;
    int lightBarY = y - h / 2 - 5;
    filledCircle(lightBarX - 6, lightBarY, 4, strobe ? RED : COLOR(80, 15, 15));
    filledCircle(lightBarX + 6, lightBarY, 4, strobe ? COLOR(15, 25, 80) : COLOR(0, 140, 255));

    if (strobe) {
        circle(lightBarX - 6, lightBarY, 8);
    } else {
        circle(lightBarX + 6, lightBarY, 8);
    }

    filledCircle(x + 16, y + h / 2 + 2, 7, BLACK);
    filledCircle(x + w - 16, y + h / 2 + 2, 7, BLACK);
    filledCircle(x + 16, y + h / 2 + 2, 3, COLOR(180, 185, 190));
    filledCircle(x + w - 16, y + h / 2 + 2, 3, COLOR(180, 185, 190));

    filledCircle(x + w - 2, y + 6, 3, COLOR(255, 255, 200));
}

void drawFireTruckVehicle(int x, int y, bool strobe) {
    int w = 92;
    int h = 30;

    filledRect(x, y - h / 2, x + w, y + h / 2, COLOR(210, 35, 35), BLACK);
    filledRect(x + 8, y - 2, x + 48, y + h / 2 - 3, COLOR(225, 228, 232), BLACK);

    filledRect(x + 58, y - h / 2 + 4, x + w - 4, y + h / 2, COLOR(190, 25, 25), BLACK);
    filledRect(x + 64, y - h / 2 + 6, x + w - 8, y + 2, COLOR(100, 180, 220), BLACK);

    int ladderY = y - h / 2 - 7;
    setcolor(COLOR(255, 215, 0));
    line(x + 8, ladderY, x + 54, ladderY);
    line(x + 8, ladderY + 4, x + 54, ladderY + 4);
    for (int rx = x + 12; rx < x + 52; rx += 7) {
        line(rx, ladderY, rx, ladderY + 4);
    }

    int strobeX = x + 74;
    int strobeY = y - h / 2 - 4;
    filledCircle(strobeX - 5, strobeY, 4, strobe ? COLOR(255, 180, 0) : COLOR(220, 20, 20));
    filledCircle(strobeX + 5, strobeY, 4, strobe ? COLOR(220, 20, 20) : COLOR(0, 140, 255));

    filledCircle(x + 18, y + h / 2 + 3, 8, BLACK);
    filledCircle(x + 42, y + h / 2 + 3, 8, BLACK);
    filledCircle(x + w - 16, y + h / 2 + 3, 8, BLACK);
    filledCircle(x + 18, y + h / 2 + 3, 3, COLOR(180, 185, 190));
    filledCircle(x + 42, y + h / 2 + 3, 3, COLOR(180, 185, 190));
    filledCircle(x + w - 16, y + h / 2 + 3, 3, COLOR(180, 185, 190));

    filledCircle(x + w - 2, y + 6, 3, COLOR(255, 255, 200));
}

// ============================================================================
// ZERO-OVERLAP VEHICLE PHYSICS & QUEUING SYSTEM
// ============================================================================
void updateVehicles() {
    bool crosswalkActive = (gantryState == GANTRY_PEDESTRIAN);

    for (int lane = 0; lane < 3; lane++) {
        vector<int> list;
        for (size_t i = 0; i < vehicles.size(); i++) {
            if (vehicles[i].lane == lane) {
                list.push_back((int)i);
            }
        }

        // Sort descending by X (frontmost car first)
        sort(list.begin(), list.end(), [](int a, int b) {
            return vehicles[a].x > vehicles[b].x;
        });

        for (size_t k = 0; k < list.size(); k++) {
            Vehicle& v = vehicles[list[k]];
            float targetSpeed = v.maxSpeed;
            v.isBraking = false;

            // 1. Crosswalk & Stop Line Deceleration
            if (crosswalkActive) {
                if (v.x < STOP_LINE_X - 10 && v.x > STOP_LINE_X - 250) {
                    float dist = (STOP_LINE_X - 10) - v.x;
                    targetSpeed = (dist / 250.0f) * v.maxSpeed;
                    if (targetSpeed < 0.2f) targetSpeed = 0.0f;
                    v.isBraking = true;
                } else if (v.x >= STOP_LINE_X - 10 && v.x < STOP_LINE_X + 20) {
                    targetSpeed = 0.0f;
                    v.isBraking = true;
                }
            }

            // 2. Safe Following Distance towards car ahead
            if (k > 0) {
                Vehicle& ahead = vehicles[list[k - 1]];
                float minSafeGap = (v.length / 2.0f) + (ahead.length / 2.0f) + 35.0f;
                float actualGap = ahead.x - v.x;

                if (actualGap < minSafeGap + 120.0f) {
                    float factor = (actualGap - minSafeGap) / 120.0f;
                    if (factor < 0.0f) factor = 0.0f;
                    targetSpeed = min(targetSpeed, ahead.speed * factor);
                    v.isBraking = true;
                }
            }

            // Smooth Acceleration / Braking
            if (v.speed < targetSpeed)
                v.speed += 0.12f;
            else if (v.speed > targetSpeed)
                v.speed -= 0.25f;

            if (v.speed < 0.0f) v.speed = 0.0f;

            v.x += v.speed;

            // HARD CLAMPING: Physically impossible to penetrate into the car ahead!
            if (k > 0) {
                Vehicle& ahead = vehicles[list[k - 1]];
                float minSafeGap = (v.length / 2.0f) + (ahead.length / 2.0f) + 28.0f;
                if (v.x > ahead.x - minSafeGap) {
                    v.x = ahead.x - minSafeGap;
                    v.speed = min(v.speed, ahead.speed);
                    v.isBraking = true;
                }
            }

            // Hard clamp at stop line during active pedestrian crossing
            if (crosswalkActive && v.x > STOP_LINE_X - 10 && v.x < STOP_LINE_X + 20) {
                v.x = STOP_LINE_X - 10;
                v.speed = 0.0f;
                v.isBraking = true;
            }

            // 3. Safe Wraparound: Always respawn safely behind rearmost car
            if (v.x > W + 140) {
                float rearmostX = 0.0f;
                for (size_t j = 0; j < list.size(); j++) {
                    if (list[j] != list[k]) {
                        rearmostX = min(rearmostX, vehicles[list[j]].x);
                    }
                }
                v.x = min(-160.0f, rearmostX - (v.length + 120.0f));
                v.speed = v.maxSpeed;
                v.isBraking = false;
            }
        }
    }
}

void drawAllVehicles() {
    for (size_t i = 0; i < vehicles.size(); i++) {
        Vehicle& v = vehicles[i];
        int y = LANE_Y[v.lane];
        float scale = LANE_SCALE[v.lane];

        if (v.type == 1) {
            drawElectricBus((int)v.x, y, scale, v.isBraking);
        } else if (v.type == 2) {
            drawEcoTruck((int)v.x, y, scale, v.isBraking);
        } else if (v.type == 3) {
            drawRobotaxi((int)v.x, y, scale, v.isBraking);
        } else if (v.type == 4) {
            drawSportsCoupe((int)v.x, y, scale, v.isBraking);
        } else {
            drawCarSedan((int)v.x, y, v.bodyColor, scale, v.isBraking);
        }
    }

    // Emergency Vehicles Dispatch
    if (ambulanceActive) {
        ambulance.x += ambulance.speed;
        ambulance.strobeTimer++;
        bool strobe = ((ambulance.strobeTimer / 5) % 2 == 0);
        drawAmbulanceVehicle((int)ambulance.x, LANE_Y[ambulance.lane], strobe);
        if (ambulance.x > W + 120) {
            ambulanceActive = false;
            ambulance.x = -160.0f;
        }
    }

    if (fireTruckActive) {
        fireTruck.x += fireTruck.speed;
        fireTruck.strobeTimer++;
        bool strobe = ((fireTruck.strobeTimer / 5) % 2 == 0);
        drawFireTruckVehicle((int)fireTruck.x, LANE_Y[fireTruck.lane], strobe);
        if (fireTruck.x > W + 120) {
            fireTruckActive = false;
            fireTruck.x = -200.0f;
        }
    }
}

// ============================================================================
// PEDESTRIAN CROSSING KINEMATICS & UMBRELLA SYSTEM
// ============================================================================
void drawPedestrianModel(int x, int y, int shirtCol, int pantCol, int skinCol, int umbrellaCol, float walkPhase, bool isWalking) {
    filledCircle(x, y - 18, 6, skinCol);
    filledCircle(x, y - 22, 5, COLOR(45, 30, 20));

    filledRect(x - 4, y - 12, x + 4, y + 2, shirtCol, BLACK);

    float swingAngle = isWalking ? (float)(0.45 * sin(walkPhase)) : 0.0f;

    int hipY = y + 2;
    int legLen = 14;
    int leg1X = x + (int)(legLen * sin(swingAngle));
    int leg1Y = hipY + (int)(legLen * cos(swingAngle));
    int leg2X = x + (int)(legLen * sin(-swingAngle));
    int leg2Y = hipY + (int)(legLen * cos(-swingAngle));

    setcolor(pantCol);
    line(x - 2, hipY, leg1X, leg1Y);
    line(x + 2, hipY, leg2X, leg2Y);

    int shY = y - 10;
    int armLen = 11;
    int arm1X = x + (int)(armLen * sin(-swingAngle));
    int arm1Y = shY + (int)(armLen * cos(-swingAngle));
    int arm2X = x + (int)(armLen * sin(swingAngle));
    int arm2Y = shY + (int)(armLen * cos(swingAngle));

    setcolor(shirtCol);
    line(x - 4, shY, arm1X, arm1Y);
    line(x + 4, shY, arm2X, arm2Y);

    if (rainMode) {
        setcolor(COLOR(180, 185, 190));
        line(x + 3, y - 10, x + 8, y - 26);
        filledCircle(x + 8, y - 28, 14, umbrellaCol, COLOR(30, 30, 30));
        filledRect(x - 8, y - 28, x + 24, y - 20, rainMode ? (nightMode ? COLOR(16, 20, 32) : COLOR(88, 106, 122)) : WHITE);
    }
}

void updatePedestrians() {
    bool canCross = (gantryState == GANTRY_PEDESTRIAN);

    for (size_t i = 0; i < pedestrians.size(); i++) {
        Pedestrian& p = pedestrians[i];
        bool isMoving = false;

        if (canCross) {
            p.waiting = false;
            p.y += p.speed * p.direction;
            p.walkPhase += 0.18f;
            isMoving = true;

            if (p.direction == 1 && p.y > ROAD_BOT + 12) {
                p.direction = -1;
                p.y = ROAD_BOT + 12;
            } else if (p.direction == -1 && p.y < SIDEWALK_TOP + 12) {
                p.direction = 1;
                p.y = SIDEWALK_TOP + 12;
            }
        } else {
            p.waiting = true;
            isMoving = false;
        }

        drawPedestrianModel((int)p.x, (int)p.y, p.shirtColor, p.pantColor, p.skinColor, p.umbrellaColor, p.walkPhase, isMoving);
    }
}

// ============================================================================
// WEATHER SIMULATION: DYNAMIC RAIN STREAKS & ROAD SPLASH RIPPLES
// ============================================================================
void updateAndDrawRain() {
    if (!rainMode)
        return;

    int rainCol = nightMode ? COLOR(140, 175, 215) : COLOR(195, 225, 245);
    setcolor(rainCol);

    for (size_t i = 0; i < rainDrops.size(); i++) {
        RainDrop& r = rainDrops[i];
        r.x -= 2.2f;
        r.y += r.speed;

        if (r.y > ROAD_BOT) {
            if (ripples.size() < 25 && rand() % 5 == 0) {
                RoadRipple rip = { (int)r.x, (int)r.y, 1, 8 + (rand() % 6), 0 };
                ripples.push_back(rip);
            }
            r.y = SKY_TOP;
            r.x = rand() % (W + 100);
        }

        line((int)r.x, (int)r.y, (int)(r.x - 3), (int)(r.y + r.length));
    }

    for (size_t i = 0; i < ripples.size(); ) {
        RoadRipple& rip = ripples[i];
        rip.radius++;
        rip.age++;

        setcolor(nightMode ? COLOR(90, 115, 140) : COLOR(160, 185, 210));
        ellipse(rip.x, rip.y, 0, 360, rip.radius * 2, rip.radius);

        if (rip.radius >= rip.maxRadius) {
            ripples.erase(ripples.begin() + i);
        } else {
            i++;
        }
    }
}

// ============================================================================
// SMART CITY IOT TELEMETRY HUD & DASHBOARD
// ============================================================================
void updateClock() {
    frameCount++;
    if (frameCount % 30 == 0) {
        clockSeconds++;
        if (clockSeconds >= 60) {
            clockSeconds = 0;
            clockMinutes++;
            if (clockMinutes >= 60) {
                clockMinutes = 0;
                clockHours = (clockHours + 1) % 24;
            }
        }
    }
}

void drawHUD() {
    filledRect(0, 0, W, TOP_HUD_H, COLOR(18, 22, 30), BLACK);
    setcolor(COLOR(0, 229, 255));
    line(0, TOP_HUD_H, W, TOP_HUD_H);

    drawText(15, 12, "SMART CITY OS v4.2 | AUTONOMOUS V2X URBAN GRID", COLOR(0, 229, 255), 1);

    char timeStr[64];
    sprintf(timeStr, "TIME %02d:%02d:%02d | %s | %s",
            clockHours, clockMinutes, clockSeconds,
            nightMode ? "NIGHT" : "DAY",
            rainMode ? "17 C RAIN" : "24 C SUNNY");
    drawText(440, 12, timeStr, WHITE, 1);

    int totalKW = solarOutputKW + windOutputKW;
    char energyStr[80];
    sprintf(energyStr, "CLEAN GRID: %d kW (SOLAR: %d | WIND: %d) [100%% GREEN]",
            totalKW, (nightMode ? 0 : solarOutputKW), windOutputKW);
    drawText(790, 12, energyStr, COLOR(76, 217, 100), 1);

    filledRect(0, BOTTOM_HUD_Y, W, H, COLOR(18, 22, 30), BLACK);
    setcolor(COLOR(0, 229, 255));
    line(0, BOTTOM_HUD_Y, W, BOTTOM_HUD_Y);

    const char* gantryName = (gantryState == GANTRY_FLOW) ? "AUTONOMOUS FLOW" : (gantryState == GANTRY_WARNING ? "CAUTION" : "CROSSWALK ACTIVE");
    const char* trafDensityName = (trafficDensity == 1) ? "LOW" : (trafficDensity == 2 ? "OPTIMAL" : "RUSH HOUR");

    char statusLeft[120];
    sprintf(statusLeft, "TRAFFIC: %s | ITS GANTRY: [%s] | AQI: 24 (EXCELLENT)", trafDensityName, gantryName);
    drawText(15, BOTTOM_HUD_Y + 7, statusLeft, (gantryState == GANTRY_PEDESTRIAN) ? COLOR(255, 100, 100) : COLOR(0, 255, 150), 1);

    const char* keyGuide = "[N] Day/Night | [R] Rain | [T] Train Boost | [E] Ambulance | [F] Fire | [SPACE] Gantry Mode | [1/2/3] Traffic | [P] EV Bay | [S] Sound | [H] Help";
    drawText(15, BOTTOM_HUD_Y + 24, keyGuide, COLOR(200, 210, 225), 1);
}

// ============================================================================
// DIAGNOSTICS & HELP MODAL OVERLAY ([H] KEY)
// ============================================================================
void drawDiagnosticsOverlay() {
    int modalX1 = 220;
    int modalY1 = 70;
    int modalX2 = W - 220;
    int modalY2 = H - 70;

    filledRect(modalX1, modalY1, modalX2, modalY2, COLOR(20, 26, 38), COLOR(0, 229, 255));
    filledRect(modalX1 + 4, modalY1 + 4, modalX2 - 4, modalY1 + 32, COLOR(30, 42, 60));

    drawText(modalX1 + 15, modalY1 + 12, "SMART CITY AUTONOMOUS INFRASTRUCTURE DIAGNOSTICS", COLOR(0, 229, 255), 1);

    int curY = modalY1 + 46;
    drawText(modalX1 + 20, curY, "=== UNIQUE SMART CITY ARCHITECTURE ===", WHITE, 1);
    curY += 20;
    drawText(modalX1 + 25, curY, "1. Smart Highway Gantry: Replaces clunky signals with dynamic overhead lane displays & V2X radar.", COLOR(200, 215, 230), 1);
    curY += 17;
    drawText(modalX1 + 25, curY, "2. Photoluminescent LED Crosswalk: Ground-embedded neon lighting strips for pedestrian safety.", COLOR(200, 215, 230), 1);
    curY += 17;
    drawText(modalX1 + 25, curY, "3. Zero-Overlap Vehicle Queuing: Hard distance clamping and safe circular wraparound logic.", COLOR(200, 215, 230), 1);
    curY += 17;
    drawText(modalX1 + 25, curY, "4. Aerial Delivery Drone: 4 spinning quadcopter rotors carrying an autonomous package pod.", COLOR(200, 215, 230), 1);
    curY += 17;
    drawText(modalX1 + 25, curY, "5. Scrolling LED Video Billboard: Tech Tower facade with real-time smart city news ticker.", COLOR(200, 215, 230), 1);
    curY += 17;
    drawText(modalX1 + 25, curY, "6. Central Park Water Fountain: Parabolic water droplets animated with gravity acceleration.", COLOR(200, 215, 230), 1);
    curY += 17;
    drawText(modalX1 + 25, curY, "7. Smart Commuter E-Scooter: Citizen riding on sidewalk with spinning alloy wheels & LED light.", COLOR(200, 215, 230), 1);
    curY += 17;
    drawText(modalX1 + 25, curY, "8. Elevated Bullet Train: Viaduct track across skyline with [T] Express Boost speed lines.", COLOR(200, 215, 230), 1);

    curY += 24;
    drawText(modalX1 + 20, curY, "=== INTERACTIVE COMMAND REFERENCE ===", COLOR(255, 215, 0), 1);
    curY += 18;
    drawText(modalX1 + 25, curY, "[N] Toggle Day / Night Mode (activates streetlamps, headlights & lit windows)", WHITE, 1);
    curY += 16;
    drawText(modalX1 + 25, curY, "[R] Toggle Dynamic Rain (falling streaks, water splash ripples & umbrellas)", WHITE, 1);
    curY += 16;
    drawText(modalX1 + 25, curY, "[T] Elevated Metro Bullet Train Express Boost / Launch", WHITE, 1);
    curY += 16;
    drawText(modalX1 + 25, curY, "[E] Smart Ambulance Priority Dispatch (5G V2X green clearance wave)", WHITE, 1);
    curY += 16;
    drawText(modalX1 + 25, curY, "[F] Fire Rescue Emergency Dispatch (ladder engine & priority green wave)", WHITE, 1);
    curY += 16;
    drawText(modalX1 + 25, curY, "[SPACE] Manual Autonomous Gantry Override (toggle Flow vs Pedestrian Mode)", WHITE, 1);
    curY += 16;
    drawText(modalX1 + 25, curY, "[1 / 2 / 3] Traffic Density Level (Low: 3 vehicles, Med: 6, Rush: 9)", WHITE, 1);
    curY += 16;
    drawText(modalX1 + 25, curY, "[P] EV Fast-Charging Station Bay Status Toggle", WHITE, 1);
    curY += 16;
    drawText(modalX1 + 25, curY, "[S] Audio Sound Feedback Chirps Toggle | [ESC] Exit Simulation", COLOR(0, 229, 255), 1);

    curY += 24;
    drawText(modalX1 + 140, modalY2 - 20, "Press [H] or any key to close this diagnostics window", COLOR(255, 235, 120), 1);
}

// ============================================================================
// MASTER FRAME RENDERER
// ============================================================================
void renderFrame() {
    setactivepage(activePage);

    // Physics & Logic Updates
    updateClock();
    updateAutonomousGantryLogic();
    updateVehicles();

    // Render Layers
    drawSky();
    drawClouds();
    drawAirplane();

    drawDistantSkyline();
    drawWindFarm();
    drawSkyscrapers();

    // Aerial Delivery Drone cruising between skyscrapers
    updateAndDrawDeliveryDrone();

    drawElevatedViaduct();
    drawBulletTrain();

    // Sidewalk & Amenities
    drawTrees();
    drawParkBench();
    drawSmartWasteBin();
    drawParkFountain();
    drawCommuterScooter();
    drawEVChargingBay();

    // Smart Highway & Photoluminescent Crosswalk
    drawSmartRoadway();
    drawSmartPoles();

    // Smart Highway Gantry (ITS)
    drawSmartHighwayGantry();

    // Pedestrian Crossing
    updatePedestrians();

    // Multi-Lane Autonomous Highway Vehicles
    drawAllVehicles();

    // Weather Effects
    updateAndDrawRain();

    // IoT Telemetry Dashboard
    drawHUD();

    // Diagnostics Modal
    if (showDiagnostics) {
        drawDiagnosticsOverlay();
    }

    setvisualpage(activePage);
    activePage = 1 - activePage;
}

// ============================================================================
// MAIN ENTRY POINT & INTERACTIVE EVENT CONTROLLER
// ============================================================================
int main() {
    srand((unsigned)time(NULL));

    initwindow(W, H, "Smart City Animation - Advanced Urban Simulation | Computer Graphics");

    initStars();
    initClouds();
    initRain();
    initVehicles();
    initPedestrians();

    bool running = true;

    while (running) {
        renderFrame();

        if (kbhit()) {
            char key = getch();

            switch (key) {
                case 27: // ESC
                    running = false;
                    break;

                case 'n':
                case 'N':
                    nightMode = !nightMode;
                    playAudioFeedback(600, 20);
                    break;

                case 'r':
                case 'R':
                    rainMode = !rainMode;
                    playAudioFeedback(500, 20);
                    break;

                case 't':
                case 'T':
                    expressTrainActive = !expressTrainActive;
                    playAudioFeedback(900, 30);
                    break;

                case 'e':
                case 'E':
                    if (!ambulanceActive) {
                        ambulanceActive = true;
                        ambulance.x = -160.0f;
                        ambulance.strobeTimer = 0;
                        playAudioFeedback(750, 40);
                    }
                    break;

                case 'f':
                case 'F':
                    if (!fireTruckActive) {
                        fireTruckActive = true;
                        fireTruck.x = -200.0f;
                        fireTruck.strobeTimer = 0;
                        playAudioFeedback(450, 40);
                    }
                    break;

                case ' ':
                    if (gantryState == GANTRY_FLOW)
                        gantryState = GANTRY_PEDESTRIAN;
                    else
                        gantryState = GANTRY_FLOW;
                    gantryTimer = 0;
                    playAudioFeedback(650, 25);
                    break;

                case '1':
                    trafficDensity = 1;
                    initVehicles();
                    playAudioFeedback(550, 15);
                    break;
                case '2':
                    trafficDensity = 2;
                    initVehicles();
                    playAudioFeedback(650, 15);
                    break;
                case '3':
                    trafficDensity = 3;
                    initVehicles();
                    playAudioFeedback(750, 15);
                    break;

                case 'p':
                case 'P':
                    evChargingActive = !evChargingActive;
                    playAudioFeedback(800, 20);
                    break;

                case 's':
                case 'S':
                    soundEnabled = !soundEnabled;
                    if (soundEnabled) Beep(1000, 30);
                    break;

                case 'h':
                case 'H':
                    showDiagnostics = !showDiagnostics;
                    playAudioFeedback(700, 20);
                    break;
            }
        }

        delay(20);
    }

    closegraph();
    return 0;
}
