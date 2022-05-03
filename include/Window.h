#include <string>

typedef struct GLFWwindow GLFWwindow;

class Window
{
private:
    int Width;
    int Height;
    std::string Title;
    GLFWwindow* RawWindow;
    float DeltaTime;
	float LastTime;
    
public:
    Window(int InWidth, int InHeight, const std::string& InTitle);
    ~Window();

    bool IsOpened();
    void Update();
    void SetTitle(const std::string& InTitle);
    float GetDeltaTime();
};
