// window.cc
// by Reuel Nkomo 2025
#include "window.hh"
#include "types.hh"
#include "utils.hh"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define HAS_GL
#include <glad/glad.h>

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

// -------------------------------------------------------------------------------------------------
// Helper functions
// -------------------------------------------------------------------------------------------------
static void frame_buffer_resize_callback(GLFWwindow* window, int width, int height) {
    GL_QUERY_ERROR(glViewport(0, 0, width, height);)
}

static void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

static u32 build_shader(u32 type, const std::string& path) {
    std::string source = utils::load_text_from_file(path);
    if (source == "") {
        std::cerr << "Error::GL: received an empty shader\n";
        return 0;
    }

    u32 shader = glCreateShader(type);
    const char* raw_source = source.c_str();
    GL_QUERY_ERROR(glShaderSource(shader, 1, &raw_source, NULL);)
    GL_QUERY_ERROR(glCompileShader(shader);)

    i32 shader_compiled;
    GL_QUERY_ERROR(glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_compiled);)
    if (!shader_compiled) {
        char message[512];
        GL_QUERY_ERROR(glGetShaderInfoLog(shader, 512, NULL, message);)
        std::cerr << "Error::Shader::" << ((type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment") << ":\n" << 
            message << "\n";
    }
    return shader;
}

static u32 create_shader(const std::string& vpath, const std::string& fpath) {
    u32 vertex = build_shader(GL_VERTEX_SHADER, vpath);
    u32 fragment = build_shader(GL_FRAGMENT_SHADER, fpath);
    GL_QUERY_ERROR(u32 shader = glCreateProgram();)

    GL_QUERY_ERROR(glAttachShader(shader, vertex);)
    GL_QUERY_ERROR(glAttachShader(shader, fragment);)
    GL_QUERY_ERROR(glLinkProgram(shader);)

    i32 shader_linked;
    GL_QUERY_ERROR(glGetProgramiv(shader, GL_LINK_STATUS, &shader_linked);)
    if (!shader_linked) {
        char message[512];
        GL_QUERY_ERROR(glGetProgramInfoLog(shader, 512, NULL, message);)
        std::cerr << "Error::Shader::Link:\n" << 
            message << "\n";
    }

    GL_QUERY_ERROR(glDeleteShader(vertex);)
    GL_QUERY_ERROR(glDeleteShader(fragment);)
    return shader;
}

static u32 load_image_to_gpu(utils::ImageData image, u32 shader, u32 texture_unit, const char* uniform) {
    u32 id;
    GL_QUERY_ERROR(glGenTextures(1, &id);)
    GL_QUERY_ERROR(glBindTexture(GL_TEXTURE_2D, id);)

    GL_QUERY_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);)
    GL_QUERY_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);)
    GL_QUERY_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);)
    GL_QUERY_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);)

    u32 internal_format = (image.channel_count == 3) ? GL_RGB8 : GL_RGBA8;

    if (image.data != nullptr) {
        GL_QUERY_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)image.data);)
        GL_QUERY_ERROR(glGenerateMipmap(GL_TEXTURE_2D);)
    }

    GL_QUERY_ERROR(glActiveTexture(GL_TEXTURE0 + texture_unit);)
    GL_QUERY_ERROR(glBindTexture(GL_TEXTURE_2D, id);)

    // Set uniform in shader
    GL_QUERY_ERROR(glUseProgram(shader);)
    GL_QUERY_ERROR(u32 sampler_location = glGetUniformLocation(shader, uniform);)
    GL_QUERY_ERROR(glUniform1i(sampler_location, texture_unit);)
    GL_QUERY_ERROR(glUseProgram(0);)
    return id;
}

static void pause_thread(f64 s) {
    std::this_thread::sleep_for(std::chrono::duration<f64>(s));
}

static void setup_opengl_draw_surface(u32* vao, u32* vbo, u32* ebo) {
    const f32 vertices[] = {
        -1.0, -1.0, 0.0, 0.0,   // bottom left
         1.0, -1.0, 1.0, 0.0,   // bottom right
         1.0,  1.0, 1.0, 1.0, // top right
        -1.0, 1.0, 0.0, 1.0 // top left
    };

    const u32 indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GL_QUERY_ERROR(glGenVertexArrays(1, vao);)
    GL_QUERY_ERROR(glBindVertexArray(*vao);)

    GL_QUERY_ERROR(glGenBuffers(1, vbo);)
    GL_QUERY_ERROR(glBindBuffer(GL_ARRAY_BUFFER, *vbo);)
    GL_QUERY_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);)
    GL_QUERY_ERROR(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)0);)
    GL_QUERY_ERROR(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)(2*sizeof(f32)));)
    GL_QUERY_ERROR(glEnableVertexAttribArray(0);)
    GL_QUERY_ERROR(glEnableVertexAttribArray(1);)
    GL_QUERY_ERROR(glGenBuffers(1, ebo);)
    GL_QUERY_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);)
    GL_QUERY_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);)

    GL_QUERY_ERROR(glBindVertexArray(0);)
    GL_QUERY_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0);)
    GL_QUERY_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);)
}

static DECL_WINDOW_RENDER_CALLBACK(default_window_render_callback) {
    return {};
}

// -------------------------------------------------------------------------------------------------
// class Window implementation
// -------------------------------------------------------------------------------------------------
Window::Window(
    __window_render_callback_type__* render_callback, 
    f32 width, f32 aspect_ratio, const std::string& name
) 
    : m_width{width}, m_aspect_ratio{aspect_ratio}, m_name{name},
      m_render_callback{render_callback}
{
    if (render_callback == nullptr) {
        std::cerr << "Warning: window not provided with valid callback!\n";
        m_render_callback = default_window_render_callback;
    } else {
        m_render_callback = render_callback;
    }
}

bool Window::create_opengl_window() {
    if (!glfwInit()) {
        std::cerr << "Error::GLFW: could not initialize glfw!\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif 
    glfwWindowHint(GLFW_RESIZABLE, DEFAULT_WINDOW_RESIZE);

    f32 height = m_width / m_aspect_ratio;
    GLFWwindow* window = glfwCreateWindow(
        static_cast<i32>(m_width), 
        static_cast<i32>(height), 
        m_name.c_str(), 
        NULL, NULL);
    
    if (window == nullptr) {
        std::cerr << "Error::GLFW: could not create a window!\n";
        m_window_handle = nullptr;
        glfwTerminate();
        return false;
    }   
    m_window_handle = (void*) window;
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cerr << "Error:GLAD: could not load OpenGL function pointers, defaulting to glfw\n";
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cerr << "Error::GLAD: could not load glfw loader!\n";
            glfwDestroyWindow(window);
            glfwTerminate();
            m_window_handle = nullptr;
            return false;
        }
    }

    glfwSetWindowSizeCallback(window, frame_buffer_resize_callback);
    return true;
}

void Window::launch_window_loop() {
    if (!m_window_handle) {
        std::cerr << "Error: cannot launch a window with failed initialization\n";
        return;
    }
    // -----------------------------
    // Bind vertex data for canvas
    // -----------------------------
    setup_opengl_draw_surface(&m_vao, &m_vbo, &m_ebo);

    // -----------------------------
    // Create a shader
    // -----------------------------
    m_shader = create_shader("../resources/window.vertex", "../resources/window.fragment");
    
    // -----------------------------
    // Render loop
    // -----------------------------
    f64 last_time = glfwGetTime();
    GLFWwindow* window = (GLFWwindow*) m_window_handle;
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        process_input(window);

        // ------------------------------------------------
        // Request image from owner else load default image
        // ------------------------------------------------
        m_surface = m_render_callback();
        if (m_surface.data == nullptr) {
            m_surface = utils::load_image_from_file("../resources/sample.ppm");
        } 
        m_texture_id = load_image_to_gpu(m_surface, m_shader, 1, "canvas");
        free_image_data(&m_surface);

        GL_QUERY_ERROR(glClearColor(0.0, 0.0, 0.0, 1.0);)
        GL_QUERY_ERROR(glClear(GL_COLOR_BUFFER_BIT);)

        GL_QUERY_ERROR(glUseProgram(m_shader);)
        GL_QUERY_ERROR(glBindVertexArray(m_vao);)
        GL_QUERY_ERROR(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);)

        glfwSwapBuffers(window);
        GL_QUERY_ERROR(glDeleteTextures(1, &m_texture_id);)

        // -----------------------------
        // Enforce 30 fps
        // -----------------------------
        /// TODO: maybe come back and use better timing (maybe)
        const f64 new_time = glfwGetTime(); // Time in seconds
        const f64 frame_time = new_time-last_time;
        const f64 target_frame_time = 1.0 / 30.0; // ~0.0333 secs
        
        if (frame_time < target_frame_time) {
            f64 sleep_duration = target_frame_time - frame_time;
            pause_thread(sleep_duration);
        }
        last_time = glfwGetTime();
    }
    GL_QUERY_ERROR(glDeleteProgram(m_shader);)
}

Window::~Window() {
    if (m_window_handle != nullptr) {
        glfwDestroyWindow((GLFWwindow*) m_window_handle);
        glfwTerminate();
    }
}
// -------------------------------------------------------------------------------------------------
// class Window end
// -------------------------------------------------------------------------------------------------