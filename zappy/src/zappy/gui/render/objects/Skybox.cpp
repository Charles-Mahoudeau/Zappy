/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Skybox
*/

#include "Skybox.hpp"

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <array>
#include <cstddef>
#include <filesystem>
#include <span>
#include <string>
#include <string_view>

namespace zappy::gui::render {

#ifdef PLATFORM_DESKTOP
constexpr int GLSL_VERSION = 330;
#else  // PLATFORM_ANDROID, PLATFORM_WEB
constexpr int GLSL_VERSION = 100;
#endif

std::string Skybox::shadersPath(std::string_view shadersDirectory) {
    const std::string directory = std::string{shadersDirectory} + "/glsl" + std::to_string(GLSL_VERSION);

    if (!std::filesystem::is_directory(directory)) {
        throw SkyboxException{"Shader directory for GLSL version " + std::to_string(GLSL_VERSION) +
                              " not found: " + directory};
    }
    return directory;
}

::TextureCubemap Skybox::loadCubemapImage(std::string_view path) {
    const Image image = LoadImage(std::string{path}.c_str());
    if (!IsImageValid(image)) {
        throw SkyboxException{"Failed to load skybox image from: " + std::string{path}};
    }
    const ::TextureCubemap cubemap = LoadTextureCubemap(image, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(image);
    if (!IsTextureValid(cubemap)) {
        throw SkyboxException{"Failed to create skybox cubemap from: " + std::string{path}};
    }
    return cubemap;
}

::TextureCubemap Skybox::generateCubemap(::Shader shader, ::Texture2D panorama, int size) {
    ::TextureCubemap cubemap{};

    rlDisableBackfaceCulling();

    const unsigned int rbo = rlLoadTextureDepth(size, size, true);
    cubemap.id = rlLoadTextureCubemap(nullptr, size, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);

    const unsigned int fbo = rlLoadFramebuffer();
    rlFramebufferAttach(fbo, rbo, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
    rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X, 0);
    if (!rlFramebufferComplete(fbo)) {
        rlUnloadFramebuffer(fbo);
        return cubemap;
    }

    rlEnableShader(shader.id);

    const Matrix projection = MatrixPerspective(90.0 * DEG2RAD, 1.0, rlGetCullDistanceNear(), rlGetCullDistanceFar());
    rlSetUniformMatrix(GetShaderLocation(shader, "matProjection"), projection);

    const std::array<Matrix, 6> views = {
        MatrixLookAt(Vector3{.x = 0.0F, .y = 0.0F, .z = 0.0F}, Vector3{.x = 1.0F, .y = 0.0F, .z = 0.0F},
                     Vector3{.x = 0.0F, .y = -1.0F, .z = 0.0F}),
        MatrixLookAt(Vector3{.x = 0.0F, .y = 0.0F, .z = 0.0F}, Vector3{.x = -1.0F, .y = 0.0F, .z = 0.0F},
                     Vector3{.x = 0.0F, .y = -1.0F, .z = 0.0F}),
        MatrixLookAt(Vector3{.x = 0.0F, .y = 0.0F, .z = 0.0F}, Vector3{.x = 0.0F, .y = 1.0F, .z = 0.0F},
                     Vector3{.x = 0.0F, .y = 0.0F, .z = 1.0F}),
        MatrixLookAt(Vector3{.x = 0.0F, .y = 0.0F, .z = 0.0F}, Vector3{.x = 0.0F, .y = -1.0F, .z = 0.0F},
                     Vector3{.x = 0.0F, .y = 0.0F, .z = -1.0F}),
        MatrixLookAt(Vector3{.x = 0.0F, .y = 0.0F, .z = 0.0F}, Vector3{.x = 0.0F, .y = 0.0F, .z = 1.0F},
                     Vector3{.x = 0.0F, .y = -1.0F, .z = 0.0F}),
        MatrixLookAt(Vector3{.x = 0.0F, .y = 0.0F, .z = 0.0F}, Vector3{.x = 0.0F, .y = 0.0F, .z = -1.0F},
                     Vector3{.x = 0.0F, .y = -1.0F, .z = 0.0F}),
    };

    rlViewport(0, 0, size, size);
    rlActiveTextureSlot(0);
    rlEnableTexture(panorama.id);

    const int viewLocation = GetShaderLocation(shader, "matView");
    for (std::size_t face = 0; face < views.size(); ++face) {
        rlSetUniformMatrix(viewLocation, views.at(face));
        rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0,
                            RL_ATTACHMENT_CUBEMAP_POSITIVE_X + static_cast<int>(face), 0);
        rlEnableFramebuffer(fbo);
        rlClearScreenBuffers();
        rlLoadDrawCube();
    }

    rlDisableShader();
    rlDisableTexture();
    rlDisableFramebuffer();
    rlUnloadFramebuffer(fbo);
    rlViewport(0, 0, rlGetFramebufferWidth(), rlGetFramebufferHeight());

    cubemap.width = size;
    cubemap.height = size;
    cubemap.mipmaps = 1;
    cubemap.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    return cubemap;
}

::TextureCubemap Skybox::loadPanoramaCubemap(std::string_view path, const std::string& shadersDirectory) {
    const ::Shader conversion =
        LoadShader((shadersDirectory + "/cubemap.vs").c_str(), (shadersDirectory + "/cubemap.fs").c_str());
    if (!IsShaderValid(conversion)) {
        throw SkyboxException{"Failed to load cubemap conversion shader from: " + shadersDirectory};
    }
    const int equirectangularMap = 0;
    SetShaderValue(conversion, GetShaderLocation(conversion, "equirectangularMap"), &equirectangularMap,
                   SHADER_UNIFORM_INT);

    const ::Texture2D panorama = LoadTexture(std::string{path}.c_str());
    if (!IsTextureValid(panorama)) {
        UnloadShader(conversion);
        throw SkyboxException{"Failed to load panorama from: " + std::string{path}};
    }
    const ::TextureCubemap cubemap = generateCubemap(conversion, panorama, kCubemapSize);
    UnloadTexture(panorama);
    UnloadShader(conversion);
    if (!IsTextureValid(cubemap)) {
        throw SkyboxException{"Failed to generate cubemap from panorama: " + std::string{path}};
    }
    return cubemap;
}

Skybox::Skybox(std::string_view path, std::string_view shadersDirectory, bool hdr) {
    const std::string directory = shadersPath(shadersDirectory);

    _cubemap = hdr ? loadPanoramaCubemap(path, directory) : loadCubemapImage(path);

    _shader = LoadShader((directory + "/skybox.vs").c_str(), (directory + "/skybox.fs").c_str());
    if (!IsShaderValid(_shader)) {
        UnloadTexture(_cubemap);
        throw SkyboxException{"Failed to load skybox shader from: " + directory};
    }

    const int environmentMap = MATERIAL_MAP_CUBEMAP;
    const int toggle = hdr ? 1 : 0;
    SetShaderValue(_shader, GetShaderLocation(_shader, "environmentMap"), &environmentMap, SHADER_UNIFORM_INT);
    SetShaderValue(_shader, GetShaderLocation(_shader, "doGamma"), &toggle, SHADER_UNIFORM_INT);
    SetShaderValue(_shader, GetShaderLocation(_shader, "vflipped"), &toggle, SHADER_UNIFORM_INT);

    _model = LoadModelFromMesh(GenMeshCube(kCubeSize, kCubeSize, kCubeSize));
    if (!IsModelValid(_model)) {
        UnloadShader(_shader);
        UnloadTexture(_cubemap);
        throw SkyboxException{"Failed to create skybox model"};
    }
    const std::span<::Material> materials{_model.materials, static_cast<std::size_t>(_model.materialCount)};
    materials.front().shader = _shader;
    SetMaterialTexture(&materials.front(), MATERIAL_MAP_CUBEMAP, _cubemap);
}

Skybox::~Skybox() {
    if (IsTextureValid(_cubemap)) {
        UnloadTexture(_cubemap);
    }
    if (IsModelValid(_model)) {
        UnloadModel(_model);
    }
    if (IsShaderValid(_shader)) {
        UnloadShader(_shader);
    }
}

Skybox::Skybox(Skybox&& other) noexcept : _model(other._model), _shader(other._shader), _cubemap(other._cubemap) {
    other._model = {};
    other._shader = {};
    other._cubemap = {};
}

Skybox& Skybox::operator=(Skybox&& other) noexcept {
    if (this != &other) {
        if (IsTextureValid(_cubemap)) {
            UnloadTexture(_cubemap);
        }
        if (IsModelValid(_model)) {
            UnloadModel(_model);
        }
        if (IsShaderValid(_shader)) {
            UnloadShader(_shader);
        }
        _model = other._model;
        _shader = other._shader;
        _cubemap = other._cubemap;
        other._model = {};
        other._shader = {};
        other._cubemap = {};
    }
    return *this;
}

void Skybox::draw() const {
    if (!IsModelValid(_model)) {
        return;
    }
    rlDisableBackfaceCulling();
    rlDisableDepthMask();
    DrawModel(_model, Vector3{.x = 0.0F, .y = 0.0F, .z = 0.0F}, 1.0F, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
}

}  // namespace zappy::gui::render
