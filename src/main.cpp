#include "ftxui/component/checkbox.hpp"

#include "ftxui/component/component.hpp"
#include "ftxui/component/container.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

class UIComponet : public Component {
public:
    Element Render() override
    {
        // clang-format off
        return  window(text(L"Just Fast") | center,
		    hbox(
			)
		);
        // clang-format on
    }
};

int main(int argc, const char* argv[])
{
    auto screen = ScreenInteractive::Fullscreen();
    UIComponet component;
    screen.Loop(&component);
    return 0;
}
