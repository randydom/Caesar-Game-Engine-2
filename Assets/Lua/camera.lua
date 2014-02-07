local camera = {}

local ScreenWidth = GetClientsSize()["Width"];
local ScreenHeight = GetClientsSize()["Height"];

local verticeArray = {};
verticeArray[1] = Vertice({["Point"] = Vector4(0.0, 0.0, 0.0), 
                           ["Texture"] = Vector4(0.0, 0.0, 0.0), });
verticeArray[2] = Vertice({["Point"] = Vector4(1.0, 0.0, 0.0), 
                           ["Texture"] = Vector4(1.0, 0.0, 0.0), });
verticeArray[3] = Vertice({["Point"] = Vector4(0.0, 1.0, 0.0), 
                           ["Texture"] = Vector4(0.0, 1.0, 0.0), });
verticeArray[4] = Vertice({["Point"] = Vector4(1.0, 1.0, 0.0), 
                           ["Texture"] = Vector4(1.0, 1.0, 0.0), });
local faces = {0, 1, 2, 2, 1, 3};

local spriteModel = Model();
spriteModel.Vertices = verticeArray;
spriteModel.Faces = faces;
local Quad2DDrawable = BasicDrawableObject({[Keys["BasicDrawable"]["MODEL"]]            = spriteModel,
                                            [Keys["BasicDrawable"]["Dimension"]]        = Dimension["2D"],
                                            [Keys["BasicDrawable"]["VertexShaderFile"]] = "Assets/ShaderFiles/VS_0_Regular.cso",
                                            [Keys["BasicDrawable"]["PixelShaderFile"]]  = "Assets/ShaderFiles/PS_2_Texture.cso",
                                           });
local Quad2D = Object({[Keys["ObjectInfo"]["DrawableObj"]]  = Quad2DDrawable,
                       [Keys["ObjectInfo"]["Scale"]]        = Vector4(ScreenWidth, ScreenHeight, 1.0, 1.0),
                      });
local sniperTexture = BasicTexture("Assets/Texture/Sniper2.png");

                      
local Quad2DCam = Camera({[Keys["Camera"]["Eye"]]              = Vector4(0.0, 5.0, -60.0, 0.0),
                           [Keys["Camera"]["TargetMagnitude"]] = Vector4(0.0, 0.0, 1.0, 0.0),
                           [Keys["Camera"]["Up"]]              = Vector4(0.0, 1.0, 0.0, 0.0),
                           [Keys["Camera"]["RadianRoll"]]      = 0.0,
                           [Keys["Camera"]["RadianPitch"]]     = 0.0,
                           [Keys["Camera"]["RadianYaw"]]       = 0.0,
                           [Keys["Camera"]["FovAngle"]]        = 0.785398163,
                           [Keys["Camera"]["NearZ"]]           = 0.01,              
                           [Keys["Camera"]["FarZ"]]            = 5000.0,
                           [Keys["Camera"]["ClearColor"]]      = Vector4(0.5, 0.5, 0.5, 1.0),
                           [Keys["Camera"]["Global2DTexture"]] = sniperTexture,
                           [Keys["Camera"]["GlobalUserData"]]  = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  },
                           [Keys["Camera"]["InclusionState"]]  = InclusionType["Include"],
                           [Keys["Camera"]["ObjectList"]]      = { Quad2D },
                         }); 
Quad2DCam:SetAsMain();
                      

regularCam = Camera({[Keys["Camera"]["Eye"]]             = Vector4(0.0, 5.0, -60.0, 0.0),
                     [Keys["Camera"]["TargetMagnitude"]] = Vector4(0.0, 0.0, 1.0, 0.0),
                     [Keys["Camera"]["Up"]]              = Vector4(0.0, 1.0, 0.0, 0.0),
                     [Keys["Camera"]["RadianRoll"]]      = 0.0,
                     [Keys["Camera"]["RadianPitch"]]     = 0.0,
                     [Keys["Camera"]["RadianYaw"]]       = 0.0,
                     [Keys["Camera"]["FovAngle"]]        = 0.785398163,
                     [Keys["Camera"]["NearZ"]]           = 0.01,              
                     [Keys["Camera"]["FarZ"]]            = 5000.0,
                     [Keys["Camera"]["ClearColor"]]      = Vector4(0.5, 0.5, 0.5, 1.0),
                     [Keys["Camera"]["GlobalUserData"]]  = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  },
                     [Keys["Camera"]["InclusionState"]]  = InclusionType["Exclude"],
                     [Keys["Camera"]["ObjectList"]]      = { Quad2D },
                   }); 
--regularCam:SetAsMain();

local regularCapture = BasicScreenCapture({[Keys["ScreenShot"]["Width"]]    = ScreenWidth,
                                           [Keys["ScreenShot"]["Height"]]   = ScreenHeight,
                                           [Keys["ScreenShot"]["CameraID"]] = regularCam, });

Quad2D:Add2DTexture(regularCapture:GetTexture());

local currentShaderState = 0;
OnKeyDown(KeyCode["R"],
    function()
        if(currentShaderState == 0) then
            Quad2DDrawable:ChangePS("Assets/ShaderFiles/PS_3_Sniper.cso");
            currentShaderState = 1;
        else 
            Quad2DDrawable:ChangePS("Assets/ShaderFiles/PS_2_Texture.cso");
            currentShaderState = 0;
        end
    end);

local CamLeftButton   = false;    local CamRightButton  = false;
local CamUpButton     = false;    local CamDownButton   = false;
local CamPgUpButton   = false;    local CamPgDownButton = false;

function UpdateCamera(time, ID)
    if(CamLeftButton == true) then
        regularCam.Yaw = regularCam.Yaw - (0.001 * time);
        Quad2DCam.Yaw = regularCam.Yaw;
    end
    if(CamRightButton == true) then
        regularCam.Yaw = regularCam.Yaw + (0.001 * time);
        Quad2DCam.Yaw = regularCam.Yaw;
    end
    
    if(CamPgUpButton == true) then
        regularCam.Pitch = regularCam.Pitch - (0.001 * time);
        Quad2DCam.Pitch = regularCam.Pitch;
    end
    if(CamPgDownButton == true) then
        regularCam.Pitch = regularCam.Pitch + (0.001 * time);
        Quad2DCam.Pitch = regularCam.Pitch;
    end
    
    if(CamUpButton == true) then
        local moveDistance = 0.05 * time;
        regularCam.Eye = MoveObject(regularCam.Eye, regularCam.TargetMagintude, regularCam.Pitch,  regularCam.Yaw,  regularCam.Roll, moveDistance)
        Quad2DCam.Eye = regularCam.Eye;
    end
    if(CamDownButton == true) then
        local moveDistance = -0.05 * time;
        regularCam.Eye = MoveObject(regularCam.Eye, regularCam.TargetMagintude, regularCam.Pitch,  regularCam.Yaw,  regularCam.Roll, moveDistance)
        Quad2DCam.Eye = regularCam.Eye;
    end
end

LoopCall(15, UpdateCamera);

OnKeyDown(KeyCode["A"], function() CamLeftButton = true; end);-- Left
OnKeyDown(KeyCode["D"], function() CamRightButton = true; end);-- Right
OnKeyDown(KeyCode["W"], function() CamUpButton = true; end);-- Up
OnKeyDown(KeyCode["S"], function() CamDownButton = true; end);-- Down
OnKeyDown(KeyCode["Q"], function() CamPgUpButton = true; end);-- Up
OnKeyDown(KeyCode["E"], function() CamPgDownButton = true; end);-- Down

OnKeyUp(KeyCode["A"], function() CamLeftButton = false; end);-- Left
OnKeyUp(KeyCode["D"], function() CamRightButton = false; end);-- Right
OnKeyUp(KeyCode["W"], function() CamUpButton = false; end);-- Up
OnKeyUp(KeyCode["S"], function() CamDownButton = false; end);-- Down
OnKeyUp(KeyCode["Q"], function() CamPgUpButton = false; end);-- Up
OnKeyUp(KeyCode["E"], function() CamPgDownButton = false; end);-- Down

LoopCall(1000, 
    function() 
        local clientSize = GetClientsSize();
        if(clientSize["Width"] ~= ScreenWidth or clientSize["Height"] ~= ScreenHeight) then
            ScreenWidth = clientSize["Width"];
            ScreenHeight = clientSize["Height"];
            Quad2D.Scale = Vector4(ScreenWidth, ScreenHeight, 1.0, 1.0);
            regularCapture:Release();
            regularCapture = BasicScreenCapture({[Keys["ScreenShot"]["Width"]]    = ScreenWidth,
                                                 [Keys["ScreenShot"]["Height"]]   = ScreenHeight,
                                                  [Keys["ScreenShot"]["CameraID"]] = regularCam, });

            Quad2D.Texture2D = { regularCapture:GetTexture() };
        end
    end);

return camera

