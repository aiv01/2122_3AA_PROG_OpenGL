#include <string>

typedef struct GLFWwindow GLFWwindow;

class Window
{    
public:
    Window(int InWidth, int InHeight, const std::string& InTitle);
    ~Window();

    void SetTitle(const std::string& InTitle);
    bool IsOpened() const;
    float GetDeltaTime() const;
    void Update();
    void PrintInfo() const;

private:
    int Width;
    int Height;
    std::string Title;
    GLFWwindow* RawWindow;
    float DeltaTime;
	float LastTime;
};
