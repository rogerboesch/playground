//
//  game_vexxon.cpp
//  vector game similiar to Zaxxon
//  Showcase game of the 3d wireframe game engine
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//
//  Controls used:
//   - Joystick 1:
//     - Game: left, right, up, down
//   - Joystick 1, Button 1:
//     - Intro|Game: when hold change camera position with left, right, up, down
//   - Joystick 2, Button 2:
//     - Game: enable/disable shadow
//   - Joystick 1, Button 3:
//     - Game: fire 2 bullets
//   - Joystick 1, Button 4:
//     - Intro: start game
//     - Game: fire 1 bullet
//     - Lost: restart
//

#include "rb_engine.hpp"
#include "rb_object.hpp"
#include "rb_level.hpp"
#include "rb_platform.h"
#include "rb_base.h"
#include "rb_log.h"

#define SCREEN_WIDTH         362    // Screen width
#define SCREEN_HEIGHT        482    //        height
#define HORIZONT             234    // Y position of horizont
#define LEVEL_OFFSET         2      // Offset in level creation
#define SECTION_TIME         2.0    // Time until next section gets created

#define CAMERA_INTRO        {0, 0, -9}
#define CAMER_YAW_INTRO     0
#define CAMERA_PLAY         {-21.799999,-7.900000,-7.290111}
#define CAMERA_YAW_PLAY      -0.916769

#define PLAYER_INTRO        {0, 2, 0}
#define PLAYER_PLAY         {-5, -2, 0}
#define MAX_FLY_HEIGHT       6      // Maximum height of flight
#define MIN_FLY_HEIGHT       2      // Minimum height of flight

#define DISTANCE_LEVEL_END   5      // Switch to end when reach this
#define DISTANCE_ALARM       20     // Z position of when alarm gets fired
#define GROUND               2      // Y position of ground
#define MAX_BORDER_HEIGHT    7      // Maximum height of left border
#define START_DISTANCE       50     // Z position where element start drawing

#define SCORE_BULLETS        50     // Initial numbers of bullets
#define SCORE_BULLETS_ALARM  5      // Alarm when bullets gets below this value
#define SCORE_FUEL           200    // Initial fuel level
#define SCORE_FUEL_ALARM     50     // Alarm when fuel gets below this value
#define SCORE_FUEL_PS        1      // Fuel litres per second reduced
#define SCORE_PS             0.5    // Points per second you get

#define SPEED_MOVEMENT_JET   10     // Speed of jet
#define SPEED_UPDOPWN_JET     7     // Speed of jet (up/down)
#define SPEED_ENEMY_JET     -12     // Speed of enemy jet
#define SPEED_GROUND        -10     // Ground speed
#define SPEED_TANK          -8      // Speed of tanksend

#define STR_PRG_NAME (char*)"VEXXON"
#define STR_COPYRIGHT (char*)"VEXXON 0.3.5 BY RORO"
#define STR_START_MSG (char*)"PRESS BUTTON TO START"
#define STR_FLIGHT_ALARM (char*)"FLIGHT ALARM"
#define STR_PLANE_ALARM (char*)"PLANE ALARM"
#define STR_TANK_ALARM (char*)"TANK ALARM"
#define STR_ROCKET_ALARM (char*)"ROCKET ALARM"
#define STR_HEIGHT_ABBRV (char*)"H"
#define STR_LEVEL_END (char*)"LEVEL COMPLETED"
#define STR_LEVEL_LOST (char*)"LEVEL LOST"

#define GAME_OBJECT_PLAYER 100

typedef enum _GAME_STATE {
    GAME_INITIALIZE, GAME_INTRO, GAME_PLAY, GAME_END, GAME_LOST
} GAME_STATE;

int random(int min, int max) {
    int num = (rand() % (max - min + 1)) + min;
    return num;
}

class Vexxon : public GameEngine {
public:
    Vexxon() {}
    
    GAME_STATE GetGameState() { return _state; }
    
private:
    GameObject* _spaceship;
    GameObject* _player;
    GameObject* _tank;
    GameObject* _rocket;
    GameObject* _jet;
    Mesh* _spaceshipMesh;
    
    int _levelNumber = 1;
    Level _level;

    bool _topView = false;

    float _score = 0;
    GAME_STATE _state = GAME_INITIALIZE;
    
    float _borderDelay = 0;
    float _fYaw = 0;

// MARK: - Life cycle
protected:
	
	virtual bool OnCreate() {
        if (!_level.LoadNumber(_levelNumber)) {
            return false;
        }
        
        // To preload the objects
        _spaceship = new GameObject("spaceship");
        _jet = new GameObject("jet", LEVEL_OBJECT_JET_STANDING);
        _jet->SetColor(colorBlue);

        _tank = new GameObject("tank", LEVEL_OBJECT_TANK);
        _tank->SetColor(colorCyan);

        _rocket = new GameObject("rocket", LEVEL_OBJECT_ROCKET);
        _rocket->SetColor(colorRed);

        _player = new GameObject(_jet->GetMesh(), GAME_OBJECT_PLAYER);
        _player->SetPosition(PLAYER_PLAY);
        _player->SetSpeed(0, 0, 0);
        _player->SetColor(colorBlue);
        AddGameObject(_player);

        _spaceship->SetPosition(PLAYER_INTRO);
        _spaceship->SetSpeed(0, 0, 0);
        _spaceship->SetColor(colorBlue);
        AddGameObject(_spaceship);

		Mat4x4 matProj = MatrixMakeProjection(90.0f, (float)GetScreenHeight() / (float)GetScreenWidth(), 0.1f, 1000.0f);
		SetProjectionMatrix(matProj);

        SetCamera(GAME_INTRO);

        _state = GAME_INTRO;
        
		return true;
	}

	virtual bool OnUpdate(float deltaTime) {
        if (_state == GAME_INTRO) {
            AddNextSection(deltaTime);
            IntroScene();
            
            if (IsControlPressed(CONTROL1_BTN1)) {
                UpdateCameraPosition(deltaTime);
            }
            
            if (IsControlPressed(CONTROL1_BTN4)) {
                _state = GAME_PLAY;

                _player->SetPosition(PLAYER_PLAY);
                _player->SetRotation(0, 0, 0);
                _player->SetRotationSpeed(0, 0, 0);
                _player->SetScale(1, 1, 1);
                _player->SetHidden(false);
                _spaceship->SetHidden(true);
                
                SetCamera(GAME_PLAY);
            }
        }
        else {            
            AddNextSection(deltaTime);

            VerifyGameObjects();

            if (IsControlHold(CONTROL1_BTN1)) {
                UpdateCameraPosition(deltaTime);
            }
            else {
                if (IsJoystick1Up()) {
                    _player->Move(0, -SPEED_UPDOPWN_JET*deltaTime, 0);
                    LimitFlightRange();
                }
                else if (IsJoystick1Down()) {
                    _player->Move(0, SPEED_UPDOPWN_JET*deltaTime, 0);
                    LimitFlightRange();
                }

                if (IsJoystick1Left()) {
                    _player->Move(SPEED_MOVEMENT_JET*deltaTime, 0, 0);
                    _player->SetRotation(0, 0, DEG_TO_RAD(-180));
                    LimitFlightRange();
                }
                else {
                    if (IsJoystick1Right()) {
                        _player->Move(-SPEED_MOVEMENT_JET*deltaTime, 0, 0);
                        _player->SetRotation(0, 0, DEG_TO_RAD(180));
                        LimitFlightRange();
                    }
                    else {
                        _player->SetRotation(0, 0, 0);
                    }
                }
            }
        }
        
		BuildWorldMatrix();
		UpdateCamera(_fYaw);

		return true;
	}

    void Restart() {
        _spaceship->SetHidden(true);

        _player->SetHidden(false);
        _player->SetRotationSpeed(0, 0, 0);
        
        _level.Rewind();
        _score = 0;

        _state = GAME_PLAY;
    }
    
// MARK: - Scenes
private:
    
    void IntroScene() {
        char temp[256];
        sprintf(temp, "LEVEL %d", _levelNumber);
        
        DrawBitmapString(STR_COPYRIGHT, 20, 20, 40, colorWhite);
        DrawBitmapString(temp, 20, 40, 40, colorGray);
        DrawBitmapString(STR_START_MSG, 20, 60, 40, colorWhite);

        _spaceship->SetHidden(false);
        _spaceship->SetRotationSpeed(0, 2, 0);
        _spaceship->SetScale(3, 3, 3);

        _player->SetHidden(true);
    }

// MARK: - State helpers
private:
    void LimitFlightRange() {
        Vec3D pos = _player->GetPosition();
        float height = -pos.y;
        if (height >= MAX_FLY_HEIGHT) {
            pos.y = -MAX_FLY_HEIGHT;
        }
        else if (height <= MIN_FLY_HEIGHT) {
            pos.y = -MIN_FLY_HEIGHT;
        }
        
        if (pos.x < -15) {
            pos.x = -15;
        }
        else if (pos.x > 4) {
            pos.x = 4;
        }

        _player->SetPosition(pos);
    }

    void SetCamera(int state) {
        if (state == GAME_INTRO) {
            SetCameraPos(CAMERA_INTRO);
            _fYaw = CAMER_YAW_INTRO;
        }
        else {
            SetCameraPos(CAMERA_PLAY);
            _fYaw = CAMERA_YAW_PLAY;
        }
    }
 
    void UpdateCameraPosition(float deltaTime) {
        // Used for testing
        if (IsJoystick1Up()) {
            ChangeCameraPosY(-10.0f * deltaTime);
        }
        else if (IsJoystick1Down()) {
            ChangeCameraPosY(10.0f * deltaTime);
        }
        if (IsJoystick1Left()) {
            ChangeCameraPosX(10.0f * deltaTime);
        }
        else if (IsJoystick1Right()) {
            ChangeCameraPosX(-10.0f * deltaTime);
        }

        if (IsControlHold(CONTROL1_BTN2)) {
            _fYaw -= 1 * deltaTime;
        }
        else if (IsControlHold(CONTROL1_BTN3)) {
            _fYaw += 1 * deltaTime;
        }
        else if (IsControlHold(CONTROL1_BTN4)) {
            ChangeCameraPosZ(-10.0f * deltaTime);
            
            if (GetCameraPos().z < -30) {
                SetCameraPosZ(0);
            }
        }
    }
    
// MARK: - Level
private:

    void AddEndLevel(LevelObject levelObject) {
        GameObject* gameObject = new GameObject(GAME_OBJECT_TYPE_CUBE, LEVEL_OBJECT_END);
        gameObject->SetPosition(1, GROUND, START_DISTANCE);
        gameObject->SetSpeed(0, 0, SPEED_GROUND);
        gameObject->SetHidden(true);
        gameObject->SetColor(colorRed);
        AddGameObject(gameObject);
        
        RBLOG(" Add end level marker");
    }
    
    void AddBottomWall(int height) {
        GameObject* gameObject = new GameObject(GAME_OBJECT_TYPE_CUBE);
        gameObject->SetPosition(-14, GROUND-height, START_DISTANCE);
        gameObject->SetScale(29, height, 1);
        gameObject->SetSpeed(0, 0, SPEED_GROUND);
        gameObject->SetColor(colorGreenLight);
        AddGameObject(gameObject);
    }
    
    void AddTopWall(int height) {
        GameObject* gameObject = new GameObject(GAME_OBJECT_TYPE_CUBE);
        gameObject->SetPosition(-14, GROUND-MAX_BORDER_HEIGHT+height-1, START_DISTANCE);
        gameObject->SetScale(29, height, 1);
        gameObject->SetSpeed(0, 0, SPEED_GROUND);
        gameObject->SetColor(colorGreenLight);
        AddGameObject(gameObject);
    }
    
    void AddRightWall() {
        GameObject* gameObject = new GameObject(GAME_OBJECT_TYPE_CUBE);
        gameObject->SetPosition(-14, GROUND-MAX_BORDER_HEIGHT, START_DISTANCE);
        gameObject->SetScale(20, MAX_BORDER_HEIGHT, 1);
        gameObject->SetSpeed(0, 0, SPEED_GROUND);
        gameObject->SetColor(colorGreenLight);
        AddGameObject(gameObject);
    }
    
    void AddLeftWall() {
        GameObject* gameObject = new GameObject(GAME_OBJECT_TYPE_CUBE);
        gameObject->SetPosition(-5, GROUND-MAX_BORDER_HEIGHT, START_DISTANCE);
        gameObject->SetScale(20, MAX_BORDER_HEIGHT, 1);
        gameObject->SetSpeed(0, 0, SPEED_GROUND);
        gameObject->SetColor(colorGreenLight);
        AddGameObject(gameObject);
    }

    void AddRightBorderCube(int height) {
        if (height == 0) {
            GameObject* cube;
            cube = new GameObject(GAME_OBJECT_TYPE_RECTANGLE);
            cube->SetColor(colorGreen);
            cube->SetPosition(-15, GROUND-height, START_DISTANCE);
            cube->SetScale(1, height, 1);
            cube->SetSpeed(0, 0, SPEED_GROUND);
            cube->SetColor(colorGreen);
            AddGameObject(cube);
        }
        else {
            GameObject* cube;
            cube = new GameObject(GAME_OBJECT_TYPE_CUBE);
            cube->SetColor(colorGreen);
            cube->SetPosition(-15, GROUND-height, START_DISTANCE);
            cube->SetScale(1, height, 1);
            cube->SetSpeed(0, 0, SPEED_GROUND);
            AddGameObject(cube);
        }
    }
    
    void AddLeftBorderCube(int height) {
        GameObject* cube = new GameObject(GAME_OBJECT_TYPE_CUBE);
        cube->SetColor(colorGreen);
        cube->SetPosition(15, GROUND-height, START_DISTANCE);
        cube->SetScale(1, height, 1);
        cube->SetSpeed(0, 0, SPEED_GROUND);
        AddGameObject(cube);
    }

    void AddLevelObject(LevelObject levelObject) {
        switch (levelObject.type) {
            case LEVEL_OBJECT_JET_FLYING:
                RBLOG(" Add flying jet");
                AddFlyingJet(levelObject);
                break;
            case LEVEL_OBJECT_JET_STANDING:
                RBLOG(" Add jet");
                AddJet(levelObject);
                break;
            case LEVEL_OBJECT_TANK:
                RBLOG(" Add tank");
                AddTank(levelObject);
                break;
            case LEVEL_OBJECT_ROCKET:
                RBLOG(" Add rocket");
                AddRocket(levelObject);
                break;
            case LEVEL_OBJECT_FUELSILO:
                RBLOG(" Add fuel silo");
                AddFuelSilo(levelObject);
                break;
            case LEVEL_OBJECT_END:
                RBLOG(" Add end level marker");
                AddEndLevel(levelObject);
                break;
        }
    }

    void CreateSection(LevelLine line) {
        int height = line.border-2;
        
        // Wall types
        switch (line.border) {
            // No border
            case 0:
                RBLOG(" Add no border");
                break;
            // Has border
            case 1:
                RBLOG(" Add small border");
                AddLeftBorderCube(3);
                AddRightBorderCube(0);
                break;
            // End level border
            case 2:
                RBLOG(" Add end level border");
                AddLeftBorderCube(1);
                AddRightBorderCube(1);
                break;
            // Has border: height
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                RBLOG_NUM1(" Add wall with height", height);
                AddLeftBorderCube(MAX_BORDER_HEIGHT);
                AddRightBorderCube(height+1);
                AddBottomWall(height);
                break;
            case 10:
                RBLOG(" Add fly trough wall (X)");
                AddLeftBorderCube(MAX_BORDER_HEIGHT);
                AddRightBorderCube(MAX_BORDER_HEIGHT);
                AddBottomWall(2);
                AddTopWall(2);
                break;
            case 11:
                RBLOG(" Add left open wall (Y)");
                AddLeftBorderCube(MAX_BORDER_HEIGHT);
                AddRightBorderCube(MAX_BORDER_HEIGHT);
                AddRightWall();
                break;
            case 12:
                RBLOG(" Add right open wall (Z)");
                AddLeftBorderCube(MAX_BORDER_HEIGHT);
                AddRightBorderCube(MAX_BORDER_HEIGHT);
                AddLeftWall();
                break;
        }
        
        for (auto object : line.gameObjects) {
            AddLevelObject(object);
        }
    }

    void AddNextSection(float deltaTime) {
        _borderDelay += deltaTime;

        if (_borderDelay >= SECTION_TIME) {
            if (_state == GAME_INTRO) {
                AddLeftBorderCube(MAX_BORDER_HEIGHT);
                AddRightBorderCube(MAX_BORDER_HEIGHT);
            }
            else if (_state == GAME_END) {
                AddLeftBorderCube(1);
                AddRightBorderCube(1);
            }
            else {
                if (_level.HasMoreLines()) {
                    LevelLine line;
                    _level.GetLine(line);
                    
                    CreateSection(line);
                }
            }
            
            _borderDelay = 0;
        }
    }

// MARK: - Gems
private:

    void AddFuelSilo(LevelObject levelObject) {
        int x = -levelObject.x;

        GameObject* gameObject = new GameObject(GAME_OBJECT_TYPE_CUBE, levelObject.type);
        gameObject->SetPosition(LEVEL_OFFSET+x, GROUND+1, START_DISTANCE);
        gameObject->SetScale(2, 3, 2);
        gameObject->SetSpeed(0, 0, SPEED_GROUND);
        gameObject->SetColor(colorCyan);
        AddGameObject(gameObject);
    }

// MARK: - Enemies
private:

    void AddFlyingJet(LevelObject levelObject) {
        int x = -levelObject.x;

        GameObject* gameObject = new GameObject(GAME_OBJECT_TYPE_CUBE, levelObject.type);
        gameObject->SetPosition(LEVEL_OFFSET+x, GROUND-2, START_DISTANCE);
        gameObject->SetRotationSpeed(2, 0, 0);
        gameObject->SetScale(1, 1, 1);
        gameObject->SetSpeed(0, 0, SPEED_ENEMY_JET);
        gameObject->SetColor(colorRed);
        AddGameObject(gameObject);
    }

    void AddJet(LevelObject levelObject) {
        int x = -levelObject.x;
        
        GameObject* gameObject = new GameObject(_jet->GetMesh(), levelObject.type);
        gameObject->SetPosition(LEVEL_OFFSET+x, GROUND+1, START_DISTANCE);
        gameObject->SetSpeed(0, 0, SPEED_GROUND);
        gameObject->SetColor(colorRedLight);
        AddGameObject(gameObject);
    }

    void AddRocket(LevelObject levelObject) {
        int x = -levelObject.x;
        
        GameObject* gameObject = new GameObject(_rocket->GetMesh(), levelObject.type);
        gameObject->SetPosition(LEVEL_OFFSET+x, GROUND+1, START_DISTANCE);
        gameObject->SetRotation(0, 0, DEG_TO_RAD(-180));
        gameObject->SetSpeed(0, 0, SPEED_GROUND);
        gameObject->SetColor(colorYellow);
        AddGameObject(gameObject);
    }

    void AddTank(LevelObject levelObject) {
        int x = -levelObject.x;

        GameObject* gameObject = new GameObject(_tank->GetMesh(), levelObject.type);
        gameObject->SetPosition(LEVEL_OFFSET+x, GROUND+1, START_DISTANCE);
        gameObject->SetRotation(0, 0, DEG_TO_RAD(-180));
        gameObject->SetColor(colorViolett);

        // Change speed maybe later?
        gameObject->SetSpeed(0, 0, SPEED_GROUND);
        AddGameObject(gameObject);
    }

    void VerifyGameObjects() {
        for (auto gameObject : m_gameObjects) {
            // End of level marker
            if (gameObject->GetTag() == LEVEL_OBJECT_END && !gameObject->IsDead()) {
                if (gameObject->GetPosition().z < DISTANCE_LEVEL_END) {
                    RBLOG("End of level marker reached");
                    gameObject->SetDead();
                    _state = GAME_END;
                    return;
                }
            }
        }
    }

// MARK: - Player
private:

// MARK: - Drawing
private:

    void DrawFloor() {
        DrawLine(134, HORIZONT, SCREEN_WIDTH-136, HORIZONT, colorGray);
    }
    
};

extern "C" {
	
    static Vexxon* game = NULL;

	void vexxon_start() {
        if (game == NULL) {
            game = new Vexxon();
        }

        game->Init(STR_PRG_NAME, SCREEN_WIDTH, SCREEN_HEIGHT, false);
        game->Start();
	}

	void vexxon_frame() {
        if (game == NULL) {
            return;
        }

        if (game->IsFinished()) {
            return;
        }

        game->Frame();
	}

    void vexxon_stop() {
        if (game == NULL) {
            return;
        }

        game->Stop();
        delete game;
        game = NULL;
    }

    void game_set_control_state(int code, int state) {
        if (game == NULL) {
            return;
        }

        game->SetControlState(code, (CONTROLSTATE)state);
    }

    int game_get_control_state(int code) {
        if (game == NULL) {
            return 0;
        }

        return game->GetControl(code).state;
    }

    int game_get_state(int code) {
        if (game == NULL) {
            return 0;
        }

        return game->GetGameState();
    }

    void game_set_filled(int code) {
        if (game == NULL) {
            return;
        }

        return game->SetFilled(code);
    }

}
