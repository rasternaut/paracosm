#include "core.h"
#include "raylib.h"
#include "raymath.h"
#include "paracosm-colors.h"

#include "action.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <string>
#include <format>
#include <vector>
namespace paracosm {

Core::Core(){
    screenHeight = 1080;
    screenWidth = 1920;

    InitWindow(screenWidth, screenHeight, "Paracosm Client");
    SetTargetFPS(60);
}

Core::~Core(){

    CloseWindow();
}

void Core::Run(){

    Image map = LoadImage("./caves.jpeg");
    int scaledMapWidth= map.width / 100;
    int scaledMapHeight = map.height / 100;
    Model mapFloor = LoadModelFromMesh(GenMeshPlane(scaledMapWidth, scaledMapHeight, 1, 1));
    mapFloor.materials[MATERIAL_MAP_DIFFUSE].maps->texture = LoadTextureFromImage(map);

    Vector3 mapCenter = {(float)scaledMapWidth / 2, 0, (float)scaledMapHeight / 2};

    Matrix mapTranslate = MatrixTranslate(mapCenter.x, mapCenter.y, mapCenter.z);

    Vector3 cameraPosition = mapCenter;
    cameraPosition.y = 40;
    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = cameraPosition;    // Camera position
    camera.target = {14, 1, 14};      // Camera looking at point
    camera.up = { 0.0f, 0.0f, -1.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Vector3 origin = {0,0,0};
    Vector3 opposite = {(float)scaledMapWidth, 0, (float)scaledMapHeight};

    std::vector<Vector3> clickedPoints;


        while(!WindowShouldClose())
        {


            UpdateCamera(&camera, CAMERA_FREE);

            camera.target.x = camera.position.x;
            camera.target.z = camera.position.z;
            camera.target.y = 0;

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){

                Ray ray = GetScreenToWorldRay(GetMousePosition(), camera);
                for(int ii = 0; ii < mapFloor.meshCount; ii++){
                    RayCollision collision = {};
                    collision = GetRayCollisionMesh(ray, mapFloor.meshes[ii], mapTranslate);
                    if(collision.hit){
                        clickedPoints.push_back(collision.point);
                    }

                }

            }

             BeginDrawing();
                ClearBackground(PARA_BLACK);

                BeginMode3D(camera);
                    DrawModel(mapFloor, mapCenter, 1.0, WHITE);
                    DrawSphere(camera.target, .5, PINK);
                    DrawSphere(origin, 1, GREEN);
                    DrawSphere(opposite, 1, RED);

                    for( Vector3 point : clickedPoints){
                        DrawSphere(point, .2, ORANGE);
                        Vector3 xLineStart, xLineStop;
                        Vector3 zLineStart, zLineStop;

                        xLineStart = {point.x, 0, 0};
                        xLineStop  = {point.x, 0, (float)scaledMapWidth};
                        DrawLine3D(xLineStart, xLineStop, ORANGE);

                        zLineStart = {0,0, point.z};
                        zLineStop = {(float)scaledMapHeight, 0, point.z};
                        DrawLine3D(zLineStart, zLineStop, ORANGE);

                    }
                EndMode3D();
            // draw scene
            // draw any "active/interacted" peices
            // draw GUI

            std::string cameraStatus = std::format("Camera\nx:{}\ny:{}\nz:{}\n\nTarget\nx:{}\ny:{}\nz:{}", camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z);
            GuiTextBox({5,5,140,190}, &cameraStatus.front(), cameraStatus.size(), false);

             if(GuiButton({5, 210,140, 20},"Map Grid" )){

             }

            EndDrawing();
        }


}

}
