#include "UIContainer.hpp"
#include "Menu.hpp"

using namespace ui;
using utils::non_owning_ptr;

UIContainer::UIContainer(std::unique_ptr<Title>&& title,
	std::unique_ptr<Menu>&& menu,
	std::unique_ptr<ConsoleLog>&& log,
	std::unique_ptr<Helper>&& helper
)
	: title{ std::move(title) },
	menu{ std::move(menu) },
	log{ std::move(log) },
	helper{ std::move(helper) }
{
	VTSizeType height, width;
	std::tie(height, width) = getViewportSize();

	this->title->alignToCenter(width);
	this->menu->setPosition(3, 0);
	this->log->setPosition(height - 4, 0);
	this->helper->setPosition(height - 2, 0);
}

Title& UIContainer::getTitle()
{
	return *(title.get());
}

Menu& UIContainer::getMenu()
{
	return *(menu.get());
}

ConsoleLog& UIContainer::getLog()
{
	return *(log.get());
}

Helper& UIContainer::getHelper()
{
	return *(helper.get());
}

std::unique_ptr<UIContainer> UIContainer::createSimple(std::string&& titleLabel, std::unique_ptr<Menu>&& menu)
{
	return std::make_unique<UIContainer>(
		std::make_unique<Title>(std::move(titleLabel)),
		std::move(menu),
		std::make_unique<ConsoleLog>(),
		std::make_unique<Helper>()
	);				
}

void UIContainer::print()
{
	title->print();
	menu->print();
	log->print();
	helper->print();
}