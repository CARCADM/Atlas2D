#include <renderer.h>
#include <shader.h>

// This method should do all this shit pressCheck does. Fuck you GLFW.
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(keyBuffer.empty())
    {
        keyBuffer.emplace_back(key, 0);
        return;
    }

    for (std::pair<int, int> &iKey : keyBuffer)
    {
        if(iKey.first == key)
        {
            if(action == 0)
            {
                iKey.second = -1;
                return;
            }
            return;
        }
    }
    keyBuffer.emplace_back(key, 0);
}

// This method shouldn't have to exist... but it does.
// This method checks for keys that are pressed... if they are there they are now being held.
void pressCheck()
{
    int i = 0;
    for (std::pair<int, int> &key : keyBuffer)
    {
        if(key.second == 0)
        {
            key.second = 1;
        }
        else if (key.second == -1)
        {
            keyBuffer.erase(keyBuffer.begin() + i);
            return;
        }
        ++i;
    }

}

// This method is probably useless (it's not)
void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glfwSetWindowAspectRatio(window, 16, 9);
    context::getInstance().setWidth(width);
    context::getInstance().setHeight(height);
    oCamera.recalculateProjectionMatrix();
}

// Run wake functions and start render loop
void renderer::startRenderLoop(rendererContext customRendererContext)
{
    mainRenderContext = customRendererContext;
    startRenderLoop();
}

void renderer::startRenderLoop()
{
    // This is the spine of the entire fucking engine, don't change stuff in here unless you 100% know what you are doing.
    glfwInit();

    // Window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, mainRenderContext.isResizeable);

    gameWindow = glfwCreateWindow(mainRenderContext.windowWidth, mainRenderContext.windowHeight, mainRenderContext.projectName, NULL, NULL);
    if(gameWindow == NULL)
    {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(gameWindow);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD" << std::endl;
    }

    glViewport(0, 0, mainRenderContext.windowWidth, mainRenderContext.windowHeight);
    glfwSetFramebufferSizeCallback(gameWindow, frameBufferSizeCallback);
    glfwSetKeyCallback(gameWindow, keyCallback);

    // Allow the alpha channel (Transparency)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader baseShader(
        "../src/shaders/baseVertexShader.vs", 
        "../src/shaders/baseFragmentShader.fs"
    );
    baseShaderID = baseShader.ID;

    gameScript::runWakeFunctions();
    deltaPrevious = glfwGetTime();

    while (!glfwWindowShouldClose(gameWindow))
    {
        if (glfwGetTime() - deltaPrevious >= minFrameTime)
        {
            glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            context::deltaTime = glfwGetTime() - deltaPrevious;
            oCamera.applyProjection(baseShaderID);
            gameScript::runUpdateFunctionsOnce(glfwGetTime() - deltaPrevious);
            deltaPrevious = glfwGetTime();

            pressCheck(); // Allows the code to quickly set a key pressed to repeated since GLFW is a bitch and won't do it for us.
            
            if (keyBuffer.size() > 50)
            {
                keyBuffer.clear();
                std::cout << "WARNING KEY BUFFER OVERFLOW" << std::endl;
            }


            glfwSwapBuffers(gameWindow);
            glfwPollEvents();
        }
    }
    glfwTerminate();
}

// Runs once
bool renderer::getKeyPressed(int keyCode)
{
    for(std::pair<int, int> &key : keyBuffer) 
    { 
        if(key.first == keyCode && key.second == 0) { return true; } 
    }
    return false;
}
// Runs when the user finally lets go of the key
bool renderer::getKeyReleased(int keyCode)
{
    for(std::pair<int, int> &key : keyBuffer) 
    { 
        if(key.first == keyCode && key.second == -1) { return true; } 
    }
    return false;
}
// Runs a lot of times
bool renderer::getKeyHeld(int keyCode)
{
    for(std::pair<int, int> &key : keyBuffer) 
    { 
        if(key.first == keyCode && (key.second == 0 || key.second == 1)) { return true; } 
    }
    return false;
}

unsigned int renderer::getBaseShader()
{
    return baseShaderID;
}

void renderer::assignBaseShader(unsigned int newBaseShader)
{
    baseShaderID = newBaseShader;
}