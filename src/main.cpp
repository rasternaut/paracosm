// #include <stdexcept>
// #include <filesystem>
// #include <format>
// #include <print>
// #include "paracosm-colors.h"
// #include "raylib.h"
// #include "raymath.h"

// #include "raygui.h"

// constexpr char ASSET_DIRECTORY[] = "assets";

 #include "core.h"


int main(void)
{

    paracosm::Core pcmCore;
    pcmCore.Run();

    // const int screenWidth = 1920;
    // const int screenHeight = 1080;
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    // InitWindow(screenWidth, screenHeight, "Paracosm Client");

    // // Define the camera to look into our 3d world
    // Camera camera = { 0 };
    // camera.position ={ 5.0f, 4.0f, 5.0f };    // Camera position
    // camera.target = { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    // camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    // camera.fovy = 45.0f;                                // Camera field-of-view Y
    // camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Vector3 cube1Position = {0,0,0};

    // std::filesystem::path asset_dir= std::filesystem::current_path();
    // asset_dir /= "assets";
    // asset_dir /= "kenney_retro-medieval-kit";
    // asset_dir /= "Models";
    // asset_dir /= "GLB Format";

    // std::vector<Model> tiles;
    // std::vector<std::string> nameStrings;
    // std::vector<const char*> tileNames;
    // int activeTile = 0;
    // int tileScrollIndex = 0;

    // for (const auto& entry : std::filesystem::directory_iterator(asset_dir)){
    //     Model model = LoadModel(entry.path().string().c_str());
    //     if(IsModelValid(model)){
    //         tiles.push_back(model);
    //         nameStrings.push_back(entry.path().filename().stem().string());
    //     }
    // }
    // Image map = LoadImage("./caves.jpeg");
    // Model mapFloor = LoadModelFromMesh(GenMeshPlane(map.width/100, map.height/100, 10, 10));
    // // Texture2D mapTexture = LoadTextureFromImage
    // mapFloor.materials[MATERIAL_MAP_DIFFUSE].maps->texture = LoadTextureFromImage(map);


    // for(auto& file : nameStrings){
    //     tileNames.push_back(&file.front());
    // }

    // SetTargetFPS(60);


    // while (!WindowShouldClose())
    // {
    //     UpdateCamera(&camera, CAMERA_ORBITAL);

    //     BeginDrawing();

    //         ClearBackground(PARA_BLACK);

    //         BeginMode3D(camera);

    //             DrawModel(mapFloor, {0,0,0}, 1.0, WHITE);

    //             if(activeTile > -1)
    //                 DrawModel(tiles[activeTile], cube1Position, 1, WHITE);


    //         EndMode3D();

    //         GuiGroupBox({5,5,screenWidth - 10,40}, "Paracosm");

    //         GuiListViewEx({5,50,300, screenHeight - 60}, tileNames.data(), tileNames.size(), &tileScrollIndex, &activeTile, nullptr);

    //     EndDrawing();

    // }

    // CloseWindow();

    // return 0;
}