
#include"Config.hpp"

void Main()
{
	const Font font(20);
	ScalableWindow::Setup(600, 480);
	Window::SetTitle(L"Filter Editer");	

	std::shared_ptr<Config>config = std::make_shared<Config>();

	while (System::Update()){
		
		config->update();

		
	}
}
