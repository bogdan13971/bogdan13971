#include <ConsoleUI/ConsoleUI.hpp>
#include <ConsoleUI/ConsoleCommands.hpp>
#include <ConsoleUI/Component.hpp>
#include <sstream>
#include <conio.h>

using namespace ui;

int main()
{
	using namespace ui;

	std::cout << ALTERNATE_BUFFER;

	int width, height;
	std::tie(height, width) = getViewportSize();

	Title title("Cel mai smecher titlu");
	title.setPosition(0, width / 2 - 11);

	//title.print();

	Helper helper;
	helper.setPosition(height - 2, 0);
	helper.addHelper("<-", "left");
	helper.addHelper("->", "right");

	//helper.print();

	int counter = 1;
	bool isDirty = false;
	ConsoleUI ui;

	auto eventListener = std::make_unique<EventListener>();
	eventListener->registerEventHandler(EVENT_TYPE::START, [&ui]() {ui.log("hello from start"); });
	eventListener->registerEventHandler(EVENT_TYPE::EXIT, [&ui]() {ui.log("exited"); });
	eventListener->registerEventHandler(EVENT_TYPE::UP, [&ui]() {ui.log("up pressed"); });
	eventListener->registerEventHandler(EVENT_TYPE::DOWN, [&ui]() {ui.log("down pressed"); });
				 
	eventListener->registerKeyPressEventHandler('j', [&ui]() {ui.log("j key pressed"); });
	eventListener->registerKeyPressEventHandler('i', [&counter, &isDirty]() {counter++; isDirty = true; });

	auto menu = std::make_unique<Menu>();
	auto it1 = menu->createItem("Item 1");
	it1->setExecCallback([&ui]() {ui.log("Item 1 was pressed"); });
	auto sub1 = menu->createSubmenu("Sub 1");
	sub1->setExecCallback([&ui]() {ui.log("Entered submenu 1"); });
	auto it2 = menu->createItem("Item 2");
	it2->setExecCallback([&ui]() {ui.log("Item 2 was pressed"); });
	auto it3 = menu->createItem("Item 3");
	it3->setExecCallback([&ui]() {ui.log("Item 3 was pressed"); });

	auto sub1it1 = sub1->createItem("Item 1 in Sub 1");
	sub1it1->setExecCallback([&ui]() {ui.log("Item 1 in submenu 1"); });
	auto sub1it2 = sub1->createItem("Maciuca");
	sub1it2->setExecCallback([&ui]() {ui.log("Vai ce madular"); });
	auto sub1it3 = sub1->createSubmenu("Madular");
	sub1it3->setExecCallback([&ui]() {ui.log("Doamna mirabela"); });
	sub1it3->setBackCallback([&ui]() {ui.log("Out of mirabela"); });
	auto subit3sub1 = sub1it3->createItem("Ce gagica exploziva");
	subit3sub1->setExecCallback([&ui]() {ui.log("Ati explodad in amor"); });

	auto subit3sub2 = sub1it3->createItem("E facuta pe masura mea");
	subit3sub2->setExecCallback([&ui]() {ui.log("E frumoasa nebuna"); });
	
	auto dyn1 = menu->createItem("Dyn 1");
	dyn1->setUpdateCallback([&counter]() { 
		return std::to_string(counter); 
	});

	auto& a = sub1->addItem("Another item")
		.addItem("Yet another one")
		.addItem("And anoterh one")
		.createSubmenu("hello")->addItem("myes");

	menu->setPosition(3, 0);

	ui.title = &title;
	ui.helper = &helper;

	ui.setMenu(std::move(menu));
	ui.setEventListener(std::move(eventListener));

	ui.setUpdateCallback([&]() 
	{
		if (isDirty)
		{
			dyn1->update();
			isDirty = false;
		}
	});

	isDirty = true;

	ui.start();

	return 0;
}