#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Project/ProjectApplication.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include <spdlog/spdlog.h>

#include <unordered_map>
#include <filesystem>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <vector>
#include <queue>
#include <set>

static std::string Slurp(std::string_view path)
{
    std::ifstream file(path.data(), std::ios::ate);
    std::string result(file.tellg(), '\0');
    file.seekg(0);
    file.read((char*)result.data(), result.size());
    return result;
}

namespace fs = std::filesystem;

// This function generates a path to a texture file based on a base path and a cgltf_image object.
static std::string FindTexturePath(const fs::path& basePath, const cgltf_image* image)
{
    std::string texturePath;

    // If the image URI is null, generate a new path for the texture file based on the image name.
    if (!image->uri)
    {
        auto newPath = basePath / image->name;

        // If the image name does not have an extension, add the appropriate extension based on the MIME type.
        if (!newPath.has_extension())
        {
            if (std::strcmp(image->mime_type, "image/png") == 0)
            {
                newPath.replace_extension("png");
            }
            else if (std::strcmp(image->mime_type, "image/jpg") == 0)
            {
                newPath.replace_extension("jpg");
            }
        }

        // Convert the new path to a string and store it in the texturePath variable.
        texturePath = newPath.generic_string();
    }
    else
    {
        // If the image URI is not null, generate a new path for the texture file by appending the URI to the base path.
        texturePath = (basePath / image->uri).generic_string();
    }

    // Return the texture path as a string.
    return texturePath;
}

void ProjectApplication::AfterCreatedUiContext()
{
}

void ProjectApplication::BeforeDestroyUiContext()
{
}

// This function loads the application and its resources.
bool ProjectApplication::Load()
{
    // Call the base class Load function to initialize the application.
    if (!Application::Load())
    {
        spdlog::error("App: Unable to load");
        return false;
    }

    // Load the main shader program using the MakeShader function.
    if (!MakeShader("./data/shaders/main.vs.glsl", "./data/shaders/main.fs.glsl"))
    {
        return false;
    }

    // Load the model using the LoadModel function.
    LoadModel("./data/models/SM_Deccer_Cubes_Textured.gltf");

    // Return true to indicate that the application and its resources have been loaded successfully.
    return true;
}

// This function updates the application state every frame.
void ProjectApplication::Update(float deltaTime)
{
    // Check if the escape key is pressed. If it is, close the application.
    if (IsKeyPressed(GLFW_KEY_ESCAPE))
    {
        Close();
    }

    // Update the elapsed time by adding the delta time to the _elapsedTime member variable.
    _elapsedTime += deltaTime;
}

// This function renders a scene using OpenGL.
void ProjectApplication::RenderScene([[maybe_unused]] float deltaTime)
{
    // Create the projection and view matrices using the glm library.
    const auto projection = glm::perspective(glm::radians(80.0f), 1920.0f / 1080.0f, 0.1f, 256.0f);
    const auto view = glm::lookAt(
        glm::vec3(3 * std::cos(glfwGetTime() / 4), 2, -3 * std::sin(glfwGetTime() / 4)),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0));

    // Clear the color and depth buffers using the glClear function.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the shader program using the glUseProgram function.
    glUseProgram(_shaderProgram);

    // Set the projection and view matrices using the glUniformMatrix4fv function.
    glUniformMatrix4fv(0, 1, false, glm::value_ptr(projection));
    glUniformMatrix4fv(1, 1, false, glm::value_ptr(view));

    // Create two structs called ObjectData and BatchData, which are used to store information about the objects and batches in the scene.
    struct ObjectData
    {
        uint32_t transformIndex;
        uint32_t baseColorIndex;
        uint32_t normalIndex;
    };
    struct BatchData
    {
        std::vector<ObjectData> objects;
        std::vector<MeshIndirectInfo> indirectCommands;
    };

    // Create a vector of BatchData objects called objectBatches, which is used to group objects with the same base color texture.
    std::vector<BatchData> objectBatches(_cubes.Commands.size());

    // Create a vector of std::set<uint32_t> objects called textureHandles, which is used to store the texture handles for each batch.
    std::vector<std::set<uint32_t>> textureHandles(_cubes.Commands.size());

    // Loop through each mesh in the scene and add it to the appropriate batch in the objectBatches vector.
    // Also add the base color texture handle to the textureHandles set for the appropriate batch.
    for (const auto& mesh : _cubes.Meshes)
    {
        const auto index = mesh.BaseColorTexture / 16;
        objectBatches[index].indirectCommands.emplace_back(MeshIndirectInfo
        {
            mesh.IndexCount,
            1,
            mesh.indexOffset,
            mesh.VertexOffset,
            1
        });
        objectBatches[index].objects.emplace_back(ObjectData
        {
            mesh.TransformIndex,
            mesh.BaseColorTexture % 16,
            mesh.NormalTexture
        });
        textureHandles[index].insert(_cubes.Textures[mesh.BaseColorTexture]);
    }

    // Update the transform data buffer using the glNamedBufferData function, and bind it to the shader storage buffer using the glBindBufferBase function.
    glNamedBufferData(
        _cubes.TransformData,
        _cubes.Transforms.size() * sizeof(glm::mat4),
        _cubes.Transforms.data(),
        GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _cubes.TransformData);

    // Loop through each batch in the objectBatches vector and update the object data buffer and command buffer using the glNamedBufferData function.
    // Also bind the object data buffer to the shader storage buffer using the glBindBufferBase function.
    for (uint32_t index = 0; const auto& batch : objectBatches)
    {
        glNamedBufferData(
            _cubes.ObjectData[index],
            batch.objects.size() * sizeof(ObjectData),
            batch.objects.data(),
            GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _cubes.ObjectData[index]);

        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, _cubes.Commands[index]);
        glNamedBufferData(
            _cubes.Commands[index],
            batch.indirectCommands.size() * sizeof(MeshIndirectInfo),
            batch.indirectCommands.data(),
            GL_DYNAMIC_DRAW);

        // Loop through each texture handle in the textureHandles set and set the texture uniforms using the glUniform1i function.
        // Also bind the texture using the glActiveTexture and glBindTexture functions.
        uint32_t offset = 0;
        for (const auto texture : textureHandles[index])
        {
            glUniform1i(2 + offset, offset);
            glActiveTexture(GL_TEXTURE0 + offset);
            glBindTexture(GL_TEXTURE_2D, texture);
            offset++;
        }

        // Bind the vertex array using the glBindVertexArray function, and draw the meshes using the glMultiDrawElementsIndirect function.
        glBindVertexArray(_cubes.InputLayout);
        glMultiDrawElementsIndirect(
            GL_TRIANGLES,
            GL_UNSIGNED_INT,
            nullptr,
            batch.indirectCommands.size(),
            sizeof(MeshIndirectInfo));
        index++;
    }
}

// This function renders the user interface using the ImGui library.
void ProjectApplication::RenderUI(float deltaTime)
{
    // Begin the ImGui window.
    ImGui::Begin("Window");

    // Add some text to the window.
    ImGui::TextUnformatted("Hello World!");

    // Add some more text to the window, including the elapsed time and delta time.
    ImGui::Text("Time in seconds since startup: %f", _elapsedTime);
    ImGui::Text("The delta time between frames: %f", deltaTime);

    // End the ImGui window.
    ImGui::End();

    // Show the ImGui demo window.
    ImGui::ShowDemoWindow();
}

// This function creates a shader program from a vertex shader file and a fragment shader file.
bool ProjectApplication::MakeShader(std::string_view vertexShaderFilePath, std::string_view fragmentShaderFilePath)
{
    int success = false;
    char log[1024] = {};

    // Read the vertex shader source code from the file using the Slurp function.
    const auto vertexShaderSource = Slurp(vertexShaderFilePath);
    const char* vertexShaderSourcePtr = vertexShaderSource.c_str();

    // Create a vertex shader object using the glCreateShader function, and set its source code using the glShaderSource function.
    const auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourcePtr, nullptr);

    // Compile the vertex shader using the glCompileShader function, and check for errors using the glGetShaderiv and glGetShaderInfoLog functions.
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 1024, nullptr, log);
        spdlog::error(log);
        return false;
    }

    // Read the fragment shader source code from the file using the Slurp function.
    const auto fragmentShaderSource = Slurp(fragmentShaderFilePath);
    const char* fragmentShaderSourcePtr = fragmentShaderSource.c_str();

    // Create a fragment shader object using the glCreateShader function, and set its source code using the glShaderSource function.
    const auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourcePtr, nullptr);

    // Compile the fragment shader using the glCompileShader function, and check for errors using the glGetShaderiv and glGetShaderInfoLog functions.
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 1024, nullptr, log);
        spdlog::error(log);
        return false;
    }

    // Create a shader program object using the glCreateProgram function, and attach the vertex and fragment shaders using the glAttachShader function.
    _shaderProgram = glCreateProgram();
    glAttachShader(_shaderProgram, vertexShader);
    glAttachShader(_shaderProgram, fragmentShader);

    // Link the shader program using the glLinkProgram function, and check for errors using the glGetProgramiv and glGetProgramInfoLog functions.
    glLinkProgram(_shaderProgram);
    glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(_shaderProgram, 1024, nullptr, log);
        spdlog::error(log);
        return false;
    }

    // Delete the vertex and fragment shader objects using the glDeleteShader function.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void ProjectApplication::LoadModel(std::string_view file)
{
    // Read GLTF
    cgltf_options options = {};
    cgltf_data* model = nullptr;
    cgltf_parse_file(&options, file.data(), &model);
    cgltf_load_buffers(&options, model, file.data());

    fs::path path(file.data());
    const auto basePath = path.parent_path();
    std::unordered_map<std::string, size_t> textureIds;
    _cubes.Textures.reserve(model->materials_count);
    const uint32_t maxBatches = model->materials_count / 16 + 1;
    for (uint32_t i = 0; i < model->materials_count; ++i)
    {
        const auto& material = model->materials[i];
        if (material.has_pbr_metallic_roughness && material.pbr_metallic_roughness.base_color_texture.texture != nullptr)
        {
            const auto* image = material.pbr_metallic_roughness.base_color_texture.texture->image;
            const auto texturePath = FindTexturePath(basePath, image);
            if (textureIds.contains(texturePath))
            {
                continue;
            }
            uint32_t texture;
            glCreateTextures(GL_TEXTURE_2D, 1, &texture);

            glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            int32_t width = 0;
            int32_t height = 0;
            int32_t channels = STBI_rgb_alpha;
            const auto* textureData = stbi_load(texturePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
            const auto levels = (uint32_t)std::floor(std::log2(std::max(width, height)));
            glTextureStorage2D(texture, levels, GL_RGBA8, width, height);
            glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
            glGenerateTextureMipmap(texture);
            stbi_image_free((void*)textureData);
            _cubes.Textures.emplace_back(texture);
            textureIds[texturePath] = _cubes.Textures.size() - 1;
        }
    }

    uint32_t transformIndex = 0;
    size_t vertexOffset = 0;
    size_t indexOffset = 0;
    std::vector<MeshCreateInfo> meshCreateInfos;
    meshCreateInfos.reserve(1024);
    for (uint32_t i = 0; i < model->scene->nodes_count; ++i)
    {
        std::queue<cgltf_node*> nodes;
        nodes.push(model->scene->nodes[i]);
        while (!nodes.empty())
        {
            const auto* node = nodes.front();
            nodes.pop();
            if (!node->mesh)
            {
                for (uint32_t j = 0; j < node->children_count; ++j)
                {
                    nodes.push(node->children[j]);
                }
                continue;
            }
            for (uint32_t j = 0; j < node->mesh->primitives_count; ++j)
            {
                const auto& primitive = node->mesh->primitives[j];
                const glm::vec3* positionPtr = nullptr;
                const glm::vec3* normalPtr = nullptr;
                const glm::vec2* uvPtr = nullptr;
                const glm::vec4* tangentPtr = nullptr;
                uint64_t vertexCount = 0;
                // Vertices
                for (uint32_t k = 0; k < primitive.attributes_count; ++k)
                {
                    const auto& attribute = primitive.attributes[k];
                    const auto* accessor = attribute.data;
                    const auto* view = accessor->buffer_view;
                    const auto* dataPtr = (const char*)view->buffer->data;
                    switch (attribute.type)
                    {
                        case cgltf_attribute_type_position:
                            vertexCount = accessor->count;
                            positionPtr = (const glm::vec3*)(dataPtr + view->offset + accessor->offset);
                            break;

                        case cgltf_attribute_type_normal:
                            normalPtr = (const glm::vec3*)(dataPtr + view->offset + accessor->offset);
                            break;

                        case cgltf_attribute_type_texcoord:
                            uvPtr = (const glm::vec2*)(dataPtr + view->offset + accessor->offset);
                            break;

                        case cgltf_attribute_type_tangent:
                            tangentPtr = (const glm::vec4*)(dataPtr + view->offset + accessor->offset);
                            break;

                        default: break;
                    }
                }
                std::vector<Vertex> vertices;
                vertices.resize(vertexCount);
                {
                    auto* ptr = vertices.data();
                    for (uint32_t v = 0; v < vertexCount; ++v, ++ptr)
                    {
                        if (positionPtr)
                        {
                            std::memcpy(&ptr->Position, positionPtr + v, sizeof(glm::vec3));
                        }
                        if (normalPtr)
                        {
                            std::memcpy(&ptr->Normal, normalPtr + v, sizeof(glm::vec3));
                        }
                        if (uvPtr)
                        {
                            std::memcpy(&ptr->Uv, uvPtr + v, sizeof(glm::vec2));
                        }
                        if (tangentPtr)
                        {
                            std::memcpy(&ptr->Tangent, tangentPtr + v, sizeof(glm::vec4));
                        }
                    }
                }

                std::vector<uint32_t> indices;
                {
                    const auto* accessor = primitive.indices;
                    const auto* view = accessor->buffer_view;
                    const char* dataPtr = (const char*)view->buffer->data;
                    indices.reserve(accessor->count);
                    switch (accessor->component_type)
                    {
                        case cgltf_component_type_r_8:
                        case cgltf_component_type_r_8u:
                        {
                            const auto* ptr = (const uint8_t*)(dataPtr + view->offset + accessor->offset);
                            std::copy(ptr, ptr + accessor->count, std::back_inserter(indices));
                        } break;

                        case cgltf_component_type_r_16:
                        case cgltf_component_type_r_16u:
                        {
                            const auto* ptr = (const uint16_t*)(dataPtr + view->offset + accessor->offset);
                            std::copy(ptr, ptr + accessor->count, std::back_inserter(indices));
                        } break;

                        case cgltf_component_type_r_32f:
                        case cgltf_component_type_r_32u:
                        {
                            const auto* ptr = (const uint32_t*)(dataPtr + view->offset + accessor->offset);
                            std::copy(ptr, ptr + accessor->count, std::back_inserter(indices));
                        } break;

                        default: break;
                    }
                }
                std::string baseColorURI = "";
                if (primitive.material->pbr_metallic_roughness.base_color_texture.texture != nullptr)
                {
                    baseColorURI = FindTexturePath(basePath, primitive.material->pbr_metallic_roughness.base_color_texture.texture->image);
                }
                else
                {
                    baseColorURI = "";
                }
                const auto indexCount = indices.size();
                meshCreateInfos.emplace_back(MeshCreateInfo
                {
                    std::move(vertices),
                    std::move(indices),
                    transformIndex++,
                    baseColorURI == "" ? 0 :(uint32_t)textureIds[baseColorURI],
                    0,
                    vertexOffset,
                    indexOffset,
                    _cubes.VertexBuffer,
                    _cubes.IndexBuffer
                });
                cgltf_node_transform_world(node, glm::value_ptr(_cubes.Transforms.emplace_back()));
                vertexOffset += vertexCount * sizeof(Vertex);
                indexOffset += indexCount * sizeof(uint32_t);
            }
            for (uint32_t j = 0; j < node->children_count; ++j)
            {
                nodes.push(node->children[j]);
            }
        }
    }
    _cubes.Commands.resize(maxBatches);
    _cubes.ObjectData.resize(maxBatches);

    // Allocate GL buffers
    glCreateVertexArrays(1, &_cubes.InputLayout);
    glCreateBuffers(1, &_cubes.VertexBuffer);
    glCreateBuffers(1, &_cubes.IndexBuffer);
    glCreateBuffers(1, &_cubes.TransformData);
    glGenBuffers(_cubes.Commands.size(), _cubes.Commands.data());
    glCreateBuffers(_cubes.ObjectData.size(), _cubes.ObjectData.data());

    size_t vertexSize = 0;
    size_t indexSize = 0;
    for (const auto& meshCreateInfo : meshCreateInfos)
    {
        vertexSize += meshCreateInfo.Vertices.size() * sizeof(Vertex);
        indexSize += meshCreateInfo.Indices.size() * sizeof(uint32_t);
    }

    glNamedBufferStorage(_cubes.VertexBuffer, vertexSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(_cubes.IndexBuffer, indexSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayVertexBuffer(_cubes.InputLayout, 0, _cubes.VertexBuffer, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(_cubes.InputLayout, _cubes.IndexBuffer);

    glEnableVertexArrayAttrib(_cubes.InputLayout, 0);
    glEnableVertexArrayAttrib(_cubes.InputLayout, 1);
    glEnableVertexArrayAttrib(_cubes.InputLayout, 2);
    glEnableVertexArrayAttrib(_cubes.InputLayout, 3);

    glVertexArrayAttribFormat(_cubes.InputLayout, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position));
    glVertexArrayAttribFormat(_cubes.InputLayout, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Normal));
    glVertexArrayAttribFormat(_cubes.InputLayout, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, Uv));
    glVertexArrayAttribFormat(_cubes.InputLayout, 3, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, Tangent));

    glVertexArrayAttribBinding(_cubes.InputLayout, 0, 0);
    glVertexArrayAttribBinding(_cubes.InputLayout, 1, 0);
    glVertexArrayAttribBinding(_cubes.InputLayout, 2, 0);
    glVertexArrayAttribBinding(_cubes.InputLayout, 3, 0);

    for (auto& info : meshCreateInfos)
    {
        info.VertexBuffer = _cubes.VertexBuffer;
        info.IndexBuffer = _cubes.IndexBuffer;
        glNamedBufferSubData(
            info.VertexBuffer,
            info.VertexOffset,
            info.Vertices.size() * sizeof(Vertex),
            info.Vertices.data());
        glNamedBufferSubData(
            info.IndexBuffer,
            info.IndexOffset,
            info.Indices.size() * sizeof(uint32_t),
            info.Indices.data());
        _cubes.Meshes.emplace_back(Mesh
        {
            (uint32_t)info.Indices.size(),
            (int32_t)(info.VertexOffset / sizeof(Vertex)),
            (uint32_t)(info.IndexOffset / sizeof(uint32_t)),
            info.TransformIndex,
            info.BaseColorTexture,
            info.NormalTexture
        });
    }
}
